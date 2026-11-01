/*
* Name		    : agent.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : [xbilko03] Source file for a separate process which handles reading shared memory and sending that information to UI
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
/* AI-Assisted: Claude Opus 5.5: syntax, entire file */
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <vector>
#include "../../transport/include/vkdbgFormat.hpp"

#pragma comment(lib, "ws2_32.lib")

namespace vkdebug::agent {
    /* [xbilko03] pointer to a function that takes the target's program's handle, functions are used to control that program's runtime (suspendProcess, resumeProcess) */
    typedef LONG (NTAPI*NtProcessFunction)(HANDLE);

    /* [xbilko03] view of the target's program's shared memory */
    static uint8_t *globalShared = nullptr;
    /* [xbilko03] socket of the UI */
    static SOCKET globalUi = INVALID_SOCKET;

    static vkdebug::sharedHeader *headerPage() {
        return (vkdebug::sharedHeader *) (globalShared);
    }

    static vkdebug::chunkHeader *chunkAt(uint32_t index) {
        return (vkdebug::chunkHeader *) (globalShared + VKDBG_HEADER_PAGE_SIZE + size_t(index) * VKDBG_CHUNK_SIZE);
    }

    static uint8_t *payloadOf(vkdebug::chunkHeader *chunk) {
        return (uint8_t *) (chunk) + sizeof(vkdebug::chunkHeader);
    }

    /* [xbilko03] per-thread order of chunks
     * threadId + creationTime = thread identification
     * nextSequence = number of the chunk which is sent next by the agent for this thread (others wait until their turn)
     * (e.g. thread A writes into chunks 1 and 2, agent always sends 1 before 2)
     */
    struct threadStream {
        uint32_t threadId;
        uint64_t creationTime;
        uint32_t nextSequence;
    };

    /* [xbilko03] globalStreams = agent's notebook:
     * row 1: thread 4120 (created 10:00:01) | next chunk to send: 3
     * row 2: thread 7788 (created 10:00:05) | next chunk to send: 0
     */
    static std::vector<threadStream> globalStreams;

    /* [xbilko03] find a stream of a thread, add a new one when a new thread is seen */
    static threadStream *streamOf(uint32_t threadId, uint64_t creationTime) {
        for (size_t i = 0; i < globalStreams.size(); i++) {
            if (globalStreams[i].threadId == threadId && globalStreams[i].creationTime == creationTime)
                return &globalStreams[i];
        }
        threadStream stream;
        stream.threadId = threadId;
        stream.creationTime = creationTime;
        stream.nextSequence = 0;
        globalStreams.push_back(stream);
        return &globalStreams.back();
    }

    static bool sendAll(const void *data, size_t size) {
        const char *bytes = (const char *) (data);
        while (size > 0) {
            /* [xbilko03] limit of one send(...) is 1GB but message can be much bigger */
            size_t part = size;
            if (part > 0x40000000)
                part = 0x40000000;
            int sent = send(globalUi, bytes, (int) (part), 0);
            if (sent == SOCKET_ERROR)
                return false;
            bytes += sent;
            size -= (size_t) (sent);
        }
        return true;
    }

    /* [xbilko03] result of drainOnce()
     * DrainNothing = nothing new
     * DrainMoved = something sent
     * DrainUiGone = lost connection to UI
     */
    enum drain { DrainNothing, DrainMoved, DrainUiGone };

    /* [xbilko03] send each thread's next finished chunk to UI and free it, when appDead == true (target application has crashed), then it also sends unfinished chunks of data */
    static drain drainOnce(bool appDead) {
        bool moved = false;
        for (uint32_t i = 0; i < VKDBG_CHUNK_COUNT; i++) {
            vkdebug::chunkHeader *chunk = chunkAt(i);
            /* [xbilko03] read state of a chunk in shared memory: free | claimed | writing | complete */
            uint32_t state = vkdebug::sharedMemoryRead(&chunk->state);
            /* [xbilko03] skip incomplete chunks unless target app is dead */
            if (state != vkdebug::ChunkComplete && !(appDead && state == vkdebug::ChunkWriting))
                continue;
            /* [xbilko03] skip chunks that are not next to their thread's order (to preserve order of chunks being sent) */
            threadStream *stream = streamOf(chunk->threadId, chunk->threadCreationTime);
            if (chunk->sequence != stream->nextSequence)
                continue;

            uint32_t used = vkdebug::sharedMemoryRead(&chunk->used);
            if (used > 0) {
                vkdebug::resultChunkHeader segment;
                segment.threadId = chunk->threadId;
                segment.threadCreationTime = chunk->threadCreationTime;
                segment.length = used;
                segment.flags = chunk->flags;
                /* [xbilko03] send complete chunks's used bytes, failing send means UI is probably closed, function returns that result */
                if (!sendAll(&segment, sizeof(segment)) || !sendAll(payloadOf(chunk), used))
                    return DrainUiGone;
            }
            /* [xbilko03] mark sent chunk as free */
            vkdebug::sharedMemoryWrite(&chunk->state, vkdebug::ChunkFree);
            /* [xbilko03] allow next chunk of this thread to be sent */
            stream->nextSequence++;
            moved = true;
        }
        if (moved)
            return DrainMoved;
        return DrainNothing;
    }

    static bool appExited(HANDLE app) {
        return WaitForSingleObject(app, 0) == WAIT_OBJECT_0;
    }

    /* [xbilko03] get pid of target app, open its shared memory and wait for the UI to connect, then send data while both processes live, if target app dies, also send whatever it didn't finish writing */
    int agentMain(int argc, char **argv) {
        if (argc < 2)
            return 1;
        /* [xbilko03] pid of target app */
        DWORD pid = strtoul(argv[1], nullptr, 10);

        /* [xbilko03] shared memory name is derived from pid */
        wchar_t name[64];
        swprintf(name, 64, VKDBG_SHARED_NAME_FORMAT, (unsigned long) (pid));

        /* [xbilko03] find mapping the layer created */
        HANDLE mapping = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, name);
        if (!mapping)
            return 1;

        /* [xbilko03] make mapping visible in agent's memory */
        uint64_t total = uint64_t(VKDBG_HEADER_PAGE_SIZE) + VKDBG_SHARED_MEMORY_SIZE;
        globalShared = (uint8_t *) (MapViewOfFile(mapping, FILE_MAP_ALL_ACCESS, 0, 0, (SIZE_T) (total)));
        if (!globalShared)
            return 1;

        /* [xbilko03] open the target app with specific rights */
        HANDLE app = OpenProcess(PROCESS_SUSPEND_RESUME | PROCESS_TERMINATE | SYNCHRONIZE, FALSE, pid);
        if (!app)
            return 1;

        /* [xbilko03] get two functions for controlling target app's runtime */
        HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
        NtProcessFunction suspendProcess = (NtProcessFunction) (GetProcAddress(ntdll, "NtSuspendProcess"));
        NtProcessFunction resumeProcess = (NtProcessFunction) (GetProcAddress(ntdll, "NtResumeProcess"));
        if (!suspendProcess || !resumeProcess)
            return 1;

        /* [xbilko03] start winsock */
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
            return 1;

        /* [xbilko03] create TCP socket, windows chooses free port */
        SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        address.sin_port = 0;
        if (listener == INVALID_SOCKET || bind(listener, (sockaddr *) (&address), sizeof(address)) == SOCKET_ERROR ||
            listen(listener, 1) == SOCKET_ERROR)
            return 1;

        /* [xbilko03] obtain port and write it to shared (agent|layer) header, layer sends the port to the UI */
        int addressLength = sizeof(address);
        getsockname(listener, (sockaddr *) (&address), &addressLength);
        vkdebug::sharedMemoryWrite(&headerPage()->agentPort, ntohs(address.sin_port));

        /* [xbilko03] wait until UI connects */
        while (globalUi == INVALID_SOCKET) {
            if (appExited(app))
                return 0;
            fd_set readSet;
            FD_ZERO(&readSet);
            FD_SET(listener, &readSet);
            timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = 100 * 1000;
            if (select(0, &readSet, nullptr, nullptr, &timeout) > 0)
                globalUi = accept(listener, nullptr, nullptr);
        }
        closesocket(listener);

        /* [xbilko03] tell layer the agent is successfully reading shared memory (otherwise no messages are written) */
        vkdebug::sharedMemoryWrite(&headerPage()->readerAlive, 1);

        while (true) {
            drain drained = drainOnce(false);
            /* [xbilko03] if UI is dead, nobody reads so there is no point in writing, kill the target app, break the loop to clean up and exit the agent */
            if (drained == DrainUiGone) {
                vkdebug::sharedMemoryWrite(&headerPage()->readerAlive, 0);
                TerminateProcess(app, 1);
                break;
            }

            /* [xbilko03] layer found no free chunk */
            if (vkdebug::sharedMemoryRead(&headerPage()->full) != 0) {
                /* [xbilko03] agent pauses the app, drains everything, clears the full flag, resumes operation */
                suspendProcess(app);
                drain result = drainOnce(false);
                while (result == DrainMoved)
                    result = drainOnce(false);
                vkdebug::sharedMemoryWrite(&headerPage()->full, 0);
                resumeProcess(app);
                if (result == DrainUiGone) {
                    vkdebug::sharedMemoryWrite(&headerPage()->readerAlive, 0);
                    TerminateProcess(app, 1);
                    break;
                }
                continue;
            }

            if (appExited(app)) {
                /* [xbilko03] target app crashed|closed, send what is left (even unfinished chunks) and exit cycle */
                while (drainOnce(true) == DrainMoved);
                break;
            }
            if (drained == DrainNothing)
                Sleep(1);
        }

        /* [xbilko03] cleanup */
        closesocket(globalUi);
        WSACleanup();
        UnmapViewOfFile(globalShared);
        CloseHandle(mapping);
        CloseHandle(app);
        return 0;
    }
}

/* [xbilko03] run the agent */
int main(int argc, char **argv) {
    return vkdebug::agent::agentMain(argc, argv);
}

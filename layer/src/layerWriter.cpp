/*
* Name		    : layerWriter.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : [xbilko03] Source file of the layer writing into shared memory
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
/* AI-Assisted: Claude Opus 5.5: syntax, entire file */
#include "../include/layerWriter.hpp"
#include <windows.h>
#include <atomic>
#include <cwchar>

namespace vkdebug::layer {
    /* [xbilko03] hierarchy:
    * shared memory
        * header page (4 KB)
        * 1024 chunks (32 KB each)
            * chunk = chunkHeader + payload = (32 KB)
            * payload = fragments
                * fragment = 4 B length + piece of message
                    * message = messageHeader (12 B) + content

    * shared memory = RAM shared by two processes, layer writes, agent reads
    * header page = is agent alive? + where threads should look for a free chunk (nextChunk)
    * chunk = a box that one thread fills with data, agent reads it, sends it, flag it as free chunk again
    * chunkHeader = state of a chunk: free to claim | claimed by a thread | ready to be sent + thread that claimed the chunk + other information about writing into shared memory
    * payload = stored data about parts of one or more messages
    * fragment = part of one message
    * message = one piece of information about vulkan's call including corresponding data
    */
    static std::atomic<uint8_t *> globalShared{nullptr};
    static HANDLE globalMapping = NULL;

    /* [xbilko03] tracks current call number per thread */
    static thread_local uint64_t threadCall = 0;
    /* [xbilko03] current open chunk when writing */
    static thread_local vkdebug::chunkHeader *threadChunk = nullptr;
    /* [xbilko03] position in current open chunk */
    static thread_local uint32_t threadPos = 0;
    /* [xbilko03] tracks position of where current fragment starts in the scope of a chunk */
    static thread_local uint32_t threadFragmentAt = 0;
    /* [xbilko03] chunk order number */
    static thread_local uint32_t threadSequence = 0;
    /* [xbilko03] guard against nested writing */
    static thread_local bool threadWriting = false;

    /* [xbilko03] proper identification of a thread is a pair of threadId and thread creation time */
    static thread_local bool threadKeyReady = false;
    static thread_local uint32_t threadOwnId = 0;
    static thread_local uint64_t threadOwnCreationTime = 0;

    static vkdebug::sharedHeader *headerPage(uint8_t *shared) {
        return (vkdebug::sharedHeader *) (shared);
    }

    static vkdebug::chunkHeader *chunkAt(uint8_t *shared, uint32_t index) {
        return (vkdebug::chunkHeader *) (shared + VKDBG_HEADER_PAGE_SIZE + size_t(index) * VKDBG_CHUNK_SIZE);
    }

    static uint8_t *payloadOf(vkdebug::chunkHeader *chunk) {
        return (uint8_t *) (chunk) + sizeof(vkdebug::chunkHeader);
    }

    /* [xbilko03] return pointer to shared memory when agent is reading, otherwise return nullptr */
    static uint8_t *sharedIfRead() {
        uint8_t *shared = globalShared.load();
        if (!shared || vkdebug::sharedMemoryRead(&headerPage(shared)->readerAlive) == 0)
            return nullptr;
        return shared;
    }

    /* [xbilko03] threadId + thread create time = unique thread ID (OS may re-use threadId, so this pair makes it safer) */
    static void threadKey() {
        if (threadKeyReady)
            return;
        threadOwnId = GetCurrentThreadId();
        FILETIME creation{}, exitTime{}, kernelTime{}, userTime{};
        if (GetThreadTimes(GetCurrentThread(), &creation, &exitTime, &kernelTime, &userTime))
            threadOwnCreationTime = ((uint64_t) (creation.dwHighDateTime) << 32) | creation.dwLowDateTime;
        threadKeyReady = true;
    }

    /* [xbilko03] exposes written data to the agent so it can read it, order of writing is guaranteed:
     * 1. thread writes payload
     * 2. publish writes into variable used (i.e. how many bytes are ready) using atomic_ref
     * 3. operation is done and now agent can read the bytes that are ready
     * */
    static void publish() {
        vkdebug::sharedMemoryWrite(&threadChunk->used, threadPos);
    }

    /* [xbilko03] reserve space for fragment header */
    static void beginFragment() {
        threadFragmentAt = threadPos;
        threadPos += VKDBG_FRAGMENT_HEADER_SIZE;
    }

    /* [xbilko03] write final length of current fragment */
    static void endFragment() {
        uint32_t size = threadPos - threadFragmentAt - VKDBG_FRAGMENT_HEADER_SIZE;
        memcpy(payloadOf(threadChunk) + threadFragmentAt, &size, sizeof(size));
    }

    /* [xbilko03] chunk cannot hold more messages, flag it as complete, if chunk is full like that in middle of a message however, mark it as lastContinues = rest of this message goes into another chunk */
    static void closeChunk(bool lastContinues) {
        if (!threadChunk)
            return;
        if (lastContinues)
            threadChunk->flags |= vkdebug::ChunkLastContinues;
        publish();
        vkdebug::sharedMemoryWrite(&threadChunk->state, vkdebug::ChunkComplete);
        threadChunk = nullptr;
        threadPos = 0;
    }

    /* [xbilko03] thread attempts to claim a chunk, if none are free, wait for the agent */
    static bool openChunk(uint32_t flags) {
        threadKey();
        LARGE_INTEGER frequency{}, last{};
        QueryPerformanceFrequency(&frequency);
        LONGLONG waited = 0;
        while (true) {
            uint8_t *shared = sharedIfRead();
            if (!shared)
                return false;
            vkdebug::sharedHeader *header = headerPage(shared);
            uint32_t start = vkdebug::sharedMemoryAdd(&header->nextChunk, 1);
            for (uint32_t i = 0; i < VKDBG_CHUNK_COUNT; i++) {
                vkdebug::chunkHeader *chunk = chunkAt(shared, (start + i) % VKDBG_CHUNK_COUNT);
                if (vkdebug::sharedMemoryAttemptClaim(&chunk->state, vkdebug::ChunkFree, vkdebug::ChunkClaimed)) {
                    chunk->flags = flags;
                    chunk->threadId = threadOwnId;
                    chunk->threadCreationTime = threadOwnCreationTime;
                    chunk->sequence = threadSequence++;
                    chunk->used = 0;
                    vkdebug::sharedMemoryWrite(&chunk->state, vkdebug::ChunkWriting);
                    threadChunk = chunk;
                    threadPos = 0;
                    return true;
                }
            }
            vkdebug::sharedMemoryWrite(&header->full, 1);

            /* [xbilko03] wait for timeout, otherwise consider agent not working */
            LARGE_INTEGER now{};
            QueryPerformanceCounter(&now);
            if (last.QuadPart != 0) {
                LONGLONG delta = now.QuadPart - last.QuadPart;
                if (delta < frequency.QuadPart)
                    waited += delta;
            }
            last = now;
            if (waited > 120 * frequency.QuadPart) {
                vkdebug::sharedMemoryWrite(&header->readerAlive, 0);
                return false;
            }
            Sleep(1);
        }
    }

    bool layer_WriterInit() {
        if (globalShared.load())
            return true;

        wchar_t name[64];
        swprintf(name, 64, VKDBG_SHARED_NAME_FORMAT, (unsigned long) (GetCurrentProcessId()));
        uint64_t total = uint64_t(VKDBG_HEADER_PAGE_SIZE) + VKDBG_SHARED_MEMORY_SIZE;
        globalMapping = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
                                           (DWORD) (total >> 32), (DWORD) (total & 0xFFFFFFFF), name);
        if (!globalMapping)
            return false;
        void *view = MapViewOfFile(globalMapping, FILE_MAP_ALL_ACCESS, 0, 0, (SIZE_T) (total));
        if (!view) {
            CloseHandle(globalMapping);
            globalMapping = NULL;
            return false;
        }

        uint8_t *shared = (uint8_t *) (view);
        vkdebug::sharedHeader *header = headerPage(shared);
        header->chunkSize = VKDBG_CHUNK_SIZE;
        header->chunkCount = VKDBG_CHUNK_COUNT;
        header->processId = GetCurrentProcessId();
        globalShared.store(shared);
        return true;
    }

    uint16_t layer_WriterGetAgentPort() {
        uint8_t *shared = globalShared.load();
        if (!shared)
            return 0;
        return (uint16_t) (vkdebug::sharedMemoryRead(&headerPage(shared)->agentPort));
    }

    bool layer_WriterReaderAlive() {
        return sharedIfRead() != nullptr;
    }

    bool layer_StartAgent(const std::string &uiPath) {
        if (!globalShared.load())
            return false;

        /* [xbilko03] agent is started in folder next to VkDebugger.exe */
        std::string folder = uiPath;
        size_t slash = folder.find_last_of("\\/");
        if (slash == std::string::npos)
            folder = ".";
        else
            folder = folder.substr(0, slash);
        std::string agentPath = folder + "\\VkDebuggerAgent.exe";

        std::string commandLine = "\"" + agentPath + "\" " + std::to_string(GetCurrentProcessId());
        STARTUPINFOA startup = {sizeof(startup)};
        PROCESS_INFORMATION process{};
        if (!CreateProcessA(agentPath.c_str(), commandLine.data(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL,
                            &startup, &process))
            return false;
        CloseHandle(process.hProcess);
        CloseHandle(process.hThread);
        return true;
    }

    void layer_SetCurrentCall(uint64_t call) {
        threadCall = call;
    }

    /* [xbilko03] starts creating a message: new fragment with header, opens a new chunk when needed or does nothing when agent is not running */
    void messageWriter::begin(uint8_t messageType, uint16_t function) {
        if (threadWriting || !sharedIfRead())
            return;
        if (threadChunk && threadPos + VKDBG_FRAGMENT_HEADER_SIZE + sizeof(vkdebug::messageHeader) >
            VKDBG_CHUNK_PAYLOAD)
            closeChunk(false);
        if (!threadChunk && !openChunk(0))
            return;

        threadWriting = true;
        active = true;
        beginFragment();
        vkdebug::messageHeader header{};
        header.messageType = messageType;
        header.function = function;
        header.call = threadCall;
        memcpy(payloadOf(threadChunk) + threadPos, &header, sizeof(header));
        threadPos += sizeof(vkdebug::messageHeader);
    }

    /* [xbilko03] finishes creating a message: writes length of last fragment and performs publish (expose the bytes for reading by the agent) and clear threadWriting variable */
    void messageWriter::end() {
        if (!active)
            return;
        if (threadChunk) {
            endFragment();
            publish();
        }
        threadWriting = false;
        active = false;
    }

    /* [xbilko03] writes data, when chunk is full end the fragment, close chunk (flagged as: message continues in next chunk) and continues in a new chunk */
    void messageWriter::bytes(const void *data, size_t size) {
        if (!active)
            return;
        const uint8_t *source = (const uint8_t *) (data);
        while (size > 0) {
            if (threadPos == VKDBG_CHUNK_PAYLOAD) {
                endFragment();
                closeChunk(true);
                if (!openChunk(vkdebug::ChunkFirstContinues)) {
                    active = false;
                    threadWriting = false;
                    return;
                }
                beginFragment();
            }
            size_t room = VKDBG_CHUNK_PAYLOAD - threadPos;
            size_t part = room;
            if (size < room)
                part = size;
            memcpy(payloadOf(threadChunk) + threadPos, source, part);
            threadPos += (uint32_t) (part);
            source += part;
            size -= part;
        }
    }

    void layer_CreateCallBegin(uint16_t function) {
        messageWriter writer;
        writer.begin(vkdebug::MessageCallBegin, function);
        writer.end();
    }

    /* [xbilko03] message type + value type */
    static void layerInfoHeader(messageWriter &writer, uint8_t infoType, uint8_t kind) {
        writer.value(infoType);
        writer.value(kind);
    }

    void layer_CreateLayerInfoU64(uint8_t infoType, uint64_t value) {
        messageWriter writer;
        writer.begin(vkdebug::MessageLayerInfo, 0);
        layerInfoHeader(writer, infoType, vkdebug::ValueU64);
        writer.value(value);
        writer.end();
    }

    void layer_CreateLayerInfoHandle(uint8_t infoType, uint64_t handle) {
        messageWriter writer;
        writer.begin(vkdebug::MessageLayerInfo, 0);
        layerInfoHeader(writer, infoType, vkdebug::ValueHandle);
        writer.value(handle);
        writer.end();
    }

    void layer_CreateLayerInfoDouble(uint8_t infoType, double value) {
        messageWriter writer;
        writer.begin(vkdebug::MessageLayerInfo, 0);
        layerInfoHeader(writer, infoType, vkdebug::ValueDouble);
        writer.value(value);
        writer.end();
    }

    void layer_CreateLayerInfoBool(uint8_t infoType, bool value) {
        uint8_t byte = 0;
        if (value)
            byte = 1;
        messageWriter writer;
        writer.begin(vkdebug::MessageLayerInfo, 0);
        layerInfoHeader(writer, infoType, vkdebug::ValueBool);
        writer.value(byte);
        writer.end();
    }

    void layer_CreateLayerInfoString(uint8_t infoType, const std::string &value) {
        messageWriter writer;
        writer.begin(vkdebug::MessageLayerInfo, 0);
        layerInfoHeader(writer, infoType, vkdebug::ValueString);
        writer.string(value.c_str());
        writer.end();
    }

    void layer_CreateMemoryData(uint64_t handle, const void *bytes, uint64_t size) {
        const uint8_t *source = (const uint8_t *) (bytes);
        uint64_t offset = 0;
        do {
            uint64_t part = size - offset;
            if (part > VKDBG_MAX_MEMORY_MESSAGE_BYTES)
                part = VKDBG_MAX_MEMORY_MESSAGE_BYTES;
            messageWriter writer;
            writer.begin(vkdebug::MessageMemoryData, 0);
            writer.value(handle);
            writer.value(offset);
            writer.value(part);
            writer.bytes(source + offset, (size_t) (part));
            writer.end();
            offset += part;
        } while (offset < size);
    }
}

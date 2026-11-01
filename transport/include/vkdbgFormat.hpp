/*
* Name		    : vkdbgFormat.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : [xbilko03] Header file to distribute shared binary communication format
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
/* AI-Assisted: Claude Opus 5.5: syntax, entire file */
#pragma once

#include <cstdint>
#include <atomic>

/* [xbilko03] shared memory = 4KB header page + 32 MB (1024 * 32 KB chunks)
 *
 * shared memory
 * ------------------------------------------------------------------------------------
 * | header page = 4KB | chunk 0 = 32 KB | chunk 1 = 32 KB | ... | chunk 1023 = 32 KB |
 * ------------------------------------------------------------------------------------
 */
#define VKDBG_SHARED_MEMORY_SIZE (32u * 1024 * 1024)
/* [xbilko03] chunk */
#define VKDBG_CHUNK_SIZE (32u * 1024)
/* [xbilko03] 0...1023 */
#define VKDBG_CHUNK_COUNT (VKDBG_SHARED_MEMORY_SIZE / VKDBG_CHUNK_SIZE)
/* [xbilko03] header page */
#define VKDBG_HEADER_PAGE_SIZE 4096u

/* [xbilko03] %lu = PID of debugged program, it ensures the shared memory's name is unique, so it can be bound to correct target app */
#define VKDBG_SHARED_NAME_FORMAT L"Local\\VkDebugger_%lu"

/* [xbilko03] room for messages within this chunk = chunk size - chunk header
 *
 * chunk
 * -------------------------------------------------
 * | chunk header = 32B | chunk payload 32KB - 32B |
 * -------------------------------------------------
 */
#define VKDBG_CHUNK_PAYLOAD (VKDBG_CHUNK_SIZE - 32u)

/* [xbilko03] each fragment has a header, header includes information about how long that fragment is, fragments of a single message might be scattered across chunks and are stored tightly */
#define VKDBG_FRAGMENT_HEADER_SIZE 4u

/* [xbilko03] VkDeviceMemory (memory, buffers, images) larger than '1024u * 1024 * 1024' B (1GB) is split into more messages */
#define VKDBG_MAX_MEMORY_MESSAGE_BYTES (1024u * 1024 * 1024)

/* [xbilko03]
 * namespace list:
 * vkdebug
 * vkdebug::ui
 * vkdebug::layer
 * vkdebug::generator
 * vkdebug::agent
 */
namespace vkdebug {
    enum messageType : uint8_t {
        MessageCallBegin = 1,
        MessageCallEnd = 2,
        MessageMemoryData = 3,
        MessageLayerInfo = 4,
        MessageCallInputs = 5
    };

    /* [xbilko03] header page for shared memory */
    struct sharedHeader {
        uint32_t chunkSize;
        uint32_t chunkCount;
        uint32_t processId;
        uint32_t readerAlive;
        uint32_t full;
        uint32_t nextChunk;
        uint32_t agentPort;
    };

    /* [xbilko03] flag to indicate state of chunk */
    enum chunkState : uint32_t {
        ChunkFree = 0,
        ChunkWriting = 1,
        ChunkComplete = 2,
        ChunkClaimed = 3
    };

    /* [xbilko03]
     * 0 = 00: chunk holds only full messages
     * 1 = 01: chunk continues message from last chunk
     * 2 = 10: message is too large to fit rest of this chunk, continue message in next chunk
     * 3 = 11: chunk continues message from last chunk, but the message is still too large to fit rest of this chunk, continue message in next chunk
     */
    enum chunkFlags : uint32_t {
        ChunkFirstContinues = 1,
        ChunkLastContinues = 2
    };

    /* [xbilko03]
     * state = what state is chunk in (chunkState)
     * flags = what part of message this chunk contains (chunkFlags)
     * threadId = ID of which thread wrote to the chunk
     * threadCreationTime = time when given thread was created (prevents misidentifying thread based on just ID)
     * sequence = order of chunk among chunks assigned to given thread
     * used = how filled chunk is, agent uses this to send that amount of data
     * padding = 4B, unused to fit header into 32B
     */
    struct chunkHeader {
        uint32_t state;
        uint32_t flags;
        uint32_t threadId;
        uint32_t sequence;
        uint64_t threadCreationTime;
        uint32_t used;
        uint32_t padding;
    };

    static_assert(sizeof(chunkHeader) == 32, "chunk header layout (VKDBG_CHUNK_PAYLOAD counts with 32 B)");

    /* [xbilko03] atomic read for both layer and agent */
    static uint32_t sharedMemoryRead(uint32_t *sharedMemoryValue) {
        return std::atomic_ref<uint32_t>(*sharedMemoryValue).load();
    }

    /* [xbilko03] atomic write */
    static void sharedMemoryWrite(uint32_t *sharedMemoryValue, uint32_t value) {
        std::atomic_ref<uint32_t>(*sharedMemoryValue).store(value);
    }

    /* [xbilko03] atomic add */
    static uint32_t sharedMemoryAdd(uint32_t *sharedMemoryValue, uint32_t add) {
        return std::atomic_ref<uint32_t>(*sharedMemoryValue).fetch_add(add);
    }

    /* [xbilko03] thread attempts to claim a chunk, return true = thread owns chunk, otherwise false and try other chunk */
    static bool sharedMemoryAttemptClaim(uint32_t *sharedMemoryValue, uint32_t expected, uint32_t newValue) {
        return std::atomic_ref<uint32_t>(*sharedMemoryValue).compare_exchange_strong(expected, newValue);
    }

    /* [xbilko03] first byte for message payload of message type MessageLayerInfo, tells UI what kind of specific layer information message is about */
    enum layerInfoType : uint8_t {
        InfoNone = 0,
        InfoWarnings, InfoLogCalls, InfoLogBuffers, InfoLogImages, InfoLogMemory, InfoBreaks, InfoDelim,
        InfoAppName, InfoProcessId, InfoAppPath,
        InfoPtr, InfoSize, InfoMemPtr, InfoBufPtr, InfoImgPtr, InfoSrcBuf, InfoDstBuf, InfoSrcImg, InfoDstImg,
        InfoWidth, InfoHeight, InfoLayoutMismatch,
        InfoMemoryNewBreak, InfoBufferNewBreak, InfoImageNewBreak,
        InfoCallAtBreakTrigger, InfoCallEveryBreakTrigger,
        InfoFrameAtBreakTrigger, InfoFrameEveryBreakTrigger,
        InfoFpsEveryTrigger, InfoFpsBelowTrigger, InfoFpsBelowOptionBreak,
        InfoBreakThread,
        InfoCount
    };

    enum valueKind : uint8_t {
        ValueU64 = 1,
        ValueDouble = 2,
        ValueString = 3,
        ValueBool = 4,
        ValueHandle = 5
    };


    /* [xbilko03] save current padding configuration */
#pragma pack(push)

    /* [xbilko03] ensure the compiler does not touch the layout of these structs (so that it doesn't add padding in between the attributes) */
#pragma pack(1)
    /* [xbilko03] struct for vkDebugger's UI, written by layer at start of each message */
    struct messageHeader {
        uint8_t messageType;
        uint8_t padding;
        uint16_t function;
        uint64_t call;
    };

    /* [xbilko03] struct for vkDebugger's UI, written by agent, attached to resulting chunk sent over TCP */
    struct resultChunkHeader {
        uint32_t threadId;
        uint64_t threadCreationTime;
        uint32_t length;
        uint32_t flags;
    };

    /* [xbilko03] restore padding configuration */
#pragma pack(pop)

    static_assert(sizeof(messageHeader) == 12, "message header must stay 12 B");
    static_assert(sizeof(resultChunkHeader) == 20, "result chunk header must stay 20 B");

    /* [xbilko03] messages to communicate between UI and layer directly outside shared memory */
    enum controlCommand : uint8_t {
        /* [xbilko03] communication: UI to layer */
        ControlPause = 1,
        ControlResume = 2,
        ControlStabilize = 4,

        /* [xbilko03] communication: layer to UI */
        ControlPaused = 20,
        ControlStable = 21,
        ControlUserPause = 22,
        ControlAgentPort = 23,
        ControlThreadsBegin = 24,
        ControlThreadInfo = 25,
        ControlThreadsOther = 26,
        /* [xbilko03] warning: about vulkan timeline semaphores */
        ControlTimelineWait = 27
    };

    enum threadState : uint8_t {
        ThreadRunning = 0,
        ThreadBreakpoint = 1,
        ThreadDead = 2,
        ThreadFrozenWritingMaps = 3,
        ThreadFrozenInsideDriver = 4,
        ThreadFrozenBlockingWait = 5,
        ThreadWaiting = 6,
        ThreadFrozen = 7,
        ThreadInBlockingWait = 8,
        ThreadStillFinishing = 9,
        ThreadStateCount
    };
}

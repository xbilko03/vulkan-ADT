/*
* Name		    : layerControl.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : [xbilko03] Source file for the interface of the control thread for other layer files + control thread logic
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
/* AI-Assisted: Claude Opus 5.5: syntax, entire file */
#include "../include/layer.hpp"
#include "../include/layerControl.hpp"
#include <tlhelp32.h>
#include <vector>
#include <string>
#include <cstring>

namespace vkdebug::layer {
#pragma comment(lib, "Synchronization.lib")

    std::atomic<DWORD> globalControlThreadId{0};

    /* [xbilko03] keeps track of information about a given thread */
    struct threadEntry {
        /* [xbilko03] whether the thread is registered */
        std::atomic<bool> valid{false};
        /* [xbilko03] thread ID = ID|creationTime */
        DWORD id = 0;
        unsigned long long creationTime = 0;
        /* [xbilko03] whereabouts of the execution */
        std::atomic<int> holdsLock{0};
        std::atomic<int> inDriver{0};
        /* [xbilko03] name of the current vulkan command of said thread */
        std::atomic<const char *> currentCall{nullptr};
        /* [xbilko03] waiting at waitWhilePaused() */
        std::atomic<bool> atCheck{false};
        /* [xbilko03] objects the thread works with */
        std::atomic<unsigned long long> objects[5] = {};
    };

#define ENTRIES_PER_BLOCK 64

    struct entryBlock {
        threadEntry entries[ENTRIES_PER_BLOCK];
        std::atomic<entryBlock *> next{nullptr};
    };

    static entryBlock globalFirstBlock;
    static std::atomic<uint32_t> globalThreadCount{0};

    static thread_local threadEntry *ownThreadEntry = nullptr;
    static thread_local bool threadPendingBreak = false;

    static unsigned long long threadCreationTime(HANDLE thread) {
        FILETIME creation{}, exitTime{}, kernelTime{}, userTime{};
        if (!GetThreadTimes(thread, &creation, &exitTime, &kernelTime, &userTime))
            return 0;
        return ((unsigned long long) (creation.dwHighDateTime) << 32) | creation.dwLowDateTime;
    }

    static entryBlock *blockAt(uint32_t blockIndex) {
        entryBlock *block = &globalFirstBlock;
        for (uint32_t i = 0; i < blockIndex; i++) {
            entryBlock *next = block->next.load();
            if (!next) {
                entryBlock *fresh = new entryBlock();
                if (block->next.compare_exchange_strong(next, fresh))
                    next = fresh;
                else
                    delete fresh;
            }
            block = next;
        }
        return block;
    }

    /* [xbilko03] return thread's threadEntry (if not created, create a new threadEntry) */
    static threadEntry *ownEntry() {
        if (ownThreadEntry)
            return ownThreadEntry;

        uint32_t index = globalThreadCount.fetch_add(1);
        threadEntry &entry = blockAt(index / ENTRIES_PER_BLOCK)->entries[index % ENTRIES_PER_BLOCK];
        entry.id = GetCurrentThreadId();
        entry.creationTime = threadCreationTime(GetCurrentThread());
        entry.valid.store(true);
        ownThreadEntry = &entry;
        return ownThreadEntry;
    }

    /* [xbilko03] find thread's threadEntry using its ID|creationTime */
    static threadEntry *findEntry(DWORD id, unsigned long long creationTime) {
        threadEntry *found = nullptr;
        for (entryBlock *block = &globalFirstBlock; block; block = block->next.load())
            for (threadEntry &entry: block->entries) {
                if (!entry.valid.load())
                    continue;
                if (!found && entry.id == id && (creationTime == 0 || entry.creationTime == creationTime))
                    found = &entry;
            }
        return found;
    }

    /* [xbilko03] information about current pause, 1 = paused */
    static std::atomic<uint32_t> globalPaused{0};
    /* [xbilko03] which thread asked for pause */
    static std::atomic<DWORD> globalPausingThreadId{0};
    /* [xbilko03] says whether connection to the UI is ready */
    static std::atomic<bool> globalTransportReady{false};
    /* [xbilko03] windows event that wakes the control thread, requesting a pause */
    static HANDLE globalPauseRequestEvent = NULL;

    /* [xbilko03] a counter for the control thread to wake up every time there is a change during stabilization phase when program is paused (for example thread leaves driver) */
    static std::atomic<uint32_t> globalStateChange{0};

    static void notifyStateChange() {
        if (globalPaused.load() == 0)
            return;
        globalStateChange.fetch_add(1);
        WakeByAddressSingle((PVOID) (&globalStateChange));
    }

    /* [xbilko03] sleep here while program is paused */
    static void waitWhilePaused() {
        threadEntry *entry = ownEntry();
        entry->atCheck.store(true);
        uint32_t pausedValue = 1;
        while (globalPaused.load() == 1)
            WaitOnAddress((volatile VOID *) (&globalPaused), &pausedValue, sizeof(pausedValue), INFINITE);
        entry->atCheck.store(false);
    }

    /* [xbilko03] whereabouts of thread when program is paused (for example, thread is inside a driver) */
    enum frozenLocation { FrozenNothing, FrozenMapLock, FrozenDriverCall, FrozenWaitCall, FrozenCheck };

    struct snapshotEntry {
        DWORD id;
        HANDLE handle;
        unsigned long long creationTime;
        threadEntry *entry;
        bool frozen;
        frozenLocation frozenAt;
        bool armed;
    };

    static std::vector<snapshotEntry> globalSnapshot;

    /* [xbilko03] build control command: length + command type + data */
    struct controlBuilder {
        uint8_t bytes[512];
        size_t length = 2;

        controlBuilder(uint8_t command) { u8(command); }

        void raw(const void *data, size_t size) {
            if (length + size > sizeof(bytes))
                return;
            memcpy(bytes + length, data, size);
            length += size;
        }

        void u8(uint8_t value) { raw(&value, 1); }
        void u16(uint16_t value) { raw(&value, 2); }
        void u32(uint32_t value) { raw(&value, 4); }
        void u64(uint64_t value) { raw(&value, 8); }

        void string(const char *text) {
            size_t size = 0;
            if (text)
                size = strlen(text);
            if (size > 255)
                size = 255;
            u16((uint16_t) (size));
            raw(text, size);
        }

        void send() {
            uint16_t payload = (uint16_t) (length - 2);
            memcpy(bytes, &payload, 2);
            winsockSendRaw(&connectSocket, (const char *) (bytes), length);
        }
    };

    /* [xbilko03] send message without payload, some messages are understood by just values contained within that command's type */
    static void sendEmpty(uint8_t command) {
        controlBuilder m(command);
        m.send();
    }

    static void sendFlag(uint8_t command, bool value) {
        controlBuilder m(command);
        if (value)
            m.u8(1);
        else
            m.u8(0);
        m.send();
    }

    /* [xbilko03] list all threads of target program (without control thread & thread who asked for pause) */
    static void takeSnapshot() {
        globalSnapshot.clear();
        DWORD self = GetCurrentThreadId();
        DWORD pausing = globalPausingThreadId.load();
        DWORD processId = GetCurrentProcessId();

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (snapshot == INVALID_HANDLE_VALUE)
            return;

        THREADENTRY32 entry;
        entry.dwSize = sizeof(entry);
        if (Thread32First(snapshot, &entry)) {
            do {
                if (entry.dwSize >= (FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(entry.th32OwnerProcessID))
                    && entry.th32OwnerProcessID == processId
                    && entry.th32ThreadID != self
                    && entry.th32ThreadID != pausing) {
                    HANDLE thread = OpenThread(
                        THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_SET_CONTEXT |
                        THREAD_QUERY_LIMITED_INFORMATION, FALSE, entry.th32ThreadID);
                    if (thread != NULL) {
                        snapshotEntry item;
                        item.id = entry.th32ThreadID;
                        item.handle = thread;
                        item.creationTime = threadCreationTime(thread);
                        item.entry = findEntry(item.id, item.creationTime);
                        item.frozen = false;
                        item.frozenAt = FrozenNothing;
                        item.armed = false;
                        globalSnapshot.push_back(item);
                    }
                }
                entry.dwSize = sizeof(entry);
            } while (Thread32Next(snapshot, &entry));
        }
        CloseHandle(snapshot);
    }

    /* [xbilko03] suspend a thread & note down its whereabouts */
    static void freezeOne(snapshotEntry &item) {
        /* [xbilko03] ask OS to freeze a thread */
        if (SuspendThread(item.handle) == (DWORD) -1)
            return;

        /* [xbilko03] thread may take a while to freeze, calling GetThreadContext returns only once the thread has fully stopped */
        CONTEXT context{};
        context.ContextFlags = CONTEXT_CONTROL;
        GetThreadContext(item.handle, &context);

        item.frozen = true;
        threadEntry *entry = item.entry;
        if (!entry)
            item.frozenAt = FrozenNothing;
        else if (entry->holdsLock.load() > 0)
            item.frozenAt = FrozenMapLock;
        else if (entry->inDriver.load() == 1)
            item.frozenAt = FrozenDriverCall;
        else if (entry->inDriver.load() == 2)
            item.frozenAt = FrozenWaitCall;
        else if (entry->atCheck.load())
            item.frozenAt = FrozenCheck;
        else
            item.frozenAt = FrozenNothing;
    }

    /* [xbilko03] suspend all threads, note down their whereabouts */
    static void freezeAll() {
        for (snapshotEntry &item: globalSnapshot)
            freezeOne(item);
    }

    /* [xbilko03] flag signaling control thread claimed heap lock */
    static bool globalHeapLocked = false;

    static void lockHeap() {
        if (!globalHeapLocked)
            globalHeapLocked = HeapLock(GetProcessHeap()) != FALSE;
    }

    static void unlockHeap() {
        if (globalHeapLocked) {
            HeapUnlock(GetProcessHeap());
            globalHeapLocked = false;
        }
    }

    /* [xbilko03] start/end address of ntdll lib in memory */
    static uintptr_t globalNtdllStart = 0, globalNtdllEnd = 0;

    struct armEntry {
        std::atomic<DWORD> tid{0};
        DWORD64 address = 0;
        DWORD64 expectedRsp = 0;

        /* 0 = not armed, 1 = armed (running to the exit), 2 = parked */
        std::atomic<int> state{0};
    };

    static armEntry globalArm[1024];

    static armEntry *findArm(DWORD tid) {
        for (armEntry &entry: globalArm)
            if (entry.tid.load() == tid)
                return &entry;
        return nullptr;
    }

    static armEntry *armFor(DWORD tid) {
        armEntry *found = findArm(tid);
        if (found)
            return found;
        for (armEntry &entry: globalArm) {
            DWORD empty = 0;
            if (entry.tid.compare_exchange_strong(empty, tid))
                return &entry;
        }
        return nullptr;
    }

    static bool inNtdll(DWORD64 address) {
        return address >= globalNtdllStart && address < globalNtdllEnd;
    }

    /* [xbilko03] vectored exception handler, catches hardware trap on ntdll's exit spot, pauses thread
     * Dr0 - Dr7 = Debug registers
     * Dr0 = holds the target instruction's address trap
     * Dr6 & Dr7 = debug status and control registers which are used to manage hardware breakpoints
     */
    static LONG CALLBACK parkHandler(EXCEPTION_POINTERS *info) {
        if (info->ExceptionRecord->ExceptionCode != EXCEPTION_SINGLE_STEP)
            return EXCEPTION_CONTINUE_SEARCH;
        CONTEXT *context = info->ContextRecord;
        armEntry *entry = findArm(GetCurrentThreadId());
        if (!entry || context->Rip != entry->address)
            return EXCEPTION_CONTINUE_SEARCH;

        /* [xbilko03] pause was cancelled, remove trap and continue */
        if (entry->state.load() != 1) {
            context->Dr7 = 0;
            context->Dr6 = 0;
            return EXCEPTION_CONTINUE_EXECUTION;
        }
        /* [xbilko03] hit via recursion on a different stack, skip parking */
        if (context->Rsp != entry->expectedRsp) {
            context->EFlags |= 0x10000;
            context->Dr6 = 0;
            return EXCEPTION_CONTINUE_EXECUTION;
        }

        /* [xbilko03] uses thread states and WaitOnAddress to freeze execution until resumed */
        context->Dr7 = 0;
        context->Dr0 = 0;
        context->Dr6 = 0;
        entry->state.store(2);
        notifyStateChange();
        uint32_t pausedValue = 1;
        while (globalPaused.load() == 1)
            WaitOnAddress((volatile VOID *) (&globalPaused), &pausedValue, sizeof(pausedValue), INFINITE);
        entry->state.store(0);
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    /* [xbilko03] when thread stops inside ntdll, find and extract address & stack pointer to first place outside ntdll and returns true */
    static bool ntdllExit(HANDLE thread, DWORD64 &address, DWORD64 &rsp) {
        CONTEXT context{};
        context.ContextFlags = CONTEXT_FULL;
        /* [xbilko03] look at thread's current state at GetThreadContext */
        if (!GetThreadContext(thread, &context) || !inNtdll(context.Rip))
            return false;
        /* [xbilko03] if thread is inside ntdll, simulate return from each function call until it finds exit spot */
        for (int i = 0; i < 64 && inNtdll(context.Rip); i++) {
            DWORD64 imageBase = 0;
            PRUNTIME_FUNCTION function = RtlLookupFunctionEntry(context.Rip, &imageBase, nullptr);
            if (!function) {
                context.Rip = *(DWORD64 *) (context.Rsp);
                context.Rsp += 8;
            } else {
                PVOID handlerData = nullptr;
                DWORD64 establisher = 0;
                RtlVirtualUnwind(UNW_FLAG_NHANDLER, imageBase, context.Rip, function, &context, &handlerData,
                                 &establisher, nullptr);
            }
            if (context.Rip == 0)
                return false;
        }
        if (inNtdll(context.Rip))
            return false;
        address = context.Rip;
        rsp = context.Rsp;
        return true;
    }

    /* [xbilko03] arm hardware trap at ntdll's exit point for frozen threads inside ntdll, then wake them up */
    static int releaseNtdllFrozen() {
        int released = 0;
        /* [xbilko03] for each thread in snapshot */
        for (snapshotEntry &item: globalSnapshot) {
            /* [xbilko03] skip running threads and frozen threads parked safely */
            if (!item.frozen || item.frozenAt == FrozenCheck)
                continue;
            /* [xbilko03] skip if thread holds a lock, is inside a driver, or parked safely (2nd check intended) */
            if (item.entry && (item.entry->holdsLock.load() > 0 || item.entry->inDriver.load() != 0 || item.entry->
                               atCheck.load()))
                continue;

            /* [xbilko03] find where the thread exits ntdll */
            DWORD64 address = 0, rsp = 0;
            if (!ntdllExit(item.handle, address, rsp))
                continue;

            /* [xbilko03] prepare arm slot (an entry of globalArm[1024]) for thread and save target address & stack pointer */
            armEntry *entry = armFor(item.id);
            if (!entry)
                continue;
            entry->address = address;
            entry->expectedRsp = rsp;
            entry->state.store(1);

            /* [xbilko03] set debug register Dr0 to trap the thread at the exit address */
            CONTEXT context{};
            context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
            context.Dr0 = address;
            context.Dr7 = 1; /* L0 enable, execute, length 1 */
            if (!SetThreadContext(item.handle, &context)) {
                entry->state.store(0);
                continue;
            }
            /* [xbilko03] unfreeze, mark as armed, and let the thread run until it hits the trap */
            item.frozen = false;
            item.armed = true;
            ResumeThread(item.handle);
            released++;
        }
        return released;
    }

    /* [xbilko03] wake up paused threads & unarm all threads that never reached exit (should never occur, just in case) */
    static void disarmAll() {
        /* [xbilko03] for each thread in snapshot */
        for (snapshotEntry &item: globalSnapshot) {
            if (!item.armed)
                continue;
            armEntry *entry = findArm(item.id);
            if (!entry || entry->state.load() == 2)
                continue;
            /* [xbilko03] pause this thread to attempt to prevent it hitting armed trap */
            SuspendThread(item.handle);
            int armedState = 1;
            /* [xbilko03] atomic operation to ensure the thread is still armed (state 1) before disarming it, in case SuspendThread did not prevent hitting armed trap in time */
            if (entry->state.compare_exchange_strong(armedState, 0)) {
                CONTEXT context{};
                context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
                SetThreadContext(item.handle, &context);
            }
            /* [xbilko03] unpause thread */
            ResumeThread(item.handle);
        }
    }

    /* [xbilko03] check if pause is stable = no other thread holds the maps lock, are inside vulkan driver doing potentially data-changing operations */
    static bool isStable() {
        DWORD self = GetCurrentThreadId();
        bool stable = true;
        for (entryBlock *block = &globalFirstBlock; block; block = block->next.load())
            for (threadEntry &entry: block->entries) {
                if (!entry.valid.load())
                    continue;
                if (entry.id != self && (entry.holdsLock.load() > 0 || entry.inDriver.load() == 1))
                    stable = false;
            }
        return stable;
    }

    /* [xbilko03] timeline semaphores, submitted GPU wait on a timeline semaphore can wait for a value not provided (wait-before-signal),
     * if frozen thread was supposed to provide it, it might never occurred and job might hang
     * TIMELINE_SEMAPHORES = limit of how many semaphores layer tracks
     * TIMELINE_WAITS = limit of how many submitted waits layer tracks
     */
#define TIMELINE_SEMAPHORES 1024
#define TIMELINE_WAITS 256

    struct timelineSemaphore {
        std::atomic<uint64_t> handle{0};
        std::atomic<uint64_t> provided{0};
    };

    struct timelineWait {
        std::atomic<bool> valid{false};
        uint64_t submit = 0;
        uint32_t batch = 0;
        uint64_t queue = 0;
        uint64_t semaphore = 0;
        uint64_t value = 0;
    };

    static timelineSemaphore globalTimelines[TIMELINE_SEMAPHORES];
    static timelineWait globalTimelineWaits[TIMELINE_WAITS];
    static std::atomic<uint32_t> globalTimelineWaitNext{0};
    static std::atomic<uint64_t> globalSubmitNumber{0};

    /* [xbilko03] try to find semaphore within globalTimelines */
    static timelineSemaphore *findTimeline(uint64_t handle) {
        if (handle == 0)
            return nullptr;
        for (timelineSemaphore &timeline: globalTimelines)
            if (timeline.handle.load() == handle)
                return &timeline;
        return nullptr;
    }

    /* [xbilko03] note value which semaphore received (for example vkSignalSemaphore) */
    static void timelineProvided(uint64_t handle, uint64_t value) {
        timelineSemaphore *timeline = findTimeline(handle);
        if (timeline && timeline->provided.load() < value)
            timeline->provided.store(value);
    }

    /* [xbilko03] write information to layer-local buffer about work waiting for timeline semaphore value before running */
    static void timelineWaitSubmitted(uint64_t submit, uint32_t batch, uint64_t queue, uint64_t handle,
                                      uint64_t value) {
        if (!findTimeline(handle))
            return;
        timelineWait &wait = globalTimelineWaits[globalTimelineWaitNext.fetch_add(1) % TIMELINE_WAITS];
        wait.valid.store(false);
        wait.submit = submit;
        wait.batch = batch;
        wait.queue = queue;
        wait.semaphore = handle;
        wait.value = value;
        wait.valid.store(true);
    }

    void layer_TimelineCreated(VkSemaphoreCreateInfo *pCreateInfo, VkSemaphore semaphore) {
        if (pCreateInfo == nullptr)
            return;
        for (const VkBaseInStructure *next = (const VkBaseInStructure *) (pCreateInfo->pNext); next;
             next = next->pNext) {
            if (next->sType != VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO)
                continue;
            const VkSemaphoreTypeCreateInfo *type = (const VkSemaphoreTypeCreateInfo *) (next);
            if (type->semaphoreType != VK_SEMAPHORE_TYPE_TIMELINE)
                return;
            for (timelineSemaphore &timeline: globalTimelines) {
                uint64_t empty = 0;
                if (timeline.handle.load() == 0) {
                    timeline.provided.store(type->initialValue);
                    if (timeline.handle.compare_exchange_strong(empty, (uint64_t) (semaphore)))
                        return;
                }
            }
            return;
        }
    }

    void layer_TimelineDestroyed(VkSemaphore semaphore) {
        timelineSemaphore *timeline = findTimeline((uint64_t) (semaphore));
        if (timeline)
            timeline->handle.store(0);
    }

    void layer_TimelineSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo *pSubmits) {
        uint64_t submit = globalSubmitNumber.fetch_add(1) + 1;
        for (uint32_t batch = 0; pSubmits && batch < submitCount; batch++) {
            const VkSubmitInfo &info = pSubmits[batch];
            const VkTimelineSemaphoreSubmitInfo *values = nullptr;
            for (const VkBaseInStructure *next = (const VkBaseInStructure *) (info.pNext); next; next = next->pNext)
                if (next->sType == VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO)
                    values = (const VkTimelineSemaphoreSubmitInfo *) (next);
            if (!values)
                continue;
            for (uint32_t i = 0; i < info.signalSemaphoreCount && i < values->signalSemaphoreValueCount; i++)
                timelineProvided((uint64_t) (info.pSignalSemaphores[i]), values->pSignalSemaphoreValues[i]);
            for (uint32_t i = 0; i < info.waitSemaphoreCount && i < values->waitSemaphoreValueCount; i++)
                timelineWaitSubmitted(submit, batch, (uint64_t) (queue), (uint64_t) (info.pWaitSemaphores[i]),
                                      values->pWaitSemaphoreValues[i]);
        }
    }

    void layer_TimelineSubmit2(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2 *pSubmits) {
        uint64_t submit = globalSubmitNumber.fetch_add(1) + 1;
        for (uint32_t batch = 0; pSubmits && batch < submitCount; batch++) {
            const VkSubmitInfo2 &info = pSubmits[batch];
            for (uint32_t i = 0; i < info.signalSemaphoreInfoCount; i++)
                timelineProvided((uint64_t) (info.pSignalSemaphoreInfos[i].semaphore),
                                 info.pSignalSemaphoreInfos[i].value);
            for (uint32_t i = 0; i < info.waitSemaphoreInfoCount; i++)
                timelineWaitSubmitted(submit, batch, (uint64_t) (queue),
                                      (uint64_t) (info.pWaitSemaphoreInfos[i].semaphore),
                                      info.pWaitSemaphoreInfos[i].value);
        }
    }

    void layer_TimelineHostSignal(const VkSemaphoreSignalInfo *pSignalInfo) {
        if (pSignalInfo)
            timelineProvided((uint64_t) (pSignalInfo->semaphore), pSignalInfo->value);
    }

    /* [xbilko03] send every submitted waiting timeline semaphore, which has not received the blocking value yet to UI */
    static void sendTimelineWaits() {
        for (timelineWait &wait: globalTimelineWaits) {
            if (!wait.valid.load())
                continue;
            timelineSemaphore *timeline = findTimeline(wait.semaphore);
            if (!timeline)
                continue;
            uint64_t provided = timeline->provided.load();
            if (provided >= wait.value)
                continue;
            controlBuilder command(vkdebug::ControlTimelineWait);
            command.u64(wait.submit);
            command.u32(wait.batch);
            command.u64(wait.queue);
            command.u64(wait.semaphore);
            command.u64(wait.value);
            command.u64(provided);
            command.send();
        }
    }

    /* [xbilko03] send thread list to UI (state, current call, objects) */
    static void sendThreadList() {
        DWORD self = GetCurrentThreadId();
        DWORD pausing = globalPausingThreadId.load();

        sendEmpty(vkdebug::ControlThreadsBegin);

        for (entryBlock *block = &globalFirstBlock; block; block = block->next.load())
            for (threadEntry &entry: block->entries) {
                if (!entry.valid.load())
                    continue;
                if (entry.id == self)
                    continue;

                const snapshotEntry *found = nullptr;
                for (snapshotEntry &item: globalSnapshot)
                    if (item.entry == &entry)
                        found = &item;

                uint8_t state = vkdebug::ThreadRunning;
                bool showCall = false;
                bool midChange = false;
                if (entry.id == pausing) {
                    state = vkdebug::ThreadBreakpoint;
                    showCall = true;
                } else if (!found)
                    state = vkdebug::ThreadDead;
                else if (found->frozen || found->armed) {
                    switch (found->frozenAt) {
                        case FrozenMapLock: state = vkdebug::ThreadFrozenWritingMaps;
                            showCall = true;
                            midChange = true;
                            break;
                        case FrozenDriverCall: state = vkdebug::ThreadFrozenInsideDriver;
                            showCall = true;
                            midChange = true;
                            break;
                        case FrozenWaitCall: state = vkdebug::ThreadFrozenBlockingWait;
                            showCall = true;
                            break;
                        case FrozenCheck: state = vkdebug::ThreadWaiting;
                            showCall = true;
                            break;
                        default: state = vkdebug::ThreadFrozen;
                            break;
                    }
                } else if (entry.atCheck.load()) {
                    state = vkdebug::ThreadWaiting;
                    showCall = true;
                } else if (entry.inDriver.load() == 2) {
                    state = vkdebug::ThreadInBlockingWait;
                    showCall = true;
                } else if (entry.holdsLock.load() > 0 || entry.inDriver.load() == 1) {
                    state = vkdebug::ThreadStillFinishing;
                    showCall = true;
                    midChange = true;
                }

                controlBuilder line(vkdebug::ControlThreadInfo);
                line.u32(entry.id);
                line.u8(state);
                const char *call = entry.currentCall.load();
                if (!showCall)
                    call = nullptr;
                line.string(call);
                uint8_t count = 0;
                uint64_t objects[5];
                if (midChange) {
                    for (std::atomic<unsigned long long> &object: entry.objects) {
                        unsigned long long value = object.load();
                        if (value != 0)
                            objects[count++] = value;
                    }
                }
                line.u8(count);
                for (uint8_t i = 0; i < count; i++)
                    line.u64(objects[i]);
                line.send();
            }

        uint32_t others = 0;
        for (snapshotEntry &item: globalSnapshot)
            if (item.entry == nullptr)
                others++;
        controlBuilder other(vkdebug::ControlThreadsOther);
        other.u32(others);
        other.send();

        sendTimelineWaits();
    }

    /* [xbilko03] read one command from the UI */
    static uint8_t readCommand() {
        uint16_t length = 0;
        if (!layer_RecvExact((char *) (&length), 2) || length == 0)
            return 0;
        uint8_t command = 0;
        if (!layer_RecvExact((char *) (&command), 1))
            return 0;
        /* [xbilko03] skip extra bytes, command is only of 'size' length */
        char skip[64];
        for (uint16_t left = length - 1; left > 0;) {
            uint16_t part = sizeof(skip);
            if (left < part)
                part = left;
            if (!layer_RecvExact(skip, part))
                return 0;
            left -= part;
        }
        return command;
    }

    /* [xbilko03] check for a command without waiting */
    static int pollCommand(uint8_t &command) {
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(connectSocket, &readSet);
        timeval timeout{0, 0};
        int ready = select(0, &readSet, nullptr, nullptr, &timeout);
        if (ready == SOCKET_ERROR)
            return -1;
        if (ready == 0)
            return 0;
        command = readCommand();
        /* [xbilko03] connection error */
        if (command == 0)
            return -1;
        return 1;
    }

    enum stabilizeResult { StabilizeStable, StabilizeResume };

    /* [xbilko03]
     * for each thread holding a lock or stuck inside a driver call, release it and stop it at a point considered stable:
     * stable = no thread holds a lock and no thread is inside the vulkan api driver
     */
    static void resumeMidWork() {
        for (snapshotEntry &item: globalSnapshot) {
            if (item.frozen && (item.frozenAt == FrozenMapLock || item.frozenAt == FrozenDriverCall || item.
                                frozenAt == FrozenWaitCall)) {
                ResumeThread(item.handle);
                item.frozen = false;
            }
        }
    }

    /* [xbilko03] let frozen threads finish their work until a state is stable or user presses continue, it works in a continuous loop until pause is stable or program continues:
     * 1. UI sent continue or connection drop -> resume threads and exit loop
     * 2. track state of threads in globalStateChange
     * 3. stable pause state check:
     *      yes -> freeze unpaused threads -> check again:
     *                                                  -> if state is stable, proclaim pause stable, break loop/return that fact
     *                                                  -> else release the threads (resumeMidWork) and go to step 4
     *      no -> go to step 4
     * 4. sleep until thread changes a state (globalStateChange) or a 50ms timeout finishes -> go to step 1
     */
    static stabilizeResult stabilize() {
        /* [xbilko03] let frozen threads finish their work */
        resumeMidWork();
        /* [xbilko03] release threads frozen in ntdll (resumed thread might need to also finish its vulkan driver call) */
        releaseNtdllFrozen();

        while (true) {
            /* [xbilko03] check if user wishes to resume program's execution (or if UI is dead) (important due to the fact this loop might never finish so program could get stuck) */
            uint8_t command = 0;
            int polled = pollCommand(command);
            if (polled < 0)
                return StabilizeResume;
            if (polled > 0) {
                if (command == vkdebug::ControlResume)
                    return StabilizeResume;
            }

            /* [xbilko03] read counter before isStable so a thread that finishes in between still wakes the wait below */
            uint32_t seen = globalStateChange.load();
            if (isStable()) {
                /* [xbilko03] attempt to claim heap lock and re-freeze released threads */
                lockHeap();
                for (snapshotEntry &item: globalSnapshot)
                    if (!item.frozen && !item.armed)
                        freezeOne(item);
                unlockHeap();

                /* [xbilko03] nobody holds maps lock, nobody in non-waiting driver calls */
                if (isStable()) {
                    /* [xbilko03] threads inside ntdll are unpaused and stop outside ntdll */
                    if (releaseNtdllFrozen() > 0)
                        continue;
                    sendThreadList();
                    sendFlag(vkdebug::ControlStable, true);
                    return StabilizeStable;
                }

                resumeMidWork();
                releaseNtdllFrozen();
            }

            /* [xbilko03] sleep until a thread leaves the driver or maps lock (ms timeout only to check 'continue' command from UI) */
            WaitOnAddress((volatile VOID *) (&globalStateChange), &seen, sizeof(seen), 50);
        }
    }

    /* [xbilko03] resume all threads, wake waiting threads, inform UI */
    static void endPause() {
        disarmAll();
        for (snapshotEntry &item: globalSnapshot)
            if (item.frozen)
                ResumeThread(item.handle);

        globalPausingThreadId.store(0);
        globalPaused.store(0);
        WakeByAddressAll((PVOID) (&globalPaused));

        sendFlag(vkdebug::ControlPaused, false);

        for (snapshotEntry &item: globalSnapshot)
            CloseHandle(item.handle);
        globalSnapshot.clear();

        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
    }

    /* [xbilko03] stop all threads, snapshot threads state, report it to UI, then wait for instructions from UI */
    static void runPause() {
        /* [xbilko03] set highest priority for control thread while pausing */
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
        takeSnapshot();

        /* [xbilko03] attempt to claim heap lock while freezing (stabilize operation most likely needs it) */
        lockHeap();
        freezeAll();
        unlockHeap();

        /* [xbilko03] if pause seems stable, double check if a thread is still not stuck within ntdll */
        bool stable = isStable();
        if (stable)
            releaseNtdllFrozen();

        sendThreadList();
        sendFlag(vkdebug::ControlStable, stable);
        sendFlag(vkdebug::ControlPaused, true);

        while (true) {
            uint8_t command = readCommand();
            if (command == 0)
                break;
            if (command == vkdebug::ControlResume)
                break;
            if (command == vkdebug::ControlStabilize && !stable) {
                stabilizeResult result = stabilize();
                if (result == StabilizeStable) {
                    stable = true;
                    continue;
                }
                break;
            }
        }

        endPause();
    }

    /* [xbilko03] control thread loop, at start send port of the agent to the UI, watches for pause requests (UI|breakpoints), controls pause logic */
    static DWORD WINAPI controlThreadMain(LPVOID parameter) {
        ownEntry();
        bool agentPortSent = false;
        while (true) {
            /* [xbilko03] sleep until breakpoint is detected (globalPauseRequestEvent) or 10ms passes */
            WaitForSingleObject(globalPauseRequestEvent, 10);
            if (!globalTransportReady.load())
                continue;

            /* [xbilko03] send port of the agent to UI */
            if (!agentPortSent) {
                uint16_t port = layer_WriterGetAgentPort();
                if (port != 0) {
                    controlBuilder command(vkdebug::ControlAgentPort);
                    command.u16(port);
                    command.send();
                    agentPortSent = true;
                }
            }

            /* [xbilko03] decide whether to start a pause:
             * globalPaused.load() == 0 = not paused yet
             */
            if (globalPaused.load() == 0) {
                uint8_t command = 0;
                /* [xbilko03] check if UI asked for pause */
                if (pollCommand(command) > 0 && command == vkdebug::ControlPause) {
                    uint32_t expected = 0;
                    /* [xbilko03] set pause flag and pause program */
                    if (globalPaused.compare_exchange_strong(expected, 1)) {
                        sendEmpty(vkdebug::ControlUserPause);
                        runPause();
                        continue;
                    }
                }
                continue;
            }
            runPause();
        }
    }

    mapsLock::mapsLock(std::mutex &m) {
        lockedMutex = &m;
        ownEntry()->holdsLock.fetch_add(1);
        lockedMutex->lock();
    }

    mapsLock::~mapsLock() {
        lockedMutex->unlock();
        ownEntry()->holdsLock.fetch_sub(1);
        notifyStateChange();
    }

    void layer_ThreadAt(const char *functionName, unsigned long long object0, unsigned long long object1,
                        unsigned long long object2, unsigned long long object3, unsigned long long object4) {
        threadEntry *entry = ownEntry();
        entry->currentCall.store(functionName);
        entry->objects[0].store(object0);
        entry->objects[1].store(object1);
        entry->objects[2].store(object2);
        entry->objects[3].store(object3);
        entry->objects[4].store(object4);
    }

    void layer_EnterDriver(bool waitingCall) {
        /* [xbilko03] waiting calls generally okay to be in driver, others may potentially cause trouble, so they cannot stay a part of stable pause */
        if (waitingCall)
            ownEntry()->inDriver.store(2);
        else
            ownEntry()->inDriver.store(1);
    }

    void layer_LeaveDriver() {
        ownEntry()->inDriver.store(0);
        notifyStateChange();
    }

    void layer_CheckPaused() {
        if (globalPaused.load() == 0)
            return;
        waitWhilePaused();
    }

    void layer_RequestBreak() {
        threadPendingBreak = true;
    }

    void layer_HandlePendingBreak() {
        if (!threadPendingBreak)
            return;
        threadPendingBreak = false;

        if (globalControlThreadId.load() == 0)
            return;

        layer_CreateLayerInfoU64(vkdebug::InfoBreakThread, GetCurrentThreadId());

        /* [xbilko03] only first thread hitting breakpoint starts pause, others wait */
        uint32_t expected = 0;
        if (globalPaused.compare_exchange_strong(expected, 1)) {
            globalPausingThreadId.store(GetCurrentThreadId());
            SetEvent(globalPauseRequestEvent);
        }

        waitWhilePaused();
        layer_AfterBreakResumed();
    }

    void layer_ControlThread_TransportReady() {
        globalTransportReady.store(true);
    }

    void layer_ControlThread_EnsureStarted() {
        static std::atomic<bool> started{false};
        if (started.load())
            return;
        bool expected = false;
        if (!started.compare_exchange_strong(expected, true))
            return;

        if (getWindowName() == "VkDebugger.exe")
            return;

        layer_WriterInit();
        globalSnapshot.reserve(4 * ENTRIES_PER_BLOCK);
        globalPauseRequestEvent = CreateEventW(NULL, FALSE, FALSE, NULL);

        HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
        if (ntdll) {
            IMAGE_NT_HEADERS *headers = (IMAGE_NT_HEADERS *) ((char *) ntdll + ((IMAGE_DOS_HEADER *) ntdll)->e_lfanew);
            globalNtdllStart = (uintptr_t) ntdll;
            globalNtdllEnd = globalNtdllStart + headers->OptionalHeader.SizeOfImage;
            AddVectoredExceptionHandler(1, parkHandler);
        }

        DWORD threadId = 0;
        HANDLE thread = CreateThread(NULL, 0, controlThreadMain, NULL, 0, &threadId);
        if (thread != NULL) {
            globalControlThreadId.store(threadId);
            /* [xbilko03] handle is not needed anymore, safe to do, thread won't stop */
            CloseHandle(thread);
        }
    }
}

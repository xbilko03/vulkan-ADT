/*
* Name		    : layerControl.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : [xbilko03] Header file for the interface of the control thread for other layer files
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
/* AI-Assisted: Claude Opus 5.5: syntax, entire file */
#pragma once

#include <windows.h>
#include <vulkan/vulkan.h>
#include <atomic>
#include <mutex>

namespace vkdebug::layer {
    /* [xbilko03] starts control thread (once in target application) */
    void layer_ControlThread_EnsureStarted();

    /* [xbilko03] signal that the connection between layer <-> UI is established */
    void layer_ControlThread_TransportReady();

    /* [xbilko03] ID of the control thread */
    extern std::atomic<DWORD> globalControlThreadId;

    /* [xbilko03] guards layer's object maps (memory, buffers, images) */
    extern std::mutex globalLock;

    /* [xbilko03] thread locks global lock, mark the fact it holds it (to prevent deadlock in case of a pause) */
    class mapsLock {
    public:
        mapsLock(std::mutex &m);

        ~mapsLock();

    private:
        std::mutex *lockedMutex;
    };

    /* [xbilko03]
     * save function name and handles of thread's current vulkan call
     * objects are useful so that UI shows if displayed data may be outdated during a pause
     * (object0-4 = 5 objects due to current vk.xml allowing at most 5 handles to be sent within a function)
     */
    void layer_ThreadAt(const char *functionName, unsigned long long object0 = 0, unsigned long long object1 = 0,
                        unsigned long long object2 = 0, unsigned long long object3 = 0, unsigned long long object4 = 0);

    /* [xbilko03] mark the thread is inside the driver/other layers */
    void layer_EnterDriver(bool waitingCall);

    /* [xbilko03] mark the thread is no longer inside the driver */
    void layer_LeaveDriver();

    /* [xbilko03] wait here while the app is paused */
    void layer_CheckPaused();

    /* [xbilko03] note that timeline semaphore was created */
    void layer_TimelineCreated(VkSemaphoreCreateInfo *pCreateInfo, VkSemaphore semaphore);

    /* [xbilko03] note that timeline semaphore was destroyed */
    void layer_TimelineDestroyed(VkSemaphore semaphore);

    /* [xbilko03] note that work with timeline semaphore was submitted */
    void layer_TimelineSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo *pSubmits);

    /* [xbilko03] note that work with timeline semaphore was submitted */
    void layer_TimelineSubmit2(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2 *pSubmits);

    /* [xbilko03] note that the semaphore has received a signal from CPU */
    void layer_TimelineHostSignal(const VkSemaphoreSignalInfo *pSignalInfo);

    /* [xbilko03] flag this thread that a breakpoint was hit, pause happens after it frees the lock to layer's maps (image, memory, etc.) */
    void layer_RequestBreak();

    /* [xbilko03] when breakpoint hits, start the pause and wait until signal from UI */
    void layer_HandlePendingBreak();
}

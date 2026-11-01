/*
* Name		    : layer.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file to implement extra behavior to a set of chosen Vulkan functions
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vk_layer.h>
#include "../../appUI/include/communication/winsock.hpp"
#include <string>
#include <map>
#include <filesystem>
#include <iostream>
#include <windows.h>
#include "layerStrings.hpp"
/* AI-Assisted: Claude Opus 5.5: syntax */
#include "layerControl.hpp"
#include "layerWriter.hpp"

namespace vkdebug::layer {
    /* [xbilko03] path to VkDebugger executable, agent runs in the same folder */
    extern std::string globalUiPath;
    /* AI-Assisted: end */

    /* this layer's socket */
    extern SOCKET connectSocket;
    /* AI-Assisted: Claude Sonnet 5: syntax */
    /* [xbilko03] receive 'n' bytes, returns false when connection is broken or closed */
    bool layer_RecvExact(char *buf, int n);

    /* AI-Assisted: end */
    /* connected bool and skipLock makes it possible to use Vulkan functions right on the layer */
    extern bool connected;
    /* AI-Assisted: Claude Sonnet 5: syntax */
    extern thread_local bool skipLock;
    /* AI-Assisted: end */
    /* enables the counting of Vulkan calls to ensure the breakpoint */
    extern bool callAtBreak;
    extern bool callEveryBreak;

    /* breakpoint based on the counting of Vulkan function calls */
    void layer_NewCall();

    /* get current's application name */
    std::string getWindowName();

    /* extracts the starting parameters out of 'VkConfig' */
    void layer_SetEnvVariables();

#define WINDOW_NAME "VkDebugger.exe"

    /* starts the new VkDebuggerApp in case the .exe is found */
    void layer_AppStarter(std::string configContent);

    /* macro to ensure the export of these functions to a .dll */
#define VK_LAYER_EXPORT extern "C" __declspec(dllexport)

    /* checks for the frame warn and breakpoint conditions */
    void frameWarnBreak();

    /* AI-Assisted: Claude Opus 5.5: syntax */
    /* [xbilko03] resume layer capture state correctly after breakpoint (for example for correct FPS tracking) */
    void layer_AfterBreakResumed();

    /* AI-Assisted: end */
    /* corresponding define and function call enable the extra functionality for that chosen Vulkan function before_exec_exists and after_exec_exists decide whether this behavior is on the way down the dispatch chain of up back to the source program */
#define FREEMEMORY_BEFORE_EXEC_EXISTS

    /* free VkDeviceMemory, invalidate its reference, inform the VkDebuggerApp about it too */
    void layer_FreeMemory_before(VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks *pAllocator);

#define BINDIMAGEMEMORY_AFTER_EXEC_EXISTS

    /* change state of the relationship between the VkImage and VkDeviceMemory, inform the VkDebuggerApp about it too */
    void layer_BindImageMemory_after(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);

#define BINDBUFFERMEMORY_AFTER_EXEC_EXISTS

    /* change state of the relationship between the VkBuffer and VkDeviceMemory, inform the VkDebuggerApp about it too */
    void layer_BindBufferMemory_after(VkDevice device, VkBuffer buffer, VkDeviceMemory memory,
                                      VkDeviceSize memoryOffset);

#define ALLOCATEMEMORY_AFTER_EXEC_EXISTS

    /* create a new VkDeviceMemory, save if as an VkMemoryObject and inform VkDebuggerApp */
    void layer_AllocateMemory_after(VkDevice device, VkMemoryAllocateInfo *pAllocateInfo,
                                    VkAllocationCallbacks *pAllocator, VkDeviceMemory *pMemory);

#define CREATEBUFFER_AFTER_EXEC_EXISTS

    /* create a new VkBuffer, save if as an VkBufferObject and inform VkDebuggerApp */
    void layer_CreateBuffer_after(VkDevice device, VkBufferCreateInfo *pCreateInfo, VkAllocationCallbacks *pAllocator,
                                  VkBuffer *pBuffer);

#define CREATEIMAGE_AFTER_EXEC_EXISTS

    /* create a new VkImage, save if as an VkImageObject and inform VkDebuggerApp */
    void layer_CreateImage_after(VkDevice device, VkImageCreateInfo *pCreateInfo, VkAllocationCallbacks *pAllocator,
                                 VkImage *pImage);

#define DESTROYBUFFER_BEFORE_EXEC_EXISTS

    /* free VkBuffer, invalidate its reference, inform the VkDebuggerApp about it too */
    void layer_DestroyBuffer_before(VkDevice device, VkBuffer buffer, VkAllocationCallbacks *pAllocator);

#define DESTROYIMAGE_BEFORE_EXEC_EXISTS

    /* free VkImage, invalidate its reference, inform the VkDebuggerApp about it too */
    void layer_DestroyImage_before(VkDevice device, VkImage image, VkAllocationCallbacks *pAllocator);

#define MAPMEMORY_AFTER_EXEC_EXISTS

    /* maps the CPU ppData to the VkDeviceMemory, set this relationship between these objects along with size and offset */
    void layer_MapMemory_after(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size,
                               VkMemoryMapFlags flags, void **ppData);

#define UNMAPMEMORY_BEFORE_EXEC_EXISTS

    /* unmaps the CPU ppData to the VkDeviceMemory, data read from ppData is no longer relevant and shouldn't be read from */
    void layer_UnmapMemory_before(VkDevice device, VkDeviceMemory memory);

#define CMDCOPYBUFFERTOIMAGE_BEFORE_EXEC_EXISTS

    /* set this relationship between VkImage, VkBuffer and copying the data in the VkDebuggerApp */
    void layer_CmdCopyBufferToImage_before(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage,
                                           VkImageLayout dstImageLayout, uint32_t regionCount,
                                           VkBufferImageCopy *pRegions);

#define CMDCOPYIMAGETOBUFFER_BEFORE_EXEC_EXISTS

    /* set this relationship between VkImage, VkBuffer and copying the data in the VkDebuggerApp */
    void layer_CmdCopyImageToBuffer_before(VkCommandBuffer commandBuffer, VkImage srcImage,
                                           VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount,
                                           VkBufferImageCopy *pRegions);

#define CMDCOPYBUFFER_BEFORE_EXEC_EXISTS

    /* set this relationship between them and copying the data in the VkDebuggerApp */
    void layer_CmdCopyBuffer_before(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer,
                                    uint32_t regionCount, VkBufferCopy *pRegions);

#define CMDCOPYIMAGE_BEFORE_EXEC_EXISTS

    /* set this relationship between them and copying the data in the VkDebuggerApp */
    void layer_CmdCopyImage_before(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout,
                                   VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                                   VkImageCopy *pRegions);

    /* AI-Assisted: Claude Sonnet 5: syntax */
    /* [xbilko03] barriers/waits change image layouts, layer needs to update tracker for these objects */
#define CMDPIPELINEBARRIER_AFTER_EXEC_EXISTS

    void layer_CmdPipelineBarrier_after(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask,
                                        VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags,
                                        uint32_t memoryBarrierCount, VkMemoryBarrier *pMemoryBarriers,
                                        uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier *pBufferMemoryBarriers,
                                        uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier *pImageMemoryBarriers);

#define CMDPIPELINEBARRIER2_AFTER_EXEC_EXISTS

    void layer_CmdPipelineBarrier2_after(VkCommandBuffer commandBuffer, VkDependencyInfo *pDependencyInfo);

#define CMDPIPELINEBARRIER2KHR_AFTER_EXEC_EXISTS

    void layer_CmdPipelineBarrier2KHR_after(VkCommandBuffer commandBuffer, VkDependencyInfo *pDependencyInfo);

#define CMDWAITEVENTS_AFTER_EXEC_EXISTS

    void layer_CmdWaitEvents_after(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent *pEvents,
                                   VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
                                   uint32_t memoryBarrierCount, VkMemoryBarrier *pMemoryBarriers,
                                   uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier *pBufferMemoryBarriers,
                                   uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier *pImageMemoryBarriers);

#define CMDWAITEVENTS2_AFTER_EXEC_EXISTS

    void layer_CmdWaitEvents2_after(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent *pEvents,
                                    VkDependencyInfo *pDependencyInfos);

#define CMDWAITEVENTS2KHR_AFTER_EXEC_EXISTS

    void layer_CmdWaitEvents2KHR_after(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent *pEvents,
                                       VkDependencyInfo *pDependencyInfos);

    /* AI-Assisted: end */

#define CREATEINSTANCE_AFTER_EXEC_EXISTS

    /* creates a new data object, stores the instance for use later */
    void layer_CreateInstance_after(const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator,
                                    VkInstance *pInstance);

#define CREATEDEVICE_AFTER_EXEC_EXISTS

    /* gets the reference to currently used device so it can be used inside the image/buffer data reader */
    void layer_CreateDevice_after(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo *pCreateInfo,
                                  const VkAllocationCallbacks *pAllocator, VkDevice *pDevice);

#define ACQUIRENEXTIMAGEKHR_BEFORE_EXEC_EXISTS

    /* potential frame delimiter function, ensure the breakpoints and warnings are counted in case it is a frame delimiter function */
    void layer_AcquireNextImageKHR_before(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout,
                                          VkSemaphore semaphore, VkFence fence, uint32_t *pImageIndex);

#define QUEUESUBMIT_AFTER_EXEC_EXISTS

    /* potential frame delimiter function, ensure the breakpoints and warnings are counted in case it is a frame delimiter function */
    void layer_QueueSubmit_after(VkQueue queue, uint32_t submitCount, VkSubmitInfo *pSubmits, VkFence fence);

    /* AI-Assisted: Claude Opus 5.5: syntax */
    /* [xbilko03] timeline semaphores/submits are tracked, UI is informed about the fact that GPU might be stuck during a pause */
#define CREATESEMAPHORE_AFTER_EXEC_EXISTS

    void layer_CreateSemaphore_after(VkDevice device, VkSemaphoreCreateInfo *pCreateInfo,
                                     VkAllocationCallbacks *pAllocator, VkSemaphore *pSemaphore);

#define DESTROYSEMAPHORE_BEFORE_EXEC_EXISTS

    void layer_DestroySemaphore_before(VkDevice device, VkSemaphore semaphore, VkAllocationCallbacks *pAllocator);

#define SIGNALSEMAPHORE_AFTER_EXEC_EXISTS

    void layer_SignalSemaphore_after(VkDevice device, VkSemaphoreSignalInfo *pSignalInfo);

#define SIGNALSEMAPHOREKHR_AFTER_EXEC_EXISTS

    void layer_SignalSemaphoreKHR_after(VkDevice device, VkSemaphoreSignalInfo *pSignalInfo);

#define QUEUESUBMIT2_AFTER_EXEC_EXISTS

    void layer_QueueSubmit2_after(VkQueue queue, uint32_t submitCount, VkSubmitInfo2 *pSubmits, VkFence fence);

#define QUEUESUBMIT2KHR_AFTER_EXEC_EXISTS

    void layer_QueueSubmit2KHR_after(VkQueue queue, uint32_t submitCount, VkSubmitInfo2 *pSubmits, VkFence fence);

    /* AI-Assisted: end */

#define QUEUEPRESENTKHR_BEFORE_EXEC_EXISTS

    /* [xbilko03] one of possible delimiters, checks for FPS counter */
    void layer_QueuePresentKHR_before(VkQueue queue, VkPresentInfoKHR *pPresentInfo);

    /* AI-Assisted: Claude Opus 5.5: syntax */
}

/* AI-Assisted: end */

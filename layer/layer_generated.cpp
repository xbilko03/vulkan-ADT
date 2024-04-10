#include "layer.hpp"
#include <mutex>
#include <ws2tcpip.h>
#include "../layergen/vk_layer_dispatch_table.h"
#include "../vklayer/vk_layer_table.h"

std::map<void*, VkLayerInstanceDispatchTable> instance_dispatch;
std::map<void*, VkLayerDispatchTable> device_dispatch;
std::mutex global_lock;
typedef std::lock_guard<std::mutex> scoped_lock;
template<typename DispatchableType>
void* GetKey(DispatchableType inst)
{ return *(void**)inst; }
void CreateDeviceDispatch(PFN_vkGetDeviceProcAddr gdpa, VkDevice* pDevice) {
VkLayerDispatchTable dispatchTable;
dispatchTable.GetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)gdpa(*pDevice, "vkGetDeviceProcAddr");
dispatchTable.DestroyDevice = (PFN_vkDestroyDevice)gdpa(*pDevice, "vkDestroyDevice");
dispatchTable.GetDeviceQueue = (PFN_vkGetDeviceQueue)gdpa(*pDevice, "vkGetDeviceQueue");
dispatchTable.QueueSubmit = (PFN_vkQueueSubmit)gdpa(*pDevice, "vkQueueSubmit");
dispatchTable.QueueWaitIdle = (PFN_vkQueueWaitIdle)gdpa(*pDevice, "vkQueueWaitIdle");
dispatchTable.DeviceWaitIdle = (PFN_vkDeviceWaitIdle)gdpa(*pDevice, "vkDeviceWaitIdle");
dispatchTable.AllocateMemory = (PFN_vkAllocateMemory)gdpa(*pDevice, "vkAllocateMemory");
dispatchTable.FreeMemory = (PFN_vkFreeMemory)gdpa(*pDevice, "vkFreeMemory");
dispatchTable.MapMemory = (PFN_vkMapMemory)gdpa(*pDevice, "vkMapMemory");
dispatchTable.UnmapMemory = (PFN_vkUnmapMemory)gdpa(*pDevice, "vkUnmapMemory");
dispatchTable.FlushMappedMemoryRanges = (PFN_vkFlushMappedMemoryRanges)gdpa(*pDevice, "vkFlushMappedMemoryRanges");
dispatchTable.InvalidateMappedMemoryRanges = (PFN_vkInvalidateMappedMemoryRanges)gdpa(*pDevice, "vkInvalidateMappedMemoryRanges");
dispatchTable.GetDeviceMemoryCommitment = (PFN_vkGetDeviceMemoryCommitment)gdpa(*pDevice, "vkGetDeviceMemoryCommitment");
dispatchTable.GetBufferMemoryRequirements = (PFN_vkGetBufferMemoryRequirements)gdpa(*pDevice, "vkGetBufferMemoryRequirements");
dispatchTable.BindBufferMemory = (PFN_vkBindBufferMemory)gdpa(*pDevice, "vkBindBufferMemory");
dispatchTable.GetImageMemoryRequirements = (PFN_vkGetImageMemoryRequirements)gdpa(*pDevice, "vkGetImageMemoryRequirements");
dispatchTable.BindImageMemory = (PFN_vkBindImageMemory)gdpa(*pDevice, "vkBindImageMemory");
dispatchTable.GetImageSparseMemoryRequirements = (PFN_vkGetImageSparseMemoryRequirements)gdpa(*pDevice, "vkGetImageSparseMemoryRequirements");
dispatchTable.QueueBindSparse = (PFN_vkQueueBindSparse)gdpa(*pDevice, "vkQueueBindSparse");
dispatchTable.CreateFence = (PFN_vkCreateFence)gdpa(*pDevice, "vkCreateFence");
dispatchTable.DestroyFence = (PFN_vkDestroyFence)gdpa(*pDevice, "vkDestroyFence");
dispatchTable.ResetFences = (PFN_vkResetFences)gdpa(*pDevice, "vkResetFences");
dispatchTable.GetFenceStatus = (PFN_vkGetFenceStatus)gdpa(*pDevice, "vkGetFenceStatus");
dispatchTable.WaitForFences = (PFN_vkWaitForFences)gdpa(*pDevice, "vkWaitForFences");
dispatchTable.CreateSemaphore = (PFN_vkCreateSemaphore)gdpa(*pDevice, "vkCreateSemaphore");
dispatchTable.DestroySemaphore = (PFN_vkDestroySemaphore)gdpa(*pDevice, "vkDestroySemaphore");
dispatchTable.CreateEvent = (PFN_vkCreateEvent)gdpa(*pDevice, "vkCreateEvent");
dispatchTable.DestroyEvent = (PFN_vkDestroyEvent)gdpa(*pDevice, "vkDestroyEvent");
dispatchTable.GetEventStatus = (PFN_vkGetEventStatus)gdpa(*pDevice, "vkGetEventStatus");
dispatchTable.SetEvent = (PFN_vkSetEvent)gdpa(*pDevice, "vkSetEvent");
dispatchTable.ResetEvent = (PFN_vkResetEvent)gdpa(*pDevice, "vkResetEvent");
dispatchTable.CreateQueryPool = (PFN_vkCreateQueryPool)gdpa(*pDevice, "vkCreateQueryPool");
dispatchTable.DestroyQueryPool = (PFN_vkDestroyQueryPool)gdpa(*pDevice, "vkDestroyQueryPool");
dispatchTable.GetQueryPoolResults = (PFN_vkGetQueryPoolResults)gdpa(*pDevice, "vkGetQueryPoolResults");
dispatchTable.ResetQueryPool = (PFN_vkResetQueryPool)gdpa(*pDevice, "vkResetQueryPool");
dispatchTable.CreateBuffer = (PFN_vkCreateBuffer)gdpa(*pDevice, "vkCreateBuffer");
dispatchTable.DestroyBuffer = (PFN_vkDestroyBuffer)gdpa(*pDevice, "vkDestroyBuffer");
dispatchTable.CreateBufferView = (PFN_vkCreateBufferView)gdpa(*pDevice, "vkCreateBufferView");
dispatchTable.DestroyBufferView = (PFN_vkDestroyBufferView)gdpa(*pDevice, "vkDestroyBufferView");
dispatchTable.CreateImage = (PFN_vkCreateImage)gdpa(*pDevice, "vkCreateImage");
dispatchTable.DestroyImage = (PFN_vkDestroyImage)gdpa(*pDevice, "vkDestroyImage");
dispatchTable.GetImageSubresourceLayout = (PFN_vkGetImageSubresourceLayout)gdpa(*pDevice, "vkGetImageSubresourceLayout");
dispatchTable.CreateImageView = (PFN_vkCreateImageView)gdpa(*pDevice, "vkCreateImageView");
dispatchTable.DestroyImageView = (PFN_vkDestroyImageView)gdpa(*pDevice, "vkDestroyImageView");
dispatchTable.CreateShaderModule = (PFN_vkCreateShaderModule)gdpa(*pDevice, "vkCreateShaderModule");
dispatchTable.DestroyShaderModule = (PFN_vkDestroyShaderModule)gdpa(*pDevice, "vkDestroyShaderModule");
dispatchTable.CreatePipelineCache = (PFN_vkCreatePipelineCache)gdpa(*pDevice, "vkCreatePipelineCache");
dispatchTable.DestroyPipelineCache = (PFN_vkDestroyPipelineCache)gdpa(*pDevice, "vkDestroyPipelineCache");
dispatchTable.GetPipelineCacheData = (PFN_vkGetPipelineCacheData)gdpa(*pDevice, "vkGetPipelineCacheData");
dispatchTable.MergePipelineCaches = (PFN_vkMergePipelineCaches)gdpa(*pDevice, "vkMergePipelineCaches");
dispatchTable.CreateGraphicsPipelines = (PFN_vkCreateGraphicsPipelines)gdpa(*pDevice, "vkCreateGraphicsPipelines");
dispatchTable.CreateComputePipelines = (PFN_vkCreateComputePipelines)gdpa(*pDevice, "vkCreateComputePipelines");
dispatchTable.GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI = (PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI)gdpa(*pDevice, "vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI");
dispatchTable.DestroyPipeline = (PFN_vkDestroyPipeline)gdpa(*pDevice, "vkDestroyPipeline");
dispatchTable.CreatePipelineLayout = (PFN_vkCreatePipelineLayout)gdpa(*pDevice, "vkCreatePipelineLayout");
dispatchTable.DestroyPipelineLayout = (PFN_vkDestroyPipelineLayout)gdpa(*pDevice, "vkDestroyPipelineLayout");
dispatchTable.CreateSampler = (PFN_vkCreateSampler)gdpa(*pDevice, "vkCreateSampler");
dispatchTable.DestroySampler = (PFN_vkDestroySampler)gdpa(*pDevice, "vkDestroySampler");
dispatchTable.CreateDescriptorSetLayout = (PFN_vkCreateDescriptorSetLayout)gdpa(*pDevice, "vkCreateDescriptorSetLayout");
dispatchTable.DestroyDescriptorSetLayout = (PFN_vkDestroyDescriptorSetLayout)gdpa(*pDevice, "vkDestroyDescriptorSetLayout");
dispatchTable.CreateDescriptorPool = (PFN_vkCreateDescriptorPool)gdpa(*pDevice, "vkCreateDescriptorPool");
dispatchTable.DestroyDescriptorPool = (PFN_vkDestroyDescriptorPool)gdpa(*pDevice, "vkDestroyDescriptorPool");
dispatchTable.ResetDescriptorPool = (PFN_vkResetDescriptorPool)gdpa(*pDevice, "vkResetDescriptorPool");
dispatchTable.AllocateDescriptorSets = (PFN_vkAllocateDescriptorSets)gdpa(*pDevice, "vkAllocateDescriptorSets");
dispatchTable.FreeDescriptorSets = (PFN_vkFreeDescriptorSets)gdpa(*pDevice, "vkFreeDescriptorSets");
dispatchTable.UpdateDescriptorSets = (PFN_vkUpdateDescriptorSets)gdpa(*pDevice, "vkUpdateDescriptorSets");
dispatchTable.CreateFramebuffer = (PFN_vkCreateFramebuffer)gdpa(*pDevice, "vkCreateFramebuffer");
dispatchTable.DestroyFramebuffer = (PFN_vkDestroyFramebuffer)gdpa(*pDevice, "vkDestroyFramebuffer");
dispatchTable.CreateRenderPass = (PFN_vkCreateRenderPass)gdpa(*pDevice, "vkCreateRenderPass");
dispatchTable.DestroyRenderPass = (PFN_vkDestroyRenderPass)gdpa(*pDevice, "vkDestroyRenderPass");
dispatchTable.GetRenderAreaGranularity = (PFN_vkGetRenderAreaGranularity)gdpa(*pDevice, "vkGetRenderAreaGranularity");
dispatchTable.GetRenderingAreaGranularityKHR = (PFN_vkGetRenderingAreaGranularityKHR)gdpa(*pDevice, "vkGetRenderingAreaGranularityKHR");
dispatchTable.CreateCommandPool = (PFN_vkCreateCommandPool)gdpa(*pDevice, "vkCreateCommandPool");
dispatchTable.DestroyCommandPool = (PFN_vkDestroyCommandPool)gdpa(*pDevice, "vkDestroyCommandPool");
dispatchTable.ResetCommandPool = (PFN_vkResetCommandPool)gdpa(*pDevice, "vkResetCommandPool");
dispatchTable.AllocateCommandBuffers = (PFN_vkAllocateCommandBuffers)gdpa(*pDevice, "vkAllocateCommandBuffers");
dispatchTable.FreeCommandBuffers = (PFN_vkFreeCommandBuffers)gdpa(*pDevice, "vkFreeCommandBuffers");
dispatchTable.BeginCommandBuffer = (PFN_vkBeginCommandBuffer)gdpa(*pDevice, "vkBeginCommandBuffer");
dispatchTable.EndCommandBuffer = (PFN_vkEndCommandBuffer)gdpa(*pDevice, "vkEndCommandBuffer");
dispatchTable.ResetCommandBuffer = (PFN_vkResetCommandBuffer)gdpa(*pDevice, "vkResetCommandBuffer");
dispatchTable.CmdBindPipeline = (PFN_vkCmdBindPipeline)gdpa(*pDevice, "vkCmdBindPipeline");
dispatchTable.CmdSetAttachmentFeedbackLoopEnableEXT = (PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT)gdpa(*pDevice, "vkCmdSetAttachmentFeedbackLoopEnableEXT");
dispatchTable.CmdSetViewport = (PFN_vkCmdSetViewport)gdpa(*pDevice, "vkCmdSetViewport");
dispatchTable.CmdSetScissor = (PFN_vkCmdSetScissor)gdpa(*pDevice, "vkCmdSetScissor");
dispatchTable.CmdSetLineWidth = (PFN_vkCmdSetLineWidth)gdpa(*pDevice, "vkCmdSetLineWidth");
dispatchTable.CmdSetDepthBias = (PFN_vkCmdSetDepthBias)gdpa(*pDevice, "vkCmdSetDepthBias");
dispatchTable.CmdSetBlendConstants = (PFN_vkCmdSetBlendConstants)gdpa(*pDevice, "vkCmdSetBlendConstants");
dispatchTable.CmdSetDepthBounds = (PFN_vkCmdSetDepthBounds)gdpa(*pDevice, "vkCmdSetDepthBounds");
dispatchTable.CmdSetStencilCompareMask = (PFN_vkCmdSetStencilCompareMask)gdpa(*pDevice, "vkCmdSetStencilCompareMask");
dispatchTable.CmdSetStencilWriteMask = (PFN_vkCmdSetStencilWriteMask)gdpa(*pDevice, "vkCmdSetStencilWriteMask");
dispatchTable.CmdSetStencilReference = (PFN_vkCmdSetStencilReference)gdpa(*pDevice, "vkCmdSetStencilReference");
dispatchTable.CmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets)gdpa(*pDevice, "vkCmdBindDescriptorSets");
dispatchTable.CmdBindIndexBuffer = (PFN_vkCmdBindIndexBuffer)gdpa(*pDevice, "vkCmdBindIndexBuffer");
dispatchTable.CmdBindVertexBuffers = (PFN_vkCmdBindVertexBuffers)gdpa(*pDevice, "vkCmdBindVertexBuffers");
dispatchTable.CmdDraw = (PFN_vkCmdDraw)gdpa(*pDevice, "vkCmdDraw");
dispatchTable.CmdDrawIndexed = (PFN_vkCmdDrawIndexed)gdpa(*pDevice, "vkCmdDrawIndexed");
dispatchTable.CmdDrawMultiEXT = (PFN_vkCmdDrawMultiEXT)gdpa(*pDevice, "vkCmdDrawMultiEXT");
dispatchTable.CmdDrawMultiIndexedEXT = (PFN_vkCmdDrawMultiIndexedEXT)gdpa(*pDevice, "vkCmdDrawMultiIndexedEXT");
dispatchTable.CmdDrawIndirect = (PFN_vkCmdDrawIndirect)gdpa(*pDevice, "vkCmdDrawIndirect");
dispatchTable.CmdDrawIndexedIndirect = (PFN_vkCmdDrawIndexedIndirect)gdpa(*pDevice, "vkCmdDrawIndexedIndirect");
dispatchTable.CmdDispatch = (PFN_vkCmdDispatch)gdpa(*pDevice, "vkCmdDispatch");
dispatchTable.CmdDispatchIndirect = (PFN_vkCmdDispatchIndirect)gdpa(*pDevice, "vkCmdDispatchIndirect");
dispatchTable.CmdSubpassShadingHUAWEI = (PFN_vkCmdSubpassShadingHUAWEI)gdpa(*pDevice, "vkCmdSubpassShadingHUAWEI");
dispatchTable.CmdDrawClusterHUAWEI = (PFN_vkCmdDrawClusterHUAWEI)gdpa(*pDevice, "vkCmdDrawClusterHUAWEI");
dispatchTable.CmdDrawClusterIndirectHUAWEI = (PFN_vkCmdDrawClusterIndirectHUAWEI)gdpa(*pDevice, "vkCmdDrawClusterIndirectHUAWEI");
dispatchTable.CmdUpdatePipelineIndirectBufferNV = (PFN_vkCmdUpdatePipelineIndirectBufferNV)gdpa(*pDevice, "vkCmdUpdatePipelineIndirectBufferNV");
dispatchTable.CmdCopyBuffer = (PFN_vkCmdCopyBuffer)gdpa(*pDevice, "vkCmdCopyBuffer");
dispatchTable.CmdCopyImage = (PFN_vkCmdCopyImage)gdpa(*pDevice, "vkCmdCopyImage");
dispatchTable.CmdBlitImage = (PFN_vkCmdBlitImage)gdpa(*pDevice, "vkCmdBlitImage");
dispatchTable.CmdCopyBufferToImage = (PFN_vkCmdCopyBufferToImage)gdpa(*pDevice, "vkCmdCopyBufferToImage");
dispatchTable.CmdCopyImageToBuffer = (PFN_vkCmdCopyImageToBuffer)gdpa(*pDevice, "vkCmdCopyImageToBuffer");
dispatchTable.CmdCopyMemoryIndirectNV = (PFN_vkCmdCopyMemoryIndirectNV)gdpa(*pDevice, "vkCmdCopyMemoryIndirectNV");
dispatchTable.CmdCopyMemoryToImageIndirectNV = (PFN_vkCmdCopyMemoryToImageIndirectNV)gdpa(*pDevice, "vkCmdCopyMemoryToImageIndirectNV");
dispatchTable.CmdUpdateBuffer = (PFN_vkCmdUpdateBuffer)gdpa(*pDevice, "vkCmdUpdateBuffer");
dispatchTable.CmdFillBuffer = (PFN_vkCmdFillBuffer)gdpa(*pDevice, "vkCmdFillBuffer");
dispatchTable.CmdClearColorImage = (PFN_vkCmdClearColorImage)gdpa(*pDevice, "vkCmdClearColorImage");
dispatchTable.CmdClearDepthStencilImage = (PFN_vkCmdClearDepthStencilImage)gdpa(*pDevice, "vkCmdClearDepthStencilImage");
dispatchTable.CmdClearAttachments = (PFN_vkCmdClearAttachments)gdpa(*pDevice, "vkCmdClearAttachments");
dispatchTable.CmdResolveImage = (PFN_vkCmdResolveImage)gdpa(*pDevice, "vkCmdResolveImage");
dispatchTable.CmdSetEvent = (PFN_vkCmdSetEvent)gdpa(*pDevice, "vkCmdSetEvent");
dispatchTable.CmdResetEvent = (PFN_vkCmdResetEvent)gdpa(*pDevice, "vkCmdResetEvent");
dispatchTable.CmdWaitEvents = (PFN_vkCmdWaitEvents)gdpa(*pDevice, "vkCmdWaitEvents");
dispatchTable.CmdPipelineBarrier = (PFN_vkCmdPipelineBarrier)gdpa(*pDevice, "vkCmdPipelineBarrier");
dispatchTable.CmdBeginQuery = (PFN_vkCmdBeginQuery)gdpa(*pDevice, "vkCmdBeginQuery");
dispatchTable.CmdEndQuery = (PFN_vkCmdEndQuery)gdpa(*pDevice, "vkCmdEndQuery");
dispatchTable.CmdBeginConditionalRenderingEXT = (PFN_vkCmdBeginConditionalRenderingEXT)gdpa(*pDevice, "vkCmdBeginConditionalRenderingEXT");
dispatchTable.CmdEndConditionalRenderingEXT = (PFN_vkCmdEndConditionalRenderingEXT)gdpa(*pDevice, "vkCmdEndConditionalRenderingEXT");
dispatchTable.CmdResetQueryPool = (PFN_vkCmdResetQueryPool)gdpa(*pDevice, "vkCmdResetQueryPool");
dispatchTable.CmdWriteTimestamp = (PFN_vkCmdWriteTimestamp)gdpa(*pDevice, "vkCmdWriteTimestamp");
dispatchTable.CmdCopyQueryPoolResults = (PFN_vkCmdCopyQueryPoolResults)gdpa(*pDevice, "vkCmdCopyQueryPoolResults");
dispatchTable.CmdPushConstants = (PFN_vkCmdPushConstants)gdpa(*pDevice, "vkCmdPushConstants");
dispatchTable.CmdBeginRenderPass = (PFN_vkCmdBeginRenderPass)gdpa(*pDevice, "vkCmdBeginRenderPass");
dispatchTable.CmdNextSubpass = (PFN_vkCmdNextSubpass)gdpa(*pDevice, "vkCmdNextSubpass");
dispatchTable.CmdEndRenderPass = (PFN_vkCmdEndRenderPass)gdpa(*pDevice, "vkCmdEndRenderPass");
dispatchTable.CmdExecuteCommands = (PFN_vkCmdExecuteCommands)gdpa(*pDevice, "vkCmdExecuteCommands");
dispatchTable.CreateSharedSwapchainsKHR = (PFN_vkCreateSharedSwapchainsKHR)gdpa(*pDevice, "vkCreateSharedSwapchainsKHR");
dispatchTable.CreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)gdpa(*pDevice, "vkCreateSwapchainKHR");
dispatchTable.DestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)gdpa(*pDevice, "vkDestroySwapchainKHR");
dispatchTable.GetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)gdpa(*pDevice, "vkGetSwapchainImagesKHR");
dispatchTable.AcquireNextImageKHR = (PFN_vkAcquireNextImageKHR)gdpa(*pDevice, "vkAcquireNextImageKHR");
dispatchTable.QueuePresentKHR = (PFN_vkQueuePresentKHR)gdpa(*pDevice, "vkQueuePresentKHR");
dispatchTable.DebugMarkerSetObjectNameEXT = (PFN_vkDebugMarkerSetObjectNameEXT)gdpa(*pDevice, "vkDebugMarkerSetObjectNameEXT");
dispatchTable.DebugMarkerSetObjectTagEXT = (PFN_vkDebugMarkerSetObjectTagEXT)gdpa(*pDevice, "vkDebugMarkerSetObjectTagEXT");
dispatchTable.CmdDebugMarkerBeginEXT = (PFN_vkCmdDebugMarkerBeginEXT)gdpa(*pDevice, "vkCmdDebugMarkerBeginEXT");
dispatchTable.CmdDebugMarkerEndEXT = (PFN_vkCmdDebugMarkerEndEXT)gdpa(*pDevice, "vkCmdDebugMarkerEndEXT");
dispatchTable.CmdDebugMarkerInsertEXT = (PFN_vkCmdDebugMarkerInsertEXT)gdpa(*pDevice, "vkCmdDebugMarkerInsertEXT");
#if defined(VK_USE_PLATFORM_WIN32_KHR)
dispatchTable.GetMemoryWin32HandleNV = (PFN_vkGetMemoryWin32HandleNV)gdpa(*pDevice, "vkGetMemoryWin32HandleNV");
#endif
dispatchTable.CmdExecuteGeneratedCommandsNV = (PFN_vkCmdExecuteGeneratedCommandsNV)gdpa(*pDevice, "vkCmdExecuteGeneratedCommandsNV");
dispatchTable.CmdPreprocessGeneratedCommandsNV = (PFN_vkCmdPreprocessGeneratedCommandsNV)gdpa(*pDevice, "vkCmdPreprocessGeneratedCommandsNV");
dispatchTable.CmdBindPipelineShaderGroupNV = (PFN_vkCmdBindPipelineShaderGroupNV)gdpa(*pDevice, "vkCmdBindPipelineShaderGroupNV");
dispatchTable.GetGeneratedCommandsMemoryRequirementsNV = (PFN_vkGetGeneratedCommandsMemoryRequirementsNV)gdpa(*pDevice, "vkGetGeneratedCommandsMemoryRequirementsNV");
dispatchTable.CreateIndirectCommandsLayoutNV = (PFN_vkCreateIndirectCommandsLayoutNV)gdpa(*pDevice, "vkCreateIndirectCommandsLayoutNV");
dispatchTable.DestroyIndirectCommandsLayoutNV = (PFN_vkDestroyIndirectCommandsLayoutNV)gdpa(*pDevice, "vkDestroyIndirectCommandsLayoutNV");
dispatchTable.CmdPushDescriptorSetKHR = (PFN_vkCmdPushDescriptorSetKHR)gdpa(*pDevice, "vkCmdPushDescriptorSetKHR");
dispatchTable.TrimCommandPool = (PFN_vkTrimCommandPool)gdpa(*pDevice, "vkTrimCommandPool");
#if defined(VK_USE_PLATFORM_WIN32_KHR)
dispatchTable.GetMemoryWin32HandleKHR = (PFN_vkGetMemoryWin32HandleKHR)gdpa(*pDevice, "vkGetMemoryWin32HandleKHR");
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
dispatchTable.GetMemoryWin32HandlePropertiesKHR = (PFN_vkGetMemoryWin32HandlePropertiesKHR)gdpa(*pDevice, "vkGetMemoryWin32HandlePropertiesKHR");
#endif
dispatchTable.GetMemoryFdKHR = (PFN_vkGetMemoryFdKHR)gdpa(*pDevice, "vkGetMemoryFdKHR");
dispatchTable.GetMemoryFdPropertiesKHR = (PFN_vkGetMemoryFdPropertiesKHR)gdpa(*pDevice, "vkGetMemoryFdPropertiesKHR");
#if defined(VK_USE_PLATFORM_FUCHSIA)
dispatchTable.GetMemoryZirconHandleFUCHSIA = (PFN_vkGetMemoryZirconHandleFUCHSIA)gdpa(*pDevice, "vkGetMemoryZirconHandleFUCHSIA");
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
dispatchTable.GetMemoryZirconHandlePropertiesFUCHSIA = (PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA)gdpa(*pDevice, "vkGetMemoryZirconHandlePropertiesFUCHSIA");
#endif
dispatchTable.GetMemoryRemoteAddressNV = (PFN_vkGetMemoryRemoteAddressNV)gdpa(*pDevice, "vkGetMemoryRemoteAddressNV");
#if defined(VK_USE_PLATFORM_SCI)
dispatchTable.GetMemorySciBufNV = (PFN_vkGetMemorySciBufNV)gdpa(*pDevice, "vkGetMemorySciBufNV");
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
dispatchTable.GetSemaphoreWin32HandleKHR = (PFN_vkGetSemaphoreWin32HandleKHR)gdpa(*pDevice, "vkGetSemaphoreWin32HandleKHR");
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
dispatchTable.ImportSemaphoreWin32HandleKHR = (PFN_vkImportSemaphoreWin32HandleKHR)gdpa(*pDevice, "vkImportSemaphoreWin32HandleKHR");
#endif
dispatchTable.GetSemaphoreFdKHR = (PFN_vkGetSemaphoreFdKHR)gdpa(*pDevice, "vkGetSemaphoreFdKHR");
dispatchTable.ImportSemaphoreFdKHR = (PFN_vkImportSemaphoreFdKHR)gdpa(*pDevice, "vkImportSemaphoreFdKHR");
#if defined(VK_USE_PLATFORM_FUCHSIA)
dispatchTable.GetSemaphoreZirconHandleFUCHSIA = (PFN_vkGetSemaphoreZirconHandleFUCHSIA)gdpa(*pDevice, "vkGetSemaphoreZirconHandleFUCHSIA");
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
dispatchTable.ImportSemaphoreZirconHandleFUCHSIA = (PFN_vkImportSemaphoreZirconHandleFUCHSIA)gdpa(*pDevice, "vkImportSemaphoreZirconHandleFUCHSIA");
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
dispatchTable.GetFenceWin32HandleKHR = (PFN_vkGetFenceWin32HandleKHR)gdpa(*pDevice, "vkGetFenceWin32HandleKHR");
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
dispatchTable.ImportFenceWin32HandleKHR = (PFN_vkImportFenceWin32HandleKHR)gdpa(*pDevice, "vkImportFenceWin32HandleKHR");
#endif
dispatchTable.GetFenceFdKHR = (PFN_vkGetFenceFdKHR)gdpa(*pDevice, "vkGetFenceFdKHR");
dispatchTable.ImportFenceFdKHR = (PFN_vkImportFenceFdKHR)gdpa(*pDevice, "vkImportFenceFdKHR");
#if defined(VK_USE_PLATFORM_SCI)
dispatchTable.GetFenceSciSyncFenceNV = (PFN_vkGetFenceSciSyncFenceNV)gdpa(*pDevice, "vkGetFenceSciSyncFenceNV");
#endif
#if defined(VK_USE_PLATFORM_SCI)
dispatchTable.GetFenceSciSyncObjNV = (PFN_vkGetFenceSciSyncObjNV)gdpa(*pDevice, "vkGetFenceSciSyncObjNV");
#endif
#if defined(VK_USE_PLATFORM_SCI)
dispatchTable.ImportFenceSciSyncFenceNV = (PFN_vkImportFenceSciSyncFenceNV)gdpa(*pDevice, "vkImportFenceSciSyncFenceNV");
#endif
#if defined(VK_USE_PLATFORM_SCI)
dispatchTable.ImportFenceSciSyncObjNV = (PFN_vkImportFenceSciSyncObjNV)gdpa(*pDevice, "vkImportFenceSciSyncObjNV");
#endif
#if defined(VK_USE_PLATFORM_SCI)
dispatchTable.GetSemaphoreSciSyncObjNV = (PFN_vkGetSemaphoreSciSyncObjNV)gdpa(*pDevice, "vkGetSemaphoreSciSyncObjNV");
#endif
#if defined(VK_USE_PLATFORM_SCI)
dispatchTable.ImportSemaphoreSciSyncObjNV = (PFN_vkImportSemaphoreSciSyncObjNV)gdpa(*pDevice, "vkImportSemaphoreSciSyncObjNV");
#endif
#if defined(VK_USE_PLATFORM_SCI)
dispatchTable.CreateSemaphoreSciSyncPoolNV = (PFN_vkCreateSemaphoreSciSyncPoolNV)gdpa(*pDevice, "vkCreateSemaphoreSciSyncPoolNV");
#endif
#if defined(VK_USE_PLATFORM_SCI)
dispatchTable.DestroySemaphoreSciSyncPoolNV = (PFN_vkDestroySemaphoreSciSyncPoolNV)gdpa(*pDevice, "vkDestroySemaphoreSciSyncPoolNV");
#endif
dispatchTable.DisplayPowerControlEXT = (PFN_vkDisplayPowerControlEXT)gdpa(*pDevice, "vkDisplayPowerControlEXT");
dispatchTable.RegisterDeviceEventEXT = (PFN_vkRegisterDeviceEventEXT)gdpa(*pDevice, "vkRegisterDeviceEventEXT");
dispatchTable.RegisterDisplayEventEXT = (PFN_vkRegisterDisplayEventEXT)gdpa(*pDevice, "vkRegisterDisplayEventEXT");
dispatchTable.GetSwapchainCounterEXT = (PFN_vkGetSwapchainCounterEXT)gdpa(*pDevice, "vkGetSwapchainCounterEXT");
dispatchTable.GetDeviceGroupPeerMemoryFeatures = (PFN_vkGetDeviceGroupPeerMemoryFeatures)gdpa(*pDevice, "vkGetDeviceGroupPeerMemoryFeatures");
dispatchTable.BindBufferMemory2 = (PFN_vkBindBufferMemory2)gdpa(*pDevice, "vkBindBufferMemory2");
dispatchTable.BindImageMemory2 = (PFN_vkBindImageMemory2)gdpa(*pDevice, "vkBindImageMemory2");
dispatchTable.CmdSetDeviceMask = (PFN_vkCmdSetDeviceMask)gdpa(*pDevice, "vkCmdSetDeviceMask");
dispatchTable.GetDeviceGroupPresentCapabilitiesKHR = (PFN_vkGetDeviceGroupPresentCapabilitiesKHR)gdpa(*pDevice, "vkGetDeviceGroupPresentCapabilitiesKHR");
dispatchTable.GetDeviceGroupSurfacePresentModesKHR = (PFN_vkGetDeviceGroupSurfacePresentModesKHR)gdpa(*pDevice, "vkGetDeviceGroupSurfacePresentModesKHR");
dispatchTable.AcquireNextImage2KHR = (PFN_vkAcquireNextImage2KHR)gdpa(*pDevice, "vkAcquireNextImage2KHR");
dispatchTable.CmdDispatchBase = (PFN_vkCmdDispatchBase)gdpa(*pDevice, "vkCmdDispatchBase");
dispatchTable.CreateDescriptorUpdateTemplate = (PFN_vkCreateDescriptorUpdateTemplate)gdpa(*pDevice, "vkCreateDescriptorUpdateTemplate");
dispatchTable.DestroyDescriptorUpdateTemplate = (PFN_vkDestroyDescriptorUpdateTemplate)gdpa(*pDevice, "vkDestroyDescriptorUpdateTemplate");
dispatchTable.UpdateDescriptorSetWithTemplate = (PFN_vkUpdateDescriptorSetWithTemplate)gdpa(*pDevice, "vkUpdateDescriptorSetWithTemplate");
dispatchTable.CmdPushDescriptorSetWithTemplateKHR = (PFN_vkCmdPushDescriptorSetWithTemplateKHR)gdpa(*pDevice, "vkCmdPushDescriptorSetWithTemplateKHR");
dispatchTable.SetHdrMetadataEXT = (PFN_vkSetHdrMetadataEXT)gdpa(*pDevice, "vkSetHdrMetadataEXT");
dispatchTable.GetSwapchainStatusKHR = (PFN_vkGetSwapchainStatusKHR)gdpa(*pDevice, "vkGetSwapchainStatusKHR");
dispatchTable.GetRefreshCycleDurationGOOGLE = (PFN_vkGetRefreshCycleDurationGOOGLE)gdpa(*pDevice, "vkGetRefreshCycleDurationGOOGLE");
dispatchTable.GetPastPresentationTimingGOOGLE = (PFN_vkGetPastPresentationTimingGOOGLE)gdpa(*pDevice, "vkGetPastPresentationTimingGOOGLE");
dispatchTable.CmdSetViewportWScalingNV = (PFN_vkCmdSetViewportWScalingNV)gdpa(*pDevice, "vkCmdSetViewportWScalingNV");
dispatchTable.CmdSetDiscardRectangleEXT = (PFN_vkCmdSetDiscardRectangleEXT)gdpa(*pDevice, "vkCmdSetDiscardRectangleEXT");
dispatchTable.CmdSetDiscardRectangleEnableEXT = (PFN_vkCmdSetDiscardRectangleEnableEXT)gdpa(*pDevice, "vkCmdSetDiscardRectangleEnableEXT");
dispatchTable.CmdSetDiscardRectangleModeEXT = (PFN_vkCmdSetDiscardRectangleModeEXT)gdpa(*pDevice, "vkCmdSetDiscardRectangleModeEXT");
dispatchTable.CmdSetSampleLocationsEXT = (PFN_vkCmdSetSampleLocationsEXT)gdpa(*pDevice, "vkCmdSetSampleLocationsEXT");
dispatchTable.GetBufferMemoryRequirements2 = (PFN_vkGetBufferMemoryRequirements2)gdpa(*pDevice, "vkGetBufferMemoryRequirements2");
dispatchTable.GetImageMemoryRequirements2 = (PFN_vkGetImageMemoryRequirements2)gdpa(*pDevice, "vkGetImageMemoryRequirements2");
dispatchTable.GetImageSparseMemoryRequirements2 = (PFN_vkGetImageSparseMemoryRequirements2)gdpa(*pDevice, "vkGetImageSparseMemoryRequirements2");
dispatchTable.GetDeviceBufferMemoryRequirements = (PFN_vkGetDeviceBufferMemoryRequirements)gdpa(*pDevice, "vkGetDeviceBufferMemoryRequirements");
dispatchTable.GetDeviceImageMemoryRequirements = (PFN_vkGetDeviceImageMemoryRequirements)gdpa(*pDevice, "vkGetDeviceImageMemoryRequirements");
dispatchTable.GetDeviceImageSparseMemoryRequirements = (PFN_vkGetDeviceImageSparseMemoryRequirements)gdpa(*pDevice, "vkGetDeviceImageSparseMemoryRequirements");
dispatchTable.CreateSamplerYcbcrConversion = (PFN_vkCreateSamplerYcbcrConversion)gdpa(*pDevice, "vkCreateSamplerYcbcrConversion");
dispatchTable.DestroySamplerYcbcrConversion = (PFN_vkDestroySamplerYcbcrConversion)gdpa(*pDevice, "vkDestroySamplerYcbcrConversion");
dispatchTable.GetDeviceQueue2 = (PFN_vkGetDeviceQueue2)gdpa(*pDevice, "vkGetDeviceQueue2");
dispatchTable.CreateValidationCacheEXT = (PFN_vkCreateValidationCacheEXT)gdpa(*pDevice, "vkCreateValidationCacheEXT");
dispatchTable.DestroyValidationCacheEXT = (PFN_vkDestroyValidationCacheEXT)gdpa(*pDevice, "vkDestroyValidationCacheEXT");
dispatchTable.GetValidationCacheDataEXT = (PFN_vkGetValidationCacheDataEXT)gdpa(*pDevice, "vkGetValidationCacheDataEXT");
dispatchTable.MergeValidationCachesEXT = (PFN_vkMergeValidationCachesEXT)gdpa(*pDevice, "vkMergeValidationCachesEXT");
dispatchTable.GetDescriptorSetLayoutSupport = (PFN_vkGetDescriptorSetLayoutSupport)gdpa(*pDevice, "vkGetDescriptorSetLayoutSupport");
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
dispatchTable.GetSwapchainGrallocUsageANDROID = (PFN_vkGetSwapchainGrallocUsageANDROID)gdpa(*pDevice, "vkGetSwapchainGrallocUsageANDROID");
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
dispatchTable.GetSwapchainGrallocUsage2ANDROID = (PFN_vkGetSwapchainGrallocUsage2ANDROID)gdpa(*pDevice, "vkGetSwapchainGrallocUsage2ANDROID");
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
dispatchTable.AcquireImageANDROID = (PFN_vkAcquireImageANDROID)gdpa(*pDevice, "vkAcquireImageANDROID");
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
dispatchTable.QueueSignalReleaseImageANDROID = (PFN_vkQueueSignalReleaseImageANDROID)gdpa(*pDevice, "vkQueueSignalReleaseImageANDROID");
#endif
dispatchTable.GetShaderInfoAMD = (PFN_vkGetShaderInfoAMD)gdpa(*pDevice, "vkGetShaderInfoAMD");
dispatchTable.SetLocalDimmingAMD = (PFN_vkSetLocalDimmingAMD)gdpa(*pDevice, "vkSetLocalDimmingAMD");
dispatchTable.GetCalibratedTimestampsKHR = (PFN_vkGetCalibratedTimestampsKHR)gdpa(*pDevice, "vkGetCalibratedTimestampsKHR");
dispatchTable.SetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)gdpa(*pDevice, "vkSetDebugUtilsObjectNameEXT");
dispatchTable.SetDebugUtilsObjectTagEXT = (PFN_vkSetDebugUtilsObjectTagEXT)gdpa(*pDevice, "vkSetDebugUtilsObjectTagEXT");
dispatchTable.QueueBeginDebugUtilsLabelEXT = (PFN_vkQueueBeginDebugUtilsLabelEXT)gdpa(*pDevice, "vkQueueBeginDebugUtilsLabelEXT");
dispatchTable.QueueEndDebugUtilsLabelEXT = (PFN_vkQueueEndDebugUtilsLabelEXT)gdpa(*pDevice, "vkQueueEndDebugUtilsLabelEXT");
dispatchTable.QueueInsertDebugUtilsLabelEXT = (PFN_vkQueueInsertDebugUtilsLabelEXT)gdpa(*pDevice, "vkQueueInsertDebugUtilsLabelEXT");
dispatchTable.CmdBeginDebugUtilsLabelEXT = (PFN_vkCmdBeginDebugUtilsLabelEXT)gdpa(*pDevice, "vkCmdBeginDebugUtilsLabelEXT");
dispatchTable.CmdEndDebugUtilsLabelEXT = (PFN_vkCmdEndDebugUtilsLabelEXT)gdpa(*pDevice, "vkCmdEndDebugUtilsLabelEXT");
dispatchTable.CmdInsertDebugUtilsLabelEXT = (PFN_vkCmdInsertDebugUtilsLabelEXT)gdpa(*pDevice, "vkCmdInsertDebugUtilsLabelEXT");
dispatchTable.GetMemoryHostPointerPropertiesEXT = (PFN_vkGetMemoryHostPointerPropertiesEXT)gdpa(*pDevice, "vkGetMemoryHostPointerPropertiesEXT");
dispatchTable.CmdWriteBufferMarkerAMD = (PFN_vkCmdWriteBufferMarkerAMD)gdpa(*pDevice, "vkCmdWriteBufferMarkerAMD");
dispatchTable.CreateRenderPass2 = (PFN_vkCreateRenderPass2)gdpa(*pDevice, "vkCreateRenderPass2");
dispatchTable.CmdBeginRenderPass2 = (PFN_vkCmdBeginRenderPass2)gdpa(*pDevice, "vkCmdBeginRenderPass2");
dispatchTable.CmdNextSubpass2 = (PFN_vkCmdNextSubpass2)gdpa(*pDevice, "vkCmdNextSubpass2");
dispatchTable.CmdEndRenderPass2 = (PFN_vkCmdEndRenderPass2)gdpa(*pDevice, "vkCmdEndRenderPass2");
dispatchTable.GetSemaphoreCounterValue = (PFN_vkGetSemaphoreCounterValue)gdpa(*pDevice, "vkGetSemaphoreCounterValue");
dispatchTable.WaitSemaphores = (PFN_vkWaitSemaphores)gdpa(*pDevice, "vkWaitSemaphores");
dispatchTable.SignalSemaphore = (PFN_vkSignalSemaphore)gdpa(*pDevice, "vkSignalSemaphore");
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
dispatchTable.GetAndroidHardwareBufferPropertiesANDROID = (PFN_vkGetAndroidHardwareBufferPropertiesANDROID)gdpa(*pDevice, "vkGetAndroidHardwareBufferPropertiesANDROID");
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
dispatchTable.GetMemoryAndroidHardwareBufferANDROID = (PFN_vkGetMemoryAndroidHardwareBufferANDROID)gdpa(*pDevice, "vkGetMemoryAndroidHardwareBufferANDROID");
#endif
dispatchTable.CmdDrawIndirectCount = (PFN_vkCmdDrawIndirectCount)gdpa(*pDevice, "vkCmdDrawIndirectCount");
dispatchTable.CmdDrawIndexedIndirectCount = (PFN_vkCmdDrawIndexedIndirectCount)gdpa(*pDevice, "vkCmdDrawIndexedIndirectCount");
dispatchTable.CmdSetCheckpointNV = (PFN_vkCmdSetCheckpointNV)gdpa(*pDevice, "vkCmdSetCheckpointNV");
dispatchTable.GetQueueCheckpointDataNV = (PFN_vkGetQueueCheckpointDataNV)gdpa(*pDevice, "vkGetQueueCheckpointDataNV");
dispatchTable.CmdBindTransformFeedbackBuffersEXT = (PFN_vkCmdBindTransformFeedbackBuffersEXT)gdpa(*pDevice, "vkCmdBindTransformFeedbackBuffersEXT");
dispatchTable.CmdBeginTransformFeedbackEXT = (PFN_vkCmdBeginTransformFeedbackEXT)gdpa(*pDevice, "vkCmdBeginTransformFeedbackEXT");
dispatchTable.CmdEndTransformFeedbackEXT = (PFN_vkCmdEndTransformFeedbackEXT)gdpa(*pDevice, "vkCmdEndTransformFeedbackEXT");
dispatchTable.CmdBeginQueryIndexedEXT = (PFN_vkCmdBeginQueryIndexedEXT)gdpa(*pDevice, "vkCmdBeginQueryIndexedEXT");
dispatchTable.CmdEndQueryIndexedEXT = (PFN_vkCmdEndQueryIndexedEXT)gdpa(*pDevice, "vkCmdEndQueryIndexedEXT");
dispatchTable.CmdDrawIndirectByteCountEXT = (PFN_vkCmdDrawIndirectByteCountEXT)gdpa(*pDevice, "vkCmdDrawIndirectByteCountEXT");
dispatchTable.CmdSetExclusiveScissorNV = (PFN_vkCmdSetExclusiveScissorNV)gdpa(*pDevice, "vkCmdSetExclusiveScissorNV");
dispatchTable.CmdSetExclusiveScissorEnableNV = (PFN_vkCmdSetExclusiveScissorEnableNV)gdpa(*pDevice, "vkCmdSetExclusiveScissorEnableNV");
dispatchTable.CmdBindShadingRateImageNV = (PFN_vkCmdBindShadingRateImageNV)gdpa(*pDevice, "vkCmdBindShadingRateImageNV");
dispatchTable.CmdSetViewportShadingRatePaletteNV = (PFN_vkCmdSetViewportShadingRatePaletteNV)gdpa(*pDevice, "vkCmdSetViewportShadingRatePaletteNV");
dispatchTable.CmdSetCoarseSampleOrderNV = (PFN_vkCmdSetCoarseSampleOrderNV)gdpa(*pDevice, "vkCmdSetCoarseSampleOrderNV");
dispatchTable.CmdDrawMeshTasksNV = (PFN_vkCmdDrawMeshTasksNV)gdpa(*pDevice, "vkCmdDrawMeshTasksNV");
dispatchTable.CmdDrawMeshTasksIndirectNV = (PFN_vkCmdDrawMeshTasksIndirectNV)gdpa(*pDevice, "vkCmdDrawMeshTasksIndirectNV");
dispatchTable.CmdDrawMeshTasksIndirectCountNV = (PFN_vkCmdDrawMeshTasksIndirectCountNV)gdpa(*pDevice, "vkCmdDrawMeshTasksIndirectCountNV");
dispatchTable.CmdDrawMeshTasksEXT = (PFN_vkCmdDrawMeshTasksEXT)gdpa(*pDevice, "vkCmdDrawMeshTasksEXT");
dispatchTable.CmdDrawMeshTasksIndirectEXT = (PFN_vkCmdDrawMeshTasksIndirectEXT)gdpa(*pDevice, "vkCmdDrawMeshTasksIndirectEXT");
dispatchTable.CmdDrawMeshTasksIndirectCountEXT = (PFN_vkCmdDrawMeshTasksIndirectCountEXT)gdpa(*pDevice, "vkCmdDrawMeshTasksIndirectCountEXT");
dispatchTable.CompileDeferredNV = (PFN_vkCompileDeferredNV)gdpa(*pDevice, "vkCompileDeferredNV");
dispatchTable.CreateAccelerationStructureNV = (PFN_vkCreateAccelerationStructureNV)gdpa(*pDevice, "vkCreateAccelerationStructureNV");
dispatchTable.CmdBindInvocationMaskHUAWEI = (PFN_vkCmdBindInvocationMaskHUAWEI)gdpa(*pDevice, "vkCmdBindInvocationMaskHUAWEI");
dispatchTable.DestroyAccelerationStructureKHR = (PFN_vkDestroyAccelerationStructureKHR)gdpa(*pDevice, "vkDestroyAccelerationStructureKHR");
dispatchTable.DestroyAccelerationStructureNV = (PFN_vkDestroyAccelerationStructureNV)gdpa(*pDevice, "vkDestroyAccelerationStructureNV");
dispatchTable.GetAccelerationStructureMemoryRequirementsNV = (PFN_vkGetAccelerationStructureMemoryRequirementsNV)gdpa(*pDevice, "vkGetAccelerationStructureMemoryRequirementsNV");
dispatchTable.BindAccelerationStructureMemoryNV = (PFN_vkBindAccelerationStructureMemoryNV)gdpa(*pDevice, "vkBindAccelerationStructureMemoryNV");
dispatchTable.CmdCopyAccelerationStructureNV = (PFN_vkCmdCopyAccelerationStructureNV)gdpa(*pDevice, "vkCmdCopyAccelerationStructureNV");
dispatchTable.CmdCopyAccelerationStructureKHR = (PFN_vkCmdCopyAccelerationStructureKHR)gdpa(*pDevice, "vkCmdCopyAccelerationStructureKHR");
dispatchTable.CopyAccelerationStructureKHR = (PFN_vkCopyAccelerationStructureKHR)gdpa(*pDevice, "vkCopyAccelerationStructureKHR");
dispatchTable.CmdCopyAccelerationStructureToMemoryKHR = (PFN_vkCmdCopyAccelerationStructureToMemoryKHR)gdpa(*pDevice, "vkCmdCopyAccelerationStructureToMemoryKHR");
dispatchTable.CopyAccelerationStructureToMemoryKHR = (PFN_vkCopyAccelerationStructureToMemoryKHR)gdpa(*pDevice, "vkCopyAccelerationStructureToMemoryKHR");
dispatchTable.CmdCopyMemoryToAccelerationStructureKHR = (PFN_vkCmdCopyMemoryToAccelerationStructureKHR)gdpa(*pDevice, "vkCmdCopyMemoryToAccelerationStructureKHR");
dispatchTable.CopyMemoryToAccelerationStructureKHR = (PFN_vkCopyMemoryToAccelerationStructureKHR)gdpa(*pDevice, "vkCopyMemoryToAccelerationStructureKHR");
dispatchTable.CmdWriteAccelerationStructuresPropertiesKHR = (PFN_vkCmdWriteAccelerationStructuresPropertiesKHR)gdpa(*pDevice, "vkCmdWriteAccelerationStructuresPropertiesKHR");
dispatchTable.CmdWriteAccelerationStructuresPropertiesNV = (PFN_vkCmdWriteAccelerationStructuresPropertiesNV)gdpa(*pDevice, "vkCmdWriteAccelerationStructuresPropertiesNV");
dispatchTable.CmdBuildAccelerationStructureNV = (PFN_vkCmdBuildAccelerationStructureNV)gdpa(*pDevice, "vkCmdBuildAccelerationStructureNV");
dispatchTable.WriteAccelerationStructuresPropertiesKHR = (PFN_vkWriteAccelerationStructuresPropertiesKHR)gdpa(*pDevice, "vkWriteAccelerationStructuresPropertiesKHR");
dispatchTable.CmdTraceRaysKHR = (PFN_vkCmdTraceRaysKHR)gdpa(*pDevice, "vkCmdTraceRaysKHR");
dispatchTable.CmdTraceRaysNV = (PFN_vkCmdTraceRaysNV)gdpa(*pDevice, "vkCmdTraceRaysNV");
dispatchTable.GetRayTracingShaderGroupHandlesKHR = (PFN_vkGetRayTracingShaderGroupHandlesKHR)gdpa(*pDevice, "vkGetRayTracingShaderGroupHandlesKHR");
dispatchTable.GetRayTracingCaptureReplayShaderGroupHandlesKHR = (PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR)gdpa(*pDevice, "vkGetRayTracingCaptureReplayShaderGroupHandlesKHR");
dispatchTable.GetAccelerationStructureHandleNV = (PFN_vkGetAccelerationStructureHandleNV)gdpa(*pDevice, "vkGetAccelerationStructureHandleNV");
dispatchTable.CreateRayTracingPipelinesNV = (PFN_vkCreateRayTracingPipelinesNV)gdpa(*pDevice, "vkCreateRayTracingPipelinesNV");
dispatchTable.CreateRayTracingPipelinesKHR = (PFN_vkCreateRayTracingPipelinesKHR)gdpa(*pDevice, "vkCreateRayTracingPipelinesKHR");
dispatchTable.CmdTraceRaysIndirectKHR = (PFN_vkCmdTraceRaysIndirectKHR)gdpa(*pDevice, "vkCmdTraceRaysIndirectKHR");
dispatchTable.CmdTraceRaysIndirect2KHR = (PFN_vkCmdTraceRaysIndirect2KHR)gdpa(*pDevice, "vkCmdTraceRaysIndirect2KHR");
dispatchTable.GetDeviceAccelerationStructureCompatibilityKHR = (PFN_vkGetDeviceAccelerationStructureCompatibilityKHR)gdpa(*pDevice, "vkGetDeviceAccelerationStructureCompatibilityKHR");
dispatchTable.GetRayTracingShaderGroupStackSizeKHR = (PFN_vkGetRayTracingShaderGroupStackSizeKHR)gdpa(*pDevice, "vkGetRayTracingShaderGroupStackSizeKHR");
dispatchTable.CmdSetRayTracingPipelineStackSizeKHR = (PFN_vkCmdSetRayTracingPipelineStackSizeKHR)gdpa(*pDevice, "vkCmdSetRayTracingPipelineStackSizeKHR");
dispatchTable.GetImageViewHandleNVX = (PFN_vkGetImageViewHandleNVX)gdpa(*pDevice, "vkGetImageViewHandleNVX");
dispatchTable.GetImageViewAddressNVX = (PFN_vkGetImageViewAddressNVX)gdpa(*pDevice, "vkGetImageViewAddressNVX");
#if defined(VK_USE_PLATFORM_WIN32_KHR)
dispatchTable.GetDeviceGroupSurfacePresentModes2EXT = (PFN_vkGetDeviceGroupSurfacePresentModes2EXT)gdpa(*pDevice, "vkGetDeviceGroupSurfacePresentModes2EXT");
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
dispatchTable.AcquireFullScreenExclusiveModeEXT = (PFN_vkAcquireFullScreenExclusiveModeEXT)gdpa(*pDevice, "vkAcquireFullScreenExclusiveModeEXT");
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
dispatchTable.ReleaseFullScreenExclusiveModeEXT = (PFN_vkReleaseFullScreenExclusiveModeEXT)gdpa(*pDevice, "vkReleaseFullScreenExclusiveModeEXT");
#endif
dispatchTable.AcquireProfilingLockKHR = (PFN_vkAcquireProfilingLockKHR)gdpa(*pDevice, "vkAcquireProfilingLockKHR");
dispatchTable.ReleaseProfilingLockKHR = (PFN_vkReleaseProfilingLockKHR)gdpa(*pDevice, "vkReleaseProfilingLockKHR");
dispatchTable.GetImageDrmFormatModifierPropertiesEXT = (PFN_vkGetImageDrmFormatModifierPropertiesEXT)gdpa(*pDevice, "vkGetImageDrmFormatModifierPropertiesEXT");
dispatchTable.GetBufferOpaqueCaptureAddress = (PFN_vkGetBufferOpaqueCaptureAddress)gdpa(*pDevice, "vkGetBufferOpaqueCaptureAddress");
dispatchTable.GetBufferDeviceAddress = (PFN_vkGetBufferDeviceAddress)gdpa(*pDevice, "vkGetBufferDeviceAddress");
dispatchTable.InitializePerformanceApiINTEL = (PFN_vkInitializePerformanceApiINTEL)gdpa(*pDevice, "vkInitializePerformanceApiINTEL");
dispatchTable.UninitializePerformanceApiINTEL = (PFN_vkUninitializePerformanceApiINTEL)gdpa(*pDevice, "vkUninitializePerformanceApiINTEL");
dispatchTable.CmdSetPerformanceMarkerINTEL = (PFN_vkCmdSetPerformanceMarkerINTEL)gdpa(*pDevice, "vkCmdSetPerformanceMarkerINTEL");
dispatchTable.CmdSetPerformanceStreamMarkerINTEL = (PFN_vkCmdSetPerformanceStreamMarkerINTEL)gdpa(*pDevice, "vkCmdSetPerformanceStreamMarkerINTEL");
dispatchTable.CmdSetPerformanceOverrideINTEL = (PFN_vkCmdSetPerformanceOverrideINTEL)gdpa(*pDevice, "vkCmdSetPerformanceOverrideINTEL");
dispatchTable.AcquirePerformanceConfigurationINTEL = (PFN_vkAcquirePerformanceConfigurationINTEL)gdpa(*pDevice, "vkAcquirePerformanceConfigurationINTEL");
dispatchTable.ReleasePerformanceConfigurationINTEL = (PFN_vkReleasePerformanceConfigurationINTEL)gdpa(*pDevice, "vkReleasePerformanceConfigurationINTEL");
dispatchTable.QueueSetPerformanceConfigurationINTEL = (PFN_vkQueueSetPerformanceConfigurationINTEL)gdpa(*pDevice, "vkQueueSetPerformanceConfigurationINTEL");
dispatchTable.GetPerformanceParameterINTEL = (PFN_vkGetPerformanceParameterINTEL)gdpa(*pDevice, "vkGetPerformanceParameterINTEL");
dispatchTable.GetDeviceMemoryOpaqueCaptureAddress = (PFN_vkGetDeviceMemoryOpaqueCaptureAddress)gdpa(*pDevice, "vkGetDeviceMemoryOpaqueCaptureAddress");
dispatchTable.GetPipelineExecutablePropertiesKHR = (PFN_vkGetPipelineExecutablePropertiesKHR)gdpa(*pDevice, "vkGetPipelineExecutablePropertiesKHR");
dispatchTable.GetPipelineExecutableStatisticsKHR = (PFN_vkGetPipelineExecutableStatisticsKHR)gdpa(*pDevice, "vkGetPipelineExecutableStatisticsKHR");
dispatchTable.GetPipelineExecutableInternalRepresentationsKHR = (PFN_vkGetPipelineExecutableInternalRepresentationsKHR)gdpa(*pDevice, "vkGetPipelineExecutableInternalRepresentationsKHR");
dispatchTable.CreateAccelerationStructureKHR = (PFN_vkCreateAccelerationStructureKHR)gdpa(*pDevice, "vkCreateAccelerationStructureKHR");
dispatchTable.CmdBuildAccelerationStructuresKHR = (PFN_vkCmdBuildAccelerationStructuresKHR)gdpa(*pDevice, "vkCmdBuildAccelerationStructuresKHR");
dispatchTable.CmdBuildAccelerationStructuresIndirectKHR = (PFN_vkCmdBuildAccelerationStructuresIndirectKHR)gdpa(*pDevice, "vkCmdBuildAccelerationStructuresIndirectKHR");
dispatchTable.BuildAccelerationStructuresKHR = (PFN_vkBuildAccelerationStructuresKHR)gdpa(*pDevice, "vkBuildAccelerationStructuresKHR");
dispatchTable.GetAccelerationStructureDeviceAddressKHR = (PFN_vkGetAccelerationStructureDeviceAddressKHR)gdpa(*pDevice, "vkGetAccelerationStructureDeviceAddressKHR");
dispatchTable.CreateDeferredOperationKHR = (PFN_vkCreateDeferredOperationKHR)gdpa(*pDevice, "vkCreateDeferredOperationKHR");
dispatchTable.DestroyDeferredOperationKHR = (PFN_vkDestroyDeferredOperationKHR)gdpa(*pDevice, "vkDestroyDeferredOperationKHR");
dispatchTable.GetDeferredOperationMaxConcurrencyKHR = (PFN_vkGetDeferredOperationMaxConcurrencyKHR)gdpa(*pDevice, "vkGetDeferredOperationMaxConcurrencyKHR");
dispatchTable.GetDeferredOperationResultKHR = (PFN_vkGetDeferredOperationResultKHR)gdpa(*pDevice, "vkGetDeferredOperationResultKHR");
dispatchTable.DeferredOperationJoinKHR = (PFN_vkDeferredOperationJoinKHR)gdpa(*pDevice, "vkDeferredOperationJoinKHR");
dispatchTable.GetPipelineIndirectMemoryRequirementsNV = (PFN_vkGetPipelineIndirectMemoryRequirementsNV)gdpa(*pDevice, "vkGetPipelineIndirectMemoryRequirementsNV");
dispatchTable.GetPipelineIndirectDeviceAddressNV = (PFN_vkGetPipelineIndirectDeviceAddressNV)gdpa(*pDevice, "vkGetPipelineIndirectDeviceAddressNV");
dispatchTable.CmdSetCullMode = (PFN_vkCmdSetCullMode)gdpa(*pDevice, "vkCmdSetCullMode");
dispatchTable.CmdSetFrontFace = (PFN_vkCmdSetFrontFace)gdpa(*pDevice, "vkCmdSetFrontFace");
dispatchTable.CmdSetPrimitiveTopology = (PFN_vkCmdSetPrimitiveTopology)gdpa(*pDevice, "vkCmdSetPrimitiveTopology");
dispatchTable.CmdSetViewportWithCount = (PFN_vkCmdSetViewportWithCount)gdpa(*pDevice, "vkCmdSetViewportWithCount");
dispatchTable.CmdSetScissorWithCount = (PFN_vkCmdSetScissorWithCount)gdpa(*pDevice, "vkCmdSetScissorWithCount");
dispatchTable.CmdBindIndexBuffer2KHR = (PFN_vkCmdBindIndexBuffer2KHR)gdpa(*pDevice, "vkCmdBindIndexBuffer2KHR");
dispatchTable.CmdBindVertexBuffers2 = (PFN_vkCmdBindVertexBuffers2)gdpa(*pDevice, "vkCmdBindVertexBuffers2");
dispatchTable.CmdSetDepthTestEnable = (PFN_vkCmdSetDepthTestEnable)gdpa(*pDevice, "vkCmdSetDepthTestEnable");
dispatchTable.CmdSetDepthWriteEnable = (PFN_vkCmdSetDepthWriteEnable)gdpa(*pDevice, "vkCmdSetDepthWriteEnable");
dispatchTable.CmdSetDepthCompareOp = (PFN_vkCmdSetDepthCompareOp)gdpa(*pDevice, "vkCmdSetDepthCompareOp");
dispatchTable.CmdSetDepthBoundsTestEnable = (PFN_vkCmdSetDepthBoundsTestEnable)gdpa(*pDevice, "vkCmdSetDepthBoundsTestEnable");
dispatchTable.CmdSetStencilTestEnable = (PFN_vkCmdSetStencilTestEnable)gdpa(*pDevice, "vkCmdSetStencilTestEnable");
dispatchTable.CmdSetStencilOp = (PFN_vkCmdSetStencilOp)gdpa(*pDevice, "vkCmdSetStencilOp");
dispatchTable.CmdSetPatchControlPointsEXT = (PFN_vkCmdSetPatchControlPointsEXT)gdpa(*pDevice, "vkCmdSetPatchControlPointsEXT");
dispatchTable.CmdSetRasterizerDiscardEnable = (PFN_vkCmdSetRasterizerDiscardEnable)gdpa(*pDevice, "vkCmdSetRasterizerDiscardEnable");
dispatchTable.CmdSetDepthBiasEnable = (PFN_vkCmdSetDepthBiasEnable)gdpa(*pDevice, "vkCmdSetDepthBiasEnable");
dispatchTable.CmdSetLogicOpEXT = (PFN_vkCmdSetLogicOpEXT)gdpa(*pDevice, "vkCmdSetLogicOpEXT");
dispatchTable.CmdSetPrimitiveRestartEnable = (PFN_vkCmdSetPrimitiveRestartEnable)gdpa(*pDevice, "vkCmdSetPrimitiveRestartEnable");
dispatchTable.CmdSetTessellationDomainOriginEXT = (PFN_vkCmdSetTessellationDomainOriginEXT)gdpa(*pDevice, "vkCmdSetTessellationDomainOriginEXT");
dispatchTable.CmdSetDepthClampEnableEXT = (PFN_vkCmdSetDepthClampEnableEXT)gdpa(*pDevice, "vkCmdSetDepthClampEnableEXT");
dispatchTable.CmdSetPolygonModeEXT = (PFN_vkCmdSetPolygonModeEXT)gdpa(*pDevice, "vkCmdSetPolygonModeEXT");
dispatchTable.CmdSetRasterizationSamplesEXT = (PFN_vkCmdSetRasterizationSamplesEXT)gdpa(*pDevice, "vkCmdSetRasterizationSamplesEXT");
dispatchTable.CmdSetSampleMaskEXT = (PFN_vkCmdSetSampleMaskEXT)gdpa(*pDevice, "vkCmdSetSampleMaskEXT");
dispatchTable.CmdSetAlphaToCoverageEnableEXT = (PFN_vkCmdSetAlphaToCoverageEnableEXT)gdpa(*pDevice, "vkCmdSetAlphaToCoverageEnableEXT");
dispatchTable.CmdSetAlphaToOneEnableEXT = (PFN_vkCmdSetAlphaToOneEnableEXT)gdpa(*pDevice, "vkCmdSetAlphaToOneEnableEXT");
dispatchTable.CmdSetLogicOpEnableEXT = (PFN_vkCmdSetLogicOpEnableEXT)gdpa(*pDevice, "vkCmdSetLogicOpEnableEXT");
dispatchTable.CmdSetColorBlendEnableEXT = (PFN_vkCmdSetColorBlendEnableEXT)gdpa(*pDevice, "vkCmdSetColorBlendEnableEXT");
dispatchTable.CmdSetColorBlendEquationEXT = (PFN_vkCmdSetColorBlendEquationEXT)gdpa(*pDevice, "vkCmdSetColorBlendEquationEXT");
dispatchTable.CmdSetColorWriteMaskEXT = (PFN_vkCmdSetColorWriteMaskEXT)gdpa(*pDevice, "vkCmdSetColorWriteMaskEXT");
dispatchTable.CmdSetRasterizationStreamEXT = (PFN_vkCmdSetRasterizationStreamEXT)gdpa(*pDevice, "vkCmdSetRasterizationStreamEXT");
dispatchTable.CmdSetConservativeRasterizationModeEXT = (PFN_vkCmdSetConservativeRasterizationModeEXT)gdpa(*pDevice, "vkCmdSetConservativeRasterizationModeEXT");
dispatchTable.CmdSetExtraPrimitiveOverestimationSizeEXT = (PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT)gdpa(*pDevice, "vkCmdSetExtraPrimitiveOverestimationSizeEXT");
dispatchTable.CmdSetDepthClipEnableEXT = (PFN_vkCmdSetDepthClipEnableEXT)gdpa(*pDevice, "vkCmdSetDepthClipEnableEXT");
dispatchTable.CmdSetSampleLocationsEnableEXT = (PFN_vkCmdSetSampleLocationsEnableEXT)gdpa(*pDevice, "vkCmdSetSampleLocationsEnableEXT");
dispatchTable.CmdSetColorBlendAdvancedEXT = (PFN_vkCmdSetColorBlendAdvancedEXT)gdpa(*pDevice, "vkCmdSetColorBlendAdvancedEXT");
dispatchTable.CmdSetProvokingVertexModeEXT = (PFN_vkCmdSetProvokingVertexModeEXT)gdpa(*pDevice, "vkCmdSetProvokingVertexModeEXT");
dispatchTable.CmdSetLineRasterizationModeEXT = (PFN_vkCmdSetLineRasterizationModeEXT)gdpa(*pDevice, "vkCmdSetLineRasterizationModeEXT");
dispatchTable.CmdSetLineStippleEnableEXT = (PFN_vkCmdSetLineStippleEnableEXT)gdpa(*pDevice, "vkCmdSetLineStippleEnableEXT");
dispatchTable.CmdSetDepthClipNegativeOneToOneEXT = (PFN_vkCmdSetDepthClipNegativeOneToOneEXT)gdpa(*pDevice, "vkCmdSetDepthClipNegativeOneToOneEXT");
dispatchTable.CmdSetViewportWScalingEnableNV = (PFN_vkCmdSetViewportWScalingEnableNV)gdpa(*pDevice, "vkCmdSetViewportWScalingEnableNV");
dispatchTable.CmdSetViewportSwizzleNV = (PFN_vkCmdSetViewportSwizzleNV)gdpa(*pDevice, "vkCmdSetViewportSwizzleNV");
dispatchTable.CmdSetCoverageToColorEnableNV = (PFN_vkCmdSetCoverageToColorEnableNV)gdpa(*pDevice, "vkCmdSetCoverageToColorEnableNV");
dispatchTable.CmdSetCoverageToColorLocationNV = (PFN_vkCmdSetCoverageToColorLocationNV)gdpa(*pDevice, "vkCmdSetCoverageToColorLocationNV");
dispatchTable.CmdSetCoverageModulationModeNV = (PFN_vkCmdSetCoverageModulationModeNV)gdpa(*pDevice, "vkCmdSetCoverageModulationModeNV");
dispatchTable.CmdSetCoverageModulationTableEnableNV = (PFN_vkCmdSetCoverageModulationTableEnableNV)gdpa(*pDevice, "vkCmdSetCoverageModulationTableEnableNV");
dispatchTable.CmdSetCoverageModulationTableNV = (PFN_vkCmdSetCoverageModulationTableNV)gdpa(*pDevice, "vkCmdSetCoverageModulationTableNV");
dispatchTable.CmdSetShadingRateImageEnableNV = (PFN_vkCmdSetShadingRateImageEnableNV)gdpa(*pDevice, "vkCmdSetShadingRateImageEnableNV");
dispatchTable.CmdSetCoverageReductionModeNV = (PFN_vkCmdSetCoverageReductionModeNV)gdpa(*pDevice, "vkCmdSetCoverageReductionModeNV");
dispatchTable.CmdSetRepresentativeFragmentTestEnableNV = (PFN_vkCmdSetRepresentativeFragmentTestEnableNV)gdpa(*pDevice, "vkCmdSetRepresentativeFragmentTestEnableNV");
dispatchTable.CreatePrivateDataSlot = (PFN_vkCreatePrivateDataSlot)gdpa(*pDevice, "vkCreatePrivateDataSlot");
dispatchTable.DestroyPrivateDataSlot = (PFN_vkDestroyPrivateDataSlot)gdpa(*pDevice, "vkDestroyPrivateDataSlot");
dispatchTable.SetPrivateData = (PFN_vkSetPrivateData)gdpa(*pDevice, "vkSetPrivateData");
dispatchTable.GetPrivateData = (PFN_vkGetPrivateData)gdpa(*pDevice, "vkGetPrivateData");
dispatchTable.CmdCopyBuffer2 = (PFN_vkCmdCopyBuffer2)gdpa(*pDevice, "vkCmdCopyBuffer2");
dispatchTable.CmdCopyImage2 = (PFN_vkCmdCopyImage2)gdpa(*pDevice, "vkCmdCopyImage2");
dispatchTable.CmdBlitImage2 = (PFN_vkCmdBlitImage2)gdpa(*pDevice, "vkCmdBlitImage2");
dispatchTable.CmdCopyBufferToImage2 = (PFN_vkCmdCopyBufferToImage2)gdpa(*pDevice, "vkCmdCopyBufferToImage2");
dispatchTable.CmdCopyImageToBuffer2 = (PFN_vkCmdCopyImageToBuffer2)gdpa(*pDevice, "vkCmdCopyImageToBuffer2");
dispatchTable.CmdResolveImage2 = (PFN_vkCmdResolveImage2)gdpa(*pDevice, "vkCmdResolveImage2");
dispatchTable.CmdSetFragmentShadingRateKHR = (PFN_vkCmdSetFragmentShadingRateKHR)gdpa(*pDevice, "vkCmdSetFragmentShadingRateKHR");
dispatchTable.CmdSetFragmentShadingRateEnumNV = (PFN_vkCmdSetFragmentShadingRateEnumNV)gdpa(*pDevice, "vkCmdSetFragmentShadingRateEnumNV");
dispatchTable.GetAccelerationStructureBuildSizesKHR = (PFN_vkGetAccelerationStructureBuildSizesKHR)gdpa(*pDevice, "vkGetAccelerationStructureBuildSizesKHR");
dispatchTable.CmdSetVertexInputEXT = (PFN_vkCmdSetVertexInputEXT)gdpa(*pDevice, "vkCmdSetVertexInputEXT");
dispatchTable.CmdSetColorWriteEnableEXT = (PFN_vkCmdSetColorWriteEnableEXT)gdpa(*pDevice, "vkCmdSetColorWriteEnableEXT");
dispatchTable.CmdSetEvent2 = (PFN_vkCmdSetEvent2)gdpa(*pDevice, "vkCmdSetEvent2");
dispatchTable.CmdResetEvent2 = (PFN_vkCmdResetEvent2)gdpa(*pDevice, "vkCmdResetEvent2");
dispatchTable.CmdWaitEvents2 = (PFN_vkCmdWaitEvents2)gdpa(*pDevice, "vkCmdWaitEvents2");
dispatchTable.CmdPipelineBarrier2 = (PFN_vkCmdPipelineBarrier2)gdpa(*pDevice, "vkCmdPipelineBarrier2");
dispatchTable.QueueSubmit2 = (PFN_vkQueueSubmit2)gdpa(*pDevice, "vkQueueSubmit2");
dispatchTable.CmdWriteTimestamp2 = (PFN_vkCmdWriteTimestamp2)gdpa(*pDevice, "vkCmdWriteTimestamp2");
dispatchTable.CmdWriteBufferMarker2AMD = (PFN_vkCmdWriteBufferMarker2AMD)gdpa(*pDevice, "vkCmdWriteBufferMarker2AMD");
dispatchTable.GetQueueCheckpointData2NV = (PFN_vkGetQueueCheckpointData2NV)gdpa(*pDevice, "vkGetQueueCheckpointData2NV");
dispatchTable.CopyMemoryToImageEXT = (PFN_vkCopyMemoryToImageEXT)gdpa(*pDevice, "vkCopyMemoryToImageEXT");
dispatchTable.CopyImageToMemoryEXT = (PFN_vkCopyImageToMemoryEXT)gdpa(*pDevice, "vkCopyImageToMemoryEXT");
dispatchTable.CopyImageToImageEXT = (PFN_vkCopyImageToImageEXT)gdpa(*pDevice, "vkCopyImageToImageEXT");
dispatchTable.TransitionImageLayoutEXT = (PFN_vkTransitionImageLayoutEXT)gdpa(*pDevice, "vkTransitionImageLayoutEXT");
dispatchTable.CreateVideoSessionKHR = (PFN_vkCreateVideoSessionKHR)gdpa(*pDevice, "vkCreateVideoSessionKHR");
dispatchTable.DestroyVideoSessionKHR = (PFN_vkDestroyVideoSessionKHR)gdpa(*pDevice, "vkDestroyVideoSessionKHR");
dispatchTable.CreateVideoSessionParametersKHR = (PFN_vkCreateVideoSessionParametersKHR)gdpa(*pDevice, "vkCreateVideoSessionParametersKHR");
dispatchTable.UpdateVideoSessionParametersKHR = (PFN_vkUpdateVideoSessionParametersKHR)gdpa(*pDevice, "vkUpdateVideoSessionParametersKHR");
dispatchTable.GetEncodedVideoSessionParametersKHR = (PFN_vkGetEncodedVideoSessionParametersKHR)gdpa(*pDevice, "vkGetEncodedVideoSessionParametersKHR");
dispatchTable.DestroyVideoSessionParametersKHR = (PFN_vkDestroyVideoSessionParametersKHR)gdpa(*pDevice, "vkDestroyVideoSessionParametersKHR");
dispatchTable.GetVideoSessionMemoryRequirementsKHR = (PFN_vkGetVideoSessionMemoryRequirementsKHR)gdpa(*pDevice, "vkGetVideoSessionMemoryRequirementsKHR");
dispatchTable.BindVideoSessionMemoryKHR = (PFN_vkBindVideoSessionMemoryKHR)gdpa(*pDevice, "vkBindVideoSessionMemoryKHR");
dispatchTable.CmdDecodeVideoKHR = (PFN_vkCmdDecodeVideoKHR)gdpa(*pDevice, "vkCmdDecodeVideoKHR");
dispatchTable.CmdBeginVideoCodingKHR = (PFN_vkCmdBeginVideoCodingKHR)gdpa(*pDevice, "vkCmdBeginVideoCodingKHR");
dispatchTable.CmdControlVideoCodingKHR = (PFN_vkCmdControlVideoCodingKHR)gdpa(*pDevice, "vkCmdControlVideoCodingKHR");
dispatchTable.CmdEndVideoCodingKHR = (PFN_vkCmdEndVideoCodingKHR)gdpa(*pDevice, "vkCmdEndVideoCodingKHR");
dispatchTable.CmdEncodeVideoKHR = (PFN_vkCmdEncodeVideoKHR)gdpa(*pDevice, "vkCmdEncodeVideoKHR");
dispatchTable.CmdDecompressMemoryNV = (PFN_vkCmdDecompressMemoryNV)gdpa(*pDevice, "vkCmdDecompressMemoryNV");
dispatchTable.CmdDecompressMemoryIndirectCountNV = (PFN_vkCmdDecompressMemoryIndirectCountNV)gdpa(*pDevice, "vkCmdDecompressMemoryIndirectCountNV");
dispatchTable.CreateCuModuleNVX = (PFN_vkCreateCuModuleNVX)gdpa(*pDevice, "vkCreateCuModuleNVX");
dispatchTable.CreateCuFunctionNVX = (PFN_vkCreateCuFunctionNVX)gdpa(*pDevice, "vkCreateCuFunctionNVX");
dispatchTable.DestroyCuModuleNVX = (PFN_vkDestroyCuModuleNVX)gdpa(*pDevice, "vkDestroyCuModuleNVX");
dispatchTable.DestroyCuFunctionNVX = (PFN_vkDestroyCuFunctionNVX)gdpa(*pDevice, "vkDestroyCuFunctionNVX");
dispatchTable.CmdCuLaunchKernelNVX = (PFN_vkCmdCuLaunchKernelNVX)gdpa(*pDevice, "vkCmdCuLaunchKernelNVX");
dispatchTable.GetDescriptorSetLayoutSizeEXT = (PFN_vkGetDescriptorSetLayoutSizeEXT)gdpa(*pDevice, "vkGetDescriptorSetLayoutSizeEXT");
dispatchTable.GetDescriptorSetLayoutBindingOffsetEXT = (PFN_vkGetDescriptorSetLayoutBindingOffsetEXT)gdpa(*pDevice, "vkGetDescriptorSetLayoutBindingOffsetEXT");
dispatchTable.GetDescriptorEXT = (PFN_vkGetDescriptorEXT)gdpa(*pDevice, "vkGetDescriptorEXT");
dispatchTable.CmdBindDescriptorBuffersEXT = (PFN_vkCmdBindDescriptorBuffersEXT)gdpa(*pDevice, "vkCmdBindDescriptorBuffersEXT");
dispatchTable.CmdSetDescriptorBufferOffsetsEXT = (PFN_vkCmdSetDescriptorBufferOffsetsEXT)gdpa(*pDevice, "vkCmdSetDescriptorBufferOffsetsEXT");
dispatchTable.CmdBindDescriptorBufferEmbeddedSamplersEXT = (PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT)gdpa(*pDevice, "vkCmdBindDescriptorBufferEmbeddedSamplersEXT");
dispatchTable.GetBufferOpaqueCaptureDescriptorDataEXT = (PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT)gdpa(*pDevice, "vkGetBufferOpaqueCaptureDescriptorDataEXT");
dispatchTable.GetImageOpaqueCaptureDescriptorDataEXT = (PFN_vkGetImageOpaqueCaptureDescriptorDataEXT)gdpa(*pDevice, "vkGetImageOpaqueCaptureDescriptorDataEXT");
dispatchTable.GetImageViewOpaqueCaptureDescriptorDataEXT = (PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT)gdpa(*pDevice, "vkGetImageViewOpaqueCaptureDescriptorDataEXT");
dispatchTable.GetSamplerOpaqueCaptureDescriptorDataEXT = (PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT)gdpa(*pDevice, "vkGetSamplerOpaqueCaptureDescriptorDataEXT");
dispatchTable.GetAccelerationStructureOpaqueCaptureDescriptorDataEXT = (PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT)gdpa(*pDevice, "vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT");
dispatchTable.SetDeviceMemoryPriorityEXT = (PFN_vkSetDeviceMemoryPriorityEXT)gdpa(*pDevice, "vkSetDeviceMemoryPriorityEXT");
dispatchTable.WaitForPresentKHR = (PFN_vkWaitForPresentKHR)gdpa(*pDevice, "vkWaitForPresentKHR");
#if defined(VK_USE_PLATFORM_FUCHSIA)
dispatchTable.CreateBufferCollectionFUCHSIA = (PFN_vkCreateBufferCollectionFUCHSIA)gdpa(*pDevice, "vkCreateBufferCollectionFUCHSIA");
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
dispatchTable.SetBufferCollectionBufferConstraintsFUCHSIA = (PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA)gdpa(*pDevice, "vkSetBufferCollectionBufferConstraintsFUCHSIA");
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
dispatchTable.SetBufferCollectionImageConstraintsFUCHSIA = (PFN_vkSetBufferCollectionImageConstraintsFUCHSIA)gdpa(*pDevice, "vkSetBufferCollectionImageConstraintsFUCHSIA");
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
dispatchTable.DestroyBufferCollectionFUCHSIA = (PFN_vkDestroyBufferCollectionFUCHSIA)gdpa(*pDevice, "vkDestroyBufferCollectionFUCHSIA");
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
dispatchTable.GetBufferCollectionPropertiesFUCHSIA = (PFN_vkGetBufferCollectionPropertiesFUCHSIA)gdpa(*pDevice, "vkGetBufferCollectionPropertiesFUCHSIA");
#endif
dispatchTable.CreateCudaModuleNV = (PFN_vkCreateCudaModuleNV)gdpa(*pDevice, "vkCreateCudaModuleNV");
dispatchTable.GetCudaModuleCacheNV = (PFN_vkGetCudaModuleCacheNV)gdpa(*pDevice, "vkGetCudaModuleCacheNV");
dispatchTable.CreateCudaFunctionNV = (PFN_vkCreateCudaFunctionNV)gdpa(*pDevice, "vkCreateCudaFunctionNV");
dispatchTable.DestroyCudaModuleNV = (PFN_vkDestroyCudaModuleNV)gdpa(*pDevice, "vkDestroyCudaModuleNV");
dispatchTable.DestroyCudaFunctionNV = (PFN_vkDestroyCudaFunctionNV)gdpa(*pDevice, "vkDestroyCudaFunctionNV");
dispatchTable.CmdCudaLaunchKernelNV = (PFN_vkCmdCudaLaunchKernelNV)gdpa(*pDevice, "vkCmdCudaLaunchKernelNV");
dispatchTable.CmdBeginRendering = (PFN_vkCmdBeginRendering)gdpa(*pDevice, "vkCmdBeginRendering");
dispatchTable.CmdEndRendering = (PFN_vkCmdEndRendering)gdpa(*pDevice, "vkCmdEndRendering");
dispatchTable.GetDescriptorSetLayoutHostMappingInfoVALVE = (PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE)gdpa(*pDevice, "vkGetDescriptorSetLayoutHostMappingInfoVALVE");
dispatchTable.GetDescriptorSetHostMappingVALVE = (PFN_vkGetDescriptorSetHostMappingVALVE)gdpa(*pDevice, "vkGetDescriptorSetHostMappingVALVE");
dispatchTable.CreateMicromapEXT = (PFN_vkCreateMicromapEXT)gdpa(*pDevice, "vkCreateMicromapEXT");
dispatchTable.CmdBuildMicromapsEXT = (PFN_vkCmdBuildMicromapsEXT)gdpa(*pDevice, "vkCmdBuildMicromapsEXT");
dispatchTable.BuildMicromapsEXT = (PFN_vkBuildMicromapsEXT)gdpa(*pDevice, "vkBuildMicromapsEXT");
dispatchTable.DestroyMicromapEXT = (PFN_vkDestroyMicromapEXT)gdpa(*pDevice, "vkDestroyMicromapEXT");
dispatchTable.CmdCopyMicromapEXT = (PFN_vkCmdCopyMicromapEXT)gdpa(*pDevice, "vkCmdCopyMicromapEXT");
dispatchTable.CopyMicromapEXT = (PFN_vkCopyMicromapEXT)gdpa(*pDevice, "vkCopyMicromapEXT");
dispatchTable.CmdCopyMicromapToMemoryEXT = (PFN_vkCmdCopyMicromapToMemoryEXT)gdpa(*pDevice, "vkCmdCopyMicromapToMemoryEXT");
dispatchTable.CopyMicromapToMemoryEXT = (PFN_vkCopyMicromapToMemoryEXT)gdpa(*pDevice, "vkCopyMicromapToMemoryEXT");
dispatchTable.CmdCopyMemoryToMicromapEXT = (PFN_vkCmdCopyMemoryToMicromapEXT)gdpa(*pDevice, "vkCmdCopyMemoryToMicromapEXT");
dispatchTable.CopyMemoryToMicromapEXT = (PFN_vkCopyMemoryToMicromapEXT)gdpa(*pDevice, "vkCopyMemoryToMicromapEXT");
dispatchTable.CmdWriteMicromapsPropertiesEXT = (PFN_vkCmdWriteMicromapsPropertiesEXT)gdpa(*pDevice, "vkCmdWriteMicromapsPropertiesEXT");
dispatchTable.WriteMicromapsPropertiesEXT = (PFN_vkWriteMicromapsPropertiesEXT)gdpa(*pDevice, "vkWriteMicromapsPropertiesEXT");
dispatchTable.GetDeviceMicromapCompatibilityEXT = (PFN_vkGetDeviceMicromapCompatibilityEXT)gdpa(*pDevice, "vkGetDeviceMicromapCompatibilityEXT");
dispatchTable.GetMicromapBuildSizesEXT = (PFN_vkGetMicromapBuildSizesEXT)gdpa(*pDevice, "vkGetMicromapBuildSizesEXT");
dispatchTable.GetShaderModuleIdentifierEXT = (PFN_vkGetShaderModuleIdentifierEXT)gdpa(*pDevice, "vkGetShaderModuleIdentifierEXT");
dispatchTable.GetShaderModuleCreateInfoIdentifierEXT = (PFN_vkGetShaderModuleCreateInfoIdentifierEXT)gdpa(*pDevice, "vkGetShaderModuleCreateInfoIdentifierEXT");
dispatchTable.GetImageSubresourceLayout2KHR = (PFN_vkGetImageSubresourceLayout2KHR)gdpa(*pDevice, "vkGetImageSubresourceLayout2KHR");
dispatchTable.GetPipelinePropertiesEXT = (PFN_vkGetPipelinePropertiesEXT)gdpa(*pDevice, "vkGetPipelinePropertiesEXT");
#if defined(VK_USE_PLATFORM_METAL_EXT)
dispatchTable.ExportMetalObjectsEXT = (PFN_vkExportMetalObjectsEXT)gdpa(*pDevice, "vkExportMetalObjectsEXT");
#endif
dispatchTable.GetFramebufferTilePropertiesQCOM = (PFN_vkGetFramebufferTilePropertiesQCOM)gdpa(*pDevice, "vkGetFramebufferTilePropertiesQCOM");
dispatchTable.GetDynamicRenderingTilePropertiesQCOM = (PFN_vkGetDynamicRenderingTilePropertiesQCOM)gdpa(*pDevice, "vkGetDynamicRenderingTilePropertiesQCOM");
dispatchTable.CreateOpticalFlowSessionNV = (PFN_vkCreateOpticalFlowSessionNV)gdpa(*pDevice, "vkCreateOpticalFlowSessionNV");
dispatchTable.DestroyOpticalFlowSessionNV = (PFN_vkDestroyOpticalFlowSessionNV)gdpa(*pDevice, "vkDestroyOpticalFlowSessionNV");
dispatchTable.BindOpticalFlowSessionImageNV = (PFN_vkBindOpticalFlowSessionImageNV)gdpa(*pDevice, "vkBindOpticalFlowSessionImageNV");
dispatchTable.CmdOpticalFlowExecuteNV = (PFN_vkCmdOpticalFlowExecuteNV)gdpa(*pDevice, "vkCmdOpticalFlowExecuteNV");
dispatchTable.GetDeviceFaultInfoEXT = (PFN_vkGetDeviceFaultInfoEXT)gdpa(*pDevice, "vkGetDeviceFaultInfoEXT");
dispatchTable.CmdSetDepthBias2EXT = (PFN_vkCmdSetDepthBias2EXT)gdpa(*pDevice, "vkCmdSetDepthBias2EXT");
dispatchTable.ReleaseSwapchainImagesEXT = (PFN_vkReleaseSwapchainImagesEXT)gdpa(*pDevice, "vkReleaseSwapchainImagesEXT");
dispatchTable.GetDeviceImageSubresourceLayoutKHR = (PFN_vkGetDeviceImageSubresourceLayoutKHR)gdpa(*pDevice, "vkGetDeviceImageSubresourceLayoutKHR");
dispatchTable.MapMemory2KHR = (PFN_vkMapMemory2KHR)gdpa(*pDevice, "vkMapMemory2KHR");
dispatchTable.UnmapMemory2KHR = (PFN_vkUnmapMemory2KHR)gdpa(*pDevice, "vkUnmapMemory2KHR");
dispatchTable.CreateShadersEXT = (PFN_vkCreateShadersEXT)gdpa(*pDevice, "vkCreateShadersEXT");
dispatchTable.DestroyShaderEXT = (PFN_vkDestroyShaderEXT)gdpa(*pDevice, "vkDestroyShaderEXT");
dispatchTable.GetShaderBinaryDataEXT = (PFN_vkGetShaderBinaryDataEXT)gdpa(*pDevice, "vkGetShaderBinaryDataEXT");
dispatchTable.CmdBindShadersEXT = (PFN_vkCmdBindShadersEXT)gdpa(*pDevice, "vkCmdBindShadersEXT");
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
dispatchTable.GetScreenBufferPropertiesQNX = (PFN_vkGetScreenBufferPropertiesQNX)gdpa(*pDevice, "vkGetScreenBufferPropertiesQNX");
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
dispatchTable.GetExecutionGraphPipelineScratchSizeAMDX = (PFN_vkGetExecutionGraphPipelineScratchSizeAMDX)gdpa(*pDevice, "vkGetExecutionGraphPipelineScratchSizeAMDX");
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
dispatchTable.GetExecutionGraphPipelineNodeIndexAMDX = (PFN_vkGetExecutionGraphPipelineNodeIndexAMDX)gdpa(*pDevice, "vkGetExecutionGraphPipelineNodeIndexAMDX");
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
dispatchTable.CreateExecutionGraphPipelinesAMDX = (PFN_vkCreateExecutionGraphPipelinesAMDX)gdpa(*pDevice, "vkCreateExecutionGraphPipelinesAMDX");
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
dispatchTable.CmdInitializeGraphScratchMemoryAMDX = (PFN_vkCmdInitializeGraphScratchMemoryAMDX)gdpa(*pDevice, "vkCmdInitializeGraphScratchMemoryAMDX");
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
dispatchTable.CmdDispatchGraphAMDX = (PFN_vkCmdDispatchGraphAMDX)gdpa(*pDevice, "vkCmdDispatchGraphAMDX");
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
dispatchTable.CmdDispatchGraphIndirectAMDX = (PFN_vkCmdDispatchGraphIndirectAMDX)gdpa(*pDevice, "vkCmdDispatchGraphIndirectAMDX");
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
dispatchTable.CmdDispatchGraphIndirectCountAMDX = (PFN_vkCmdDispatchGraphIndirectCountAMDX)gdpa(*pDevice, "vkCmdDispatchGraphIndirectCountAMDX");
#endif
dispatchTable.CmdBindDescriptorSets2KHR = (PFN_vkCmdBindDescriptorSets2KHR)gdpa(*pDevice, "vkCmdBindDescriptorSets2KHR");
dispatchTable.CmdPushConstants2KHR = (PFN_vkCmdPushConstants2KHR)gdpa(*pDevice, "vkCmdPushConstants2KHR");
dispatchTable.CmdPushDescriptorSet2KHR = (PFN_vkCmdPushDescriptorSet2KHR)gdpa(*pDevice, "vkCmdPushDescriptorSet2KHR");
dispatchTable.CmdPushDescriptorSetWithTemplate2KHR = (PFN_vkCmdPushDescriptorSetWithTemplate2KHR)gdpa(*pDevice, "vkCmdPushDescriptorSetWithTemplate2KHR");
dispatchTable.CmdSetDescriptorBufferOffsets2EXT = (PFN_vkCmdSetDescriptorBufferOffsets2EXT)gdpa(*pDevice, "vkCmdSetDescriptorBufferOffsets2EXT");
dispatchTable.CmdBindDescriptorBufferEmbeddedSamplers2EXT = (PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT)gdpa(*pDevice, "vkCmdBindDescriptorBufferEmbeddedSamplers2EXT");
dispatchTable.SetLatencySleepModeNV = (PFN_vkSetLatencySleepModeNV)gdpa(*pDevice, "vkSetLatencySleepModeNV");
dispatchTable.LatencySleepNV = (PFN_vkLatencySleepNV)gdpa(*pDevice, "vkLatencySleepNV");
dispatchTable.SetLatencyMarkerNV = (PFN_vkSetLatencyMarkerNV)gdpa(*pDevice, "vkSetLatencyMarkerNV");
dispatchTable.GetLatencyTimingsNV = (PFN_vkGetLatencyTimingsNV)gdpa(*pDevice, "vkGetLatencyTimingsNV");
dispatchTable.QueueNotifyOutOfBandNV = (PFN_vkQueueNotifyOutOfBandNV)gdpa(*pDevice, "vkQueueNotifyOutOfBandNV");
dispatchTable.ResetQueryPoolEXT = (PFN_vkResetQueryPoolEXT)gdpa(*pDevice, "vkResetQueryPoolEXT");
dispatchTable.TrimCommandPoolKHR = (PFN_vkTrimCommandPoolKHR)gdpa(*pDevice, "vkTrimCommandPoolKHR");
dispatchTable.GetDeviceGroupPeerMemoryFeaturesKHR = (PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR)gdpa(*pDevice, "vkGetDeviceGroupPeerMemoryFeaturesKHR");
dispatchTable.BindBufferMemory2KHR = (PFN_vkBindBufferMemory2KHR)gdpa(*pDevice, "vkBindBufferMemory2KHR");
dispatchTable.BindImageMemory2KHR = (PFN_vkBindImageMemory2KHR)gdpa(*pDevice, "vkBindImageMemory2KHR");
dispatchTable.CmdSetDeviceMaskKHR = (PFN_vkCmdSetDeviceMaskKHR)gdpa(*pDevice, "vkCmdSetDeviceMaskKHR");
dispatchTable.CmdDispatchBaseKHR = (PFN_vkCmdDispatchBaseKHR)gdpa(*pDevice, "vkCmdDispatchBaseKHR");
dispatchTable.CreateDescriptorUpdateTemplateKHR = (PFN_vkCreateDescriptorUpdateTemplateKHR)gdpa(*pDevice, "vkCreateDescriptorUpdateTemplateKHR");
dispatchTable.DestroyDescriptorUpdateTemplateKHR = (PFN_vkDestroyDescriptorUpdateTemplateKHR)gdpa(*pDevice, "vkDestroyDescriptorUpdateTemplateKHR");
dispatchTable.UpdateDescriptorSetWithTemplateKHR = (PFN_vkUpdateDescriptorSetWithTemplateKHR)gdpa(*pDevice, "vkUpdateDescriptorSetWithTemplateKHR");
dispatchTable.GetBufferMemoryRequirements2KHR = (PFN_vkGetBufferMemoryRequirements2KHR)gdpa(*pDevice, "vkGetBufferMemoryRequirements2KHR");
dispatchTable.GetImageMemoryRequirements2KHR = (PFN_vkGetImageMemoryRequirements2KHR)gdpa(*pDevice, "vkGetImageMemoryRequirements2KHR");
dispatchTable.GetImageSparseMemoryRequirements2KHR = (PFN_vkGetImageSparseMemoryRequirements2KHR)gdpa(*pDevice, "vkGetImageSparseMemoryRequirements2KHR");
dispatchTable.GetDeviceBufferMemoryRequirementsKHR = (PFN_vkGetDeviceBufferMemoryRequirementsKHR)gdpa(*pDevice, "vkGetDeviceBufferMemoryRequirementsKHR");
dispatchTable.GetDeviceImageMemoryRequirementsKHR = (PFN_vkGetDeviceImageMemoryRequirementsKHR)gdpa(*pDevice, "vkGetDeviceImageMemoryRequirementsKHR");
dispatchTable.GetDeviceImageSparseMemoryRequirementsKHR = (PFN_vkGetDeviceImageSparseMemoryRequirementsKHR)gdpa(*pDevice, "vkGetDeviceImageSparseMemoryRequirementsKHR");
dispatchTable.CreateSamplerYcbcrConversionKHR = (PFN_vkCreateSamplerYcbcrConversionKHR)gdpa(*pDevice, "vkCreateSamplerYcbcrConversionKHR");
dispatchTable.DestroySamplerYcbcrConversionKHR = (PFN_vkDestroySamplerYcbcrConversionKHR)gdpa(*pDevice, "vkDestroySamplerYcbcrConversionKHR");
dispatchTable.GetDescriptorSetLayoutSupportKHR = (PFN_vkGetDescriptorSetLayoutSupportKHR)gdpa(*pDevice, "vkGetDescriptorSetLayoutSupportKHR");
dispatchTable.GetCalibratedTimestampsEXT = (PFN_vkGetCalibratedTimestampsEXT)gdpa(*pDevice, "vkGetCalibratedTimestampsEXT");
dispatchTable.CreateRenderPass2KHR = (PFN_vkCreateRenderPass2KHR)gdpa(*pDevice, "vkCreateRenderPass2KHR");
dispatchTable.CmdBeginRenderPass2KHR = (PFN_vkCmdBeginRenderPass2KHR)gdpa(*pDevice, "vkCmdBeginRenderPass2KHR");
dispatchTable.CmdNextSubpass2KHR = (PFN_vkCmdNextSubpass2KHR)gdpa(*pDevice, "vkCmdNextSubpass2KHR");
dispatchTable.CmdEndRenderPass2KHR = (PFN_vkCmdEndRenderPass2KHR)gdpa(*pDevice, "vkCmdEndRenderPass2KHR");
dispatchTable.GetSemaphoreCounterValueKHR = (PFN_vkGetSemaphoreCounterValueKHR)gdpa(*pDevice, "vkGetSemaphoreCounterValueKHR");
dispatchTable.WaitSemaphoresKHR = (PFN_vkWaitSemaphoresKHR)gdpa(*pDevice, "vkWaitSemaphoresKHR");
dispatchTable.SignalSemaphoreKHR = (PFN_vkSignalSemaphoreKHR)gdpa(*pDevice, "vkSignalSemaphoreKHR");
dispatchTable.CmdDrawIndirectCountKHR = (PFN_vkCmdDrawIndirectCountKHR)gdpa(*pDevice, "vkCmdDrawIndirectCountKHR");
dispatchTable.CmdDrawIndirectCountAMD = (PFN_vkCmdDrawIndirectCountAMD)gdpa(*pDevice, "vkCmdDrawIndirectCountAMD");
dispatchTable.CmdDrawIndexedIndirectCountKHR = (PFN_vkCmdDrawIndexedIndirectCountKHR)gdpa(*pDevice, "vkCmdDrawIndexedIndirectCountKHR");
dispatchTable.CmdDrawIndexedIndirectCountAMD = (PFN_vkCmdDrawIndexedIndirectCountAMD)gdpa(*pDevice, "vkCmdDrawIndexedIndirectCountAMD");
dispatchTable.GetRayTracingShaderGroupHandlesNV = (PFN_vkGetRayTracingShaderGroupHandlesNV)gdpa(*pDevice, "vkGetRayTracingShaderGroupHandlesNV");
dispatchTable.GetBufferOpaqueCaptureAddressKHR = (PFN_vkGetBufferOpaqueCaptureAddressKHR)gdpa(*pDevice, "vkGetBufferOpaqueCaptureAddressKHR");
dispatchTable.GetBufferDeviceAddressKHR = (PFN_vkGetBufferDeviceAddressKHR)gdpa(*pDevice, "vkGetBufferDeviceAddressKHR");
dispatchTable.GetBufferDeviceAddressEXT = (PFN_vkGetBufferDeviceAddressEXT)gdpa(*pDevice, "vkGetBufferDeviceAddressEXT");
dispatchTable.GetDeviceMemoryOpaqueCaptureAddressKHR = (PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR)gdpa(*pDevice, "vkGetDeviceMemoryOpaqueCaptureAddressKHR");
dispatchTable.CmdSetLineStippleEXT = (PFN_vkCmdSetLineStippleEXT)gdpa(*pDevice, "vkCmdSetLineStippleEXT");
dispatchTable.CmdSetCullModeEXT = (PFN_vkCmdSetCullModeEXT)gdpa(*pDevice, "vkCmdSetCullModeEXT");
dispatchTable.CmdSetFrontFaceEXT = (PFN_vkCmdSetFrontFaceEXT)gdpa(*pDevice, "vkCmdSetFrontFaceEXT");
dispatchTable.CmdSetPrimitiveTopologyEXT = (PFN_vkCmdSetPrimitiveTopologyEXT)gdpa(*pDevice, "vkCmdSetPrimitiveTopologyEXT");
dispatchTable.CmdSetViewportWithCountEXT = (PFN_vkCmdSetViewportWithCountEXT)gdpa(*pDevice, "vkCmdSetViewportWithCountEXT");
dispatchTable.CmdSetScissorWithCountEXT = (PFN_vkCmdSetScissorWithCountEXT)gdpa(*pDevice, "vkCmdSetScissorWithCountEXT");
dispatchTable.CmdBindVertexBuffers2EXT = (PFN_vkCmdBindVertexBuffers2EXT)gdpa(*pDevice, "vkCmdBindVertexBuffers2EXT");
dispatchTable.CmdSetDepthTestEnableEXT = (PFN_vkCmdSetDepthTestEnableEXT)gdpa(*pDevice, "vkCmdSetDepthTestEnableEXT");
dispatchTable.CmdSetDepthWriteEnableEXT = (PFN_vkCmdSetDepthWriteEnableEXT)gdpa(*pDevice, "vkCmdSetDepthWriteEnableEXT");
dispatchTable.CmdSetDepthCompareOpEXT = (PFN_vkCmdSetDepthCompareOpEXT)gdpa(*pDevice, "vkCmdSetDepthCompareOpEXT");
dispatchTable.CmdSetDepthBoundsTestEnableEXT = (PFN_vkCmdSetDepthBoundsTestEnableEXT)gdpa(*pDevice, "vkCmdSetDepthBoundsTestEnableEXT");
dispatchTable.CmdSetStencilTestEnableEXT = (PFN_vkCmdSetStencilTestEnableEXT)gdpa(*pDevice, "vkCmdSetStencilTestEnableEXT");
dispatchTable.CmdSetStencilOpEXT = (PFN_vkCmdSetStencilOpEXT)gdpa(*pDevice, "vkCmdSetStencilOpEXT");
dispatchTable.CmdSetRasterizerDiscardEnableEXT = (PFN_vkCmdSetRasterizerDiscardEnableEXT)gdpa(*pDevice, "vkCmdSetRasterizerDiscardEnableEXT");
dispatchTable.CmdSetDepthBiasEnableEXT = (PFN_vkCmdSetDepthBiasEnableEXT)gdpa(*pDevice, "vkCmdSetDepthBiasEnableEXT");
dispatchTable.CmdSetPrimitiveRestartEnableEXT = (PFN_vkCmdSetPrimitiveRestartEnableEXT)gdpa(*pDevice, "vkCmdSetPrimitiveRestartEnableEXT");
dispatchTable.CreatePrivateDataSlotEXT = (PFN_vkCreatePrivateDataSlotEXT)gdpa(*pDevice, "vkCreatePrivateDataSlotEXT");
dispatchTable.DestroyPrivateDataSlotEXT = (PFN_vkDestroyPrivateDataSlotEXT)gdpa(*pDevice, "vkDestroyPrivateDataSlotEXT");
dispatchTable.SetPrivateDataEXT = (PFN_vkSetPrivateDataEXT)gdpa(*pDevice, "vkSetPrivateDataEXT");
dispatchTable.GetPrivateDataEXT = (PFN_vkGetPrivateDataEXT)gdpa(*pDevice, "vkGetPrivateDataEXT");
dispatchTable.CmdCopyBuffer2KHR = (PFN_vkCmdCopyBuffer2KHR)gdpa(*pDevice, "vkCmdCopyBuffer2KHR");
dispatchTable.CmdCopyImage2KHR = (PFN_vkCmdCopyImage2KHR)gdpa(*pDevice, "vkCmdCopyImage2KHR");
dispatchTable.CmdBlitImage2KHR = (PFN_vkCmdBlitImage2KHR)gdpa(*pDevice, "vkCmdBlitImage2KHR");
dispatchTable.CmdCopyBufferToImage2KHR = (PFN_vkCmdCopyBufferToImage2KHR)gdpa(*pDevice, "vkCmdCopyBufferToImage2KHR");
dispatchTable.CmdCopyImageToBuffer2KHR = (PFN_vkCmdCopyImageToBuffer2KHR)gdpa(*pDevice, "vkCmdCopyImageToBuffer2KHR");
dispatchTable.CmdResolveImage2KHR = (PFN_vkCmdResolveImage2KHR)gdpa(*pDevice, "vkCmdResolveImage2KHR");
dispatchTable.CmdSetEvent2KHR = (PFN_vkCmdSetEvent2KHR)gdpa(*pDevice, "vkCmdSetEvent2KHR");
dispatchTable.CmdResetEvent2KHR = (PFN_vkCmdResetEvent2KHR)gdpa(*pDevice, "vkCmdResetEvent2KHR");
dispatchTable.CmdWaitEvents2KHR = (PFN_vkCmdWaitEvents2KHR)gdpa(*pDevice, "vkCmdWaitEvents2KHR");
dispatchTable.CmdPipelineBarrier2KHR = (PFN_vkCmdPipelineBarrier2KHR)gdpa(*pDevice, "vkCmdPipelineBarrier2KHR");
dispatchTable.QueueSubmit2KHR = (PFN_vkQueueSubmit2KHR)gdpa(*pDevice, "vkQueueSubmit2KHR");
dispatchTable.CmdWriteTimestamp2KHR = (PFN_vkCmdWriteTimestamp2KHR)gdpa(*pDevice, "vkCmdWriteTimestamp2KHR");
dispatchTable.CmdBeginRenderingKHR = (PFN_vkCmdBeginRenderingKHR)gdpa(*pDevice, "vkCmdBeginRenderingKHR");
dispatchTable.CmdEndRenderingKHR = (PFN_vkCmdEndRenderingKHR)gdpa(*pDevice, "vkCmdEndRenderingKHR");
dispatchTable.GetImageSubresourceLayout2EXT = (PFN_vkGetImageSubresourceLayout2EXT)gdpa(*pDevice, "vkGetImageSubresourceLayout2EXT");

{ scoped_lock l(global_lock); 
device_dispatch[GetKey(*pDevice)] = dispatchTable; 
} 
} 
 
void CreateInstanceDispatch(PFN_vkGetInstanceProcAddr gpa, VkInstance* pInstance) {
VkLayerInstanceDispatchTable dispatchTable;
dispatchTable.CreateInstance = (PFN_vkCreateInstance)gpa(*pInstance, "vkCreateInstance");
dispatchTable.DestroyInstance = (PFN_vkDestroyInstance)gpa(*pInstance, "vkDestroyInstance");
dispatchTable.EnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)gpa(*pInstance, "vkEnumeratePhysicalDevices");
dispatchTable.GetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)gpa(*pInstance, "vkGetInstanceProcAddr");
dispatchTable.GetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)gpa(*pInstance, "vkGetPhysicalDeviceProperties");
dispatchTable.GetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)gpa(*pInstance, "vkGetPhysicalDeviceQueueFamilyProperties");
dispatchTable.GetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties)gpa(*pInstance, "vkGetPhysicalDeviceMemoryProperties");
dispatchTable.GetPhysicalDeviceFeatures = (PFN_vkGetPhysicalDeviceFeatures)gpa(*pInstance, "vkGetPhysicalDeviceFeatures");
dispatchTable.GetPhysicalDeviceFormatProperties = (PFN_vkGetPhysicalDeviceFormatProperties)gpa(*pInstance, "vkGetPhysicalDeviceFormatProperties");
dispatchTable.GetPhysicalDeviceImageFormatProperties = (PFN_vkGetPhysicalDeviceImageFormatProperties)gpa(*pInstance, "vkGetPhysicalDeviceImageFormatProperties");
dispatchTable.CreateDevice = (PFN_vkCreateDevice)gpa(*pInstance, "vkCreateDevice");
dispatchTable.EnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)gpa(*pInstance, "vkEnumerateInstanceExtensionProperties");
dispatchTable.EnumerateDeviceLayerProperties = (PFN_vkEnumerateDeviceLayerProperties)gpa(*pInstance, "vkEnumerateDeviceLayerProperties");
dispatchTable.EnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties)gpa(*pInstance, "vkEnumerateDeviceExtensionProperties");
dispatchTable.GetPhysicalDeviceSparseImageFormatProperties = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties)gpa(*pInstance, "vkGetPhysicalDeviceSparseImageFormatProperties");
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
dispatchTable.CreateAndroidSurfaceKHR = (PFN_vkCreateAndroidSurfaceKHR)gpa(*pInstance, "vkCreateAndroidSurfaceKHR");
#endif
dispatchTable.GetPhysicalDeviceDisplayPropertiesKHR = (PFN_vkGetPhysicalDeviceDisplayPropertiesKHR)gpa(*pInstance, "vkGetPhysicalDeviceDisplayPropertiesKHR");
dispatchTable.GetPhysicalDeviceDisplayPlanePropertiesKHR = (PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR)gpa(*pInstance, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR");
dispatchTable.GetDisplayPlaneSupportedDisplaysKHR = (PFN_vkGetDisplayPlaneSupportedDisplaysKHR)gpa(*pInstance, "vkGetDisplayPlaneSupportedDisplaysKHR");
dispatchTable.GetDisplayModePropertiesKHR = (PFN_vkGetDisplayModePropertiesKHR)gpa(*pInstance, "vkGetDisplayModePropertiesKHR");
dispatchTable.CreateDisplayModeKHR = (PFN_vkCreateDisplayModeKHR)gpa(*pInstance, "vkCreateDisplayModeKHR");
dispatchTable.GetDisplayPlaneCapabilitiesKHR = (PFN_vkGetDisplayPlaneCapabilitiesKHR)gpa(*pInstance, "vkGetDisplayPlaneCapabilitiesKHR");
dispatchTable.CreateDisplayPlaneSurfaceKHR = (PFN_vkCreateDisplayPlaneSurfaceKHR)gpa(*pInstance, "vkCreateDisplayPlaneSurfaceKHR");
dispatchTable.DestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)gpa(*pInstance, "vkDestroySurfaceKHR");
dispatchTable.GetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)gpa(*pInstance, "vkGetPhysicalDeviceSurfaceSupportKHR");
dispatchTable.GetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)gpa(*pInstance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
dispatchTable.GetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)gpa(*pInstance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
dispatchTable.GetPhysicalDeviceSurfacePresentModesKHR = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)gpa(*pInstance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
#if defined(VK_USE_PLATFORM_VI_NN)
dispatchTable.CreateViSurfaceNN = (PFN_vkCreateViSurfaceNN)gpa(*pInstance, "vkCreateViSurfaceNN");
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
dispatchTable.CreateWaylandSurfaceKHR = (PFN_vkCreateWaylandSurfaceKHR)gpa(*pInstance, "vkCreateWaylandSurfaceKHR");
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
dispatchTable.GetPhysicalDeviceWaylandPresentationSupportKHR = (PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR)gpa(*pInstance, "vkGetPhysicalDeviceWaylandPresentationSupportKHR");
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
dispatchTable.CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)gpa(*pInstance, "vkCreateWin32SurfaceKHR");
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
dispatchTable.GetPhysicalDeviceWin32PresentationSupportKHR = (PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)gpa(*pInstance, "vkGetPhysicalDeviceWin32PresentationSupportKHR");
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR)
dispatchTable.CreateXlibSurfaceKHR = (PFN_vkCreateXlibSurfaceKHR)gpa(*pInstance, "vkCreateXlibSurfaceKHR");
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR)
dispatchTable.GetPhysicalDeviceXlibPresentationSupportKHR = (PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)gpa(*pInstance, "vkGetPhysicalDeviceXlibPresentationSupportKHR");
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
dispatchTable.CreateXcbSurfaceKHR = (PFN_vkCreateXcbSurfaceKHR)gpa(*pInstance, "vkCreateXcbSurfaceKHR");
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
dispatchTable.GetPhysicalDeviceXcbPresentationSupportKHR = (PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR)gpa(*pInstance, "vkGetPhysicalDeviceXcbPresentationSupportKHR");
#endif
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
dispatchTable.CreateDirectFBSurfaceEXT = (PFN_vkCreateDirectFBSurfaceEXT)gpa(*pInstance, "vkCreateDirectFBSurfaceEXT");
#endif
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
dispatchTable.GetPhysicalDeviceDirectFBPresentationSupportEXT = (PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT)gpa(*pInstance, "vkGetPhysicalDeviceDirectFBPresentationSupportEXT");
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
dispatchTable.CreateImagePipeSurfaceFUCHSIA = (PFN_vkCreateImagePipeSurfaceFUCHSIA)gpa(*pInstance, "vkCreateImagePipeSurfaceFUCHSIA");
#endif
#if defined(VK_USE_PLATFORM_GGP)
dispatchTable.CreateStreamDescriptorSurfaceGGP = (PFN_vkCreateStreamDescriptorSurfaceGGP)gpa(*pInstance, "vkCreateStreamDescriptorSurfaceGGP");
#endif
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
dispatchTable.CreateScreenSurfaceQNX = (PFN_vkCreateScreenSurfaceQNX)gpa(*pInstance, "vkCreateScreenSurfaceQNX");
#endif
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
dispatchTable.GetPhysicalDeviceScreenPresentationSupportQNX = (PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX)gpa(*pInstance, "vkGetPhysicalDeviceScreenPresentationSupportQNX");
#endif
dispatchTable.CreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)gpa(*pInstance, "vkCreateDebugReportCallbackEXT");
dispatchTable.DestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)gpa(*pInstance, "vkDestroyDebugReportCallbackEXT");
dispatchTable.DebugReportMessageEXT = (PFN_vkDebugReportMessageEXT)gpa(*pInstance, "vkDebugReportMessageEXT");
dispatchTable.GetPhysicalDeviceExternalImageFormatPropertiesNV = (PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV)gpa(*pInstance, "vkGetPhysicalDeviceExternalImageFormatPropertiesNV");
dispatchTable.GetPhysicalDeviceFeatures2 = (PFN_vkGetPhysicalDeviceFeatures2)gpa(*pInstance, "vkGetPhysicalDeviceFeatures2");
dispatchTable.GetPhysicalDeviceProperties2 = (PFN_vkGetPhysicalDeviceProperties2)gpa(*pInstance, "vkGetPhysicalDeviceProperties2");
dispatchTable.GetPhysicalDeviceFormatProperties2 = (PFN_vkGetPhysicalDeviceFormatProperties2)gpa(*pInstance, "vkGetPhysicalDeviceFormatProperties2");
dispatchTable.GetPhysicalDeviceImageFormatProperties2 = (PFN_vkGetPhysicalDeviceImageFormatProperties2)gpa(*pInstance, "vkGetPhysicalDeviceImageFormatProperties2");
dispatchTable.GetPhysicalDeviceQueueFamilyProperties2 = (PFN_vkGetPhysicalDeviceQueueFamilyProperties2)gpa(*pInstance, "vkGetPhysicalDeviceQueueFamilyProperties2");
dispatchTable.GetPhysicalDeviceMemoryProperties2 = (PFN_vkGetPhysicalDeviceMemoryProperties2)gpa(*pInstance, "vkGetPhysicalDeviceMemoryProperties2");
dispatchTable.GetPhysicalDeviceSparseImageFormatProperties2 = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties2)gpa(*pInstance, "vkGetPhysicalDeviceSparseImageFormatProperties2");
dispatchTable.GetPhysicalDeviceExternalBufferProperties = (PFN_vkGetPhysicalDeviceExternalBufferProperties)gpa(*pInstance, "vkGetPhysicalDeviceExternalBufferProperties");
#if defined(VK_USE_PLATFORM_SCI)
dispatchTable.GetPhysicalDeviceExternalMemorySciBufPropertiesNV = (PFN_vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV)gpa(*pInstance, "vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV");
#endif
#if defined(VK_USE_PLATFORM_SCI)
dispatchTable.GetPhysicalDeviceSciBufAttributesNV = (PFN_vkGetPhysicalDeviceSciBufAttributesNV)gpa(*pInstance, "vkGetPhysicalDeviceSciBufAttributesNV");
#endif
dispatchTable.GetPhysicalDeviceExternalSemaphoreProperties = (PFN_vkGetPhysicalDeviceExternalSemaphoreProperties)gpa(*pInstance, "vkGetPhysicalDeviceExternalSemaphoreProperties");
dispatchTable.GetPhysicalDeviceExternalFenceProperties = (PFN_vkGetPhysicalDeviceExternalFenceProperties)gpa(*pInstance, "vkGetPhysicalDeviceExternalFenceProperties");
#if defined(VK_USE_PLATFORM_SCI)
dispatchTable.GetPhysicalDeviceSciSyncAttributesNV = (PFN_vkGetPhysicalDeviceSciSyncAttributesNV)gpa(*pInstance, "vkGetPhysicalDeviceSciSyncAttributesNV");
#endif
dispatchTable.ReleaseDisplayEXT = (PFN_vkReleaseDisplayEXT)gpa(*pInstance, "vkReleaseDisplayEXT");
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
dispatchTable.AcquireXlibDisplayEXT = (PFN_vkAcquireXlibDisplayEXT)gpa(*pInstance, "vkAcquireXlibDisplayEXT");
#endif
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
dispatchTable.GetRandROutputDisplayEXT = (PFN_vkGetRandROutputDisplayEXT)gpa(*pInstance, "vkGetRandROutputDisplayEXT");
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
dispatchTable.AcquireWinrtDisplayNV = (PFN_vkAcquireWinrtDisplayNV)gpa(*pInstance, "vkAcquireWinrtDisplayNV");
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
dispatchTable.GetWinrtDisplayNV = (PFN_vkGetWinrtDisplayNV)gpa(*pInstance, "vkGetWinrtDisplayNV");
#endif
dispatchTable.GetPhysicalDeviceSurfaceCapabilities2EXT = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT)gpa(*pInstance, "vkGetPhysicalDeviceSurfaceCapabilities2EXT");
dispatchTable.EnumeratePhysicalDeviceGroups = (PFN_vkEnumeratePhysicalDeviceGroups)gpa(*pInstance, "vkEnumeratePhysicalDeviceGroups");
dispatchTable.GetPhysicalDevicePresentRectanglesKHR = (PFN_vkGetPhysicalDevicePresentRectanglesKHR)gpa(*pInstance, "vkGetPhysicalDevicePresentRectanglesKHR");
#if defined(VK_USE_PLATFORM_IOS_MVK)
dispatchTable.CreateIOSSurfaceMVK = (PFN_vkCreateIOSSurfaceMVK)gpa(*pInstance, "vkCreateIOSSurfaceMVK");
#endif
#if defined(VK_USE_PLATFORM_MACOS_MVK)
dispatchTable.CreateMacOSSurfaceMVK = (PFN_vkCreateMacOSSurfaceMVK)gpa(*pInstance, "vkCreateMacOSSurfaceMVK");
#endif
#if defined(VK_USE_PLATFORM_METAL_EXT)
dispatchTable.CreateMetalSurfaceEXT = (PFN_vkCreateMetalSurfaceEXT)gpa(*pInstance, "vkCreateMetalSurfaceEXT");
#endif
dispatchTable.GetPhysicalDeviceMultisamplePropertiesEXT = (PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT)gpa(*pInstance, "vkGetPhysicalDeviceMultisamplePropertiesEXT");
dispatchTable.GetPhysicalDeviceSurfaceCapabilities2KHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR)gpa(*pInstance, "vkGetPhysicalDeviceSurfaceCapabilities2KHR");
dispatchTable.GetPhysicalDeviceSurfaceFormats2KHR = (PFN_vkGetPhysicalDeviceSurfaceFormats2KHR)gpa(*pInstance, "vkGetPhysicalDeviceSurfaceFormats2KHR");
dispatchTable.GetPhysicalDeviceDisplayProperties2KHR = (PFN_vkGetPhysicalDeviceDisplayProperties2KHR)gpa(*pInstance, "vkGetPhysicalDeviceDisplayProperties2KHR");
dispatchTable.GetPhysicalDeviceDisplayPlaneProperties2KHR = (PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR)gpa(*pInstance, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR");
dispatchTable.GetDisplayModeProperties2KHR = (PFN_vkGetDisplayModeProperties2KHR)gpa(*pInstance, "vkGetDisplayModeProperties2KHR");
dispatchTable.GetDisplayPlaneCapabilities2KHR = (PFN_vkGetDisplayPlaneCapabilities2KHR)gpa(*pInstance, "vkGetDisplayPlaneCapabilities2KHR");
dispatchTable.GetPhysicalDeviceCalibrateableTimeDomainsKHR = (PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR)gpa(*pInstance, "vkGetPhysicalDeviceCalibrateableTimeDomainsKHR");
dispatchTable.CreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)gpa(*pInstance, "vkCreateDebugUtilsMessengerEXT");
dispatchTable.DestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)gpa(*pInstance, "vkDestroyDebugUtilsMessengerEXT");
dispatchTable.SubmitDebugUtilsMessageEXT = (PFN_vkSubmitDebugUtilsMessageEXT)gpa(*pInstance, "vkSubmitDebugUtilsMessageEXT");
dispatchTable.GetPhysicalDeviceCooperativeMatrixPropertiesNV = (PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV)gpa(*pInstance, "vkGetPhysicalDeviceCooperativeMatrixPropertiesNV");
#if defined(VK_USE_PLATFORM_WIN32_KHR)
dispatchTable.GetPhysicalDeviceSurfacePresentModes2EXT = (PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT)gpa(*pInstance, "vkGetPhysicalDeviceSurfacePresentModes2EXT");
#endif
dispatchTable.EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR = (PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR)gpa(*pInstance, "vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR");
dispatchTable.GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR = (PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR)gpa(*pInstance, "vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR");
dispatchTable.CreateHeadlessSurfaceEXT = (PFN_vkCreateHeadlessSurfaceEXT)gpa(*pInstance, "vkCreateHeadlessSurfaceEXT");
dispatchTable.GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV = (PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV)gpa(*pInstance, "vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV");
dispatchTable.GetPhysicalDeviceToolProperties = (PFN_vkGetPhysicalDeviceToolProperties)gpa(*pInstance, "vkGetPhysicalDeviceToolProperties");
dispatchTable.GetPhysicalDeviceFragmentShadingRatesKHR = (PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR)gpa(*pInstance, "vkGetPhysicalDeviceFragmentShadingRatesKHR");
dispatchTable.GetPhysicalDeviceVideoCapabilitiesKHR = (PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR)gpa(*pInstance, "vkGetPhysicalDeviceVideoCapabilitiesKHR");
dispatchTable.GetPhysicalDeviceVideoFormatPropertiesKHR = (PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR)gpa(*pInstance, "vkGetPhysicalDeviceVideoFormatPropertiesKHR");
dispatchTable.GetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR = (PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR)gpa(*pInstance, "vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR");
dispatchTable.AcquireDrmDisplayEXT = (PFN_vkAcquireDrmDisplayEXT)gpa(*pInstance, "vkAcquireDrmDisplayEXT");
dispatchTable.GetDrmDisplayEXT = (PFN_vkGetDrmDisplayEXT)gpa(*pInstance, "vkGetDrmDisplayEXT");
dispatchTable.GetPhysicalDeviceOpticalFlowImageFormatsNV = (PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV)gpa(*pInstance, "vkGetPhysicalDeviceOpticalFlowImageFormatsNV");
dispatchTable.GetPhysicalDeviceCooperativeMatrixPropertiesKHR = (PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR)gpa(*pInstance, "vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR");
dispatchTable.GetPhysicalDeviceFeatures2KHR = (PFN_vkGetPhysicalDeviceFeatures2KHR)gpa(*pInstance, "vkGetPhysicalDeviceFeatures2KHR");
dispatchTable.GetPhysicalDeviceProperties2KHR = (PFN_vkGetPhysicalDeviceProperties2KHR)gpa(*pInstance, "vkGetPhysicalDeviceProperties2KHR");
dispatchTable.GetPhysicalDeviceFormatProperties2KHR = (PFN_vkGetPhysicalDeviceFormatProperties2KHR)gpa(*pInstance, "vkGetPhysicalDeviceFormatProperties2KHR");
dispatchTable.GetPhysicalDeviceImageFormatProperties2KHR = (PFN_vkGetPhysicalDeviceImageFormatProperties2KHR)gpa(*pInstance, "vkGetPhysicalDeviceImageFormatProperties2KHR");
dispatchTable.GetPhysicalDeviceQueueFamilyProperties2KHR = (PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR)gpa(*pInstance, "vkGetPhysicalDeviceQueueFamilyProperties2KHR");
dispatchTable.GetPhysicalDeviceMemoryProperties2KHR = (PFN_vkGetPhysicalDeviceMemoryProperties2KHR)gpa(*pInstance, "vkGetPhysicalDeviceMemoryProperties2KHR");
dispatchTable.GetPhysicalDeviceSparseImageFormatProperties2KHR = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR)gpa(*pInstance, "vkGetPhysicalDeviceSparseImageFormatProperties2KHR");
dispatchTable.GetPhysicalDeviceExternalBufferPropertiesKHR = (PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR)gpa(*pInstance, "vkGetPhysicalDeviceExternalBufferPropertiesKHR");
dispatchTable.GetPhysicalDeviceExternalSemaphorePropertiesKHR = (PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR)gpa(*pInstance, "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR");
dispatchTable.GetPhysicalDeviceExternalFencePropertiesKHR = (PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR)gpa(*pInstance, "vkGetPhysicalDeviceExternalFencePropertiesKHR");
dispatchTable.EnumeratePhysicalDeviceGroupsKHR = (PFN_vkEnumeratePhysicalDeviceGroupsKHR)gpa(*pInstance, "vkEnumeratePhysicalDeviceGroupsKHR");
dispatchTable.GetPhysicalDeviceCalibrateableTimeDomainsEXT = (PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT)gpa(*pInstance, "vkGetPhysicalDeviceCalibrateableTimeDomainsEXT");
dispatchTable.GetPhysicalDeviceToolPropertiesEXT = (PFN_vkGetPhysicalDeviceToolPropertiesEXT)gpa(*pInstance, "vkGetPhysicalDeviceToolPropertiesEXT");

{ scoped_lock l(global_lock); 
instance_dispatch[GetKey(*pInstance)] = dispatchTable; 
} 
} 
 

/* actual data we're recording in this layer */

/*struct CommandStats
{
    uint32_t drawCount = 0, instanceCount = 0, vertCount = 0;
};
std::map<VkCommandBuffer, CommandStats> commandbuffer_stats;
*/
bool connected = false;

std::string bool_as_text(VkBool32 b)
{
    std::stringstream converter;
    converter << std::boolalpha << b;   // flag boolalpha calls converter.setf(std::ios_base::boolalpha)
    return converter.str();
}

std::string ptrToString(void* input)
{
    if (input == NULL)
        return "NULL";
    std::stringstream s;
    s << input;
    return s.str();
}
std::string charToString(auto* input)
{
    if (input == NULL)
        return "NULL";
    std::stringstream s;
    s << *input;
    return s.str();
}
std::string GetWindowName()
{
    /* get the name of the program the layer is on [windows only] */
    char buf[MAX_PATH];
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    std::filesystem::path progPath(buf);
    auto filename = progPath.filename().string();
    return filename;
}

/* Layer init and shutdown */
VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    /* create new process */
    STARTUPINFO info = { sizeof(info) };
    PROCESS_INFORMATION processInfo;

    /* get the name of the program the layer is on [windows only] */
    char buf[MAX_PATH];
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    std::filesystem::path progPath(buf);
    std::string filename = progPath.filename().string();

    /* prevent opening vkDetails on appUI startup */
    if (GetWindowName() != "vkDetails.exe")
    {
        /* open new vkDetails window */
        CreateProcess("C:\\Users\\jozef\\Desktop\\vk details\\out\\build\\x64-debug\\vkDetails.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &info, &processInfo);
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);

        /* connect layer to vkDetails */
        if (layerWinsockInit(&ConnectSocket) == 0)
        {
            connected = true;
        }
    }

    VkLayerInstanceCreateInfo* layerCreateInfo = (VkLayerInstanceCreateInfo*)pCreateInfo->pNext;

    /* step through the chain of pNext until we get to the link info */
    while (layerCreateInfo && (layerCreateInfo->sType != VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO || layerCreateInfo->function != VK_LAYER_LINK_INFO))
    {
        layerCreateInfo = (VkLayerInstanceCreateInfo*)layerCreateInfo->pNext;
    }

    if (layerCreateInfo == NULL)
    {
        /* No loader instance create info */
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    /* map this function to dispatch table */
    PFN_vkGetInstanceProcAddr gpa = layerCreateInfo->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    layerCreateInfo->u.pLayerInfo = layerCreateInfo->u.pLayerInfo->pNext;
    PFN_vkCreateInstance createFunc = (PFN_vkCreateInstance)gpa(VK_NULL_HANDLE, "vkCreateInstance");

    /* fetch our own dispatch table for the functions we need, into the next layer */
    CreateInstanceDispatch(gpa, pInstance);

    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "begin_vkCreateInstance!");
    }

    /* additional call if defined by user [before taken to loader] */
    #ifdef CREATEINSTANCE_BEFORE_EXEC_EXISTS
    if (connected) {
        layer_CreateInstance_before(pCreateInfo, pAllocator, pInstance);
    }
    #endif

    /* send to loader */
    VkResult ret = createFunc(pCreateInfo, pAllocator, pInstance);

    /* additional call if defined by user [on the way back out of loader] */
    #ifdef CREATEINSTANCE_AFTER_EXEC_EXISTS
    if (connected) {
        layer_CreateInstance_after(pCreateInfo, pAllocator, pInstance);
    }
    #endif 

    /* send call after loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "end_vkCreateInstance!");
    }

    return ret;
}

VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator)
{
    scoped_lock l(global_lock);

    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "begin_vkDestroyInstance!");
    }

    /* additional call if defined by user [before taken to loader] */
    #ifdef DESTROYINSTANCE_BEFORE_EXEC_EXISTS
    if (connected) {
        layer_DestroyInstance_before(instance, pAllocator);
    }
    #endif

    /* Destroy layer */
    instance_dispatch.erase(GetKey(instance));

    /* additional call if defined by user [on the way back out of loader] */
    #ifdef DESTROYINSTANCE_AFTER_EXEC_EXISTS
    if (connected) {
        layer_DestroyInstance_after(instance, pAllocator);
    }
    #endif 

    /* send call after loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "end_vkDestroyInstance!");
    }

    /* Disconnect from the VkDetails */
    layerWinsockExit(&ConnectSocket);
}

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    VkLayerDeviceCreateInfo* layerCreateInfo = (VkLayerDeviceCreateInfo*)pCreateInfo->pNext;

    /* step through the chain of pNext until we get to the link info */
    while (layerCreateInfo && (layerCreateInfo->sType != VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO ||
        layerCreateInfo->function != VK_LAYER_LINK_INFO))
    {
        layerCreateInfo = (VkLayerDeviceCreateInfo*)layerCreateInfo->pNext;
    }

    if (layerCreateInfo == NULL)
    {
        /* No loader instance create info */
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    PFN_vkGetInstanceProcAddr gipa = layerCreateInfo->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr gdpa = layerCreateInfo->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    /* move chain on for next layer */
    layerCreateInfo->u.pLayerInfo = layerCreateInfo->u.pLayerInfo->pNext;

    PFN_vkCreateDevice createFunc = (PFN_vkCreateDevice)gipa(VK_NULL_HANDLE, "vkCreateDevice");

    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "begin_vkCreateDevice!");
    }

    /* additional call if defined by user [before taken to loader] */
    #ifdef CREATEDEVICE_BEFORE_EXEC_EXISTS
    if (connected) {
        layer_CreateDevice_before(physicalDevice, pCreateInfo, pAllocator, pDevice);
    }
    #endif

    /* send to loader */
    VkResult ret = createFunc(physicalDevice, pCreateInfo, pAllocator, pDevice);

    /* additional call if defined by user [on the way back out of loader] */
    #ifdef CREATEDEVICE_AFTER_EXEC_EXISTS
    if (connected) {
        layer_CreateDevice_after(physicalDevice, pCreateInfo, pAllocator, pDevice);
    }
    #endif

    /* send call after loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "end_vkCreateDevice!");
    }



    /* fetch our own dispatch table for the functions we need, into the next layer */
    CreateDeviceDispatch(gdpa, pDevice);

    return ret;
}

VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator)
{
    scoped_lock l(global_lock);
    
    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "begin_vkDestroyDevice!");
    }

    /* additional call if defined by user [before taken to loader] */
    #ifdef DESTROYDEVICE_BEFORE_EXEC_EXISTS
    if (connected) {
        layer_DestroyDevice_before(device, pAllocator);
    }
    #endif

    device_dispatch.erase(GetKey(device));

    /* additional call if defined by user [on the way back out of loader] */
    #ifdef DESTROYDEVICE_AFTER_EXEC_EXISTS
    if (connected) {
        layer_DestroyDevice_after(device, pAllocator);
    }
    #endif 

    /* send call after loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "end_vkDestroyDevice!");
    }
}

/* Enumeration function */
VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "begin_vkEnumerateInstanceLayerProperties!");
    }

    /* additional call if defined by user [before taken to loader] */
#ifdef ENUMERATEINSTANCELAYERPROPERTIES_BEFORE_EXEC_EXISTS
    if (connected) {
        layer_EnumerateInstanceLayerProperties_before(pPropertyCount, pProperties);
    }
#endif
    if (pPropertyCount) *pPropertyCount = 1;

    if (pProperties)
    {
        strcpy_s(pProperties->layerName, "vkDetailsLayer");
        strcpy_s(pProperties->description, "https://github.com/xbilko03/ADT_VAPI");
        pProperties->implementationVersion = 1;
        pProperties->specVersion = VK_API_VERSION_1_0;
    }

    return VK_SUCCESS;
}

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{

    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "begin_vkEnumerateDeviceLayerProperties!");
    }

    /* additional call if defined by user [before taken to loader] */
    #ifdef ENUMERATEDEVICELAYERPROPERTIES_BEFORE_EXEC_EXISTS
    if (connected) {
        layer_EnumerateDeviceLayerProperties_before(physicalDevice, pPropertyCount, pProperties);
    }
    #endif

    if (pPropertyCount) *pPropertyCount = 1;

    if (pProperties)
    {
        strcpy_s(pProperties->layerName, "vkDetailsLayer");
        strcpy_s(pProperties->description, "https://github.com/xbilko03/ADT_VAPI");
        pProperties->implementationVersion = 1;
        pProperties->specVersion = VK_API_VERSION_1_0;
    }
    return VK_SUCCESS;
}

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "begin_vkEnumerateInstanceExtensionProperties!");
    }

    /* additional call if defined by user [before taken to loader] */
    #ifdef ENUMERATEINSTANCEEXTENSIONPROPERTIES_BEFORE_EXEC_EXISTS
    if (connected) {
        layer_EnumerateInstanceExtensionProperties_before(pLayerName, pPropertyCount, pProperties);
    }
    #endif

    if (pLayerName == NULL || strcmp(pLayerName, "VK_LAYER_SAMPLE_DetailsLayer"))
        return VK_ERROR_LAYER_NOT_PRESENT;

    /* don't expose any extensions */
    if (pPropertyCount) *pPropertyCount = 0;
    return VK_SUCCESS;
}

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{

    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "begin_vkEnumerateDeviceExtensionProperties!");
    }

    /* additional call if defined by user [before taken to loader] */
    #ifdef ENUMERATEDEVICEEXTENSIONPROPERTIES_BEFORE_EXEC_EXISTS
    if (connected) {
        layer_EnumerateDeviceExtensionProperties_before(physicalDevice, pLayerName, pPropertyCount, pProperties);
    }
    #endif

    /*  pass through any queries that aren't to us */
    if (pLayerName == NULL || strcmp(pLayerName, "VK_LAYER_SAMPLE_DetailsLayer"))
    {
        if (physicalDevice == VK_NULL_HANDLE)
            return VK_SUCCESS;

        scoped_lock l(global_lock);
        return instance_dispatch[GetKey(physicalDevice)].EnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pPropertyCount, pProperties);
    }

    /* don't expose any extensions */
    if (pPropertyCount) *pPropertyCount = 0;
    return VK_SUCCESS;
}

/* Generated part */

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkEnumeratePhysicalDevices!");
}
#ifdef ENUMERATEPHYSICALDEVICES_BEFORE_EXEC_EXISTS
if(connected) {
layer_EnumeratePhysicalDevices_before(instance, pPhysicalDeviceCount, pPhysicalDevices);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].EnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pPhysicalDeviceCount=" + ptrToString((void*)pPhysicalDeviceCount) + '!');
winsockSendToUI(&ConnectSocket,"pPhysicalDevices=" + ptrToString((void*)pPhysicalDevices) + '!');
#ifdef ENUMERATEPHYSICALDEVICES_AFTER_EXEC_EXISTS
if(connected) {
layer_EnumeratePhysicalDevices_after(instance, pPhysicalDeviceCount, pPhysicalDevices);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkEnumeratePhysicalDevices!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceProperties!");
}
#ifdef GETPHYSICALDEVICEPROPERTIES_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceProperties_before(physicalDevice, pProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceProperties(physicalDevice, pProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETPHYSICALDEVICEPROPERTIES_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceProperties_after(physicalDevice, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceProperties!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceQueueFamilyProperties!");
}
#ifdef GETPHYSICALDEVICEQUEUEFAMILYPROPERTIES_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceQueueFamilyProperties_before(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceQueueFamilyProperties(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pQueueFamilyPropertyCount=" + ptrToString((void*)pQueueFamilyPropertyCount) + '!');
winsockSendToUI(&ConnectSocket,"pQueueFamilyProperties=" + ptrToString((void*)pQueueFamilyProperties) + '!');
#ifdef GETPHYSICALDEVICEQUEUEFAMILYPROPERTIES_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceQueueFamilyProperties_after(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceQueueFamilyProperties!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceMemoryProperties!");
}
#ifdef GETPHYSICALDEVICEMEMORYPROPERTIES_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceMemoryProperties_before(physicalDevice, pMemoryProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryProperties=" + ptrToString((void*)pMemoryProperties) + '!');
#ifdef GETPHYSICALDEVICEMEMORYPROPERTIES_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceMemoryProperties_after(physicalDevice, pMemoryProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceMemoryProperties!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceFeatures!");
}
#ifdef GETPHYSICALDEVICEFEATURES_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceFeatures_before(physicalDevice, pFeatures);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceFeatures(physicalDevice, pFeatures);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pFeatures=" + ptrToString((void*)pFeatures) + '!');
#ifdef GETPHYSICALDEVICEFEATURES_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceFeatures_after(physicalDevice, pFeatures);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceFeatures!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceFormatProperties!");
}
#ifdef GETPHYSICALDEVICEFORMATPROPERTIES_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceFormatProperties_before(physicalDevice, format, pFormatProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"format=" + std::to_string(format) + '!');
winsockSendToUI(&ConnectSocket,"pFormatProperties=" + ptrToString((void*)pFormatProperties) + '!');
#ifdef GETPHYSICALDEVICEFORMATPROPERTIES_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceFormatProperties_after(physicalDevice, format, pFormatProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceFormatProperties!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceImageFormatProperties!");
}
#ifdef GETPHYSICALDEVICEIMAGEFORMATPROPERTIES_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceImageFormatProperties_before(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceImageFormatProperties(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"format=" + std::to_string(format) + '!');
winsockSendToUI(&ConnectSocket,"type=" + std::to_string(type) + '!');
winsockSendToUI(&ConnectSocket,"tiling=" + std::to_string(tiling) + '!');
winsockSendToUI(&ConnectSocket,"usage=" + ptrToString((void*)usage) + '!');
winsockSendToUI(&ConnectSocket,"flags=" + ptrToString((void*)flags) + '!');
winsockSendToUI(&ConnectSocket,"pImageFormatProperties=" + ptrToString((void*)pImageFormatProperties) + '!');
#ifdef GETPHYSICALDEVICEIMAGEFORMATPROPERTIES_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceImageFormatProperties_after(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceImageFormatProperties!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceSparseImageFormatProperties!");
}
#ifdef GETPHYSICALDEVICESPARSEIMAGEFORMATPROPERTIES_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSparseImageFormatProperties_before(physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSparseImageFormatProperties(physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"format=" + std::to_string(format) + '!');
winsockSendToUI(&ConnectSocket,"type=" + std::to_string(type) + '!');
winsockSendToUI(&ConnectSocket,"samples=" + std::to_string(samples) + '!');
winsockSendToUI(&ConnectSocket,"usage=" + ptrToString((void*)usage) + '!');
winsockSendToUI(&ConnectSocket,"tiling=" + std::to_string(tiling) + '!');
winsockSendToUI(&ConnectSocket,"pPropertyCount=" + ptrToString((void*)pPropertyCount) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETPHYSICALDEVICESPARSEIMAGEFORMATPROPERTIES_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSparseImageFormatProperties_after(physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceSparseImageFormatProperties!");
}
}

#if defined(VK_USE_PLATFORM_ANDROID_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateAndroidSurfaceKHR(VkInstance instance, VkAndroidSurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateAndroidSurfaceKHR!");
}
#ifdef CREATEANDROIDSURFACEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateAndroidSurfaceKHR_before(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateAndroidSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSurface=" + ptrToString((void*)pSurface) + '!');
#ifdef CREATEANDROIDSURFACEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateAndroidSurfaceKHR_after(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateAndroidSurfaceKHR!");
}
return ret;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceDisplayPropertiesKHR!");
}
#ifdef GETPHYSICALDEVICEDISPLAYPROPERTIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceDisplayPropertiesKHR_before(physicalDevice, pPropertyCount, pProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pPropertyCount=" + ptrToString((void*)pPropertyCount) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETPHYSICALDEVICEDISPLAYPROPERTIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceDisplayPropertiesKHR_after(physicalDevice, pPropertyCount, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceDisplayPropertiesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceDisplayPlanePropertiesKHR!");
}
#ifdef GETPHYSICALDEVICEDISPLAYPLANEPROPERTIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceDisplayPlanePropertiesKHR_before(physicalDevice, pPropertyCount, pProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, pPropertyCount, pProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pPropertyCount=" + ptrToString((void*)pPropertyCount) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETPHYSICALDEVICEDISPLAYPLANEPROPERTIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceDisplayPlanePropertiesKHR_after(physicalDevice, pPropertyCount, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceDisplayPlanePropertiesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDisplayPlaneSupportedDisplaysKHR!");
}
#ifdef GETDISPLAYPLANESUPPORTEDDISPLAYSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDisplayPlaneSupportedDisplaysKHR_before(physicalDevice, planeIndex, pDisplayCount, pDisplays);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, pDisplayCount, pDisplays);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"planeIndex=" + std::to_string(planeIndex) + '!');
winsockSendToUI(&ConnectSocket,"pDisplayCount=" + ptrToString((void*)pDisplayCount) + '!');
winsockSendToUI(&ConnectSocket,"pDisplays=" + ptrToString((void*)pDisplays) + '!');
#ifdef GETDISPLAYPLANESUPPORTEDDISPLAYSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDisplayPlaneSupportedDisplaysKHR_after(physicalDevice, planeIndex, pDisplayCount, pDisplays);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDisplayPlaneSupportedDisplaysKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDisplayModePropertiesKHR!");
}
#ifdef GETDISPLAYMODEPROPERTIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDisplayModePropertiesKHR_before(physicalDevice, display, pPropertyCount, pProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetDisplayModePropertiesKHR(physicalDevice, display, pPropertyCount, pProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"display=" + ptrToString((void*)display) + '!');
winsockSendToUI(&ConnectSocket,"pPropertyCount=" + ptrToString((void*)pPropertyCount) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETDISPLAYMODEPROPERTIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDisplayModePropertiesKHR_after(physicalDevice, display, pPropertyCount, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDisplayModePropertiesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, VkDisplayModeCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateDisplayModeKHR!");
}
#ifdef CREATEDISPLAYMODEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateDisplayModeKHR_before(physicalDevice, display, pCreateInfo, pAllocator, pMode);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].CreateDisplayModeKHR(physicalDevice, display, pCreateInfo, pAllocator, pMode);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"display=" + ptrToString((void*)display) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pMode=" + ptrToString((void*)pMode) + '!');
#ifdef CREATEDISPLAYMODEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateDisplayModeKHR_after(physicalDevice, display, pCreateInfo, pAllocator, pMode);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateDisplayModeKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDisplayPlaneCapabilitiesKHR!");
}
#ifdef GETDISPLAYPLANECAPABILITIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDisplayPlaneCapabilitiesKHR_before(physicalDevice, mode, planeIndex, pCapabilities);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"mode=" + ptrToString((void*)mode) + '!');
winsockSendToUI(&ConnectSocket,"planeIndex=" + std::to_string(planeIndex) + '!');
winsockSendToUI(&ConnectSocket,"pCapabilities=" + ptrToString((void*)pCapabilities) + '!');
#ifdef GETDISPLAYPLANECAPABILITIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDisplayPlaneCapabilitiesKHR_after(physicalDevice, mode, planeIndex, pCapabilities);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDisplayPlaneCapabilitiesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDisplayPlaneSurfaceKHR(VkInstance instance, VkDisplaySurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateDisplayPlaneSurfaceKHR!");
}
#ifdef CREATEDISPLAYPLANESURFACEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateDisplayPlaneSurfaceKHR_before(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateDisplayPlaneSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSurface=" + ptrToString((void*)pSurface) + '!');
#ifdef CREATEDISPLAYPLANESURFACEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateDisplayPlaneSurfaceKHR_after(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateDisplayPlaneSurfaceKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroySurfaceKHR!");
}
#ifdef DESTROYSURFACEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroySurfaceKHR_before(instance, surface, pAllocator);
}
#endif 
instance_dispatch[GetKey(instance)].DestroySurfaceKHR(instance, surface, pAllocator);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"surface=" + ptrToString((void*)surface) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYSURFACEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroySurfaceKHR_after(instance, surface, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroySurfaceKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceSurfaceSupportKHR!");
}
#ifdef GETPHYSICALDEVICESURFACESUPPORTKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSurfaceSupportKHR_before(physicalDevice, queueFamilyIndex, surface, pSupported);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"queueFamilyIndex=" + std::to_string(queueFamilyIndex) + '!');
winsockSendToUI(&ConnectSocket,"surface=" + ptrToString((void*)surface) + '!');
winsockSendToUI(&ConnectSocket,"pSupported=" + ptrToString((void*)pSupported) + '!');
#ifdef GETPHYSICALDEVICESURFACESUPPORTKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSurfaceSupportKHR_after(physicalDevice, queueFamilyIndex, surface, pSupported);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceSurfaceSupportKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceSurfaceCapabilitiesKHR!");
}
#ifdef GETPHYSICALDEVICESURFACECAPABILITIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSurfaceCapabilitiesKHR_before(physicalDevice, surface, pSurfaceCapabilities);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"surface=" + ptrToString((void*)surface) + '!');
winsockSendToUI(&ConnectSocket,"pSurfaceCapabilities=" + ptrToString((void*)pSurfaceCapabilities) + '!');
#ifdef GETPHYSICALDEVICESURFACECAPABILITIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSurfaceCapabilitiesKHR_after(physicalDevice, surface, pSurfaceCapabilities);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceSurfaceCapabilitiesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceSurfaceFormatsKHR!");
}
#ifdef GETPHYSICALDEVICESURFACEFORMATSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSurfaceFormatsKHR_before(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"surface=" + ptrToString((void*)surface) + '!');
winsockSendToUI(&ConnectSocket,"pSurfaceFormatCount=" + ptrToString((void*)pSurfaceFormatCount) + '!');
winsockSendToUI(&ConnectSocket,"pSurfaceFormats=" + ptrToString((void*)pSurfaceFormats) + '!');
#ifdef GETPHYSICALDEVICESURFACEFORMATSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSurfaceFormatsKHR_after(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceSurfaceFormatsKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceSurfacePresentModesKHR!");
}
#ifdef GETPHYSICALDEVICESURFACEPRESENTMODESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSurfacePresentModesKHR_before(physicalDevice, surface, pPresentModeCount, pPresentModes);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"surface=" + ptrToString((void*)surface) + '!');
winsockSendToUI(&ConnectSocket,"pPresentModeCount=" + ptrToString((void*)pPresentModeCount) + '!');
winsockSendToUI(&ConnectSocket,"pPresentModes=" + ptrToString((void*)pPresentModes) + '!');
#ifdef GETPHYSICALDEVICESURFACEPRESENTMODESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSurfacePresentModesKHR_after(physicalDevice, surface, pPresentModeCount, pPresentModes);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceSurfacePresentModesKHR!");
}
return ret;
}

#if defined(VK_USE_PLATFORM_VI_NN)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateViSurfaceNN(VkInstance instance, VkViSurfaceCreateInfoNN* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateViSurfaceNN!");
}
#ifdef CREATEVISURFACENN_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateViSurfaceNN_before(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateViSurfaceNN(instance, pCreateInfo, pAllocator, pSurface);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSurface=" + ptrToString((void*)pSurface) + '!');
#ifdef CREATEVISURFACENN_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateViSurfaceNN_after(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateViSurfaceNN!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateWaylandSurfaceKHR(VkInstance instance, VkWaylandSurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateWaylandSurfaceKHR!");
}
#ifdef CREATEWAYLANDSURFACEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateWaylandSurfaceKHR_before(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateWaylandSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSurface=" + ptrToString((void*)pSurface) + '!');
#ifdef CREATEWAYLANDSURFACEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateWaylandSurfaceKHR_after(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateWaylandSurfaceKHR!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
 VK_LAYER_EXPORT VkBool32 VKAPI_CALL DetailsLayer_GetPhysicalDeviceWaylandPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, wl_display* display) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceWaylandPresentationSupportKHR!");
}
#ifdef GETPHYSICALDEVICEWAYLANDPRESENTATIONSUPPORTKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceWaylandPresentationSupportKHR_before(physicalDevice, queueFamilyIndex, display);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceWaylandPresentationSupportKHR(physicalDevice, queueFamilyIndex, display);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"queueFamilyIndex=" + std::to_string(queueFamilyIndex) + '!');
winsockSendToUI(&ConnectSocket,"display=" + ptrToString((void*)display) + '!');
#ifdef GETPHYSICALDEVICEWAYLANDPRESENTATIONSUPPORTKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceWaylandPresentationSupportKHR_after(physicalDevice, queueFamilyIndex, display);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceWaylandPresentationSupportKHR!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateWin32SurfaceKHR(VkInstance instance, VkWin32SurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateWin32SurfaceKHR!");
}
#ifdef CREATEWIN32SURFACEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateWin32SurfaceKHR_before(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateWin32SurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSurface=" + ptrToString((void*)pSurface) + '!');
#ifdef CREATEWIN32SURFACEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateWin32SurfaceKHR_after(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateWin32SurfaceKHR!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkBool32 VKAPI_CALL DetailsLayer_GetPhysicalDeviceWin32PresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceWin32PresentationSupportKHR!");
}
#ifdef GETPHYSICALDEVICEWIN32PRESENTATIONSUPPORTKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceWin32PresentationSupportKHR_before(physicalDevice, queueFamilyIndex);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, queueFamilyIndex);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"queueFamilyIndex=" + std::to_string(queueFamilyIndex) + '!');
#ifdef GETPHYSICALDEVICEWIN32PRESENTATIONSUPPORTKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceWin32PresentationSupportKHR_after(physicalDevice, queueFamilyIndex);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceWin32PresentationSupportKHR!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateXlibSurfaceKHR(VkInstance instance, VkXlibSurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateXlibSurfaceKHR!");
}
#ifdef CREATEXLIBSURFACEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateXlibSurfaceKHR_before(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateXlibSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSurface=" + ptrToString((void*)pSurface) + '!');
#ifdef CREATEXLIBSURFACEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateXlibSurfaceKHR_after(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateXlibSurfaceKHR!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR)
 VK_LAYER_EXPORT VkBool32 VKAPI_CALL DetailsLayer_GetPhysicalDeviceXlibPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceXlibPresentationSupportKHR!");
}
#ifdef GETPHYSICALDEVICEXLIBPRESENTATIONSUPPORTKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceXlibPresentationSupportKHR_before(physicalDevice, queueFamilyIndex, dpy, visualID);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice, queueFamilyIndex, dpy, visualID);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"queueFamilyIndex=" + std::to_string(queueFamilyIndex) + '!');
winsockSendToUI(&ConnectSocket,"dpy=" + ptrToString((void*)dpy) + '!');
winsockSendToUI(&ConnectSocket,"visualID=" + ptrToString((void*)visualID) + '!');
#ifdef GETPHYSICALDEVICEXLIBPRESENTATIONSUPPORTKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceXlibPresentationSupportKHR_after(physicalDevice, queueFamilyIndex, dpy, visualID);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceXlibPresentationSupportKHR!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateXcbSurfaceKHR(VkInstance instance, VkXcbSurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateXcbSurfaceKHR!");
}
#ifdef CREATEXCBSURFACEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateXcbSurfaceKHR_before(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateXcbSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSurface=" + ptrToString((void*)pSurface) + '!');
#ifdef CREATEXCBSURFACEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateXcbSurfaceKHR_after(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateXcbSurfaceKHR!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
 VK_LAYER_EXPORT VkBool32 VKAPI_CALL DetailsLayer_GetPhysicalDeviceXcbPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceXcbPresentationSupportKHR!");
}
#ifdef GETPHYSICALDEVICEXCBPRESENTATIONSUPPORTKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceXcbPresentationSupportKHR_before(physicalDevice, queueFamilyIndex, connection, visual_id);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceXcbPresentationSupportKHR(physicalDevice, queueFamilyIndex, connection, visual_id);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"queueFamilyIndex=" + std::to_string(queueFamilyIndex) + '!');
winsockSendToUI(&ConnectSocket,"connection=" + ptrToString((void*)connection) + '!');
winsockSendToUI(&ConnectSocket,"visual_id=" + ptrToString((void*)visual_id) + '!');
#ifdef GETPHYSICALDEVICEXCBPRESENTATIONSUPPORTKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceXcbPresentationSupportKHR_after(physicalDevice, queueFamilyIndex, connection, visual_id);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceXcbPresentationSupportKHR!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDirectFBSurfaceEXT(VkInstance instance, VkDirectFBSurfaceCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateDirectFBSurfaceEXT!");
}
#ifdef CREATEDIRECTFBSURFACEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateDirectFBSurfaceEXT_before(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateDirectFBSurfaceEXT(instance, pCreateInfo, pAllocator, pSurface);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSurface=" + ptrToString((void*)pSurface) + '!');
#ifdef CREATEDIRECTFBSURFACEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateDirectFBSurfaceEXT_after(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateDirectFBSurfaceEXT!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
 VK_LAYER_EXPORT VkBool32 VKAPI_CALL DetailsLayer_GetPhysicalDeviceDirectFBPresentationSupportEXT(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceDirectFBPresentationSupportEXT!");
}
#ifdef GETPHYSICALDEVICEDIRECTFBPRESENTATIONSUPPORTEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceDirectFBPresentationSupportEXT_before(physicalDevice, queueFamilyIndex, dfb);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceDirectFBPresentationSupportEXT(physicalDevice, queueFamilyIndex, dfb);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"queueFamilyIndex=" + std::to_string(queueFamilyIndex) + '!');
winsockSendToUI(&ConnectSocket,"dfb=" + ptrToString((void*)dfb) + '!');
#ifdef GETPHYSICALDEVICEDIRECTFBPRESENTATIONSUPPORTEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceDirectFBPresentationSupportEXT_after(physicalDevice, queueFamilyIndex, dfb);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceDirectFBPresentationSupportEXT!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateImagePipeSurfaceFUCHSIA(VkInstance instance, VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateImagePipeSurfaceFUCHSIA!");
}
#ifdef CREATEIMAGEPIPESURFACEFUCHSIA_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateImagePipeSurfaceFUCHSIA_before(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateImagePipeSurfaceFUCHSIA(instance, pCreateInfo, pAllocator, pSurface);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSurface=" + ptrToString((void*)pSurface) + '!');
#ifdef CREATEIMAGEPIPESURFACEFUCHSIA_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateImagePipeSurfaceFUCHSIA_after(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateImagePipeSurfaceFUCHSIA!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_GGP)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateStreamDescriptorSurfaceGGP(VkInstance instance, VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateStreamDescriptorSurfaceGGP!");
}
#ifdef CREATESTREAMDESCRIPTORSURFACEGGP_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateStreamDescriptorSurfaceGGP_before(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateStreamDescriptorSurfaceGGP(instance, pCreateInfo, pAllocator, pSurface);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSurface=" + ptrToString((void*)pSurface) + '!');
#ifdef CREATESTREAMDESCRIPTORSURFACEGGP_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateStreamDescriptorSurfaceGGP_after(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateStreamDescriptorSurfaceGGP!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateScreenSurfaceQNX(VkInstance instance, VkScreenSurfaceCreateInfoQNX* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateScreenSurfaceQNX!");
}
#ifdef CREATESCREENSURFACEQNX_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateScreenSurfaceQNX_before(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateScreenSurfaceQNX(instance, pCreateInfo, pAllocator, pSurface);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSurface=" + ptrToString((void*)pSurface) + '!');
#ifdef CREATESCREENSURFACEQNX_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateScreenSurfaceQNX_after(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateScreenSurfaceQNX!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
 VK_LAYER_EXPORT VkBool32 VKAPI_CALL DetailsLayer_GetPhysicalDeviceScreenPresentationSupportQNX(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, _screen_window* window) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceScreenPresentationSupportQNX!");
}
#ifdef GETPHYSICALDEVICESCREENPRESENTATIONSUPPORTQNX_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceScreenPresentationSupportQNX_before(physicalDevice, queueFamilyIndex, window);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceScreenPresentationSupportQNX(physicalDevice, queueFamilyIndex, window);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"queueFamilyIndex=" + std::to_string(queueFamilyIndex) + '!');
winsockSendToUI(&ConnectSocket,"window=" + ptrToString((void*)window) + '!');
#ifdef GETPHYSICALDEVICESCREENPRESENTATIONSUPPORTQNX_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceScreenPresentationSupportQNX_after(physicalDevice, queueFamilyIndex, window);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceScreenPresentationSupportQNX!");
}
return ret;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateDebugReportCallbackEXT!");
}
#ifdef CREATEDEBUGREPORTCALLBACKEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateDebugReportCallbackEXT_before(instance, pCreateInfo, pAllocator, pCallback);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pCallback=" + ptrToString((void*)pCallback) + '!');
#ifdef CREATEDEBUGREPORTCALLBACKEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateDebugReportCallbackEXT_after(instance, pCreateInfo, pAllocator, pCallback);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateDebugReportCallbackEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyDebugReportCallbackEXT!");
}
#ifdef DESTROYDEBUGREPORTCALLBACKEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyDebugReportCallbackEXT_before(instance, callback, pAllocator);
}
#endif 
instance_dispatch[GetKey(instance)].DestroyDebugReportCallbackEXT(instance, callback, pAllocator);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"callback=" + ptrToString((void*)callback) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYDEBUGREPORTCALLBACKEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyDebugReportCallbackEXT_after(instance, callback, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyDebugReportCallbackEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, char* pLayerPrefix, char* pMessage) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDebugReportMessageEXT!");
}
#ifdef DEBUGREPORTMESSAGEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_DebugReportMessageEXT_before(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}
#endif 
instance_dispatch[GetKey(instance)].DebugReportMessageEXT(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"flags=" + ptrToString((void*)flags) + '!');
winsockSendToUI(&ConnectSocket,"objectType=" + std::to_string(objectType) + '!');
winsockSendToUI(&ConnectSocket,"object=" + std::to_string(object) + '!');
winsockSendToUI(&ConnectSocket,"location=" + std::to_string(location) + '!');
winsockSendToUI(&ConnectSocket,"messageCode=" + std::to_string(messageCode) + '!');
winsockSendToUI(&ConnectSocket,"pLayerPrefix=" + charToString(pLayerPrefix) + '!');
winsockSendToUI(&ConnectSocket,"pMessage=" + charToString(pMessage) + '!');
#ifdef DEBUGREPORTMESSAGEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_DebugReportMessageEXT_after(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDebugReportMessageEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceExternalImageFormatPropertiesNV!");
}
#ifdef GETPHYSICALDEVICEEXTERNALIMAGEFORMATPROPERTIESNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceExternalImageFormatPropertiesNV_before(physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceExternalImageFormatPropertiesNV(physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"format=" + std::to_string(format) + '!');
winsockSendToUI(&ConnectSocket,"type=" + std::to_string(type) + '!');
winsockSendToUI(&ConnectSocket,"tiling=" + std::to_string(tiling) + '!');
winsockSendToUI(&ConnectSocket,"usage=" + ptrToString((void*)usage) + '!');
winsockSendToUI(&ConnectSocket,"flags=" + ptrToString((void*)flags) + '!');
winsockSendToUI(&ConnectSocket,"externalHandleType=" + ptrToString((void*)externalHandleType) + '!');
winsockSendToUI(&ConnectSocket,"pExternalImageFormatProperties=" + ptrToString((void*)pExternalImageFormatProperties) + '!');
#ifdef GETPHYSICALDEVICEEXTERNALIMAGEFORMATPROPERTIESNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceExternalImageFormatPropertiesNV_after(physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceExternalImageFormatPropertiesNV!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceFeatures2!");
}
#ifdef GETPHYSICALDEVICEFEATURES2_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceFeatures2_before(physicalDevice, pFeatures);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceFeatures2(physicalDevice, pFeatures);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pFeatures=" + ptrToString((void*)pFeatures) + '!');
#ifdef GETPHYSICALDEVICEFEATURES2_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceFeatures2_after(physicalDevice, pFeatures);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceFeatures2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceProperties2!");
}
#ifdef GETPHYSICALDEVICEPROPERTIES2_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceProperties2_before(physicalDevice, pProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceProperties2(physicalDevice, pProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETPHYSICALDEVICEPROPERTIES2_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceProperties2_after(physicalDevice, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceProperties2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceFormatProperties2!");
}
#ifdef GETPHYSICALDEVICEFORMATPROPERTIES2_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceFormatProperties2_before(physicalDevice, format, pFormatProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceFormatProperties2(physicalDevice, format, pFormatProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"format=" + std::to_string(format) + '!');
winsockSendToUI(&ConnectSocket,"pFormatProperties=" + ptrToString((void*)pFormatProperties) + '!');
#ifdef GETPHYSICALDEVICEFORMATPROPERTIES2_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceFormatProperties2_after(physicalDevice, format, pFormatProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceFormatProperties2!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceImageFormatProperties2!");
}
#ifdef GETPHYSICALDEVICEIMAGEFORMATPROPERTIES2_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceImageFormatProperties2_before(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceImageFormatProperties2(physicalDevice, pImageFormatInfo, pImageFormatProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pImageFormatInfo=" + ptrToString((void*)pImageFormatInfo) + '!');
winsockSendToUI(&ConnectSocket,"pImageFormatProperties=" + ptrToString((void*)pImageFormatProperties) + '!');
#ifdef GETPHYSICALDEVICEIMAGEFORMATPROPERTIES2_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceImageFormatProperties2_after(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceImageFormatProperties2!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceQueueFamilyProperties2!");
}
#ifdef GETPHYSICALDEVICEQUEUEFAMILYPROPERTIES2_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceQueueFamilyProperties2_before(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceQueueFamilyProperties2(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pQueueFamilyPropertyCount=" + ptrToString((void*)pQueueFamilyPropertyCount) + '!');
winsockSendToUI(&ConnectSocket,"pQueueFamilyProperties=" + ptrToString((void*)pQueueFamilyProperties) + '!');
#ifdef GETPHYSICALDEVICEQUEUEFAMILYPROPERTIES2_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceQueueFamilyProperties2_after(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceQueueFamilyProperties2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceMemoryProperties2!");
}
#ifdef GETPHYSICALDEVICEMEMORYPROPERTIES2_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceMemoryProperties2_before(physicalDevice, pMemoryProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceMemoryProperties2(physicalDevice, pMemoryProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryProperties=" + ptrToString((void*)pMemoryProperties) + '!');
#ifdef GETPHYSICALDEVICEMEMORYPROPERTIES2_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceMemoryProperties2_after(physicalDevice, pMemoryProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceMemoryProperties2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceSparseImageFormatProperties2!");
}
#ifdef GETPHYSICALDEVICESPARSEIMAGEFORMATPROPERTIES2_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSparseImageFormatProperties2_before(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSparseImageFormatProperties2(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pFormatInfo=" + ptrToString((void*)pFormatInfo) + '!');
winsockSendToUI(&ConnectSocket,"pPropertyCount=" + ptrToString((void*)pPropertyCount) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETPHYSICALDEVICESPARSEIMAGEFORMATPROPERTIES2_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSparseImageFormatProperties2_after(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceSparseImageFormatProperties2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceExternalBufferProperties!");
}
#ifdef GETPHYSICALDEVICEEXTERNALBUFFERPROPERTIES_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceExternalBufferProperties_before(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceExternalBufferProperties(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pExternalBufferInfo=" + ptrToString((void*)pExternalBufferInfo) + '!');
winsockSendToUI(&ConnectSocket,"pExternalBufferProperties=" + ptrToString((void*)pExternalBufferProperties) + '!');
#ifdef GETPHYSICALDEVICEEXTERNALBUFFERPROPERTIES_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceExternalBufferProperties_after(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceExternalBufferProperties!");
}
}

#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceExternalMemorySciBufPropertiesNV(VkPhysicalDevice physicalDevice, VkExternalMemoryHandleTypeFlagBits handleType, NvSciBufObj handle, VkMemorySciBufPropertiesNV* pMemorySciBufProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV!");
}
#ifdef GETPHYSICALDEVICEEXTERNALMEMORYSCIBUFPROPERTIESNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceExternalMemorySciBufPropertiesNV_before(physicalDevice, handleType, handle, pMemorySciBufProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceExternalMemorySciBufPropertiesNV(physicalDevice, handleType, handle, pMemorySciBufProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"handleType=" + std::to_string(handleType) + '!');
winsockSendToUI(&ConnectSocket,"handle=" + ptrToString((void*)handle) + '!');
winsockSendToUI(&ConnectSocket,"pMemorySciBufProperties=" + ptrToString((void*)pMemorySciBufProperties) + '!');
#ifdef GETPHYSICALDEVICEEXTERNALMEMORYSCIBUFPROPERTIESNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceExternalMemorySciBufPropertiesNV_after(physicalDevice, handleType, handle, pMemorySciBufProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSciBufAttributesNV(VkPhysicalDevice physicalDevice, NvSciBufAttrList pAttributes) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceSciBufAttributesNV!");
}
#ifdef GETPHYSICALDEVICESCIBUFATTRIBUTESNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSciBufAttributesNV_before(physicalDevice, pAttributes);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSciBufAttributesNV(physicalDevice, pAttributes);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pAttributes=" + ptrToString((void*)pAttributes) + '!');
#ifdef GETPHYSICALDEVICESCIBUFATTRIBUTESNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSciBufAttributesNV_after(physicalDevice, pAttributes);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceSciBufAttributesNV!");
}
return ret;
}

#endif
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceExternalSemaphoreProperties!");
}
#ifdef GETPHYSICALDEVICEEXTERNALSEMAPHOREPROPERTIES_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceExternalSemaphoreProperties_before(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceExternalSemaphoreProperties(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pExternalSemaphoreInfo=" + ptrToString((void*)pExternalSemaphoreInfo) + '!');
winsockSendToUI(&ConnectSocket,"pExternalSemaphoreProperties=" + ptrToString((void*)pExternalSemaphoreProperties) + '!');
#ifdef GETPHYSICALDEVICEEXTERNALSEMAPHOREPROPERTIES_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceExternalSemaphoreProperties_after(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceExternalSemaphoreProperties!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceExternalFenceProperties!");
}
#ifdef GETPHYSICALDEVICEEXTERNALFENCEPROPERTIES_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceExternalFenceProperties_before(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceExternalFenceProperties(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pExternalFenceInfo=" + ptrToString((void*)pExternalFenceInfo) + '!');
winsockSendToUI(&ConnectSocket,"pExternalFenceProperties=" + ptrToString((void*)pExternalFenceProperties) + '!');
#ifdef GETPHYSICALDEVICEEXTERNALFENCEPROPERTIES_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceExternalFenceProperties_after(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceExternalFenceProperties!");
}
}

#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSciSyncAttributesNV(VkPhysicalDevice physicalDevice, VkSciSyncAttributesInfoNV* pSciSyncAttributesInfo, NvSciSyncAttrList pAttributes) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceSciSyncAttributesNV!");
}
#ifdef GETPHYSICALDEVICESCISYNCATTRIBUTESNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSciSyncAttributesNV_before(physicalDevice, pSciSyncAttributesInfo, pAttributes);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSciSyncAttributesNV(physicalDevice, pSciSyncAttributesInfo, pAttributes);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pSciSyncAttributesInfo=" + ptrToString((void*)pSciSyncAttributesInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAttributes=" + ptrToString((void*)pAttributes) + '!');
#ifdef GETPHYSICALDEVICESCISYNCATTRIBUTESNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSciSyncAttributesNV_after(physicalDevice, pSciSyncAttributesInfo, pAttributes);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceSciSyncAttributesNV!");
}
return ret;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkReleaseDisplayEXT!");
}
#ifdef RELEASEDISPLAYEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_ReleaseDisplayEXT_before(physicalDevice, display);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].ReleaseDisplayEXT(physicalDevice, display);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"display=" + ptrToString((void*)display) + '!');
#ifdef RELEASEDISPLAYEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_ReleaseDisplayEXT_after(physicalDevice, display);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkReleaseDisplayEXT!");
}
return ret;
}

#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquireXlibDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkAcquireXlibDisplayEXT!");
}
#ifdef ACQUIREXLIBDISPLAYEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_AcquireXlibDisplayEXT_before(physicalDevice, dpy, display);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].AcquireXlibDisplayEXT(physicalDevice, dpy, display);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"dpy=" + ptrToString((void*)dpy) + '!');
winsockSendToUI(&ConnectSocket,"display=" + ptrToString((void*)display) + '!');
#ifdef ACQUIREXLIBDISPLAYEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_AcquireXlibDisplayEXT_after(physicalDevice, dpy, display);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkAcquireXlibDisplayEXT!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetRandROutputDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetRandROutputDisplayEXT!");
}
#ifdef GETRANDROUTPUTDISPLAYEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetRandROutputDisplayEXT_before(physicalDevice, dpy, rrOutput, pDisplay);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetRandROutputDisplayEXT(physicalDevice, dpy, rrOutput, pDisplay);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"dpy=" + ptrToString((void*)dpy) + '!');
winsockSendToUI(&ConnectSocket,"rrOutput=" + ptrToString((void*)rrOutput) + '!');
winsockSendToUI(&ConnectSocket,"pDisplay=" + ptrToString((void*)pDisplay) + '!');
#ifdef GETRANDROUTPUTDISPLAYEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetRandROutputDisplayEXT_after(physicalDevice, dpy, rrOutput, pDisplay);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetRandROutputDisplayEXT!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkAcquireWinrtDisplayNV!");
}
#ifdef ACQUIREWINRTDISPLAYNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_AcquireWinrtDisplayNV_before(physicalDevice, display);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].AcquireWinrtDisplayNV(physicalDevice, display);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"display=" + ptrToString((void*)display) + '!');
#ifdef ACQUIREWINRTDISPLAYNV_AFTER_EXEC_EXISTS
if(connected) {
layer_AcquireWinrtDisplayNV_after(physicalDevice, display);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkAcquireWinrtDisplayNV!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetWinrtDisplayNV!");
}
#ifdef GETWINRTDISPLAYNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetWinrtDisplayNV_before(physicalDevice, deviceRelativeId, pDisplay);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetWinrtDisplayNV(physicalDevice, deviceRelativeId, pDisplay);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"deviceRelativeId=" + std::to_string(deviceRelativeId) + '!');
winsockSendToUI(&ConnectSocket,"pDisplay=" + ptrToString((void*)pDisplay) + '!');
#ifdef GETWINRTDISPLAYNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetWinrtDisplayNV_after(physicalDevice, deviceRelativeId, pDisplay);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetWinrtDisplayNV!");
}
return ret;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceSurfaceCapabilities2EXT!");
}
#ifdef GETPHYSICALDEVICESURFACECAPABILITIES2EXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSurfaceCapabilities2EXT_before(physicalDevice, surface, pSurfaceCapabilities);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSurfaceCapabilities2EXT(physicalDevice, surface, pSurfaceCapabilities);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"surface=" + ptrToString((void*)surface) + '!');
winsockSendToUI(&ConnectSocket,"pSurfaceCapabilities=" + ptrToString((void*)pSurfaceCapabilities) + '!');
#ifdef GETPHYSICALDEVICESURFACECAPABILITIES2EXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSurfaceCapabilities2EXT_after(physicalDevice, surface, pSurfaceCapabilities);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceSurfaceCapabilities2EXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkEnumeratePhysicalDeviceGroups!");
}
#ifdef ENUMERATEPHYSICALDEVICEGROUPS_BEFORE_EXEC_EXISTS
if(connected) {
layer_EnumeratePhysicalDeviceGroups_before(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].EnumeratePhysicalDeviceGroups(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pPhysicalDeviceGroupCount=" + ptrToString((void*)pPhysicalDeviceGroupCount) + '!');
winsockSendToUI(&ConnectSocket,"pPhysicalDeviceGroupProperties=" + ptrToString((void*)pPhysicalDeviceGroupProperties) + '!');
#ifdef ENUMERATEPHYSICALDEVICEGROUPS_AFTER_EXEC_EXISTS
if(connected) {
layer_EnumeratePhysicalDeviceGroups_after(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkEnumeratePhysicalDeviceGroups!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDevicePresentRectanglesKHR!");
}
#ifdef GETPHYSICALDEVICEPRESENTRECTANGLESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDevicePresentRectanglesKHR_before(physicalDevice, surface, pRectCount, pRects);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDevicePresentRectanglesKHR(physicalDevice, surface, pRectCount, pRects);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"surface=" + ptrToString((void*)surface) + '!');
winsockSendToUI(&ConnectSocket,"pRectCount=" + ptrToString((void*)pRectCount) + '!');
winsockSendToUI(&ConnectSocket,"pRects=" + ptrToString((void*)pRects) + '!');
#ifdef GETPHYSICALDEVICEPRESENTRECTANGLESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDevicePresentRectanglesKHR_after(physicalDevice, surface, pRectCount, pRects);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDevicePresentRectanglesKHR!");
}
return ret;
}

#if defined(VK_USE_PLATFORM_IOS_MVK)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateIOSSurfaceMVK(VkInstance instance, VkIOSSurfaceCreateInfoMVK* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateIOSSurfaceMVK!");
}
#ifdef CREATEIOSSURFACEMVK_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateIOSSurfaceMVK_before(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateIOSSurfaceMVK(instance, pCreateInfo, pAllocator, pSurface);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSurface=" + ptrToString((void*)pSurface) + '!');
#ifdef CREATEIOSSURFACEMVK_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateIOSSurfaceMVK_after(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateIOSSurfaceMVK!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_MACOS_MVK)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateMacOSSurfaceMVK(VkInstance instance, VkMacOSSurfaceCreateInfoMVK* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateMacOSSurfaceMVK!");
}
#ifdef CREATEMACOSSURFACEMVK_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateMacOSSurfaceMVK_before(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateMacOSSurfaceMVK(instance, pCreateInfo, pAllocator, pSurface);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSurface=" + ptrToString((void*)pSurface) + '!');
#ifdef CREATEMACOSSURFACEMVK_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateMacOSSurfaceMVK_after(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateMacOSSurfaceMVK!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_METAL_EXT)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateMetalSurfaceEXT(VkInstance instance, VkMetalSurfaceCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateMetalSurfaceEXT!");
}
#ifdef CREATEMETALSURFACEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateMetalSurfaceEXT_before(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateMetalSurfaceEXT(instance, pCreateInfo, pAllocator, pSurface);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSurface=" + ptrToString((void*)pSurface) + '!');
#ifdef CREATEMETALSURFACEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateMetalSurfaceEXT_after(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateMetalSurfaceEXT!");
}
return ret;
}

#endif
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceMultisamplePropertiesEXT!");
}
#ifdef GETPHYSICALDEVICEMULTISAMPLEPROPERTIESEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceMultisamplePropertiesEXT_before(physicalDevice, samples, pMultisampleProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceMultisamplePropertiesEXT(physicalDevice, samples, pMultisampleProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"samples=" + std::to_string(samples) + '!');
winsockSendToUI(&ConnectSocket,"pMultisampleProperties=" + ptrToString((void*)pMultisampleProperties) + '!');
#ifdef GETPHYSICALDEVICEMULTISAMPLEPROPERTIESEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceMultisamplePropertiesEXT_after(physicalDevice, samples, pMultisampleProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceMultisamplePropertiesEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceSurfaceCapabilities2KHR!");
}
#ifdef GETPHYSICALDEVICESURFACECAPABILITIES2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSurfaceCapabilities2KHR_before(physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pSurfaceInfo=" + ptrToString((void*)pSurfaceInfo) + '!');
winsockSendToUI(&ConnectSocket,"pSurfaceCapabilities=" + ptrToString((void*)pSurfaceCapabilities) + '!');
#ifdef GETPHYSICALDEVICESURFACECAPABILITIES2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSurfaceCapabilities2KHR_after(physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceSurfaceCapabilities2KHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceSurfaceFormats2KHR!");
}
#ifdef GETPHYSICALDEVICESURFACEFORMATS2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSurfaceFormats2KHR_before(physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSurfaceFormats2KHR(physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pSurfaceInfo=" + ptrToString((void*)pSurfaceInfo) + '!');
winsockSendToUI(&ConnectSocket,"pSurfaceFormatCount=" + ptrToString((void*)pSurfaceFormatCount) + '!');
winsockSendToUI(&ConnectSocket,"pSurfaceFormats=" + ptrToString((void*)pSurfaceFormats) + '!');
#ifdef GETPHYSICALDEVICESURFACEFORMATS2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSurfaceFormats2KHR_after(physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceSurfaceFormats2KHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceDisplayProperties2KHR!");
}
#ifdef GETPHYSICALDEVICEDISPLAYPROPERTIES2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceDisplayProperties2KHR_before(physicalDevice, pPropertyCount, pProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceDisplayProperties2KHR(physicalDevice, pPropertyCount, pProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pPropertyCount=" + ptrToString((void*)pPropertyCount) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETPHYSICALDEVICEDISPLAYPROPERTIES2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceDisplayProperties2KHR_after(physicalDevice, pPropertyCount, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceDisplayProperties2KHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceDisplayPlaneProperties2KHR!");
}
#ifdef GETPHYSICALDEVICEDISPLAYPLANEPROPERTIES2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceDisplayPlaneProperties2KHR_before(physicalDevice, pPropertyCount, pProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceDisplayPlaneProperties2KHR(physicalDevice, pPropertyCount, pProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pPropertyCount=" + ptrToString((void*)pPropertyCount) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETPHYSICALDEVICEDISPLAYPLANEPROPERTIES2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceDisplayPlaneProperties2KHR_after(physicalDevice, pPropertyCount, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceDisplayPlaneProperties2KHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDisplayModeProperties2KHR!");
}
#ifdef GETDISPLAYMODEPROPERTIES2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDisplayModeProperties2KHR_before(physicalDevice, display, pPropertyCount, pProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetDisplayModeProperties2KHR(physicalDevice, display, pPropertyCount, pProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"display=" + ptrToString((void*)display) + '!');
winsockSendToUI(&ConnectSocket,"pPropertyCount=" + ptrToString((void*)pPropertyCount) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETDISPLAYMODEPROPERTIES2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDisplayModeProperties2KHR_after(physicalDevice, display, pPropertyCount, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDisplayModeProperties2KHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDisplayPlaneCapabilities2KHR!");
}
#ifdef GETDISPLAYPLANECAPABILITIES2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDisplayPlaneCapabilities2KHR_before(physicalDevice, pDisplayPlaneInfo, pCapabilities);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetDisplayPlaneCapabilities2KHR(physicalDevice, pDisplayPlaneInfo, pCapabilities);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pDisplayPlaneInfo=" + ptrToString((void*)pDisplayPlaneInfo) + '!');
winsockSendToUI(&ConnectSocket,"pCapabilities=" + ptrToString((void*)pCapabilities) + '!');
#ifdef GETDISPLAYPLANECAPABILITIES2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDisplayPlaneCapabilities2KHR_after(physicalDevice, pDisplayPlaneInfo, pCapabilities);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDisplayPlaneCapabilities2KHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceCalibrateableTimeDomainsKHR(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR!");
}
#ifdef GETPHYSICALDEVICECALIBRATEABLETIMEDOMAINSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceCalibrateableTimeDomainsKHR_before(physicalDevice, pTimeDomainCount, pTimeDomains);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceCalibrateableTimeDomainsKHR(physicalDevice, pTimeDomainCount, pTimeDomains);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pTimeDomainCount=" + ptrToString((void*)pTimeDomainCount) + '!');
winsockSendToUI(&ConnectSocket,"pTimeDomains=" + ptrToString((void*)pTimeDomains) + '!');
#ifdef GETPHYSICALDEVICECALIBRATEABLETIMEDOMAINSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceCalibrateableTimeDomainsKHR_after(physicalDevice, pTimeDomainCount, pTimeDomains);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateDebugUtilsMessengerEXT!");
}
#ifdef CREATEDEBUGUTILSMESSENGEREXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateDebugUtilsMessengerEXT_before(instance, pCreateInfo, pAllocator, pMessenger);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pMessenger=" + ptrToString((void*)pMessenger) + '!');
#ifdef CREATEDEBUGUTILSMESSENGEREXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateDebugUtilsMessengerEXT_after(instance, pCreateInfo, pAllocator, pMessenger);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateDebugUtilsMessengerEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyDebugUtilsMessengerEXT!");
}
#ifdef DESTROYDEBUGUTILSMESSENGEREXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyDebugUtilsMessengerEXT_before(instance, messenger, pAllocator);
}
#endif 
instance_dispatch[GetKey(instance)].DestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"messenger=" + ptrToString((void*)messenger) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYDEBUGUTILSMESSENGEREXT_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyDebugUtilsMessengerEXT_after(instance, messenger, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyDebugUtilsMessengerEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_SubmitDebugUtilsMessageEXT(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkSubmitDebugUtilsMessageEXT!");
}
#ifdef SUBMITDEBUGUTILSMESSAGEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_SubmitDebugUtilsMessageEXT_before(instance, messageSeverity, messageTypes, pCallbackData);
}
#endif 
instance_dispatch[GetKey(instance)].SubmitDebugUtilsMessageEXT(instance, messageSeverity, messageTypes, pCallbackData);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"messageSeverity=" + std::to_string(messageSeverity) + '!');
winsockSendToUI(&ConnectSocket,"messageTypes=" + ptrToString((void*)messageTypes) + '!');
winsockSendToUI(&ConnectSocket,"pCallbackData=" + ptrToString((void*)pCallbackData) + '!');
#ifdef SUBMITDEBUGUTILSMESSAGEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_SubmitDebugUtilsMessageEXT_after(instance, messageSeverity, messageTypes, pCallbackData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkSubmitDebugUtilsMessageEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV!");
}
#ifdef GETPHYSICALDEVICECOOPERATIVEMATRIXPROPERTIESNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceCooperativeMatrixPropertiesNV_before(physicalDevice, pPropertyCount, pProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceCooperativeMatrixPropertiesNV(physicalDevice, pPropertyCount, pProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pPropertyCount=" + ptrToString((void*)pPropertyCount) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETPHYSICALDEVICECOOPERATIVEMATRIXPROPERTIESNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceCooperativeMatrixPropertiesNV_after(physicalDevice, pPropertyCount, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV!");
}
return ret;
}

#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSurfacePresentModes2EXT(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceSurfacePresentModes2EXT!");
}
#ifdef GETPHYSICALDEVICESURFACEPRESENTMODES2EXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSurfacePresentModes2EXT_before(physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSurfacePresentModes2EXT(physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pSurfaceInfo=" + ptrToString((void*)pSurfaceInfo) + '!');
winsockSendToUI(&ConnectSocket,"pPresentModeCount=" + ptrToString((void*)pPresentModeCount) + '!');
winsockSendToUI(&ConnectSocket,"pPresentModes=" + ptrToString((void*)pPresentModes) + '!');
#ifdef GETPHYSICALDEVICESURFACEPRESENTMODES2EXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSurfacePresentModes2EXT_after(physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceSurfacePresentModes2EXT!");
}
return ret;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR!");
}
#ifdef ENUMERATEPHYSICALDEVICEQUEUEFAMILYPERFORMANCEQUERYCOUNTERSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR_before(physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"queueFamilyIndex=" + std::to_string(queueFamilyIndex) + '!');
winsockSendToUI(&ConnectSocket,"pCounterCount=" + ptrToString((void*)pCounterCount) + '!');
winsockSendToUI(&ConnectSocket,"pCounters=" + ptrToString((void*)pCounters) + '!');
winsockSendToUI(&ConnectSocket,"pCounterDescriptions=" + ptrToString((void*)pCounterDescriptions) + '!');
#ifdef ENUMERATEPHYSICALDEVICEQUEUEFAMILYPERFORMANCEQUERYCOUNTERSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR_after(physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice, VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR!");
}
#ifdef GETPHYSICALDEVICEQUEUEFAMILYPERFORMANCEQUERYPASSESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR_before(physicalDevice, pPerformanceQueryCreateInfo, pNumPasses);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(physicalDevice, pPerformanceQueryCreateInfo, pNumPasses);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pPerformanceQueryCreateInfo=" + ptrToString((void*)pPerformanceQueryCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pNumPasses=" + ptrToString((void*)pNumPasses) + '!');
#ifdef GETPHYSICALDEVICEQUEUEFAMILYPERFORMANCEQUERYPASSESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR_after(physicalDevice, pPerformanceQueryCreateInfo, pNumPasses);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateHeadlessSurfaceEXT(VkInstance instance, VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateHeadlessSurfaceEXT!");
}
#ifdef CREATEHEADLESSSURFACEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateHeadlessSurfaceEXT_before(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].CreateHeadlessSurfaceEXT(instance, pCreateInfo, pAllocator, pSurface);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSurface=" + ptrToString((void*)pSurface) + '!');
#ifdef CREATEHEADLESSSURFACEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateHeadlessSurfaceEXT_after(instance, pCreateInfo, pAllocator, pSurface);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateHeadlessSurfaceEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV!");
}
#ifdef GETPHYSICALDEVICESUPPORTEDFRAMEBUFFERMIXEDSAMPLESCOMBINATIONSNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV_before(physicalDevice, pCombinationCount, pCombinations);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(physicalDevice, pCombinationCount, pCombinations);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pCombinationCount=" + ptrToString((void*)pCombinationCount) + '!');
winsockSendToUI(&ConnectSocket,"pCombinations=" + ptrToString((void*)pCombinations) + '!');
#ifdef GETPHYSICALDEVICESUPPORTEDFRAMEBUFFERMIXEDSAMPLESCOMBINATIONSNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV_after(physicalDevice, pCombinationCount, pCombinations);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceToolProperties!");
}
#ifdef GETPHYSICALDEVICETOOLPROPERTIES_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceToolProperties_before(physicalDevice, pToolCount, pToolProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceToolProperties(physicalDevice, pToolCount, pToolProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pToolCount=" + ptrToString((void*)pToolCount) + '!');
winsockSendToUI(&ConnectSocket,"pToolProperties=" + ptrToString((void*)pToolProperties) + '!');
#ifdef GETPHYSICALDEVICETOOLPROPERTIES_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceToolProperties_after(physicalDevice, pToolCount, pToolProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceToolProperties!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceFragmentShadingRatesKHR!");
}
#ifdef GETPHYSICALDEVICEFRAGMENTSHADINGRATESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceFragmentShadingRatesKHR_before(physicalDevice, pFragmentShadingRateCount, pFragmentShadingRates);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceFragmentShadingRatesKHR(physicalDevice, pFragmentShadingRateCount, pFragmentShadingRates);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pFragmentShadingRateCount=" + ptrToString((void*)pFragmentShadingRateCount) + '!');
winsockSendToUI(&ConnectSocket,"pFragmentShadingRates=" + ptrToString((void*)pFragmentShadingRates) + '!');
#ifdef GETPHYSICALDEVICEFRAGMENTSHADINGRATESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceFragmentShadingRatesKHR_after(physicalDevice, pFragmentShadingRateCount, pFragmentShadingRates);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceFragmentShadingRatesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceVideoCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceVideoCapabilitiesKHR!");
}
#ifdef GETPHYSICALDEVICEVIDEOCAPABILITIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceVideoCapabilitiesKHR_before(physicalDevice, pVideoProfile, pCapabilities);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceVideoCapabilitiesKHR(physicalDevice, pVideoProfile, pCapabilities);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pVideoProfile=" + ptrToString((void*)pVideoProfile) + '!');
winsockSendToUI(&ConnectSocket,"pCapabilities=" + ptrToString((void*)pCapabilities) + '!');
#ifdef GETPHYSICALDEVICEVIDEOCAPABILITIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceVideoCapabilitiesKHR_after(physicalDevice, pVideoProfile, pCapabilities);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceVideoCapabilitiesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceVideoFormatPropertiesKHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceVideoFormatPropertiesKHR!");
}
#ifdef GETPHYSICALDEVICEVIDEOFORMATPROPERTIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceVideoFormatPropertiesKHR_before(physicalDevice, pVideoFormatInfo, pVideoFormatPropertyCount, pVideoFormatProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceVideoFormatPropertiesKHR(physicalDevice, pVideoFormatInfo, pVideoFormatPropertyCount, pVideoFormatProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pVideoFormatInfo=" + ptrToString((void*)pVideoFormatInfo) + '!');
winsockSendToUI(&ConnectSocket,"pVideoFormatPropertyCount=" + ptrToString((void*)pVideoFormatPropertyCount) + '!');
winsockSendToUI(&ConnectSocket,"pVideoFormatProperties=" + ptrToString((void*)pVideoFormatProperties) + '!');
#ifdef GETPHYSICALDEVICEVIDEOFORMATPROPERTIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceVideoFormatPropertiesKHR_after(physicalDevice, pVideoFormatInfo, pVideoFormatPropertyCount, pVideoFormatProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceVideoFormatPropertiesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR* pQualityLevelInfo, VkVideoEncodeQualityLevelPropertiesKHR* pQualityLevelProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR!");
}
#ifdef GETPHYSICALDEVICEVIDEOENCODEQUALITYLEVELPROPERTIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR_before(physicalDevice, pQualityLevelInfo, pQualityLevelProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(physicalDevice, pQualityLevelInfo, pQualityLevelProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pQualityLevelInfo=" + ptrToString((void*)pQualityLevelInfo) + '!');
winsockSendToUI(&ConnectSocket,"pQualityLevelProperties=" + ptrToString((void*)pQualityLevelProperties) + '!');
#ifdef GETPHYSICALDEVICEVIDEOENCODEQUALITYLEVELPROPERTIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR_after(physicalDevice, pQualityLevelInfo, pQualityLevelProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkAcquireDrmDisplayEXT!");
}
#ifdef ACQUIREDRMDISPLAYEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_AcquireDrmDisplayEXT_before(physicalDevice, drmFd, display);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].AcquireDrmDisplayEXT(physicalDevice, drmFd, display);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"drmFd=" + std::to_string(drmFd) + '!');
winsockSendToUI(&ConnectSocket,"display=" + ptrToString((void*)display) + '!');
#ifdef ACQUIREDRMDISPLAYEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_AcquireDrmDisplayEXT_after(physicalDevice, drmFd, display);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkAcquireDrmDisplayEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDrmDisplayEXT!");
}
#ifdef GETDRMDISPLAYEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDrmDisplayEXT_before(physicalDevice, drmFd, connectorId, display);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetDrmDisplayEXT(physicalDevice, drmFd, connectorId, display);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"drmFd=" + std::to_string(drmFd) + '!');
winsockSendToUI(&ConnectSocket,"connectorId=" + std::to_string(connectorId) + '!');
winsockSendToUI(&ConnectSocket,"display=" + ptrToString((void*)display) + '!');
#ifdef GETDRMDISPLAYEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDrmDisplayEXT_after(physicalDevice, drmFd, connectorId, display);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDrmDisplayEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceOpticalFlowImageFormatsNV(VkPhysicalDevice physicalDevice, VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceOpticalFlowImageFormatsNV!");
}
#ifdef GETPHYSICALDEVICEOPTICALFLOWIMAGEFORMATSNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceOpticalFlowImageFormatsNV_before(physicalDevice, pOpticalFlowImageFormatInfo, pFormatCount, pImageFormatProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceOpticalFlowImageFormatsNV(physicalDevice, pOpticalFlowImageFormatInfo, pFormatCount, pImageFormatProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pOpticalFlowImageFormatInfo=" + ptrToString((void*)pOpticalFlowImageFormatInfo) + '!');
winsockSendToUI(&ConnectSocket,"pFormatCount=" + ptrToString((void*)pFormatCount) + '!');
winsockSendToUI(&ConnectSocket,"pImageFormatProperties=" + ptrToString((void*)pImageFormatProperties) + '!');
#ifdef GETPHYSICALDEVICEOPTICALFLOWIMAGEFORMATSNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceOpticalFlowImageFormatsNV_after(physicalDevice, pOpticalFlowImageFormatInfo, pFormatCount, pImageFormatProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceOpticalFlowImageFormatsNV!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceCooperativeMatrixPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR!");
}
#ifdef GETPHYSICALDEVICECOOPERATIVEMATRIXPROPERTIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceCooperativeMatrixPropertiesKHR_before(physicalDevice, pPropertyCount, pProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceCooperativeMatrixPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pPropertyCount=" + ptrToString((void*)pPropertyCount) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETPHYSICALDEVICECOOPERATIVEMATRIXPROPERTIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceCooperativeMatrixPropertiesKHR_after(physicalDevice, pPropertyCount, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceFeatures2KHR!");
}
#ifdef GETPHYSICALDEVICEFEATURES2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceFeatures2KHR_before(physicalDevice, pFeatures);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceFeatures2KHR(physicalDevice, pFeatures);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pFeatures=" + ptrToString((void*)pFeatures) + '!');
#ifdef GETPHYSICALDEVICEFEATURES2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceFeatures2KHR_after(physicalDevice, pFeatures);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceFeatures2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceProperties2KHR!");
}
#ifdef GETPHYSICALDEVICEPROPERTIES2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceProperties2KHR_before(physicalDevice, pProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceProperties2KHR(physicalDevice, pProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETPHYSICALDEVICEPROPERTIES2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceProperties2KHR_after(physicalDevice, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceProperties2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceFormatProperties2KHR!");
}
#ifdef GETPHYSICALDEVICEFORMATPROPERTIES2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceFormatProperties2KHR_before(physicalDevice, format, pFormatProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceFormatProperties2KHR(physicalDevice, format, pFormatProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"format=" + std::to_string(format) + '!');
winsockSendToUI(&ConnectSocket,"pFormatProperties=" + ptrToString((void*)pFormatProperties) + '!');
#ifdef GETPHYSICALDEVICEFORMATPROPERTIES2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceFormatProperties2KHR_after(physicalDevice, format, pFormatProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceFormatProperties2KHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceImageFormatProperties2KHR!");
}
#ifdef GETPHYSICALDEVICEIMAGEFORMATPROPERTIES2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceImageFormatProperties2KHR_before(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceImageFormatProperties2KHR(physicalDevice, pImageFormatInfo, pImageFormatProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pImageFormatInfo=" + ptrToString((void*)pImageFormatInfo) + '!');
winsockSendToUI(&ConnectSocket,"pImageFormatProperties=" + ptrToString((void*)pImageFormatProperties) + '!');
#ifdef GETPHYSICALDEVICEIMAGEFORMATPROPERTIES2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceImageFormatProperties2KHR_after(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceImageFormatProperties2KHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceQueueFamilyProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceQueueFamilyProperties2KHR!");
}
#ifdef GETPHYSICALDEVICEQUEUEFAMILYPROPERTIES2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceQueueFamilyProperties2KHR_before(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceQueueFamilyProperties2KHR(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pQueueFamilyPropertyCount=" + ptrToString((void*)pQueueFamilyPropertyCount) + '!');
winsockSendToUI(&ConnectSocket,"pQueueFamilyProperties=" + ptrToString((void*)pQueueFamilyProperties) + '!');
#ifdef GETPHYSICALDEVICEQUEUEFAMILYPROPERTIES2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceQueueFamilyProperties2KHR_after(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceQueueFamilyProperties2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceMemoryProperties2KHR!");
}
#ifdef GETPHYSICALDEVICEMEMORYPROPERTIES2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceMemoryProperties2KHR_before(physicalDevice, pMemoryProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceMemoryProperties2KHR(physicalDevice, pMemoryProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryProperties=" + ptrToString((void*)pMemoryProperties) + '!');
#ifdef GETPHYSICALDEVICEMEMORYPROPERTIES2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceMemoryProperties2KHR_after(physicalDevice, pMemoryProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceMemoryProperties2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceSparseImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceSparseImageFormatProperties2KHR!");
}
#ifdef GETPHYSICALDEVICESPARSEIMAGEFORMATPROPERTIES2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSparseImageFormatProperties2KHR_before(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSparseImageFormatProperties2KHR(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pFormatInfo=" + ptrToString((void*)pFormatInfo) + '!');
winsockSendToUI(&ConnectSocket,"pPropertyCount=" + ptrToString((void*)pPropertyCount) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETPHYSICALDEVICESPARSEIMAGEFORMATPROPERTIES2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceSparseImageFormatProperties2KHR_after(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceSparseImageFormatProperties2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceExternalBufferPropertiesKHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceExternalBufferPropertiesKHR!");
}
#ifdef GETPHYSICALDEVICEEXTERNALBUFFERPROPERTIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceExternalBufferPropertiesKHR_before(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceExternalBufferPropertiesKHR(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pExternalBufferInfo=" + ptrToString((void*)pExternalBufferInfo) + '!');
winsockSendToUI(&ConnectSocket,"pExternalBufferProperties=" + ptrToString((void*)pExternalBufferProperties) + '!');
#ifdef GETPHYSICALDEVICEEXTERNALBUFFERPROPERTIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceExternalBufferPropertiesKHR_after(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceExternalBufferPropertiesKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceExternalSemaphorePropertiesKHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR!");
}
#ifdef GETPHYSICALDEVICEEXTERNALSEMAPHOREPROPERTIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceExternalSemaphorePropertiesKHR_before(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceExternalSemaphorePropertiesKHR(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pExternalSemaphoreInfo=" + ptrToString((void*)pExternalSemaphoreInfo) + '!');
winsockSendToUI(&ConnectSocket,"pExternalSemaphoreProperties=" + ptrToString((void*)pExternalSemaphoreProperties) + '!');
#ifdef GETPHYSICALDEVICEEXTERNALSEMAPHOREPROPERTIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceExternalSemaphorePropertiesKHR_after(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceExternalFencePropertiesKHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceExternalFencePropertiesKHR!");
}
#ifdef GETPHYSICALDEVICEEXTERNALFENCEPROPERTIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceExternalFencePropertiesKHR_before(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceExternalFencePropertiesKHR(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pExternalFenceInfo=" + ptrToString((void*)pExternalFenceInfo) + '!');
winsockSendToUI(&ConnectSocket,"pExternalFenceProperties=" + ptrToString((void*)pExternalFenceProperties) + '!');
#ifdef GETPHYSICALDEVICEEXTERNALFENCEPROPERTIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceExternalFencePropertiesKHR_after(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceExternalFencePropertiesKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumeratePhysicalDeviceGroupsKHR(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkEnumeratePhysicalDeviceGroupsKHR!");
}
#ifdef ENUMERATEPHYSICALDEVICEGROUPSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_EnumeratePhysicalDeviceGroupsKHR_before(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(instance)].EnumeratePhysicalDeviceGroupsKHR(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
winsockSendToUI(&ConnectSocket,"instance=" + ptrToString((void*)instance) + '!');
winsockSendToUI(&ConnectSocket,"pPhysicalDeviceGroupCount=" + ptrToString((void*)pPhysicalDeviceGroupCount) + '!');
winsockSendToUI(&ConnectSocket,"pPhysicalDeviceGroupProperties=" + ptrToString((void*)pPhysicalDeviceGroupProperties) + '!');
#ifdef ENUMERATEPHYSICALDEVICEGROUPSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_EnumeratePhysicalDeviceGroupsKHR_after(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkEnumeratePhysicalDeviceGroupsKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT!");
}
#ifdef GETPHYSICALDEVICECALIBRATEABLETIMEDOMAINSEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceCalibrateableTimeDomainsEXT_before(physicalDevice, pTimeDomainCount, pTimeDomains);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceCalibrateableTimeDomainsEXT(physicalDevice, pTimeDomainCount, pTimeDomains);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pTimeDomainCount=" + ptrToString((void*)pTimeDomainCount) + '!');
winsockSendToUI(&ConnectSocket,"pTimeDomains=" + ptrToString((void*)pTimeDomains) + '!');
#ifdef GETPHYSICALDEVICECALIBRATEABLETIMEDOMAINSEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceCalibrateableTimeDomainsEXT_after(physicalDevice, pTimeDomainCount, pTimeDomains);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPhysicalDeviceToolPropertiesEXT!");
}
#ifdef GETPHYSICALDEVICETOOLPROPERTIESEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceToolPropertiesEXT_before(physicalDevice, pToolCount, pToolProperties);
}
#endif 
auto ret = instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceToolPropertiesEXT(physicalDevice, pToolCount, pToolProperties);
winsockSendToUI(&ConnectSocket,"physicalDevice=" + ptrToString((void*)physicalDevice) + '!');
winsockSendToUI(&ConnectSocket,"pToolCount=" + ptrToString((void*)pToolCount) + '!');
winsockSendToUI(&ConnectSocket,"pToolProperties=" + ptrToString((void*)pToolProperties) + '!');
#ifdef GETPHYSICALDEVICETOOLPROPERTIESEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPhysicalDeviceToolPropertiesEXT_after(physicalDevice, pToolCount, pToolProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPhysicalDeviceToolPropertiesEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceQueue!");
}
#ifdef GETDEVICEQUEUE_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceQueue_before(device, queueFamilyIndex, queueIndex, pQueue);
}
#endif 
device_dispatch[GetKey(device)].GetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"queueFamilyIndex=" + std::to_string(queueFamilyIndex) + '!');
winsockSendToUI(&ConnectSocket,"queueIndex=" + std::to_string(queueIndex) + '!');
winsockSendToUI(&ConnectSocket,"pQueue=" + ptrToString((void*)pQueue) + '!');
#ifdef GETDEVICEQUEUE_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceQueue_after(device, queueFamilyIndex, queueIndex, pQueue);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceQueue!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_QueueSubmit(VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkQueueSubmit!");
}
#ifdef QUEUESUBMIT_BEFORE_EXEC_EXISTS
if(connected) {
layer_QueueSubmit_before(queue, submitCount, pSubmits, fence);
}
#endif 
auto ret = device_dispatch[GetKey(queue)].QueueSubmit(queue, submitCount, pSubmits, fence);
winsockSendToUI(&ConnectSocket,"queue=" + ptrToString((void*)queue) + '!');
winsockSendToUI(&ConnectSocket,"submitCount=" + std::to_string(submitCount) + '!');
winsockSendToUI(&ConnectSocket,"pSubmits=" + ptrToString((void*)pSubmits) + '!');
winsockSendToUI(&ConnectSocket,"fence=" + ptrToString((void*)fence) + '!');
#ifdef QUEUESUBMIT_AFTER_EXEC_EXISTS
if(connected) {
layer_QueueSubmit_after(queue, submitCount, pSubmits, fence);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkQueueSubmit!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_QueueWaitIdle(VkQueue queue) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkQueueWaitIdle!");
}
#ifdef QUEUEWAITIDLE_BEFORE_EXEC_EXISTS
if(connected) {
layer_QueueWaitIdle_before(queue);
}
#endif 
auto ret = device_dispatch[GetKey(queue)].QueueWaitIdle(queue);
winsockSendToUI(&ConnectSocket,"queue=" + ptrToString((void*)queue) + '!');
#ifdef QUEUEWAITIDLE_AFTER_EXEC_EXISTS
if(connected) {
layer_QueueWaitIdle_after(queue);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkQueueWaitIdle!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_DeviceWaitIdle(VkDevice device) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDeviceWaitIdle!");
}
#ifdef DEVICEWAITIDLE_BEFORE_EXEC_EXISTS
if(connected) {
layer_DeviceWaitIdle_before(device);
}
#endif 
auto ret = device_dispatch[GetKey(device)].DeviceWaitIdle(device);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
#ifdef DEVICEWAITIDLE_AFTER_EXEC_EXISTS
if(connected) {
layer_DeviceWaitIdle_after(device);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDeviceWaitIdle!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AllocateMemory(VkDevice device, VkMemoryAllocateInfo* pAllocateInfo, VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkAllocateMemory!");
}
#ifdef ALLOCATEMEMORY_BEFORE_EXEC_EXISTS
if(connected) {
layer_AllocateMemory_before(device, pAllocateInfo, pAllocator, pMemory);
}
#endif 
auto ret = device_dispatch[GetKey(device)].AllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pAllocateInfo=" + ptrToString((void*)pAllocateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pMemory=" + ptrToString((void*)pMemory) + '!');
#ifdef ALLOCATEMEMORY_AFTER_EXEC_EXISTS
if(connected) {
layer_AllocateMemory_after(device, pAllocateInfo, pAllocator, pMemory);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkAllocateMemory!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_FreeMemory(VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkFreeMemory!");
}
#ifdef FREEMEMORY_BEFORE_EXEC_EXISTS
if(connected) {
layer_FreeMemory_before(device, memory, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].FreeMemory(device, memory, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"memory=" + ptrToString((void*)memory) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef FREEMEMORY_AFTER_EXEC_EXISTS
if(connected) {
layer_FreeMemory_after(device, memory, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkFreeMemory!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkMapMemory!");
}
#ifdef MAPMEMORY_BEFORE_EXEC_EXISTS
if(connected) {
layer_MapMemory_before(device, memory, offset, size, flags, ppData);
}
#endif 
auto ret = device_dispatch[GetKey(device)].MapMemory(device, memory, offset, size, flags, ppData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"memory=" + ptrToString((void*)memory) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
winsockSendToUI(&ConnectSocket,"size=" + ptrToString((void*)size) + '!');
winsockSendToUI(&ConnectSocket,"flags=" + ptrToString((void*)flags) + '!');
winsockSendToUI(&ConnectSocket,"ppData=" + ptrToString((void*)ppData) + '!');
#ifdef MAPMEMORY_AFTER_EXEC_EXISTS
if(connected) {
layer_MapMemory_after(device, memory, offset, size, flags, ppData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkMapMemory!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_UnmapMemory(VkDevice device, VkDeviceMemory memory) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkUnmapMemory!");
}
#ifdef UNMAPMEMORY_BEFORE_EXEC_EXISTS
if(connected) {
layer_UnmapMemory_before(device, memory);
}
#endif 
device_dispatch[GetKey(device)].UnmapMemory(device, memory);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"memory=" + ptrToString((void*)memory) + '!');
#ifdef UNMAPMEMORY_AFTER_EXEC_EXISTS
if(connected) {
layer_UnmapMemory_after(device, memory);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkUnmapMemory!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, VkMappedMemoryRange* pMemoryRanges) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkFlushMappedMemoryRanges!");
}
#ifdef FLUSHMAPPEDMEMORYRANGES_BEFORE_EXEC_EXISTS
if(connected) {
layer_FlushMappedMemoryRanges_before(device, memoryRangeCount, pMemoryRanges);
}
#endif 
auto ret = device_dispatch[GetKey(device)].FlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"memoryRangeCount=" + std::to_string(memoryRangeCount) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRanges=" + ptrToString((void*)pMemoryRanges) + '!');
#ifdef FLUSHMAPPEDMEMORYRANGES_AFTER_EXEC_EXISTS
if(connected) {
layer_FlushMappedMemoryRanges_after(device, memoryRangeCount, pMemoryRanges);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkFlushMappedMemoryRanges!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, VkMappedMemoryRange* pMemoryRanges) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkInvalidateMappedMemoryRanges!");
}
#ifdef INVALIDATEMAPPEDMEMORYRANGES_BEFORE_EXEC_EXISTS
if(connected) {
layer_InvalidateMappedMemoryRanges_before(device, memoryRangeCount, pMemoryRanges);
}
#endif 
auto ret = device_dispatch[GetKey(device)].InvalidateMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"memoryRangeCount=" + std::to_string(memoryRangeCount) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRanges=" + ptrToString((void*)pMemoryRanges) + '!');
#ifdef INVALIDATEMAPPEDMEMORYRANGES_AFTER_EXEC_EXISTS
if(connected) {
layer_InvalidateMappedMemoryRanges_after(device, memoryRangeCount, pMemoryRanges);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkInvalidateMappedMemoryRanges!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceMemoryCommitment!");
}
#ifdef GETDEVICEMEMORYCOMMITMENT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceMemoryCommitment_before(device, memory, pCommittedMemoryInBytes);
}
#endif 
device_dispatch[GetKey(device)].GetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"memory=" + ptrToString((void*)memory) + '!');
winsockSendToUI(&ConnectSocket,"pCommittedMemoryInBytes=" + ptrToString((void*)pCommittedMemoryInBytes) + '!');
#ifdef GETDEVICEMEMORYCOMMITMENT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceMemoryCommitment_after(device, memory, pCommittedMemoryInBytes);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceMemoryCommitment!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetBufferMemoryRequirements!");
}
#ifdef GETBUFFERMEMORYREQUIREMENTS_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetBufferMemoryRequirements_before(device, buffer, pMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRequirements=" + ptrToString((void*)pMemoryRequirements) + '!');
#ifdef GETBUFFERMEMORYREQUIREMENTS_AFTER_EXEC_EXISTS
if(connected) {
layer_GetBufferMemoryRequirements_after(device, buffer, pMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetBufferMemoryRequirements!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkBindBufferMemory!");
}
#ifdef BINDBUFFERMEMORY_BEFORE_EXEC_EXISTS
if(connected) {
layer_BindBufferMemory_before(device, buffer, memory, memoryOffset);
}
#endif 
auto ret = device_dispatch[GetKey(device)].BindBufferMemory(device, buffer, memory, memoryOffset);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"memory=" + ptrToString((void*)memory) + '!');
winsockSendToUI(&ConnectSocket,"memoryOffset=" + ptrToString((void*)memoryOffset) + '!');
#ifdef BINDBUFFERMEMORY_AFTER_EXEC_EXISTS
if(connected) {
layer_BindBufferMemory_after(device, buffer, memory, memoryOffset);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkBindBufferMemory!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetImageMemoryRequirements!");
}
#ifdef GETIMAGEMEMORYREQUIREMENTS_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetImageMemoryRequirements_before(device, image, pMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetImageMemoryRequirements(device, image, pMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"image=" + ptrToString((void*)image) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRequirements=" + ptrToString((void*)pMemoryRequirements) + '!');
#ifdef GETIMAGEMEMORYREQUIREMENTS_AFTER_EXEC_EXISTS
if(connected) {
layer_GetImageMemoryRequirements_after(device, image, pMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetImageMemoryRequirements!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkBindImageMemory!");
}
#ifdef BINDIMAGEMEMORY_BEFORE_EXEC_EXISTS
if(connected) {
layer_BindImageMemory_before(device, image, memory, memoryOffset);
}
#endif 
auto ret = device_dispatch[GetKey(device)].BindImageMemory(device, image, memory, memoryOffset);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"image=" + ptrToString((void*)image) + '!');
winsockSendToUI(&ConnectSocket,"memory=" + ptrToString((void*)memory) + '!');
winsockSendToUI(&ConnectSocket,"memoryOffset=" + ptrToString((void*)memoryOffset) + '!');
#ifdef BINDIMAGEMEMORY_AFTER_EXEC_EXISTS
if(connected) {
layer_BindImageMemory_after(device, image, memory, memoryOffset);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkBindImageMemory!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetImageSparseMemoryRequirements!");
}
#ifdef GETIMAGESPARSEMEMORYREQUIREMENTS_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetImageSparseMemoryRequirements_before(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetImageSparseMemoryRequirements(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"image=" + ptrToString((void*)image) + '!');
winsockSendToUI(&ConnectSocket,"pSparseMemoryRequirementCount=" + ptrToString((void*)pSparseMemoryRequirementCount) + '!');
winsockSendToUI(&ConnectSocket,"pSparseMemoryRequirements=" + ptrToString((void*)pSparseMemoryRequirements) + '!');
#ifdef GETIMAGESPARSEMEMORYREQUIREMENTS_AFTER_EXEC_EXISTS
if(connected) {
layer_GetImageSparseMemoryRequirements_after(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetImageSparseMemoryRequirements!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, VkBindSparseInfo* pBindInfo, VkFence fence) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkQueueBindSparse!");
}
#ifdef QUEUEBINDSPARSE_BEFORE_EXEC_EXISTS
if(connected) {
layer_QueueBindSparse_before(queue, bindInfoCount, pBindInfo, fence);
}
#endif 
auto ret = device_dispatch[GetKey(queue)].QueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
winsockSendToUI(&ConnectSocket,"queue=" + ptrToString((void*)queue) + '!');
winsockSendToUI(&ConnectSocket,"bindInfoCount=" + std::to_string(bindInfoCount) + '!');
winsockSendToUI(&ConnectSocket,"pBindInfo=" + ptrToString((void*)pBindInfo) + '!');
winsockSendToUI(&ConnectSocket,"fence=" + ptrToString((void*)fence) + '!');
#ifdef QUEUEBINDSPARSE_AFTER_EXEC_EXISTS
if(connected) {
layer_QueueBindSparse_after(queue, bindInfoCount, pBindInfo, fence);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkQueueBindSparse!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateFence(VkDevice device, VkFenceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkFence* pFence) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateFence!");
}
#ifdef CREATEFENCE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateFence_before(device, pCreateInfo, pAllocator, pFence);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateFence(device, pCreateInfo, pAllocator, pFence);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pFence=" + ptrToString((void*)pFence) + '!');
#ifdef CREATEFENCE_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateFence_after(device, pCreateInfo, pAllocator, pFence);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateFence!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyFence(VkDevice device, VkFence fence, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyFence!");
}
#ifdef DESTROYFENCE_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyFence_before(device, fence, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyFence(device, fence, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"fence=" + ptrToString((void*)fence) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYFENCE_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyFence_after(device, fence, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyFence!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ResetFences(VkDevice device, uint32_t fenceCount, VkFence* pFences) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkResetFences!");
}
#ifdef RESETFENCES_BEFORE_EXEC_EXISTS
if(connected) {
layer_ResetFences_before(device, fenceCount, pFences);
}
#endif 
auto ret = device_dispatch[GetKey(device)].ResetFences(device, fenceCount, pFences);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"fenceCount=" + std::to_string(fenceCount) + '!');
winsockSendToUI(&ConnectSocket,"pFences=" + ptrToString((void*)pFences) + '!');
#ifdef RESETFENCES_AFTER_EXEC_EXISTS
if(connected) {
layer_ResetFences_after(device, fenceCount, pFences);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkResetFences!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetFenceStatus(VkDevice device, VkFence fence) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetFenceStatus!");
}
#ifdef GETFENCESTATUS_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetFenceStatus_before(device, fence);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetFenceStatus(device, fence);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"fence=" + ptrToString((void*)fence) + '!');
#ifdef GETFENCESTATUS_AFTER_EXEC_EXISTS
if(connected) {
layer_GetFenceStatus_after(device, fence);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetFenceStatus!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_WaitForFences(VkDevice device, uint32_t fenceCount, VkFence* pFences, VkBool32 waitAll, uint64_t timeout) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkWaitForFences!");
}
#ifdef WAITFORFENCES_BEFORE_EXEC_EXISTS
if(connected) {
layer_WaitForFences_before(device, fenceCount, pFences, waitAll, timeout);
}
#endif 
auto ret = device_dispatch[GetKey(device)].WaitForFences(device, fenceCount, pFences, waitAll, timeout);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"fenceCount=" + std::to_string(fenceCount) + '!');
winsockSendToUI(&ConnectSocket,"pFences=" + ptrToString((void*)pFences) + '!');
winsockSendToUI(&ConnectSocket,"waitAll=" + bool_as_text(waitAll) + '!');
winsockSendToUI(&ConnectSocket,"timeout=" + std::to_string(timeout) + '!');
#ifdef WAITFORFENCES_AFTER_EXEC_EXISTS
if(connected) {
layer_WaitForFences_after(device, fenceCount, pFences, waitAll, timeout);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkWaitForFences!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateSemaphore(VkDevice device, VkSemaphoreCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateSemaphore!");
}
#ifdef CREATESEMAPHORE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateSemaphore_before(device, pCreateInfo, pAllocator, pSemaphore);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSemaphore=" + ptrToString((void*)pSemaphore) + '!');
#ifdef CREATESEMAPHORE_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateSemaphore_after(device, pCreateInfo, pAllocator, pSemaphore);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateSemaphore!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroySemaphore(VkDevice device, VkSemaphore semaphore, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroySemaphore!");
}
#ifdef DESTROYSEMAPHORE_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroySemaphore_before(device, semaphore, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroySemaphore(device, semaphore, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"semaphore=" + ptrToString((void*)semaphore) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYSEMAPHORE_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroySemaphore_after(device, semaphore, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroySemaphore!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateEvent(VkDevice device, VkEventCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkEvent* pEvent) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateEvent!");
}
#ifdef CREATEEVENT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateEvent_before(device, pCreateInfo, pAllocator, pEvent);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateEvent(device, pCreateInfo, pAllocator, pEvent);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pEvent=" + ptrToString((void*)pEvent) + '!');
#ifdef CREATEEVENT_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateEvent_after(device, pCreateInfo, pAllocator, pEvent);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateEvent!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyEvent(VkDevice device, VkEvent event, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyEvent!");
}
#ifdef DESTROYEVENT_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyEvent_before(device, event, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyEvent(device, event, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"event=" + ptrToString((void*)event) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYEVENT_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyEvent_after(device, event, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyEvent!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetEventStatus(VkDevice device, VkEvent event) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetEventStatus!");
}
#ifdef GETEVENTSTATUS_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetEventStatus_before(device, event);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetEventStatus(device, event);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"event=" + ptrToString((void*)event) + '!');
#ifdef GETEVENTSTATUS_AFTER_EXEC_EXISTS
if(connected) {
layer_GetEventStatus_after(device, event);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetEventStatus!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SetEvent(VkDevice device, VkEvent event) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkSetEvent!");
}
#ifdef SETEVENT_BEFORE_EXEC_EXISTS
if(connected) {
layer_SetEvent_before(device, event);
}
#endif 
auto ret = device_dispatch[GetKey(device)].SetEvent(device, event);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"event=" + ptrToString((void*)event) + '!');
#ifdef SETEVENT_AFTER_EXEC_EXISTS
if(connected) {
layer_SetEvent_after(device, event);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkSetEvent!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ResetEvent(VkDevice device, VkEvent event) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkResetEvent!");
}
#ifdef RESETEVENT_BEFORE_EXEC_EXISTS
if(connected) {
layer_ResetEvent_before(device, event);
}
#endif 
auto ret = device_dispatch[GetKey(device)].ResetEvent(device, event);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"event=" + ptrToString((void*)event) + '!');
#ifdef RESETEVENT_AFTER_EXEC_EXISTS
if(connected) {
layer_ResetEvent_after(device, event);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkResetEvent!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateQueryPool(VkDevice device, VkQueryPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateQueryPool!");
}
#ifdef CREATEQUERYPOOL_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateQueryPool_before(device, pCreateInfo, pAllocator, pQueryPool);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateQueryPool(device, pCreateInfo, pAllocator, pQueryPool);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pQueryPool=" + ptrToString((void*)pQueryPool) + '!');
#ifdef CREATEQUERYPOOL_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateQueryPool_after(device, pCreateInfo, pAllocator, pQueryPool);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateQueryPool!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyQueryPool!");
}
#ifdef DESTROYQUERYPOOL_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyQueryPool_before(device, queryPool, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyQueryPool(device, queryPool, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"queryPool=" + ptrToString((void*)queryPool) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYQUERYPOOL_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyQueryPool_after(device, queryPool, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyQueryPool!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetQueryPoolResults!");
}
#ifdef GETQUERYPOOLRESULTS_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetQueryPoolResults_before(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"queryPool=" + ptrToString((void*)queryPool) + '!');
winsockSendToUI(&ConnectSocket,"firstQuery=" + std::to_string(firstQuery) + '!');
winsockSendToUI(&ConnectSocket,"queryCount=" + std::to_string(queryCount) + '!');
winsockSendToUI(&ConnectSocket,"dataSize=" + std::to_string(dataSize) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + ptrToString((void*)stride) + '!');
winsockSendToUI(&ConnectSocket,"flags=" + ptrToString((void*)flags) + '!');
#ifdef GETQUERYPOOLRESULTS_AFTER_EXEC_EXISTS
if(connected) {
layer_GetQueryPoolResults_after(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetQueryPoolResults!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_ResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkResetQueryPool!");
}
#ifdef RESETQUERYPOOL_BEFORE_EXEC_EXISTS
if(connected) {
layer_ResetQueryPool_before(device, queryPool, firstQuery, queryCount);
}
#endif 
device_dispatch[GetKey(device)].ResetQueryPool(device, queryPool, firstQuery, queryCount);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"queryPool=" + ptrToString((void*)queryPool) + '!');
winsockSendToUI(&ConnectSocket,"firstQuery=" + std::to_string(firstQuery) + '!');
winsockSendToUI(&ConnectSocket,"queryCount=" + std::to_string(queryCount) + '!');
#ifdef RESETQUERYPOOL_AFTER_EXEC_EXISTS
if(connected) {
layer_ResetQueryPool_after(device, queryPool, firstQuery, queryCount);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkResetQueryPool!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateBuffer(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateBuffer!");
}
#ifdef CREATEBUFFER_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateBuffer_before(device, pCreateInfo, pAllocator, pBuffer);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pBuffer=" + ptrToString((void*)pBuffer) + '!');
#ifdef CREATEBUFFER_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateBuffer_after(device, pCreateInfo, pAllocator, pBuffer);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateBuffer!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyBuffer(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyBuffer!");
}
#ifdef DESTROYBUFFER_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyBuffer_before(device, buffer, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyBuffer(device, buffer, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYBUFFER_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyBuffer_after(device, buffer, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyBuffer!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateBufferView(VkDevice device, VkBufferViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBufferView* pView) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateBufferView!");
}
#ifdef CREATEBUFFERVIEW_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateBufferView_before(device, pCreateInfo, pAllocator, pView);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateBufferView(device, pCreateInfo, pAllocator, pView);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pView=" + ptrToString((void*)pView) + '!');
#ifdef CREATEBUFFERVIEW_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateBufferView_after(device, pCreateInfo, pAllocator, pView);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateBufferView!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyBufferView(VkDevice device, VkBufferView bufferView, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyBufferView!");
}
#ifdef DESTROYBUFFERVIEW_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyBufferView_before(device, bufferView, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyBufferView(device, bufferView, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"bufferView=" + ptrToString((void*)bufferView) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYBUFFERVIEW_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyBufferView_after(device, bufferView, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyBufferView!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateImage(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateImage!");
}
#ifdef CREATEIMAGE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateImage_before(device, pCreateInfo, pAllocator, pImage);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateImage(device, pCreateInfo, pAllocator, pImage);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pImage=" + ptrToString((void*)pImage) + '!');
#ifdef CREATEIMAGE_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateImage_after(device, pCreateInfo, pAllocator, pImage);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateImage!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyImage(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyImage!");
}
#ifdef DESTROYIMAGE_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyImage_before(device, image, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyImage(device, image, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"image=" + ptrToString((void*)image) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYIMAGE_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyImage_after(device, image, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyImage!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageSubresourceLayout(VkDevice device, VkImage image, VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetImageSubresourceLayout!");
}
#ifdef GETIMAGESUBRESOURCELAYOUT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetImageSubresourceLayout_before(device, image, pSubresource, pLayout);
}
#endif 
device_dispatch[GetKey(device)].GetImageSubresourceLayout(device, image, pSubresource, pLayout);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"image=" + ptrToString((void*)image) + '!');
winsockSendToUI(&ConnectSocket,"pSubresource=" + ptrToString((void*)pSubresource) + '!');
winsockSendToUI(&ConnectSocket,"pLayout=" + ptrToString((void*)pLayout) + '!');
#ifdef GETIMAGESUBRESOURCELAYOUT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetImageSubresourceLayout_after(device, image, pSubresource, pLayout);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetImageSubresourceLayout!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateImageView(VkDevice device, VkImageViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImageView* pView) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateImageView!");
}
#ifdef CREATEIMAGEVIEW_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateImageView_before(device, pCreateInfo, pAllocator, pView);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateImageView(device, pCreateInfo, pAllocator, pView);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pView=" + ptrToString((void*)pView) + '!');
#ifdef CREATEIMAGEVIEW_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateImageView_after(device, pCreateInfo, pAllocator, pView);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateImageView!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyImageView(VkDevice device, VkImageView imageView, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyImageView!");
}
#ifdef DESTROYIMAGEVIEW_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyImageView_before(device, imageView, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyImageView(device, imageView, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"imageView=" + ptrToString((void*)imageView) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYIMAGEVIEW_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyImageView_after(device, imageView, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyImageView!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateShaderModule(VkDevice device, VkShaderModuleCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateShaderModule!");
}
#ifdef CREATESHADERMODULE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateShaderModule_before(device, pCreateInfo, pAllocator, pShaderModule);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pShaderModule=" + ptrToString((void*)pShaderModule) + '!');
#ifdef CREATESHADERMODULE_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateShaderModule_after(device, pCreateInfo, pAllocator, pShaderModule);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateShaderModule!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyShaderModule!");
}
#ifdef DESTROYSHADERMODULE_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyShaderModule_before(device, shaderModule, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyShaderModule(device, shaderModule, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"shaderModule=" + ptrToString((void*)shaderModule) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYSHADERMODULE_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyShaderModule_after(device, shaderModule, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyShaderModule!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreatePipelineCache(VkDevice device, VkPipelineCacheCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreatePipelineCache!");
}
#ifdef CREATEPIPELINECACHE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreatePipelineCache_before(device, pCreateInfo, pAllocator, pPipelineCache);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pPipelineCache=" + ptrToString((void*)pPipelineCache) + '!');
#ifdef CREATEPIPELINECACHE_AFTER_EXEC_EXISTS
if(connected) {
layer_CreatePipelineCache_after(device, pCreateInfo, pAllocator, pPipelineCache);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreatePipelineCache!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyPipelineCache!");
}
#ifdef DESTROYPIPELINECACHE_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyPipelineCache_before(device, pipelineCache, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyPipelineCache(device, pipelineCache, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pipelineCache=" + ptrToString((void*)pipelineCache) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYPIPELINECACHE_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyPipelineCache_after(device, pipelineCache, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyPipelineCache!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPipelineCacheData!");
}
#ifdef GETPIPELINECACHEDATA_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPipelineCacheData_before(device, pipelineCache, pDataSize, pData);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetPipelineCacheData(device, pipelineCache, pDataSize, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pipelineCache=" + ptrToString((void*)pipelineCache) + '!');
winsockSendToUI(&ConnectSocket,"pDataSize=" + ptrToString((void*)pDataSize) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef GETPIPELINECACHEDATA_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPipelineCacheData_after(device, pipelineCache, pDataSize, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPipelineCacheData!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, VkPipelineCache* pSrcCaches) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkMergePipelineCaches!");
}
#ifdef MERGEPIPELINECACHES_BEFORE_EXEC_EXISTS
if(connected) {
layer_MergePipelineCaches_before(device, dstCache, srcCacheCount, pSrcCaches);
}
#endif 
auto ret = device_dispatch[GetKey(device)].MergePipelineCaches(device, dstCache, srcCacheCount, pSrcCaches);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"dstCache=" + ptrToString((void*)dstCache) + '!');
winsockSendToUI(&ConnectSocket,"srcCacheCount=" + std::to_string(srcCacheCount) + '!');
winsockSendToUI(&ConnectSocket,"pSrcCaches=" + ptrToString((void*)pSrcCaches) + '!');
#ifdef MERGEPIPELINECACHES_AFTER_EXEC_EXISTS
if(connected) {
layer_MergePipelineCaches_after(device, dstCache, srcCacheCount, pSrcCaches);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkMergePipelineCaches!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkGraphicsPipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateGraphicsPipelines!");
}
#ifdef CREATEGRAPHICSPIPELINES_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateGraphicsPipelines_before(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pipelineCache=" + ptrToString((void*)pipelineCache) + '!');
winsockSendToUI(&ConnectSocket,"createInfoCount=" + std::to_string(createInfoCount) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfos=" + ptrToString((void*)pCreateInfos) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pPipelines=" + ptrToString((void*)pPipelines) + '!');
#ifdef CREATEGRAPHICSPIPELINES_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateGraphicsPipelines_after(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateGraphicsPipelines!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkComputePipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateComputePipelines!");
}
#ifdef CREATECOMPUTEPIPELINES_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateComputePipelines_before(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pipelineCache=" + ptrToString((void*)pipelineCache) + '!');
winsockSendToUI(&ConnectSocket,"createInfoCount=" + std::to_string(createInfoCount) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfos=" + ptrToString((void*)pCreateInfos) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pPipelines=" + ptrToString((void*)pPipelines) + '!');
#ifdef CREATECOMPUTEPIPELINES_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateComputePipelines_after(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateComputePipelines!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI!");
}
#ifdef GETDEVICESUBPASSSHADINGMAXWORKGROUPSIZEHUAWEI_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI_before(device, renderpass, pMaxWorkgroupSize);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(device, renderpass, pMaxWorkgroupSize);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"renderpass=" + ptrToString((void*)renderpass) + '!');
winsockSendToUI(&ConnectSocket,"pMaxWorkgroupSize=" + ptrToString((void*)pMaxWorkgroupSize) + '!');
#ifdef GETDEVICESUBPASSSHADINGMAXWORKGROUPSIZEHUAWEI_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI_after(device, renderpass, pMaxWorkgroupSize);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyPipeline(VkDevice device, VkPipeline pipeline, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyPipeline!");
}
#ifdef DESTROYPIPELINE_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyPipeline_before(device, pipeline, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyPipeline(device, pipeline, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pipeline=" + ptrToString((void*)pipeline) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYPIPELINE_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyPipeline_after(device, pipeline, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyPipeline!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreatePipelineLayout(VkDevice device, VkPipelineLayoutCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreatePipelineLayout!");
}
#ifdef CREATEPIPELINELAYOUT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreatePipelineLayout_before(device, pCreateInfo, pAllocator, pPipelineLayout);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreatePipelineLayout(device, pCreateInfo, pAllocator, pPipelineLayout);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pPipelineLayout=" + ptrToString((void*)pPipelineLayout) + '!');
#ifdef CREATEPIPELINELAYOUT_AFTER_EXEC_EXISTS
if(connected) {
layer_CreatePipelineLayout_after(device, pCreateInfo, pAllocator, pPipelineLayout);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreatePipelineLayout!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyPipelineLayout!");
}
#ifdef DESTROYPIPELINELAYOUT_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyPipelineLayout_before(device, pipelineLayout, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyPipelineLayout(device, pipelineLayout, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pipelineLayout=" + ptrToString((void*)pipelineLayout) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYPIPELINELAYOUT_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyPipelineLayout_after(device, pipelineLayout, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyPipelineLayout!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateSampler(VkDevice device, VkSamplerCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSampler* pSampler) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateSampler!");
}
#ifdef CREATESAMPLER_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateSampler_before(device, pCreateInfo, pAllocator, pSampler);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateSampler(device, pCreateInfo, pAllocator, pSampler);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSampler=" + ptrToString((void*)pSampler) + '!');
#ifdef CREATESAMPLER_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateSampler_after(device, pCreateInfo, pAllocator, pSampler);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateSampler!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroySampler(VkDevice device, VkSampler sampler, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroySampler!");
}
#ifdef DESTROYSAMPLER_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroySampler_before(device, sampler, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroySampler(device, sampler, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"sampler=" + ptrToString((void*)sampler) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYSAMPLER_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroySampler_after(device, sampler, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroySampler!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDescriptorSetLayout(VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateDescriptorSetLayout!");
}
#ifdef CREATEDESCRIPTORSETLAYOUT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateDescriptorSetLayout_before(device, pCreateInfo, pAllocator, pSetLayout);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateDescriptorSetLayout(device, pCreateInfo, pAllocator, pSetLayout);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSetLayout=" + ptrToString((void*)pSetLayout) + '!');
#ifdef CREATEDESCRIPTORSETLAYOUT_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateDescriptorSetLayout_after(device, pCreateInfo, pAllocator, pSetLayout);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateDescriptorSetLayout!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyDescriptorSetLayout!");
}
#ifdef DESTROYDESCRIPTORSETLAYOUT_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyDescriptorSetLayout_before(device, descriptorSetLayout, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"descriptorSetLayout=" + ptrToString((void*)descriptorSetLayout) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYDESCRIPTORSETLAYOUT_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyDescriptorSetLayout_after(device, descriptorSetLayout, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyDescriptorSetLayout!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDescriptorPool(VkDevice device, VkDescriptorPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateDescriptorPool!");
}
#ifdef CREATEDESCRIPTORPOOL_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateDescriptorPool_before(device, pCreateInfo, pAllocator, pDescriptorPool);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pDescriptorPool=" + ptrToString((void*)pDescriptorPool) + '!');
#ifdef CREATEDESCRIPTORPOOL_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateDescriptorPool_after(device, pCreateInfo, pAllocator, pDescriptorPool);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateDescriptorPool!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyDescriptorPool!");
}
#ifdef DESTROYDESCRIPTORPOOL_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyDescriptorPool_before(device, descriptorPool, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyDescriptorPool(device, descriptorPool, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"descriptorPool=" + ptrToString((void*)descriptorPool) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYDESCRIPTORPOOL_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyDescriptorPool_after(device, descriptorPool, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyDescriptorPool!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkResetDescriptorPool!");
}
#ifdef RESETDESCRIPTORPOOL_BEFORE_EXEC_EXISTS
if(connected) {
layer_ResetDescriptorPool_before(device, descriptorPool, flags);
}
#endif 
auto ret = device_dispatch[GetKey(device)].ResetDescriptorPool(device, descriptorPool, flags);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"descriptorPool=" + ptrToString((void*)descriptorPool) + '!');
winsockSendToUI(&ConnectSocket,"flags=" + ptrToString((void*)flags) + '!');
#ifdef RESETDESCRIPTORPOOL_AFTER_EXEC_EXISTS
if(connected) {
layer_ResetDescriptorPool_after(device, descriptorPool, flags);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkResetDescriptorPool!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AllocateDescriptorSets(VkDevice device, VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkAllocateDescriptorSets!");
}
#ifdef ALLOCATEDESCRIPTORSETS_BEFORE_EXEC_EXISTS
if(connected) {
layer_AllocateDescriptorSets_before(device, pAllocateInfo, pDescriptorSets);
}
#endif 
auto ret = device_dispatch[GetKey(device)].AllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pAllocateInfo=" + ptrToString((void*)pAllocateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pDescriptorSets=" + ptrToString((void*)pDescriptorSets) + '!');
#ifdef ALLOCATEDESCRIPTORSETS_AFTER_EXEC_EXISTS
if(connected) {
layer_AllocateDescriptorSets_after(device, pAllocateInfo, pDescriptorSets);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkAllocateDescriptorSets!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, VkDescriptorSet* pDescriptorSets) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkFreeDescriptorSets!");
}
#ifdef FREEDESCRIPTORSETS_BEFORE_EXEC_EXISTS
if(connected) {
layer_FreeDescriptorSets_before(device, descriptorPool, descriptorSetCount, pDescriptorSets);
}
#endif 
auto ret = device_dispatch[GetKey(device)].FreeDescriptorSets(device, descriptorPool, descriptorSetCount, pDescriptorSets);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"descriptorPool=" + ptrToString((void*)descriptorPool) + '!');
winsockSendToUI(&ConnectSocket,"descriptorSetCount=" + std::to_string(descriptorSetCount) + '!');
winsockSendToUI(&ConnectSocket,"pDescriptorSets=" + ptrToString((void*)pDescriptorSets) + '!');
#ifdef FREEDESCRIPTORSETS_AFTER_EXEC_EXISTS
if(connected) {
layer_FreeDescriptorSets_after(device, descriptorPool, descriptorSetCount, pDescriptorSets);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkFreeDescriptorSets!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, VkCopyDescriptorSet* pDescriptorCopies) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkUpdateDescriptorSets!");
}
#ifdef UPDATEDESCRIPTORSETS_BEFORE_EXEC_EXISTS
if(connected) {
layer_UpdateDescriptorSets_before(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
}
#endif 
device_dispatch[GetKey(device)].UpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"descriptorWriteCount=" + std::to_string(descriptorWriteCount) + '!');
winsockSendToUI(&ConnectSocket,"pDescriptorWrites=" + ptrToString((void*)pDescriptorWrites) + '!');
winsockSendToUI(&ConnectSocket,"descriptorCopyCount=" + std::to_string(descriptorCopyCount) + '!');
winsockSendToUI(&ConnectSocket,"pDescriptorCopies=" + ptrToString((void*)pDescriptorCopies) + '!');
#ifdef UPDATEDESCRIPTORSETS_AFTER_EXEC_EXISTS
if(connected) {
layer_UpdateDescriptorSets_after(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkUpdateDescriptorSets!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateFramebuffer(VkDevice device, VkFramebufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateFramebuffer!");
}
#ifdef CREATEFRAMEBUFFER_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateFramebuffer_before(device, pCreateInfo, pAllocator, pFramebuffer);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateFramebuffer(device, pCreateInfo, pAllocator, pFramebuffer);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pFramebuffer=" + ptrToString((void*)pFramebuffer) + '!');
#ifdef CREATEFRAMEBUFFER_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateFramebuffer_after(device, pCreateInfo, pAllocator, pFramebuffer);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateFramebuffer!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyFramebuffer!");
}
#ifdef DESTROYFRAMEBUFFER_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyFramebuffer_before(device, framebuffer, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyFramebuffer(device, framebuffer, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"framebuffer=" + ptrToString((void*)framebuffer) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYFRAMEBUFFER_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyFramebuffer_after(device, framebuffer, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyFramebuffer!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateRenderPass(VkDevice device, VkRenderPassCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateRenderPass!");
}
#ifdef CREATERENDERPASS_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateRenderPass_before(device, pCreateInfo, pAllocator, pRenderPass);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pRenderPass=" + ptrToString((void*)pRenderPass) + '!');
#ifdef CREATERENDERPASS_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateRenderPass_after(device, pCreateInfo, pAllocator, pRenderPass);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateRenderPass!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyRenderPass!");
}
#ifdef DESTROYRENDERPASS_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyRenderPass_before(device, renderPass, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyRenderPass(device, renderPass, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"renderPass=" + ptrToString((void*)renderPass) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYRENDERPASS_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyRenderPass_after(device, renderPass, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyRenderPass!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetRenderAreaGranularity!");
}
#ifdef GETRENDERAREAGRANULARITY_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetRenderAreaGranularity_before(device, renderPass, pGranularity);
}
#endif 
device_dispatch[GetKey(device)].GetRenderAreaGranularity(device, renderPass, pGranularity);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"renderPass=" + ptrToString((void*)renderPass) + '!');
winsockSendToUI(&ConnectSocket,"pGranularity=" + ptrToString((void*)pGranularity) + '!');
#ifdef GETRENDERAREAGRANULARITY_AFTER_EXEC_EXISTS
if(connected) {
layer_GetRenderAreaGranularity_after(device, renderPass, pGranularity);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetRenderAreaGranularity!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetRenderingAreaGranularityKHR(VkDevice device, VkRenderingAreaInfoKHR* pRenderingAreaInfo, VkExtent2D* pGranularity) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetRenderingAreaGranularityKHR!");
}
#ifdef GETRENDERINGAREAGRANULARITYKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetRenderingAreaGranularityKHR_before(device, pRenderingAreaInfo, pGranularity);
}
#endif 
device_dispatch[GetKey(device)].GetRenderingAreaGranularityKHR(device, pRenderingAreaInfo, pGranularity);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pRenderingAreaInfo=" + ptrToString((void*)pRenderingAreaInfo) + '!');
winsockSendToUI(&ConnectSocket,"pGranularity=" + ptrToString((void*)pGranularity) + '!');
#ifdef GETRENDERINGAREAGRANULARITYKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetRenderingAreaGranularityKHR_after(device, pRenderingAreaInfo, pGranularity);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetRenderingAreaGranularityKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateCommandPool(VkDevice device, VkCommandPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateCommandPool!");
}
#ifdef CREATECOMMANDPOOL_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateCommandPool_before(device, pCreateInfo, pAllocator, pCommandPool);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pCommandPool=" + ptrToString((void*)pCommandPool) + '!');
#ifdef CREATECOMMANDPOOL_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateCommandPool_after(device, pCreateInfo, pAllocator, pCommandPool);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateCommandPool!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyCommandPool!");
}
#ifdef DESTROYCOMMANDPOOL_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyCommandPool_before(device, commandPool, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyCommandPool(device, commandPool, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"commandPool=" + ptrToString((void*)commandPool) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYCOMMANDPOOL_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyCommandPool_after(device, commandPool, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyCommandPool!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkResetCommandPool!");
}
#ifdef RESETCOMMANDPOOL_BEFORE_EXEC_EXISTS
if(connected) {
layer_ResetCommandPool_before(device, commandPool, flags);
}
#endif 
auto ret = device_dispatch[GetKey(device)].ResetCommandPool(device, commandPool, flags);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"commandPool=" + ptrToString((void*)commandPool) + '!');
winsockSendToUI(&ConnectSocket,"flags=" + ptrToString((void*)flags) + '!');
#ifdef RESETCOMMANDPOOL_AFTER_EXEC_EXISTS
if(connected) {
layer_ResetCommandPool_after(device, commandPool, flags);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkResetCommandPool!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AllocateCommandBuffers(VkDevice device, VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkAllocateCommandBuffers!");
}
#ifdef ALLOCATECOMMANDBUFFERS_BEFORE_EXEC_EXISTS
if(connected) {
layer_AllocateCommandBuffers_before(device, pAllocateInfo, pCommandBuffers);
}
#endif 
auto ret = device_dispatch[GetKey(device)].AllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pAllocateInfo=" + ptrToString((void*)pAllocateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pCommandBuffers=" + ptrToString((void*)pCommandBuffers) + '!');
#ifdef ALLOCATECOMMANDBUFFERS_AFTER_EXEC_EXISTS
if(connected) {
layer_AllocateCommandBuffers_after(device, pAllocateInfo, pCommandBuffers);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkAllocateCommandBuffers!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkFreeCommandBuffers!");
}
#ifdef FREECOMMANDBUFFERS_BEFORE_EXEC_EXISTS
if(connected) {
layer_FreeCommandBuffers_before(device, commandPool, commandBufferCount, pCommandBuffers);
}
#endif 
device_dispatch[GetKey(device)].FreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"commandPool=" + ptrToString((void*)commandPool) + '!');
winsockSendToUI(&ConnectSocket,"commandBufferCount=" + std::to_string(commandBufferCount) + '!');
winsockSendToUI(&ConnectSocket,"pCommandBuffers=" + ptrToString((void*)pCommandBuffers) + '!');
#ifdef FREECOMMANDBUFFERS_AFTER_EXEC_EXISTS
if(connected) {
layer_FreeCommandBuffers_after(device, commandPool, commandBufferCount, pCommandBuffers);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkFreeCommandBuffers!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BeginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo* pBeginInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkBeginCommandBuffer!");
}
#ifdef BEGINCOMMANDBUFFER_BEFORE_EXEC_EXISTS
if(connected) {
layer_BeginCommandBuffer_before(commandBuffer, pBeginInfo);
}
#endif 
auto ret = device_dispatch[GetKey(commandBuffer)].BeginCommandBuffer(commandBuffer, pBeginInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pBeginInfo=" + ptrToString((void*)pBeginInfo) + '!');
#ifdef BEGINCOMMANDBUFFER_AFTER_EXEC_EXISTS
if(connected) {
layer_BeginCommandBuffer_after(commandBuffer, pBeginInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkBeginCommandBuffer!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EndCommandBuffer(VkCommandBuffer commandBuffer) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkEndCommandBuffer!");
}
#ifdef ENDCOMMANDBUFFER_BEFORE_EXEC_EXISTS
if(connected) {
layer_EndCommandBuffer_before(commandBuffer);
}
#endif 
auto ret = device_dispatch[GetKey(commandBuffer)].EndCommandBuffer(commandBuffer);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
#ifdef ENDCOMMANDBUFFER_AFTER_EXEC_EXISTS
if(connected) {
layer_EndCommandBuffer_after(commandBuffer);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkEndCommandBuffer!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkResetCommandBuffer!");
}
#ifdef RESETCOMMANDBUFFER_BEFORE_EXEC_EXISTS
if(connected) {
layer_ResetCommandBuffer_before(commandBuffer, flags);
}
#endif 
auto ret = device_dispatch[GetKey(commandBuffer)].ResetCommandBuffer(commandBuffer, flags);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"flags=" + ptrToString((void*)flags) + '!');
#ifdef RESETCOMMANDBUFFER_AFTER_EXEC_EXISTS
if(connected) {
layer_ResetCommandBuffer_after(commandBuffer, flags);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkResetCommandBuffer!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBindPipeline!");
}
#ifdef CMDBINDPIPELINE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBindPipeline_before(commandBuffer, pipelineBindPoint, pipeline);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pipelineBindPoint=" + std::to_string(pipelineBindPoint) + '!');
winsockSendToUI(&ConnectSocket,"pipeline=" + ptrToString((void*)pipeline) + '!');
#ifdef CMDBINDPIPELINE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBindPipeline_after(commandBuffer, pipelineBindPoint, pipeline);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBindPipeline!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetAttachmentFeedbackLoopEnableEXT(VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetAttachmentFeedbackLoopEnableEXT!");
}
#ifdef CMDSETATTACHMENTFEEDBACKLOOPENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetAttachmentFeedbackLoopEnableEXT_before(commandBuffer, aspectMask);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetAttachmentFeedbackLoopEnableEXT(commandBuffer, aspectMask);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"aspectMask=" + ptrToString((void*)aspectMask) + '!');
#ifdef CMDSETATTACHMENTFEEDBACKLOOPENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetAttachmentFeedbackLoopEnableEXT_after(commandBuffer, aspectMask);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetAttachmentFeedbackLoopEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewport* pViewports) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetViewport!");
}
#ifdef CMDSETVIEWPORT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetViewport_before(commandBuffer, firstViewport, viewportCount, pViewports);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstViewport=" + std::to_string(firstViewport) + '!');
winsockSendToUI(&ConnectSocket,"viewportCount=" + std::to_string(viewportCount) + '!');
winsockSendToUI(&ConnectSocket,"pViewports=" + ptrToString((void*)pViewports) + '!');
#ifdef CMDSETVIEWPORT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetViewport_after(commandBuffer, firstViewport, viewportCount, pViewports);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetViewport!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, VkRect2D* pScissors) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetScissor!");
}
#ifdef CMDSETSCISSOR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetScissor_before(commandBuffer, firstScissor, scissorCount, pScissors);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstScissor=" + std::to_string(firstScissor) + '!');
winsockSendToUI(&ConnectSocket,"scissorCount=" + std::to_string(scissorCount) + '!');
winsockSendToUI(&ConnectSocket,"pScissors=" + ptrToString((void*)pScissors) + '!');
#ifdef CMDSETSCISSOR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetScissor_after(commandBuffer, firstScissor, scissorCount, pScissors);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetScissor!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetLineWidth!");
}
#ifdef CMDSETLINEWIDTH_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetLineWidth_before(commandBuffer, lineWidth);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetLineWidth(commandBuffer, lineWidth);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"lineWidth=" + std::to_string(lineWidth) + '!');
#ifdef CMDSETLINEWIDTH_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetLineWidth_after(commandBuffer, lineWidth);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetLineWidth!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDepthBias!");
}
#ifdef CMDSETDEPTHBIAS_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthBias_before(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"depthBiasConstantFactor=" + std::to_string(depthBiasConstantFactor) + '!');
winsockSendToUI(&ConnectSocket,"depthBiasClamp=" + std::to_string(depthBiasClamp) + '!');
winsockSendToUI(&ConnectSocket,"depthBiasSlopeFactor=" + std::to_string(depthBiasSlopeFactor) + '!');
#ifdef CMDSETDEPTHBIAS_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthBias_after(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDepthBias!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetBlendConstants(VkCommandBuffer commandBuffer, float* blendConstants) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetBlendConstants!");
}
#ifdef CMDSETBLENDCONSTANTS_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetBlendConstants_before(commandBuffer, blendConstants);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetBlendConstants(commandBuffer, blendConstants);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"blendConstants=" + ptrToString((void*)blendConstants) + '!');
#ifdef CMDSETBLENDCONSTANTS_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetBlendConstants_after(commandBuffer, blendConstants);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetBlendConstants!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDepthBounds!");
}
#ifdef CMDSETDEPTHBOUNDS_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthBounds_before(commandBuffer, minDepthBounds, maxDepthBounds);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"minDepthBounds=" + std::to_string(minDepthBounds) + '!');
winsockSendToUI(&ConnectSocket,"maxDepthBounds=" + std::to_string(maxDepthBounds) + '!');
#ifdef CMDSETDEPTHBOUNDS_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthBounds_after(commandBuffer, minDepthBounds, maxDepthBounds);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDepthBounds!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetStencilCompareMask!");
}
#ifdef CMDSETSTENCILCOMPAREMASK_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetStencilCompareMask_before(commandBuffer, faceMask, compareMask);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"faceMask=" + ptrToString((void*)faceMask) + '!');
winsockSendToUI(&ConnectSocket,"compareMask=" + std::to_string(compareMask) + '!');
#ifdef CMDSETSTENCILCOMPAREMASK_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetStencilCompareMask_after(commandBuffer, faceMask, compareMask);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetStencilCompareMask!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetStencilWriteMask!");
}
#ifdef CMDSETSTENCILWRITEMASK_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetStencilWriteMask_before(commandBuffer, faceMask, writeMask);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"faceMask=" + ptrToString((void*)faceMask) + '!');
winsockSendToUI(&ConnectSocket,"writeMask=" + std::to_string(writeMask) + '!');
#ifdef CMDSETSTENCILWRITEMASK_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetStencilWriteMask_after(commandBuffer, faceMask, writeMask);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetStencilWriteMask!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetStencilReference!");
}
#ifdef CMDSETSTENCILREFERENCE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetStencilReference_before(commandBuffer, faceMask, reference);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetStencilReference(commandBuffer, faceMask, reference);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"faceMask=" + ptrToString((void*)faceMask) + '!');
winsockSendToUI(&ConnectSocket,"reference=" + std::to_string(reference) + '!');
#ifdef CMDSETSTENCILREFERENCE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetStencilReference_after(commandBuffer, faceMask, reference);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetStencilReference!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, uint32_t* pDynamicOffsets) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBindDescriptorSets!");
}
#ifdef CMDBINDDESCRIPTORSETS_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBindDescriptorSets_before(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pipelineBindPoint=" + std::to_string(pipelineBindPoint) + '!');
winsockSendToUI(&ConnectSocket,"layout=" + ptrToString((void*)layout) + '!');
winsockSendToUI(&ConnectSocket,"firstSet=" + std::to_string(firstSet) + '!');
winsockSendToUI(&ConnectSocket,"descriptorSetCount=" + std::to_string(descriptorSetCount) + '!');
winsockSendToUI(&ConnectSocket,"pDescriptorSets=" + ptrToString((void*)pDescriptorSets) + '!');
winsockSendToUI(&ConnectSocket,"dynamicOffsetCount=" + std::to_string(dynamicOffsetCount) + '!');
winsockSendToUI(&ConnectSocket,"pDynamicOffsets=" + ptrToString((void*)pDynamicOffsets) + '!');
#ifdef CMDBINDDESCRIPTORSETS_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBindDescriptorSets_after(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBindDescriptorSets!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBindIndexBuffer!");
}
#ifdef CMDBINDINDEXBUFFER_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBindIndexBuffer_before(commandBuffer, buffer, offset, indexType);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
winsockSendToUI(&ConnectSocket,"indexType=" + std::to_string(indexType) + '!');
#ifdef CMDBINDINDEXBUFFER_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBindIndexBuffer_after(commandBuffer, buffer, offset, indexType);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBindIndexBuffer!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBindVertexBuffers!");
}
#ifdef CMDBINDVERTEXBUFFERS_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBindVertexBuffers_before(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstBinding=" + std::to_string(firstBinding) + '!');
winsockSendToUI(&ConnectSocket,"bindingCount=" + std::to_string(bindingCount) + '!');
winsockSendToUI(&ConnectSocket,"pBuffers=" + ptrToString((void*)pBuffers) + '!');
winsockSendToUI(&ConnectSocket,"pOffsets=" + ptrToString((void*)pOffsets) + '!');
#ifdef CMDBINDVERTEXBUFFERS_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBindVertexBuffers_after(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBindVertexBuffers!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDraw!");
}
#ifdef CMDDRAW_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDraw_before(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"vertexCount=" + std::to_string(vertexCount) + '!');
winsockSendToUI(&ConnectSocket,"instanceCount=" + std::to_string(instanceCount) + '!');
winsockSendToUI(&ConnectSocket,"firstVertex=" + std::to_string(firstVertex) + '!');
winsockSendToUI(&ConnectSocket,"firstInstance=" + std::to_string(firstInstance) + '!');
#ifdef CMDDRAW_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDraw_after(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDraw!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawIndexed!");
}
#ifdef CMDDRAWINDEXED_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndexed_before(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"indexCount=" + std::to_string(indexCount) + '!');
winsockSendToUI(&ConnectSocket,"instanceCount=" + std::to_string(instanceCount) + '!');
winsockSendToUI(&ConnectSocket,"firstIndex=" + std::to_string(firstIndex) + '!');
winsockSendToUI(&ConnectSocket,"vertexOffset=" + std::to_string(vertexOffset) + '!');
winsockSendToUI(&ConnectSocket,"firstInstance=" + std::to_string(firstInstance) + '!');
#ifdef CMDDRAWINDEXED_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndexed_after(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawIndexed!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawMultiEXT!");
}
#ifdef CMDDRAWMULTIEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawMultiEXT_before(commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawMultiEXT(commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"drawCount=" + std::to_string(drawCount) + '!');
winsockSendToUI(&ConnectSocket,"pVertexInfo=" + ptrToString((void*)pVertexInfo) + '!');
winsockSendToUI(&ConnectSocket,"instanceCount=" + std::to_string(instanceCount) + '!');
winsockSendToUI(&ConnectSocket,"firstInstance=" + std::to_string(firstInstance) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef CMDDRAWMULTIEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawMultiEXT_after(commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawMultiEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, int32_t* pVertexOffset) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawMultiIndexedEXT!");
}
#ifdef CMDDRAWMULTIINDEXEDEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawMultiIndexedEXT_before(commandBuffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawMultiIndexedEXT(commandBuffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"drawCount=" + std::to_string(drawCount) + '!');
winsockSendToUI(&ConnectSocket,"pIndexInfo=" + ptrToString((void*)pIndexInfo) + '!');
winsockSendToUI(&ConnectSocket,"instanceCount=" + std::to_string(instanceCount) + '!');
winsockSendToUI(&ConnectSocket,"firstInstance=" + std::to_string(firstInstance) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
winsockSendToUI(&ConnectSocket,"pVertexOffset=" + ptrToString((void*)pVertexOffset) + '!');
#ifdef CMDDRAWMULTIINDEXEDEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawMultiIndexedEXT_after(commandBuffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawMultiIndexedEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawIndirect!");
}
#ifdef CMDDRAWINDIRECT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndirect_before(commandBuffer, buffer, offset, drawCount, stride);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
winsockSendToUI(&ConnectSocket,"drawCount=" + std::to_string(drawCount) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef CMDDRAWINDIRECT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndirect_after(commandBuffer, buffer, offset, drawCount, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawIndirect!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawIndexedIndirect!");
}
#ifdef CMDDRAWINDEXEDINDIRECT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndexedIndirect_before(commandBuffer, buffer, offset, drawCount, stride);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
winsockSendToUI(&ConnectSocket,"drawCount=" + std::to_string(drawCount) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef CMDDRAWINDEXEDINDIRECT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndexedIndirect_after(commandBuffer, buffer, offset, drawCount, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawIndexedIndirect!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDispatch!");
}
#ifdef CMDDISPATCH_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDispatch_before(commandBuffer, groupCountX, groupCountY, groupCountZ);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"groupCountX=" + std::to_string(groupCountX) + '!');
winsockSendToUI(&ConnectSocket,"groupCountY=" + std::to_string(groupCountY) + '!');
winsockSendToUI(&ConnectSocket,"groupCountZ=" + std::to_string(groupCountZ) + '!');
#ifdef CMDDISPATCH_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDispatch_after(commandBuffer, groupCountX, groupCountY, groupCountZ);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDispatch!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDispatchIndirect!");
}
#ifdef CMDDISPATCHINDIRECT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDispatchIndirect_before(commandBuffer, buffer, offset);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDispatchIndirect(commandBuffer, buffer, offset);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
#ifdef CMDDISPATCHINDIRECT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDispatchIndirect_after(commandBuffer, buffer, offset);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDispatchIndirect!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSubpassShadingHUAWEI!");
}
#ifdef CMDSUBPASSSHADINGHUAWEI_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSubpassShadingHUAWEI_before(commandBuffer);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSubpassShadingHUAWEI(commandBuffer);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
#ifdef CMDSUBPASSSHADINGHUAWEI_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSubpassShadingHUAWEI_after(commandBuffer);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSubpassShadingHUAWEI!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawClusterHUAWEI!");
}
#ifdef CMDDRAWCLUSTERHUAWEI_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawClusterHUAWEI_before(commandBuffer, groupCountX, groupCountY, groupCountZ);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawClusterHUAWEI(commandBuffer, groupCountX, groupCountY, groupCountZ);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"groupCountX=" + std::to_string(groupCountX) + '!');
winsockSendToUI(&ConnectSocket,"groupCountY=" + std::to_string(groupCountY) + '!');
winsockSendToUI(&ConnectSocket,"groupCountZ=" + std::to_string(groupCountZ) + '!');
#ifdef CMDDRAWCLUSTERHUAWEI_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawClusterHUAWEI_after(commandBuffer, groupCountX, groupCountY, groupCountZ);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawClusterHUAWEI!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawClusterIndirectHUAWEI!");
}
#ifdef CMDDRAWCLUSTERINDIRECTHUAWEI_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawClusterIndirectHUAWEI_before(commandBuffer, buffer, offset);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawClusterIndirectHUAWEI(commandBuffer, buffer, offset);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
#ifdef CMDDRAWCLUSTERINDIRECTHUAWEI_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawClusterIndirectHUAWEI_after(commandBuffer, buffer, offset);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawClusterIndirectHUAWEI!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdUpdatePipelineIndirectBufferNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdUpdatePipelineIndirectBufferNV!");
}
#ifdef CMDUPDATEPIPELINEINDIRECTBUFFERNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdUpdatePipelineIndirectBufferNV_before(commandBuffer, pipelineBindPoint, pipeline);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdUpdatePipelineIndirectBufferNV(commandBuffer, pipelineBindPoint, pipeline);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pipelineBindPoint=" + std::to_string(pipelineBindPoint) + '!');
winsockSendToUI(&ConnectSocket,"pipeline=" + ptrToString((void*)pipeline) + '!');
#ifdef CMDUPDATEPIPELINEINDIRECTBUFFERNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdUpdatePipelineIndirectBufferNV_after(commandBuffer, pipelineBindPoint, pipeline);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdUpdatePipelineIndirectBufferNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy* pRegions) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyBuffer!");
}
#ifdef CMDCOPYBUFFER_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyBuffer_before(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"srcBuffer=" + ptrToString((void*)srcBuffer) + '!');
winsockSendToUI(&ConnectSocket,"dstBuffer=" + ptrToString((void*)dstBuffer) + '!');
winsockSendToUI(&ConnectSocket,"regionCount=" + std::to_string(regionCount) + '!');
winsockSendToUI(&ConnectSocket,"pRegions=" + ptrToString((void*)pRegions) + '!');
#ifdef CMDCOPYBUFFER_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyBuffer_after(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyBuffer!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageCopy* pRegions) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyImage!");
}
#ifdef CMDCOPYIMAGE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyImage_before(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"srcImage=" + ptrToString((void*)srcImage) + '!');
winsockSendToUI(&ConnectSocket,"srcImageLayout=" + std::to_string(srcImageLayout) + '!');
winsockSendToUI(&ConnectSocket,"dstImage=" + ptrToString((void*)dstImage) + '!');
winsockSendToUI(&ConnectSocket,"dstImageLayout=" + std::to_string(dstImageLayout) + '!');
winsockSendToUI(&ConnectSocket,"regionCount=" + std::to_string(regionCount) + '!');
winsockSendToUI(&ConnectSocket,"pRegions=" + ptrToString((void*)pRegions) + '!');
#ifdef CMDCOPYIMAGE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyImage_after(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyImage!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageBlit* pRegions, VkFilter filter) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBlitImage!");
}
#ifdef CMDBLITIMAGE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBlitImage_before(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"srcImage=" + ptrToString((void*)srcImage) + '!');
winsockSendToUI(&ConnectSocket,"srcImageLayout=" + std::to_string(srcImageLayout) + '!');
winsockSendToUI(&ConnectSocket,"dstImage=" + ptrToString((void*)dstImage) + '!');
winsockSendToUI(&ConnectSocket,"dstImageLayout=" + std::to_string(dstImageLayout) + '!');
winsockSendToUI(&ConnectSocket,"regionCount=" + std::to_string(regionCount) + '!');
winsockSendToUI(&ConnectSocket,"pRegions=" + ptrToString((void*)pRegions) + '!');
winsockSendToUI(&ConnectSocket,"filter=" + std::to_string(filter) + '!');
#ifdef CMDBLITIMAGE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBlitImage_after(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBlitImage!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkBufferImageCopy* pRegions) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyBufferToImage!");
}
#ifdef CMDCOPYBUFFERTOIMAGE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyBufferToImage_before(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"srcBuffer=" + ptrToString((void*)srcBuffer) + '!');
winsockSendToUI(&ConnectSocket,"dstImage=" + ptrToString((void*)dstImage) + '!');
winsockSendToUI(&ConnectSocket,"dstImageLayout=" + std::to_string(dstImageLayout) + '!');
winsockSendToUI(&ConnectSocket,"regionCount=" + std::to_string(regionCount) + '!');
winsockSendToUI(&ConnectSocket,"pRegions=" + ptrToString((void*)pRegions) + '!');
#ifdef CMDCOPYBUFFERTOIMAGE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyBufferToImage_after(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyBufferToImage!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, VkBufferImageCopy* pRegions) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyImageToBuffer!");
}
#ifdef CMDCOPYIMAGETOBUFFER_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyImageToBuffer_before(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"srcImage=" + ptrToString((void*)srcImage) + '!');
winsockSendToUI(&ConnectSocket,"srcImageLayout=" + std::to_string(srcImageLayout) + '!');
winsockSendToUI(&ConnectSocket,"dstBuffer=" + ptrToString((void*)dstBuffer) + '!');
winsockSendToUI(&ConnectSocket,"regionCount=" + std::to_string(regionCount) + '!');
winsockSendToUI(&ConnectSocket,"pRegions=" + ptrToString((void*)pRegions) + '!');
#ifdef CMDCOPYIMAGETOBUFFER_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyImageToBuffer_after(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyImageToBuffer!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyMemoryIndirectNV!");
}
#ifdef CMDCOPYMEMORYINDIRECTNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyMemoryIndirectNV_before(commandBuffer, copyBufferAddress, copyCount, stride);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyMemoryIndirectNV(commandBuffer, copyBufferAddress, copyCount, stride);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"copyBufferAddress=" + ptrToString((void*)copyBufferAddress) + '!');
winsockSendToUI(&ConnectSocket,"copyCount=" + std::to_string(copyCount) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef CMDCOPYMEMORYINDIRECTNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyMemoryIndirectNV_after(commandBuffer, copyBufferAddress, copyCount, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyMemoryIndirectNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, VkImageSubresourceLayers* pImageSubresources) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyMemoryToImageIndirectNV!");
}
#ifdef CMDCOPYMEMORYTOIMAGEINDIRECTNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyMemoryToImageIndirectNV_before(commandBuffer, copyBufferAddress, copyCount, stride, dstImage, dstImageLayout, pImageSubresources);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyMemoryToImageIndirectNV(commandBuffer, copyBufferAddress, copyCount, stride, dstImage, dstImageLayout, pImageSubresources);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"copyBufferAddress=" + ptrToString((void*)copyBufferAddress) + '!');
winsockSendToUI(&ConnectSocket,"copyCount=" + std::to_string(copyCount) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
winsockSendToUI(&ConnectSocket,"dstImage=" + ptrToString((void*)dstImage) + '!');
winsockSendToUI(&ConnectSocket,"dstImageLayout=" + std::to_string(dstImageLayout) + '!');
winsockSendToUI(&ConnectSocket,"pImageSubresources=" + ptrToString((void*)pImageSubresources) + '!');
#ifdef CMDCOPYMEMORYTOIMAGEINDIRECTNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyMemoryToImageIndirectNV_after(commandBuffer, copyBufferAddress, copyCount, stride, dstImage, dstImageLayout, pImageSubresources);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyMemoryToImageIndirectNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdUpdateBuffer!");
}
#ifdef CMDUPDATEBUFFER_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdUpdateBuffer_before(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"dstBuffer=" + ptrToString((void*)dstBuffer) + '!');
winsockSendToUI(&ConnectSocket,"dstOffset=" + ptrToString((void*)dstOffset) + '!');
winsockSendToUI(&ConnectSocket,"dataSize=" + ptrToString((void*)dataSize) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef CMDUPDATEBUFFER_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdUpdateBuffer_after(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdUpdateBuffer!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdFillBuffer!");
}
#ifdef CMDFILLBUFFER_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdFillBuffer_before(commandBuffer, dstBuffer, dstOffset, size, data);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"dstBuffer=" + ptrToString((void*)dstBuffer) + '!');
winsockSendToUI(&ConnectSocket,"dstOffset=" + ptrToString((void*)dstOffset) + '!');
winsockSendToUI(&ConnectSocket,"size=" + ptrToString((void*)size) + '!');
winsockSendToUI(&ConnectSocket,"data=" + std::to_string(data) + '!');
#ifdef CMDFILLBUFFER_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdFillBuffer_after(commandBuffer, dstBuffer, dstOffset, size, data);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdFillBuffer!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearColorValue* pColor, uint32_t rangeCount, VkImageSubresourceRange* pRanges) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdClearColorImage!");
}
#ifdef CMDCLEARCOLORIMAGE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdClearColorImage_before(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"image=" + ptrToString((void*)image) + '!');
winsockSendToUI(&ConnectSocket,"imageLayout=" + std::to_string(imageLayout) + '!');
winsockSendToUI(&ConnectSocket,"pColor=" + ptrToString((void*)pColor) + '!');
winsockSendToUI(&ConnectSocket,"rangeCount=" + std::to_string(rangeCount) + '!');
winsockSendToUI(&ConnectSocket,"pRanges=" + ptrToString((void*)pRanges) + '!');
#ifdef CMDCLEARCOLORIMAGE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdClearColorImage_after(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdClearColorImage!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, VkImageSubresourceRange* pRanges) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdClearDepthStencilImage!");
}
#ifdef CMDCLEARDEPTHSTENCILIMAGE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdClearDepthStencilImage_before(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"image=" + ptrToString((void*)image) + '!');
winsockSendToUI(&ConnectSocket,"imageLayout=" + std::to_string(imageLayout) + '!');
winsockSendToUI(&ConnectSocket,"pDepthStencil=" + ptrToString((void*)pDepthStencil) + '!');
winsockSendToUI(&ConnectSocket,"rangeCount=" + std::to_string(rangeCount) + '!');
winsockSendToUI(&ConnectSocket,"pRanges=" + ptrToString((void*)pRanges) + '!');
#ifdef CMDCLEARDEPTHSTENCILIMAGE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdClearDepthStencilImage_after(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdClearDepthStencilImage!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, VkClearAttachment* pAttachments, uint32_t rectCount, VkClearRect* pRects) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdClearAttachments!");
}
#ifdef CMDCLEARATTACHMENTS_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdClearAttachments_before(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"attachmentCount=" + std::to_string(attachmentCount) + '!');
winsockSendToUI(&ConnectSocket,"pAttachments=" + ptrToString((void*)pAttachments) + '!');
winsockSendToUI(&ConnectSocket,"rectCount=" + std::to_string(rectCount) + '!');
winsockSendToUI(&ConnectSocket,"pRects=" + ptrToString((void*)pRects) + '!');
#ifdef CMDCLEARATTACHMENTS_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdClearAttachments_after(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdClearAttachments!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageResolve* pRegions) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdResolveImage!");
}
#ifdef CMDRESOLVEIMAGE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdResolveImage_before(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"srcImage=" + ptrToString((void*)srcImage) + '!');
winsockSendToUI(&ConnectSocket,"srcImageLayout=" + std::to_string(srcImageLayout) + '!');
winsockSendToUI(&ConnectSocket,"dstImage=" + ptrToString((void*)dstImage) + '!');
winsockSendToUI(&ConnectSocket,"dstImageLayout=" + std::to_string(dstImageLayout) + '!');
winsockSendToUI(&ConnectSocket,"regionCount=" + std::to_string(regionCount) + '!');
winsockSendToUI(&ConnectSocket,"pRegions=" + ptrToString((void*)pRegions) + '!');
#ifdef CMDRESOLVEIMAGE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdResolveImage_after(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdResolveImage!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetEvent!");
}
#ifdef CMDSETEVENT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetEvent_before(commandBuffer, event, stageMask);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetEvent(commandBuffer, event, stageMask);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"event=" + ptrToString((void*)event) + '!');
winsockSendToUI(&ConnectSocket,"stageMask=" + ptrToString((void*)stageMask) + '!');
#ifdef CMDSETEVENT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetEvent_after(commandBuffer, event, stageMask);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetEvent!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdResetEvent!");
}
#ifdef CMDRESETEVENT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdResetEvent_before(commandBuffer, event, stageMask);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdResetEvent(commandBuffer, event, stageMask);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"event=" + ptrToString((void*)event) + '!');
winsockSendToUI(&ConnectSocket,"stageMask=" + ptrToString((void*)stageMask) + '!');
#ifdef CMDRESETEVENT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdResetEvent_after(commandBuffer, event, stageMask);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdResetEvent!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier* pImageMemoryBarriers) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdWaitEvents!");
}
#ifdef CMDWAITEVENTS_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdWaitEvents_before(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"eventCount=" + std::to_string(eventCount) + '!');
winsockSendToUI(&ConnectSocket,"pEvents=" + ptrToString((void*)pEvents) + '!');
winsockSendToUI(&ConnectSocket,"srcStageMask=" + ptrToString((void*)srcStageMask) + '!');
winsockSendToUI(&ConnectSocket,"dstStageMask=" + ptrToString((void*)dstStageMask) + '!');
winsockSendToUI(&ConnectSocket,"memoryBarrierCount=" + std::to_string(memoryBarrierCount) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryBarriers=" + ptrToString((void*)pMemoryBarriers) + '!');
winsockSendToUI(&ConnectSocket,"bufferMemoryBarrierCount=" + std::to_string(bufferMemoryBarrierCount) + '!');
winsockSendToUI(&ConnectSocket,"pBufferMemoryBarriers=" + ptrToString((void*)pBufferMemoryBarriers) + '!');
winsockSendToUI(&ConnectSocket,"imageMemoryBarrierCount=" + std::to_string(imageMemoryBarrierCount) + '!');
winsockSendToUI(&ConnectSocket,"pImageMemoryBarriers=" + ptrToString((void*)pImageMemoryBarriers) + '!');
#ifdef CMDWAITEVENTS_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdWaitEvents_after(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdWaitEvents!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier* pImageMemoryBarriers) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdPipelineBarrier!");
}
#ifdef CMDPIPELINEBARRIER_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdPipelineBarrier_before(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"srcStageMask=" + ptrToString((void*)srcStageMask) + '!');
winsockSendToUI(&ConnectSocket,"dstStageMask=" + ptrToString((void*)dstStageMask) + '!');
winsockSendToUI(&ConnectSocket,"dependencyFlags=" + ptrToString((void*)dependencyFlags) + '!');
winsockSendToUI(&ConnectSocket,"memoryBarrierCount=" + std::to_string(memoryBarrierCount) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryBarriers=" + ptrToString((void*)pMemoryBarriers) + '!');
winsockSendToUI(&ConnectSocket,"bufferMemoryBarrierCount=" + std::to_string(bufferMemoryBarrierCount) + '!');
winsockSendToUI(&ConnectSocket,"pBufferMemoryBarriers=" + ptrToString((void*)pBufferMemoryBarriers) + '!');
winsockSendToUI(&ConnectSocket,"imageMemoryBarrierCount=" + std::to_string(imageMemoryBarrierCount) + '!');
winsockSendToUI(&ConnectSocket,"pImageMemoryBarriers=" + ptrToString((void*)pImageMemoryBarriers) + '!');
#ifdef CMDPIPELINEBARRIER_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdPipelineBarrier_after(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdPipelineBarrier!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBeginQuery!");
}
#ifdef CMDBEGINQUERY_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBeginQuery_before(commandBuffer, queryPool, query, flags);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginQuery(commandBuffer, queryPool, query, flags);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"queryPool=" + ptrToString((void*)queryPool) + '!');
winsockSendToUI(&ConnectSocket,"query=" + std::to_string(query) + '!');
winsockSendToUI(&ConnectSocket,"flags=" + ptrToString((void*)flags) + '!');
#ifdef CMDBEGINQUERY_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBeginQuery_after(commandBuffer, queryPool, query, flags);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBeginQuery!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdEndQuery!");
}
#ifdef CMDENDQUERY_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdEndQuery_before(commandBuffer, queryPool, query);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndQuery(commandBuffer, queryPool, query);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"queryPool=" + ptrToString((void*)queryPool) + '!');
winsockSendToUI(&ConnectSocket,"query=" + std::to_string(query) + '!');
#ifdef CMDENDQUERY_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdEndQuery_after(commandBuffer, queryPool, query);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdEndQuery!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBeginConditionalRenderingEXT!");
}
#ifdef CMDBEGINCONDITIONALRENDERINGEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBeginConditionalRenderingEXT_before(commandBuffer, pConditionalRenderingBegin);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginConditionalRenderingEXT(commandBuffer, pConditionalRenderingBegin);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pConditionalRenderingBegin=" + ptrToString((void*)pConditionalRenderingBegin) + '!');
#ifdef CMDBEGINCONDITIONALRENDERINGEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBeginConditionalRenderingEXT_after(commandBuffer, pConditionalRenderingBegin);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBeginConditionalRenderingEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdEndConditionalRenderingEXT!");
}
#ifdef CMDENDCONDITIONALRENDERINGEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdEndConditionalRenderingEXT_before(commandBuffer);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndConditionalRenderingEXT(commandBuffer);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
#ifdef CMDENDCONDITIONALRENDERINGEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdEndConditionalRenderingEXT_after(commandBuffer);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdEndConditionalRenderingEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdResetQueryPool!");
}
#ifdef CMDRESETQUERYPOOL_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdResetQueryPool_before(commandBuffer, queryPool, firstQuery, queryCount);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"queryPool=" + ptrToString((void*)queryPool) + '!');
winsockSendToUI(&ConnectSocket,"firstQuery=" + std::to_string(firstQuery) + '!');
winsockSendToUI(&ConnectSocket,"queryCount=" + std::to_string(queryCount) + '!');
#ifdef CMDRESETQUERYPOOL_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdResetQueryPool_after(commandBuffer, queryPool, firstQuery, queryCount);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdResetQueryPool!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdWriteTimestamp!");
}
#ifdef CMDWRITETIMESTAMP_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdWriteTimestamp_before(commandBuffer, pipelineStage, queryPool, query);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pipelineStage=" + std::to_string(pipelineStage) + '!');
winsockSendToUI(&ConnectSocket,"queryPool=" + ptrToString((void*)queryPool) + '!');
winsockSendToUI(&ConnectSocket,"query=" + std::to_string(query) + '!');
#ifdef CMDWRITETIMESTAMP_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdWriteTimestamp_after(commandBuffer, pipelineStage, queryPool, query);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdWriteTimestamp!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyQueryPoolResults!");
}
#ifdef CMDCOPYQUERYPOOLRESULTS_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyQueryPoolResults_before(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"queryPool=" + ptrToString((void*)queryPool) + '!');
winsockSendToUI(&ConnectSocket,"firstQuery=" + std::to_string(firstQuery) + '!');
winsockSendToUI(&ConnectSocket,"queryCount=" + std::to_string(queryCount) + '!');
winsockSendToUI(&ConnectSocket,"dstBuffer=" + ptrToString((void*)dstBuffer) + '!');
winsockSendToUI(&ConnectSocket,"dstOffset=" + ptrToString((void*)dstOffset) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + ptrToString((void*)stride) + '!');
winsockSendToUI(&ConnectSocket,"flags=" + ptrToString((void*)flags) + '!');
#ifdef CMDCOPYQUERYPOOLRESULTS_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyQueryPoolResults_after(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyQueryPoolResults!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, void* pValues) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdPushConstants!");
}
#ifdef CMDPUSHCONSTANTS_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdPushConstants_before(commandBuffer, layout, stageFlags, offset, size, pValues);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"layout=" + ptrToString((void*)layout) + '!');
winsockSendToUI(&ConnectSocket,"stageFlags=" + ptrToString((void*)stageFlags) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + std::to_string(offset) + '!');
winsockSendToUI(&ConnectSocket,"size=" + std::to_string(size) + '!');
winsockSendToUI(&ConnectSocket,"pValues=" + ptrToString((void*)pValues) + '!');
#ifdef CMDPUSHCONSTANTS_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdPushConstants_after(commandBuffer, layout, stageFlags, offset, size, pValues);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdPushConstants!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginRenderPass(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBeginRenderPass!");
}
#ifdef CMDBEGINRENDERPASS_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBeginRenderPass_before(commandBuffer, pRenderPassBegin, contents);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pRenderPassBegin=" + ptrToString((void*)pRenderPassBegin) + '!');
winsockSendToUI(&ConnectSocket,"contents=" + std::to_string(contents) + '!');
#ifdef CMDBEGINRENDERPASS_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBeginRenderPass_after(commandBuffer, pRenderPassBegin, contents);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBeginRenderPass!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdNextSubpass!");
}
#ifdef CMDNEXTSUBPASS_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdNextSubpass_before(commandBuffer, contents);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdNextSubpass(commandBuffer, contents);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"contents=" + std::to_string(contents) + '!');
#ifdef CMDNEXTSUBPASS_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdNextSubpass_after(commandBuffer, contents);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdNextSubpass!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndRenderPass(VkCommandBuffer commandBuffer) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdEndRenderPass!");
}
#ifdef CMDENDRENDERPASS_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdEndRenderPass_before(commandBuffer);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndRenderPass(commandBuffer);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
#ifdef CMDENDRENDERPASS_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdEndRenderPass_after(commandBuffer);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdEndRenderPass!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdExecuteCommands!");
}
#ifdef CMDEXECUTECOMMANDS_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdExecuteCommands_before(commandBuffer, commandBufferCount, pCommandBuffers);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"commandBufferCount=" + std::to_string(commandBufferCount) + '!');
winsockSendToUI(&ConnectSocket,"pCommandBuffers=" + ptrToString((void*)pCommandBuffers) + '!');
#ifdef CMDEXECUTECOMMANDS_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdExecuteCommands_after(commandBuffer, commandBufferCount, pCommandBuffers);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdExecuteCommands!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, VkSwapchainCreateInfoKHR* pCreateInfos, VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateSharedSwapchainsKHR!");
}
#ifdef CREATESHAREDSWAPCHAINSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateSharedSwapchainsKHR_before(device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateSharedSwapchainsKHR(device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapchainCount=" + std::to_string(swapchainCount) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfos=" + ptrToString((void*)pCreateInfos) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSwapchains=" + ptrToString((void*)pSwapchains) + '!');
#ifdef CREATESHAREDSWAPCHAINSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateSharedSwapchainsKHR_after(device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateSharedSwapchainsKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateSwapchainKHR(VkDevice device, VkSwapchainCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateSwapchainKHR!");
}
#ifdef CREATESWAPCHAINKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateSwapchainKHR_before(device, pCreateInfo, pAllocator, pSwapchain);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSwapchain=" + ptrToString((void*)pSwapchain) + '!');
#ifdef CREATESWAPCHAINKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateSwapchainKHR_after(device, pCreateInfo, pAllocator, pSwapchain);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateSwapchainKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroySwapchainKHR!");
}
#ifdef DESTROYSWAPCHAINKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroySwapchainKHR_before(device, swapchain, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroySwapchainKHR(device, swapchain, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapchain=" + ptrToString((void*)swapchain) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYSWAPCHAINKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroySwapchainKHR_after(device, swapchain, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroySwapchainKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetSwapchainImagesKHR!");
}
#ifdef GETSWAPCHAINIMAGESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetSwapchainImagesKHR_before(device, swapchain, pSwapchainImageCount, pSwapchainImages);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapchain=" + ptrToString((void*)swapchain) + '!');
winsockSendToUI(&ConnectSocket,"pSwapchainImageCount=" + ptrToString((void*)pSwapchainImageCount) + '!');
winsockSendToUI(&ConnectSocket,"pSwapchainImages=" + ptrToString((void*)pSwapchainImages) + '!');
#ifdef GETSWAPCHAINIMAGESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetSwapchainImagesKHR_after(device, swapchain, pSwapchainImageCount, pSwapchainImages);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetSwapchainImagesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkAcquireNextImageKHR!");
}
#ifdef ACQUIRENEXTIMAGEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_AcquireNextImageKHR_before(device, swapchain, timeout, semaphore, fence, pImageIndex);
}
#endif 
auto ret = device_dispatch[GetKey(device)].AcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapchain=" + ptrToString((void*)swapchain) + '!');
winsockSendToUI(&ConnectSocket,"timeout=" + std::to_string(timeout) + '!');
winsockSendToUI(&ConnectSocket,"semaphore=" + ptrToString((void*)semaphore) + '!');
winsockSendToUI(&ConnectSocket,"fence=" + ptrToString((void*)fence) + '!');
winsockSendToUI(&ConnectSocket,"pImageIndex=" + ptrToString((void*)pImageIndex) + '!');
#ifdef ACQUIRENEXTIMAGEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_AcquireNextImageKHR_after(device, swapchain, timeout, semaphore, fence, pImageIndex);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkAcquireNextImageKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_QueuePresentKHR(VkQueue queue, VkPresentInfoKHR* pPresentInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkQueuePresentKHR!");
}
#ifdef QUEUEPRESENTKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_QueuePresentKHR_before(queue, pPresentInfo);
}
#endif 
auto ret = device_dispatch[GetKey(queue)].QueuePresentKHR(queue, pPresentInfo);
winsockSendToUI(&ConnectSocket,"queue=" + ptrToString((void*)queue) + '!');
winsockSendToUI(&ConnectSocket,"pPresentInfo=" + ptrToString((void*)pPresentInfo) + '!');
#ifdef QUEUEPRESENTKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_QueuePresentKHR_after(queue, pPresentInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkQueuePresentKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_DebugMarkerSetObjectNameEXT(VkDevice device, VkDebugMarkerObjectNameInfoEXT* pNameInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDebugMarkerSetObjectNameEXT!");
}
#ifdef DEBUGMARKERSETOBJECTNAMEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_DebugMarkerSetObjectNameEXT_before(device, pNameInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].DebugMarkerSetObjectNameEXT(device, pNameInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pNameInfo=" + ptrToString((void*)pNameInfo) + '!');
#ifdef DEBUGMARKERSETOBJECTNAMEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_DebugMarkerSetObjectNameEXT_after(device, pNameInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDebugMarkerSetObjectNameEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_DebugMarkerSetObjectTagEXT(VkDevice device, VkDebugMarkerObjectTagInfoEXT* pTagInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDebugMarkerSetObjectTagEXT!");
}
#ifdef DEBUGMARKERSETOBJECTTAGEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_DebugMarkerSetObjectTagEXT_before(device, pTagInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].DebugMarkerSetObjectTagEXT(device, pTagInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pTagInfo=" + ptrToString((void*)pTagInfo) + '!');
#ifdef DEBUGMARKERSETOBJECTTAGEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_DebugMarkerSetObjectTagEXT_after(device, pTagInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDebugMarkerSetObjectTagEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDebugMarkerBeginEXT!");
}
#ifdef CMDDEBUGMARKERBEGINEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDebugMarkerBeginEXT_before(commandBuffer, pMarkerInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pMarkerInfo=" + ptrToString((void*)pMarkerInfo) + '!');
#ifdef CMDDEBUGMARKERBEGINEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDebugMarkerBeginEXT_after(commandBuffer, pMarkerInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDebugMarkerBeginEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDebugMarkerEndEXT!");
}
#ifdef CMDDEBUGMARKERENDEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDebugMarkerEndEXT_before(commandBuffer);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDebugMarkerEndEXT(commandBuffer);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
#ifdef CMDDEBUGMARKERENDEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDebugMarkerEndEXT_after(commandBuffer);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDebugMarkerEndEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDebugMarkerInsertEXT!");
}
#ifdef CMDDEBUGMARKERINSERTEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDebugMarkerInsertEXT_before(commandBuffer, pMarkerInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pMarkerInfo=" + ptrToString((void*)pMarkerInfo) + '!');
#ifdef CMDDEBUGMARKERINSERTEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDebugMarkerInsertEXT_after(commandBuffer, pMarkerInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDebugMarkerInsertEXT!");
}
}

#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetMemoryWin32HandleNV!");
}
#ifdef GETMEMORYWIN32HANDLENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetMemoryWin32HandleNV_before(device, memory, handleType, pHandle);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetMemoryWin32HandleNV(device, memory, handleType, pHandle);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"memory=" + ptrToString((void*)memory) + '!');
winsockSendToUI(&ConnectSocket,"handleType=" + ptrToString((void*)handleType) + '!');
winsockSendToUI(&ConnectSocket,"pHandle=" + ptrToString((void*)pHandle) + '!');
#ifdef GETMEMORYWIN32HANDLENV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetMemoryWin32HandleNV_after(device, memory, handleType, pHandle);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetMemoryWin32HandleNV!");
}
return ret;
}

#endif
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdExecuteGeneratedCommandsNV!");
}
#ifdef CMDEXECUTEGENERATEDCOMMANDSNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdExecuteGeneratedCommandsNV_before(commandBuffer, isPreprocessed, pGeneratedCommandsInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdExecuteGeneratedCommandsNV(commandBuffer, isPreprocessed, pGeneratedCommandsInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"isPreprocessed=" + bool_as_text(isPreprocessed) + '!');
winsockSendToUI(&ConnectSocket,"pGeneratedCommandsInfo=" + ptrToString((void*)pGeneratedCommandsInfo) + '!');
#ifdef CMDEXECUTEGENERATEDCOMMANDSNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdExecuteGeneratedCommandsNV_after(commandBuffer, isPreprocessed, pGeneratedCommandsInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdExecuteGeneratedCommandsNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdPreprocessGeneratedCommandsNV!");
}
#ifdef CMDPREPROCESSGENERATEDCOMMANDSNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdPreprocessGeneratedCommandsNV_before(commandBuffer, pGeneratedCommandsInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPreprocessGeneratedCommandsNV(commandBuffer, pGeneratedCommandsInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pGeneratedCommandsInfo=" + ptrToString((void*)pGeneratedCommandsInfo) + '!');
#ifdef CMDPREPROCESSGENERATEDCOMMANDSNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdPreprocessGeneratedCommandsNV_after(commandBuffer, pGeneratedCommandsInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdPreprocessGeneratedCommandsNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBindPipelineShaderGroupNV!");
}
#ifdef CMDBINDPIPELINESHADERGROUPNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBindPipelineShaderGroupNV_before(commandBuffer, pipelineBindPoint, pipeline, groupIndex);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindPipelineShaderGroupNV(commandBuffer, pipelineBindPoint, pipeline, groupIndex);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pipelineBindPoint=" + std::to_string(pipelineBindPoint) + '!');
winsockSendToUI(&ConnectSocket,"pipeline=" + ptrToString((void*)pipeline) + '!');
winsockSendToUI(&ConnectSocket,"groupIndex=" + std::to_string(groupIndex) + '!');
#ifdef CMDBINDPIPELINESHADERGROUPNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBindPipelineShaderGroupNV_after(commandBuffer, pipelineBindPoint, pipeline, groupIndex);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBindPipelineShaderGroupNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetGeneratedCommandsMemoryRequirementsNV(VkDevice device, VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetGeneratedCommandsMemoryRequirementsNV!");
}
#ifdef GETGENERATEDCOMMANDSMEMORYREQUIREMENTSNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetGeneratedCommandsMemoryRequirementsNV_before(device, pInfo, pMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetGeneratedCommandsMemoryRequirementsNV(device, pInfo, pMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRequirements=" + ptrToString((void*)pMemoryRequirements) + '!');
#ifdef GETGENERATEDCOMMANDSMEMORYREQUIREMENTSNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetGeneratedCommandsMemoryRequirementsNV_after(device, pInfo, pMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetGeneratedCommandsMemoryRequirementsNV!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateIndirectCommandsLayoutNV!");
}
#ifdef CREATEINDIRECTCOMMANDSLAYOUTNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateIndirectCommandsLayoutNV_before(device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateIndirectCommandsLayoutNV(device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pIndirectCommandsLayout=" + ptrToString((void*)pIndirectCommandsLayout) + '!');
#ifdef CREATEINDIRECTCOMMANDSLAYOUTNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateIndirectCommandsLayoutNV_after(device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateIndirectCommandsLayoutNV!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyIndirectCommandsLayoutNV!");
}
#ifdef DESTROYINDIRECTCOMMANDSLAYOUTNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyIndirectCommandsLayoutNV_before(device, indirectCommandsLayout, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyIndirectCommandsLayoutNV(device, indirectCommandsLayout, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"indirectCommandsLayout=" + ptrToString((void*)indirectCommandsLayout) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYINDIRECTCOMMANDSLAYOUTNV_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyIndirectCommandsLayoutNV_after(device, indirectCommandsLayout, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyIndirectCommandsLayoutNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, VkWriteDescriptorSet* pDescriptorWrites) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdPushDescriptorSetKHR!");
}
#ifdef CMDPUSHDESCRIPTORSETKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdPushDescriptorSetKHR_before(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pipelineBindPoint=" + std::to_string(pipelineBindPoint) + '!');
winsockSendToUI(&ConnectSocket,"layout=" + ptrToString((void*)layout) + '!');
winsockSendToUI(&ConnectSocket,"set=" + std::to_string(set) + '!');
winsockSendToUI(&ConnectSocket,"descriptorWriteCount=" + std::to_string(descriptorWriteCount) + '!');
winsockSendToUI(&ConnectSocket,"pDescriptorWrites=" + ptrToString((void*)pDescriptorWrites) + '!');
#ifdef CMDPUSHDESCRIPTORSETKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdPushDescriptorSetKHR_after(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdPushDescriptorSetKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkTrimCommandPool!");
}
#ifdef TRIMCOMMANDPOOL_BEFORE_EXEC_EXISTS
if(connected) {
layer_TrimCommandPool_before(device, commandPool, flags);
}
#endif 
device_dispatch[GetKey(device)].TrimCommandPool(device, commandPool, flags);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"commandPool=" + ptrToString((void*)commandPool) + '!');
winsockSendToUI(&ConnectSocket,"flags=" + ptrToString((void*)flags) + '!');
#ifdef TRIMCOMMANDPOOL_AFTER_EXEC_EXISTS
if(connected) {
layer_TrimCommandPool_after(device, commandPool, flags);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkTrimCommandPool!");
}
}

#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryWin32HandleKHR(VkDevice device, VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetMemoryWin32HandleKHR!");
}
#ifdef GETMEMORYWIN32HANDLEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetMemoryWin32HandleKHR_before(device, pGetWin32HandleInfo, pHandle);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetMemoryWin32HandleKHR(device, pGetWin32HandleInfo, pHandle);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pGetWin32HandleInfo=" + ptrToString((void*)pGetWin32HandleInfo) + '!');
winsockSendToUI(&ConnectSocket,"pHandle=" + ptrToString((void*)pHandle) + '!');
#ifdef GETMEMORYWIN32HANDLEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetMemoryWin32HandleKHR_after(device, pGetWin32HandleInfo, pHandle);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetMemoryWin32HandleKHR!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetMemoryWin32HandlePropertiesKHR!");
}
#ifdef GETMEMORYWIN32HANDLEPROPERTIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetMemoryWin32HandlePropertiesKHR_before(device, handleType, handle, pMemoryWin32HandleProperties);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetMemoryWin32HandlePropertiesKHR(device, handleType, handle, pMemoryWin32HandleProperties);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"handleType=" + std::to_string(handleType) + '!');
winsockSendToUI(&ConnectSocket,"handle=" + ptrToString((void*)handle) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryWin32HandleProperties=" + ptrToString((void*)pMemoryWin32HandleProperties) + '!');
#ifdef GETMEMORYWIN32HANDLEPROPERTIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetMemoryWin32HandlePropertiesKHR_after(device, handleType, handle, pMemoryWin32HandleProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetMemoryWin32HandlePropertiesKHR!");
}
return ret;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryFdKHR(VkDevice device, VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetMemoryFdKHR!");
}
#ifdef GETMEMORYFDKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetMemoryFdKHR_before(device, pGetFdInfo, pFd);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetMemoryFdKHR(device, pGetFdInfo, pFd);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pGetFdInfo=" + ptrToString((void*)pGetFdInfo) + '!');
winsockSendToUI(&ConnectSocket,"pFd=" + ptrToString((void*)pFd) + '!');
#ifdef GETMEMORYFDKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetMemoryFdKHR_after(device, pGetFdInfo, pFd);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetMemoryFdKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetMemoryFdPropertiesKHR!");
}
#ifdef GETMEMORYFDPROPERTIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetMemoryFdPropertiesKHR_before(device, handleType, fd, pMemoryFdProperties);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetMemoryFdPropertiesKHR(device, handleType, fd, pMemoryFdProperties);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"handleType=" + std::to_string(handleType) + '!');
winsockSendToUI(&ConnectSocket,"fd=" + std::to_string(fd) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryFdProperties=" + ptrToString((void*)pMemoryFdProperties) + '!');
#ifdef GETMEMORYFDPROPERTIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetMemoryFdPropertiesKHR_after(device, handleType, fd, pMemoryFdProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetMemoryFdPropertiesKHR!");
}
return ret;
}

#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryZirconHandleFUCHSIA(VkDevice device, VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetMemoryZirconHandleFUCHSIA!");
}
#ifdef GETMEMORYZIRCONHANDLEFUCHSIA_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetMemoryZirconHandleFUCHSIA_before(device, pGetZirconHandleInfo, pZirconHandle);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetMemoryZirconHandleFUCHSIA(device, pGetZirconHandleInfo, pZirconHandle);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pGetZirconHandleInfo=" + ptrToString((void*)pGetZirconHandleInfo) + '!');
winsockSendToUI(&ConnectSocket,"pZirconHandle=" + ptrToString((void*)pZirconHandle) + '!');
#ifdef GETMEMORYZIRCONHANDLEFUCHSIA_AFTER_EXEC_EXISTS
if(connected) {
layer_GetMemoryZirconHandleFUCHSIA_after(device, pGetZirconHandleInfo, pZirconHandle);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetMemoryZirconHandleFUCHSIA!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryZirconHandlePropertiesFUCHSIA(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetMemoryZirconHandlePropertiesFUCHSIA!");
}
#ifdef GETMEMORYZIRCONHANDLEPROPERTIESFUCHSIA_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetMemoryZirconHandlePropertiesFUCHSIA_before(device, handleType, zirconHandle, pMemoryZirconHandleProperties);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetMemoryZirconHandlePropertiesFUCHSIA(device, handleType, zirconHandle, pMemoryZirconHandleProperties);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"handleType=" + std::to_string(handleType) + '!');
winsockSendToUI(&ConnectSocket,"zirconHandle=" + ptrToString((void*)zirconHandle) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryZirconHandleProperties=" + ptrToString((void*)pMemoryZirconHandleProperties) + '!');
#ifdef GETMEMORYZIRCONHANDLEPROPERTIESFUCHSIA_AFTER_EXEC_EXISTS
if(connected) {
layer_GetMemoryZirconHandlePropertiesFUCHSIA_after(device, handleType, zirconHandle, pMemoryZirconHandleProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetMemoryZirconHandlePropertiesFUCHSIA!");
}
return ret;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryRemoteAddressNV(VkDevice device, VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetMemoryRemoteAddressNV!");
}
#ifdef GETMEMORYREMOTEADDRESSNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetMemoryRemoteAddressNV_before(device, pMemoryGetRemoteAddressInfo, pAddress);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetMemoryRemoteAddressNV(device, pMemoryGetRemoteAddressInfo, pAddress);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryGetRemoteAddressInfo=" + ptrToString((void*)pMemoryGetRemoteAddressInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAddress=" + ptrToString((void*)pAddress) + '!');
#ifdef GETMEMORYREMOTEADDRESSNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetMemoryRemoteAddressNV_after(device, pMemoryGetRemoteAddressInfo, pAddress);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetMemoryRemoteAddressNV!");
}
return ret;
}

#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemorySciBufNV(VkDevice device, VkMemoryGetSciBufInfoNV* pGetSciBufInfo, NvSciBufObj* pHandle) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetMemorySciBufNV!");
}
#ifdef GETMEMORYSCIBUFNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetMemorySciBufNV_before(device, pGetSciBufInfo, pHandle);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetMemorySciBufNV(device, pGetSciBufInfo, pHandle);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pGetSciBufInfo=" + ptrToString((void*)pGetSciBufInfo) + '!');
winsockSendToUI(&ConnectSocket,"pHandle=" + ptrToString((void*)pHandle) + '!');
#ifdef GETMEMORYSCIBUFNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetMemorySciBufNV_after(device, pGetSciBufInfo, pHandle);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetMemorySciBufNV!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSemaphoreWin32HandleKHR(VkDevice device, VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetSemaphoreWin32HandleKHR!");
}
#ifdef GETSEMAPHOREWIN32HANDLEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetSemaphoreWin32HandleKHR_before(device, pGetWin32HandleInfo, pHandle);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetSemaphoreWin32HandleKHR(device, pGetWin32HandleInfo, pHandle);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pGetWin32HandleInfo=" + ptrToString((void*)pGetWin32HandleInfo) + '!');
winsockSendToUI(&ConnectSocket,"pHandle=" + ptrToString((void*)pHandle) + '!');
#ifdef GETSEMAPHOREWIN32HANDLEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetSemaphoreWin32HandleKHR_after(device, pGetWin32HandleInfo, pHandle);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetSemaphoreWin32HandleKHR!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ImportSemaphoreWin32HandleKHR(VkDevice device, VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkImportSemaphoreWin32HandleKHR!");
}
#ifdef IMPORTSEMAPHOREWIN32HANDLEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_ImportSemaphoreWin32HandleKHR_before(device, pImportSemaphoreWin32HandleInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].ImportSemaphoreWin32HandleKHR(device, pImportSemaphoreWin32HandleInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pImportSemaphoreWin32HandleInfo=" + ptrToString((void*)pImportSemaphoreWin32HandleInfo) + '!');
#ifdef IMPORTSEMAPHOREWIN32HANDLEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_ImportSemaphoreWin32HandleKHR_after(device, pImportSemaphoreWin32HandleInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkImportSemaphoreWin32HandleKHR!");
}
return ret;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSemaphoreFdKHR(VkDevice device, VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetSemaphoreFdKHR!");
}
#ifdef GETSEMAPHOREFDKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetSemaphoreFdKHR_before(device, pGetFdInfo, pFd);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetSemaphoreFdKHR(device, pGetFdInfo, pFd);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pGetFdInfo=" + ptrToString((void*)pGetFdInfo) + '!');
winsockSendToUI(&ConnectSocket,"pFd=" + ptrToString((void*)pFd) + '!');
#ifdef GETSEMAPHOREFDKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetSemaphoreFdKHR_after(device, pGetFdInfo, pFd);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetSemaphoreFdKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ImportSemaphoreFdKHR(VkDevice device, VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkImportSemaphoreFdKHR!");
}
#ifdef IMPORTSEMAPHOREFDKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_ImportSemaphoreFdKHR_before(device, pImportSemaphoreFdInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].ImportSemaphoreFdKHR(device, pImportSemaphoreFdInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pImportSemaphoreFdInfo=" + ptrToString((void*)pImportSemaphoreFdInfo) + '!');
#ifdef IMPORTSEMAPHOREFDKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_ImportSemaphoreFdKHR_after(device, pImportSemaphoreFdInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkImportSemaphoreFdKHR!");
}
return ret;
}

#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSemaphoreZirconHandleFUCHSIA(VkDevice device, VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetSemaphoreZirconHandleFUCHSIA!");
}
#ifdef GETSEMAPHOREZIRCONHANDLEFUCHSIA_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetSemaphoreZirconHandleFUCHSIA_before(device, pGetZirconHandleInfo, pZirconHandle);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetSemaphoreZirconHandleFUCHSIA(device, pGetZirconHandleInfo, pZirconHandle);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pGetZirconHandleInfo=" + ptrToString((void*)pGetZirconHandleInfo) + '!');
winsockSendToUI(&ConnectSocket,"pZirconHandle=" + ptrToString((void*)pZirconHandle) + '!');
#ifdef GETSEMAPHOREZIRCONHANDLEFUCHSIA_AFTER_EXEC_EXISTS
if(connected) {
layer_GetSemaphoreZirconHandleFUCHSIA_after(device, pGetZirconHandleInfo, pZirconHandle);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetSemaphoreZirconHandleFUCHSIA!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ImportSemaphoreZirconHandleFUCHSIA(VkDevice device, VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkImportSemaphoreZirconHandleFUCHSIA!");
}
#ifdef IMPORTSEMAPHOREZIRCONHANDLEFUCHSIA_BEFORE_EXEC_EXISTS
if(connected) {
layer_ImportSemaphoreZirconHandleFUCHSIA_before(device, pImportSemaphoreZirconHandleInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].ImportSemaphoreZirconHandleFUCHSIA(device, pImportSemaphoreZirconHandleInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pImportSemaphoreZirconHandleInfo=" + ptrToString((void*)pImportSemaphoreZirconHandleInfo) + '!');
#ifdef IMPORTSEMAPHOREZIRCONHANDLEFUCHSIA_AFTER_EXEC_EXISTS
if(connected) {
layer_ImportSemaphoreZirconHandleFUCHSIA_after(device, pImportSemaphoreZirconHandleInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkImportSemaphoreZirconHandleFUCHSIA!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetFenceWin32HandleKHR(VkDevice device, VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetFenceWin32HandleKHR!");
}
#ifdef GETFENCEWIN32HANDLEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetFenceWin32HandleKHR_before(device, pGetWin32HandleInfo, pHandle);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetFenceWin32HandleKHR(device, pGetWin32HandleInfo, pHandle);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pGetWin32HandleInfo=" + ptrToString((void*)pGetWin32HandleInfo) + '!');
winsockSendToUI(&ConnectSocket,"pHandle=" + ptrToString((void*)pHandle) + '!');
#ifdef GETFENCEWIN32HANDLEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetFenceWin32HandleKHR_after(device, pGetWin32HandleInfo, pHandle);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetFenceWin32HandleKHR!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ImportFenceWin32HandleKHR(VkDevice device, VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkImportFenceWin32HandleKHR!");
}
#ifdef IMPORTFENCEWIN32HANDLEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_ImportFenceWin32HandleKHR_before(device, pImportFenceWin32HandleInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].ImportFenceWin32HandleKHR(device, pImportFenceWin32HandleInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pImportFenceWin32HandleInfo=" + ptrToString((void*)pImportFenceWin32HandleInfo) + '!');
#ifdef IMPORTFENCEWIN32HANDLEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_ImportFenceWin32HandleKHR_after(device, pImportFenceWin32HandleInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkImportFenceWin32HandleKHR!");
}
return ret;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetFenceFdKHR(VkDevice device, VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetFenceFdKHR!");
}
#ifdef GETFENCEFDKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetFenceFdKHR_before(device, pGetFdInfo, pFd);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetFenceFdKHR(device, pGetFdInfo, pFd);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pGetFdInfo=" + ptrToString((void*)pGetFdInfo) + '!');
winsockSendToUI(&ConnectSocket,"pFd=" + ptrToString((void*)pFd) + '!');
#ifdef GETFENCEFDKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetFenceFdKHR_after(device, pGetFdInfo, pFd);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetFenceFdKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ImportFenceFdKHR(VkDevice device, VkImportFenceFdInfoKHR* pImportFenceFdInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkImportFenceFdKHR!");
}
#ifdef IMPORTFENCEFDKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_ImportFenceFdKHR_before(device, pImportFenceFdInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].ImportFenceFdKHR(device, pImportFenceFdInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pImportFenceFdInfo=" + ptrToString((void*)pImportFenceFdInfo) + '!');
#ifdef IMPORTFENCEFDKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_ImportFenceFdKHR_after(device, pImportFenceFdInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkImportFenceFdKHR!");
}
return ret;
}

#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetFenceSciSyncFenceNV(VkDevice device, VkFenceGetSciSyncInfoNV* pGetSciSyncHandleInfo, void* pHandle) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetFenceSciSyncFenceNV!");
}
#ifdef GETFENCESCISYNCFENCENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetFenceSciSyncFenceNV_before(device, pGetSciSyncHandleInfo, pHandle);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetFenceSciSyncFenceNV(device, pGetSciSyncHandleInfo, pHandle);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pGetSciSyncHandleInfo=" + ptrToString((void*)pGetSciSyncHandleInfo) + '!');
winsockSendToUI(&ConnectSocket,"pHandle=" + ptrToString((void*)pHandle) + '!');
#ifdef GETFENCESCISYNCFENCENV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetFenceSciSyncFenceNV_after(device, pGetSciSyncHandleInfo, pHandle);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetFenceSciSyncFenceNV!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetFenceSciSyncObjNV(VkDevice device, VkFenceGetSciSyncInfoNV* pGetSciSyncHandleInfo, void* pHandle) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetFenceSciSyncObjNV!");
}
#ifdef GETFENCESCISYNCOBJNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetFenceSciSyncObjNV_before(device, pGetSciSyncHandleInfo, pHandle);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetFenceSciSyncObjNV(device, pGetSciSyncHandleInfo, pHandle);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pGetSciSyncHandleInfo=" + ptrToString((void*)pGetSciSyncHandleInfo) + '!');
winsockSendToUI(&ConnectSocket,"pHandle=" + ptrToString((void*)pHandle) + '!');
#ifdef GETFENCESCISYNCOBJNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetFenceSciSyncObjNV_after(device, pGetSciSyncHandleInfo, pHandle);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetFenceSciSyncObjNV!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ImportFenceSciSyncFenceNV(VkDevice device, VkImportFenceSciSyncInfoNV* pImportFenceSciSyncInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkImportFenceSciSyncFenceNV!");
}
#ifdef IMPORTFENCESCISYNCFENCENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_ImportFenceSciSyncFenceNV_before(device, pImportFenceSciSyncInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].ImportFenceSciSyncFenceNV(device, pImportFenceSciSyncInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pImportFenceSciSyncInfo=" + ptrToString((void*)pImportFenceSciSyncInfo) + '!');
#ifdef IMPORTFENCESCISYNCFENCENV_AFTER_EXEC_EXISTS
if(connected) {
layer_ImportFenceSciSyncFenceNV_after(device, pImportFenceSciSyncInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkImportFenceSciSyncFenceNV!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ImportFenceSciSyncObjNV(VkDevice device, VkImportFenceSciSyncInfoNV* pImportFenceSciSyncInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkImportFenceSciSyncObjNV!");
}
#ifdef IMPORTFENCESCISYNCOBJNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_ImportFenceSciSyncObjNV_before(device, pImportFenceSciSyncInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].ImportFenceSciSyncObjNV(device, pImportFenceSciSyncInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pImportFenceSciSyncInfo=" + ptrToString((void*)pImportFenceSciSyncInfo) + '!');
#ifdef IMPORTFENCESCISYNCOBJNV_AFTER_EXEC_EXISTS
if(connected) {
layer_ImportFenceSciSyncObjNV_after(device, pImportFenceSciSyncInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkImportFenceSciSyncObjNV!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSemaphoreSciSyncObjNV(VkDevice device, VkSemaphoreGetSciSyncInfoNV* pGetSciSyncInfo, void* pHandle) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetSemaphoreSciSyncObjNV!");
}
#ifdef GETSEMAPHORESCISYNCOBJNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetSemaphoreSciSyncObjNV_before(device, pGetSciSyncInfo, pHandle);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetSemaphoreSciSyncObjNV(device, pGetSciSyncInfo, pHandle);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pGetSciSyncInfo=" + ptrToString((void*)pGetSciSyncInfo) + '!');
winsockSendToUI(&ConnectSocket,"pHandle=" + ptrToString((void*)pHandle) + '!');
#ifdef GETSEMAPHORESCISYNCOBJNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetSemaphoreSciSyncObjNV_after(device, pGetSciSyncInfo, pHandle);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetSemaphoreSciSyncObjNV!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ImportSemaphoreSciSyncObjNV(VkDevice device, VkImportSemaphoreSciSyncInfoNV* pImportSemaphoreSciSyncInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkImportSemaphoreSciSyncObjNV!");
}
#ifdef IMPORTSEMAPHORESCISYNCOBJNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_ImportSemaphoreSciSyncObjNV_before(device, pImportSemaphoreSciSyncInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].ImportSemaphoreSciSyncObjNV(device, pImportSemaphoreSciSyncInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pImportSemaphoreSciSyncInfo=" + ptrToString((void*)pImportSemaphoreSciSyncInfo) + '!');
#ifdef IMPORTSEMAPHORESCISYNCOBJNV_AFTER_EXEC_EXISTS
if(connected) {
layer_ImportSemaphoreSciSyncObjNV_after(device, pImportSemaphoreSciSyncInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkImportSemaphoreSciSyncObjNV!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateSemaphoreSciSyncPoolNV(VkDevice device, VkSemaphoreSciSyncPoolCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSemaphoreSciSyncPoolNV* pSemaphorePool) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateSemaphoreSciSyncPoolNV!");
}
#ifdef CREATESEMAPHORESCISYNCPOOLNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateSemaphoreSciSyncPoolNV_before(device, pCreateInfo, pAllocator, pSemaphorePool);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateSemaphoreSciSyncPoolNV(device, pCreateInfo, pAllocator, pSemaphorePool);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSemaphorePool=" + ptrToString((void*)pSemaphorePool) + '!');
#ifdef CREATESEMAPHORESCISYNCPOOLNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateSemaphoreSciSyncPoolNV_after(device, pCreateInfo, pAllocator, pSemaphorePool);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateSemaphoreSciSyncPoolNV!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroySemaphoreSciSyncPoolNV(VkDevice device, VkSemaphoreSciSyncPoolNV semaphorePool, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroySemaphoreSciSyncPoolNV!");
}
#ifdef DESTROYSEMAPHORESCISYNCPOOLNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroySemaphoreSciSyncPoolNV_before(device, semaphorePool, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroySemaphoreSciSyncPoolNV(device, semaphorePool, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"semaphorePool=" + ptrToString((void*)semaphorePool) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYSEMAPHORESCISYNCPOOLNV_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroySemaphoreSciSyncPoolNV_after(device, semaphorePool, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroySemaphoreSciSyncPoolNV!");
}
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, VkDisplayPowerInfoEXT* pDisplayPowerInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDisplayPowerControlEXT!");
}
#ifdef DISPLAYPOWERCONTROLEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_DisplayPowerControlEXT_before(device, display, pDisplayPowerInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].DisplayPowerControlEXT(device, display, pDisplayPowerInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"display=" + ptrToString((void*)display) + '!');
winsockSendToUI(&ConnectSocket,"pDisplayPowerInfo=" + ptrToString((void*)pDisplayPowerInfo) + '!');
#ifdef DISPLAYPOWERCONTROLEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_DisplayPowerControlEXT_after(device, display, pDisplayPowerInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDisplayPowerControlEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_RegisterDeviceEventEXT(VkDevice device, VkDeviceEventInfoEXT* pDeviceEventInfo, VkAllocationCallbacks* pAllocator, VkFence* pFence) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkRegisterDeviceEventEXT!");
}
#ifdef REGISTERDEVICEEVENTEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_RegisterDeviceEventEXT_before(device, pDeviceEventInfo, pAllocator, pFence);
}
#endif 
auto ret = device_dispatch[GetKey(device)].RegisterDeviceEventEXT(device, pDeviceEventInfo, pAllocator, pFence);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pDeviceEventInfo=" + ptrToString((void*)pDeviceEventInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pFence=" + ptrToString((void*)pFence) + '!');
#ifdef REGISTERDEVICEEVENTEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_RegisterDeviceEventEXT_after(device, pDeviceEventInfo, pAllocator, pFence);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkRegisterDeviceEventEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, VkDisplayEventInfoEXT* pDisplayEventInfo, VkAllocationCallbacks* pAllocator, VkFence* pFence) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkRegisterDisplayEventEXT!");
}
#ifdef REGISTERDISPLAYEVENTEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_RegisterDisplayEventEXT_before(device, display, pDisplayEventInfo, pAllocator, pFence);
}
#endif 
auto ret = device_dispatch[GetKey(device)].RegisterDisplayEventEXT(device, display, pDisplayEventInfo, pAllocator, pFence);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"display=" + ptrToString((void*)display) + '!');
winsockSendToUI(&ConnectSocket,"pDisplayEventInfo=" + ptrToString((void*)pDisplayEventInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pFence=" + ptrToString((void*)pFence) + '!');
#ifdef REGISTERDISPLAYEVENTEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_RegisterDisplayEventEXT_after(device, display, pDisplayEventInfo, pAllocator, pFence);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkRegisterDisplayEventEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetSwapchainCounterEXT!");
}
#ifdef GETSWAPCHAINCOUNTEREXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetSwapchainCounterEXT_before(device, swapchain, counter, pCounterValue);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetSwapchainCounterEXT(device, swapchain, counter, pCounterValue);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapchain=" + ptrToString((void*)swapchain) + '!');
winsockSendToUI(&ConnectSocket,"counter=" + std::to_string(counter) + '!');
winsockSendToUI(&ConnectSocket,"pCounterValue=" + ptrToString((void*)pCounterValue) + '!');
#ifdef GETSWAPCHAINCOUNTEREXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetSwapchainCounterEXT_after(device, swapchain, counter, pCounterValue);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetSwapchainCounterEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceGroupPeerMemoryFeatures!");
}
#ifdef GETDEVICEGROUPPEERMEMORYFEATURES_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceGroupPeerMemoryFeatures_before(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}
#endif 
device_dispatch[GetKey(device)].GetDeviceGroupPeerMemoryFeatures(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"heapIndex=" + std::to_string(heapIndex) + '!');
winsockSendToUI(&ConnectSocket,"localDeviceIndex=" + std::to_string(localDeviceIndex) + '!');
winsockSendToUI(&ConnectSocket,"remoteDeviceIndex=" + std::to_string(remoteDeviceIndex) + '!');
winsockSendToUI(&ConnectSocket,"pPeerMemoryFeatures=" + ptrToString((void*)pPeerMemoryFeatures) + '!');
#ifdef GETDEVICEGROUPPEERMEMORYFEATURES_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceGroupPeerMemoryFeatures_after(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceGroupPeerMemoryFeatures!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, VkBindBufferMemoryInfo* pBindInfos) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkBindBufferMemory2!");
}
#ifdef BINDBUFFERMEMORY2_BEFORE_EXEC_EXISTS
if(connected) {
layer_BindBufferMemory2_before(device, bindInfoCount, pBindInfos);
}
#endif 
auto ret = device_dispatch[GetKey(device)].BindBufferMemory2(device, bindInfoCount, pBindInfos);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"bindInfoCount=" + std::to_string(bindInfoCount) + '!');
winsockSendToUI(&ConnectSocket,"pBindInfos=" + ptrToString((void*)pBindInfos) + '!');
#ifdef BINDBUFFERMEMORY2_AFTER_EXEC_EXISTS
if(connected) {
layer_BindBufferMemory2_after(device, bindInfoCount, pBindInfos);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkBindBufferMemory2!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, VkBindImageMemoryInfo* pBindInfos) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkBindImageMemory2!");
}
#ifdef BINDIMAGEMEMORY2_BEFORE_EXEC_EXISTS
if(connected) {
layer_BindImageMemory2_before(device, bindInfoCount, pBindInfos);
}
#endif 
auto ret = device_dispatch[GetKey(device)].BindImageMemory2(device, bindInfoCount, pBindInfos);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"bindInfoCount=" + std::to_string(bindInfoCount) + '!');
winsockSendToUI(&ConnectSocket,"pBindInfos=" + ptrToString((void*)pBindInfos) + '!');
#ifdef BINDIMAGEMEMORY2_AFTER_EXEC_EXISTS
if(connected) {
layer_BindImageMemory2_after(device, bindInfoCount, pBindInfos);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkBindImageMemory2!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDeviceMask!");
}
#ifdef CMDSETDEVICEMASK_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDeviceMask_before(commandBuffer, deviceMask);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDeviceMask(commandBuffer, deviceMask);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"deviceMask=" + std::to_string(deviceMask) + '!');
#ifdef CMDSETDEVICEMASK_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDeviceMask_after(commandBuffer, deviceMask);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDeviceMask!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceGroupPresentCapabilitiesKHR!");
}
#ifdef GETDEVICEGROUPPRESENTCAPABILITIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceGroupPresentCapabilitiesKHR_before(device, pDeviceGroupPresentCapabilities);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetDeviceGroupPresentCapabilitiesKHR(device, pDeviceGroupPresentCapabilities);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pDeviceGroupPresentCapabilities=" + ptrToString((void*)pDeviceGroupPresentCapabilities) + '!');
#ifdef GETDEVICEGROUPPRESENTCAPABILITIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceGroupPresentCapabilitiesKHR_after(device, pDeviceGroupPresentCapabilities);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceGroupPresentCapabilitiesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceGroupSurfacePresentModesKHR!");
}
#ifdef GETDEVICEGROUPSURFACEPRESENTMODESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceGroupSurfacePresentModesKHR_before(device, surface, pModes);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetDeviceGroupSurfacePresentModesKHR(device, surface, pModes);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"surface=" + ptrToString((void*)surface) + '!');
winsockSendToUI(&ConnectSocket,"pModes=" + ptrToString((void*)pModes) + '!');
#ifdef GETDEVICEGROUPSURFACEPRESENTMODESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceGroupSurfacePresentModesKHR_after(device, surface, pModes);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceGroupSurfacePresentModesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquireNextImage2KHR(VkDevice device, VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkAcquireNextImage2KHR!");
}
#ifdef ACQUIRENEXTIMAGE2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_AcquireNextImage2KHR_before(device, pAcquireInfo, pImageIndex);
}
#endif 
auto ret = device_dispatch[GetKey(device)].AcquireNextImage2KHR(device, pAcquireInfo, pImageIndex);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pAcquireInfo=" + ptrToString((void*)pAcquireInfo) + '!');
winsockSendToUI(&ConnectSocket,"pImageIndex=" + ptrToString((void*)pImageIndex) + '!');
#ifdef ACQUIRENEXTIMAGE2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_AcquireNextImage2KHR_after(device, pAcquireInfo, pImageIndex);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkAcquireNextImage2KHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDispatchBase!");
}
#ifdef CMDDISPATCHBASE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDispatchBase_before(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"baseGroupX=" + std::to_string(baseGroupX) + '!');
winsockSendToUI(&ConnectSocket,"baseGroupY=" + std::to_string(baseGroupY) + '!');
winsockSendToUI(&ConnectSocket,"baseGroupZ=" + std::to_string(baseGroupZ) + '!');
winsockSendToUI(&ConnectSocket,"groupCountX=" + std::to_string(groupCountX) + '!');
winsockSendToUI(&ConnectSocket,"groupCountY=" + std::to_string(groupCountY) + '!');
winsockSendToUI(&ConnectSocket,"groupCountZ=" + std::to_string(groupCountZ) + '!');
#ifdef CMDDISPATCHBASE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDispatchBase_after(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDispatchBase!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateDescriptorUpdateTemplate!");
}
#ifdef CREATEDESCRIPTORUPDATETEMPLATE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateDescriptorUpdateTemplate_before(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateDescriptorUpdateTemplate(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pDescriptorUpdateTemplate=" + ptrToString((void*)pDescriptorUpdateTemplate) + '!');
#ifdef CREATEDESCRIPTORUPDATETEMPLATE_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateDescriptorUpdateTemplate_after(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateDescriptorUpdateTemplate!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyDescriptorUpdateTemplate!");
}
#ifdef DESTROYDESCRIPTORUPDATETEMPLATE_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyDescriptorUpdateTemplate_before(device, descriptorUpdateTemplate, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyDescriptorUpdateTemplate(device, descriptorUpdateTemplate, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"descriptorUpdateTemplate=" + ptrToString((void*)descriptorUpdateTemplate) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYDESCRIPTORUPDATETEMPLATE_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyDescriptorUpdateTemplate_after(device, descriptorUpdateTemplate, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyDescriptorUpdateTemplate!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkUpdateDescriptorSetWithTemplate!");
}
#ifdef UPDATEDESCRIPTORSETWITHTEMPLATE_BEFORE_EXEC_EXISTS
if(connected) {
layer_UpdateDescriptorSetWithTemplate_before(device, descriptorSet, descriptorUpdateTemplate, pData);
}
#endif 
device_dispatch[GetKey(device)].UpdateDescriptorSetWithTemplate(device, descriptorSet, descriptorUpdateTemplate, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"descriptorSet=" + ptrToString((void*)descriptorSet) + '!');
winsockSendToUI(&ConnectSocket,"descriptorUpdateTemplate=" + ptrToString((void*)descriptorUpdateTemplate) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef UPDATEDESCRIPTORSETWITHTEMPLATE_AFTER_EXEC_EXISTS
if(connected) {
layer_UpdateDescriptorSetWithTemplate_after(device, descriptorSet, descriptorUpdateTemplate, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkUpdateDescriptorSetWithTemplate!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdPushDescriptorSetWithTemplateKHR!");
}
#ifdef CMDPUSHDESCRIPTORSETWITHTEMPLATEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdPushDescriptorSetWithTemplateKHR_before(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"descriptorUpdateTemplate=" + ptrToString((void*)descriptorUpdateTemplate) + '!');
winsockSendToUI(&ConnectSocket,"layout=" + ptrToString((void*)layout) + '!');
winsockSendToUI(&ConnectSocket,"set=" + std::to_string(set) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef CMDPUSHDESCRIPTORSETWITHTEMPLATEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdPushDescriptorSetWithTemplateKHR_after(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdPushDescriptorSetWithTemplateKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_SetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, VkSwapchainKHR* pSwapchains, VkHdrMetadataEXT* pMetadata) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkSetHdrMetadataEXT!");
}
#ifdef SETHDRMETADATAEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_SetHdrMetadataEXT_before(device, swapchainCount, pSwapchains, pMetadata);
}
#endif 
device_dispatch[GetKey(device)].SetHdrMetadataEXT(device, swapchainCount, pSwapchains, pMetadata);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapchainCount=" + std::to_string(swapchainCount) + '!');
winsockSendToUI(&ConnectSocket,"pSwapchains=" + ptrToString((void*)pSwapchains) + '!');
winsockSendToUI(&ConnectSocket,"pMetadata=" + ptrToString((void*)pMetadata) + '!');
#ifdef SETHDRMETADATAEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_SetHdrMetadataEXT_after(device, swapchainCount, pSwapchains, pMetadata);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkSetHdrMetadataEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetSwapchainStatusKHR!");
}
#ifdef GETSWAPCHAINSTATUSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetSwapchainStatusKHR_before(device, swapchain);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetSwapchainStatusKHR(device, swapchain);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapchain=" + ptrToString((void*)swapchain) + '!');
#ifdef GETSWAPCHAINSTATUSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetSwapchainStatusKHR_after(device, swapchain);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetSwapchainStatusKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetRefreshCycleDurationGOOGLE!");
}
#ifdef GETREFRESHCYCLEDURATIONGOOGLE_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetRefreshCycleDurationGOOGLE_before(device, swapchain, pDisplayTimingProperties);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetRefreshCycleDurationGOOGLE(device, swapchain, pDisplayTimingProperties);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapchain=" + ptrToString((void*)swapchain) + '!');
winsockSendToUI(&ConnectSocket,"pDisplayTimingProperties=" + ptrToString((void*)pDisplayTimingProperties) + '!');
#ifdef GETREFRESHCYCLEDURATIONGOOGLE_AFTER_EXEC_EXISTS
if(connected) {
layer_GetRefreshCycleDurationGOOGLE_after(device, swapchain, pDisplayTimingProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetRefreshCycleDurationGOOGLE!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPastPresentationTimingGOOGLE!");
}
#ifdef GETPASTPRESENTATIONTIMINGGOOGLE_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPastPresentationTimingGOOGLE_before(device, swapchain, pPresentationTimingCount, pPresentationTimings);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetPastPresentationTimingGOOGLE(device, swapchain, pPresentationTimingCount, pPresentationTimings);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapchain=" + ptrToString((void*)swapchain) + '!');
winsockSendToUI(&ConnectSocket,"pPresentationTimingCount=" + ptrToString((void*)pPresentationTimingCount) + '!');
winsockSendToUI(&ConnectSocket,"pPresentationTimings=" + ptrToString((void*)pPresentationTimings) + '!');
#ifdef GETPASTPRESENTATIONTIMINGGOOGLE_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPastPresentationTimingGOOGLE_after(device, swapchain, pPresentationTimingCount, pPresentationTimings);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPastPresentationTimingGOOGLE!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewportWScalingNV* pViewportWScalings) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetViewportWScalingNV!");
}
#ifdef CMDSETVIEWPORTWSCALINGNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetViewportWScalingNV_before(commandBuffer, firstViewport, viewportCount, pViewportWScalings);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetViewportWScalingNV(commandBuffer, firstViewport, viewportCount, pViewportWScalings);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstViewport=" + std::to_string(firstViewport) + '!');
winsockSendToUI(&ConnectSocket,"viewportCount=" + std::to_string(viewportCount) + '!');
winsockSendToUI(&ConnectSocket,"pViewportWScalings=" + ptrToString((void*)pViewportWScalings) + '!');
#ifdef CMDSETVIEWPORTWSCALINGNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetViewportWScalingNV_after(commandBuffer, firstViewport, viewportCount, pViewportWScalings);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetViewportWScalingNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, VkRect2D* pDiscardRectangles) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDiscardRectangleEXT!");
}
#ifdef CMDSETDISCARDRECTANGLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDiscardRectangleEXT_before(commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDiscardRectangleEXT(commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstDiscardRectangle=" + std::to_string(firstDiscardRectangle) + '!');
winsockSendToUI(&ConnectSocket,"discardRectangleCount=" + std::to_string(discardRectangleCount) + '!');
winsockSendToUI(&ConnectSocket,"pDiscardRectangles=" + ptrToString((void*)pDiscardRectangles) + '!');
#ifdef CMDSETDISCARDRECTANGLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDiscardRectangleEXT_after(commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDiscardRectangleEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDiscardRectangleEnableEXT!");
}
#ifdef CMDSETDISCARDRECTANGLEENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDiscardRectangleEnableEXT_before(commandBuffer, discardRectangleEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDiscardRectangleEnableEXT(commandBuffer, discardRectangleEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"discardRectangleEnable=" + bool_as_text(discardRectangleEnable) + '!');
#ifdef CMDSETDISCARDRECTANGLEENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDiscardRectangleEnableEXT_after(commandBuffer, discardRectangleEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDiscardRectangleEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDiscardRectangleModeEXT!");
}
#ifdef CMDSETDISCARDRECTANGLEMODEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDiscardRectangleModeEXT_before(commandBuffer, discardRectangleMode);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDiscardRectangleModeEXT(commandBuffer, discardRectangleMode);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"discardRectangleMode=" + std::to_string(discardRectangleMode) + '!');
#ifdef CMDSETDISCARDRECTANGLEMODEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDiscardRectangleModeEXT_after(commandBuffer, discardRectangleMode);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDiscardRectangleModeEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, VkSampleLocationsInfoEXT* pSampleLocationsInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetSampleLocationsEXT!");
}
#ifdef CMDSETSAMPLELOCATIONSEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetSampleLocationsEXT_before(commandBuffer, pSampleLocationsInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pSampleLocationsInfo=" + ptrToString((void*)pSampleLocationsInfo) + '!');
#ifdef CMDSETSAMPLELOCATIONSEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetSampleLocationsEXT_after(commandBuffer, pSampleLocationsInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetSampleLocationsEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetBufferMemoryRequirements2(VkDevice device, VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetBufferMemoryRequirements2!");
}
#ifdef GETBUFFERMEMORYREQUIREMENTS2_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetBufferMemoryRequirements2_before(device, pInfo, pMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetBufferMemoryRequirements2(device, pInfo, pMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRequirements=" + ptrToString((void*)pMemoryRequirements) + '!');
#ifdef GETBUFFERMEMORYREQUIREMENTS2_AFTER_EXEC_EXISTS
if(connected) {
layer_GetBufferMemoryRequirements2_after(device, pInfo, pMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetBufferMemoryRequirements2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageMemoryRequirements2(VkDevice device, VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetImageMemoryRequirements2!");
}
#ifdef GETIMAGEMEMORYREQUIREMENTS2_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetImageMemoryRequirements2_before(device, pInfo, pMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetImageMemoryRequirements2(device, pInfo, pMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRequirements=" + ptrToString((void*)pMemoryRequirements) + '!');
#ifdef GETIMAGEMEMORYREQUIREMENTS2_AFTER_EXEC_EXISTS
if(connected) {
layer_GetImageMemoryRequirements2_after(device, pInfo, pMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetImageMemoryRequirements2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageSparseMemoryRequirements2(VkDevice device, VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetImageSparseMemoryRequirements2!");
}
#ifdef GETIMAGESPARSEMEMORYREQUIREMENTS2_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetImageSparseMemoryRequirements2_before(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetImageSparseMemoryRequirements2(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pSparseMemoryRequirementCount=" + ptrToString((void*)pSparseMemoryRequirementCount) + '!');
winsockSendToUI(&ConnectSocket,"pSparseMemoryRequirements=" + ptrToString((void*)pSparseMemoryRequirements) + '!');
#ifdef GETIMAGESPARSEMEMORYREQUIREMENTS2_AFTER_EXEC_EXISTS
if(connected) {
layer_GetImageSparseMemoryRequirements2_after(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetImageSparseMemoryRequirements2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceBufferMemoryRequirements(VkDevice device, VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceBufferMemoryRequirements!");
}
#ifdef GETDEVICEBUFFERMEMORYREQUIREMENTS_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceBufferMemoryRequirements_before(device, pInfo, pMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetDeviceBufferMemoryRequirements(device, pInfo, pMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRequirements=" + ptrToString((void*)pMemoryRequirements) + '!');
#ifdef GETDEVICEBUFFERMEMORYREQUIREMENTS_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceBufferMemoryRequirements_after(device, pInfo, pMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceBufferMemoryRequirements!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceImageMemoryRequirements(VkDevice device, VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceImageMemoryRequirements!");
}
#ifdef GETDEVICEIMAGEMEMORYREQUIREMENTS_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceImageMemoryRequirements_before(device, pInfo, pMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetDeviceImageMemoryRequirements(device, pInfo, pMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRequirements=" + ptrToString((void*)pMemoryRequirements) + '!');
#ifdef GETDEVICEIMAGEMEMORYREQUIREMENTS_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceImageMemoryRequirements_after(device, pInfo, pMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceImageMemoryRequirements!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceImageSparseMemoryRequirements(VkDevice device, VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceImageSparseMemoryRequirements!");
}
#ifdef GETDEVICEIMAGESPARSEMEMORYREQUIREMENTS_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceImageSparseMemoryRequirements_before(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetDeviceImageSparseMemoryRequirements(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pSparseMemoryRequirementCount=" + ptrToString((void*)pSparseMemoryRequirementCount) + '!');
winsockSendToUI(&ConnectSocket,"pSparseMemoryRequirements=" + ptrToString((void*)pSparseMemoryRequirements) + '!');
#ifdef GETDEVICEIMAGESPARSEMEMORYREQUIREMENTS_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceImageSparseMemoryRequirements_after(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceImageSparseMemoryRequirements!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateSamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversionCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateSamplerYcbcrConversion!");
}
#ifdef CREATESAMPLERYCBCRCONVERSION_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateSamplerYcbcrConversion_before(device, pCreateInfo, pAllocator, pYcbcrConversion);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateSamplerYcbcrConversion(device, pCreateInfo, pAllocator, pYcbcrConversion);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pYcbcrConversion=" + ptrToString((void*)pYcbcrConversion) + '!');
#ifdef CREATESAMPLERYCBCRCONVERSION_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateSamplerYcbcrConversion_after(device, pCreateInfo, pAllocator, pYcbcrConversion);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateSamplerYcbcrConversion!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroySamplerYcbcrConversion!");
}
#ifdef DESTROYSAMPLERYCBCRCONVERSION_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroySamplerYcbcrConversion_before(device, ycbcrConversion, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroySamplerYcbcrConversion(device, ycbcrConversion, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"ycbcrConversion=" + ptrToString((void*)ycbcrConversion) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYSAMPLERYCBCRCONVERSION_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroySamplerYcbcrConversion_after(device, ycbcrConversion, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroySamplerYcbcrConversion!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceQueue2(VkDevice device, VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceQueue2!");
}
#ifdef GETDEVICEQUEUE2_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceQueue2_before(device, pQueueInfo, pQueue);
}
#endif 
device_dispatch[GetKey(device)].GetDeviceQueue2(device, pQueueInfo, pQueue);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pQueueInfo=" + ptrToString((void*)pQueueInfo) + '!');
winsockSendToUI(&ConnectSocket,"pQueue=" + ptrToString((void*)pQueue) + '!');
#ifdef GETDEVICEQUEUE2_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceQueue2_after(device, pQueueInfo, pQueue);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceQueue2!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateValidationCacheEXT(VkDevice device, VkValidationCacheCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateValidationCacheEXT!");
}
#ifdef CREATEVALIDATIONCACHEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateValidationCacheEXT_before(device, pCreateInfo, pAllocator, pValidationCache);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateValidationCacheEXT(device, pCreateInfo, pAllocator, pValidationCache);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pValidationCache=" + ptrToString((void*)pValidationCache) + '!');
#ifdef CREATEVALIDATIONCACHEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateValidationCacheEXT_after(device, pCreateInfo, pAllocator, pValidationCache);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateValidationCacheEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyValidationCacheEXT!");
}
#ifdef DESTROYVALIDATIONCACHEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyValidationCacheEXT_before(device, validationCache, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyValidationCacheEXT(device, validationCache, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"validationCache=" + ptrToString((void*)validationCache) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYVALIDATIONCACHEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyValidationCacheEXT_after(device, validationCache, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyValidationCacheEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetValidationCacheDataEXT!");
}
#ifdef GETVALIDATIONCACHEDATAEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetValidationCacheDataEXT_before(device, validationCache, pDataSize, pData);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetValidationCacheDataEXT(device, validationCache, pDataSize, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"validationCache=" + ptrToString((void*)validationCache) + '!');
winsockSendToUI(&ConnectSocket,"pDataSize=" + ptrToString((void*)pDataSize) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef GETVALIDATIONCACHEDATAEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetValidationCacheDataEXT_after(device, validationCache, pDataSize, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetValidationCacheDataEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_MergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, VkValidationCacheEXT* pSrcCaches) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkMergeValidationCachesEXT!");
}
#ifdef MERGEVALIDATIONCACHESEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_MergeValidationCachesEXT_before(device, dstCache, srcCacheCount, pSrcCaches);
}
#endif 
auto ret = device_dispatch[GetKey(device)].MergeValidationCachesEXT(device, dstCache, srcCacheCount, pSrcCaches);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"dstCache=" + ptrToString((void*)dstCache) + '!');
winsockSendToUI(&ConnectSocket,"srcCacheCount=" + std::to_string(srcCacheCount) + '!');
winsockSendToUI(&ConnectSocket,"pSrcCaches=" + ptrToString((void*)pSrcCaches) + '!');
#ifdef MERGEVALIDATIONCACHESEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_MergeValidationCachesEXT_after(device, dstCache, srcCacheCount, pSrcCaches);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkMergeValidationCachesEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDescriptorSetLayoutSupport(VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDescriptorSetLayoutSupport!");
}
#ifdef GETDESCRIPTORSETLAYOUTSUPPORT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDescriptorSetLayoutSupport_before(device, pCreateInfo, pSupport);
}
#endif 
device_dispatch[GetKey(device)].GetDescriptorSetLayoutSupport(device, pCreateInfo, pSupport);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pSupport=" + ptrToString((void*)pSupport) + '!');
#ifdef GETDESCRIPTORSETLAYOUTSUPPORT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDescriptorSetLayoutSupport_after(device, pCreateInfo, pSupport);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDescriptorSetLayoutSupport!");
}
}

#if defined(VK_USE_PLATFORM_ANDROID_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetSwapchainGrallocUsageANDROID!");
}
#ifdef GETSWAPCHAINGRALLOCUSAGEANDROID_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetSwapchainGrallocUsageANDROID_before(device, format, imageUsage, grallocUsage);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetSwapchainGrallocUsageANDROID(device, format, imageUsage, grallocUsage);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"format=" + std::to_string(format) + '!');
winsockSendToUI(&ConnectSocket,"imageUsage=" + ptrToString((void*)imageUsage) + '!');
winsockSendToUI(&ConnectSocket,"grallocUsage=" + ptrToString((void*)grallocUsage) + '!');
#ifdef GETSWAPCHAINGRALLOCUSAGEANDROID_AFTER_EXEC_EXISTS
if(connected) {
layer_GetSwapchainGrallocUsageANDROID_after(device, format, imageUsage, grallocUsage);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetSwapchainGrallocUsageANDROID!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetSwapchainGrallocUsage2ANDROID!");
}
#ifdef GETSWAPCHAINGRALLOCUSAGE2ANDROID_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetSwapchainGrallocUsage2ANDROID_before(device, format, imageUsage, swapchainImageUsage, grallocConsumerUsage, grallocProducerUsage);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetSwapchainGrallocUsage2ANDROID(device, format, imageUsage, swapchainImageUsage, grallocConsumerUsage, grallocProducerUsage);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"format=" + std::to_string(format) + '!');
winsockSendToUI(&ConnectSocket,"imageUsage=" + ptrToString((void*)imageUsage) + '!');
winsockSendToUI(&ConnectSocket,"swapchainImageUsage=" + ptrToString((void*)swapchainImageUsage) + '!');
winsockSendToUI(&ConnectSocket,"grallocConsumerUsage=" + ptrToString((void*)grallocConsumerUsage) + '!');
winsockSendToUI(&ConnectSocket,"grallocProducerUsage=" + ptrToString((void*)grallocProducerUsage) + '!');
#ifdef GETSWAPCHAINGRALLOCUSAGE2ANDROID_AFTER_EXEC_EXISTS
if(connected) {
layer_GetSwapchainGrallocUsage2ANDROID_after(device, format, imageUsage, swapchainImageUsage, grallocConsumerUsage, grallocProducerUsage);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetSwapchainGrallocUsage2ANDROID!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkAcquireImageANDROID!");
}
#ifdef ACQUIREIMAGEANDROID_BEFORE_EXEC_EXISTS
if(connected) {
layer_AcquireImageANDROID_before(device, image, nativeFenceFd, semaphore, fence);
}
#endif 
auto ret = device_dispatch[GetKey(device)].AcquireImageANDROID(device, image, nativeFenceFd, semaphore, fence);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"image=" + ptrToString((void*)image) + '!');
winsockSendToUI(&ConnectSocket,"nativeFenceFd=" + std::to_string(nativeFenceFd) + '!');
winsockSendToUI(&ConnectSocket,"semaphore=" + ptrToString((void*)semaphore) + '!');
winsockSendToUI(&ConnectSocket,"fence=" + ptrToString((void*)fence) + '!');
#ifdef ACQUIREIMAGEANDROID_AFTER_EXEC_EXISTS
if(connected) {
layer_AcquireImageANDROID_after(device, image, nativeFenceFd, semaphore, fence);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkAcquireImageANDROID!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkQueueSignalReleaseImageANDROID!");
}
#ifdef QUEUESIGNALRELEASEIMAGEANDROID_BEFORE_EXEC_EXISTS
if(connected) {
layer_QueueSignalReleaseImageANDROID_before(queue, waitSemaphoreCount, pWaitSemaphores, image, pNativeFenceFd);
}
#endif 
auto ret = device_dispatch[GetKey(queue)].QueueSignalReleaseImageANDROID(queue, waitSemaphoreCount, pWaitSemaphores, image, pNativeFenceFd);
winsockSendToUI(&ConnectSocket,"queue=" + ptrToString((void*)queue) + '!');
winsockSendToUI(&ConnectSocket,"waitSemaphoreCount=" + std::to_string(waitSemaphoreCount) + '!');
winsockSendToUI(&ConnectSocket,"pWaitSemaphores=" + ptrToString((void*)pWaitSemaphores) + '!');
winsockSendToUI(&ConnectSocket,"image=" + ptrToString((void*)image) + '!');
winsockSendToUI(&ConnectSocket,"pNativeFenceFd=" + ptrToString((void*)pNativeFenceFd) + '!');
#ifdef QUEUESIGNALRELEASEIMAGEANDROID_AFTER_EXEC_EXISTS
if(connected) {
layer_QueueSignalReleaseImageANDROID_after(queue, waitSemaphoreCount, pWaitSemaphores, image, pNativeFenceFd);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkQueueSignalReleaseImageANDROID!");
}
return ret;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetShaderInfoAMD!");
}
#ifdef GETSHADERINFOAMD_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetShaderInfoAMD_before(device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetShaderInfoAMD(device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pipeline=" + ptrToString((void*)pipeline) + '!');
winsockSendToUI(&ConnectSocket,"shaderStage=" + std::to_string(shaderStage) + '!');
winsockSendToUI(&ConnectSocket,"infoType=" + std::to_string(infoType) + '!');
winsockSendToUI(&ConnectSocket,"pInfoSize=" + ptrToString((void*)pInfoSize) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef GETSHADERINFOAMD_AFTER_EXEC_EXISTS
if(connected) {
layer_GetShaderInfoAMD_after(device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetShaderInfoAMD!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_SetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkSetLocalDimmingAMD!");
}
#ifdef SETLOCALDIMMINGAMD_BEFORE_EXEC_EXISTS
if(connected) {
layer_SetLocalDimmingAMD_before(device, swapChain, localDimmingEnable);
}
#endif 
device_dispatch[GetKey(device)].SetLocalDimmingAMD(device, swapChain, localDimmingEnable);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapChain=" + ptrToString((void*)swapChain) + '!');
winsockSendToUI(&ConnectSocket,"localDimmingEnable=" + bool_as_text(localDimmingEnable) + '!');
#ifdef SETLOCALDIMMINGAMD_AFTER_EXEC_EXISTS
if(connected) {
layer_SetLocalDimmingAMD_after(device, swapChain, localDimmingEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkSetLocalDimmingAMD!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetCalibratedTimestampsKHR(VkDevice device, uint32_t timestampCount, VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetCalibratedTimestampsKHR!");
}
#ifdef GETCALIBRATEDTIMESTAMPSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetCalibratedTimestampsKHR_before(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetCalibratedTimestampsKHR(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"timestampCount=" + std::to_string(timestampCount) + '!');
winsockSendToUI(&ConnectSocket,"pTimestampInfos=" + ptrToString((void*)pTimestampInfos) + '!');
winsockSendToUI(&ConnectSocket,"pTimestamps=" + ptrToString((void*)pTimestamps) + '!');
winsockSendToUI(&ConnectSocket,"pMaxDeviation=" + ptrToString((void*)pMaxDeviation) + '!');
#ifdef GETCALIBRATEDTIMESTAMPSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetCalibratedTimestampsKHR_after(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetCalibratedTimestampsKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SetDebugUtilsObjectNameEXT(VkDevice device, VkDebugUtilsObjectNameInfoEXT* pNameInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkSetDebugUtilsObjectNameEXT!");
}
#ifdef SETDEBUGUTILSOBJECTNAMEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_SetDebugUtilsObjectNameEXT_before(device, pNameInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].SetDebugUtilsObjectNameEXT(device, pNameInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pNameInfo=" + ptrToString((void*)pNameInfo) + '!');
#ifdef SETDEBUGUTILSOBJECTNAMEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_SetDebugUtilsObjectNameEXT_after(device, pNameInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkSetDebugUtilsObjectNameEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SetDebugUtilsObjectTagEXT(VkDevice device, VkDebugUtilsObjectTagInfoEXT* pTagInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkSetDebugUtilsObjectTagEXT!");
}
#ifdef SETDEBUGUTILSOBJECTTAGEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_SetDebugUtilsObjectTagEXT_before(device, pTagInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].SetDebugUtilsObjectTagEXT(device, pTagInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pTagInfo=" + ptrToString((void*)pTagInfo) + '!');
#ifdef SETDEBUGUTILSOBJECTTAGEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_SetDebugUtilsObjectTagEXT_after(device, pTagInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkSetDebugUtilsObjectTagEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_QueueBeginDebugUtilsLabelEXT(VkQueue queue, VkDebugUtilsLabelEXT* pLabelInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkQueueBeginDebugUtilsLabelEXT!");
}
#ifdef QUEUEBEGINDEBUGUTILSLABELEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_QueueBeginDebugUtilsLabelEXT_before(queue, pLabelInfo);
}
#endif 
device_dispatch[GetKey(queue)].QueueBeginDebugUtilsLabelEXT(queue, pLabelInfo);
winsockSendToUI(&ConnectSocket,"queue=" + ptrToString((void*)queue) + '!');
winsockSendToUI(&ConnectSocket,"pLabelInfo=" + ptrToString((void*)pLabelInfo) + '!');
#ifdef QUEUEBEGINDEBUGUTILSLABELEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_QueueBeginDebugUtilsLabelEXT_after(queue, pLabelInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkQueueBeginDebugUtilsLabelEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_QueueEndDebugUtilsLabelEXT(VkQueue queue) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkQueueEndDebugUtilsLabelEXT!");
}
#ifdef QUEUEENDDEBUGUTILSLABELEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_QueueEndDebugUtilsLabelEXT_before(queue);
}
#endif 
device_dispatch[GetKey(queue)].QueueEndDebugUtilsLabelEXT(queue);
winsockSendToUI(&ConnectSocket,"queue=" + ptrToString((void*)queue) + '!');
#ifdef QUEUEENDDEBUGUTILSLABELEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_QueueEndDebugUtilsLabelEXT_after(queue);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkQueueEndDebugUtilsLabelEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_QueueInsertDebugUtilsLabelEXT(VkQueue queue, VkDebugUtilsLabelEXT* pLabelInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkQueueInsertDebugUtilsLabelEXT!");
}
#ifdef QUEUEINSERTDEBUGUTILSLABELEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_QueueInsertDebugUtilsLabelEXT_before(queue, pLabelInfo);
}
#endif 
device_dispatch[GetKey(queue)].QueueInsertDebugUtilsLabelEXT(queue, pLabelInfo);
winsockSendToUI(&ConnectSocket,"queue=" + ptrToString((void*)queue) + '!');
winsockSendToUI(&ConnectSocket,"pLabelInfo=" + ptrToString((void*)pLabelInfo) + '!');
#ifdef QUEUEINSERTDEBUGUTILSLABELEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_QueueInsertDebugUtilsLabelEXT_after(queue, pLabelInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkQueueInsertDebugUtilsLabelEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, VkDebugUtilsLabelEXT* pLabelInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBeginDebugUtilsLabelEXT!");
}
#ifdef CMDBEGINDEBUGUTILSLABELEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBeginDebugUtilsLabelEXT_before(commandBuffer, pLabelInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pLabelInfo=" + ptrToString((void*)pLabelInfo) + '!');
#ifdef CMDBEGINDEBUGUTILSLABELEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBeginDebugUtilsLabelEXT_after(commandBuffer, pLabelInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBeginDebugUtilsLabelEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdEndDebugUtilsLabelEXT!");
}
#ifdef CMDENDDEBUGUTILSLABELEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdEndDebugUtilsLabelEXT_before(commandBuffer);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndDebugUtilsLabelEXT(commandBuffer);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
#ifdef CMDENDDEBUGUTILSLABELEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdEndDebugUtilsLabelEXT_after(commandBuffer);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdEndDebugUtilsLabelEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, VkDebugUtilsLabelEXT* pLabelInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdInsertDebugUtilsLabelEXT!");
}
#ifdef CMDINSERTDEBUGUTILSLABELEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdInsertDebugUtilsLabelEXT_before(commandBuffer, pLabelInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pLabelInfo=" + ptrToString((void*)pLabelInfo) + '!');
#ifdef CMDINSERTDEBUGUTILSLABELEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdInsertDebugUtilsLabelEXT_after(commandBuffer, pLabelInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdInsertDebugUtilsLabelEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetMemoryHostPointerPropertiesEXT!");
}
#ifdef GETMEMORYHOSTPOINTERPROPERTIESEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetMemoryHostPointerPropertiesEXT_before(device, handleType, pHostPointer, pMemoryHostPointerProperties);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetMemoryHostPointerPropertiesEXT(device, handleType, pHostPointer, pMemoryHostPointerProperties);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"handleType=" + std::to_string(handleType) + '!');
winsockSendToUI(&ConnectSocket,"pHostPointer=" + ptrToString((void*)pHostPointer) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryHostPointerProperties=" + ptrToString((void*)pMemoryHostPointerProperties) + '!');
#ifdef GETMEMORYHOSTPOINTERPROPERTIESEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetMemoryHostPointerPropertiesEXT_after(device, handleType, pHostPointer, pMemoryHostPointerProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetMemoryHostPointerPropertiesEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdWriteBufferMarkerAMD!");
}
#ifdef CMDWRITEBUFFERMARKERAMD_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdWriteBufferMarkerAMD_before(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pipelineStage=" + std::to_string(pipelineStage) + '!');
winsockSendToUI(&ConnectSocket,"dstBuffer=" + ptrToString((void*)dstBuffer) + '!');
winsockSendToUI(&ConnectSocket,"dstOffset=" + ptrToString((void*)dstOffset) + '!');
winsockSendToUI(&ConnectSocket,"marker=" + std::to_string(marker) + '!');
#ifdef CMDWRITEBUFFERMARKERAMD_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdWriteBufferMarkerAMD_after(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdWriteBufferMarkerAMD!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateRenderPass2(VkDevice device, VkRenderPassCreateInfo2* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateRenderPass2!");
}
#ifdef CREATERENDERPASS2_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateRenderPass2_before(device, pCreateInfo, pAllocator, pRenderPass);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateRenderPass2(device, pCreateInfo, pAllocator, pRenderPass);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pRenderPass=" + ptrToString((void*)pRenderPass) + '!');
#ifdef CREATERENDERPASS2_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateRenderPass2_after(device, pCreateInfo, pAllocator, pRenderPass);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateRenderPass2!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassBeginInfo* pSubpassBeginInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBeginRenderPass2!");
}
#ifdef CMDBEGINRENDERPASS2_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBeginRenderPass2_before(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginRenderPass2(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pRenderPassBegin=" + ptrToString((void*)pRenderPassBegin) + '!');
winsockSendToUI(&ConnectSocket,"pSubpassBeginInfo=" + ptrToString((void*)pSubpassBeginInfo) + '!');
#ifdef CMDBEGINRENDERPASS2_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBeginRenderPass2_after(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBeginRenderPass2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdNextSubpass2(VkCommandBuffer commandBuffer, VkSubpassBeginInfo* pSubpassBeginInfo, VkSubpassEndInfo* pSubpassEndInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdNextSubpass2!");
}
#ifdef CMDNEXTSUBPASS2_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdNextSubpass2_before(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdNextSubpass2(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pSubpassBeginInfo=" + ptrToString((void*)pSubpassBeginInfo) + '!');
winsockSendToUI(&ConnectSocket,"pSubpassEndInfo=" + ptrToString((void*)pSubpassEndInfo) + '!');
#ifdef CMDNEXTSUBPASS2_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdNextSubpass2_after(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdNextSubpass2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndRenderPass2(VkCommandBuffer commandBuffer, VkSubpassEndInfo* pSubpassEndInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdEndRenderPass2!");
}
#ifdef CMDENDRENDERPASS2_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdEndRenderPass2_before(commandBuffer, pSubpassEndInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndRenderPass2(commandBuffer, pSubpassEndInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pSubpassEndInfo=" + ptrToString((void*)pSubpassEndInfo) + '!');
#ifdef CMDENDRENDERPASS2_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdEndRenderPass2_after(commandBuffer, pSubpassEndInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdEndRenderPass2!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetSemaphoreCounterValue!");
}
#ifdef GETSEMAPHORECOUNTERVALUE_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetSemaphoreCounterValue_before(device, semaphore, pValue);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetSemaphoreCounterValue(device, semaphore, pValue);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"semaphore=" + ptrToString((void*)semaphore) + '!');
winsockSendToUI(&ConnectSocket,"pValue=" + ptrToString((void*)pValue) + '!');
#ifdef GETSEMAPHORECOUNTERVALUE_AFTER_EXEC_EXISTS
if(connected) {
layer_GetSemaphoreCounterValue_after(device, semaphore, pValue);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetSemaphoreCounterValue!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_WaitSemaphores(VkDevice device, VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkWaitSemaphores!");
}
#ifdef WAITSEMAPHORES_BEFORE_EXEC_EXISTS
if(connected) {
layer_WaitSemaphores_before(device, pWaitInfo, timeout);
}
#endif 
auto ret = device_dispatch[GetKey(device)].WaitSemaphores(device, pWaitInfo, timeout);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pWaitInfo=" + ptrToString((void*)pWaitInfo) + '!');
winsockSendToUI(&ConnectSocket,"timeout=" + std::to_string(timeout) + '!');
#ifdef WAITSEMAPHORES_AFTER_EXEC_EXISTS
if(connected) {
layer_WaitSemaphores_after(device, pWaitInfo, timeout);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkWaitSemaphores!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SignalSemaphore(VkDevice device, VkSemaphoreSignalInfo* pSignalInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkSignalSemaphore!");
}
#ifdef SIGNALSEMAPHORE_BEFORE_EXEC_EXISTS
if(connected) {
layer_SignalSemaphore_before(device, pSignalInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].SignalSemaphore(device, pSignalInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pSignalInfo=" + ptrToString((void*)pSignalInfo) + '!');
#ifdef SIGNALSEMAPHORE_AFTER_EXEC_EXISTS
if(connected) {
layer_SignalSemaphore_after(device, pSignalInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkSignalSemaphore!");
}
return ret;
}

#if defined(VK_USE_PLATFORM_ANDROID_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetAndroidHardwareBufferPropertiesANDROID!");
}
#ifdef GETANDROIDHARDWAREBUFFERPROPERTIESANDROID_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetAndroidHardwareBufferPropertiesANDROID_before(device, buffer, pProperties);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetAndroidHardwareBufferPropertiesANDROID(device, buffer, pProperties);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETANDROIDHARDWAREBUFFERPROPERTIESANDROID_AFTER_EXEC_EXISTS
if(connected) {
layer_GetAndroidHardwareBufferPropertiesANDROID_after(device, buffer, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetAndroidHardwareBufferPropertiesANDROID!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, AHardwareBuffer* pBuffer) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetMemoryAndroidHardwareBufferANDROID!");
}
#ifdef GETMEMORYANDROIDHARDWAREBUFFERANDROID_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetMemoryAndroidHardwareBufferANDROID_before(device, pInfo, pBuffer);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetMemoryAndroidHardwareBufferANDROID(device, pInfo, pBuffer);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pBuffer=" + ptrToString((void*)pBuffer) + '!');
#ifdef GETMEMORYANDROIDHARDWAREBUFFERANDROID_AFTER_EXEC_EXISTS
if(connected) {
layer_GetMemoryAndroidHardwareBufferANDROID_after(device, pInfo, pBuffer);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetMemoryAndroidHardwareBufferANDROID!");
}
return ret;
}

#endif
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawIndirectCount!");
}
#ifdef CMDDRAWINDIRECTCOUNT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndirectCount_before(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
winsockSendToUI(&ConnectSocket,"countBuffer=" + ptrToString((void*)countBuffer) + '!');
winsockSendToUI(&ConnectSocket,"countBufferOffset=" + ptrToString((void*)countBufferOffset) + '!');
winsockSendToUI(&ConnectSocket,"maxDrawCount=" + std::to_string(maxDrawCount) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef CMDDRAWINDIRECTCOUNT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndirectCount_after(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawIndirectCount!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawIndexedIndirectCount!");
}
#ifdef CMDDRAWINDEXEDINDIRECTCOUNT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndexedIndirectCount_before(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndexedIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
winsockSendToUI(&ConnectSocket,"countBuffer=" + ptrToString((void*)countBuffer) + '!');
winsockSendToUI(&ConnectSocket,"countBufferOffset=" + ptrToString((void*)countBufferOffset) + '!');
winsockSendToUI(&ConnectSocket,"maxDrawCount=" + std::to_string(maxDrawCount) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef CMDDRAWINDEXEDINDIRECTCOUNT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndexedIndirectCount_after(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawIndexedIndirectCount!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCheckpointNV(VkCommandBuffer commandBuffer, void* pCheckpointMarker) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetCheckpointNV!");
}
#ifdef CMDSETCHECKPOINTNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetCheckpointNV_before(commandBuffer, pCheckpointMarker);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCheckpointNV(commandBuffer, pCheckpointMarker);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pCheckpointMarker=" + ptrToString((void*)pCheckpointMarker) + '!');
#ifdef CMDSETCHECKPOINTNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetCheckpointNV_after(commandBuffer, pCheckpointMarker);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetCheckpointNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetQueueCheckpointDataNV!");
}
#ifdef GETQUEUECHECKPOINTDATANV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetQueueCheckpointDataNV_before(queue, pCheckpointDataCount, pCheckpointData);
}
#endif 
device_dispatch[GetKey(queue)].GetQueueCheckpointDataNV(queue, pCheckpointDataCount, pCheckpointData);
winsockSendToUI(&ConnectSocket,"queue=" + ptrToString((void*)queue) + '!');
winsockSendToUI(&ConnectSocket,"pCheckpointDataCount=" + ptrToString((void*)pCheckpointDataCount) + '!');
winsockSendToUI(&ConnectSocket,"pCheckpointData=" + ptrToString((void*)pCheckpointData) + '!');
#ifdef GETQUEUECHECKPOINTDATANV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetQueueCheckpointDataNV_after(queue, pCheckpointDataCount, pCheckpointData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetQueueCheckpointDataNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets, VkDeviceSize* pSizes) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBindTransformFeedbackBuffersEXT!");
}
#ifdef CMDBINDTRANSFORMFEEDBACKBUFFERSEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBindTransformFeedbackBuffersEXT_before(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindTransformFeedbackBuffersEXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstBinding=" + std::to_string(firstBinding) + '!');
winsockSendToUI(&ConnectSocket,"bindingCount=" + std::to_string(bindingCount) + '!');
winsockSendToUI(&ConnectSocket,"pBuffers=" + ptrToString((void*)pBuffers) + '!');
winsockSendToUI(&ConnectSocket,"pOffsets=" + ptrToString((void*)pOffsets) + '!');
winsockSendToUI(&ConnectSocket,"pSizes=" + ptrToString((void*)pSizes) + '!');
#ifdef CMDBINDTRANSFORMFEEDBACKBUFFERSEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBindTransformFeedbackBuffersEXT_after(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBindTransformFeedbackBuffersEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, VkBuffer* pCounterBuffers, VkDeviceSize* pCounterBufferOffsets) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBeginTransformFeedbackEXT!");
}
#ifdef CMDBEGINTRANSFORMFEEDBACKEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBeginTransformFeedbackEXT_before(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstCounterBuffer=" + std::to_string(firstCounterBuffer) + '!');
winsockSendToUI(&ConnectSocket,"counterBufferCount=" + std::to_string(counterBufferCount) + '!');
winsockSendToUI(&ConnectSocket,"pCounterBuffers=" + ptrToString((void*)pCounterBuffers) + '!');
winsockSendToUI(&ConnectSocket,"pCounterBufferOffsets=" + ptrToString((void*)pCounterBufferOffsets) + '!');
#ifdef CMDBEGINTRANSFORMFEEDBACKEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBeginTransformFeedbackEXT_after(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBeginTransformFeedbackEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, VkBuffer* pCounterBuffers, VkDeviceSize* pCounterBufferOffsets) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdEndTransformFeedbackEXT!");
}
#ifdef CMDENDTRANSFORMFEEDBACKEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdEndTransformFeedbackEXT_before(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstCounterBuffer=" + std::to_string(firstCounterBuffer) + '!');
winsockSendToUI(&ConnectSocket,"counterBufferCount=" + std::to_string(counterBufferCount) + '!');
winsockSendToUI(&ConnectSocket,"pCounterBuffers=" + ptrToString((void*)pCounterBuffers) + '!');
winsockSendToUI(&ConnectSocket,"pCounterBufferOffsets=" + ptrToString((void*)pCounterBufferOffsets) + '!');
#ifdef CMDENDTRANSFORMFEEDBACKEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdEndTransformFeedbackEXT_after(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdEndTransformFeedbackEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBeginQueryIndexedEXT!");
}
#ifdef CMDBEGINQUERYINDEXEDEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBeginQueryIndexedEXT_before(commandBuffer, queryPool, query, flags, index);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginQueryIndexedEXT(commandBuffer, queryPool, query, flags, index);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"queryPool=" + ptrToString((void*)queryPool) + '!');
winsockSendToUI(&ConnectSocket,"query=" + std::to_string(query) + '!');
winsockSendToUI(&ConnectSocket,"flags=" + ptrToString((void*)flags) + '!');
winsockSendToUI(&ConnectSocket,"index=" + std::to_string(index) + '!');
#ifdef CMDBEGINQUERYINDEXEDEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBeginQueryIndexedEXT_after(commandBuffer, queryPool, query, flags, index);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBeginQueryIndexedEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdEndQueryIndexedEXT!");
}
#ifdef CMDENDQUERYINDEXEDEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdEndQueryIndexedEXT_before(commandBuffer, queryPool, query, index);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndQueryIndexedEXT(commandBuffer, queryPool, query, index);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"queryPool=" + ptrToString((void*)queryPool) + '!');
winsockSendToUI(&ConnectSocket,"query=" + std::to_string(query) + '!');
winsockSendToUI(&ConnectSocket,"index=" + std::to_string(index) + '!');
#ifdef CMDENDQUERYINDEXEDEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdEndQueryIndexedEXT_after(commandBuffer, queryPool, query, index);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdEndQueryIndexedEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawIndirectByteCountEXT!");
}
#ifdef CMDDRAWINDIRECTBYTECOUNTEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndirectByteCountEXT_before(commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndirectByteCountEXT(commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"instanceCount=" + std::to_string(instanceCount) + '!');
winsockSendToUI(&ConnectSocket,"firstInstance=" + std::to_string(firstInstance) + '!');
winsockSendToUI(&ConnectSocket,"counterBuffer=" + ptrToString((void*)counterBuffer) + '!');
winsockSendToUI(&ConnectSocket,"counterBufferOffset=" + ptrToString((void*)counterBufferOffset) + '!');
winsockSendToUI(&ConnectSocket,"counterOffset=" + std::to_string(counterOffset) + '!');
winsockSendToUI(&ConnectSocket,"vertexStride=" + std::to_string(vertexStride) + '!');
#ifdef CMDDRAWINDIRECTBYTECOUNTEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndirectByteCountEXT_after(commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawIndirectByteCountEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, VkRect2D* pExclusiveScissors) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetExclusiveScissorNV!");
}
#ifdef CMDSETEXCLUSIVESCISSORNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetExclusiveScissorNV_before(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetExclusiveScissorNV(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstExclusiveScissor=" + std::to_string(firstExclusiveScissor) + '!');
winsockSendToUI(&ConnectSocket,"exclusiveScissorCount=" + std::to_string(exclusiveScissorCount) + '!');
winsockSendToUI(&ConnectSocket,"pExclusiveScissors=" + ptrToString((void*)pExclusiveScissors) + '!');
#ifdef CMDSETEXCLUSIVESCISSORNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetExclusiveScissorNV_after(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetExclusiveScissorNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, VkBool32* pExclusiveScissorEnables) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetExclusiveScissorEnableNV!");
}
#ifdef CMDSETEXCLUSIVESCISSORENABLENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetExclusiveScissorEnableNV_before(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissorEnables);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetExclusiveScissorEnableNV(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissorEnables);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstExclusiveScissor=" + std::to_string(firstExclusiveScissor) + '!');
winsockSendToUI(&ConnectSocket,"exclusiveScissorCount=" + std::to_string(exclusiveScissorCount) + '!');
winsockSendToUI(&ConnectSocket,"pExclusiveScissorEnables=" + ptrToString((void*)pExclusiveScissorEnables) + '!');
#ifdef CMDSETEXCLUSIVESCISSORENABLENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetExclusiveScissorEnableNV_after(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissorEnables);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetExclusiveScissorEnableNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBindShadingRateImageNV!");
}
#ifdef CMDBINDSHADINGRATEIMAGENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBindShadingRateImageNV_before(commandBuffer, imageView, imageLayout);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindShadingRateImageNV(commandBuffer, imageView, imageLayout);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"imageView=" + ptrToString((void*)imageView) + '!');
winsockSendToUI(&ConnectSocket,"imageLayout=" + std::to_string(imageLayout) + '!');
#ifdef CMDBINDSHADINGRATEIMAGENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBindShadingRateImageNV_after(commandBuffer, imageView, imageLayout);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBindShadingRateImageNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkShadingRatePaletteNV* pShadingRatePalettes) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetViewportShadingRatePaletteNV!");
}
#ifdef CMDSETVIEWPORTSHADINGRATEPALETTENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetViewportShadingRatePaletteNV_before(commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetViewportShadingRatePaletteNV(commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstViewport=" + std::to_string(firstViewport) + '!');
winsockSendToUI(&ConnectSocket,"viewportCount=" + std::to_string(viewportCount) + '!');
winsockSendToUI(&ConnectSocket,"pShadingRatePalettes=" + ptrToString((void*)pShadingRatePalettes) + '!');
#ifdef CMDSETVIEWPORTSHADINGRATEPALETTENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetViewportShadingRatePaletteNV_after(commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetViewportShadingRatePaletteNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, VkCoarseSampleOrderCustomNV* pCustomSampleOrders) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetCoarseSampleOrderNV!");
}
#ifdef CMDSETCOARSESAMPLEORDERNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetCoarseSampleOrderNV_before(commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCoarseSampleOrderNV(commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"sampleOrderType=" + std::to_string(sampleOrderType) + '!');
winsockSendToUI(&ConnectSocket,"customSampleOrderCount=" + std::to_string(customSampleOrderCount) + '!');
winsockSendToUI(&ConnectSocket,"pCustomSampleOrders=" + ptrToString((void*)pCustomSampleOrders) + '!');
#ifdef CMDSETCOARSESAMPLEORDERNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetCoarseSampleOrderNV_after(commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetCoarseSampleOrderNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawMeshTasksNV!");
}
#ifdef CMDDRAWMESHTASKSNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawMeshTasksNV_before(commandBuffer, taskCount, firstTask);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawMeshTasksNV(commandBuffer, taskCount, firstTask);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"taskCount=" + std::to_string(taskCount) + '!');
winsockSendToUI(&ConnectSocket,"firstTask=" + std::to_string(firstTask) + '!');
#ifdef CMDDRAWMESHTASKSNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawMeshTasksNV_after(commandBuffer, taskCount, firstTask);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawMeshTasksNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawMeshTasksIndirectNV!");
}
#ifdef CMDDRAWMESHTASKSINDIRECTNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawMeshTasksIndirectNV_before(commandBuffer, buffer, offset, drawCount, stride);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawMeshTasksIndirectNV(commandBuffer, buffer, offset, drawCount, stride);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
winsockSendToUI(&ConnectSocket,"drawCount=" + std::to_string(drawCount) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef CMDDRAWMESHTASKSINDIRECTNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawMeshTasksIndirectNV_after(commandBuffer, buffer, offset, drawCount, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawMeshTasksIndirectNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawMeshTasksIndirectCountNV!");
}
#ifdef CMDDRAWMESHTASKSINDIRECTCOUNTNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawMeshTasksIndirectCountNV_before(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawMeshTasksIndirectCountNV(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
winsockSendToUI(&ConnectSocket,"countBuffer=" + ptrToString((void*)countBuffer) + '!');
winsockSendToUI(&ConnectSocket,"countBufferOffset=" + ptrToString((void*)countBufferOffset) + '!');
winsockSendToUI(&ConnectSocket,"maxDrawCount=" + std::to_string(maxDrawCount) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef CMDDRAWMESHTASKSINDIRECTCOUNTNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawMeshTasksIndirectCountNV_after(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawMeshTasksIndirectCountNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawMeshTasksEXT!");
}
#ifdef CMDDRAWMESHTASKSEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawMeshTasksEXT_before(commandBuffer, groupCountX, groupCountY, groupCountZ);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawMeshTasksEXT(commandBuffer, groupCountX, groupCountY, groupCountZ);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"groupCountX=" + std::to_string(groupCountX) + '!');
winsockSendToUI(&ConnectSocket,"groupCountY=" + std::to_string(groupCountY) + '!');
winsockSendToUI(&ConnectSocket,"groupCountZ=" + std::to_string(groupCountZ) + '!');
#ifdef CMDDRAWMESHTASKSEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawMeshTasksEXT_after(commandBuffer, groupCountX, groupCountY, groupCountZ);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawMeshTasksEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawMeshTasksIndirectEXT!");
}
#ifdef CMDDRAWMESHTASKSINDIRECTEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawMeshTasksIndirectEXT_before(commandBuffer, buffer, offset, drawCount, stride);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawMeshTasksIndirectEXT(commandBuffer, buffer, offset, drawCount, stride);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
winsockSendToUI(&ConnectSocket,"drawCount=" + std::to_string(drawCount) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef CMDDRAWMESHTASKSINDIRECTEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawMeshTasksIndirectEXT_after(commandBuffer, buffer, offset, drawCount, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawMeshTasksIndirectEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawMeshTasksIndirectCountEXT!");
}
#ifdef CMDDRAWMESHTASKSINDIRECTCOUNTEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawMeshTasksIndirectCountEXT_before(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawMeshTasksIndirectCountEXT(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
winsockSendToUI(&ConnectSocket,"countBuffer=" + ptrToString((void*)countBuffer) + '!');
winsockSendToUI(&ConnectSocket,"countBufferOffset=" + ptrToString((void*)countBufferOffset) + '!');
winsockSendToUI(&ConnectSocket,"maxDrawCount=" + std::to_string(maxDrawCount) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef CMDDRAWMESHTASKSINDIRECTCOUNTEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawMeshTasksIndirectCountEXT_after(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawMeshTasksIndirectCountEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCompileDeferredNV!");
}
#ifdef COMPILEDEFERREDNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CompileDeferredNV_before(device, pipeline, shader);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CompileDeferredNV(device, pipeline, shader);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pipeline=" + ptrToString((void*)pipeline) + '!');
winsockSendToUI(&ConnectSocket,"shader=" + std::to_string(shader) + '!');
#ifdef COMPILEDEFERREDNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CompileDeferredNV_after(device, pipeline, shader);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCompileDeferredNV!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateAccelerationStructureNV(VkDevice device, VkAccelerationStructureCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateAccelerationStructureNV!");
}
#ifdef CREATEACCELERATIONSTRUCTURENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateAccelerationStructureNV_before(device, pCreateInfo, pAllocator, pAccelerationStructure);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateAccelerationStructureNV(device, pCreateInfo, pAllocator, pAccelerationStructure);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pAccelerationStructure=" + ptrToString((void*)pAccelerationStructure) + '!');
#ifdef CREATEACCELERATIONSTRUCTURENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateAccelerationStructureNV_after(device, pCreateInfo, pAllocator, pAccelerationStructure);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateAccelerationStructureNV!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBindInvocationMaskHUAWEI!");
}
#ifdef CMDBINDINVOCATIONMASKHUAWEI_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBindInvocationMaskHUAWEI_before(commandBuffer, imageView, imageLayout);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindInvocationMaskHUAWEI(commandBuffer, imageView, imageLayout);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"imageView=" + ptrToString((void*)imageView) + '!');
winsockSendToUI(&ConnectSocket,"imageLayout=" + std::to_string(imageLayout) + '!');
#ifdef CMDBINDINVOCATIONMASKHUAWEI_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBindInvocationMaskHUAWEI_after(commandBuffer, imageView, imageLayout);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBindInvocationMaskHUAWEI!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyAccelerationStructureKHR!");
}
#ifdef DESTROYACCELERATIONSTRUCTUREKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyAccelerationStructureKHR_before(device, accelerationStructure, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyAccelerationStructureKHR(device, accelerationStructure, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"accelerationStructure=" + ptrToString((void*)accelerationStructure) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYACCELERATIONSTRUCTUREKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyAccelerationStructureKHR_after(device, accelerationStructure, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyAccelerationStructureKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyAccelerationStructureNV!");
}
#ifdef DESTROYACCELERATIONSTRUCTURENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyAccelerationStructureNV_before(device, accelerationStructure, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyAccelerationStructureNV(device, accelerationStructure, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"accelerationStructure=" + ptrToString((void*)accelerationStructure) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYACCELERATIONSTRUCTURENV_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyAccelerationStructureNV_after(device, accelerationStructure, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyAccelerationStructureNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetAccelerationStructureMemoryRequirementsNV(VkDevice device, VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetAccelerationStructureMemoryRequirementsNV!");
}
#ifdef GETACCELERATIONSTRUCTUREMEMORYREQUIREMENTSNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetAccelerationStructureMemoryRequirementsNV_before(device, pInfo, pMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetAccelerationStructureMemoryRequirementsNV(device, pInfo, pMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRequirements=" + ptrToString((void*)pMemoryRequirements) + '!');
#ifdef GETACCELERATIONSTRUCTUREMEMORYREQUIREMENTSNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetAccelerationStructureMemoryRequirementsNV_after(device, pInfo, pMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetAccelerationStructureMemoryRequirementsNV!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, VkBindAccelerationStructureMemoryInfoNV* pBindInfos) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkBindAccelerationStructureMemoryNV!");
}
#ifdef BINDACCELERATIONSTRUCTUREMEMORYNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_BindAccelerationStructureMemoryNV_before(device, bindInfoCount, pBindInfos);
}
#endif 
auto ret = device_dispatch[GetKey(device)].BindAccelerationStructureMemoryNV(device, bindInfoCount, pBindInfos);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"bindInfoCount=" + std::to_string(bindInfoCount) + '!');
winsockSendToUI(&ConnectSocket,"pBindInfos=" + ptrToString((void*)pBindInfos) + '!');
#ifdef BINDACCELERATIONSTRUCTUREMEMORYNV_AFTER_EXEC_EXISTS
if(connected) {
layer_BindAccelerationStructureMemoryNV_after(device, bindInfoCount, pBindInfos);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkBindAccelerationStructureMemoryNV!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyAccelerationStructureNV!");
}
#ifdef CMDCOPYACCELERATIONSTRUCTURENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyAccelerationStructureNV_before(commandBuffer, dst, src, mode);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyAccelerationStructureNV(commandBuffer, dst, src, mode);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"dst=" + ptrToString((void*)dst) + '!');
winsockSendToUI(&ConnectSocket,"src=" + ptrToString((void*)src) + '!');
winsockSendToUI(&ConnectSocket,"mode=" + std::to_string(mode) + '!');
#ifdef CMDCOPYACCELERATIONSTRUCTURENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyAccelerationStructureNV_after(commandBuffer, dst, src, mode);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyAccelerationStructureNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, VkCopyAccelerationStructureInfoKHR* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyAccelerationStructureKHR!");
}
#ifdef CMDCOPYACCELERATIONSTRUCTUREKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyAccelerationStructureKHR_before(commandBuffer, pInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyAccelerationStructureKHR(commandBuffer, pInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef CMDCOPYACCELERATIONSTRUCTUREKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyAccelerationStructureKHR_after(commandBuffer, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyAccelerationStructureKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyAccelerationStructureInfoKHR* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCopyAccelerationStructureKHR!");
}
#ifdef COPYACCELERATIONSTRUCTUREKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CopyAccelerationStructureKHR_before(device, deferredOperation, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CopyAccelerationStructureKHR(device, deferredOperation, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"deferredOperation=" + ptrToString((void*)deferredOperation) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef COPYACCELERATIONSTRUCTUREKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CopyAccelerationStructureKHR_after(device, deferredOperation, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCopyAccelerationStructureKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyAccelerationStructureToMemoryKHR!");
}
#ifdef CMDCOPYACCELERATIONSTRUCTURETOMEMORYKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyAccelerationStructureToMemoryKHR_before(commandBuffer, pInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyAccelerationStructureToMemoryKHR(commandBuffer, pInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef CMDCOPYACCELERATIONSTRUCTURETOMEMORYKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyAccelerationStructureToMemoryKHR_after(commandBuffer, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyAccelerationStructureToMemoryKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCopyAccelerationStructureToMemoryKHR!");
}
#ifdef COPYACCELERATIONSTRUCTURETOMEMORYKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CopyAccelerationStructureToMemoryKHR_before(device, deferredOperation, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CopyAccelerationStructureToMemoryKHR(device, deferredOperation, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"deferredOperation=" + ptrToString((void*)deferredOperation) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef COPYACCELERATIONSTRUCTURETOMEMORYKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CopyAccelerationStructureToMemoryKHR_after(device, deferredOperation, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCopyAccelerationStructureToMemoryKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyMemoryToAccelerationStructureKHR!");
}
#ifdef CMDCOPYMEMORYTOACCELERATIONSTRUCTUREKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyMemoryToAccelerationStructureKHR_before(commandBuffer, pInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyMemoryToAccelerationStructureKHR(commandBuffer, pInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef CMDCOPYMEMORYTOACCELERATIONSTRUCTUREKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyMemoryToAccelerationStructureKHR_after(commandBuffer, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyMemoryToAccelerationStructureKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCopyMemoryToAccelerationStructureKHR!");
}
#ifdef COPYMEMORYTOACCELERATIONSTRUCTUREKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CopyMemoryToAccelerationStructureKHR_before(device, deferredOperation, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CopyMemoryToAccelerationStructureKHR(device, deferredOperation, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"deferredOperation=" + ptrToString((void*)deferredOperation) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef COPYMEMORYTOACCELERATIONSTRUCTUREKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CopyMemoryToAccelerationStructureKHR_after(device, deferredOperation, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCopyMemoryToAccelerationStructureKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdWriteAccelerationStructuresPropertiesKHR!");
}
#ifdef CMDWRITEACCELERATIONSTRUCTURESPROPERTIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdWriteAccelerationStructuresPropertiesKHR_before(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWriteAccelerationStructuresPropertiesKHR(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"accelerationStructureCount=" + std::to_string(accelerationStructureCount) + '!');
winsockSendToUI(&ConnectSocket,"pAccelerationStructures=" + ptrToString((void*)pAccelerationStructures) + '!');
winsockSendToUI(&ConnectSocket,"queryType=" + std::to_string(queryType) + '!');
winsockSendToUI(&ConnectSocket,"queryPool=" + ptrToString((void*)queryPool) + '!');
winsockSendToUI(&ConnectSocket,"firstQuery=" + std::to_string(firstQuery) + '!');
#ifdef CMDWRITEACCELERATIONSTRUCTURESPROPERTIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdWriteAccelerationStructuresPropertiesKHR_after(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdWriteAccelerationStructuresPropertiesKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdWriteAccelerationStructuresPropertiesNV!");
}
#ifdef CMDWRITEACCELERATIONSTRUCTURESPROPERTIESNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdWriteAccelerationStructuresPropertiesNV_before(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWriteAccelerationStructuresPropertiesNV(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"accelerationStructureCount=" + std::to_string(accelerationStructureCount) + '!');
winsockSendToUI(&ConnectSocket,"pAccelerationStructures=" + ptrToString((void*)pAccelerationStructures) + '!');
winsockSendToUI(&ConnectSocket,"queryType=" + std::to_string(queryType) + '!');
winsockSendToUI(&ConnectSocket,"queryPool=" + ptrToString((void*)queryPool) + '!');
winsockSendToUI(&ConnectSocket,"firstQuery=" + std::to_string(firstQuery) + '!');
#ifdef CMDWRITEACCELERATIONSTRUCTURESPROPERTIESNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdWriteAccelerationStructuresPropertiesNV_after(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdWriteAccelerationStructuresPropertiesNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBuildAccelerationStructureNV!");
}
#ifdef CMDBUILDACCELERATIONSTRUCTURENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBuildAccelerationStructureNV_before(commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBuildAccelerationStructureNV(commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"instanceData=" + ptrToString((void*)instanceData) + '!');
winsockSendToUI(&ConnectSocket,"instanceOffset=" + ptrToString((void*)instanceOffset) + '!');
winsockSendToUI(&ConnectSocket,"update=" + bool_as_text(update) + '!');
winsockSendToUI(&ConnectSocket,"dst=" + ptrToString((void*)dst) + '!');
winsockSendToUI(&ConnectSocket,"src=" + ptrToString((void*)src) + '!');
winsockSendToUI(&ConnectSocket,"scratch=" + ptrToString((void*)scratch) + '!');
winsockSendToUI(&ConnectSocket,"scratchOffset=" + ptrToString((void*)scratchOffset) + '!');
#ifdef CMDBUILDACCELERATIONSTRUCTURENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBuildAccelerationStructureNV_after(commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBuildAccelerationStructureNV!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_WriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, size_t dataSize, void* pData, size_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkWriteAccelerationStructuresPropertiesKHR!");
}
#ifdef WRITEACCELERATIONSTRUCTURESPROPERTIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_WriteAccelerationStructuresPropertiesKHR_before(device, accelerationStructureCount, pAccelerationStructures, queryType, dataSize, pData, stride);
}
#endif 
auto ret = device_dispatch[GetKey(device)].WriteAccelerationStructuresPropertiesKHR(device, accelerationStructureCount, pAccelerationStructures, queryType, dataSize, pData, stride);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"accelerationStructureCount=" + std::to_string(accelerationStructureCount) + '!');
winsockSendToUI(&ConnectSocket,"pAccelerationStructures=" + ptrToString((void*)pAccelerationStructures) + '!');
winsockSendToUI(&ConnectSocket,"queryType=" + std::to_string(queryType) + '!');
winsockSendToUI(&ConnectSocket,"dataSize=" + std::to_string(dataSize) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef WRITEACCELERATIONSTRUCTURESPROPERTIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_WriteAccelerationStructuresPropertiesKHR_after(device, accelerationStructureCount, pAccelerationStructures, queryType, dataSize, pData, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkWriteAccelerationStructuresPropertiesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdTraceRaysKHR(VkCommandBuffer commandBuffer, VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdTraceRaysKHR!");
}
#ifdef CMDTRACERAYSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdTraceRaysKHR_before(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdTraceRaysKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pRaygenShaderBindingTable=" + ptrToString((void*)pRaygenShaderBindingTable) + '!');
winsockSendToUI(&ConnectSocket,"pMissShaderBindingTable=" + ptrToString((void*)pMissShaderBindingTable) + '!');
winsockSendToUI(&ConnectSocket,"pHitShaderBindingTable=" + ptrToString((void*)pHitShaderBindingTable) + '!');
winsockSendToUI(&ConnectSocket,"pCallableShaderBindingTable=" + ptrToString((void*)pCallableShaderBindingTable) + '!');
winsockSendToUI(&ConnectSocket,"width=" + std::to_string(width) + '!');
winsockSendToUI(&ConnectSocket,"height=" + std::to_string(height) + '!');
winsockSendToUI(&ConnectSocket,"depth=" + std::to_string(depth) + '!');
#ifdef CMDTRACERAYSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdTraceRaysKHR_after(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdTraceRaysKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdTraceRaysNV!");
}
#ifdef CMDTRACERAYSNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdTraceRaysNV_before(commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdTraceRaysNV(commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"raygenShaderBindingTableBuffer=" + ptrToString((void*)raygenShaderBindingTableBuffer) + '!');
winsockSendToUI(&ConnectSocket,"raygenShaderBindingOffset=" + ptrToString((void*)raygenShaderBindingOffset) + '!');
winsockSendToUI(&ConnectSocket,"missShaderBindingTableBuffer=" + ptrToString((void*)missShaderBindingTableBuffer) + '!');
winsockSendToUI(&ConnectSocket,"missShaderBindingOffset=" + ptrToString((void*)missShaderBindingOffset) + '!');
winsockSendToUI(&ConnectSocket,"missShaderBindingStride=" + ptrToString((void*)missShaderBindingStride) + '!');
winsockSendToUI(&ConnectSocket,"hitShaderBindingTableBuffer=" + ptrToString((void*)hitShaderBindingTableBuffer) + '!');
winsockSendToUI(&ConnectSocket,"hitShaderBindingOffset=" + ptrToString((void*)hitShaderBindingOffset) + '!');
winsockSendToUI(&ConnectSocket,"hitShaderBindingStride=" + ptrToString((void*)hitShaderBindingStride) + '!');
winsockSendToUI(&ConnectSocket,"callableShaderBindingTableBuffer=" + ptrToString((void*)callableShaderBindingTableBuffer) + '!');
winsockSendToUI(&ConnectSocket,"callableShaderBindingOffset=" + ptrToString((void*)callableShaderBindingOffset) + '!');
winsockSendToUI(&ConnectSocket,"callableShaderBindingStride=" + ptrToString((void*)callableShaderBindingStride) + '!');
winsockSendToUI(&ConnectSocket,"width=" + std::to_string(width) + '!');
winsockSendToUI(&ConnectSocket,"height=" + std::to_string(height) + '!');
winsockSendToUI(&ConnectSocket,"depth=" + std::to_string(depth) + '!');
#ifdef CMDTRACERAYSNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdTraceRaysNV_after(commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdTraceRaysNV!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetRayTracingShaderGroupHandlesKHR!");
}
#ifdef GETRAYTRACINGSHADERGROUPHANDLESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetRayTracingShaderGroupHandlesKHR_before(device, pipeline, firstGroup, groupCount, dataSize, pData);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetRayTracingShaderGroupHandlesKHR(device, pipeline, firstGroup, groupCount, dataSize, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pipeline=" + ptrToString((void*)pipeline) + '!');
winsockSendToUI(&ConnectSocket,"firstGroup=" + std::to_string(firstGroup) + '!');
winsockSendToUI(&ConnectSocket,"groupCount=" + std::to_string(groupCount) + '!');
winsockSendToUI(&ConnectSocket,"dataSize=" + std::to_string(dataSize) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef GETRAYTRACINGSHADERGROUPHANDLESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetRayTracingShaderGroupHandlesKHR_after(device, pipeline, firstGroup, groupCount, dataSize, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetRayTracingShaderGroupHandlesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR!");
}
#ifdef GETRAYTRACINGCAPTUREREPLAYSHADERGROUPHANDLESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetRayTracingCaptureReplayShaderGroupHandlesKHR_before(device, pipeline, firstGroup, groupCount, dataSize, pData);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetRayTracingCaptureReplayShaderGroupHandlesKHR(device, pipeline, firstGroup, groupCount, dataSize, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pipeline=" + ptrToString((void*)pipeline) + '!');
winsockSendToUI(&ConnectSocket,"firstGroup=" + std::to_string(firstGroup) + '!');
winsockSendToUI(&ConnectSocket,"groupCount=" + std::to_string(groupCount) + '!');
winsockSendToUI(&ConnectSocket,"dataSize=" + std::to_string(dataSize) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef GETRAYTRACINGCAPTUREREPLAYSHADERGROUPHANDLESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetRayTracingCaptureReplayShaderGroupHandlesKHR_after(device, pipeline, firstGroup, groupCount, dataSize, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetAccelerationStructureHandleNV!");
}
#ifdef GETACCELERATIONSTRUCTUREHANDLENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetAccelerationStructureHandleNV_before(device, accelerationStructure, dataSize, pData);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetAccelerationStructureHandleNV(device, accelerationStructure, dataSize, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"accelerationStructure=" + ptrToString((void*)accelerationStructure) + '!');
winsockSendToUI(&ConnectSocket,"dataSize=" + std::to_string(dataSize) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef GETACCELERATIONSTRUCTUREHANDLENV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetAccelerationStructureHandleNV_after(device, accelerationStructure, dataSize, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetAccelerationStructureHandleNV!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkRayTracingPipelineCreateInfoNV* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateRayTracingPipelinesNV!");
}
#ifdef CREATERAYTRACINGPIPELINESNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateRayTracingPipelinesNV_before(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateRayTracingPipelinesNV(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pipelineCache=" + ptrToString((void*)pipelineCache) + '!');
winsockSendToUI(&ConnectSocket,"createInfoCount=" + std::to_string(createInfoCount) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfos=" + ptrToString((void*)pCreateInfos) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pPipelines=" + ptrToString((void*)pPipelines) + '!');
#ifdef CREATERAYTRACINGPIPELINESNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateRayTracingPipelinesNV_after(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateRayTracingPipelinesNV!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkRayTracingPipelineCreateInfoKHR* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateRayTracingPipelinesKHR!");
}
#ifdef CREATERAYTRACINGPIPELINESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateRayTracingPipelinesKHR_before(device, deferredOperation, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateRayTracingPipelinesKHR(device, deferredOperation, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"deferredOperation=" + ptrToString((void*)deferredOperation) + '!');
winsockSendToUI(&ConnectSocket,"pipelineCache=" + ptrToString((void*)pipelineCache) + '!');
winsockSendToUI(&ConnectSocket,"createInfoCount=" + std::to_string(createInfoCount) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfos=" + ptrToString((void*)pCreateInfos) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pPipelines=" + ptrToString((void*)pPipelines) + '!');
#ifdef CREATERAYTRACINGPIPELINESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateRayTracingPipelinesKHR_after(device, deferredOperation, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateRayTracingPipelinesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdTraceRaysIndirectKHR!");
}
#ifdef CMDTRACERAYSINDIRECTKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdTraceRaysIndirectKHR_before(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdTraceRaysIndirectKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pRaygenShaderBindingTable=" + ptrToString((void*)pRaygenShaderBindingTable) + '!');
winsockSendToUI(&ConnectSocket,"pMissShaderBindingTable=" + ptrToString((void*)pMissShaderBindingTable) + '!');
winsockSendToUI(&ConnectSocket,"pHitShaderBindingTable=" + ptrToString((void*)pHitShaderBindingTable) + '!');
winsockSendToUI(&ConnectSocket,"pCallableShaderBindingTable=" + ptrToString((void*)pCallableShaderBindingTable) + '!');
winsockSendToUI(&ConnectSocket,"indirectDeviceAddress=" + ptrToString((void*)indirectDeviceAddress) + '!');
#ifdef CMDTRACERAYSINDIRECTKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdTraceRaysIndirectKHR_after(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdTraceRaysIndirectKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdTraceRaysIndirect2KHR!");
}
#ifdef CMDTRACERAYSINDIRECT2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdTraceRaysIndirect2KHR_before(commandBuffer, indirectDeviceAddress);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdTraceRaysIndirect2KHR(commandBuffer, indirectDeviceAddress);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"indirectDeviceAddress=" + ptrToString((void*)indirectDeviceAddress) + '!');
#ifdef CMDTRACERAYSINDIRECT2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdTraceRaysIndirect2KHR_after(commandBuffer, indirectDeviceAddress);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdTraceRaysIndirect2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceAccelerationStructureCompatibilityKHR!");
}
#ifdef GETDEVICEACCELERATIONSTRUCTURECOMPATIBILITYKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceAccelerationStructureCompatibilityKHR_before(device, pVersionInfo, pCompatibility);
}
#endif 
device_dispatch[GetKey(device)].GetDeviceAccelerationStructureCompatibilityKHR(device, pVersionInfo, pCompatibility);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pVersionInfo=" + ptrToString((void*)pVersionInfo) + '!');
winsockSendToUI(&ConnectSocket,"pCompatibility=" + ptrToString((void*)pCompatibility) + '!');
#ifdef GETDEVICEACCELERATIONSTRUCTURECOMPATIBILITYKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceAccelerationStructureCompatibilityKHR_after(device, pVersionInfo, pCompatibility);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceAccelerationStructureCompatibilityKHR!");
}
}

 VK_LAYER_EXPORT VkDeviceSize VKAPI_CALL DetailsLayer_GetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetRayTracingShaderGroupStackSizeKHR!");
}
#ifdef GETRAYTRACINGSHADERGROUPSTACKSIZEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetRayTracingShaderGroupStackSizeKHR_before(device, pipeline, group, groupShader);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetRayTracingShaderGroupStackSizeKHR(device, pipeline, group, groupShader);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pipeline=" + ptrToString((void*)pipeline) + '!');
winsockSendToUI(&ConnectSocket,"group=" + std::to_string(group) + '!');
winsockSendToUI(&ConnectSocket,"groupShader=" + std::to_string(groupShader) + '!');
#ifdef GETRAYTRACINGSHADERGROUPSTACKSIZEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetRayTracingShaderGroupStackSizeKHR_after(device, pipeline, group, groupShader);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetRayTracingShaderGroupStackSizeKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetRayTracingPipelineStackSizeKHR!");
}
#ifdef CMDSETRAYTRACINGPIPELINESTACKSIZEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetRayTracingPipelineStackSizeKHR_before(commandBuffer, pipelineStackSize);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetRayTracingPipelineStackSizeKHR(commandBuffer, pipelineStackSize);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pipelineStackSize=" + std::to_string(pipelineStackSize) + '!');
#ifdef CMDSETRAYTRACINGPIPELINESTACKSIZEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetRayTracingPipelineStackSizeKHR_after(commandBuffer, pipelineStackSize);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetRayTracingPipelineStackSizeKHR!");
}
}

 VK_LAYER_EXPORT uint32_t VKAPI_CALL DetailsLayer_GetImageViewHandleNVX(VkDevice device, VkImageViewHandleInfoNVX* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetImageViewHandleNVX!");
}
#ifdef GETIMAGEVIEWHANDLENVX_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetImageViewHandleNVX_before(device, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetImageViewHandleNVX(device, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef GETIMAGEVIEWHANDLENVX_AFTER_EXEC_EXISTS
if(connected) {
layer_GetImageViewHandleNVX_after(device, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetImageViewHandleNVX!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetImageViewAddressNVX!");
}
#ifdef GETIMAGEVIEWADDRESSNVX_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetImageViewAddressNVX_before(device, imageView, pProperties);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetImageViewAddressNVX(device, imageView, pProperties);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"imageView=" + ptrToString((void*)imageView) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETIMAGEVIEWADDRESSNVX_AFTER_EXEC_EXISTS
if(connected) {
layer_GetImageViewAddressNVX_after(device, imageView, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetImageViewAddressNVX!");
}
return ret;
}

#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDeviceGroupSurfacePresentModes2EXT(VkDevice device, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceGroupSurfacePresentModes2EXT!");
}
#ifdef GETDEVICEGROUPSURFACEPRESENTMODES2EXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceGroupSurfacePresentModes2EXT_before(device, pSurfaceInfo, pModes);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetDeviceGroupSurfacePresentModes2EXT(device, pSurfaceInfo, pModes);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pSurfaceInfo=" + ptrToString((void*)pSurfaceInfo) + '!');
winsockSendToUI(&ConnectSocket,"pModes=" + ptrToString((void*)pModes) + '!');
#ifdef GETDEVICEGROUPSURFACEPRESENTMODES2EXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceGroupSurfacePresentModes2EXT_after(device, pSurfaceInfo, pModes);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceGroupSurfacePresentModes2EXT!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkAcquireFullScreenExclusiveModeEXT!");
}
#ifdef ACQUIREFULLSCREENEXCLUSIVEMODEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_AcquireFullScreenExclusiveModeEXT_before(device, swapchain);
}
#endif 
auto ret = device_dispatch[GetKey(device)].AcquireFullScreenExclusiveModeEXT(device, swapchain);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapchain=" + ptrToString((void*)swapchain) + '!');
#ifdef ACQUIREFULLSCREENEXCLUSIVEMODEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_AcquireFullScreenExclusiveModeEXT_after(device, swapchain);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkAcquireFullScreenExclusiveModeEXT!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkReleaseFullScreenExclusiveModeEXT!");
}
#ifdef RELEASEFULLSCREENEXCLUSIVEMODEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_ReleaseFullScreenExclusiveModeEXT_before(device, swapchain);
}
#endif 
auto ret = device_dispatch[GetKey(device)].ReleaseFullScreenExclusiveModeEXT(device, swapchain);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapchain=" + ptrToString((void*)swapchain) + '!');
#ifdef RELEASEFULLSCREENEXCLUSIVEMODEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_ReleaseFullScreenExclusiveModeEXT_after(device, swapchain);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkReleaseFullScreenExclusiveModeEXT!");
}
return ret;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquireProfilingLockKHR(VkDevice device, VkAcquireProfilingLockInfoKHR* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkAcquireProfilingLockKHR!");
}
#ifdef ACQUIREPROFILINGLOCKKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_AcquireProfilingLockKHR_before(device, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].AcquireProfilingLockKHR(device, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef ACQUIREPROFILINGLOCKKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_AcquireProfilingLockKHR_after(device, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkAcquireProfilingLockKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_ReleaseProfilingLockKHR(VkDevice device) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkReleaseProfilingLockKHR!");
}
#ifdef RELEASEPROFILINGLOCKKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_ReleaseProfilingLockKHR_before(device);
}
#endif 
device_dispatch[GetKey(device)].ReleaseProfilingLockKHR(device);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
#ifdef RELEASEPROFILINGLOCKKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_ReleaseProfilingLockKHR_after(device);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkReleaseProfilingLockKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetImageDrmFormatModifierPropertiesEXT!");
}
#ifdef GETIMAGEDRMFORMATMODIFIERPROPERTIESEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetImageDrmFormatModifierPropertiesEXT_before(device, image, pProperties);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetImageDrmFormatModifierPropertiesEXT(device, image, pProperties);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"image=" + ptrToString((void*)image) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETIMAGEDRMFORMATMODIFIERPROPERTIESEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetImageDrmFormatModifierPropertiesEXT_after(device, image, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetImageDrmFormatModifierPropertiesEXT!");
}
return ret;
}

 VK_LAYER_EXPORT uint64_t VKAPI_CALL DetailsLayer_GetBufferOpaqueCaptureAddress(VkDevice device, VkBufferDeviceAddressInfo* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetBufferOpaqueCaptureAddress!");
}
#ifdef GETBUFFEROPAQUECAPTUREADDRESS_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetBufferOpaqueCaptureAddress_before(device, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetBufferOpaqueCaptureAddress(device, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef GETBUFFEROPAQUECAPTUREADDRESS_AFTER_EXEC_EXISTS
if(connected) {
layer_GetBufferOpaqueCaptureAddress_after(device, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetBufferOpaqueCaptureAddress!");
}
return ret;
}

 VK_LAYER_EXPORT VkDeviceAddress VKAPI_CALL DetailsLayer_GetBufferDeviceAddress(VkDevice device, VkBufferDeviceAddressInfo* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetBufferDeviceAddress!");
}
#ifdef GETBUFFERDEVICEADDRESS_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetBufferDeviceAddress_before(device, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetBufferDeviceAddress(device, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef GETBUFFERDEVICEADDRESS_AFTER_EXEC_EXISTS
if(connected) {
layer_GetBufferDeviceAddress_after(device, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetBufferDeviceAddress!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_InitializePerformanceApiINTEL(VkDevice device, VkInitializePerformanceApiInfoINTEL* pInitializeInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkInitializePerformanceApiINTEL!");
}
#ifdef INITIALIZEPERFORMANCEAPIINTEL_BEFORE_EXEC_EXISTS
if(connected) {
layer_InitializePerformanceApiINTEL_before(device, pInitializeInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].InitializePerformanceApiINTEL(device, pInitializeInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInitializeInfo=" + ptrToString((void*)pInitializeInfo) + '!');
#ifdef INITIALIZEPERFORMANCEAPIINTEL_AFTER_EXEC_EXISTS
if(connected) {
layer_InitializePerformanceApiINTEL_after(device, pInitializeInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkInitializePerformanceApiINTEL!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_UninitializePerformanceApiINTEL(VkDevice device) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkUninitializePerformanceApiINTEL!");
}
#ifdef UNINITIALIZEPERFORMANCEAPIINTEL_BEFORE_EXEC_EXISTS
if(connected) {
layer_UninitializePerformanceApiINTEL_before(device);
}
#endif 
device_dispatch[GetKey(device)].UninitializePerformanceApiINTEL(device);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
#ifdef UNINITIALIZEPERFORMANCEAPIINTEL_AFTER_EXEC_EXISTS
if(connected) {
layer_UninitializePerformanceApiINTEL_after(device);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkUninitializePerformanceApiINTEL!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, VkPerformanceMarkerInfoINTEL* pMarkerInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetPerformanceMarkerINTEL!");
}
#ifdef CMDSETPERFORMANCEMARKERINTEL_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetPerformanceMarkerINTEL_before(commandBuffer, pMarkerInfo);
}
#endif 
auto ret = device_dispatch[GetKey(commandBuffer)].CmdSetPerformanceMarkerINTEL(commandBuffer, pMarkerInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pMarkerInfo=" + ptrToString((void*)pMarkerInfo) + '!');
#ifdef CMDSETPERFORMANCEMARKERINTEL_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetPerformanceMarkerINTEL_after(commandBuffer, pMarkerInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetPerformanceMarkerINTEL!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetPerformanceStreamMarkerINTEL!");
}
#ifdef CMDSETPERFORMANCESTREAMMARKERINTEL_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetPerformanceStreamMarkerINTEL_before(commandBuffer, pMarkerInfo);
}
#endif 
auto ret = device_dispatch[GetKey(commandBuffer)].CmdSetPerformanceStreamMarkerINTEL(commandBuffer, pMarkerInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pMarkerInfo=" + ptrToString((void*)pMarkerInfo) + '!');
#ifdef CMDSETPERFORMANCESTREAMMARKERINTEL_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetPerformanceStreamMarkerINTEL_after(commandBuffer, pMarkerInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetPerformanceStreamMarkerINTEL!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, VkPerformanceOverrideInfoINTEL* pOverrideInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetPerformanceOverrideINTEL!");
}
#ifdef CMDSETPERFORMANCEOVERRIDEINTEL_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetPerformanceOverrideINTEL_before(commandBuffer, pOverrideInfo);
}
#endif 
auto ret = device_dispatch[GetKey(commandBuffer)].CmdSetPerformanceOverrideINTEL(commandBuffer, pOverrideInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pOverrideInfo=" + ptrToString((void*)pOverrideInfo) + '!');
#ifdef CMDSETPERFORMANCEOVERRIDEINTEL_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetPerformanceOverrideINTEL_after(commandBuffer, pOverrideInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetPerformanceOverrideINTEL!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquirePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkAcquirePerformanceConfigurationINTEL!");
}
#ifdef ACQUIREPERFORMANCECONFIGURATIONINTEL_BEFORE_EXEC_EXISTS
if(connected) {
layer_AcquirePerformanceConfigurationINTEL_before(device, pAcquireInfo, pConfiguration);
}
#endif 
auto ret = device_dispatch[GetKey(device)].AcquirePerformanceConfigurationINTEL(device, pAcquireInfo, pConfiguration);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pAcquireInfo=" + ptrToString((void*)pAcquireInfo) + '!');
winsockSendToUI(&ConnectSocket,"pConfiguration=" + ptrToString((void*)pConfiguration) + '!');
#ifdef ACQUIREPERFORMANCECONFIGURATIONINTEL_AFTER_EXEC_EXISTS
if(connected) {
layer_AcquirePerformanceConfigurationINTEL_after(device, pAcquireInfo, pConfiguration);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkAcquirePerformanceConfigurationINTEL!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkReleasePerformanceConfigurationINTEL!");
}
#ifdef RELEASEPERFORMANCECONFIGURATIONINTEL_BEFORE_EXEC_EXISTS
if(connected) {
layer_ReleasePerformanceConfigurationINTEL_before(device, configuration);
}
#endif 
auto ret = device_dispatch[GetKey(device)].ReleasePerformanceConfigurationINTEL(device, configuration);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"configuration=" + ptrToString((void*)configuration) + '!');
#ifdef RELEASEPERFORMANCECONFIGURATIONINTEL_AFTER_EXEC_EXISTS
if(connected) {
layer_ReleasePerformanceConfigurationINTEL_after(device, configuration);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkReleasePerformanceConfigurationINTEL!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_QueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkQueueSetPerformanceConfigurationINTEL!");
}
#ifdef QUEUESETPERFORMANCECONFIGURATIONINTEL_BEFORE_EXEC_EXISTS
if(connected) {
layer_QueueSetPerformanceConfigurationINTEL_before(queue, configuration);
}
#endif 
auto ret = device_dispatch[GetKey(queue)].QueueSetPerformanceConfigurationINTEL(queue, configuration);
winsockSendToUI(&ConnectSocket,"queue=" + ptrToString((void*)queue) + '!');
winsockSendToUI(&ConnectSocket,"configuration=" + ptrToString((void*)configuration) + '!');
#ifdef QUEUESETPERFORMANCECONFIGURATIONINTEL_AFTER_EXEC_EXISTS
if(connected) {
layer_QueueSetPerformanceConfigurationINTEL_after(queue, configuration);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkQueueSetPerformanceConfigurationINTEL!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPerformanceParameterINTEL!");
}
#ifdef GETPERFORMANCEPARAMETERINTEL_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPerformanceParameterINTEL_before(device, parameter, pValue);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetPerformanceParameterINTEL(device, parameter, pValue);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"parameter=" + std::to_string(parameter) + '!');
winsockSendToUI(&ConnectSocket,"pValue=" + ptrToString((void*)pValue) + '!');
#ifdef GETPERFORMANCEPARAMETERINTEL_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPerformanceParameterINTEL_after(device, parameter, pValue);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPerformanceParameterINTEL!");
}
return ret;
}

 VK_LAYER_EXPORT uint64_t VKAPI_CALL DetailsLayer_GetDeviceMemoryOpaqueCaptureAddress(VkDevice device, VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceMemoryOpaqueCaptureAddress!");
}
#ifdef GETDEVICEMEMORYOPAQUECAPTUREADDRESS_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceMemoryOpaqueCaptureAddress_before(device, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetDeviceMemoryOpaqueCaptureAddress(device, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef GETDEVICEMEMORYOPAQUECAPTUREADDRESS_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceMemoryOpaqueCaptureAddress_after(device, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceMemoryOpaqueCaptureAddress!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPipelineExecutablePropertiesKHR(VkDevice device, VkPipelineInfoKHR* pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPipelineExecutablePropertiesKHR!");
}
#ifdef GETPIPELINEEXECUTABLEPROPERTIESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPipelineExecutablePropertiesKHR_before(device, pPipelineInfo, pExecutableCount, pProperties);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetPipelineExecutablePropertiesKHR(device, pPipelineInfo, pExecutableCount, pProperties);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pPipelineInfo=" + ptrToString((void*)pPipelineInfo) + '!');
winsockSendToUI(&ConnectSocket,"pExecutableCount=" + ptrToString((void*)pExecutableCount) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETPIPELINEEXECUTABLEPROPERTIESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPipelineExecutablePropertiesKHR_after(device, pPipelineInfo, pExecutableCount, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPipelineExecutablePropertiesKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPipelineExecutableStatisticsKHR(VkDevice device, VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPipelineExecutableStatisticsKHR!");
}
#ifdef GETPIPELINEEXECUTABLESTATISTICSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPipelineExecutableStatisticsKHR_before(device, pExecutableInfo, pStatisticCount, pStatistics);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetPipelineExecutableStatisticsKHR(device, pExecutableInfo, pStatisticCount, pStatistics);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pExecutableInfo=" + ptrToString((void*)pExecutableInfo) + '!');
winsockSendToUI(&ConnectSocket,"pStatisticCount=" + ptrToString((void*)pStatisticCount) + '!');
winsockSendToUI(&ConnectSocket,"pStatistics=" + ptrToString((void*)pStatistics) + '!');
#ifdef GETPIPELINEEXECUTABLESTATISTICSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPipelineExecutableStatisticsKHR_after(device, pExecutableInfo, pStatisticCount, pStatistics);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPipelineExecutableStatisticsKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPipelineExecutableInternalRepresentationsKHR(VkDevice device, VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPipelineExecutableInternalRepresentationsKHR!");
}
#ifdef GETPIPELINEEXECUTABLEINTERNALREPRESENTATIONSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPipelineExecutableInternalRepresentationsKHR_before(device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetPipelineExecutableInternalRepresentationsKHR(device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pExecutableInfo=" + ptrToString((void*)pExecutableInfo) + '!');
winsockSendToUI(&ConnectSocket,"pInternalRepresentationCount=" + ptrToString((void*)pInternalRepresentationCount) + '!');
winsockSendToUI(&ConnectSocket,"pInternalRepresentations=" + ptrToString((void*)pInternalRepresentations) + '!');
#ifdef GETPIPELINEEXECUTABLEINTERNALREPRESENTATIONSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPipelineExecutableInternalRepresentationsKHR_after(device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPipelineExecutableInternalRepresentationsKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateAccelerationStructureKHR(VkDevice device, VkAccelerationStructureCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkAccelerationStructureKHR* pAccelerationStructure) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateAccelerationStructureKHR!");
}
#ifdef CREATEACCELERATIONSTRUCTUREKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateAccelerationStructureKHR_before(device, pCreateInfo, pAllocator, pAccelerationStructure);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateAccelerationStructureKHR(device, pCreateInfo, pAllocator, pAccelerationStructure);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pAccelerationStructure=" + ptrToString((void*)pAccelerationStructure) + '!');
#ifdef CREATEACCELERATIONSTRUCTUREKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateAccelerationStructureKHR_after(device, pCreateInfo, pAllocator, pAccelerationStructure);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateAccelerationStructureKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBuildAccelerationStructuresKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, VkAccelerationStructureBuildGeometryInfoKHR* pInfos, VkAccelerationStructureBuildRangeInfoKHR** ppBuildRangeInfos) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBuildAccelerationStructuresKHR!");
}
#ifdef CMDBUILDACCELERATIONSTRUCTURESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBuildAccelerationStructuresKHR_before(commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBuildAccelerationStructuresKHR(commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"infoCount=" + std::to_string(infoCount) + '!');
winsockSendToUI(&ConnectSocket,"pInfos=" + ptrToString((void*)pInfos) + '!');
winsockSendToUI(&ConnectSocket,"ppBuildRangeInfos=" + ptrToString((void*)ppBuildRangeInfos) + '!');
#ifdef CMDBUILDACCELERATIONSTRUCTURESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBuildAccelerationStructuresKHR_after(commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBuildAccelerationStructuresKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, VkAccelerationStructureBuildGeometryInfoKHR* pInfos, VkDeviceAddress* pIndirectDeviceAddresses, uint32_t* pIndirectStrides, uint32_t** ppMaxPrimitiveCounts) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBuildAccelerationStructuresIndirectKHR!");
}
#ifdef CMDBUILDACCELERATIONSTRUCTURESINDIRECTKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBuildAccelerationStructuresIndirectKHR_before(commandBuffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBuildAccelerationStructuresIndirectKHR(commandBuffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"infoCount=" + std::to_string(infoCount) + '!');
winsockSendToUI(&ConnectSocket,"pInfos=" + ptrToString((void*)pInfos) + '!');
winsockSendToUI(&ConnectSocket,"pIndirectDeviceAddresses=" + ptrToString((void*)pIndirectDeviceAddresses) + '!');
winsockSendToUI(&ConnectSocket,"pIndirectStrides=" + ptrToString((void*)pIndirectStrides) + '!');
winsockSendToUI(&ConnectSocket,"ppMaxPrimitiveCounts=" + ptrToString((void*)ppMaxPrimitiveCounts) + '!');
#ifdef CMDBUILDACCELERATIONSTRUCTURESINDIRECTKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBuildAccelerationStructuresIndirectKHR_after(commandBuffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBuildAccelerationStructuresIndirectKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BuildAccelerationStructuresKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, VkAccelerationStructureBuildGeometryInfoKHR* pInfos, VkAccelerationStructureBuildRangeInfoKHR** ppBuildRangeInfos) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkBuildAccelerationStructuresKHR!");
}
#ifdef BUILDACCELERATIONSTRUCTURESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_BuildAccelerationStructuresKHR_before(device, deferredOperation, infoCount, pInfos, ppBuildRangeInfos);
}
#endif 
auto ret = device_dispatch[GetKey(device)].BuildAccelerationStructuresKHR(device, deferredOperation, infoCount, pInfos, ppBuildRangeInfos);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"deferredOperation=" + ptrToString((void*)deferredOperation) + '!');
winsockSendToUI(&ConnectSocket,"infoCount=" + std::to_string(infoCount) + '!');
winsockSendToUI(&ConnectSocket,"pInfos=" + ptrToString((void*)pInfos) + '!');
winsockSendToUI(&ConnectSocket,"ppBuildRangeInfos=" + ptrToString((void*)ppBuildRangeInfos) + '!');
#ifdef BUILDACCELERATIONSTRUCTURESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_BuildAccelerationStructuresKHR_after(device, deferredOperation, infoCount, pInfos, ppBuildRangeInfos);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkBuildAccelerationStructuresKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkDeviceAddress VKAPI_CALL DetailsLayer_GetAccelerationStructureDeviceAddressKHR(VkDevice device, VkAccelerationStructureDeviceAddressInfoKHR* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetAccelerationStructureDeviceAddressKHR!");
}
#ifdef GETACCELERATIONSTRUCTUREDEVICEADDRESSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetAccelerationStructureDeviceAddressKHR_before(device, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetAccelerationStructureDeviceAddressKHR(device, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef GETACCELERATIONSTRUCTUREDEVICEADDRESSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetAccelerationStructureDeviceAddressKHR_after(device, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetAccelerationStructureDeviceAddressKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDeferredOperationKHR(VkDevice device, VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateDeferredOperationKHR!");
}
#ifdef CREATEDEFERREDOPERATIONKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateDeferredOperationKHR_before(device, pAllocator, pDeferredOperation);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateDeferredOperationKHR(device, pAllocator, pDeferredOperation);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pDeferredOperation=" + ptrToString((void*)pDeferredOperation) + '!');
#ifdef CREATEDEFERREDOPERATIONKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateDeferredOperationKHR_after(device, pAllocator, pDeferredOperation);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateDeferredOperationKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyDeferredOperationKHR!");
}
#ifdef DESTROYDEFERREDOPERATIONKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyDeferredOperationKHR_before(device, operation, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyDeferredOperationKHR(device, operation, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"operation=" + ptrToString((void*)operation) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYDEFERREDOPERATIONKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyDeferredOperationKHR_after(device, operation, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyDeferredOperationKHR!");
}
}

 VK_LAYER_EXPORT uint32_t VKAPI_CALL DetailsLayer_GetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeferredOperationMaxConcurrencyKHR!");
}
#ifdef GETDEFERREDOPERATIONMAXCONCURRENCYKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeferredOperationMaxConcurrencyKHR_before(device, operation);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetDeferredOperationMaxConcurrencyKHR(device, operation);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"operation=" + ptrToString((void*)operation) + '!');
#ifdef GETDEFERREDOPERATIONMAXCONCURRENCYKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeferredOperationMaxConcurrencyKHR_after(device, operation);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeferredOperationMaxConcurrencyKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeferredOperationResultKHR!");
}
#ifdef GETDEFERREDOPERATIONRESULTKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeferredOperationResultKHR_before(device, operation);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetDeferredOperationResultKHR(device, operation);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"operation=" + ptrToString((void*)operation) + '!');
#ifdef GETDEFERREDOPERATIONRESULTKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeferredOperationResultKHR_after(device, operation);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeferredOperationResultKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_DeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDeferredOperationJoinKHR!");
}
#ifdef DEFERREDOPERATIONJOINKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_DeferredOperationJoinKHR_before(device, operation);
}
#endif 
auto ret = device_dispatch[GetKey(device)].DeferredOperationJoinKHR(device, operation);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"operation=" + ptrToString((void*)operation) + '!');
#ifdef DEFERREDOPERATIONJOINKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_DeferredOperationJoinKHR_after(device, operation);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDeferredOperationJoinKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPipelineIndirectMemoryRequirementsNV(VkDevice device, VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPipelineIndirectMemoryRequirementsNV!");
}
#ifdef GETPIPELINEINDIRECTMEMORYREQUIREMENTSNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPipelineIndirectMemoryRequirementsNV_before(device, pCreateInfo, pMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetPipelineIndirectMemoryRequirementsNV(device, pCreateInfo, pMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRequirements=" + ptrToString((void*)pMemoryRequirements) + '!');
#ifdef GETPIPELINEINDIRECTMEMORYREQUIREMENTSNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPipelineIndirectMemoryRequirementsNV_after(device, pCreateInfo, pMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPipelineIndirectMemoryRequirementsNV!");
}
}

 VK_LAYER_EXPORT VkDeviceAddress VKAPI_CALL DetailsLayer_GetPipelineIndirectDeviceAddressNV(VkDevice device, VkPipelineIndirectDeviceAddressInfoNV* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPipelineIndirectDeviceAddressNV!");
}
#ifdef GETPIPELINEINDIRECTDEVICEADDRESSNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPipelineIndirectDeviceAddressNV_before(device, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetPipelineIndirectDeviceAddressNV(device, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef GETPIPELINEINDIRECTDEVICEADDRESSNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPipelineIndirectDeviceAddressNV_after(device, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPipelineIndirectDeviceAddressNV!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetCullMode!");
}
#ifdef CMDSETCULLMODE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetCullMode_before(commandBuffer, cullMode);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCullMode(commandBuffer, cullMode);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"cullMode=" + ptrToString((void*)cullMode) + '!');
#ifdef CMDSETCULLMODE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetCullMode_after(commandBuffer, cullMode);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetCullMode!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetFrontFace!");
}
#ifdef CMDSETFRONTFACE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetFrontFace_before(commandBuffer, frontFace);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetFrontFace(commandBuffer, frontFace);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"frontFace=" + std::to_string(frontFace) + '!');
#ifdef CMDSETFRONTFACE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetFrontFace_after(commandBuffer, frontFace);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetFrontFace!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetPrimitiveTopology!");
}
#ifdef CMDSETPRIMITIVETOPOLOGY_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetPrimitiveTopology_before(commandBuffer, primitiveTopology);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetPrimitiveTopology(commandBuffer, primitiveTopology);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"primitiveTopology=" + std::to_string(primitiveTopology) + '!');
#ifdef CMDSETPRIMITIVETOPOLOGY_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetPrimitiveTopology_after(commandBuffer, primitiveTopology);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetPrimitiveTopology!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, VkViewport* pViewports) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetViewportWithCount!");
}
#ifdef CMDSETVIEWPORTWITHCOUNT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetViewportWithCount_before(commandBuffer, viewportCount, pViewports);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetViewportWithCount(commandBuffer, viewportCount, pViewports);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"viewportCount=" + std::to_string(viewportCount) + '!');
winsockSendToUI(&ConnectSocket,"pViewports=" + ptrToString((void*)pViewports) + '!');
#ifdef CMDSETVIEWPORTWITHCOUNT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetViewportWithCount_after(commandBuffer, viewportCount, pViewports);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetViewportWithCount!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, VkRect2D* pScissors) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetScissorWithCount!");
}
#ifdef CMDSETSCISSORWITHCOUNT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetScissorWithCount_before(commandBuffer, scissorCount, pScissors);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetScissorWithCount(commandBuffer, scissorCount, pScissors);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"scissorCount=" + std::to_string(scissorCount) + '!');
winsockSendToUI(&ConnectSocket,"pScissors=" + ptrToString((void*)pScissors) + '!');
#ifdef CMDSETSCISSORWITHCOUNT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetScissorWithCount_after(commandBuffer, scissorCount, pScissors);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetScissorWithCount!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindIndexBuffer2KHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBindIndexBuffer2KHR!");
}
#ifdef CMDBINDINDEXBUFFER2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBindIndexBuffer2KHR_before(commandBuffer, buffer, offset, size, indexType);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindIndexBuffer2KHR(commandBuffer, buffer, offset, size, indexType);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
winsockSendToUI(&ConnectSocket,"size=" + ptrToString((void*)size) + '!');
winsockSendToUI(&ConnectSocket,"indexType=" + std::to_string(indexType) + '!');
#ifdef CMDBINDINDEXBUFFER2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBindIndexBuffer2KHR_after(commandBuffer, buffer, offset, size, indexType);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBindIndexBuffer2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets, VkDeviceSize* pSizes, VkDeviceSize* pStrides) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBindVertexBuffers2!");
}
#ifdef CMDBINDVERTEXBUFFERS2_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBindVertexBuffers2_before(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindVertexBuffers2(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstBinding=" + std::to_string(firstBinding) + '!');
winsockSendToUI(&ConnectSocket,"bindingCount=" + std::to_string(bindingCount) + '!');
winsockSendToUI(&ConnectSocket,"pBuffers=" + ptrToString((void*)pBuffers) + '!');
winsockSendToUI(&ConnectSocket,"pOffsets=" + ptrToString((void*)pOffsets) + '!');
winsockSendToUI(&ConnectSocket,"pSizes=" + ptrToString((void*)pSizes) + '!');
winsockSendToUI(&ConnectSocket,"pStrides=" + ptrToString((void*)pStrides) + '!');
#ifdef CMDBINDVERTEXBUFFERS2_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBindVertexBuffers2_after(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBindVertexBuffers2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDepthTestEnable!");
}
#ifdef CMDSETDEPTHTESTENABLE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthTestEnable_before(commandBuffer, depthTestEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthTestEnable(commandBuffer, depthTestEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"depthTestEnable=" + bool_as_text(depthTestEnable) + '!');
#ifdef CMDSETDEPTHTESTENABLE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthTestEnable_after(commandBuffer, depthTestEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDepthTestEnable!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDepthWriteEnable!");
}
#ifdef CMDSETDEPTHWRITEENABLE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthWriteEnable_before(commandBuffer, depthWriteEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthWriteEnable(commandBuffer, depthWriteEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"depthWriteEnable=" + bool_as_text(depthWriteEnable) + '!');
#ifdef CMDSETDEPTHWRITEENABLE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthWriteEnable_after(commandBuffer, depthWriteEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDepthWriteEnable!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDepthCompareOp!");
}
#ifdef CMDSETDEPTHCOMPAREOP_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthCompareOp_before(commandBuffer, depthCompareOp);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthCompareOp(commandBuffer, depthCompareOp);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"depthCompareOp=" + std::to_string(depthCompareOp) + '!');
#ifdef CMDSETDEPTHCOMPAREOP_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthCompareOp_after(commandBuffer, depthCompareOp);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDepthCompareOp!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDepthBoundsTestEnable!");
}
#ifdef CMDSETDEPTHBOUNDSTESTENABLE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthBoundsTestEnable_before(commandBuffer, depthBoundsTestEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthBoundsTestEnable(commandBuffer, depthBoundsTestEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"depthBoundsTestEnable=" + bool_as_text(depthBoundsTestEnable) + '!');
#ifdef CMDSETDEPTHBOUNDSTESTENABLE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthBoundsTestEnable_after(commandBuffer, depthBoundsTestEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDepthBoundsTestEnable!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetStencilTestEnable!");
}
#ifdef CMDSETSTENCILTESTENABLE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetStencilTestEnable_before(commandBuffer, stencilTestEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetStencilTestEnable(commandBuffer, stencilTestEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"stencilTestEnable=" + bool_as_text(stencilTestEnable) + '!');
#ifdef CMDSETSTENCILTESTENABLE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetStencilTestEnable_after(commandBuffer, stencilTestEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetStencilTestEnable!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetStencilOp!");
}
#ifdef CMDSETSTENCILOP_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetStencilOp_before(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetStencilOp(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"faceMask=" + ptrToString((void*)faceMask) + '!');
winsockSendToUI(&ConnectSocket,"failOp=" + std::to_string(failOp) + '!');
winsockSendToUI(&ConnectSocket,"passOp=" + std::to_string(passOp) + '!');
winsockSendToUI(&ConnectSocket,"depthFailOp=" + std::to_string(depthFailOp) + '!');
winsockSendToUI(&ConnectSocket,"compareOp=" + std::to_string(compareOp) + '!');
#ifdef CMDSETSTENCILOP_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetStencilOp_after(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetStencilOp!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetPatchControlPointsEXT!");
}
#ifdef CMDSETPATCHCONTROLPOINTSEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetPatchControlPointsEXT_before(commandBuffer, patchControlPoints);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetPatchControlPointsEXT(commandBuffer, patchControlPoints);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"patchControlPoints=" + std::to_string(patchControlPoints) + '!');
#ifdef CMDSETPATCHCONTROLPOINTSEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetPatchControlPointsEXT_after(commandBuffer, patchControlPoints);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetPatchControlPointsEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetRasterizerDiscardEnable!");
}
#ifdef CMDSETRASTERIZERDISCARDENABLE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetRasterizerDiscardEnable_before(commandBuffer, rasterizerDiscardEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetRasterizerDiscardEnable(commandBuffer, rasterizerDiscardEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"rasterizerDiscardEnable=" + bool_as_text(rasterizerDiscardEnable) + '!');
#ifdef CMDSETRASTERIZERDISCARDENABLE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetRasterizerDiscardEnable_after(commandBuffer, rasterizerDiscardEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetRasterizerDiscardEnable!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDepthBiasEnable!");
}
#ifdef CMDSETDEPTHBIASENABLE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthBiasEnable_before(commandBuffer, depthBiasEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthBiasEnable(commandBuffer, depthBiasEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"depthBiasEnable=" + bool_as_text(depthBiasEnable) + '!');
#ifdef CMDSETDEPTHBIASENABLE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthBiasEnable_after(commandBuffer, depthBiasEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDepthBiasEnable!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetLogicOpEXT!");
}
#ifdef CMDSETLOGICOPEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetLogicOpEXT_before(commandBuffer, logicOp);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetLogicOpEXT(commandBuffer, logicOp);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"logicOp=" + std::to_string(logicOp) + '!');
#ifdef CMDSETLOGICOPEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetLogicOpEXT_after(commandBuffer, logicOp);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetLogicOpEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetPrimitiveRestartEnable!");
}
#ifdef CMDSETPRIMITIVERESTARTENABLE_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetPrimitiveRestartEnable_before(commandBuffer, primitiveRestartEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetPrimitiveRestartEnable(commandBuffer, primitiveRestartEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"primitiveRestartEnable=" + bool_as_text(primitiveRestartEnable) + '!');
#ifdef CMDSETPRIMITIVERESTARTENABLE_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetPrimitiveRestartEnable_after(commandBuffer, primitiveRestartEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetPrimitiveRestartEnable!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetTessellationDomainOriginEXT!");
}
#ifdef CMDSETTESSELLATIONDOMAINORIGINEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetTessellationDomainOriginEXT_before(commandBuffer, domainOrigin);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetTessellationDomainOriginEXT(commandBuffer, domainOrigin);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"domainOrigin=" + std::to_string(domainOrigin) + '!');
#ifdef CMDSETTESSELLATIONDOMAINORIGINEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetTessellationDomainOriginEXT_after(commandBuffer, domainOrigin);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetTessellationDomainOriginEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDepthClampEnableEXT!");
}
#ifdef CMDSETDEPTHCLAMPENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthClampEnableEXT_before(commandBuffer, depthClampEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthClampEnableEXT(commandBuffer, depthClampEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"depthClampEnable=" + bool_as_text(depthClampEnable) + '!');
#ifdef CMDSETDEPTHCLAMPENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthClampEnableEXT_after(commandBuffer, depthClampEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDepthClampEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetPolygonModeEXT!");
}
#ifdef CMDSETPOLYGONMODEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetPolygonModeEXT_before(commandBuffer, polygonMode);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetPolygonModeEXT(commandBuffer, polygonMode);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"polygonMode=" + std::to_string(polygonMode) + '!');
#ifdef CMDSETPOLYGONMODEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetPolygonModeEXT_after(commandBuffer, polygonMode);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetPolygonModeEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits rasterizationSamples) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetRasterizationSamplesEXT!");
}
#ifdef CMDSETRASTERIZATIONSAMPLESEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetRasterizationSamplesEXT_before(commandBuffer, rasterizationSamples);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetRasterizationSamplesEXT(commandBuffer, rasterizationSamples);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"rasterizationSamples=" + std::to_string(rasterizationSamples) + '!');
#ifdef CMDSETRASTERIZATIONSAMPLESEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetRasterizationSamplesEXT_after(commandBuffer, rasterizationSamples);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetRasterizationSamplesEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits samples, VkSampleMask* pSampleMask) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetSampleMaskEXT!");
}
#ifdef CMDSETSAMPLEMASKEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetSampleMaskEXT_before(commandBuffer, samples, pSampleMask);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetSampleMaskEXT(commandBuffer, samples, pSampleMask);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"samples=" + std::to_string(samples) + '!');
winsockSendToUI(&ConnectSocket,"pSampleMask=" + ptrToString((void*)pSampleMask) + '!');
#ifdef CMDSETSAMPLEMASKEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetSampleMaskEXT_after(commandBuffer, samples, pSampleMask);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetSampleMaskEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetAlphaToCoverageEnableEXT!");
}
#ifdef CMDSETALPHATOCOVERAGEENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetAlphaToCoverageEnableEXT_before(commandBuffer, alphaToCoverageEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetAlphaToCoverageEnableEXT(commandBuffer, alphaToCoverageEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"alphaToCoverageEnable=" + bool_as_text(alphaToCoverageEnable) + '!');
#ifdef CMDSETALPHATOCOVERAGEENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetAlphaToCoverageEnableEXT_after(commandBuffer, alphaToCoverageEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetAlphaToCoverageEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetAlphaToOneEnableEXT!");
}
#ifdef CMDSETALPHATOONEENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetAlphaToOneEnableEXT_before(commandBuffer, alphaToOneEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetAlphaToOneEnableEXT(commandBuffer, alphaToOneEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"alphaToOneEnable=" + bool_as_text(alphaToOneEnable) + '!');
#ifdef CMDSETALPHATOONEENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetAlphaToOneEnableEXT_after(commandBuffer, alphaToOneEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetAlphaToOneEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetLogicOpEnableEXT!");
}
#ifdef CMDSETLOGICOPENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetLogicOpEnableEXT_before(commandBuffer, logicOpEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetLogicOpEnableEXT(commandBuffer, logicOpEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"logicOpEnable=" + bool_as_text(logicOpEnable) + '!');
#ifdef CMDSETLOGICOPENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetLogicOpEnableEXT_after(commandBuffer, logicOpEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetLogicOpEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, VkBool32* pColorBlendEnables) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetColorBlendEnableEXT!");
}
#ifdef CMDSETCOLORBLENDENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetColorBlendEnableEXT_before(commandBuffer, firstAttachment, attachmentCount, pColorBlendEnables);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetColorBlendEnableEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendEnables);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstAttachment=" + std::to_string(firstAttachment) + '!');
winsockSendToUI(&ConnectSocket,"attachmentCount=" + std::to_string(attachmentCount) + '!');
winsockSendToUI(&ConnectSocket,"pColorBlendEnables=" + ptrToString((void*)pColorBlendEnables) + '!');
#ifdef CMDSETCOLORBLENDENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetColorBlendEnableEXT_after(commandBuffer, firstAttachment, attachmentCount, pColorBlendEnables);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetColorBlendEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, VkColorBlendEquationEXT* pColorBlendEquations) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetColorBlendEquationEXT!");
}
#ifdef CMDSETCOLORBLENDEQUATIONEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetColorBlendEquationEXT_before(commandBuffer, firstAttachment, attachmentCount, pColorBlendEquations);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetColorBlendEquationEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendEquations);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstAttachment=" + std::to_string(firstAttachment) + '!');
winsockSendToUI(&ConnectSocket,"attachmentCount=" + std::to_string(attachmentCount) + '!');
winsockSendToUI(&ConnectSocket,"pColorBlendEquations=" + ptrToString((void*)pColorBlendEquations) + '!');
#ifdef CMDSETCOLORBLENDEQUATIONEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetColorBlendEquationEXT_after(commandBuffer, firstAttachment, attachmentCount, pColorBlendEquations);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetColorBlendEquationEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, VkColorComponentFlags* pColorWriteMasks) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetColorWriteMaskEXT!");
}
#ifdef CMDSETCOLORWRITEMASKEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetColorWriteMaskEXT_before(commandBuffer, firstAttachment, attachmentCount, pColorWriteMasks);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetColorWriteMaskEXT(commandBuffer, firstAttachment, attachmentCount, pColorWriteMasks);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstAttachment=" + std::to_string(firstAttachment) + '!');
winsockSendToUI(&ConnectSocket,"attachmentCount=" + std::to_string(attachmentCount) + '!');
winsockSendToUI(&ConnectSocket,"pColorWriteMasks=" + ptrToString((void*)pColorWriteMasks) + '!');
#ifdef CMDSETCOLORWRITEMASKEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetColorWriteMaskEXT_after(commandBuffer, firstAttachment, attachmentCount, pColorWriteMasks);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetColorWriteMaskEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetRasterizationStreamEXT!");
}
#ifdef CMDSETRASTERIZATIONSTREAMEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetRasterizationStreamEXT_before(commandBuffer, rasterizationStream);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetRasterizationStreamEXT(commandBuffer, rasterizationStream);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"rasterizationStream=" + std::to_string(rasterizationStream) + '!');
#ifdef CMDSETRASTERIZATIONSTREAMEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetRasterizationStreamEXT_after(commandBuffer, rasterizationStream);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetRasterizationStreamEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetConservativeRasterizationModeEXT!");
}
#ifdef CMDSETCONSERVATIVERASTERIZATIONMODEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetConservativeRasterizationModeEXT_before(commandBuffer, conservativeRasterizationMode);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetConservativeRasterizationModeEXT(commandBuffer, conservativeRasterizationMode);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"conservativeRasterizationMode=" + std::to_string(conservativeRasterizationMode) + '!');
#ifdef CMDSETCONSERVATIVERASTERIZATIONMODEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetConservativeRasterizationModeEXT_after(commandBuffer, conservativeRasterizationMode);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetConservativeRasterizationModeEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetExtraPrimitiveOverestimationSizeEXT!");
}
#ifdef CMDSETEXTRAPRIMITIVEOVERESTIMATIONSIZEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetExtraPrimitiveOverestimationSizeEXT_before(commandBuffer, extraPrimitiveOverestimationSize);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetExtraPrimitiveOverestimationSizeEXT(commandBuffer, extraPrimitiveOverestimationSize);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"extraPrimitiveOverestimationSize=" + std::to_string(extraPrimitiveOverestimationSize) + '!');
#ifdef CMDSETEXTRAPRIMITIVEOVERESTIMATIONSIZEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetExtraPrimitiveOverestimationSizeEXT_after(commandBuffer, extraPrimitiveOverestimationSize);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetExtraPrimitiveOverestimationSizeEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDepthClipEnableEXT!");
}
#ifdef CMDSETDEPTHCLIPENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthClipEnableEXT_before(commandBuffer, depthClipEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthClipEnableEXT(commandBuffer, depthClipEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"depthClipEnable=" + bool_as_text(depthClipEnable) + '!');
#ifdef CMDSETDEPTHCLIPENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthClipEnableEXT_after(commandBuffer, depthClipEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDepthClipEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetSampleLocationsEnableEXT!");
}
#ifdef CMDSETSAMPLELOCATIONSENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetSampleLocationsEnableEXT_before(commandBuffer, sampleLocationsEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetSampleLocationsEnableEXT(commandBuffer, sampleLocationsEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"sampleLocationsEnable=" + bool_as_text(sampleLocationsEnable) + '!');
#ifdef CMDSETSAMPLELOCATIONSENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetSampleLocationsEnableEXT_after(commandBuffer, sampleLocationsEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetSampleLocationsEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, VkColorBlendAdvancedEXT* pColorBlendAdvanced) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetColorBlendAdvancedEXT!");
}
#ifdef CMDSETCOLORBLENDADVANCEDEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetColorBlendAdvancedEXT_before(commandBuffer, firstAttachment, attachmentCount, pColorBlendAdvanced);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetColorBlendAdvancedEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendAdvanced);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstAttachment=" + std::to_string(firstAttachment) + '!');
winsockSendToUI(&ConnectSocket,"attachmentCount=" + std::to_string(attachmentCount) + '!');
winsockSendToUI(&ConnectSocket,"pColorBlendAdvanced=" + ptrToString((void*)pColorBlendAdvanced) + '!');
#ifdef CMDSETCOLORBLENDADVANCEDEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetColorBlendAdvancedEXT_after(commandBuffer, firstAttachment, attachmentCount, pColorBlendAdvanced);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetColorBlendAdvancedEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetProvokingVertexModeEXT!");
}
#ifdef CMDSETPROVOKINGVERTEXMODEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetProvokingVertexModeEXT_before(commandBuffer, provokingVertexMode);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetProvokingVertexModeEXT(commandBuffer, provokingVertexMode);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"provokingVertexMode=" + std::to_string(provokingVertexMode) + '!');
#ifdef CMDSETPROVOKINGVERTEXMODEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetProvokingVertexModeEXT_after(commandBuffer, provokingVertexMode);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetProvokingVertexModeEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetLineRasterizationModeEXT!");
}
#ifdef CMDSETLINERASTERIZATIONMODEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetLineRasterizationModeEXT_before(commandBuffer, lineRasterizationMode);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetLineRasterizationModeEXT(commandBuffer, lineRasterizationMode);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"lineRasterizationMode=" + ptrToString((void*)lineRasterizationMode) + '!');
#ifdef CMDSETLINERASTERIZATIONMODEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetLineRasterizationModeEXT_after(commandBuffer, lineRasterizationMode);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetLineRasterizationModeEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetLineStippleEnableEXT!");
}
#ifdef CMDSETLINESTIPPLEENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetLineStippleEnableEXT_before(commandBuffer, stippledLineEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetLineStippleEnableEXT(commandBuffer, stippledLineEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"stippledLineEnable=" + bool_as_text(stippledLineEnable) + '!');
#ifdef CMDSETLINESTIPPLEENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetLineStippleEnableEXT_after(commandBuffer, stippledLineEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetLineStippleEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDepthClipNegativeOneToOneEXT!");
}
#ifdef CMDSETDEPTHCLIPNEGATIVEONETOONEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthClipNegativeOneToOneEXT_before(commandBuffer, negativeOneToOne);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthClipNegativeOneToOneEXT(commandBuffer, negativeOneToOne);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"negativeOneToOne=" + bool_as_text(negativeOneToOne) + '!');
#ifdef CMDSETDEPTHCLIPNEGATIVEONETOONEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthClipNegativeOneToOneEXT_after(commandBuffer, negativeOneToOne);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDepthClipNegativeOneToOneEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetViewportWScalingEnableNV!");
}
#ifdef CMDSETVIEWPORTWSCALINGENABLENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetViewportWScalingEnableNV_before(commandBuffer, viewportWScalingEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetViewportWScalingEnableNV(commandBuffer, viewportWScalingEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"viewportWScalingEnable=" + bool_as_text(viewportWScalingEnable) + '!');
#ifdef CMDSETVIEWPORTWSCALINGENABLENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetViewportWScalingEnableNV_after(commandBuffer, viewportWScalingEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetViewportWScalingEnableNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewportSwizzleNV* pViewportSwizzles) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetViewportSwizzleNV!");
}
#ifdef CMDSETVIEWPORTSWIZZLENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetViewportSwizzleNV_before(commandBuffer, firstViewport, viewportCount, pViewportSwizzles);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetViewportSwizzleNV(commandBuffer, firstViewport, viewportCount, pViewportSwizzles);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstViewport=" + std::to_string(firstViewport) + '!');
winsockSendToUI(&ConnectSocket,"viewportCount=" + std::to_string(viewportCount) + '!');
winsockSendToUI(&ConnectSocket,"pViewportSwizzles=" + ptrToString((void*)pViewportSwizzles) + '!');
#ifdef CMDSETVIEWPORTSWIZZLENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetViewportSwizzleNV_after(commandBuffer, firstViewport, viewportCount, pViewportSwizzles);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetViewportSwizzleNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetCoverageToColorEnableNV!");
}
#ifdef CMDSETCOVERAGETOCOLORENABLENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetCoverageToColorEnableNV_before(commandBuffer, coverageToColorEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCoverageToColorEnableNV(commandBuffer, coverageToColorEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"coverageToColorEnable=" + bool_as_text(coverageToColorEnable) + '!');
#ifdef CMDSETCOVERAGETOCOLORENABLENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetCoverageToColorEnableNV_after(commandBuffer, coverageToColorEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetCoverageToColorEnableNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetCoverageToColorLocationNV!");
}
#ifdef CMDSETCOVERAGETOCOLORLOCATIONNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetCoverageToColorLocationNV_before(commandBuffer, coverageToColorLocation);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCoverageToColorLocationNV(commandBuffer, coverageToColorLocation);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"coverageToColorLocation=" + std::to_string(coverageToColorLocation) + '!');
#ifdef CMDSETCOVERAGETOCOLORLOCATIONNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetCoverageToColorLocationNV_after(commandBuffer, coverageToColorLocation);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetCoverageToColorLocationNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetCoverageModulationModeNV!");
}
#ifdef CMDSETCOVERAGEMODULATIONMODENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetCoverageModulationModeNV_before(commandBuffer, coverageModulationMode);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCoverageModulationModeNV(commandBuffer, coverageModulationMode);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"coverageModulationMode=" + std::to_string(coverageModulationMode) + '!');
#ifdef CMDSETCOVERAGEMODULATIONMODENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetCoverageModulationModeNV_after(commandBuffer, coverageModulationMode);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetCoverageModulationModeNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetCoverageModulationTableEnableNV!");
}
#ifdef CMDSETCOVERAGEMODULATIONTABLEENABLENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetCoverageModulationTableEnableNV_before(commandBuffer, coverageModulationTableEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCoverageModulationTableEnableNV(commandBuffer, coverageModulationTableEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"coverageModulationTableEnable=" + bool_as_text(coverageModulationTableEnable) + '!');
#ifdef CMDSETCOVERAGEMODULATIONTABLEENABLENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetCoverageModulationTableEnableNV_after(commandBuffer, coverageModulationTableEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetCoverageModulationTableEnableNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, float* pCoverageModulationTable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetCoverageModulationTableNV!");
}
#ifdef CMDSETCOVERAGEMODULATIONTABLENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetCoverageModulationTableNV_before(commandBuffer, coverageModulationTableCount, pCoverageModulationTable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCoverageModulationTableNV(commandBuffer, coverageModulationTableCount, pCoverageModulationTable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"coverageModulationTableCount=" + std::to_string(coverageModulationTableCount) + '!');
winsockSendToUI(&ConnectSocket,"pCoverageModulationTable=" + ptrToString((void*)pCoverageModulationTable) + '!');
#ifdef CMDSETCOVERAGEMODULATIONTABLENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetCoverageModulationTableNV_after(commandBuffer, coverageModulationTableCount, pCoverageModulationTable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetCoverageModulationTableNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetShadingRateImageEnableNV!");
}
#ifdef CMDSETSHADINGRATEIMAGEENABLENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetShadingRateImageEnableNV_before(commandBuffer, shadingRateImageEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetShadingRateImageEnableNV(commandBuffer, shadingRateImageEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"shadingRateImageEnable=" + bool_as_text(shadingRateImageEnable) + '!');
#ifdef CMDSETSHADINGRATEIMAGEENABLENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetShadingRateImageEnableNV_after(commandBuffer, shadingRateImageEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetShadingRateImageEnableNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetCoverageReductionModeNV!");
}
#ifdef CMDSETCOVERAGEREDUCTIONMODENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetCoverageReductionModeNV_before(commandBuffer, coverageReductionMode);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCoverageReductionModeNV(commandBuffer, coverageReductionMode);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"coverageReductionMode=" + std::to_string(coverageReductionMode) + '!');
#ifdef CMDSETCOVERAGEREDUCTIONMODENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetCoverageReductionModeNV_after(commandBuffer, coverageReductionMode);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetCoverageReductionModeNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetRepresentativeFragmentTestEnableNV!");
}
#ifdef CMDSETREPRESENTATIVEFRAGMENTTESTENABLENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetRepresentativeFragmentTestEnableNV_before(commandBuffer, representativeFragmentTestEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetRepresentativeFragmentTestEnableNV(commandBuffer, representativeFragmentTestEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"representativeFragmentTestEnable=" + bool_as_text(representativeFragmentTestEnable) + '!');
#ifdef CMDSETREPRESENTATIVEFRAGMENTTESTENABLENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetRepresentativeFragmentTestEnableNV_after(commandBuffer, representativeFragmentTestEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetRepresentativeFragmentTestEnableNV!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreatePrivateDataSlot(VkDevice device, VkPrivateDataSlotCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreatePrivateDataSlot!");
}
#ifdef CREATEPRIVATEDATASLOT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreatePrivateDataSlot_before(device, pCreateInfo, pAllocator, pPrivateDataSlot);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreatePrivateDataSlot(device, pCreateInfo, pAllocator, pPrivateDataSlot);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pPrivateDataSlot=" + ptrToString((void*)pPrivateDataSlot) + '!');
#ifdef CREATEPRIVATEDATASLOT_AFTER_EXEC_EXISTS
if(connected) {
layer_CreatePrivateDataSlot_after(device, pCreateInfo, pAllocator, pPrivateDataSlot);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreatePrivateDataSlot!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyPrivateDataSlot!");
}
#ifdef DESTROYPRIVATEDATASLOT_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyPrivateDataSlot_before(device, privateDataSlot, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyPrivateDataSlot(device, privateDataSlot, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"privateDataSlot=" + ptrToString((void*)privateDataSlot) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYPRIVATEDATASLOT_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyPrivateDataSlot_after(device, privateDataSlot, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyPrivateDataSlot!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkSetPrivateData!");
}
#ifdef SETPRIVATEDATA_BEFORE_EXEC_EXISTS
if(connected) {
layer_SetPrivateData_before(device, objectType, objectHandle, privateDataSlot, data);
}
#endif 
auto ret = device_dispatch[GetKey(device)].SetPrivateData(device, objectType, objectHandle, privateDataSlot, data);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"objectType=" + std::to_string(objectType) + '!');
winsockSendToUI(&ConnectSocket,"objectHandle=" + std::to_string(objectHandle) + '!');
winsockSendToUI(&ConnectSocket,"privateDataSlot=" + ptrToString((void*)privateDataSlot) + '!');
winsockSendToUI(&ConnectSocket,"data=" + std::to_string(data) + '!');
#ifdef SETPRIVATEDATA_AFTER_EXEC_EXISTS
if(connected) {
layer_SetPrivateData_after(device, objectType, objectHandle, privateDataSlot, data);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkSetPrivateData!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPrivateData!");
}
#ifdef GETPRIVATEDATA_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPrivateData_before(device, objectType, objectHandle, privateDataSlot, pData);
}
#endif 
device_dispatch[GetKey(device)].GetPrivateData(device, objectType, objectHandle, privateDataSlot, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"objectType=" + std::to_string(objectType) + '!');
winsockSendToUI(&ConnectSocket,"objectHandle=" + std::to_string(objectHandle) + '!');
winsockSendToUI(&ConnectSocket,"privateDataSlot=" + ptrToString((void*)privateDataSlot) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef GETPRIVATEDATA_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPrivateData_after(device, objectType, objectHandle, privateDataSlot, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPrivateData!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyBuffer2(VkCommandBuffer commandBuffer, VkCopyBufferInfo2* pCopyBufferInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyBuffer2!");
}
#ifdef CMDCOPYBUFFER2_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyBuffer2_before(commandBuffer, pCopyBufferInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyBuffer2(commandBuffer, pCopyBufferInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pCopyBufferInfo=" + ptrToString((void*)pCopyBufferInfo) + '!');
#ifdef CMDCOPYBUFFER2_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyBuffer2_after(commandBuffer, pCopyBufferInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyBuffer2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyImage2(VkCommandBuffer commandBuffer, VkCopyImageInfo2* pCopyImageInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyImage2!");
}
#ifdef CMDCOPYIMAGE2_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyImage2_before(commandBuffer, pCopyImageInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyImage2(commandBuffer, pCopyImageInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pCopyImageInfo=" + ptrToString((void*)pCopyImageInfo) + '!');
#ifdef CMDCOPYIMAGE2_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyImage2_after(commandBuffer, pCopyImageInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyImage2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBlitImage2(VkCommandBuffer commandBuffer, VkBlitImageInfo2* pBlitImageInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBlitImage2!");
}
#ifdef CMDBLITIMAGE2_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBlitImage2_before(commandBuffer, pBlitImageInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBlitImage2(commandBuffer, pBlitImageInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pBlitImageInfo=" + ptrToString((void*)pBlitImageInfo) + '!');
#ifdef CMDBLITIMAGE2_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBlitImage2_after(commandBuffer, pBlitImageInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBlitImage2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyBufferToImage2(VkCommandBuffer commandBuffer, VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyBufferToImage2!");
}
#ifdef CMDCOPYBUFFERTOIMAGE2_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyBufferToImage2_before(commandBuffer, pCopyBufferToImageInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyBufferToImage2(commandBuffer, pCopyBufferToImageInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pCopyBufferToImageInfo=" + ptrToString((void*)pCopyBufferToImageInfo) + '!');
#ifdef CMDCOPYBUFFERTOIMAGE2_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyBufferToImage2_after(commandBuffer, pCopyBufferToImageInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyBufferToImage2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyImageToBuffer2!");
}
#ifdef CMDCOPYIMAGETOBUFFER2_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyImageToBuffer2_before(commandBuffer, pCopyImageToBufferInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyImageToBuffer2(commandBuffer, pCopyImageToBufferInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pCopyImageToBufferInfo=" + ptrToString((void*)pCopyImageToBufferInfo) + '!');
#ifdef CMDCOPYIMAGETOBUFFER2_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyImageToBuffer2_after(commandBuffer, pCopyImageToBufferInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyImageToBuffer2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdResolveImage2(VkCommandBuffer commandBuffer, VkResolveImageInfo2* pResolveImageInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdResolveImage2!");
}
#ifdef CMDRESOLVEIMAGE2_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdResolveImage2_before(commandBuffer, pResolveImageInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdResolveImage2(commandBuffer, pResolveImageInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pResolveImageInfo=" + ptrToString((void*)pResolveImageInfo) + '!');
#ifdef CMDRESOLVEIMAGE2_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdResolveImage2_after(commandBuffer, pResolveImageInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdResolveImage2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetFragmentShadingRateKHR(VkCommandBuffer commandBuffer, VkExtent2D* pFragmentSize, VkFragmentShadingRateCombinerOpKHR* combinerOps) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetFragmentShadingRateKHR!");
}
#ifdef CMDSETFRAGMENTSHADINGRATEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetFragmentShadingRateKHR_before(commandBuffer, pFragmentSize, combinerOps);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetFragmentShadingRateKHR(commandBuffer, pFragmentSize, combinerOps);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pFragmentSize=" + ptrToString((void*)pFragmentSize) + '!');
winsockSendToUI(&ConnectSocket,"combinerOps=" + ptrToString((void*)combinerOps) + '!');
#ifdef CMDSETFRAGMENTSHADINGRATEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetFragmentShadingRateKHR_after(commandBuffer, pFragmentSize, combinerOps);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetFragmentShadingRateKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetFragmentShadingRateEnumNV(VkCommandBuffer commandBuffer, VkFragmentShadingRateNV shadingRate, VkFragmentShadingRateCombinerOpKHR* combinerOps) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetFragmentShadingRateEnumNV!");
}
#ifdef CMDSETFRAGMENTSHADINGRATEENUMNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetFragmentShadingRateEnumNV_before(commandBuffer, shadingRate, combinerOps);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetFragmentShadingRateEnumNV(commandBuffer, shadingRate, combinerOps);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"shadingRate=" + std::to_string(shadingRate) + '!');
winsockSendToUI(&ConnectSocket,"combinerOps=" + ptrToString((void*)combinerOps) + '!');
#ifdef CMDSETFRAGMENTSHADINGRATEENUMNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetFragmentShadingRateEnumNV_after(commandBuffer, shadingRate, combinerOps);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetFragmentShadingRateEnumNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetAccelerationStructureBuildSizesKHR(VkDevice device, VkAccelerationStructureBuildTypeKHR buildType, VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo, uint32_t* pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetAccelerationStructureBuildSizesKHR!");
}
#ifdef GETACCELERATIONSTRUCTUREBUILDSIZESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetAccelerationStructureBuildSizesKHR_before(device, buildType, pBuildInfo, pMaxPrimitiveCounts, pSizeInfo);
}
#endif 
device_dispatch[GetKey(device)].GetAccelerationStructureBuildSizesKHR(device, buildType, pBuildInfo, pMaxPrimitiveCounts, pSizeInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"buildType=" + std::to_string(buildType) + '!');
winsockSendToUI(&ConnectSocket,"pBuildInfo=" + ptrToString((void*)pBuildInfo) + '!');
winsockSendToUI(&ConnectSocket,"pMaxPrimitiveCounts=" + ptrToString((void*)pMaxPrimitiveCounts) + '!');
winsockSendToUI(&ConnectSocket,"pSizeInfo=" + ptrToString((void*)pSizeInfo) + '!');
#ifdef GETACCELERATIONSTRUCTUREBUILDSIZESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetAccelerationStructureBuildSizesKHR_after(device, buildType, pBuildInfo, pMaxPrimitiveCounts, pSizeInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetAccelerationStructureBuildSizesKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetVertexInputEXT!");
}
#ifdef CMDSETVERTEXINPUTEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetVertexInputEXT_before(commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetVertexInputEXT(commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"vertexBindingDescriptionCount=" + std::to_string(vertexBindingDescriptionCount) + '!');
winsockSendToUI(&ConnectSocket,"pVertexBindingDescriptions=" + ptrToString((void*)pVertexBindingDescriptions) + '!');
winsockSendToUI(&ConnectSocket,"vertexAttributeDescriptionCount=" + std::to_string(vertexAttributeDescriptionCount) + '!');
winsockSendToUI(&ConnectSocket,"pVertexAttributeDescriptions=" + ptrToString((void*)pVertexAttributeDescriptions) + '!');
#ifdef CMDSETVERTEXINPUTEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetVertexInputEXT_after(commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetVertexInputEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetColorWriteEnableEXT(VkCommandBuffer commandBuffer, uint32_t attachmentCount, VkBool32* pColorWriteEnables) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetColorWriteEnableEXT!");
}
#ifdef CMDSETCOLORWRITEENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetColorWriteEnableEXT_before(commandBuffer, attachmentCount, pColorWriteEnables);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetColorWriteEnableEXT(commandBuffer, attachmentCount, pColorWriteEnables);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"attachmentCount=" + std::to_string(attachmentCount) + '!');
winsockSendToUI(&ConnectSocket,"pColorWriteEnables=" + ptrToString((void*)pColorWriteEnables) + '!');
#ifdef CMDSETCOLORWRITEENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetColorWriteEnableEXT_after(commandBuffer, attachmentCount, pColorWriteEnables);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetColorWriteEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetEvent2(VkCommandBuffer commandBuffer, VkEvent event, VkDependencyInfo* pDependencyInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetEvent2!");
}
#ifdef CMDSETEVENT2_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetEvent2_before(commandBuffer, event, pDependencyInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetEvent2(commandBuffer, event, pDependencyInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"event=" + ptrToString((void*)event) + '!');
winsockSendToUI(&ConnectSocket,"pDependencyInfo=" + ptrToString((void*)pDependencyInfo) + '!');
#ifdef CMDSETEVENT2_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetEvent2_after(commandBuffer, event, pDependencyInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetEvent2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdResetEvent2(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdResetEvent2!");
}
#ifdef CMDRESETEVENT2_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdResetEvent2_before(commandBuffer, event, stageMask);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdResetEvent2(commandBuffer, event, stageMask);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"event=" + ptrToString((void*)event) + '!');
winsockSendToUI(&ConnectSocket,"stageMask=" + ptrToString((void*)stageMask) + '!');
#ifdef CMDRESETEVENT2_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdResetEvent2_after(commandBuffer, event, stageMask);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdResetEvent2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWaitEvents2(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent* pEvents, VkDependencyInfo* pDependencyInfos) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdWaitEvents2!");
}
#ifdef CMDWAITEVENTS2_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdWaitEvents2_before(commandBuffer, eventCount, pEvents, pDependencyInfos);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWaitEvents2(commandBuffer, eventCount, pEvents, pDependencyInfos);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"eventCount=" + std::to_string(eventCount) + '!');
winsockSendToUI(&ConnectSocket,"pEvents=" + ptrToString((void*)pEvents) + '!');
winsockSendToUI(&ConnectSocket,"pDependencyInfos=" + ptrToString((void*)pDependencyInfos) + '!');
#ifdef CMDWAITEVENTS2_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdWaitEvents2_after(commandBuffer, eventCount, pEvents, pDependencyInfos);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdWaitEvents2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPipelineBarrier2(VkCommandBuffer commandBuffer, VkDependencyInfo* pDependencyInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdPipelineBarrier2!");
}
#ifdef CMDPIPELINEBARRIER2_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdPipelineBarrier2_before(commandBuffer, pDependencyInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPipelineBarrier2(commandBuffer, pDependencyInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pDependencyInfo=" + ptrToString((void*)pDependencyInfo) + '!');
#ifdef CMDPIPELINEBARRIER2_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdPipelineBarrier2_after(commandBuffer, pDependencyInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdPipelineBarrier2!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_QueueSubmit2(VkQueue queue, uint32_t submitCount, VkSubmitInfo2* pSubmits, VkFence fence) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkQueueSubmit2!");
}
#ifdef QUEUESUBMIT2_BEFORE_EXEC_EXISTS
if(connected) {
layer_QueueSubmit2_before(queue, submitCount, pSubmits, fence);
}
#endif 
auto ret = device_dispatch[GetKey(queue)].QueueSubmit2(queue, submitCount, pSubmits, fence);
winsockSendToUI(&ConnectSocket,"queue=" + ptrToString((void*)queue) + '!');
winsockSendToUI(&ConnectSocket,"submitCount=" + std::to_string(submitCount) + '!');
winsockSendToUI(&ConnectSocket,"pSubmits=" + ptrToString((void*)pSubmits) + '!');
winsockSendToUI(&ConnectSocket,"fence=" + ptrToString((void*)fence) + '!');
#ifdef QUEUESUBMIT2_AFTER_EXEC_EXISTS
if(connected) {
layer_QueueSubmit2_after(queue, submitCount, pSubmits, fence);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkQueueSubmit2!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWriteTimestamp2(VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdWriteTimestamp2!");
}
#ifdef CMDWRITETIMESTAMP2_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdWriteTimestamp2_before(commandBuffer, stage, queryPool, query);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWriteTimestamp2(commandBuffer, stage, queryPool, query);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"stage=" + ptrToString((void*)stage) + '!');
winsockSendToUI(&ConnectSocket,"queryPool=" + ptrToString((void*)queryPool) + '!');
winsockSendToUI(&ConnectSocket,"query=" + std::to_string(query) + '!');
#ifdef CMDWRITETIMESTAMP2_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdWriteTimestamp2_after(commandBuffer, stage, queryPool, query);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdWriteTimestamp2!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWriteBufferMarker2AMD(VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdWriteBufferMarker2AMD!");
}
#ifdef CMDWRITEBUFFERMARKER2AMD_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdWriteBufferMarker2AMD_before(commandBuffer, stage, dstBuffer, dstOffset, marker);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWriteBufferMarker2AMD(commandBuffer, stage, dstBuffer, dstOffset, marker);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"stage=" + ptrToString((void*)stage) + '!');
winsockSendToUI(&ConnectSocket,"dstBuffer=" + ptrToString((void*)dstBuffer) + '!');
winsockSendToUI(&ConnectSocket,"dstOffset=" + ptrToString((void*)dstOffset) + '!');
winsockSendToUI(&ConnectSocket,"marker=" + std::to_string(marker) + '!');
#ifdef CMDWRITEBUFFERMARKER2AMD_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdWriteBufferMarker2AMD_after(commandBuffer, stage, dstBuffer, dstOffset, marker);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdWriteBufferMarker2AMD!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetQueueCheckpointData2NV!");
}
#ifdef GETQUEUECHECKPOINTDATA2NV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetQueueCheckpointData2NV_before(queue, pCheckpointDataCount, pCheckpointData);
}
#endif 
device_dispatch[GetKey(queue)].GetQueueCheckpointData2NV(queue, pCheckpointDataCount, pCheckpointData);
winsockSendToUI(&ConnectSocket,"queue=" + ptrToString((void*)queue) + '!');
winsockSendToUI(&ConnectSocket,"pCheckpointDataCount=" + ptrToString((void*)pCheckpointDataCount) + '!');
winsockSendToUI(&ConnectSocket,"pCheckpointData=" + ptrToString((void*)pCheckpointData) + '!');
#ifdef GETQUEUECHECKPOINTDATA2NV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetQueueCheckpointData2NV_after(queue, pCheckpointDataCount, pCheckpointData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetQueueCheckpointData2NV!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyMemoryToImageEXT(VkDevice device, VkCopyMemoryToImageInfoEXT* pCopyMemoryToImageInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCopyMemoryToImageEXT!");
}
#ifdef COPYMEMORYTOIMAGEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CopyMemoryToImageEXT_before(device, pCopyMemoryToImageInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CopyMemoryToImageEXT(device, pCopyMemoryToImageInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCopyMemoryToImageInfo=" + ptrToString((void*)pCopyMemoryToImageInfo) + '!');
#ifdef COPYMEMORYTOIMAGEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CopyMemoryToImageEXT_after(device, pCopyMemoryToImageInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCopyMemoryToImageEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyImageToMemoryEXT(VkDevice device, VkCopyImageToMemoryInfoEXT* pCopyImageToMemoryInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCopyImageToMemoryEXT!");
}
#ifdef COPYIMAGETOMEMORYEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CopyImageToMemoryEXT_before(device, pCopyImageToMemoryInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CopyImageToMemoryEXT(device, pCopyImageToMemoryInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCopyImageToMemoryInfo=" + ptrToString((void*)pCopyImageToMemoryInfo) + '!');
#ifdef COPYIMAGETOMEMORYEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CopyImageToMemoryEXT_after(device, pCopyImageToMemoryInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCopyImageToMemoryEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyImageToImageEXT(VkDevice device, VkCopyImageToImageInfoEXT* pCopyImageToImageInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCopyImageToImageEXT!");
}
#ifdef COPYIMAGETOIMAGEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CopyImageToImageEXT_before(device, pCopyImageToImageInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CopyImageToImageEXT(device, pCopyImageToImageInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCopyImageToImageInfo=" + ptrToString((void*)pCopyImageToImageInfo) + '!');
#ifdef COPYIMAGETOIMAGEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CopyImageToImageEXT_after(device, pCopyImageToImageInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCopyImageToImageEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_TransitionImageLayoutEXT(VkDevice device, uint32_t transitionCount, VkHostImageLayoutTransitionInfoEXT* pTransitions) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkTransitionImageLayoutEXT!");
}
#ifdef TRANSITIONIMAGELAYOUTEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_TransitionImageLayoutEXT_before(device, transitionCount, pTransitions);
}
#endif 
auto ret = device_dispatch[GetKey(device)].TransitionImageLayoutEXT(device, transitionCount, pTransitions);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"transitionCount=" + std::to_string(transitionCount) + '!');
winsockSendToUI(&ConnectSocket,"pTransitions=" + ptrToString((void*)pTransitions) + '!');
#ifdef TRANSITIONIMAGELAYOUTEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_TransitionImageLayoutEXT_after(device, transitionCount, pTransitions);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkTransitionImageLayoutEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateVideoSessionKHR(VkDevice device, VkVideoSessionCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateVideoSessionKHR!");
}
#ifdef CREATEVIDEOSESSIONKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateVideoSessionKHR_before(device, pCreateInfo, pAllocator, pVideoSession);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateVideoSessionKHR(device, pCreateInfo, pAllocator, pVideoSession);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pVideoSession=" + ptrToString((void*)pVideoSession) + '!');
#ifdef CREATEVIDEOSESSIONKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateVideoSessionKHR_after(device, pCreateInfo, pAllocator, pVideoSession);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateVideoSessionKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyVideoSessionKHR!");
}
#ifdef DESTROYVIDEOSESSIONKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyVideoSessionKHR_before(device, videoSession, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyVideoSessionKHR(device, videoSession, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"videoSession=" + ptrToString((void*)videoSession) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYVIDEOSESSIONKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyVideoSessionKHR_after(device, videoSession, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyVideoSessionKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateVideoSessionParametersKHR!");
}
#ifdef CREATEVIDEOSESSIONPARAMETERSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateVideoSessionParametersKHR_before(device, pCreateInfo, pAllocator, pVideoSessionParameters);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateVideoSessionParametersKHR(device, pCreateInfo, pAllocator, pVideoSessionParameters);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pVideoSessionParameters=" + ptrToString((void*)pVideoSessionParameters) + '!');
#ifdef CREATEVIDEOSESSIONPARAMETERSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateVideoSessionParametersKHR_after(device, pCreateInfo, pAllocator, pVideoSessionParameters);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateVideoSessionParametersKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_UpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkUpdateVideoSessionParametersKHR!");
}
#ifdef UPDATEVIDEOSESSIONPARAMETERSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_UpdateVideoSessionParametersKHR_before(device, videoSessionParameters, pUpdateInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].UpdateVideoSessionParametersKHR(device, videoSessionParameters, pUpdateInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"videoSessionParameters=" + ptrToString((void*)videoSessionParameters) + '!');
winsockSendToUI(&ConnectSocket,"pUpdateInfo=" + ptrToString((void*)pUpdateInfo) + '!');
#ifdef UPDATEVIDEOSESSIONPARAMETERSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_UpdateVideoSessionParametersKHR_after(device, videoSessionParameters, pUpdateInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkUpdateVideoSessionParametersKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetEncodedVideoSessionParametersKHR(VkDevice device, VkVideoEncodeSessionParametersGetInfoKHR* pVideoSessionParametersInfo, VkVideoEncodeSessionParametersFeedbackInfoKHR* pFeedbackInfo, size_t* pDataSize, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetEncodedVideoSessionParametersKHR!");
}
#ifdef GETENCODEDVIDEOSESSIONPARAMETERSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetEncodedVideoSessionParametersKHR_before(device, pVideoSessionParametersInfo, pFeedbackInfo, pDataSize, pData);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetEncodedVideoSessionParametersKHR(device, pVideoSessionParametersInfo, pFeedbackInfo, pDataSize, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pVideoSessionParametersInfo=" + ptrToString((void*)pVideoSessionParametersInfo) + '!');
winsockSendToUI(&ConnectSocket,"pFeedbackInfo=" + ptrToString((void*)pFeedbackInfo) + '!');
winsockSendToUI(&ConnectSocket,"pDataSize=" + ptrToString((void*)pDataSize) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef GETENCODEDVIDEOSESSIONPARAMETERSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetEncodedVideoSessionParametersKHR_after(device, pVideoSessionParametersInfo, pFeedbackInfo, pDataSize, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetEncodedVideoSessionParametersKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyVideoSessionParametersKHR!");
}
#ifdef DESTROYVIDEOSESSIONPARAMETERSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyVideoSessionParametersKHR_before(device, videoSessionParameters, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyVideoSessionParametersKHR(device, videoSessionParameters, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"videoSessionParameters=" + ptrToString((void*)videoSessionParameters) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYVIDEOSESSIONPARAMETERSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyVideoSessionParametersKHR_after(device, videoSessionParameters, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyVideoSessionParametersKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetVideoSessionMemoryRequirementsKHR!");
}
#ifdef GETVIDEOSESSIONMEMORYREQUIREMENTSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetVideoSessionMemoryRequirementsKHR_before(device, videoSession, pMemoryRequirementsCount, pMemoryRequirements);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetVideoSessionMemoryRequirementsKHR(device, videoSession, pMemoryRequirementsCount, pMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"videoSession=" + ptrToString((void*)videoSession) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRequirementsCount=" + ptrToString((void*)pMemoryRequirementsCount) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRequirements=" + ptrToString((void*)pMemoryRequirements) + '!');
#ifdef GETVIDEOSESSIONMEMORYREQUIREMENTSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetVideoSessionMemoryRequirementsKHR_after(device, videoSession, pMemoryRequirementsCount, pMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetVideoSessionMemoryRequirementsKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkBindVideoSessionMemoryKHR!");
}
#ifdef BINDVIDEOSESSIONMEMORYKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_BindVideoSessionMemoryKHR_before(device, videoSession, bindSessionMemoryInfoCount, pBindSessionMemoryInfos);
}
#endif 
auto ret = device_dispatch[GetKey(device)].BindVideoSessionMemoryKHR(device, videoSession, bindSessionMemoryInfoCount, pBindSessionMemoryInfos);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"videoSession=" + ptrToString((void*)videoSession) + '!');
winsockSendToUI(&ConnectSocket,"bindSessionMemoryInfoCount=" + std::to_string(bindSessionMemoryInfoCount) + '!');
winsockSendToUI(&ConnectSocket,"pBindSessionMemoryInfos=" + ptrToString((void*)pBindSessionMemoryInfos) + '!');
#ifdef BINDVIDEOSESSIONMEMORYKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_BindVideoSessionMemoryKHR_after(device, videoSession, bindSessionMemoryInfoCount, pBindSessionMemoryInfos);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkBindVideoSessionMemoryKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDecodeVideoKHR(VkCommandBuffer commandBuffer, VkVideoDecodeInfoKHR* pDecodeInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDecodeVideoKHR!");
}
#ifdef CMDDECODEVIDEOKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDecodeVideoKHR_before(commandBuffer, pDecodeInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDecodeVideoKHR(commandBuffer, pDecodeInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pDecodeInfo=" + ptrToString((void*)pDecodeInfo) + '!');
#ifdef CMDDECODEVIDEOKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDecodeVideoKHR_after(commandBuffer, pDecodeInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDecodeVideoKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, VkVideoBeginCodingInfoKHR* pBeginInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBeginVideoCodingKHR!");
}
#ifdef CMDBEGINVIDEOCODINGKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBeginVideoCodingKHR_before(commandBuffer, pBeginInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginVideoCodingKHR(commandBuffer, pBeginInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pBeginInfo=" + ptrToString((void*)pBeginInfo) + '!');
#ifdef CMDBEGINVIDEOCODINGKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBeginVideoCodingKHR_after(commandBuffer, pBeginInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBeginVideoCodingKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, VkVideoCodingControlInfoKHR* pCodingControlInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdControlVideoCodingKHR!");
}
#ifdef CMDCONTROLVIDEOCODINGKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdControlVideoCodingKHR_before(commandBuffer, pCodingControlInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdControlVideoCodingKHR(commandBuffer, pCodingControlInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pCodingControlInfo=" + ptrToString((void*)pCodingControlInfo) + '!');
#ifdef CMDCONTROLVIDEOCODINGKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdControlVideoCodingKHR_after(commandBuffer, pCodingControlInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdControlVideoCodingKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, VkVideoEndCodingInfoKHR* pEndCodingInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdEndVideoCodingKHR!");
}
#ifdef CMDENDVIDEOCODINGKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdEndVideoCodingKHR_before(commandBuffer, pEndCodingInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndVideoCodingKHR(commandBuffer, pEndCodingInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pEndCodingInfo=" + ptrToString((void*)pEndCodingInfo) + '!');
#ifdef CMDENDVIDEOCODINGKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdEndVideoCodingKHR_after(commandBuffer, pEndCodingInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdEndVideoCodingKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEncodeVideoKHR(VkCommandBuffer commandBuffer, VkVideoEncodeInfoKHR* pEncodeInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdEncodeVideoKHR!");
}
#ifdef CMDENCODEVIDEOKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdEncodeVideoKHR_before(commandBuffer, pEncodeInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEncodeVideoKHR(commandBuffer, pEncodeInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pEncodeInfo=" + ptrToString((void*)pEncodeInfo) + '!');
#ifdef CMDENCODEVIDEOKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdEncodeVideoKHR_after(commandBuffer, pEncodeInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdEncodeVideoKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, VkDecompressMemoryRegionNV* pDecompressMemoryRegions) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDecompressMemoryNV!");
}
#ifdef CMDDECOMPRESSMEMORYNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDecompressMemoryNV_before(commandBuffer, decompressRegionCount, pDecompressMemoryRegions);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDecompressMemoryNV(commandBuffer, decompressRegionCount, pDecompressMemoryRegions);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"decompressRegionCount=" + std::to_string(decompressRegionCount) + '!');
winsockSendToUI(&ConnectSocket,"pDecompressMemoryRegions=" + ptrToString((void*)pDecompressMemoryRegions) + '!');
#ifdef CMDDECOMPRESSMEMORYNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDecompressMemoryNV_after(commandBuffer, decompressRegionCount, pDecompressMemoryRegions);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDecompressMemoryNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDecompressMemoryIndirectCountNV!");
}
#ifdef CMDDECOMPRESSMEMORYINDIRECTCOUNTNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDecompressMemoryIndirectCountNV_before(commandBuffer, indirectCommandsAddress, indirectCommandsCountAddress, stride);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDecompressMemoryIndirectCountNV(commandBuffer, indirectCommandsAddress, indirectCommandsCountAddress, stride);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"indirectCommandsAddress=" + ptrToString((void*)indirectCommandsAddress) + '!');
winsockSendToUI(&ConnectSocket,"indirectCommandsCountAddress=" + ptrToString((void*)indirectCommandsCountAddress) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef CMDDECOMPRESSMEMORYINDIRECTCOUNTNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDecompressMemoryIndirectCountNV_after(commandBuffer, indirectCommandsAddress, indirectCommandsCountAddress, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDecompressMemoryIndirectCountNV!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateCuModuleNVX(VkDevice device, VkCuModuleCreateInfoNVX* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateCuModuleNVX!");
}
#ifdef CREATECUMODULENVX_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateCuModuleNVX_before(device, pCreateInfo, pAllocator, pModule);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateCuModuleNVX(device, pCreateInfo, pAllocator, pModule);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pModule=" + ptrToString((void*)pModule) + '!');
#ifdef CREATECUMODULENVX_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateCuModuleNVX_after(device, pCreateInfo, pAllocator, pModule);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateCuModuleNVX!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateCuFunctionNVX(VkDevice device, VkCuFunctionCreateInfoNVX* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateCuFunctionNVX!");
}
#ifdef CREATECUFUNCTIONNVX_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateCuFunctionNVX_before(device, pCreateInfo, pAllocator, pFunction);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateCuFunctionNVX(device, pCreateInfo, pAllocator, pFunction);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pFunction=" + ptrToString((void*)pFunction) + '!');
#ifdef CREATECUFUNCTIONNVX_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateCuFunctionNVX_after(device, pCreateInfo, pAllocator, pFunction);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateCuFunctionNVX!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyCuModuleNVX!");
}
#ifdef DESTROYCUMODULENVX_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyCuModuleNVX_before(device, module, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyCuModuleNVX(device, module, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"module=" + ptrToString((void*)module) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYCUMODULENVX_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyCuModuleNVX_after(device, module, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyCuModuleNVX!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyCuFunctionNVX!");
}
#ifdef DESTROYCUFUNCTIONNVX_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyCuFunctionNVX_before(device, function, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyCuFunctionNVX(device, function, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"function=" + ptrToString((void*)function) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYCUFUNCTIONNVX_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyCuFunctionNVX_after(device, function, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyCuFunctionNVX!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, VkCuLaunchInfoNVX* pLaunchInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCuLaunchKernelNVX!");
}
#ifdef CMDCULAUNCHKERNELNVX_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCuLaunchKernelNVX_before(commandBuffer, pLaunchInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCuLaunchKernelNVX(commandBuffer, pLaunchInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pLaunchInfo=" + ptrToString((void*)pLaunchInfo) + '!');
#ifdef CMDCULAUNCHKERNELNVX_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCuLaunchKernelNVX_after(commandBuffer, pLaunchInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCuLaunchKernelNVX!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDescriptorSetLayoutSizeEXT!");
}
#ifdef GETDESCRIPTORSETLAYOUTSIZEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDescriptorSetLayoutSizeEXT_before(device, layout, pLayoutSizeInBytes);
}
#endif 
device_dispatch[GetKey(device)].GetDescriptorSetLayoutSizeEXT(device, layout, pLayoutSizeInBytes);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"layout=" + ptrToString((void*)layout) + '!');
winsockSendToUI(&ConnectSocket,"pLayoutSizeInBytes=" + ptrToString((void*)pLayoutSizeInBytes) + '!');
#ifdef GETDESCRIPTORSETLAYOUTSIZEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDescriptorSetLayoutSizeEXT_after(device, layout, pLayoutSizeInBytes);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDescriptorSetLayoutSizeEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDescriptorSetLayoutBindingOffsetEXT!");
}
#ifdef GETDESCRIPTORSETLAYOUTBINDINGOFFSETEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDescriptorSetLayoutBindingOffsetEXT_before(device, layout, binding, pOffset);
}
#endif 
device_dispatch[GetKey(device)].GetDescriptorSetLayoutBindingOffsetEXT(device, layout, binding, pOffset);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"layout=" + ptrToString((void*)layout) + '!');
winsockSendToUI(&ConnectSocket,"binding=" + std::to_string(binding) + '!');
winsockSendToUI(&ConnectSocket,"pOffset=" + ptrToString((void*)pOffset) + '!');
#ifdef GETDESCRIPTORSETLAYOUTBINDINGOFFSETEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDescriptorSetLayoutBindingOffsetEXT_after(device, layout, binding, pOffset);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDescriptorSetLayoutBindingOffsetEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDescriptorEXT(VkDevice device, VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDescriptorEXT!");
}
#ifdef GETDESCRIPTOREXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDescriptorEXT_before(device, pDescriptorInfo, dataSize, pDescriptor);
}
#endif 
device_dispatch[GetKey(device)].GetDescriptorEXT(device, pDescriptorInfo, dataSize, pDescriptor);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pDescriptorInfo=" + ptrToString((void*)pDescriptorInfo) + '!');
winsockSendToUI(&ConnectSocket,"dataSize=" + std::to_string(dataSize) + '!');
winsockSendToUI(&ConnectSocket,"pDescriptor=" + ptrToString((void*)pDescriptor) + '!');
#ifdef GETDESCRIPTOREXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDescriptorEXT_after(device, pDescriptorInfo, dataSize, pDescriptor);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDescriptorEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, VkDescriptorBufferBindingInfoEXT* pBindingInfos) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBindDescriptorBuffersEXT!");
}
#ifdef CMDBINDDESCRIPTORBUFFERSEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBindDescriptorBuffersEXT_before(commandBuffer, bufferCount, pBindingInfos);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindDescriptorBuffersEXT(commandBuffer, bufferCount, pBindingInfos);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"bufferCount=" + std::to_string(bufferCount) + '!');
winsockSendToUI(&ConnectSocket,"pBindingInfos=" + ptrToString((void*)pBindingInfos) + '!');
#ifdef CMDBINDDESCRIPTORBUFFERSEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBindDescriptorBuffersEXT_after(commandBuffer, bufferCount, pBindingInfos);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBindDescriptorBuffersEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, uint32_t* pBufferIndices, VkDeviceSize* pOffsets) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDescriptorBufferOffsetsEXT!");
}
#ifdef CMDSETDESCRIPTORBUFFEROFFSETSEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDescriptorBufferOffsetsEXT_before(commandBuffer, pipelineBindPoint, layout, firstSet, setCount, pBufferIndices, pOffsets);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDescriptorBufferOffsetsEXT(commandBuffer, pipelineBindPoint, layout, firstSet, setCount, pBufferIndices, pOffsets);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pipelineBindPoint=" + std::to_string(pipelineBindPoint) + '!');
winsockSendToUI(&ConnectSocket,"layout=" + ptrToString((void*)layout) + '!');
winsockSendToUI(&ConnectSocket,"firstSet=" + std::to_string(firstSet) + '!');
winsockSendToUI(&ConnectSocket,"setCount=" + std::to_string(setCount) + '!');
winsockSendToUI(&ConnectSocket,"pBufferIndices=" + ptrToString((void*)pBufferIndices) + '!');
winsockSendToUI(&ConnectSocket,"pOffsets=" + ptrToString((void*)pOffsets) + '!');
#ifdef CMDSETDESCRIPTORBUFFEROFFSETSEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDescriptorBufferOffsetsEXT_after(commandBuffer, pipelineBindPoint, layout, firstSet, setCount, pBufferIndices, pOffsets);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDescriptorBufferOffsetsEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBindDescriptorBufferEmbeddedSamplersEXT!");
}
#ifdef CMDBINDDESCRIPTORBUFFEREMBEDDEDSAMPLERSEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBindDescriptorBufferEmbeddedSamplersEXT_before(commandBuffer, pipelineBindPoint, layout, set);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindDescriptorBufferEmbeddedSamplersEXT(commandBuffer, pipelineBindPoint, layout, set);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pipelineBindPoint=" + std::to_string(pipelineBindPoint) + '!');
winsockSendToUI(&ConnectSocket,"layout=" + ptrToString((void*)layout) + '!');
winsockSendToUI(&ConnectSocket,"set=" + std::to_string(set) + '!');
#ifdef CMDBINDDESCRIPTORBUFFEREMBEDDEDSAMPLERSEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBindDescriptorBufferEmbeddedSamplersEXT_after(commandBuffer, pipelineBindPoint, layout, set);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBindDescriptorBufferEmbeddedSamplersEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetBufferOpaqueCaptureDescriptorDataEXT!");
}
#ifdef GETBUFFEROPAQUECAPTUREDESCRIPTORDATAEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetBufferOpaqueCaptureDescriptorDataEXT_before(device, pInfo, pData);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetBufferOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef GETBUFFEROPAQUECAPTUREDESCRIPTORDATAEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetBufferOpaqueCaptureDescriptorDataEXT_after(device, pInfo, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetBufferOpaqueCaptureDescriptorDataEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetImageOpaqueCaptureDescriptorDataEXT!");
}
#ifdef GETIMAGEOPAQUECAPTUREDESCRIPTORDATAEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetImageOpaqueCaptureDescriptorDataEXT_before(device, pInfo, pData);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetImageOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef GETIMAGEOPAQUECAPTUREDESCRIPTORDATAEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetImageOpaqueCaptureDescriptorDataEXT_after(device, pInfo, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetImageOpaqueCaptureDescriptorDataEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetImageViewOpaqueCaptureDescriptorDataEXT!");
}
#ifdef GETIMAGEVIEWOPAQUECAPTUREDESCRIPTORDATAEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetImageViewOpaqueCaptureDescriptorDataEXT_before(device, pInfo, pData);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetImageViewOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef GETIMAGEVIEWOPAQUECAPTUREDESCRIPTORDATAEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetImageViewOpaqueCaptureDescriptorDataEXT_after(device, pInfo, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetImageViewOpaqueCaptureDescriptorDataEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetSamplerOpaqueCaptureDescriptorDataEXT!");
}
#ifdef GETSAMPLEROPAQUECAPTUREDESCRIPTORDATAEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetSamplerOpaqueCaptureDescriptorDataEXT_before(device, pInfo, pData);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetSamplerOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef GETSAMPLEROPAQUECAPTUREDESCRIPTORDATAEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetSamplerOpaqueCaptureDescriptorDataEXT_after(device, pInfo, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetSamplerOpaqueCaptureDescriptorDataEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT!");
}
#ifdef GETACCELERATIONSTRUCTUREOPAQUECAPTUREDESCRIPTORDATAEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT_before(device, pInfo, pData);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef GETACCELERATIONSTRUCTUREOPAQUECAPTUREDESCRIPTORDATAEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT_after(device, pInfo, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_SetDeviceMemoryPriorityEXT(VkDevice device, VkDeviceMemory memory, float priority) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkSetDeviceMemoryPriorityEXT!");
}
#ifdef SETDEVICEMEMORYPRIORITYEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_SetDeviceMemoryPriorityEXT_before(device, memory, priority);
}
#endif 
device_dispatch[GetKey(device)].SetDeviceMemoryPriorityEXT(device, memory, priority);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"memory=" + ptrToString((void*)memory) + '!');
winsockSendToUI(&ConnectSocket,"priority=" + std::to_string(priority) + '!');
#ifdef SETDEVICEMEMORYPRIORITYEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_SetDeviceMemoryPriorityEXT_after(device, memory, priority);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkSetDeviceMemoryPriorityEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_WaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkWaitForPresentKHR!");
}
#ifdef WAITFORPRESENTKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_WaitForPresentKHR_before(device, swapchain, presentId, timeout);
}
#endif 
auto ret = device_dispatch[GetKey(device)].WaitForPresentKHR(device, swapchain, presentId, timeout);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapchain=" + ptrToString((void*)swapchain) + '!');
winsockSendToUI(&ConnectSocket,"presentId=" + std::to_string(presentId) + '!');
winsockSendToUI(&ConnectSocket,"timeout=" + std::to_string(timeout) + '!');
#ifdef WAITFORPRESENTKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_WaitForPresentKHR_after(device, swapchain, presentId, timeout);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkWaitForPresentKHR!");
}
return ret;
}

#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateBufferCollectionFUCHSIA!");
}
#ifdef CREATEBUFFERCOLLECTIONFUCHSIA_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateBufferCollectionFUCHSIA_before(device, pCreateInfo, pAllocator, pCollection);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateBufferCollectionFUCHSIA(device, pCreateInfo, pAllocator, pCollection);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pCollection=" + ptrToString((void*)pCollection) + '!');
#ifdef CREATEBUFFERCOLLECTIONFUCHSIA_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateBufferCollectionFUCHSIA_after(device, pCreateInfo, pAllocator, pCollection);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateBufferCollectionFUCHSIA!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SetBufferCollectionBufferConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkSetBufferCollectionBufferConstraintsFUCHSIA!");
}
#ifdef SETBUFFERCOLLECTIONBUFFERCONSTRAINTSFUCHSIA_BEFORE_EXEC_EXISTS
if(connected) {
layer_SetBufferCollectionBufferConstraintsFUCHSIA_before(device, collection, pBufferConstraintsInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].SetBufferCollectionBufferConstraintsFUCHSIA(device, collection, pBufferConstraintsInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"collection=" + ptrToString((void*)collection) + '!');
winsockSendToUI(&ConnectSocket,"pBufferConstraintsInfo=" + ptrToString((void*)pBufferConstraintsInfo) + '!');
#ifdef SETBUFFERCOLLECTIONBUFFERCONSTRAINTSFUCHSIA_AFTER_EXEC_EXISTS
if(connected) {
layer_SetBufferCollectionBufferConstraintsFUCHSIA_after(device, collection, pBufferConstraintsInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkSetBufferCollectionBufferConstraintsFUCHSIA!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SetBufferCollectionImageConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkSetBufferCollectionImageConstraintsFUCHSIA!");
}
#ifdef SETBUFFERCOLLECTIONIMAGECONSTRAINTSFUCHSIA_BEFORE_EXEC_EXISTS
if(connected) {
layer_SetBufferCollectionImageConstraintsFUCHSIA_before(device, collection, pImageConstraintsInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].SetBufferCollectionImageConstraintsFUCHSIA(device, collection, pImageConstraintsInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"collection=" + ptrToString((void*)collection) + '!');
winsockSendToUI(&ConnectSocket,"pImageConstraintsInfo=" + ptrToString((void*)pImageConstraintsInfo) + '!');
#ifdef SETBUFFERCOLLECTIONIMAGECONSTRAINTSFUCHSIA_AFTER_EXEC_EXISTS
if(connected) {
layer_SetBufferCollectionImageConstraintsFUCHSIA_after(device, collection, pImageConstraintsInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkSetBufferCollectionImageConstraintsFUCHSIA!");
}
return ret;
}

#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyBufferCollectionFUCHSIA!");
}
#ifdef DESTROYBUFFERCOLLECTIONFUCHSIA_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyBufferCollectionFUCHSIA_before(device, collection, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyBufferCollectionFUCHSIA(device, collection, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"collection=" + ptrToString((void*)collection) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYBUFFERCOLLECTIONFUCHSIA_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyBufferCollectionFUCHSIA_after(device, collection, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyBufferCollectionFUCHSIA!");
}
}

#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetBufferCollectionPropertiesFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetBufferCollectionPropertiesFUCHSIA!");
}
#ifdef GETBUFFERCOLLECTIONPROPERTIESFUCHSIA_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetBufferCollectionPropertiesFUCHSIA_before(device, collection, pProperties);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetBufferCollectionPropertiesFUCHSIA(device, collection, pProperties);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"collection=" + ptrToString((void*)collection) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETBUFFERCOLLECTIONPROPERTIESFUCHSIA_AFTER_EXEC_EXISTS
if(connected) {
layer_GetBufferCollectionPropertiesFUCHSIA_after(device, collection, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetBufferCollectionPropertiesFUCHSIA!");
}
return ret;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateCudaModuleNV(VkDevice device, VkCudaModuleCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCudaModuleNV* pModule) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateCudaModuleNV!");
}
#ifdef CREATECUDAMODULENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateCudaModuleNV_before(device, pCreateInfo, pAllocator, pModule);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateCudaModuleNV(device, pCreateInfo, pAllocator, pModule);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pModule=" + ptrToString((void*)pModule) + '!');
#ifdef CREATECUDAMODULENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateCudaModuleNV_after(device, pCreateInfo, pAllocator, pModule);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateCudaModuleNV!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetCudaModuleCacheNV(VkDevice device, VkCudaModuleNV module, size_t* pCacheSize, void* pCacheData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetCudaModuleCacheNV!");
}
#ifdef GETCUDAMODULECACHENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetCudaModuleCacheNV_before(device, module, pCacheSize, pCacheData);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetCudaModuleCacheNV(device, module, pCacheSize, pCacheData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"module=" + ptrToString((void*)module) + '!');
winsockSendToUI(&ConnectSocket,"pCacheSize=" + ptrToString((void*)pCacheSize) + '!');
winsockSendToUI(&ConnectSocket,"pCacheData=" + ptrToString((void*)pCacheData) + '!');
#ifdef GETCUDAMODULECACHENV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetCudaModuleCacheNV_after(device, module, pCacheSize, pCacheData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetCudaModuleCacheNV!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateCudaFunctionNV(VkDevice device, VkCudaFunctionCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCudaFunctionNV* pFunction) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateCudaFunctionNV!");
}
#ifdef CREATECUDAFUNCTIONNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateCudaFunctionNV_before(device, pCreateInfo, pAllocator, pFunction);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateCudaFunctionNV(device, pCreateInfo, pAllocator, pFunction);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pFunction=" + ptrToString((void*)pFunction) + '!');
#ifdef CREATECUDAFUNCTIONNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateCudaFunctionNV_after(device, pCreateInfo, pAllocator, pFunction);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateCudaFunctionNV!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyCudaModuleNV(VkDevice device, VkCudaModuleNV module, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyCudaModuleNV!");
}
#ifdef DESTROYCUDAMODULENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyCudaModuleNV_before(device, module, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyCudaModuleNV(device, module, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"module=" + ptrToString((void*)module) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYCUDAMODULENV_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyCudaModuleNV_after(device, module, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyCudaModuleNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyCudaFunctionNV(VkDevice device, VkCudaFunctionNV function, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyCudaFunctionNV!");
}
#ifdef DESTROYCUDAFUNCTIONNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyCudaFunctionNV_before(device, function, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyCudaFunctionNV(device, function, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"function=" + ptrToString((void*)function) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYCUDAFUNCTIONNV_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyCudaFunctionNV_after(device, function, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyCudaFunctionNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCudaLaunchKernelNV(VkCommandBuffer commandBuffer, VkCudaLaunchInfoNV* pLaunchInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCudaLaunchKernelNV!");
}
#ifdef CMDCUDALAUNCHKERNELNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCudaLaunchKernelNV_before(commandBuffer, pLaunchInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCudaLaunchKernelNV(commandBuffer, pLaunchInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pLaunchInfo=" + ptrToString((void*)pLaunchInfo) + '!');
#ifdef CMDCUDALAUNCHKERNELNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCudaLaunchKernelNV_after(commandBuffer, pLaunchInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCudaLaunchKernelNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginRendering(VkCommandBuffer commandBuffer, VkRenderingInfo* pRenderingInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBeginRendering!");
}
#ifdef CMDBEGINRENDERING_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBeginRendering_before(commandBuffer, pRenderingInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginRendering(commandBuffer, pRenderingInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pRenderingInfo=" + ptrToString((void*)pRenderingInfo) + '!');
#ifdef CMDBEGINRENDERING_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBeginRendering_after(commandBuffer, pRenderingInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBeginRendering!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndRendering(VkCommandBuffer commandBuffer) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdEndRendering!");
}
#ifdef CMDENDRENDERING_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdEndRendering_before(commandBuffer);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndRendering(commandBuffer);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
#ifdef CMDENDRENDERING_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdEndRendering_after(commandBuffer);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdEndRendering!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDescriptorSetLayoutHostMappingInfoVALVE!");
}
#ifdef GETDESCRIPTORSETLAYOUTHOSTMAPPINGINFOVALVE_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDescriptorSetLayoutHostMappingInfoVALVE_before(device, pBindingReference, pHostMapping);
}
#endif 
device_dispatch[GetKey(device)].GetDescriptorSetLayoutHostMappingInfoVALVE(device, pBindingReference, pHostMapping);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pBindingReference=" + ptrToString((void*)pBindingReference) + '!');
winsockSendToUI(&ConnectSocket,"pHostMapping=" + ptrToString((void*)pHostMapping) + '!');
#ifdef GETDESCRIPTORSETLAYOUTHOSTMAPPINGINFOVALVE_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDescriptorSetLayoutHostMappingInfoVALVE_after(device, pBindingReference, pHostMapping);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDescriptorSetLayoutHostMappingInfoVALVE!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void** ppData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDescriptorSetHostMappingVALVE!");
}
#ifdef GETDESCRIPTORSETHOSTMAPPINGVALVE_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDescriptorSetHostMappingVALVE_before(device, descriptorSet, ppData);
}
#endif 
device_dispatch[GetKey(device)].GetDescriptorSetHostMappingVALVE(device, descriptorSet, ppData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"descriptorSet=" + ptrToString((void*)descriptorSet) + '!');
winsockSendToUI(&ConnectSocket,"ppData=" + ptrToString((void*)ppData) + '!');
#ifdef GETDESCRIPTORSETHOSTMAPPINGVALVE_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDescriptorSetHostMappingVALVE_after(device, descriptorSet, ppData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDescriptorSetHostMappingVALVE!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateMicromapEXT(VkDevice device, VkMicromapCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkMicromapEXT* pMicromap) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateMicromapEXT!");
}
#ifdef CREATEMICROMAPEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateMicromapEXT_before(device, pCreateInfo, pAllocator, pMicromap);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateMicromapEXT(device, pCreateInfo, pAllocator, pMicromap);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pMicromap=" + ptrToString((void*)pMicromap) + '!');
#ifdef CREATEMICROMAPEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateMicromapEXT_after(device, pCreateInfo, pAllocator, pMicromap);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateMicromapEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBuildMicromapsEXT(VkCommandBuffer commandBuffer, uint32_t infoCount, VkMicromapBuildInfoEXT* pInfos) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBuildMicromapsEXT!");
}
#ifdef CMDBUILDMICROMAPSEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBuildMicromapsEXT_before(commandBuffer, infoCount, pInfos);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBuildMicromapsEXT(commandBuffer, infoCount, pInfos);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"infoCount=" + std::to_string(infoCount) + '!');
winsockSendToUI(&ConnectSocket,"pInfos=" + ptrToString((void*)pInfos) + '!');
#ifdef CMDBUILDMICROMAPSEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBuildMicromapsEXT_after(commandBuffer, infoCount, pInfos);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBuildMicromapsEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BuildMicromapsEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, VkMicromapBuildInfoEXT* pInfos) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkBuildMicromapsEXT!");
}
#ifdef BUILDMICROMAPSEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_BuildMicromapsEXT_before(device, deferredOperation, infoCount, pInfos);
}
#endif 
auto ret = device_dispatch[GetKey(device)].BuildMicromapsEXT(device, deferredOperation, infoCount, pInfos);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"deferredOperation=" + ptrToString((void*)deferredOperation) + '!');
winsockSendToUI(&ConnectSocket,"infoCount=" + std::to_string(infoCount) + '!');
winsockSendToUI(&ConnectSocket,"pInfos=" + ptrToString((void*)pInfos) + '!');
#ifdef BUILDMICROMAPSEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_BuildMicromapsEXT_after(device, deferredOperation, infoCount, pInfos);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkBuildMicromapsEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyMicromapEXT!");
}
#ifdef DESTROYMICROMAPEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyMicromapEXT_before(device, micromap, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyMicromapEXT(device, micromap, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"micromap=" + ptrToString((void*)micromap) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYMICROMAPEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyMicromapEXT_after(device, micromap, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyMicromapEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyMicromapEXT(VkCommandBuffer commandBuffer, VkCopyMicromapInfoEXT* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyMicromapEXT!");
}
#ifdef CMDCOPYMICROMAPEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyMicromapEXT_before(commandBuffer, pInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyMicromapEXT(commandBuffer, pInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef CMDCOPYMICROMAPEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyMicromapEXT_after(commandBuffer, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyMicromapEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyMicromapInfoEXT* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCopyMicromapEXT!");
}
#ifdef COPYMICROMAPEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CopyMicromapEXT_before(device, deferredOperation, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CopyMicromapEXT(device, deferredOperation, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"deferredOperation=" + ptrToString((void*)deferredOperation) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef COPYMICROMAPEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CopyMicromapEXT_after(device, deferredOperation, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCopyMicromapEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, VkCopyMicromapToMemoryInfoEXT* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyMicromapToMemoryEXT!");
}
#ifdef CMDCOPYMICROMAPTOMEMORYEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyMicromapToMemoryEXT_before(commandBuffer, pInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyMicromapToMemoryEXT(commandBuffer, pInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef CMDCOPYMICROMAPTOMEMORYEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyMicromapToMemoryEXT_after(commandBuffer, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyMicromapToMemoryEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyMicromapToMemoryInfoEXT* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCopyMicromapToMemoryEXT!");
}
#ifdef COPYMICROMAPTOMEMORYEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CopyMicromapToMemoryEXT_before(device, deferredOperation, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CopyMicromapToMemoryEXT(device, deferredOperation, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"deferredOperation=" + ptrToString((void*)deferredOperation) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef COPYMICROMAPTOMEMORYEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CopyMicromapToMemoryEXT_after(device, deferredOperation, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCopyMicromapToMemoryEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, VkCopyMemoryToMicromapInfoEXT* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyMemoryToMicromapEXT!");
}
#ifdef CMDCOPYMEMORYTOMICROMAPEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyMemoryToMicromapEXT_before(commandBuffer, pInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyMemoryToMicromapEXT(commandBuffer, pInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef CMDCOPYMEMORYTOMICROMAPEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyMemoryToMicromapEXT_after(commandBuffer, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyMemoryToMicromapEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyMemoryToMicromapInfoEXT* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCopyMemoryToMicromapEXT!");
}
#ifdef COPYMEMORYTOMICROMAPEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CopyMemoryToMicromapEXT_before(device, deferredOperation, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CopyMemoryToMicromapEXT(device, deferredOperation, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"deferredOperation=" + ptrToString((void*)deferredOperation) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef COPYMEMORYTOMICROMAPEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CopyMemoryToMicromapEXT_after(device, deferredOperation, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCopyMemoryToMicromapEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdWriteMicromapsPropertiesEXT!");
}
#ifdef CMDWRITEMICROMAPSPROPERTIESEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdWriteMicromapsPropertiesEXT_before(commandBuffer, micromapCount, pMicromaps, queryType, queryPool, firstQuery);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWriteMicromapsPropertiesEXT(commandBuffer, micromapCount, pMicromaps, queryType, queryPool, firstQuery);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"micromapCount=" + std::to_string(micromapCount) + '!');
winsockSendToUI(&ConnectSocket,"pMicromaps=" + ptrToString((void*)pMicromaps) + '!');
winsockSendToUI(&ConnectSocket,"queryType=" + std::to_string(queryType) + '!');
winsockSendToUI(&ConnectSocket,"queryPool=" + ptrToString((void*)queryPool) + '!');
winsockSendToUI(&ConnectSocket,"firstQuery=" + std::to_string(firstQuery) + '!');
#ifdef CMDWRITEMICROMAPSPROPERTIESEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdWriteMicromapsPropertiesEXT_after(commandBuffer, micromapCount, pMicromaps, queryType, queryPool, firstQuery);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdWriteMicromapsPropertiesEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_WriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, VkMicromapEXT* pMicromaps, VkQueryType queryType, size_t dataSize, void* pData, size_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkWriteMicromapsPropertiesEXT!");
}
#ifdef WRITEMICROMAPSPROPERTIESEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_WriteMicromapsPropertiesEXT_before(device, micromapCount, pMicromaps, queryType, dataSize, pData, stride);
}
#endif 
auto ret = device_dispatch[GetKey(device)].WriteMicromapsPropertiesEXT(device, micromapCount, pMicromaps, queryType, dataSize, pData, stride);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"micromapCount=" + std::to_string(micromapCount) + '!');
winsockSendToUI(&ConnectSocket,"pMicromaps=" + ptrToString((void*)pMicromaps) + '!');
winsockSendToUI(&ConnectSocket,"queryType=" + std::to_string(queryType) + '!');
winsockSendToUI(&ConnectSocket,"dataSize=" + std::to_string(dataSize) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef WRITEMICROMAPSPROPERTIESEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_WriteMicromapsPropertiesEXT_after(device, micromapCount, pMicromaps, queryType, dataSize, pData, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkWriteMicromapsPropertiesEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceMicromapCompatibilityEXT(VkDevice device, VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceMicromapCompatibilityEXT!");
}
#ifdef GETDEVICEMICROMAPCOMPATIBILITYEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceMicromapCompatibilityEXT_before(device, pVersionInfo, pCompatibility);
}
#endif 
device_dispatch[GetKey(device)].GetDeviceMicromapCompatibilityEXT(device, pVersionInfo, pCompatibility);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pVersionInfo=" + ptrToString((void*)pVersionInfo) + '!');
winsockSendToUI(&ConnectSocket,"pCompatibility=" + ptrToString((void*)pCompatibility) + '!');
#ifdef GETDEVICEMICROMAPCOMPATIBILITYEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceMicromapCompatibilityEXT_after(device, pVersionInfo, pCompatibility);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceMicromapCompatibilityEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetMicromapBuildSizesEXT(VkDevice device, VkAccelerationStructureBuildTypeKHR buildType, VkMicromapBuildInfoEXT* pBuildInfo, VkMicromapBuildSizesInfoEXT* pSizeInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetMicromapBuildSizesEXT!");
}
#ifdef GETMICROMAPBUILDSIZESEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetMicromapBuildSizesEXT_before(device, buildType, pBuildInfo, pSizeInfo);
}
#endif 
device_dispatch[GetKey(device)].GetMicromapBuildSizesEXT(device, buildType, pBuildInfo, pSizeInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"buildType=" + std::to_string(buildType) + '!');
winsockSendToUI(&ConnectSocket,"pBuildInfo=" + ptrToString((void*)pBuildInfo) + '!');
winsockSendToUI(&ConnectSocket,"pSizeInfo=" + ptrToString((void*)pSizeInfo) + '!');
#ifdef GETMICROMAPBUILDSIZESEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetMicromapBuildSizesEXT_after(device, buildType, pBuildInfo, pSizeInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetMicromapBuildSizesEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetShaderModuleIdentifierEXT!");
}
#ifdef GETSHADERMODULEIDENTIFIEREXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetShaderModuleIdentifierEXT_before(device, shaderModule, pIdentifier);
}
#endif 
device_dispatch[GetKey(device)].GetShaderModuleIdentifierEXT(device, shaderModule, pIdentifier);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"shaderModule=" + ptrToString((void*)shaderModule) + '!');
winsockSendToUI(&ConnectSocket,"pIdentifier=" + ptrToString((void*)pIdentifier) + '!');
#ifdef GETSHADERMODULEIDENTIFIEREXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetShaderModuleIdentifierEXT_after(device, shaderModule, pIdentifier);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetShaderModuleIdentifierEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetShaderModuleCreateInfoIdentifierEXT(VkDevice device, VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetShaderModuleCreateInfoIdentifierEXT!");
}
#ifdef GETSHADERMODULECREATEINFOIDENTIFIEREXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetShaderModuleCreateInfoIdentifierEXT_before(device, pCreateInfo, pIdentifier);
}
#endif 
device_dispatch[GetKey(device)].GetShaderModuleCreateInfoIdentifierEXT(device, pCreateInfo, pIdentifier);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pIdentifier=" + ptrToString((void*)pIdentifier) + '!');
#ifdef GETSHADERMODULECREATEINFOIDENTIFIEREXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetShaderModuleCreateInfoIdentifierEXT_after(device, pCreateInfo, pIdentifier);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetShaderModuleCreateInfoIdentifierEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageSubresourceLayout2KHR(VkDevice device, VkImage image, VkImageSubresource2KHR* pSubresource, VkSubresourceLayout2KHR* pLayout) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetImageSubresourceLayout2KHR!");
}
#ifdef GETIMAGESUBRESOURCELAYOUT2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetImageSubresourceLayout2KHR_before(device, image, pSubresource, pLayout);
}
#endif 
device_dispatch[GetKey(device)].GetImageSubresourceLayout2KHR(device, image, pSubresource, pLayout);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"image=" + ptrToString((void*)image) + '!');
winsockSendToUI(&ConnectSocket,"pSubresource=" + ptrToString((void*)pSubresource) + '!');
winsockSendToUI(&ConnectSocket,"pLayout=" + ptrToString((void*)pLayout) + '!');
#ifdef GETIMAGESUBRESOURCELAYOUT2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetImageSubresourceLayout2KHR_after(device, image, pSubresource, pLayout);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetImageSubresourceLayout2KHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPipelinePropertiesEXT(VkDevice device, VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPipelinePropertiesEXT!");
}
#ifdef GETPIPELINEPROPERTIESEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPipelinePropertiesEXT_before(device, pPipelineInfo, pPipelineProperties);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetPipelinePropertiesEXT(device, pPipelineInfo, pPipelineProperties);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pPipelineInfo=" + ptrToString((void*)pPipelineInfo) + '!');
winsockSendToUI(&ConnectSocket,"pPipelineProperties=" + ptrToString((void*)pPipelineProperties) + '!');
#ifdef GETPIPELINEPROPERTIESEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPipelinePropertiesEXT_after(device, pPipelineInfo, pPipelineProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPipelinePropertiesEXT!");
}
return ret;
}

#if defined(VK_USE_PLATFORM_METAL_EXT)
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_ExportMetalObjectsEXT(VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkExportMetalObjectsEXT!");
}
#ifdef EXPORTMETALOBJECTSEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_ExportMetalObjectsEXT_before(device, pMetalObjectsInfo);
}
#endif 
device_dispatch[GetKey(device)].ExportMetalObjectsEXT(device, pMetalObjectsInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pMetalObjectsInfo=" + ptrToString((void*)pMetalObjectsInfo) + '!');
#ifdef EXPORTMETALOBJECTSEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_ExportMetalObjectsEXT_after(device, pMetalObjectsInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkExportMetalObjectsEXT!");
}
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetFramebufferTilePropertiesQCOM!");
}
#ifdef GETFRAMEBUFFERTILEPROPERTIESQCOM_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetFramebufferTilePropertiesQCOM_before(device, framebuffer, pPropertiesCount, pProperties);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetFramebufferTilePropertiesQCOM(device, framebuffer, pPropertiesCount, pProperties);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"framebuffer=" + ptrToString((void*)framebuffer) + '!');
winsockSendToUI(&ConnectSocket,"pPropertiesCount=" + ptrToString((void*)pPropertiesCount) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETFRAMEBUFFERTILEPROPERTIESQCOM_AFTER_EXEC_EXISTS
if(connected) {
layer_GetFramebufferTilePropertiesQCOM_after(device, framebuffer, pPropertiesCount, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetFramebufferTilePropertiesQCOM!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDynamicRenderingTilePropertiesQCOM(VkDevice device, VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDynamicRenderingTilePropertiesQCOM!");
}
#ifdef GETDYNAMICRENDERINGTILEPROPERTIESQCOM_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDynamicRenderingTilePropertiesQCOM_before(device, pRenderingInfo, pProperties);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetDynamicRenderingTilePropertiesQCOM(device, pRenderingInfo, pProperties);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pRenderingInfo=" + ptrToString((void*)pRenderingInfo) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETDYNAMICRENDERINGTILEPROPERTIESQCOM_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDynamicRenderingTilePropertiesQCOM_after(device, pRenderingInfo, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDynamicRenderingTilePropertiesQCOM!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateOpticalFlowSessionNV!");
}
#ifdef CREATEOPTICALFLOWSESSIONNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateOpticalFlowSessionNV_before(device, pCreateInfo, pAllocator, pSession);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateOpticalFlowSessionNV(device, pCreateInfo, pAllocator, pSession);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pSession=" + ptrToString((void*)pSession) + '!');
#ifdef CREATEOPTICALFLOWSESSIONNV_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateOpticalFlowSessionNV_after(device, pCreateInfo, pAllocator, pSession);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateOpticalFlowSessionNV!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyOpticalFlowSessionNV!");
}
#ifdef DESTROYOPTICALFLOWSESSIONNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyOpticalFlowSessionNV_before(device, session, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyOpticalFlowSessionNV(device, session, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"session=" + ptrToString((void*)session) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYOPTICALFLOWSESSIONNV_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyOpticalFlowSessionNV_after(device, session, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyOpticalFlowSessionNV!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkBindOpticalFlowSessionImageNV!");
}
#ifdef BINDOPTICALFLOWSESSIONIMAGENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_BindOpticalFlowSessionImageNV_before(device, session, bindingPoint, view, layout);
}
#endif 
auto ret = device_dispatch[GetKey(device)].BindOpticalFlowSessionImageNV(device, session, bindingPoint, view, layout);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"session=" + ptrToString((void*)session) + '!');
winsockSendToUI(&ConnectSocket,"bindingPoint=" + std::to_string(bindingPoint) + '!');
winsockSendToUI(&ConnectSocket,"view=" + ptrToString((void*)view) + '!');
winsockSendToUI(&ConnectSocket,"layout=" + std::to_string(layout) + '!');
#ifdef BINDOPTICALFLOWSESSIONIMAGENV_AFTER_EXEC_EXISTS
if(connected) {
layer_BindOpticalFlowSessionImageNV_after(device, session, bindingPoint, view, layout);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkBindOpticalFlowSessionImageNV!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, VkOpticalFlowExecuteInfoNV* pExecuteInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdOpticalFlowExecuteNV!");
}
#ifdef CMDOPTICALFLOWEXECUTENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdOpticalFlowExecuteNV_before(commandBuffer, session, pExecuteInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdOpticalFlowExecuteNV(commandBuffer, session, pExecuteInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"session=" + ptrToString((void*)session) + '!');
winsockSendToUI(&ConnectSocket,"pExecuteInfo=" + ptrToString((void*)pExecuteInfo) + '!');
#ifdef CMDOPTICALFLOWEXECUTENV_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdOpticalFlowExecuteNV_after(commandBuffer, session, pExecuteInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdOpticalFlowExecuteNV!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceFaultInfoEXT!");
}
#ifdef GETDEVICEFAULTINFOEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceFaultInfoEXT_before(device, pFaultCounts, pFaultInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetDeviceFaultInfoEXT(device, pFaultCounts, pFaultInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pFaultCounts=" + ptrToString((void*)pFaultCounts) + '!');
winsockSendToUI(&ConnectSocket,"pFaultInfo=" + ptrToString((void*)pFaultInfo) + '!');
#ifdef GETDEVICEFAULTINFOEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceFaultInfoEXT_after(device, pFaultCounts, pFaultInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceFaultInfoEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthBias2EXT(VkCommandBuffer commandBuffer, VkDepthBiasInfoEXT* pDepthBiasInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDepthBias2EXT!");
}
#ifdef CMDSETDEPTHBIAS2EXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthBias2EXT_before(commandBuffer, pDepthBiasInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthBias2EXT(commandBuffer, pDepthBiasInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pDepthBiasInfo=" + ptrToString((void*)pDepthBiasInfo) + '!');
#ifdef CMDSETDEPTHBIAS2EXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthBias2EXT_after(commandBuffer, pDepthBiasInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDepthBias2EXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ReleaseSwapchainImagesEXT(VkDevice device, VkReleaseSwapchainImagesInfoEXT* pReleaseInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkReleaseSwapchainImagesEXT!");
}
#ifdef RELEASESWAPCHAINIMAGESEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_ReleaseSwapchainImagesEXT_before(device, pReleaseInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].ReleaseSwapchainImagesEXT(device, pReleaseInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pReleaseInfo=" + ptrToString((void*)pReleaseInfo) + '!');
#ifdef RELEASESWAPCHAINIMAGESEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_ReleaseSwapchainImagesEXT_after(device, pReleaseInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkReleaseSwapchainImagesEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceImageSubresourceLayoutKHR(VkDevice device, VkDeviceImageSubresourceInfoKHR* pInfo, VkSubresourceLayout2KHR* pLayout) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceImageSubresourceLayoutKHR!");
}
#ifdef GETDEVICEIMAGESUBRESOURCELAYOUTKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceImageSubresourceLayoutKHR_before(device, pInfo, pLayout);
}
#endif 
device_dispatch[GetKey(device)].GetDeviceImageSubresourceLayoutKHR(device, pInfo, pLayout);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pLayout=" + ptrToString((void*)pLayout) + '!');
#ifdef GETDEVICEIMAGESUBRESOURCELAYOUTKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceImageSubresourceLayoutKHR_after(device, pInfo, pLayout);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceImageSubresourceLayoutKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_MapMemory2KHR(VkDevice device, VkMemoryMapInfoKHR* pMemoryMapInfo, void** ppData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkMapMemory2KHR!");
}
#ifdef MAPMEMORY2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_MapMemory2KHR_before(device, pMemoryMapInfo, ppData);
}
#endif 
auto ret = device_dispatch[GetKey(device)].MapMemory2KHR(device, pMemoryMapInfo, ppData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryMapInfo=" + ptrToString((void*)pMemoryMapInfo) + '!');
winsockSendToUI(&ConnectSocket,"ppData=" + ptrToString((void*)ppData) + '!');
#ifdef MAPMEMORY2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_MapMemory2KHR_after(device, pMemoryMapInfo, ppData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkMapMemory2KHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_UnmapMemory2KHR(VkDevice device, VkMemoryUnmapInfoKHR* pMemoryUnmapInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkUnmapMemory2KHR!");
}
#ifdef UNMAPMEMORY2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_UnmapMemory2KHR_before(device, pMemoryUnmapInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].UnmapMemory2KHR(device, pMemoryUnmapInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryUnmapInfo=" + ptrToString((void*)pMemoryUnmapInfo) + '!');
#ifdef UNMAPMEMORY2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_UnmapMemory2KHR_after(device, pMemoryUnmapInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkUnmapMemory2KHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateShadersEXT(VkDevice device, uint32_t createInfoCount, VkShaderCreateInfoEXT* pCreateInfos, VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateShadersEXT!");
}
#ifdef CREATESHADERSEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateShadersEXT_before(device, createInfoCount, pCreateInfos, pAllocator, pShaders);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateShadersEXT(device, createInfoCount, pCreateInfos, pAllocator, pShaders);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"createInfoCount=" + std::to_string(createInfoCount) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfos=" + ptrToString((void*)pCreateInfos) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pShaders=" + ptrToString((void*)pShaders) + '!');
#ifdef CREATESHADERSEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateShadersEXT_after(device, createInfoCount, pCreateInfos, pAllocator, pShaders);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateShadersEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyShaderEXT(VkDevice device, VkShaderEXT shader, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyShaderEXT!");
}
#ifdef DESTROYSHADEREXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyShaderEXT_before(device, shader, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyShaderEXT(device, shader, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"shader=" + ptrToString((void*)shader) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYSHADEREXT_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyShaderEXT_after(device, shader, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyShaderEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetShaderBinaryDataEXT!");
}
#ifdef GETSHADERBINARYDATAEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetShaderBinaryDataEXT_before(device, shader, pDataSize, pData);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetShaderBinaryDataEXT(device, shader, pDataSize, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"shader=" + ptrToString((void*)shader) + '!');
winsockSendToUI(&ConnectSocket,"pDataSize=" + ptrToString((void*)pDataSize) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef GETSHADERBINARYDATAEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetShaderBinaryDataEXT_after(device, shader, pDataSize, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetShaderBinaryDataEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, VkShaderStageFlagBits* pStages, VkShaderEXT* pShaders) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBindShadersEXT!");
}
#ifdef CMDBINDSHADERSEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBindShadersEXT_before(commandBuffer, stageCount, pStages, pShaders);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindShadersEXT(commandBuffer, stageCount, pStages, pShaders);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"stageCount=" + std::to_string(stageCount) + '!');
winsockSendToUI(&ConnectSocket,"pStages=" + ptrToString((void*)pStages) + '!');
winsockSendToUI(&ConnectSocket,"pShaders=" + ptrToString((void*)pShaders) + '!');
#ifdef CMDBINDSHADERSEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBindShadersEXT_after(commandBuffer, stageCount, pStages, pShaders);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBindShadersEXT!");
}
}

#if defined(VK_USE_PLATFORM_SCREEN_QNX)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetScreenBufferPropertiesQNX(VkDevice device, _screen_buffer* buffer, VkScreenBufferPropertiesQNX* pProperties) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetScreenBufferPropertiesQNX!");
}
#ifdef GETSCREENBUFFERPROPERTIESQNX_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetScreenBufferPropertiesQNX_before(device, buffer, pProperties);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetScreenBufferPropertiesQNX(device, buffer, pProperties);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"pProperties=" + ptrToString((void*)pProperties) + '!');
#ifdef GETSCREENBUFFERPROPERTIESQNX_AFTER_EXEC_EXISTS
if(connected) {
layer_GetScreenBufferPropertiesQNX_after(device, buffer, pProperties);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetScreenBufferPropertiesQNX!");
}
return ret;
}

#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetExecutionGraphPipelineScratchSizeAMDX(VkDevice device, VkPipeline executionGraph, VkExecutionGraphPipelineScratchSizeAMDX* pSizeInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetExecutionGraphPipelineScratchSizeAMDX!");
}
#ifdef GETEXECUTIONGRAPHPIPELINESCRATCHSIZEAMDX_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetExecutionGraphPipelineScratchSizeAMDX_before(device, executionGraph, pSizeInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetExecutionGraphPipelineScratchSizeAMDX(device, executionGraph, pSizeInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"executionGraph=" + ptrToString((void*)executionGraph) + '!');
winsockSendToUI(&ConnectSocket,"pSizeInfo=" + ptrToString((void*)pSizeInfo) + '!');
#ifdef GETEXECUTIONGRAPHPIPELINESCRATCHSIZEAMDX_AFTER_EXEC_EXISTS
if(connected) {
layer_GetExecutionGraphPipelineScratchSizeAMDX_after(device, executionGraph, pSizeInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetExecutionGraphPipelineScratchSizeAMDX!");
}
return ret;
}

#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetExecutionGraphPipelineNodeIndexAMDX(VkDevice device, VkPipeline executionGraph, VkPipelineShaderStageNodeCreateInfoAMDX* pNodeInfo, uint32_t* pNodeIndex) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetExecutionGraphPipelineNodeIndexAMDX!");
}
#ifdef GETEXECUTIONGRAPHPIPELINENODEINDEXAMDX_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetExecutionGraphPipelineNodeIndexAMDX_before(device, executionGraph, pNodeInfo, pNodeIndex);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetExecutionGraphPipelineNodeIndexAMDX(device, executionGraph, pNodeInfo, pNodeIndex);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"executionGraph=" + ptrToString((void*)executionGraph) + '!');
winsockSendToUI(&ConnectSocket,"pNodeInfo=" + ptrToString((void*)pNodeInfo) + '!');
winsockSendToUI(&ConnectSocket,"pNodeIndex=" + ptrToString((void*)pNodeIndex) + '!');
#ifdef GETEXECUTIONGRAPHPIPELINENODEINDEXAMDX_AFTER_EXEC_EXISTS
if(connected) {
layer_GetExecutionGraphPipelineNodeIndexAMDX_after(device, executionGraph, pNodeInfo, pNodeIndex);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetExecutionGraphPipelineNodeIndexAMDX!");
}
return ret;
}

#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateExecutionGraphPipelinesAMDX(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkExecutionGraphPipelineCreateInfoAMDX* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateExecutionGraphPipelinesAMDX!");
}
#ifdef CREATEEXECUTIONGRAPHPIPELINESAMDX_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateExecutionGraphPipelinesAMDX_before(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateExecutionGraphPipelinesAMDX(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pipelineCache=" + ptrToString((void*)pipelineCache) + '!');
winsockSendToUI(&ConnectSocket,"createInfoCount=" + std::to_string(createInfoCount) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfos=" + ptrToString((void*)pCreateInfos) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pPipelines=" + ptrToString((void*)pPipelines) + '!');
#ifdef CREATEEXECUTIONGRAPHPIPELINESAMDX_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateExecutionGraphPipelinesAMDX_after(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateExecutionGraphPipelinesAMDX!");
}
return ret;
}

#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdInitializeGraphScratchMemoryAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdInitializeGraphScratchMemoryAMDX!");
}
#ifdef CMDINITIALIZEGRAPHSCRATCHMEMORYAMDX_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdInitializeGraphScratchMemoryAMDX_before(commandBuffer, scratch);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdInitializeGraphScratchMemoryAMDX(commandBuffer, scratch);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"scratch=" + ptrToString((void*)scratch) + '!');
#ifdef CMDINITIALIZEGRAPHSCRATCHMEMORYAMDX_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdInitializeGraphScratchMemoryAMDX_after(commandBuffer, scratch);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdInitializeGraphScratchMemoryAMDX!");
}
}

#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDispatchGraphAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDispatchGraphCountInfoAMDX* pCountInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDispatchGraphAMDX!");
}
#ifdef CMDDISPATCHGRAPHAMDX_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDispatchGraphAMDX_before(commandBuffer, scratch, pCountInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDispatchGraphAMDX(commandBuffer, scratch, pCountInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"scratch=" + ptrToString((void*)scratch) + '!');
winsockSendToUI(&ConnectSocket,"pCountInfo=" + ptrToString((void*)pCountInfo) + '!');
#ifdef CMDDISPATCHGRAPHAMDX_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDispatchGraphAMDX_after(commandBuffer, scratch, pCountInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDispatchGraphAMDX!");
}
}

#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDispatchGraphIndirectAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDispatchGraphCountInfoAMDX* pCountInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDispatchGraphIndirectAMDX!");
}
#ifdef CMDDISPATCHGRAPHINDIRECTAMDX_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDispatchGraphIndirectAMDX_before(commandBuffer, scratch, pCountInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDispatchGraphIndirectAMDX(commandBuffer, scratch, pCountInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"scratch=" + ptrToString((void*)scratch) + '!');
winsockSendToUI(&ConnectSocket,"pCountInfo=" + ptrToString((void*)pCountInfo) + '!');
#ifdef CMDDISPATCHGRAPHINDIRECTAMDX_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDispatchGraphIndirectAMDX_after(commandBuffer, scratch, pCountInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDispatchGraphIndirectAMDX!");
}
}

#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDispatchGraphIndirectCountAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDeviceAddress countInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDispatchGraphIndirectCountAMDX!");
}
#ifdef CMDDISPATCHGRAPHINDIRECTCOUNTAMDX_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDispatchGraphIndirectCountAMDX_before(commandBuffer, scratch, countInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDispatchGraphIndirectCountAMDX(commandBuffer, scratch, countInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"scratch=" + ptrToString((void*)scratch) + '!');
winsockSendToUI(&ConnectSocket,"countInfo=" + ptrToString((void*)countInfo) + '!');
#ifdef CMDDISPATCHGRAPHINDIRECTCOUNTAMDX_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDispatchGraphIndirectCountAMDX_after(commandBuffer, scratch, countInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDispatchGraphIndirectCountAMDX!");
}
}

#endif
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindDescriptorSets2KHR(VkCommandBuffer commandBuffer, VkBindDescriptorSetsInfoKHR* pBindDescriptorSetsInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBindDescriptorSets2KHR!");
}
#ifdef CMDBINDDESCRIPTORSETS2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBindDescriptorSets2KHR_before(commandBuffer, pBindDescriptorSetsInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindDescriptorSets2KHR(commandBuffer, pBindDescriptorSetsInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pBindDescriptorSetsInfo=" + ptrToString((void*)pBindDescriptorSetsInfo) + '!');
#ifdef CMDBINDDESCRIPTORSETS2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBindDescriptorSets2KHR_after(commandBuffer, pBindDescriptorSetsInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBindDescriptorSets2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPushConstants2KHR(VkCommandBuffer commandBuffer, VkPushConstantsInfoKHR* pPushConstantsInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdPushConstants2KHR!");
}
#ifdef CMDPUSHCONSTANTS2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdPushConstants2KHR_before(commandBuffer, pPushConstantsInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPushConstants2KHR(commandBuffer, pPushConstantsInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pPushConstantsInfo=" + ptrToString((void*)pPushConstantsInfo) + '!');
#ifdef CMDPUSHCONSTANTS2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdPushConstants2KHR_after(commandBuffer, pPushConstantsInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdPushConstants2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPushDescriptorSet2KHR(VkCommandBuffer commandBuffer, VkPushDescriptorSetInfoKHR* pPushDescriptorSetInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdPushDescriptorSet2KHR!");
}
#ifdef CMDPUSHDESCRIPTORSET2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdPushDescriptorSet2KHR_before(commandBuffer, pPushDescriptorSetInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPushDescriptorSet2KHR(commandBuffer, pPushDescriptorSetInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pPushDescriptorSetInfo=" + ptrToString((void*)pPushDescriptorSetInfo) + '!');
#ifdef CMDPUSHDESCRIPTORSET2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdPushDescriptorSet2KHR_after(commandBuffer, pPushDescriptorSetInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdPushDescriptorSet2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPushDescriptorSetWithTemplate2KHR(VkCommandBuffer commandBuffer, VkPushDescriptorSetWithTemplateInfoKHR* pPushDescriptorSetWithTemplateInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdPushDescriptorSetWithTemplate2KHR!");
}
#ifdef CMDPUSHDESCRIPTORSETWITHTEMPLATE2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdPushDescriptorSetWithTemplate2KHR_before(commandBuffer, pPushDescriptorSetWithTemplateInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPushDescriptorSetWithTemplate2KHR(commandBuffer, pPushDescriptorSetWithTemplateInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pPushDescriptorSetWithTemplateInfo=" + ptrToString((void*)pPushDescriptorSetWithTemplateInfo) + '!');
#ifdef CMDPUSHDESCRIPTORSETWITHTEMPLATE2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdPushDescriptorSetWithTemplate2KHR_after(commandBuffer, pPushDescriptorSetWithTemplateInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdPushDescriptorSetWithTemplate2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDescriptorBufferOffsets2EXT(VkCommandBuffer commandBuffer, VkSetDescriptorBufferOffsetsInfoEXT* pSetDescriptorBufferOffsetsInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDescriptorBufferOffsets2EXT!");
}
#ifdef CMDSETDESCRIPTORBUFFEROFFSETS2EXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDescriptorBufferOffsets2EXT_before(commandBuffer, pSetDescriptorBufferOffsetsInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDescriptorBufferOffsets2EXT(commandBuffer, pSetDescriptorBufferOffsetsInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pSetDescriptorBufferOffsetsInfo=" + ptrToString((void*)pSetDescriptorBufferOffsetsInfo) + '!');
#ifdef CMDSETDESCRIPTORBUFFEROFFSETS2EXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDescriptorBufferOffsets2EXT_after(commandBuffer, pSetDescriptorBufferOffsetsInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDescriptorBufferOffsets2EXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindDescriptorBufferEmbeddedSamplers2EXT(VkCommandBuffer commandBuffer, VkBindDescriptorBufferEmbeddedSamplersInfoEXT* pBindDescriptorBufferEmbeddedSamplersInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT!");
}
#ifdef CMDBINDDESCRIPTORBUFFEREMBEDDEDSAMPLERS2EXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBindDescriptorBufferEmbeddedSamplers2EXT_before(commandBuffer, pBindDescriptorBufferEmbeddedSamplersInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindDescriptorBufferEmbeddedSamplers2EXT(commandBuffer, pBindDescriptorBufferEmbeddedSamplersInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pBindDescriptorBufferEmbeddedSamplersInfo=" + ptrToString((void*)pBindDescriptorBufferEmbeddedSamplersInfo) + '!');
#ifdef CMDBINDDESCRIPTORBUFFEREMBEDDEDSAMPLERS2EXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBindDescriptorBufferEmbeddedSamplers2EXT_after(commandBuffer, pBindDescriptorBufferEmbeddedSamplersInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SetLatencySleepModeNV(VkDevice device, VkSwapchainKHR swapchain, VkLatencySleepModeInfoNV* pSleepModeInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkSetLatencySleepModeNV!");
}
#ifdef SETLATENCYSLEEPMODENV_BEFORE_EXEC_EXISTS
if(connected) {
layer_SetLatencySleepModeNV_before(device, swapchain, pSleepModeInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].SetLatencySleepModeNV(device, swapchain, pSleepModeInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapchain=" + ptrToString((void*)swapchain) + '!');
winsockSendToUI(&ConnectSocket,"pSleepModeInfo=" + ptrToString((void*)pSleepModeInfo) + '!');
#ifdef SETLATENCYSLEEPMODENV_AFTER_EXEC_EXISTS
if(connected) {
layer_SetLatencySleepModeNV_after(device, swapchain, pSleepModeInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkSetLatencySleepModeNV!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_LatencySleepNV(VkDevice device, VkSwapchainKHR swapchain, VkLatencySleepInfoNV* pSleepInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkLatencySleepNV!");
}
#ifdef LATENCYSLEEPNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_LatencySleepNV_before(device, swapchain, pSleepInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].LatencySleepNV(device, swapchain, pSleepInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapchain=" + ptrToString((void*)swapchain) + '!');
winsockSendToUI(&ConnectSocket,"pSleepInfo=" + ptrToString((void*)pSleepInfo) + '!');
#ifdef LATENCYSLEEPNV_AFTER_EXEC_EXISTS
if(connected) {
layer_LatencySleepNV_after(device, swapchain, pSleepInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkLatencySleepNV!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_SetLatencyMarkerNV(VkDevice device, VkSwapchainKHR swapchain, VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkSetLatencyMarkerNV!");
}
#ifdef SETLATENCYMARKERNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_SetLatencyMarkerNV_before(device, swapchain, pLatencyMarkerInfo);
}
#endif 
device_dispatch[GetKey(device)].SetLatencyMarkerNV(device, swapchain, pLatencyMarkerInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapchain=" + ptrToString((void*)swapchain) + '!');
winsockSendToUI(&ConnectSocket,"pLatencyMarkerInfo=" + ptrToString((void*)pLatencyMarkerInfo) + '!');
#ifdef SETLATENCYMARKERNV_AFTER_EXEC_EXISTS
if(connected) {
layer_SetLatencyMarkerNV_after(device, swapchain, pLatencyMarkerInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkSetLatencyMarkerNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetLatencyTimingsNV(VkDevice device, VkSwapchainKHR swapchain, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetLatencyTimingsNV!");
}
#ifdef GETLATENCYTIMINGSNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetLatencyTimingsNV_before(device, swapchain, pLatencyMarkerInfo);
}
#endif 
device_dispatch[GetKey(device)].GetLatencyTimingsNV(device, swapchain, pLatencyMarkerInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"swapchain=" + ptrToString((void*)swapchain) + '!');
winsockSendToUI(&ConnectSocket,"pLatencyMarkerInfo=" + ptrToString((void*)pLatencyMarkerInfo) + '!');
#ifdef GETLATENCYTIMINGSNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetLatencyTimingsNV_after(device, swapchain, pLatencyMarkerInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetLatencyTimingsNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_QueueNotifyOutOfBandNV(VkQueue queue, VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkQueueNotifyOutOfBandNV!");
}
#ifdef QUEUENOTIFYOUTOFBANDNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_QueueNotifyOutOfBandNV_before(queue, pQueueTypeInfo);
}
#endif 
device_dispatch[GetKey(queue)].QueueNotifyOutOfBandNV(queue, pQueueTypeInfo);
winsockSendToUI(&ConnectSocket,"queue=" + ptrToString((void*)queue) + '!');
winsockSendToUI(&ConnectSocket,"pQueueTypeInfo=" + ptrToString((void*)pQueueTypeInfo) + '!');
#ifdef QUEUENOTIFYOUTOFBANDNV_AFTER_EXEC_EXISTS
if(connected) {
layer_QueueNotifyOutOfBandNV_after(queue, pQueueTypeInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkQueueNotifyOutOfBandNV!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkResetQueryPoolEXT!");
}
#ifdef RESETQUERYPOOLEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_ResetQueryPoolEXT_before(device, queryPool, firstQuery, queryCount);
}
#endif 
device_dispatch[GetKey(device)].ResetQueryPoolEXT(device, queryPool, firstQuery, queryCount);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"queryPool=" + ptrToString((void*)queryPool) + '!');
winsockSendToUI(&ConnectSocket,"firstQuery=" + std::to_string(firstQuery) + '!');
winsockSendToUI(&ConnectSocket,"queryCount=" + std::to_string(queryCount) + '!');
#ifdef RESETQUERYPOOLEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_ResetQueryPoolEXT_after(device, queryPool, firstQuery, queryCount);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkResetQueryPoolEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_TrimCommandPoolKHR(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkTrimCommandPoolKHR!");
}
#ifdef TRIMCOMMANDPOOLKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_TrimCommandPoolKHR_before(device, commandPool, flags);
}
#endif 
device_dispatch[GetKey(device)].TrimCommandPoolKHR(device, commandPool, flags);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"commandPool=" + ptrToString((void*)commandPool) + '!');
winsockSendToUI(&ConnectSocket,"flags=" + ptrToString((void*)flags) + '!');
#ifdef TRIMCOMMANDPOOLKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_TrimCommandPoolKHR_after(device, commandPool, flags);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkTrimCommandPoolKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceGroupPeerMemoryFeaturesKHR(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceGroupPeerMemoryFeaturesKHR!");
}
#ifdef GETDEVICEGROUPPEERMEMORYFEATURESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceGroupPeerMemoryFeaturesKHR_before(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}
#endif 
device_dispatch[GetKey(device)].GetDeviceGroupPeerMemoryFeaturesKHR(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"heapIndex=" + std::to_string(heapIndex) + '!');
winsockSendToUI(&ConnectSocket,"localDeviceIndex=" + std::to_string(localDeviceIndex) + '!');
winsockSendToUI(&ConnectSocket,"remoteDeviceIndex=" + std::to_string(remoteDeviceIndex) + '!');
winsockSendToUI(&ConnectSocket,"pPeerMemoryFeatures=" + ptrToString((void*)pPeerMemoryFeatures) + '!');
#ifdef GETDEVICEGROUPPEERMEMORYFEATURESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceGroupPeerMemoryFeaturesKHR_after(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceGroupPeerMemoryFeaturesKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, VkBindBufferMemoryInfo* pBindInfos) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkBindBufferMemory2KHR!");
}
#ifdef BINDBUFFERMEMORY2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_BindBufferMemory2KHR_before(device, bindInfoCount, pBindInfos);
}
#endif 
auto ret = device_dispatch[GetKey(device)].BindBufferMemory2KHR(device, bindInfoCount, pBindInfos);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"bindInfoCount=" + std::to_string(bindInfoCount) + '!');
winsockSendToUI(&ConnectSocket,"pBindInfos=" + ptrToString((void*)pBindInfos) + '!');
#ifdef BINDBUFFERMEMORY2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_BindBufferMemory2KHR_after(device, bindInfoCount, pBindInfos);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkBindBufferMemory2KHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, VkBindImageMemoryInfo* pBindInfos) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkBindImageMemory2KHR!");
}
#ifdef BINDIMAGEMEMORY2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_BindImageMemory2KHR_before(device, bindInfoCount, pBindInfos);
}
#endif 
auto ret = device_dispatch[GetKey(device)].BindImageMemory2KHR(device, bindInfoCount, pBindInfos);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"bindInfoCount=" + std::to_string(bindInfoCount) + '!');
winsockSendToUI(&ConnectSocket,"pBindInfos=" + ptrToString((void*)pBindInfos) + '!');
#ifdef BINDIMAGEMEMORY2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_BindImageMemory2KHR_after(device, bindInfoCount, pBindInfos);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkBindImageMemory2KHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDeviceMaskKHR!");
}
#ifdef CMDSETDEVICEMASKKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDeviceMaskKHR_before(commandBuffer, deviceMask);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDeviceMaskKHR(commandBuffer, deviceMask);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"deviceMask=" + std::to_string(deviceMask) + '!');
#ifdef CMDSETDEVICEMASKKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDeviceMaskKHR_after(commandBuffer, deviceMask);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDeviceMaskKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDispatchBaseKHR!");
}
#ifdef CMDDISPATCHBASEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDispatchBaseKHR_before(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDispatchBaseKHR(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"baseGroupX=" + std::to_string(baseGroupX) + '!');
winsockSendToUI(&ConnectSocket,"baseGroupY=" + std::to_string(baseGroupY) + '!');
winsockSendToUI(&ConnectSocket,"baseGroupZ=" + std::to_string(baseGroupZ) + '!');
winsockSendToUI(&ConnectSocket,"groupCountX=" + std::to_string(groupCountX) + '!');
winsockSendToUI(&ConnectSocket,"groupCountY=" + std::to_string(groupCountY) + '!');
winsockSendToUI(&ConnectSocket,"groupCountZ=" + std::to_string(groupCountZ) + '!');
#ifdef CMDDISPATCHBASEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDispatchBaseKHR_after(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDispatchBaseKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateDescriptorUpdateTemplateKHR!");
}
#ifdef CREATEDESCRIPTORUPDATETEMPLATEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateDescriptorUpdateTemplateKHR_before(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateDescriptorUpdateTemplateKHR(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pDescriptorUpdateTemplate=" + ptrToString((void*)pDescriptorUpdateTemplate) + '!');
#ifdef CREATEDESCRIPTORUPDATETEMPLATEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateDescriptorUpdateTemplateKHR_after(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateDescriptorUpdateTemplateKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyDescriptorUpdateTemplateKHR!");
}
#ifdef DESTROYDESCRIPTORUPDATETEMPLATEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyDescriptorUpdateTemplateKHR_before(device, descriptorUpdateTemplate, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyDescriptorUpdateTemplateKHR(device, descriptorUpdateTemplate, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"descriptorUpdateTemplate=" + ptrToString((void*)descriptorUpdateTemplate) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYDESCRIPTORUPDATETEMPLATEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyDescriptorUpdateTemplateKHR_after(device, descriptorUpdateTemplate, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyDescriptorUpdateTemplateKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_UpdateDescriptorSetWithTemplateKHR(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkUpdateDescriptorSetWithTemplateKHR!");
}
#ifdef UPDATEDESCRIPTORSETWITHTEMPLATEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_UpdateDescriptorSetWithTemplateKHR_before(device, descriptorSet, descriptorUpdateTemplate, pData);
}
#endif 
device_dispatch[GetKey(device)].UpdateDescriptorSetWithTemplateKHR(device, descriptorSet, descriptorUpdateTemplate, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"descriptorSet=" + ptrToString((void*)descriptorSet) + '!');
winsockSendToUI(&ConnectSocket,"descriptorUpdateTemplate=" + ptrToString((void*)descriptorUpdateTemplate) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef UPDATEDESCRIPTORSETWITHTEMPLATEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_UpdateDescriptorSetWithTemplateKHR_after(device, descriptorSet, descriptorUpdateTemplate, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkUpdateDescriptorSetWithTemplateKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetBufferMemoryRequirements2KHR(VkDevice device, VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetBufferMemoryRequirements2KHR!");
}
#ifdef GETBUFFERMEMORYREQUIREMENTS2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetBufferMemoryRequirements2KHR_before(device, pInfo, pMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetBufferMemoryRequirements2KHR(device, pInfo, pMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRequirements=" + ptrToString((void*)pMemoryRequirements) + '!');
#ifdef GETBUFFERMEMORYREQUIREMENTS2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetBufferMemoryRequirements2KHR_after(device, pInfo, pMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetBufferMemoryRequirements2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageMemoryRequirements2KHR(VkDevice device, VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetImageMemoryRequirements2KHR!");
}
#ifdef GETIMAGEMEMORYREQUIREMENTS2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetImageMemoryRequirements2KHR_before(device, pInfo, pMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetImageMemoryRequirements2KHR(device, pInfo, pMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRequirements=" + ptrToString((void*)pMemoryRequirements) + '!');
#ifdef GETIMAGEMEMORYREQUIREMENTS2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetImageMemoryRequirements2KHR_after(device, pInfo, pMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetImageMemoryRequirements2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageSparseMemoryRequirements2KHR(VkDevice device, VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetImageSparseMemoryRequirements2KHR!");
}
#ifdef GETIMAGESPARSEMEMORYREQUIREMENTS2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetImageSparseMemoryRequirements2KHR_before(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetImageSparseMemoryRequirements2KHR(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pSparseMemoryRequirementCount=" + ptrToString((void*)pSparseMemoryRequirementCount) + '!');
winsockSendToUI(&ConnectSocket,"pSparseMemoryRequirements=" + ptrToString((void*)pSparseMemoryRequirements) + '!');
#ifdef GETIMAGESPARSEMEMORYREQUIREMENTS2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetImageSparseMemoryRequirements2KHR_after(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetImageSparseMemoryRequirements2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceBufferMemoryRequirementsKHR(VkDevice device, VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceBufferMemoryRequirementsKHR!");
}
#ifdef GETDEVICEBUFFERMEMORYREQUIREMENTSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceBufferMemoryRequirementsKHR_before(device, pInfo, pMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetDeviceBufferMemoryRequirementsKHR(device, pInfo, pMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRequirements=" + ptrToString((void*)pMemoryRequirements) + '!');
#ifdef GETDEVICEBUFFERMEMORYREQUIREMENTSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceBufferMemoryRequirementsKHR_after(device, pInfo, pMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceBufferMemoryRequirementsKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceImageMemoryRequirementsKHR(VkDevice device, VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceImageMemoryRequirementsKHR!");
}
#ifdef GETDEVICEIMAGEMEMORYREQUIREMENTSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceImageMemoryRequirementsKHR_before(device, pInfo, pMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetDeviceImageMemoryRequirementsKHR(device, pInfo, pMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pMemoryRequirements=" + ptrToString((void*)pMemoryRequirements) + '!');
#ifdef GETDEVICEIMAGEMEMORYREQUIREMENTSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceImageMemoryRequirementsKHR_after(device, pInfo, pMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceImageMemoryRequirementsKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceImageSparseMemoryRequirementsKHR(VkDevice device, VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceImageSparseMemoryRequirementsKHR!");
}
#ifdef GETDEVICEIMAGESPARSEMEMORYREQUIREMENTSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceImageSparseMemoryRequirementsKHR_before(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
#endif 
device_dispatch[GetKey(device)].GetDeviceImageSparseMemoryRequirementsKHR(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
winsockSendToUI(&ConnectSocket,"pSparseMemoryRequirementCount=" + ptrToString((void*)pSparseMemoryRequirementCount) + '!');
winsockSendToUI(&ConnectSocket,"pSparseMemoryRequirements=" + ptrToString((void*)pSparseMemoryRequirements) + '!');
#ifdef GETDEVICEIMAGESPARSEMEMORYREQUIREMENTSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceImageSparseMemoryRequirementsKHR_after(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceImageSparseMemoryRequirementsKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateSamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversionCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateSamplerYcbcrConversionKHR!");
}
#ifdef CREATESAMPLERYCBCRCONVERSIONKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateSamplerYcbcrConversionKHR_before(device, pCreateInfo, pAllocator, pYcbcrConversion);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateSamplerYcbcrConversionKHR(device, pCreateInfo, pAllocator, pYcbcrConversion);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pYcbcrConversion=" + ptrToString((void*)pYcbcrConversion) + '!');
#ifdef CREATESAMPLERYCBCRCONVERSIONKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateSamplerYcbcrConversionKHR_after(device, pCreateInfo, pAllocator, pYcbcrConversion);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateSamplerYcbcrConversionKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroySamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroySamplerYcbcrConversionKHR!");
}
#ifdef DESTROYSAMPLERYCBCRCONVERSIONKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroySamplerYcbcrConversionKHR_before(device, ycbcrConversion, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroySamplerYcbcrConversionKHR(device, ycbcrConversion, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"ycbcrConversion=" + ptrToString((void*)ycbcrConversion) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYSAMPLERYCBCRCONVERSIONKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroySamplerYcbcrConversionKHR_after(device, ycbcrConversion, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroySamplerYcbcrConversionKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDescriptorSetLayoutSupportKHR(VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDescriptorSetLayoutSupportKHR!");
}
#ifdef GETDESCRIPTORSETLAYOUTSUPPORTKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDescriptorSetLayoutSupportKHR_before(device, pCreateInfo, pSupport);
}
#endif 
device_dispatch[GetKey(device)].GetDescriptorSetLayoutSupportKHR(device, pCreateInfo, pSupport);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pSupport=" + ptrToString((void*)pSupport) + '!');
#ifdef GETDESCRIPTORSETLAYOUTSUPPORTKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDescriptorSetLayoutSupportKHR_after(device, pCreateInfo, pSupport);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDescriptorSetLayoutSupportKHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetCalibratedTimestampsEXT!");
}
#ifdef GETCALIBRATEDTIMESTAMPSEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetCalibratedTimestampsEXT_before(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetCalibratedTimestampsEXT(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"timestampCount=" + std::to_string(timestampCount) + '!');
winsockSendToUI(&ConnectSocket,"pTimestampInfos=" + ptrToString((void*)pTimestampInfos) + '!');
winsockSendToUI(&ConnectSocket,"pTimestamps=" + ptrToString((void*)pTimestamps) + '!');
winsockSendToUI(&ConnectSocket,"pMaxDeviation=" + ptrToString((void*)pMaxDeviation) + '!');
#ifdef GETCALIBRATEDTIMESTAMPSEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetCalibratedTimestampsEXT_after(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetCalibratedTimestampsEXT!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateRenderPass2KHR(VkDevice device, VkRenderPassCreateInfo2* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreateRenderPass2KHR!");
}
#ifdef CREATERENDERPASS2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreateRenderPass2KHR_before(device, pCreateInfo, pAllocator, pRenderPass);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreateRenderPass2KHR(device, pCreateInfo, pAllocator, pRenderPass);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pRenderPass=" + ptrToString((void*)pRenderPass) + '!');
#ifdef CREATERENDERPASS2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CreateRenderPass2KHR_after(device, pCreateInfo, pAllocator, pRenderPass);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreateRenderPass2KHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassBeginInfo* pSubpassBeginInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBeginRenderPass2KHR!");
}
#ifdef CMDBEGINRENDERPASS2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBeginRenderPass2KHR_before(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginRenderPass2KHR(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pRenderPassBegin=" + ptrToString((void*)pRenderPassBegin) + '!');
winsockSendToUI(&ConnectSocket,"pSubpassBeginInfo=" + ptrToString((void*)pSubpassBeginInfo) + '!');
#ifdef CMDBEGINRENDERPASS2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBeginRenderPass2KHR_after(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBeginRenderPass2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, VkSubpassBeginInfo* pSubpassBeginInfo, VkSubpassEndInfo* pSubpassEndInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdNextSubpass2KHR!");
}
#ifdef CMDNEXTSUBPASS2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdNextSubpass2KHR_before(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdNextSubpass2KHR(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pSubpassBeginInfo=" + ptrToString((void*)pSubpassBeginInfo) + '!');
winsockSendToUI(&ConnectSocket,"pSubpassEndInfo=" + ptrToString((void*)pSubpassEndInfo) + '!');
#ifdef CMDNEXTSUBPASS2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdNextSubpass2KHR_after(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdNextSubpass2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, VkSubpassEndInfo* pSubpassEndInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdEndRenderPass2KHR!");
}
#ifdef CMDENDRENDERPASS2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdEndRenderPass2KHR_before(commandBuffer, pSubpassEndInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndRenderPass2KHR(commandBuffer, pSubpassEndInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pSubpassEndInfo=" + ptrToString((void*)pSubpassEndInfo) + '!');
#ifdef CMDENDRENDERPASS2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdEndRenderPass2KHR_after(commandBuffer, pSubpassEndInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdEndRenderPass2KHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSemaphoreCounterValueKHR(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetSemaphoreCounterValueKHR!");
}
#ifdef GETSEMAPHORECOUNTERVALUEKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetSemaphoreCounterValueKHR_before(device, semaphore, pValue);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetSemaphoreCounterValueKHR(device, semaphore, pValue);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"semaphore=" + ptrToString((void*)semaphore) + '!');
winsockSendToUI(&ConnectSocket,"pValue=" + ptrToString((void*)pValue) + '!');
#ifdef GETSEMAPHORECOUNTERVALUEKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetSemaphoreCounterValueKHR_after(device, semaphore, pValue);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetSemaphoreCounterValueKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_WaitSemaphoresKHR(VkDevice device, VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkWaitSemaphoresKHR!");
}
#ifdef WAITSEMAPHORESKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_WaitSemaphoresKHR_before(device, pWaitInfo, timeout);
}
#endif 
auto ret = device_dispatch[GetKey(device)].WaitSemaphoresKHR(device, pWaitInfo, timeout);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pWaitInfo=" + ptrToString((void*)pWaitInfo) + '!');
winsockSendToUI(&ConnectSocket,"timeout=" + std::to_string(timeout) + '!');
#ifdef WAITSEMAPHORESKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_WaitSemaphoresKHR_after(device, pWaitInfo, timeout);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkWaitSemaphoresKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SignalSemaphoreKHR(VkDevice device, VkSemaphoreSignalInfo* pSignalInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkSignalSemaphoreKHR!");
}
#ifdef SIGNALSEMAPHOREKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_SignalSemaphoreKHR_before(device, pSignalInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].SignalSemaphoreKHR(device, pSignalInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pSignalInfo=" + ptrToString((void*)pSignalInfo) + '!');
#ifdef SIGNALSEMAPHOREKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_SignalSemaphoreKHR_after(device, pSignalInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkSignalSemaphoreKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawIndirectCountKHR!");
}
#ifdef CMDDRAWINDIRECTCOUNTKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndirectCountKHR_before(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
winsockSendToUI(&ConnectSocket,"countBuffer=" + ptrToString((void*)countBuffer) + '!');
winsockSendToUI(&ConnectSocket,"countBufferOffset=" + ptrToString((void*)countBufferOffset) + '!');
winsockSendToUI(&ConnectSocket,"maxDrawCount=" + std::to_string(maxDrawCount) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef CMDDRAWINDIRECTCOUNTKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndirectCountKHR_after(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawIndirectCountKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawIndirectCountAMD!");
}
#ifdef CMDDRAWINDIRECTCOUNTAMD_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndirectCountAMD_before(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
winsockSendToUI(&ConnectSocket,"countBuffer=" + ptrToString((void*)countBuffer) + '!');
winsockSendToUI(&ConnectSocket,"countBufferOffset=" + ptrToString((void*)countBufferOffset) + '!');
winsockSendToUI(&ConnectSocket,"maxDrawCount=" + std::to_string(maxDrawCount) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef CMDDRAWINDIRECTCOUNTAMD_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndirectCountAMD_after(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawIndirectCountAMD!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawIndexedIndirectCountKHR!");
}
#ifdef CMDDRAWINDEXEDINDIRECTCOUNTKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndexedIndirectCountKHR_before(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndexedIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
winsockSendToUI(&ConnectSocket,"countBuffer=" + ptrToString((void*)countBuffer) + '!');
winsockSendToUI(&ConnectSocket,"countBufferOffset=" + ptrToString((void*)countBufferOffset) + '!');
winsockSendToUI(&ConnectSocket,"maxDrawCount=" + std::to_string(maxDrawCount) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef CMDDRAWINDEXEDINDIRECTCOUNTKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndexedIndirectCountKHR_after(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawIndexedIndirectCountKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdDrawIndexedIndirectCountAMD!");
}
#ifdef CMDDRAWINDEXEDINDIRECTCOUNTAMD_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndexedIndirectCountAMD_before(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndexedIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"buffer=" + ptrToString((void*)buffer) + '!');
winsockSendToUI(&ConnectSocket,"offset=" + ptrToString((void*)offset) + '!');
winsockSendToUI(&ConnectSocket,"countBuffer=" + ptrToString((void*)countBuffer) + '!');
winsockSendToUI(&ConnectSocket,"countBufferOffset=" + ptrToString((void*)countBufferOffset) + '!');
winsockSendToUI(&ConnectSocket,"maxDrawCount=" + std::to_string(maxDrawCount) + '!');
winsockSendToUI(&ConnectSocket,"stride=" + std::to_string(stride) + '!');
#ifdef CMDDRAWINDEXEDINDIRECTCOUNTAMD_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdDrawIndexedIndirectCountAMD_after(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdDrawIndexedIndirectCountAMD!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetRayTracingShaderGroupHandlesNV!");
}
#ifdef GETRAYTRACINGSHADERGROUPHANDLESNV_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetRayTracingShaderGroupHandlesNV_before(device, pipeline, firstGroup, groupCount, dataSize, pData);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetRayTracingShaderGroupHandlesNV(device, pipeline, firstGroup, groupCount, dataSize, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pipeline=" + ptrToString((void*)pipeline) + '!');
winsockSendToUI(&ConnectSocket,"firstGroup=" + std::to_string(firstGroup) + '!');
winsockSendToUI(&ConnectSocket,"groupCount=" + std::to_string(groupCount) + '!');
winsockSendToUI(&ConnectSocket,"dataSize=" + std::to_string(dataSize) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef GETRAYTRACINGSHADERGROUPHANDLESNV_AFTER_EXEC_EXISTS
if(connected) {
layer_GetRayTracingShaderGroupHandlesNV_after(device, pipeline, firstGroup, groupCount, dataSize, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetRayTracingShaderGroupHandlesNV!");
}
return ret;
}

 VK_LAYER_EXPORT uint64_t VKAPI_CALL DetailsLayer_GetBufferOpaqueCaptureAddressKHR(VkDevice device, VkBufferDeviceAddressInfo* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetBufferOpaqueCaptureAddressKHR!");
}
#ifdef GETBUFFEROPAQUECAPTUREADDRESSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetBufferOpaqueCaptureAddressKHR_before(device, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetBufferOpaqueCaptureAddressKHR(device, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef GETBUFFEROPAQUECAPTUREADDRESSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetBufferOpaqueCaptureAddressKHR_after(device, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetBufferOpaqueCaptureAddressKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkDeviceAddress VKAPI_CALL DetailsLayer_GetBufferDeviceAddressKHR(VkDevice device, VkBufferDeviceAddressInfo* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetBufferDeviceAddressKHR!");
}
#ifdef GETBUFFERDEVICEADDRESSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetBufferDeviceAddressKHR_before(device, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetBufferDeviceAddressKHR(device, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef GETBUFFERDEVICEADDRESSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetBufferDeviceAddressKHR_after(device, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetBufferDeviceAddressKHR!");
}
return ret;
}

 VK_LAYER_EXPORT VkDeviceAddress VKAPI_CALL DetailsLayer_GetBufferDeviceAddressEXT(VkDevice device, VkBufferDeviceAddressInfo* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetBufferDeviceAddressEXT!");
}
#ifdef GETBUFFERDEVICEADDRESSEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetBufferDeviceAddressEXT_before(device, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetBufferDeviceAddressEXT(device, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef GETBUFFERDEVICEADDRESSEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetBufferDeviceAddressEXT_after(device, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetBufferDeviceAddressEXT!");
}
return ret;
}

 VK_LAYER_EXPORT uint64_t VKAPI_CALL DetailsLayer_GetDeviceMemoryOpaqueCaptureAddressKHR(VkDevice device, VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetDeviceMemoryOpaqueCaptureAddressKHR!");
}
#ifdef GETDEVICEMEMORYOPAQUECAPTUREADDRESSKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetDeviceMemoryOpaqueCaptureAddressKHR_before(device, pInfo);
}
#endif 
auto ret = device_dispatch[GetKey(device)].GetDeviceMemoryOpaqueCaptureAddressKHR(device, pInfo);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pInfo=" + ptrToString((void*)pInfo) + '!');
#ifdef GETDEVICEMEMORYOPAQUECAPTUREADDRESSKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_GetDeviceMemoryOpaqueCaptureAddressKHR_after(device, pInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetDeviceMemoryOpaqueCaptureAddressKHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetLineStippleEXT!");
}
#ifdef CMDSETLINESTIPPLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetLineStippleEXT_before(commandBuffer, lineStippleFactor, lineStipplePattern);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"lineStippleFactor=" + std::to_string(lineStippleFactor) + '!');
winsockSendToUI(&ConnectSocket,"lineStipplePattern=" + ptrToString((void*)lineStipplePattern) + '!');
#ifdef CMDSETLINESTIPPLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetLineStippleEXT_after(commandBuffer, lineStippleFactor, lineStipplePattern);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetLineStippleEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetCullModeEXT!");
}
#ifdef CMDSETCULLMODEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetCullModeEXT_before(commandBuffer, cullMode);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCullModeEXT(commandBuffer, cullMode);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"cullMode=" + ptrToString((void*)cullMode) + '!');
#ifdef CMDSETCULLMODEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetCullModeEXT_after(commandBuffer, cullMode);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetCullModeEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetFrontFaceEXT!");
}
#ifdef CMDSETFRONTFACEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetFrontFaceEXT_before(commandBuffer, frontFace);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetFrontFaceEXT(commandBuffer, frontFace);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"frontFace=" + std::to_string(frontFace) + '!');
#ifdef CMDSETFRONTFACEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetFrontFaceEXT_after(commandBuffer, frontFace);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetFrontFaceEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetPrimitiveTopologyEXT!");
}
#ifdef CMDSETPRIMITIVETOPOLOGYEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetPrimitiveTopologyEXT_before(commandBuffer, primitiveTopology);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetPrimitiveTopologyEXT(commandBuffer, primitiveTopology);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"primitiveTopology=" + std::to_string(primitiveTopology) + '!');
#ifdef CMDSETPRIMITIVETOPOLOGYEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetPrimitiveTopologyEXT_after(commandBuffer, primitiveTopology);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetPrimitiveTopologyEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, VkViewport* pViewports) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetViewportWithCountEXT!");
}
#ifdef CMDSETVIEWPORTWITHCOUNTEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetViewportWithCountEXT_before(commandBuffer, viewportCount, pViewports);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetViewportWithCountEXT(commandBuffer, viewportCount, pViewports);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"viewportCount=" + std::to_string(viewportCount) + '!');
winsockSendToUI(&ConnectSocket,"pViewports=" + ptrToString((void*)pViewports) + '!');
#ifdef CMDSETVIEWPORTWITHCOUNTEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetViewportWithCountEXT_after(commandBuffer, viewportCount, pViewports);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetViewportWithCountEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, VkRect2D* pScissors) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetScissorWithCountEXT!");
}
#ifdef CMDSETSCISSORWITHCOUNTEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetScissorWithCountEXT_before(commandBuffer, scissorCount, pScissors);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetScissorWithCountEXT(commandBuffer, scissorCount, pScissors);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"scissorCount=" + std::to_string(scissorCount) + '!');
winsockSendToUI(&ConnectSocket,"pScissors=" + ptrToString((void*)pScissors) + '!');
#ifdef CMDSETSCISSORWITHCOUNTEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetScissorWithCountEXT_after(commandBuffer, scissorCount, pScissors);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetScissorWithCountEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets, VkDeviceSize* pSizes, VkDeviceSize* pStrides) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBindVertexBuffers2EXT!");
}
#ifdef CMDBINDVERTEXBUFFERS2EXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBindVertexBuffers2EXT_before(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindVertexBuffers2EXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"firstBinding=" + std::to_string(firstBinding) + '!');
winsockSendToUI(&ConnectSocket,"bindingCount=" + std::to_string(bindingCount) + '!');
winsockSendToUI(&ConnectSocket,"pBuffers=" + ptrToString((void*)pBuffers) + '!');
winsockSendToUI(&ConnectSocket,"pOffsets=" + ptrToString((void*)pOffsets) + '!');
winsockSendToUI(&ConnectSocket,"pSizes=" + ptrToString((void*)pSizes) + '!');
winsockSendToUI(&ConnectSocket,"pStrides=" + ptrToString((void*)pStrides) + '!');
#ifdef CMDBINDVERTEXBUFFERS2EXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBindVertexBuffers2EXT_after(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBindVertexBuffers2EXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDepthTestEnableEXT!");
}
#ifdef CMDSETDEPTHTESTENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthTestEnableEXT_before(commandBuffer, depthTestEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthTestEnableEXT(commandBuffer, depthTestEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"depthTestEnable=" + bool_as_text(depthTestEnable) + '!');
#ifdef CMDSETDEPTHTESTENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthTestEnableEXT_after(commandBuffer, depthTestEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDepthTestEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDepthWriteEnableEXT!");
}
#ifdef CMDSETDEPTHWRITEENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthWriteEnableEXT_before(commandBuffer, depthWriteEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthWriteEnableEXT(commandBuffer, depthWriteEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"depthWriteEnable=" + bool_as_text(depthWriteEnable) + '!');
#ifdef CMDSETDEPTHWRITEENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthWriteEnableEXT_after(commandBuffer, depthWriteEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDepthWriteEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDepthCompareOpEXT!");
}
#ifdef CMDSETDEPTHCOMPAREOPEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthCompareOpEXT_before(commandBuffer, depthCompareOp);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthCompareOpEXT(commandBuffer, depthCompareOp);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"depthCompareOp=" + std::to_string(depthCompareOp) + '!');
#ifdef CMDSETDEPTHCOMPAREOPEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthCompareOpEXT_after(commandBuffer, depthCompareOp);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDepthCompareOpEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDepthBoundsTestEnableEXT!");
}
#ifdef CMDSETDEPTHBOUNDSTESTENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthBoundsTestEnableEXT_before(commandBuffer, depthBoundsTestEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthBoundsTestEnableEXT(commandBuffer, depthBoundsTestEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"depthBoundsTestEnable=" + bool_as_text(depthBoundsTestEnable) + '!');
#ifdef CMDSETDEPTHBOUNDSTESTENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthBoundsTestEnableEXT_after(commandBuffer, depthBoundsTestEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDepthBoundsTestEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetStencilTestEnableEXT!");
}
#ifdef CMDSETSTENCILTESTENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetStencilTestEnableEXT_before(commandBuffer, stencilTestEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetStencilTestEnableEXT(commandBuffer, stencilTestEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"stencilTestEnable=" + bool_as_text(stencilTestEnable) + '!');
#ifdef CMDSETSTENCILTESTENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetStencilTestEnableEXT_after(commandBuffer, stencilTestEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetStencilTestEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetStencilOpEXT!");
}
#ifdef CMDSETSTENCILOPEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetStencilOpEXT_before(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetStencilOpEXT(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"faceMask=" + ptrToString((void*)faceMask) + '!');
winsockSendToUI(&ConnectSocket,"failOp=" + std::to_string(failOp) + '!');
winsockSendToUI(&ConnectSocket,"passOp=" + std::to_string(passOp) + '!');
winsockSendToUI(&ConnectSocket,"depthFailOp=" + std::to_string(depthFailOp) + '!');
winsockSendToUI(&ConnectSocket,"compareOp=" + std::to_string(compareOp) + '!');
#ifdef CMDSETSTENCILOPEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetStencilOpEXT_after(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetStencilOpEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetRasterizerDiscardEnableEXT!");
}
#ifdef CMDSETRASTERIZERDISCARDENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetRasterizerDiscardEnableEXT_before(commandBuffer, rasterizerDiscardEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetRasterizerDiscardEnableEXT(commandBuffer, rasterizerDiscardEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"rasterizerDiscardEnable=" + bool_as_text(rasterizerDiscardEnable) + '!');
#ifdef CMDSETRASTERIZERDISCARDENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetRasterizerDiscardEnableEXT_after(commandBuffer, rasterizerDiscardEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetRasterizerDiscardEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetDepthBiasEnableEXT!");
}
#ifdef CMDSETDEPTHBIASENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthBiasEnableEXT_before(commandBuffer, depthBiasEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthBiasEnableEXT(commandBuffer, depthBiasEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"depthBiasEnable=" + bool_as_text(depthBiasEnable) + '!');
#ifdef CMDSETDEPTHBIASENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetDepthBiasEnableEXT_after(commandBuffer, depthBiasEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetDepthBiasEnableEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetPrimitiveRestartEnableEXT!");
}
#ifdef CMDSETPRIMITIVERESTARTENABLEEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetPrimitiveRestartEnableEXT_before(commandBuffer, primitiveRestartEnable);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetPrimitiveRestartEnableEXT(commandBuffer, primitiveRestartEnable);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"primitiveRestartEnable=" + bool_as_text(primitiveRestartEnable) + '!');
#ifdef CMDSETPRIMITIVERESTARTENABLEEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetPrimitiveRestartEnableEXT_after(commandBuffer, primitiveRestartEnable);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetPrimitiveRestartEnableEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreatePrivateDataSlotEXT(VkDevice device, VkPrivateDataSlotCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCreatePrivateDataSlotEXT!");
}
#ifdef CREATEPRIVATEDATASLOTEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_CreatePrivateDataSlotEXT_before(device, pCreateInfo, pAllocator, pPrivateDataSlot);
}
#endif 
auto ret = device_dispatch[GetKey(device)].CreatePrivateDataSlotEXT(device, pCreateInfo, pAllocator, pPrivateDataSlot);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"pCreateInfo=" + ptrToString((void*)pCreateInfo) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
winsockSendToUI(&ConnectSocket,"pPrivateDataSlot=" + ptrToString((void*)pPrivateDataSlot) + '!');
#ifdef CREATEPRIVATEDATASLOTEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_CreatePrivateDataSlotEXT_after(device, pCreateInfo, pAllocator, pPrivateDataSlot);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCreatePrivateDataSlotEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyPrivateDataSlotEXT(VkDevice device, VkPrivateDataSlot privateDataSlot, VkAllocationCallbacks* pAllocator) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkDestroyPrivateDataSlotEXT!");
}
#ifdef DESTROYPRIVATEDATASLOTEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_DestroyPrivateDataSlotEXT_before(device, privateDataSlot, pAllocator);
}
#endif 
device_dispatch[GetKey(device)].DestroyPrivateDataSlotEXT(device, privateDataSlot, pAllocator);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"privateDataSlot=" + ptrToString((void*)privateDataSlot) + '!');
winsockSendToUI(&ConnectSocket,"pAllocator=" + ptrToString((void*)pAllocator) + '!');
#ifdef DESTROYPRIVATEDATASLOTEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_DestroyPrivateDataSlotEXT_after(device, privateDataSlot, pAllocator);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkDestroyPrivateDataSlotEXT!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkSetPrivateDataEXT!");
}
#ifdef SETPRIVATEDATAEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_SetPrivateDataEXT_before(device, objectType, objectHandle, privateDataSlot, data);
}
#endif 
auto ret = device_dispatch[GetKey(device)].SetPrivateDataEXT(device, objectType, objectHandle, privateDataSlot, data);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"objectType=" + std::to_string(objectType) + '!');
winsockSendToUI(&ConnectSocket,"objectHandle=" + std::to_string(objectHandle) + '!');
winsockSendToUI(&ConnectSocket,"privateDataSlot=" + ptrToString((void*)privateDataSlot) + '!');
winsockSendToUI(&ConnectSocket,"data=" + std::to_string(data) + '!');
#ifdef SETPRIVATEDATAEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_SetPrivateDataEXT_after(device, objectType, objectHandle, privateDataSlot, data);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkSetPrivateDataEXT!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetPrivateDataEXT!");
}
#ifdef GETPRIVATEDATAEXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetPrivateDataEXT_before(device, objectType, objectHandle, privateDataSlot, pData);
}
#endif 
device_dispatch[GetKey(device)].GetPrivateDataEXT(device, objectType, objectHandle, privateDataSlot, pData);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"objectType=" + std::to_string(objectType) + '!');
winsockSendToUI(&ConnectSocket,"objectHandle=" + std::to_string(objectHandle) + '!');
winsockSendToUI(&ConnectSocket,"privateDataSlot=" + ptrToString((void*)privateDataSlot) + '!');
winsockSendToUI(&ConnectSocket,"pData=" + ptrToString((void*)pData) + '!');
#ifdef GETPRIVATEDATAEXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetPrivateDataEXT_after(device, objectType, objectHandle, privateDataSlot, pData);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetPrivateDataEXT!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, VkCopyBufferInfo2* pCopyBufferInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyBuffer2KHR!");
}
#ifdef CMDCOPYBUFFER2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyBuffer2KHR_before(commandBuffer, pCopyBufferInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyBuffer2KHR(commandBuffer, pCopyBufferInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pCopyBufferInfo=" + ptrToString((void*)pCopyBufferInfo) + '!');
#ifdef CMDCOPYBUFFER2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyBuffer2KHR_after(commandBuffer, pCopyBufferInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyBuffer2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyImage2KHR(VkCommandBuffer commandBuffer, VkCopyImageInfo2* pCopyImageInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyImage2KHR!");
}
#ifdef CMDCOPYIMAGE2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyImage2KHR_before(commandBuffer, pCopyImageInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyImage2KHR(commandBuffer, pCopyImageInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pCopyImageInfo=" + ptrToString((void*)pCopyImageInfo) + '!');
#ifdef CMDCOPYIMAGE2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyImage2KHR_after(commandBuffer, pCopyImageInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyImage2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBlitImage2KHR(VkCommandBuffer commandBuffer, VkBlitImageInfo2* pBlitImageInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBlitImage2KHR!");
}
#ifdef CMDBLITIMAGE2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBlitImage2KHR_before(commandBuffer, pBlitImageInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBlitImage2KHR(commandBuffer, pBlitImageInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pBlitImageInfo=" + ptrToString((void*)pBlitImageInfo) + '!');
#ifdef CMDBLITIMAGE2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBlitImage2KHR_after(commandBuffer, pBlitImageInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBlitImage2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyBufferToImage2KHR!");
}
#ifdef CMDCOPYBUFFERTOIMAGE2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyBufferToImage2KHR_before(commandBuffer, pCopyBufferToImageInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyBufferToImage2KHR(commandBuffer, pCopyBufferToImageInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pCopyBufferToImageInfo=" + ptrToString((void*)pCopyBufferToImageInfo) + '!');
#ifdef CMDCOPYBUFFERTOIMAGE2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyBufferToImage2KHR_after(commandBuffer, pCopyBufferToImageInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyBufferToImage2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdCopyImageToBuffer2KHR!");
}
#ifdef CMDCOPYIMAGETOBUFFER2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdCopyImageToBuffer2KHR_before(commandBuffer, pCopyImageToBufferInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyImageToBuffer2KHR(commandBuffer, pCopyImageToBufferInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pCopyImageToBufferInfo=" + ptrToString((void*)pCopyImageToBufferInfo) + '!');
#ifdef CMDCOPYIMAGETOBUFFER2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdCopyImageToBuffer2KHR_after(commandBuffer, pCopyImageToBufferInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdCopyImageToBuffer2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdResolveImage2KHR(VkCommandBuffer commandBuffer, VkResolveImageInfo2* pResolveImageInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdResolveImage2KHR!");
}
#ifdef CMDRESOLVEIMAGE2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdResolveImage2KHR_before(commandBuffer, pResolveImageInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdResolveImage2KHR(commandBuffer, pResolveImageInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pResolveImageInfo=" + ptrToString((void*)pResolveImageInfo) + '!');
#ifdef CMDRESOLVEIMAGE2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdResolveImage2KHR_after(commandBuffer, pResolveImageInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdResolveImage2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, VkDependencyInfo* pDependencyInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdSetEvent2KHR!");
}
#ifdef CMDSETEVENT2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdSetEvent2KHR_before(commandBuffer, event, pDependencyInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetEvent2KHR(commandBuffer, event, pDependencyInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"event=" + ptrToString((void*)event) + '!');
winsockSendToUI(&ConnectSocket,"pDependencyInfo=" + ptrToString((void*)pDependencyInfo) + '!');
#ifdef CMDSETEVENT2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdSetEvent2KHR_after(commandBuffer, event, pDependencyInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdSetEvent2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdResetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdResetEvent2KHR!");
}
#ifdef CMDRESETEVENT2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdResetEvent2KHR_before(commandBuffer, event, stageMask);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdResetEvent2KHR(commandBuffer, event, stageMask);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"event=" + ptrToString((void*)event) + '!');
winsockSendToUI(&ConnectSocket,"stageMask=" + ptrToString((void*)stageMask) + '!');
#ifdef CMDRESETEVENT2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdResetEvent2KHR_after(commandBuffer, event, stageMask);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdResetEvent2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWaitEvents2KHR(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent* pEvents, VkDependencyInfo* pDependencyInfos) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdWaitEvents2KHR!");
}
#ifdef CMDWAITEVENTS2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdWaitEvents2KHR_before(commandBuffer, eventCount, pEvents, pDependencyInfos);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWaitEvents2KHR(commandBuffer, eventCount, pEvents, pDependencyInfos);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"eventCount=" + std::to_string(eventCount) + '!');
winsockSendToUI(&ConnectSocket,"pEvents=" + ptrToString((void*)pEvents) + '!');
winsockSendToUI(&ConnectSocket,"pDependencyInfos=" + ptrToString((void*)pDependencyInfos) + '!');
#ifdef CMDWAITEVENTS2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdWaitEvents2KHR_after(commandBuffer, eventCount, pEvents, pDependencyInfos);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdWaitEvents2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPipelineBarrier2KHR(VkCommandBuffer commandBuffer, VkDependencyInfo* pDependencyInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdPipelineBarrier2KHR!");
}
#ifdef CMDPIPELINEBARRIER2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdPipelineBarrier2KHR_before(commandBuffer, pDependencyInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPipelineBarrier2KHR(commandBuffer, pDependencyInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pDependencyInfo=" + ptrToString((void*)pDependencyInfo) + '!');
#ifdef CMDPIPELINEBARRIER2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdPipelineBarrier2KHR_after(commandBuffer, pDependencyInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdPipelineBarrier2KHR!");
}
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_QueueSubmit2KHR(VkQueue queue, uint32_t submitCount, VkSubmitInfo2* pSubmits, VkFence fence) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkQueueSubmit2KHR!");
}
#ifdef QUEUESUBMIT2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_QueueSubmit2KHR_before(queue, submitCount, pSubmits, fence);
}
#endif 
auto ret = device_dispatch[GetKey(queue)].QueueSubmit2KHR(queue, submitCount, pSubmits, fence);
winsockSendToUI(&ConnectSocket,"queue=" + ptrToString((void*)queue) + '!');
winsockSendToUI(&ConnectSocket,"submitCount=" + std::to_string(submitCount) + '!');
winsockSendToUI(&ConnectSocket,"pSubmits=" + ptrToString((void*)pSubmits) + '!');
winsockSendToUI(&ConnectSocket,"fence=" + ptrToString((void*)fence) + '!');
#ifdef QUEUESUBMIT2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_QueueSubmit2KHR_after(queue, submitCount, pSubmits, fence);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkQueueSubmit2KHR!");
}
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWriteTimestamp2KHR(VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdWriteTimestamp2KHR!");
}
#ifdef CMDWRITETIMESTAMP2KHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdWriteTimestamp2KHR_before(commandBuffer, stage, queryPool, query);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWriteTimestamp2KHR(commandBuffer, stage, queryPool, query);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"stage=" + ptrToString((void*)stage) + '!');
winsockSendToUI(&ConnectSocket,"queryPool=" + ptrToString((void*)queryPool) + '!');
winsockSendToUI(&ConnectSocket,"query=" + std::to_string(query) + '!');
#ifdef CMDWRITETIMESTAMP2KHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdWriteTimestamp2KHR_after(commandBuffer, stage, queryPool, query);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdWriteTimestamp2KHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginRenderingKHR(VkCommandBuffer commandBuffer, VkRenderingInfo* pRenderingInfo) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdBeginRenderingKHR!");
}
#ifdef CMDBEGINRENDERINGKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdBeginRenderingKHR_before(commandBuffer, pRenderingInfo);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginRenderingKHR(commandBuffer, pRenderingInfo);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
winsockSendToUI(&ConnectSocket,"pRenderingInfo=" + ptrToString((void*)pRenderingInfo) + '!');
#ifdef CMDBEGINRENDERINGKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdBeginRenderingKHR_after(commandBuffer, pRenderingInfo);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdBeginRenderingKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndRenderingKHR(VkCommandBuffer commandBuffer) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkCmdEndRenderingKHR!");
}
#ifdef CMDENDRENDERINGKHR_BEFORE_EXEC_EXISTS
if(connected) {
layer_CmdEndRenderingKHR_before(commandBuffer);
}
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndRenderingKHR(commandBuffer);
winsockSendToUI(&ConnectSocket,"commandBuffer=" + ptrToString((void*)commandBuffer) + '!');
#ifdef CMDENDRENDERINGKHR_AFTER_EXEC_EXISTS
if(connected) {
layer_CmdEndRenderingKHR_after(commandBuffer);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkCmdEndRenderingKHR!");
}
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageSubresourceLayout2EXT(VkDevice device, VkImage image, VkImageSubresource2KHR* pSubresource, VkSubresourceLayout2KHR* pLayout) {
scoped_lock l(global_lock);
if(connected) {
winsockSendToUI(&ConnectSocket, "begin_vkGetImageSubresourceLayout2EXT!");
}
#ifdef GETIMAGESUBRESOURCELAYOUT2EXT_BEFORE_EXEC_EXISTS
if(connected) {
layer_GetImageSubresourceLayout2EXT_before(device, image, pSubresource, pLayout);
}
#endif 
device_dispatch[GetKey(device)].GetImageSubresourceLayout2EXT(device, image, pSubresource, pLayout);
winsockSendToUI(&ConnectSocket,"device=" + ptrToString((void*)device) + '!');
winsockSendToUI(&ConnectSocket,"image=" + ptrToString((void*)image) + '!');
winsockSendToUI(&ConnectSocket,"pSubresource=" + ptrToString((void*)pSubresource) + '!');
winsockSendToUI(&ConnectSocket,"pLayout=" + ptrToString((void*)pLayout) + '!');
#ifdef GETIMAGESUBRESOURCELAYOUT2EXT_AFTER_EXEC_EXISTS
if(connected) {
layer_GetImageSubresourceLayout2EXT_after(device, image, pSubresource, pLayout);
}
#endif 
if(connected) {
winsockSendToUI(&ConnectSocket, "end_vkGetImageSubresourceLayout2EXT!");
}
}
#define GETPROCADDR(func) if(!strcmp(pName, "vk" #func)) return (PFN_vkVoidFunction)&DetailsLayer_##func;
VK_LAYER_EXPORT PFN_vkVoidFunction VKAPI_CALL DetailsLayer_GetDeviceProcAddr(VkDevice device, const char* pName) {
if (GetWindowName() == "vkDetails.exe") { return device_dispatch[GetKey(device)].GetDeviceProcAddr(device, pName); }
GETPROCADDR(GetDeviceProcAddr);
GETPROCADDR(DestroyDevice);
GETPROCADDR(GetDeviceQueue);
GETPROCADDR(QueueSubmit);
GETPROCADDR(QueueWaitIdle);
GETPROCADDR(DeviceWaitIdle);
GETPROCADDR(AllocateMemory);
GETPROCADDR(FreeMemory);
GETPROCADDR(MapMemory);
GETPROCADDR(UnmapMemory);
GETPROCADDR(FlushMappedMemoryRanges);
GETPROCADDR(InvalidateMappedMemoryRanges);
GETPROCADDR(GetDeviceMemoryCommitment);
GETPROCADDR(GetBufferMemoryRequirements);
GETPROCADDR(BindBufferMemory);
GETPROCADDR(GetImageMemoryRequirements);
GETPROCADDR(BindImageMemory);
GETPROCADDR(GetImageSparseMemoryRequirements);
GETPROCADDR(QueueBindSparse);
GETPROCADDR(CreateFence);
GETPROCADDR(DestroyFence);
GETPROCADDR(ResetFences);
GETPROCADDR(GetFenceStatus);
GETPROCADDR(WaitForFences);
GETPROCADDR(CreateSemaphore);
GETPROCADDR(DestroySemaphore);
GETPROCADDR(CreateEvent);
GETPROCADDR(DestroyEvent);
GETPROCADDR(GetEventStatus);
GETPROCADDR(SetEvent);
GETPROCADDR(ResetEvent);
GETPROCADDR(CreateQueryPool);
GETPROCADDR(DestroyQueryPool);
GETPROCADDR(GetQueryPoolResults);
GETPROCADDR(ResetQueryPool);
GETPROCADDR(CreateBuffer);
GETPROCADDR(DestroyBuffer);
GETPROCADDR(CreateBufferView);
GETPROCADDR(DestroyBufferView);
GETPROCADDR(CreateImage);
GETPROCADDR(DestroyImage);
GETPROCADDR(GetImageSubresourceLayout);
GETPROCADDR(CreateImageView);
GETPROCADDR(DestroyImageView);
GETPROCADDR(CreateShaderModule);
GETPROCADDR(DestroyShaderModule);
GETPROCADDR(CreatePipelineCache);
GETPROCADDR(DestroyPipelineCache);
GETPROCADDR(GetPipelineCacheData);
GETPROCADDR(MergePipelineCaches);
GETPROCADDR(CreateGraphicsPipelines);
GETPROCADDR(CreateComputePipelines);
GETPROCADDR(GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI);
GETPROCADDR(DestroyPipeline);
GETPROCADDR(CreatePipelineLayout);
GETPROCADDR(DestroyPipelineLayout);
GETPROCADDR(CreateSampler);
GETPROCADDR(DestroySampler);
GETPROCADDR(CreateDescriptorSetLayout);
GETPROCADDR(DestroyDescriptorSetLayout);
GETPROCADDR(CreateDescriptorPool);
GETPROCADDR(DestroyDescriptorPool);
GETPROCADDR(ResetDescriptorPool);
GETPROCADDR(AllocateDescriptorSets);
GETPROCADDR(FreeDescriptorSets);
GETPROCADDR(UpdateDescriptorSets);
GETPROCADDR(CreateFramebuffer);
GETPROCADDR(DestroyFramebuffer);
GETPROCADDR(CreateRenderPass);
GETPROCADDR(DestroyRenderPass);
GETPROCADDR(GetRenderAreaGranularity);
GETPROCADDR(GetRenderingAreaGranularityKHR);
GETPROCADDR(CreateCommandPool);
GETPROCADDR(DestroyCommandPool);
GETPROCADDR(ResetCommandPool);
GETPROCADDR(AllocateCommandBuffers);
GETPROCADDR(FreeCommandBuffers);
GETPROCADDR(BeginCommandBuffer);
GETPROCADDR(EndCommandBuffer);
GETPROCADDR(ResetCommandBuffer);
GETPROCADDR(CmdBindPipeline);
GETPROCADDR(CmdSetAttachmentFeedbackLoopEnableEXT);
GETPROCADDR(CmdSetViewport);
GETPROCADDR(CmdSetScissor);
GETPROCADDR(CmdSetLineWidth);
GETPROCADDR(CmdSetDepthBias);
GETPROCADDR(CmdSetBlendConstants);
GETPROCADDR(CmdSetDepthBounds);
GETPROCADDR(CmdSetStencilCompareMask);
GETPROCADDR(CmdSetStencilWriteMask);
GETPROCADDR(CmdSetStencilReference);
GETPROCADDR(CmdBindDescriptorSets);
GETPROCADDR(CmdBindIndexBuffer);
GETPROCADDR(CmdBindVertexBuffers);
GETPROCADDR(CmdDraw);
GETPROCADDR(CmdDrawIndexed);
GETPROCADDR(CmdDrawMultiEXT);
GETPROCADDR(CmdDrawMultiIndexedEXT);
GETPROCADDR(CmdDrawIndirect);
GETPROCADDR(CmdDrawIndexedIndirect);
GETPROCADDR(CmdDispatch);
GETPROCADDR(CmdDispatchIndirect);
GETPROCADDR(CmdSubpassShadingHUAWEI);
GETPROCADDR(CmdDrawClusterHUAWEI);
GETPROCADDR(CmdDrawClusterIndirectHUAWEI);
GETPROCADDR(CmdUpdatePipelineIndirectBufferNV);
GETPROCADDR(CmdCopyBuffer);
GETPROCADDR(CmdCopyImage);
GETPROCADDR(CmdBlitImage);
GETPROCADDR(CmdCopyBufferToImage);
GETPROCADDR(CmdCopyImageToBuffer);
GETPROCADDR(CmdCopyMemoryIndirectNV);
GETPROCADDR(CmdCopyMemoryToImageIndirectNV);
GETPROCADDR(CmdUpdateBuffer);
GETPROCADDR(CmdFillBuffer);
GETPROCADDR(CmdClearColorImage);
GETPROCADDR(CmdClearDepthStencilImage);
GETPROCADDR(CmdClearAttachments);
GETPROCADDR(CmdResolveImage);
GETPROCADDR(CmdSetEvent);
GETPROCADDR(CmdResetEvent);
GETPROCADDR(CmdWaitEvents);
GETPROCADDR(CmdPipelineBarrier);
GETPROCADDR(CmdBeginQuery);
GETPROCADDR(CmdEndQuery);
GETPROCADDR(CmdBeginConditionalRenderingEXT);
GETPROCADDR(CmdEndConditionalRenderingEXT);
GETPROCADDR(CmdResetQueryPool);
GETPROCADDR(CmdWriteTimestamp);
GETPROCADDR(CmdCopyQueryPoolResults);
GETPROCADDR(CmdPushConstants);
GETPROCADDR(CmdBeginRenderPass);
GETPROCADDR(CmdNextSubpass);
GETPROCADDR(CmdEndRenderPass);
GETPROCADDR(CmdExecuteCommands);
GETPROCADDR(CreateSharedSwapchainsKHR);
GETPROCADDR(CreateSwapchainKHR);
GETPROCADDR(DestroySwapchainKHR);
GETPROCADDR(GetSwapchainImagesKHR);
GETPROCADDR(AcquireNextImageKHR);
GETPROCADDR(QueuePresentKHR);
GETPROCADDR(DebugMarkerSetObjectNameEXT);
GETPROCADDR(DebugMarkerSetObjectTagEXT);
GETPROCADDR(CmdDebugMarkerBeginEXT);
GETPROCADDR(CmdDebugMarkerEndEXT);
GETPROCADDR(CmdDebugMarkerInsertEXT);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(GetMemoryWin32HandleNV);
#endif
GETPROCADDR(CmdExecuteGeneratedCommandsNV);
GETPROCADDR(CmdPreprocessGeneratedCommandsNV);
GETPROCADDR(CmdBindPipelineShaderGroupNV);
GETPROCADDR(GetGeneratedCommandsMemoryRequirementsNV);
GETPROCADDR(CreateIndirectCommandsLayoutNV);
GETPROCADDR(DestroyIndirectCommandsLayoutNV);
GETPROCADDR(CmdPushDescriptorSetKHR);
GETPROCADDR(TrimCommandPool);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(GetMemoryWin32HandleKHR);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(GetMemoryWin32HandlePropertiesKHR);
#endif
GETPROCADDR(GetMemoryFdKHR);
GETPROCADDR(GetMemoryFdPropertiesKHR);
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(GetMemoryZirconHandleFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(GetMemoryZirconHandlePropertiesFUCHSIA);
#endif
GETPROCADDR(GetMemoryRemoteAddressNV);
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(GetMemorySciBufNV);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(GetSemaphoreWin32HandleKHR);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(ImportSemaphoreWin32HandleKHR);
#endif
GETPROCADDR(GetSemaphoreFdKHR);
GETPROCADDR(ImportSemaphoreFdKHR);
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(GetSemaphoreZirconHandleFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(ImportSemaphoreZirconHandleFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(GetFenceWin32HandleKHR);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(ImportFenceWin32HandleKHR);
#endif
GETPROCADDR(GetFenceFdKHR);
GETPROCADDR(ImportFenceFdKHR);
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(GetFenceSciSyncFenceNV);
#endif
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(GetFenceSciSyncObjNV);
#endif
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(ImportFenceSciSyncFenceNV);
#endif
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(ImportFenceSciSyncObjNV);
#endif
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(GetSemaphoreSciSyncObjNV);
#endif
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(ImportSemaphoreSciSyncObjNV);
#endif
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(CreateSemaphoreSciSyncPoolNV);
#endif
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(DestroySemaphoreSciSyncPoolNV);
#endif
GETPROCADDR(DisplayPowerControlEXT);
GETPROCADDR(RegisterDeviceEventEXT);
GETPROCADDR(RegisterDisplayEventEXT);
GETPROCADDR(GetSwapchainCounterEXT);
GETPROCADDR(GetDeviceGroupPeerMemoryFeatures);
GETPROCADDR(BindBufferMemory2);
GETPROCADDR(BindImageMemory2);
GETPROCADDR(CmdSetDeviceMask);
GETPROCADDR(GetDeviceGroupPresentCapabilitiesKHR);
GETPROCADDR(GetDeviceGroupSurfacePresentModesKHR);
GETPROCADDR(AcquireNextImage2KHR);
GETPROCADDR(CmdDispatchBase);
GETPROCADDR(CreateDescriptorUpdateTemplate);
GETPROCADDR(DestroyDescriptorUpdateTemplate);
GETPROCADDR(UpdateDescriptorSetWithTemplate);
GETPROCADDR(CmdPushDescriptorSetWithTemplateKHR);
GETPROCADDR(SetHdrMetadataEXT);
GETPROCADDR(GetSwapchainStatusKHR);
GETPROCADDR(GetRefreshCycleDurationGOOGLE);
GETPROCADDR(GetPastPresentationTimingGOOGLE);
GETPROCADDR(CmdSetViewportWScalingNV);
GETPROCADDR(CmdSetDiscardRectangleEXT);
GETPROCADDR(CmdSetDiscardRectangleEnableEXT);
GETPROCADDR(CmdSetDiscardRectangleModeEXT);
GETPROCADDR(CmdSetSampleLocationsEXT);
GETPROCADDR(GetBufferMemoryRequirements2);
GETPROCADDR(GetImageMemoryRequirements2);
GETPROCADDR(GetImageSparseMemoryRequirements2);
GETPROCADDR(GetDeviceBufferMemoryRequirements);
GETPROCADDR(GetDeviceImageMemoryRequirements);
GETPROCADDR(GetDeviceImageSparseMemoryRequirements);
GETPROCADDR(CreateSamplerYcbcrConversion);
GETPROCADDR(DestroySamplerYcbcrConversion);
GETPROCADDR(GetDeviceQueue2);
GETPROCADDR(CreateValidationCacheEXT);
GETPROCADDR(DestroyValidationCacheEXT);
GETPROCADDR(GetValidationCacheDataEXT);
GETPROCADDR(MergeValidationCachesEXT);
GETPROCADDR(GetDescriptorSetLayoutSupport);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
GETPROCADDR(GetSwapchainGrallocUsageANDROID);
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
GETPROCADDR(GetSwapchainGrallocUsage2ANDROID);
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
GETPROCADDR(AcquireImageANDROID);
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
GETPROCADDR(QueueSignalReleaseImageANDROID);
#endif
GETPROCADDR(GetShaderInfoAMD);
GETPROCADDR(SetLocalDimmingAMD);
GETPROCADDR(GetCalibratedTimestampsKHR);
GETPROCADDR(SetDebugUtilsObjectNameEXT);
GETPROCADDR(SetDebugUtilsObjectTagEXT);
GETPROCADDR(QueueBeginDebugUtilsLabelEXT);
GETPROCADDR(QueueEndDebugUtilsLabelEXT);
GETPROCADDR(QueueInsertDebugUtilsLabelEXT);
GETPROCADDR(CmdBeginDebugUtilsLabelEXT);
GETPROCADDR(CmdEndDebugUtilsLabelEXT);
GETPROCADDR(CmdInsertDebugUtilsLabelEXT);
GETPROCADDR(GetMemoryHostPointerPropertiesEXT);
GETPROCADDR(CmdWriteBufferMarkerAMD);
GETPROCADDR(CreateRenderPass2);
GETPROCADDR(CmdBeginRenderPass2);
GETPROCADDR(CmdNextSubpass2);
GETPROCADDR(CmdEndRenderPass2);
GETPROCADDR(GetSemaphoreCounterValue);
GETPROCADDR(WaitSemaphores);
GETPROCADDR(SignalSemaphore);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
GETPROCADDR(GetAndroidHardwareBufferPropertiesANDROID);
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
GETPROCADDR(GetMemoryAndroidHardwareBufferANDROID);
#endif
GETPROCADDR(CmdDrawIndirectCount);
GETPROCADDR(CmdDrawIndexedIndirectCount);
GETPROCADDR(CmdSetCheckpointNV);
GETPROCADDR(GetQueueCheckpointDataNV);
GETPROCADDR(CmdBindTransformFeedbackBuffersEXT);
GETPROCADDR(CmdBeginTransformFeedbackEXT);
GETPROCADDR(CmdEndTransformFeedbackEXT);
GETPROCADDR(CmdBeginQueryIndexedEXT);
GETPROCADDR(CmdEndQueryIndexedEXT);
GETPROCADDR(CmdDrawIndirectByteCountEXT);
GETPROCADDR(CmdSetExclusiveScissorNV);
GETPROCADDR(CmdSetExclusiveScissorEnableNV);
GETPROCADDR(CmdBindShadingRateImageNV);
GETPROCADDR(CmdSetViewportShadingRatePaletteNV);
GETPROCADDR(CmdSetCoarseSampleOrderNV);
GETPROCADDR(CmdDrawMeshTasksNV);
GETPROCADDR(CmdDrawMeshTasksIndirectNV);
GETPROCADDR(CmdDrawMeshTasksIndirectCountNV);
GETPROCADDR(CmdDrawMeshTasksEXT);
GETPROCADDR(CmdDrawMeshTasksIndirectEXT);
GETPROCADDR(CmdDrawMeshTasksIndirectCountEXT);
GETPROCADDR(CompileDeferredNV);
GETPROCADDR(CreateAccelerationStructureNV);
GETPROCADDR(CmdBindInvocationMaskHUAWEI);
GETPROCADDR(DestroyAccelerationStructureKHR);
GETPROCADDR(DestroyAccelerationStructureNV);
GETPROCADDR(GetAccelerationStructureMemoryRequirementsNV);
GETPROCADDR(BindAccelerationStructureMemoryNV);
GETPROCADDR(CmdCopyAccelerationStructureNV);
GETPROCADDR(CmdCopyAccelerationStructureKHR);
GETPROCADDR(CopyAccelerationStructureKHR);
GETPROCADDR(CmdCopyAccelerationStructureToMemoryKHR);
GETPROCADDR(CopyAccelerationStructureToMemoryKHR);
GETPROCADDR(CmdCopyMemoryToAccelerationStructureKHR);
GETPROCADDR(CopyMemoryToAccelerationStructureKHR);
GETPROCADDR(CmdWriteAccelerationStructuresPropertiesKHR);
GETPROCADDR(CmdWriteAccelerationStructuresPropertiesNV);
GETPROCADDR(CmdBuildAccelerationStructureNV);
GETPROCADDR(WriteAccelerationStructuresPropertiesKHR);
GETPROCADDR(CmdTraceRaysKHR);
GETPROCADDR(CmdTraceRaysNV);
GETPROCADDR(GetRayTracingShaderGroupHandlesKHR);
GETPROCADDR(GetRayTracingCaptureReplayShaderGroupHandlesKHR);
GETPROCADDR(GetAccelerationStructureHandleNV);
GETPROCADDR(CreateRayTracingPipelinesNV);
GETPROCADDR(CreateRayTracingPipelinesKHR);
GETPROCADDR(CmdTraceRaysIndirectKHR);
GETPROCADDR(CmdTraceRaysIndirect2KHR);
GETPROCADDR(GetDeviceAccelerationStructureCompatibilityKHR);
GETPROCADDR(GetRayTracingShaderGroupStackSizeKHR);
GETPROCADDR(CmdSetRayTracingPipelineStackSizeKHR);
GETPROCADDR(GetImageViewHandleNVX);
GETPROCADDR(GetImageViewAddressNVX);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(GetDeviceGroupSurfacePresentModes2EXT);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(AcquireFullScreenExclusiveModeEXT);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(ReleaseFullScreenExclusiveModeEXT);
#endif
GETPROCADDR(AcquireProfilingLockKHR);
GETPROCADDR(ReleaseProfilingLockKHR);
GETPROCADDR(GetImageDrmFormatModifierPropertiesEXT);
GETPROCADDR(GetBufferOpaqueCaptureAddress);
GETPROCADDR(GetBufferDeviceAddress);
GETPROCADDR(InitializePerformanceApiINTEL);
GETPROCADDR(UninitializePerformanceApiINTEL);
GETPROCADDR(CmdSetPerformanceMarkerINTEL);
GETPROCADDR(CmdSetPerformanceStreamMarkerINTEL);
GETPROCADDR(CmdSetPerformanceOverrideINTEL);
GETPROCADDR(AcquirePerformanceConfigurationINTEL);
GETPROCADDR(ReleasePerformanceConfigurationINTEL);
GETPROCADDR(QueueSetPerformanceConfigurationINTEL);
GETPROCADDR(GetPerformanceParameterINTEL);
GETPROCADDR(GetDeviceMemoryOpaqueCaptureAddress);
GETPROCADDR(GetPipelineExecutablePropertiesKHR);
GETPROCADDR(GetPipelineExecutableStatisticsKHR);
GETPROCADDR(GetPipelineExecutableInternalRepresentationsKHR);
GETPROCADDR(CreateAccelerationStructureKHR);
GETPROCADDR(CmdBuildAccelerationStructuresKHR);
GETPROCADDR(CmdBuildAccelerationStructuresIndirectKHR);
GETPROCADDR(BuildAccelerationStructuresKHR);
GETPROCADDR(GetAccelerationStructureDeviceAddressKHR);
GETPROCADDR(CreateDeferredOperationKHR);
GETPROCADDR(DestroyDeferredOperationKHR);
GETPROCADDR(GetDeferredOperationMaxConcurrencyKHR);
GETPROCADDR(GetDeferredOperationResultKHR);
GETPROCADDR(DeferredOperationJoinKHR);
GETPROCADDR(GetPipelineIndirectMemoryRequirementsNV);
GETPROCADDR(GetPipelineIndirectDeviceAddressNV);
GETPROCADDR(CmdSetCullMode);
GETPROCADDR(CmdSetFrontFace);
GETPROCADDR(CmdSetPrimitiveTopology);
GETPROCADDR(CmdSetViewportWithCount);
GETPROCADDR(CmdSetScissorWithCount);
GETPROCADDR(CmdBindIndexBuffer2KHR);
GETPROCADDR(CmdBindVertexBuffers2);
GETPROCADDR(CmdSetDepthTestEnable);
GETPROCADDR(CmdSetDepthWriteEnable);
GETPROCADDR(CmdSetDepthCompareOp);
GETPROCADDR(CmdSetDepthBoundsTestEnable);
GETPROCADDR(CmdSetStencilTestEnable);
GETPROCADDR(CmdSetStencilOp);
GETPROCADDR(CmdSetPatchControlPointsEXT);
GETPROCADDR(CmdSetRasterizerDiscardEnable);
GETPROCADDR(CmdSetDepthBiasEnable);
GETPROCADDR(CmdSetLogicOpEXT);
GETPROCADDR(CmdSetPrimitiveRestartEnable);
GETPROCADDR(CmdSetTessellationDomainOriginEXT);
GETPROCADDR(CmdSetDepthClampEnableEXT);
GETPROCADDR(CmdSetPolygonModeEXT);
GETPROCADDR(CmdSetRasterizationSamplesEXT);
GETPROCADDR(CmdSetSampleMaskEXT);
GETPROCADDR(CmdSetAlphaToCoverageEnableEXT);
GETPROCADDR(CmdSetAlphaToOneEnableEXT);
GETPROCADDR(CmdSetLogicOpEnableEXT);
GETPROCADDR(CmdSetColorBlendEnableEXT);
GETPROCADDR(CmdSetColorBlendEquationEXT);
GETPROCADDR(CmdSetColorWriteMaskEXT);
GETPROCADDR(CmdSetRasterizationStreamEXT);
GETPROCADDR(CmdSetConservativeRasterizationModeEXT);
GETPROCADDR(CmdSetExtraPrimitiveOverestimationSizeEXT);
GETPROCADDR(CmdSetDepthClipEnableEXT);
GETPROCADDR(CmdSetSampleLocationsEnableEXT);
GETPROCADDR(CmdSetColorBlendAdvancedEXT);
GETPROCADDR(CmdSetProvokingVertexModeEXT);
GETPROCADDR(CmdSetLineRasterizationModeEXT);
GETPROCADDR(CmdSetLineStippleEnableEXT);
GETPROCADDR(CmdSetDepthClipNegativeOneToOneEXT);
GETPROCADDR(CmdSetViewportWScalingEnableNV);
GETPROCADDR(CmdSetViewportSwizzleNV);
GETPROCADDR(CmdSetCoverageToColorEnableNV);
GETPROCADDR(CmdSetCoverageToColorLocationNV);
GETPROCADDR(CmdSetCoverageModulationModeNV);
GETPROCADDR(CmdSetCoverageModulationTableEnableNV);
GETPROCADDR(CmdSetCoverageModulationTableNV);
GETPROCADDR(CmdSetShadingRateImageEnableNV);
GETPROCADDR(CmdSetCoverageReductionModeNV);
GETPROCADDR(CmdSetRepresentativeFragmentTestEnableNV);
GETPROCADDR(CreatePrivateDataSlot);
GETPROCADDR(DestroyPrivateDataSlot);
GETPROCADDR(SetPrivateData);
GETPROCADDR(GetPrivateData);
GETPROCADDR(CmdCopyBuffer2);
GETPROCADDR(CmdCopyImage2);
GETPROCADDR(CmdBlitImage2);
GETPROCADDR(CmdCopyBufferToImage2);
GETPROCADDR(CmdCopyImageToBuffer2);
GETPROCADDR(CmdResolveImage2);
GETPROCADDR(CmdSetFragmentShadingRateKHR);
GETPROCADDR(CmdSetFragmentShadingRateEnumNV);
GETPROCADDR(GetAccelerationStructureBuildSizesKHR);
GETPROCADDR(CmdSetVertexInputEXT);
GETPROCADDR(CmdSetColorWriteEnableEXT);
GETPROCADDR(CmdSetEvent2);
GETPROCADDR(CmdResetEvent2);
GETPROCADDR(CmdWaitEvents2);
GETPROCADDR(CmdPipelineBarrier2);
GETPROCADDR(QueueSubmit2);
GETPROCADDR(CmdWriteTimestamp2);
GETPROCADDR(CmdWriteBufferMarker2AMD);
GETPROCADDR(GetQueueCheckpointData2NV);
GETPROCADDR(CopyMemoryToImageEXT);
GETPROCADDR(CopyImageToMemoryEXT);
GETPROCADDR(CopyImageToImageEXT);
GETPROCADDR(TransitionImageLayoutEXT);
GETPROCADDR(CreateVideoSessionKHR);
GETPROCADDR(DestroyVideoSessionKHR);
GETPROCADDR(CreateVideoSessionParametersKHR);
GETPROCADDR(UpdateVideoSessionParametersKHR);
GETPROCADDR(GetEncodedVideoSessionParametersKHR);
GETPROCADDR(DestroyVideoSessionParametersKHR);
GETPROCADDR(GetVideoSessionMemoryRequirementsKHR);
GETPROCADDR(BindVideoSessionMemoryKHR);
GETPROCADDR(CmdDecodeVideoKHR);
GETPROCADDR(CmdBeginVideoCodingKHR);
GETPROCADDR(CmdControlVideoCodingKHR);
GETPROCADDR(CmdEndVideoCodingKHR);
GETPROCADDR(CmdEncodeVideoKHR);
GETPROCADDR(CmdDecompressMemoryNV);
GETPROCADDR(CmdDecompressMemoryIndirectCountNV);
GETPROCADDR(CreateCuModuleNVX);
GETPROCADDR(CreateCuFunctionNVX);
GETPROCADDR(DestroyCuModuleNVX);
GETPROCADDR(DestroyCuFunctionNVX);
GETPROCADDR(CmdCuLaunchKernelNVX);
GETPROCADDR(GetDescriptorSetLayoutSizeEXT);
GETPROCADDR(GetDescriptorSetLayoutBindingOffsetEXT);
GETPROCADDR(GetDescriptorEXT);
GETPROCADDR(CmdBindDescriptorBuffersEXT);
GETPROCADDR(CmdSetDescriptorBufferOffsetsEXT);
GETPROCADDR(CmdBindDescriptorBufferEmbeddedSamplersEXT);
GETPROCADDR(GetBufferOpaqueCaptureDescriptorDataEXT);
GETPROCADDR(GetImageOpaqueCaptureDescriptorDataEXT);
GETPROCADDR(GetImageViewOpaqueCaptureDescriptorDataEXT);
GETPROCADDR(GetSamplerOpaqueCaptureDescriptorDataEXT);
GETPROCADDR(GetAccelerationStructureOpaqueCaptureDescriptorDataEXT);
GETPROCADDR(SetDeviceMemoryPriorityEXT);
GETPROCADDR(WaitForPresentKHR);
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(CreateBufferCollectionFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(SetBufferCollectionBufferConstraintsFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(SetBufferCollectionImageConstraintsFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(DestroyBufferCollectionFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(GetBufferCollectionPropertiesFUCHSIA);
#endif
GETPROCADDR(CreateCudaModuleNV);
GETPROCADDR(GetCudaModuleCacheNV);
GETPROCADDR(CreateCudaFunctionNV);
GETPROCADDR(DestroyCudaModuleNV);
GETPROCADDR(DestroyCudaFunctionNV);
GETPROCADDR(CmdCudaLaunchKernelNV);
GETPROCADDR(CmdBeginRendering);
GETPROCADDR(CmdEndRendering);
GETPROCADDR(GetDescriptorSetLayoutHostMappingInfoVALVE);
GETPROCADDR(GetDescriptorSetHostMappingVALVE);
GETPROCADDR(CreateMicromapEXT);
GETPROCADDR(CmdBuildMicromapsEXT);
GETPROCADDR(BuildMicromapsEXT);
GETPROCADDR(DestroyMicromapEXT);
GETPROCADDR(CmdCopyMicromapEXT);
GETPROCADDR(CopyMicromapEXT);
GETPROCADDR(CmdCopyMicromapToMemoryEXT);
GETPROCADDR(CopyMicromapToMemoryEXT);
GETPROCADDR(CmdCopyMemoryToMicromapEXT);
GETPROCADDR(CopyMemoryToMicromapEXT);
GETPROCADDR(CmdWriteMicromapsPropertiesEXT);
GETPROCADDR(WriteMicromapsPropertiesEXT);
GETPROCADDR(GetDeviceMicromapCompatibilityEXT);
GETPROCADDR(GetMicromapBuildSizesEXT);
GETPROCADDR(GetShaderModuleIdentifierEXT);
GETPROCADDR(GetShaderModuleCreateInfoIdentifierEXT);
GETPROCADDR(GetImageSubresourceLayout2KHR);
GETPROCADDR(GetPipelinePropertiesEXT);
#if defined(VK_USE_PLATFORM_METAL_EXT)
GETPROCADDR(ExportMetalObjectsEXT);
#endif
GETPROCADDR(GetFramebufferTilePropertiesQCOM);
GETPROCADDR(GetDynamicRenderingTilePropertiesQCOM);
GETPROCADDR(CreateOpticalFlowSessionNV);
GETPROCADDR(DestroyOpticalFlowSessionNV);
GETPROCADDR(BindOpticalFlowSessionImageNV);
GETPROCADDR(CmdOpticalFlowExecuteNV);
GETPROCADDR(GetDeviceFaultInfoEXT);
GETPROCADDR(CmdSetDepthBias2EXT);
GETPROCADDR(ReleaseSwapchainImagesEXT);
GETPROCADDR(GetDeviceImageSubresourceLayoutKHR);
GETPROCADDR(MapMemory2KHR);
GETPROCADDR(UnmapMemory2KHR);
GETPROCADDR(CreateShadersEXT);
GETPROCADDR(DestroyShaderEXT);
GETPROCADDR(GetShaderBinaryDataEXT);
GETPROCADDR(CmdBindShadersEXT);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
GETPROCADDR(GetScreenBufferPropertiesQNX);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
GETPROCADDR(GetExecutionGraphPipelineScratchSizeAMDX);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
GETPROCADDR(GetExecutionGraphPipelineNodeIndexAMDX);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
GETPROCADDR(CreateExecutionGraphPipelinesAMDX);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
GETPROCADDR(CmdInitializeGraphScratchMemoryAMDX);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
GETPROCADDR(CmdDispatchGraphAMDX);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
GETPROCADDR(CmdDispatchGraphIndirectAMDX);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
GETPROCADDR(CmdDispatchGraphIndirectCountAMDX);
#endif
GETPROCADDR(CmdBindDescriptorSets2KHR);
GETPROCADDR(CmdPushConstants2KHR);
GETPROCADDR(CmdPushDescriptorSet2KHR);
GETPROCADDR(CmdPushDescriptorSetWithTemplate2KHR);
GETPROCADDR(CmdSetDescriptorBufferOffsets2EXT);
GETPROCADDR(CmdBindDescriptorBufferEmbeddedSamplers2EXT);
GETPROCADDR(SetLatencySleepModeNV);
GETPROCADDR(LatencySleepNV);
GETPROCADDR(SetLatencyMarkerNV);
GETPROCADDR(GetLatencyTimingsNV);
GETPROCADDR(QueueNotifyOutOfBandNV);
GETPROCADDR(ResetQueryPoolEXT);
GETPROCADDR(TrimCommandPoolKHR);
GETPROCADDR(GetDeviceGroupPeerMemoryFeaturesKHR);
GETPROCADDR(BindBufferMemory2KHR);
GETPROCADDR(BindImageMemory2KHR);
GETPROCADDR(CmdSetDeviceMaskKHR);
GETPROCADDR(CmdDispatchBaseKHR);
GETPROCADDR(CreateDescriptorUpdateTemplateKHR);
GETPROCADDR(DestroyDescriptorUpdateTemplateKHR);
GETPROCADDR(UpdateDescriptorSetWithTemplateKHR);
GETPROCADDR(GetBufferMemoryRequirements2KHR);
GETPROCADDR(GetImageMemoryRequirements2KHR);
GETPROCADDR(GetImageSparseMemoryRequirements2KHR);
GETPROCADDR(GetDeviceBufferMemoryRequirementsKHR);
GETPROCADDR(GetDeviceImageMemoryRequirementsKHR);
GETPROCADDR(GetDeviceImageSparseMemoryRequirementsKHR);
GETPROCADDR(CreateSamplerYcbcrConversionKHR);
GETPROCADDR(DestroySamplerYcbcrConversionKHR);
GETPROCADDR(GetDescriptorSetLayoutSupportKHR);
GETPROCADDR(GetCalibratedTimestampsEXT);
GETPROCADDR(CreateRenderPass2KHR);
GETPROCADDR(CmdBeginRenderPass2KHR);
GETPROCADDR(CmdNextSubpass2KHR);
GETPROCADDR(CmdEndRenderPass2KHR);
GETPROCADDR(GetSemaphoreCounterValueKHR);
GETPROCADDR(WaitSemaphoresKHR);
GETPROCADDR(SignalSemaphoreKHR);
GETPROCADDR(CmdDrawIndirectCountKHR);
GETPROCADDR(CmdDrawIndirectCountAMD);
GETPROCADDR(CmdDrawIndexedIndirectCountKHR);
GETPROCADDR(CmdDrawIndexedIndirectCountAMD);
GETPROCADDR(GetRayTracingShaderGroupHandlesNV);
GETPROCADDR(GetBufferOpaqueCaptureAddressKHR);
GETPROCADDR(GetBufferDeviceAddressKHR);
GETPROCADDR(GetBufferDeviceAddressEXT);
GETPROCADDR(GetDeviceMemoryOpaqueCaptureAddressKHR);
GETPROCADDR(CmdSetLineStippleEXT);
GETPROCADDR(CmdSetCullModeEXT);
GETPROCADDR(CmdSetFrontFaceEXT);
GETPROCADDR(CmdSetPrimitiveTopologyEXT);
GETPROCADDR(CmdSetViewportWithCountEXT);
GETPROCADDR(CmdSetScissorWithCountEXT);
GETPROCADDR(CmdBindVertexBuffers2EXT);
GETPROCADDR(CmdSetDepthTestEnableEXT);
GETPROCADDR(CmdSetDepthWriteEnableEXT);
GETPROCADDR(CmdSetDepthCompareOpEXT);
GETPROCADDR(CmdSetDepthBoundsTestEnableEXT);
GETPROCADDR(CmdSetStencilTestEnableEXT);
GETPROCADDR(CmdSetStencilOpEXT);
GETPROCADDR(CmdSetRasterizerDiscardEnableEXT);
GETPROCADDR(CmdSetDepthBiasEnableEXT);
GETPROCADDR(CmdSetPrimitiveRestartEnableEXT);
GETPROCADDR(CreatePrivateDataSlotEXT);
GETPROCADDR(DestroyPrivateDataSlotEXT);
GETPROCADDR(SetPrivateDataEXT);
GETPROCADDR(GetPrivateDataEXT);
GETPROCADDR(CmdCopyBuffer2KHR);
GETPROCADDR(CmdCopyImage2KHR);
GETPROCADDR(CmdBlitImage2KHR);
GETPROCADDR(CmdCopyBufferToImage2KHR);
GETPROCADDR(CmdCopyImageToBuffer2KHR);
GETPROCADDR(CmdResolveImage2KHR);
GETPROCADDR(CmdSetEvent2KHR);
GETPROCADDR(CmdResetEvent2KHR);
GETPROCADDR(CmdWaitEvents2KHR);
GETPROCADDR(CmdPipelineBarrier2KHR);
GETPROCADDR(QueueSubmit2KHR);
GETPROCADDR(CmdWriteTimestamp2KHR);
GETPROCADDR(CmdBeginRenderingKHR);
GETPROCADDR(CmdEndRenderingKHR);
GETPROCADDR(GetImageSubresourceLayout2EXT);
scoped_lock l(global_lock);
return device_dispatch[GetKey(device)].GetDeviceProcAddr(device, pName);}
VK_LAYER_EXPORT PFN_vkVoidFunction VKAPI_CALL DetailsLayer_GetInstanceProcAddr(VkInstance instance, const char* pName) {
if (GetWindowName() == "vkDetails.exe") { GETPROCADDR(CreateDevice); GETPROCADDR(CreateInstance); return instance_dispatch[GetKey(instance)].GetInstanceProcAddr(instance, pName); }
GETPROCADDR(CreateInstance);
GETPROCADDR(DestroyInstance);
GETPROCADDR(EnumeratePhysicalDevices);
GETPROCADDR(GetInstanceProcAddr);
GETPROCADDR(GetPhysicalDeviceProperties);
GETPROCADDR(GetPhysicalDeviceQueueFamilyProperties);
GETPROCADDR(GetPhysicalDeviceMemoryProperties);
GETPROCADDR(GetPhysicalDeviceFeatures);
GETPROCADDR(GetPhysicalDeviceFormatProperties);
GETPROCADDR(GetPhysicalDeviceImageFormatProperties);
GETPROCADDR(CreateDevice);
GETPROCADDR(EnumerateInstanceExtensionProperties);
GETPROCADDR(EnumerateDeviceLayerProperties);
GETPROCADDR(EnumerateDeviceExtensionProperties);
GETPROCADDR(GetPhysicalDeviceSparseImageFormatProperties);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
GETPROCADDR(CreateAndroidSurfaceKHR);
#endif
GETPROCADDR(GetPhysicalDeviceDisplayPropertiesKHR);
GETPROCADDR(GetPhysicalDeviceDisplayPlanePropertiesKHR);
GETPROCADDR(GetDisplayPlaneSupportedDisplaysKHR);
GETPROCADDR(GetDisplayModePropertiesKHR);
GETPROCADDR(CreateDisplayModeKHR);
GETPROCADDR(GetDisplayPlaneCapabilitiesKHR);
GETPROCADDR(CreateDisplayPlaneSurfaceKHR);
GETPROCADDR(DestroySurfaceKHR);
GETPROCADDR(GetPhysicalDeviceSurfaceSupportKHR);
GETPROCADDR(GetPhysicalDeviceSurfaceCapabilitiesKHR);
GETPROCADDR(GetPhysicalDeviceSurfaceFormatsKHR);
GETPROCADDR(GetPhysicalDeviceSurfacePresentModesKHR);
#if defined(VK_USE_PLATFORM_VI_NN)
GETPROCADDR(CreateViSurfaceNN);
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
GETPROCADDR(CreateWaylandSurfaceKHR);
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
GETPROCADDR(GetPhysicalDeviceWaylandPresentationSupportKHR);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(CreateWin32SurfaceKHR);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(GetPhysicalDeviceWin32PresentationSupportKHR);
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR)
GETPROCADDR(CreateXlibSurfaceKHR);
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR)
GETPROCADDR(GetPhysicalDeviceXlibPresentationSupportKHR);
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
GETPROCADDR(CreateXcbSurfaceKHR);
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
GETPROCADDR(GetPhysicalDeviceXcbPresentationSupportKHR);
#endif
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
GETPROCADDR(CreateDirectFBSurfaceEXT);
#endif
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
GETPROCADDR(GetPhysicalDeviceDirectFBPresentationSupportEXT);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(CreateImagePipeSurfaceFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_GGP)
GETPROCADDR(CreateStreamDescriptorSurfaceGGP);
#endif
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
GETPROCADDR(CreateScreenSurfaceQNX);
#endif
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
GETPROCADDR(GetPhysicalDeviceScreenPresentationSupportQNX);
#endif
GETPROCADDR(CreateDebugReportCallbackEXT);
GETPROCADDR(DestroyDebugReportCallbackEXT);
GETPROCADDR(DebugReportMessageEXT);
GETPROCADDR(GetPhysicalDeviceExternalImageFormatPropertiesNV);
GETPROCADDR(GetPhysicalDeviceFeatures2);
GETPROCADDR(GetPhysicalDeviceProperties2);
GETPROCADDR(GetPhysicalDeviceFormatProperties2);
GETPROCADDR(GetPhysicalDeviceImageFormatProperties2);
GETPROCADDR(GetPhysicalDeviceQueueFamilyProperties2);
GETPROCADDR(GetPhysicalDeviceMemoryProperties2);
GETPROCADDR(GetPhysicalDeviceSparseImageFormatProperties2);
GETPROCADDR(GetPhysicalDeviceExternalBufferProperties);
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(GetPhysicalDeviceExternalMemorySciBufPropertiesNV);
#endif
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(GetPhysicalDeviceSciBufAttributesNV);
#endif
GETPROCADDR(GetPhysicalDeviceExternalSemaphoreProperties);
GETPROCADDR(GetPhysicalDeviceExternalFenceProperties);
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(GetPhysicalDeviceSciSyncAttributesNV);
#endif
GETPROCADDR(ReleaseDisplayEXT);
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
GETPROCADDR(AcquireXlibDisplayEXT);
#endif
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
GETPROCADDR(GetRandROutputDisplayEXT);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(AcquireWinrtDisplayNV);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(GetWinrtDisplayNV);
#endif
GETPROCADDR(GetPhysicalDeviceSurfaceCapabilities2EXT);
GETPROCADDR(EnumeratePhysicalDeviceGroups);
GETPROCADDR(GetPhysicalDevicePresentRectanglesKHR);
#if defined(VK_USE_PLATFORM_IOS_MVK)
GETPROCADDR(CreateIOSSurfaceMVK);
#endif
#if defined(VK_USE_PLATFORM_MACOS_MVK)
GETPROCADDR(CreateMacOSSurfaceMVK);
#endif
#if defined(VK_USE_PLATFORM_METAL_EXT)
GETPROCADDR(CreateMetalSurfaceEXT);
#endif
GETPROCADDR(GetPhysicalDeviceMultisamplePropertiesEXT);
GETPROCADDR(GetPhysicalDeviceSurfaceCapabilities2KHR);
GETPROCADDR(GetPhysicalDeviceSurfaceFormats2KHR);
GETPROCADDR(GetPhysicalDeviceDisplayProperties2KHR);
GETPROCADDR(GetPhysicalDeviceDisplayPlaneProperties2KHR);
GETPROCADDR(GetDisplayModeProperties2KHR);
GETPROCADDR(GetDisplayPlaneCapabilities2KHR);
GETPROCADDR(GetPhysicalDeviceCalibrateableTimeDomainsKHR);
GETPROCADDR(CreateDebugUtilsMessengerEXT);
GETPROCADDR(DestroyDebugUtilsMessengerEXT);
GETPROCADDR(SubmitDebugUtilsMessageEXT);
GETPROCADDR(GetPhysicalDeviceCooperativeMatrixPropertiesNV);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(GetPhysicalDeviceSurfacePresentModes2EXT);
#endif
GETPROCADDR(EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR);
GETPROCADDR(GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR);
GETPROCADDR(CreateHeadlessSurfaceEXT);
GETPROCADDR(GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV);
GETPROCADDR(GetPhysicalDeviceToolProperties);
GETPROCADDR(GetPhysicalDeviceFragmentShadingRatesKHR);
GETPROCADDR(GetPhysicalDeviceVideoCapabilitiesKHR);
GETPROCADDR(GetPhysicalDeviceVideoFormatPropertiesKHR);
GETPROCADDR(GetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR);
GETPROCADDR(AcquireDrmDisplayEXT);
GETPROCADDR(GetDrmDisplayEXT);
GETPROCADDR(GetPhysicalDeviceOpticalFlowImageFormatsNV);
GETPROCADDR(GetPhysicalDeviceCooperativeMatrixPropertiesKHR);
GETPROCADDR(GetPhysicalDeviceFeatures2KHR);
GETPROCADDR(GetPhysicalDeviceProperties2KHR);
GETPROCADDR(GetPhysicalDeviceFormatProperties2KHR);
GETPROCADDR(GetPhysicalDeviceImageFormatProperties2KHR);
GETPROCADDR(GetPhysicalDeviceQueueFamilyProperties2KHR);
GETPROCADDR(GetPhysicalDeviceMemoryProperties2KHR);
GETPROCADDR(GetPhysicalDeviceSparseImageFormatProperties2KHR);
GETPROCADDR(GetPhysicalDeviceExternalBufferPropertiesKHR);
GETPROCADDR(GetPhysicalDeviceExternalSemaphorePropertiesKHR);
GETPROCADDR(GetPhysicalDeviceExternalFencePropertiesKHR);
GETPROCADDR(EnumeratePhysicalDeviceGroupsKHR);
GETPROCADDR(GetPhysicalDeviceCalibrateableTimeDomainsEXT);
GETPROCADDR(GetPhysicalDeviceToolPropertiesEXT);
GETPROCADDR(GetDeviceProcAddr);
GETPROCADDR(DestroyDevice);
GETPROCADDR(GetDeviceQueue);
GETPROCADDR(QueueSubmit);
GETPROCADDR(QueueWaitIdle);
GETPROCADDR(DeviceWaitIdle);
GETPROCADDR(AllocateMemory);
GETPROCADDR(FreeMemory);
GETPROCADDR(MapMemory);
GETPROCADDR(UnmapMemory);
GETPROCADDR(FlushMappedMemoryRanges);
GETPROCADDR(InvalidateMappedMemoryRanges);
GETPROCADDR(GetDeviceMemoryCommitment);
GETPROCADDR(GetBufferMemoryRequirements);
GETPROCADDR(BindBufferMemory);
GETPROCADDR(GetImageMemoryRequirements);
GETPROCADDR(BindImageMemory);
GETPROCADDR(GetImageSparseMemoryRequirements);
GETPROCADDR(QueueBindSparse);
GETPROCADDR(CreateFence);
GETPROCADDR(DestroyFence);
GETPROCADDR(ResetFences);
GETPROCADDR(GetFenceStatus);
GETPROCADDR(WaitForFences);
GETPROCADDR(CreateSemaphore);
GETPROCADDR(DestroySemaphore);
GETPROCADDR(CreateEvent);
GETPROCADDR(DestroyEvent);
GETPROCADDR(GetEventStatus);
GETPROCADDR(SetEvent);
GETPROCADDR(ResetEvent);
GETPROCADDR(CreateQueryPool);
GETPROCADDR(DestroyQueryPool);
GETPROCADDR(GetQueryPoolResults);
GETPROCADDR(ResetQueryPool);
GETPROCADDR(CreateBuffer);
GETPROCADDR(DestroyBuffer);
GETPROCADDR(CreateBufferView);
GETPROCADDR(DestroyBufferView);
GETPROCADDR(CreateImage);
GETPROCADDR(DestroyImage);
GETPROCADDR(GetImageSubresourceLayout);
GETPROCADDR(CreateImageView);
GETPROCADDR(DestroyImageView);
GETPROCADDR(CreateShaderModule);
GETPROCADDR(DestroyShaderModule);
GETPROCADDR(CreatePipelineCache);
GETPROCADDR(DestroyPipelineCache);
GETPROCADDR(GetPipelineCacheData);
GETPROCADDR(MergePipelineCaches);
GETPROCADDR(CreateGraphicsPipelines);
GETPROCADDR(CreateComputePipelines);
GETPROCADDR(GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI);
GETPROCADDR(DestroyPipeline);
GETPROCADDR(CreatePipelineLayout);
GETPROCADDR(DestroyPipelineLayout);
GETPROCADDR(CreateSampler);
GETPROCADDR(DestroySampler);
GETPROCADDR(CreateDescriptorSetLayout);
GETPROCADDR(DestroyDescriptorSetLayout);
GETPROCADDR(CreateDescriptorPool);
GETPROCADDR(DestroyDescriptorPool);
GETPROCADDR(ResetDescriptorPool);
GETPROCADDR(AllocateDescriptorSets);
GETPROCADDR(FreeDescriptorSets);
GETPROCADDR(UpdateDescriptorSets);
GETPROCADDR(CreateFramebuffer);
GETPROCADDR(DestroyFramebuffer);
GETPROCADDR(CreateRenderPass);
GETPROCADDR(DestroyRenderPass);
GETPROCADDR(GetRenderAreaGranularity);
GETPROCADDR(GetRenderingAreaGranularityKHR);
GETPROCADDR(CreateCommandPool);
GETPROCADDR(DestroyCommandPool);
GETPROCADDR(ResetCommandPool);
GETPROCADDR(AllocateCommandBuffers);
GETPROCADDR(FreeCommandBuffers);
GETPROCADDR(BeginCommandBuffer);
GETPROCADDR(EndCommandBuffer);
GETPROCADDR(ResetCommandBuffer);
GETPROCADDR(CmdBindPipeline);
GETPROCADDR(CmdSetAttachmentFeedbackLoopEnableEXT);
GETPROCADDR(CmdSetViewport);
GETPROCADDR(CmdSetScissor);
GETPROCADDR(CmdSetLineWidth);
GETPROCADDR(CmdSetDepthBias);
GETPROCADDR(CmdSetBlendConstants);
GETPROCADDR(CmdSetDepthBounds);
GETPROCADDR(CmdSetStencilCompareMask);
GETPROCADDR(CmdSetStencilWriteMask);
GETPROCADDR(CmdSetStencilReference);
GETPROCADDR(CmdBindDescriptorSets);
GETPROCADDR(CmdBindIndexBuffer);
GETPROCADDR(CmdBindVertexBuffers);
GETPROCADDR(CmdDraw);
GETPROCADDR(CmdDrawIndexed);
GETPROCADDR(CmdDrawMultiEXT);
GETPROCADDR(CmdDrawMultiIndexedEXT);
GETPROCADDR(CmdDrawIndirect);
GETPROCADDR(CmdDrawIndexedIndirect);
GETPROCADDR(CmdDispatch);
GETPROCADDR(CmdDispatchIndirect);
GETPROCADDR(CmdSubpassShadingHUAWEI);
GETPROCADDR(CmdDrawClusterHUAWEI);
GETPROCADDR(CmdDrawClusterIndirectHUAWEI);
GETPROCADDR(CmdUpdatePipelineIndirectBufferNV);
GETPROCADDR(CmdCopyBuffer);
GETPROCADDR(CmdCopyImage);
GETPROCADDR(CmdBlitImage);
GETPROCADDR(CmdCopyBufferToImage);
GETPROCADDR(CmdCopyImageToBuffer);
GETPROCADDR(CmdCopyMemoryIndirectNV);
GETPROCADDR(CmdCopyMemoryToImageIndirectNV);
GETPROCADDR(CmdUpdateBuffer);
GETPROCADDR(CmdFillBuffer);
GETPROCADDR(CmdClearColorImage);
GETPROCADDR(CmdClearDepthStencilImage);
GETPROCADDR(CmdClearAttachments);
GETPROCADDR(CmdResolveImage);
GETPROCADDR(CmdSetEvent);
GETPROCADDR(CmdResetEvent);
GETPROCADDR(CmdWaitEvents);
GETPROCADDR(CmdPipelineBarrier);
GETPROCADDR(CmdBeginQuery);
GETPROCADDR(CmdEndQuery);
GETPROCADDR(CmdBeginConditionalRenderingEXT);
GETPROCADDR(CmdEndConditionalRenderingEXT);
GETPROCADDR(CmdResetQueryPool);
GETPROCADDR(CmdWriteTimestamp);
GETPROCADDR(CmdCopyQueryPoolResults);
GETPROCADDR(CmdPushConstants);
GETPROCADDR(CmdBeginRenderPass);
GETPROCADDR(CmdNextSubpass);
GETPROCADDR(CmdEndRenderPass);
GETPROCADDR(CmdExecuteCommands);
GETPROCADDR(CreateSharedSwapchainsKHR);
GETPROCADDR(CreateSwapchainKHR);
GETPROCADDR(DestroySwapchainKHR);
GETPROCADDR(GetSwapchainImagesKHR);
GETPROCADDR(AcquireNextImageKHR);
GETPROCADDR(QueuePresentKHR);
GETPROCADDR(DebugMarkerSetObjectNameEXT);
GETPROCADDR(DebugMarkerSetObjectTagEXT);
GETPROCADDR(CmdDebugMarkerBeginEXT);
GETPROCADDR(CmdDebugMarkerEndEXT);
GETPROCADDR(CmdDebugMarkerInsertEXT);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(GetMemoryWin32HandleNV);
#endif
GETPROCADDR(CmdExecuteGeneratedCommandsNV);
GETPROCADDR(CmdPreprocessGeneratedCommandsNV);
GETPROCADDR(CmdBindPipelineShaderGroupNV);
GETPROCADDR(GetGeneratedCommandsMemoryRequirementsNV);
GETPROCADDR(CreateIndirectCommandsLayoutNV);
GETPROCADDR(DestroyIndirectCommandsLayoutNV);
GETPROCADDR(CmdPushDescriptorSetKHR);
GETPROCADDR(TrimCommandPool);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(GetMemoryWin32HandleKHR);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(GetMemoryWin32HandlePropertiesKHR);
#endif
GETPROCADDR(GetMemoryFdKHR);
GETPROCADDR(GetMemoryFdPropertiesKHR);
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(GetMemoryZirconHandleFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(GetMemoryZirconHandlePropertiesFUCHSIA);
#endif
GETPROCADDR(GetMemoryRemoteAddressNV);
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(GetMemorySciBufNV);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(GetSemaphoreWin32HandleKHR);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(ImportSemaphoreWin32HandleKHR);
#endif
GETPROCADDR(GetSemaphoreFdKHR);
GETPROCADDR(ImportSemaphoreFdKHR);
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(GetSemaphoreZirconHandleFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(ImportSemaphoreZirconHandleFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(GetFenceWin32HandleKHR);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(ImportFenceWin32HandleKHR);
#endif
GETPROCADDR(GetFenceFdKHR);
GETPROCADDR(ImportFenceFdKHR);
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(GetFenceSciSyncFenceNV);
#endif
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(GetFenceSciSyncObjNV);
#endif
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(ImportFenceSciSyncFenceNV);
#endif
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(ImportFenceSciSyncObjNV);
#endif
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(GetSemaphoreSciSyncObjNV);
#endif
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(ImportSemaphoreSciSyncObjNV);
#endif
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(CreateSemaphoreSciSyncPoolNV);
#endif
#if defined(VK_USE_PLATFORM_SCI)
GETPROCADDR(DestroySemaphoreSciSyncPoolNV);
#endif
GETPROCADDR(DisplayPowerControlEXT);
GETPROCADDR(RegisterDeviceEventEXT);
GETPROCADDR(RegisterDisplayEventEXT);
GETPROCADDR(GetSwapchainCounterEXT);
GETPROCADDR(GetDeviceGroupPeerMemoryFeatures);
GETPROCADDR(BindBufferMemory2);
GETPROCADDR(BindImageMemory2);
GETPROCADDR(CmdSetDeviceMask);
GETPROCADDR(GetDeviceGroupPresentCapabilitiesKHR);
GETPROCADDR(GetDeviceGroupSurfacePresentModesKHR);
GETPROCADDR(AcquireNextImage2KHR);
GETPROCADDR(CmdDispatchBase);
GETPROCADDR(CreateDescriptorUpdateTemplate);
GETPROCADDR(DestroyDescriptorUpdateTemplate);
GETPROCADDR(UpdateDescriptorSetWithTemplate);
GETPROCADDR(CmdPushDescriptorSetWithTemplateKHR);
GETPROCADDR(SetHdrMetadataEXT);
GETPROCADDR(GetSwapchainStatusKHR);
GETPROCADDR(GetRefreshCycleDurationGOOGLE);
GETPROCADDR(GetPastPresentationTimingGOOGLE);
GETPROCADDR(CmdSetViewportWScalingNV);
GETPROCADDR(CmdSetDiscardRectangleEXT);
GETPROCADDR(CmdSetDiscardRectangleEnableEXT);
GETPROCADDR(CmdSetDiscardRectangleModeEXT);
GETPROCADDR(CmdSetSampleLocationsEXT);
GETPROCADDR(GetBufferMemoryRequirements2);
GETPROCADDR(GetImageMemoryRequirements2);
GETPROCADDR(GetImageSparseMemoryRequirements2);
GETPROCADDR(GetDeviceBufferMemoryRequirements);
GETPROCADDR(GetDeviceImageMemoryRequirements);
GETPROCADDR(GetDeviceImageSparseMemoryRequirements);
GETPROCADDR(CreateSamplerYcbcrConversion);
GETPROCADDR(DestroySamplerYcbcrConversion);
GETPROCADDR(GetDeviceQueue2);
GETPROCADDR(CreateValidationCacheEXT);
GETPROCADDR(DestroyValidationCacheEXT);
GETPROCADDR(GetValidationCacheDataEXT);
GETPROCADDR(MergeValidationCachesEXT);
GETPROCADDR(GetDescriptorSetLayoutSupport);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
GETPROCADDR(GetSwapchainGrallocUsageANDROID);
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
GETPROCADDR(GetSwapchainGrallocUsage2ANDROID);
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
GETPROCADDR(AcquireImageANDROID);
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
GETPROCADDR(QueueSignalReleaseImageANDROID);
#endif
GETPROCADDR(GetShaderInfoAMD);
GETPROCADDR(SetLocalDimmingAMD);
GETPROCADDR(GetCalibratedTimestampsKHR);
GETPROCADDR(SetDebugUtilsObjectNameEXT);
GETPROCADDR(SetDebugUtilsObjectTagEXT);
GETPROCADDR(QueueBeginDebugUtilsLabelEXT);
GETPROCADDR(QueueEndDebugUtilsLabelEXT);
GETPROCADDR(QueueInsertDebugUtilsLabelEXT);
GETPROCADDR(CmdBeginDebugUtilsLabelEXT);
GETPROCADDR(CmdEndDebugUtilsLabelEXT);
GETPROCADDR(CmdInsertDebugUtilsLabelEXT);
GETPROCADDR(GetMemoryHostPointerPropertiesEXT);
GETPROCADDR(CmdWriteBufferMarkerAMD);
GETPROCADDR(CreateRenderPass2);
GETPROCADDR(CmdBeginRenderPass2);
GETPROCADDR(CmdNextSubpass2);
GETPROCADDR(CmdEndRenderPass2);
GETPROCADDR(GetSemaphoreCounterValue);
GETPROCADDR(WaitSemaphores);
GETPROCADDR(SignalSemaphore);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
GETPROCADDR(GetAndroidHardwareBufferPropertiesANDROID);
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
GETPROCADDR(GetMemoryAndroidHardwareBufferANDROID);
#endif
GETPROCADDR(CmdDrawIndirectCount);
GETPROCADDR(CmdDrawIndexedIndirectCount);
GETPROCADDR(CmdSetCheckpointNV);
GETPROCADDR(GetQueueCheckpointDataNV);
GETPROCADDR(CmdBindTransformFeedbackBuffersEXT);
GETPROCADDR(CmdBeginTransformFeedbackEXT);
GETPROCADDR(CmdEndTransformFeedbackEXT);
GETPROCADDR(CmdBeginQueryIndexedEXT);
GETPROCADDR(CmdEndQueryIndexedEXT);
GETPROCADDR(CmdDrawIndirectByteCountEXT);
GETPROCADDR(CmdSetExclusiveScissorNV);
GETPROCADDR(CmdSetExclusiveScissorEnableNV);
GETPROCADDR(CmdBindShadingRateImageNV);
GETPROCADDR(CmdSetViewportShadingRatePaletteNV);
GETPROCADDR(CmdSetCoarseSampleOrderNV);
GETPROCADDR(CmdDrawMeshTasksNV);
GETPROCADDR(CmdDrawMeshTasksIndirectNV);
GETPROCADDR(CmdDrawMeshTasksIndirectCountNV);
GETPROCADDR(CmdDrawMeshTasksEXT);
GETPROCADDR(CmdDrawMeshTasksIndirectEXT);
GETPROCADDR(CmdDrawMeshTasksIndirectCountEXT);
GETPROCADDR(CompileDeferredNV);
GETPROCADDR(CreateAccelerationStructureNV);
GETPROCADDR(CmdBindInvocationMaskHUAWEI);
GETPROCADDR(DestroyAccelerationStructureKHR);
GETPROCADDR(DestroyAccelerationStructureNV);
GETPROCADDR(GetAccelerationStructureMemoryRequirementsNV);
GETPROCADDR(BindAccelerationStructureMemoryNV);
GETPROCADDR(CmdCopyAccelerationStructureNV);
GETPROCADDR(CmdCopyAccelerationStructureKHR);
GETPROCADDR(CopyAccelerationStructureKHR);
GETPROCADDR(CmdCopyAccelerationStructureToMemoryKHR);
GETPROCADDR(CopyAccelerationStructureToMemoryKHR);
GETPROCADDR(CmdCopyMemoryToAccelerationStructureKHR);
GETPROCADDR(CopyMemoryToAccelerationStructureKHR);
GETPROCADDR(CmdWriteAccelerationStructuresPropertiesKHR);
GETPROCADDR(CmdWriteAccelerationStructuresPropertiesNV);
GETPROCADDR(CmdBuildAccelerationStructureNV);
GETPROCADDR(WriteAccelerationStructuresPropertiesKHR);
GETPROCADDR(CmdTraceRaysKHR);
GETPROCADDR(CmdTraceRaysNV);
GETPROCADDR(GetRayTracingShaderGroupHandlesKHR);
GETPROCADDR(GetRayTracingCaptureReplayShaderGroupHandlesKHR);
GETPROCADDR(GetAccelerationStructureHandleNV);
GETPROCADDR(CreateRayTracingPipelinesNV);
GETPROCADDR(CreateRayTracingPipelinesKHR);
GETPROCADDR(CmdTraceRaysIndirectKHR);
GETPROCADDR(CmdTraceRaysIndirect2KHR);
GETPROCADDR(GetDeviceAccelerationStructureCompatibilityKHR);
GETPROCADDR(GetRayTracingShaderGroupStackSizeKHR);
GETPROCADDR(CmdSetRayTracingPipelineStackSizeKHR);
GETPROCADDR(GetImageViewHandleNVX);
GETPROCADDR(GetImageViewAddressNVX);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(GetDeviceGroupSurfacePresentModes2EXT);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(AcquireFullScreenExclusiveModeEXT);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
GETPROCADDR(ReleaseFullScreenExclusiveModeEXT);
#endif
GETPROCADDR(AcquireProfilingLockKHR);
GETPROCADDR(ReleaseProfilingLockKHR);
GETPROCADDR(GetImageDrmFormatModifierPropertiesEXT);
GETPROCADDR(GetBufferOpaqueCaptureAddress);
GETPROCADDR(GetBufferDeviceAddress);
GETPROCADDR(InitializePerformanceApiINTEL);
GETPROCADDR(UninitializePerformanceApiINTEL);
GETPROCADDR(CmdSetPerformanceMarkerINTEL);
GETPROCADDR(CmdSetPerformanceStreamMarkerINTEL);
GETPROCADDR(CmdSetPerformanceOverrideINTEL);
GETPROCADDR(AcquirePerformanceConfigurationINTEL);
GETPROCADDR(ReleasePerformanceConfigurationINTEL);
GETPROCADDR(QueueSetPerformanceConfigurationINTEL);
GETPROCADDR(GetPerformanceParameterINTEL);
GETPROCADDR(GetDeviceMemoryOpaqueCaptureAddress);
GETPROCADDR(GetPipelineExecutablePropertiesKHR);
GETPROCADDR(GetPipelineExecutableStatisticsKHR);
GETPROCADDR(GetPipelineExecutableInternalRepresentationsKHR);
GETPROCADDR(CreateAccelerationStructureKHR);
GETPROCADDR(CmdBuildAccelerationStructuresKHR);
GETPROCADDR(CmdBuildAccelerationStructuresIndirectKHR);
GETPROCADDR(BuildAccelerationStructuresKHR);
GETPROCADDR(GetAccelerationStructureDeviceAddressKHR);
GETPROCADDR(CreateDeferredOperationKHR);
GETPROCADDR(DestroyDeferredOperationKHR);
GETPROCADDR(GetDeferredOperationMaxConcurrencyKHR);
GETPROCADDR(GetDeferredOperationResultKHR);
GETPROCADDR(DeferredOperationJoinKHR);
GETPROCADDR(GetPipelineIndirectMemoryRequirementsNV);
GETPROCADDR(GetPipelineIndirectDeviceAddressNV);
GETPROCADDR(CmdSetCullMode);
GETPROCADDR(CmdSetFrontFace);
GETPROCADDR(CmdSetPrimitiveTopology);
GETPROCADDR(CmdSetViewportWithCount);
GETPROCADDR(CmdSetScissorWithCount);
GETPROCADDR(CmdBindIndexBuffer2KHR);
GETPROCADDR(CmdBindVertexBuffers2);
GETPROCADDR(CmdSetDepthTestEnable);
GETPROCADDR(CmdSetDepthWriteEnable);
GETPROCADDR(CmdSetDepthCompareOp);
GETPROCADDR(CmdSetDepthBoundsTestEnable);
GETPROCADDR(CmdSetStencilTestEnable);
GETPROCADDR(CmdSetStencilOp);
GETPROCADDR(CmdSetPatchControlPointsEXT);
GETPROCADDR(CmdSetRasterizerDiscardEnable);
GETPROCADDR(CmdSetDepthBiasEnable);
GETPROCADDR(CmdSetLogicOpEXT);
GETPROCADDR(CmdSetPrimitiveRestartEnable);
GETPROCADDR(CmdSetTessellationDomainOriginEXT);
GETPROCADDR(CmdSetDepthClampEnableEXT);
GETPROCADDR(CmdSetPolygonModeEXT);
GETPROCADDR(CmdSetRasterizationSamplesEXT);
GETPROCADDR(CmdSetSampleMaskEXT);
GETPROCADDR(CmdSetAlphaToCoverageEnableEXT);
GETPROCADDR(CmdSetAlphaToOneEnableEXT);
GETPROCADDR(CmdSetLogicOpEnableEXT);
GETPROCADDR(CmdSetColorBlendEnableEXT);
GETPROCADDR(CmdSetColorBlendEquationEXT);
GETPROCADDR(CmdSetColorWriteMaskEXT);
GETPROCADDR(CmdSetRasterizationStreamEXT);
GETPROCADDR(CmdSetConservativeRasterizationModeEXT);
GETPROCADDR(CmdSetExtraPrimitiveOverestimationSizeEXT);
GETPROCADDR(CmdSetDepthClipEnableEXT);
GETPROCADDR(CmdSetSampleLocationsEnableEXT);
GETPROCADDR(CmdSetColorBlendAdvancedEXT);
GETPROCADDR(CmdSetProvokingVertexModeEXT);
GETPROCADDR(CmdSetLineRasterizationModeEXT);
GETPROCADDR(CmdSetLineStippleEnableEXT);
GETPROCADDR(CmdSetDepthClipNegativeOneToOneEXT);
GETPROCADDR(CmdSetViewportWScalingEnableNV);
GETPROCADDR(CmdSetViewportSwizzleNV);
GETPROCADDR(CmdSetCoverageToColorEnableNV);
GETPROCADDR(CmdSetCoverageToColorLocationNV);
GETPROCADDR(CmdSetCoverageModulationModeNV);
GETPROCADDR(CmdSetCoverageModulationTableEnableNV);
GETPROCADDR(CmdSetCoverageModulationTableNV);
GETPROCADDR(CmdSetShadingRateImageEnableNV);
GETPROCADDR(CmdSetCoverageReductionModeNV);
GETPROCADDR(CmdSetRepresentativeFragmentTestEnableNV);
GETPROCADDR(CreatePrivateDataSlot);
GETPROCADDR(DestroyPrivateDataSlot);
GETPROCADDR(SetPrivateData);
GETPROCADDR(GetPrivateData);
GETPROCADDR(CmdCopyBuffer2);
GETPROCADDR(CmdCopyImage2);
GETPROCADDR(CmdBlitImage2);
GETPROCADDR(CmdCopyBufferToImage2);
GETPROCADDR(CmdCopyImageToBuffer2);
GETPROCADDR(CmdResolveImage2);
GETPROCADDR(CmdSetFragmentShadingRateKHR);
GETPROCADDR(CmdSetFragmentShadingRateEnumNV);
GETPROCADDR(GetAccelerationStructureBuildSizesKHR);
GETPROCADDR(CmdSetVertexInputEXT);
GETPROCADDR(CmdSetColorWriteEnableEXT);
GETPROCADDR(CmdSetEvent2);
GETPROCADDR(CmdResetEvent2);
GETPROCADDR(CmdWaitEvents2);
GETPROCADDR(CmdPipelineBarrier2);
GETPROCADDR(QueueSubmit2);
GETPROCADDR(CmdWriteTimestamp2);
GETPROCADDR(CmdWriteBufferMarker2AMD);
GETPROCADDR(GetQueueCheckpointData2NV);
GETPROCADDR(CopyMemoryToImageEXT);
GETPROCADDR(CopyImageToMemoryEXT);
GETPROCADDR(CopyImageToImageEXT);
GETPROCADDR(TransitionImageLayoutEXT);
GETPROCADDR(CreateVideoSessionKHR);
GETPROCADDR(DestroyVideoSessionKHR);
GETPROCADDR(CreateVideoSessionParametersKHR);
GETPROCADDR(UpdateVideoSessionParametersKHR);
GETPROCADDR(GetEncodedVideoSessionParametersKHR);
GETPROCADDR(DestroyVideoSessionParametersKHR);
GETPROCADDR(GetVideoSessionMemoryRequirementsKHR);
GETPROCADDR(BindVideoSessionMemoryKHR);
GETPROCADDR(CmdDecodeVideoKHR);
GETPROCADDR(CmdBeginVideoCodingKHR);
GETPROCADDR(CmdControlVideoCodingKHR);
GETPROCADDR(CmdEndVideoCodingKHR);
GETPROCADDR(CmdEncodeVideoKHR);
GETPROCADDR(CmdDecompressMemoryNV);
GETPROCADDR(CmdDecompressMemoryIndirectCountNV);
GETPROCADDR(CreateCuModuleNVX);
GETPROCADDR(CreateCuFunctionNVX);
GETPROCADDR(DestroyCuModuleNVX);
GETPROCADDR(DestroyCuFunctionNVX);
GETPROCADDR(CmdCuLaunchKernelNVX);
GETPROCADDR(GetDescriptorSetLayoutSizeEXT);
GETPROCADDR(GetDescriptorSetLayoutBindingOffsetEXT);
GETPROCADDR(GetDescriptorEXT);
GETPROCADDR(CmdBindDescriptorBuffersEXT);
GETPROCADDR(CmdSetDescriptorBufferOffsetsEXT);
GETPROCADDR(CmdBindDescriptorBufferEmbeddedSamplersEXT);
GETPROCADDR(GetBufferOpaqueCaptureDescriptorDataEXT);
GETPROCADDR(GetImageOpaqueCaptureDescriptorDataEXT);
GETPROCADDR(GetImageViewOpaqueCaptureDescriptorDataEXT);
GETPROCADDR(GetSamplerOpaqueCaptureDescriptorDataEXT);
GETPROCADDR(GetAccelerationStructureOpaqueCaptureDescriptorDataEXT);
GETPROCADDR(SetDeviceMemoryPriorityEXT);
GETPROCADDR(WaitForPresentKHR);
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(CreateBufferCollectionFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(SetBufferCollectionBufferConstraintsFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(SetBufferCollectionImageConstraintsFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(DestroyBufferCollectionFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
GETPROCADDR(GetBufferCollectionPropertiesFUCHSIA);
#endif
GETPROCADDR(CreateCudaModuleNV);
GETPROCADDR(GetCudaModuleCacheNV);
GETPROCADDR(CreateCudaFunctionNV);
GETPROCADDR(DestroyCudaModuleNV);
GETPROCADDR(DestroyCudaFunctionNV);
GETPROCADDR(CmdCudaLaunchKernelNV);
GETPROCADDR(CmdBeginRendering);
GETPROCADDR(CmdEndRendering);
GETPROCADDR(GetDescriptorSetLayoutHostMappingInfoVALVE);
GETPROCADDR(GetDescriptorSetHostMappingVALVE);
GETPROCADDR(CreateMicromapEXT);
GETPROCADDR(CmdBuildMicromapsEXT);
GETPROCADDR(BuildMicromapsEXT);
GETPROCADDR(DestroyMicromapEXT);
GETPROCADDR(CmdCopyMicromapEXT);
GETPROCADDR(CopyMicromapEXT);
GETPROCADDR(CmdCopyMicromapToMemoryEXT);
GETPROCADDR(CopyMicromapToMemoryEXT);
GETPROCADDR(CmdCopyMemoryToMicromapEXT);
GETPROCADDR(CopyMemoryToMicromapEXT);
GETPROCADDR(CmdWriteMicromapsPropertiesEXT);
GETPROCADDR(WriteMicromapsPropertiesEXT);
GETPROCADDR(GetDeviceMicromapCompatibilityEXT);
GETPROCADDR(GetMicromapBuildSizesEXT);
GETPROCADDR(GetShaderModuleIdentifierEXT);
GETPROCADDR(GetShaderModuleCreateInfoIdentifierEXT);
GETPROCADDR(GetImageSubresourceLayout2KHR);
GETPROCADDR(GetPipelinePropertiesEXT);
#if defined(VK_USE_PLATFORM_METAL_EXT)
GETPROCADDR(ExportMetalObjectsEXT);
#endif
GETPROCADDR(GetFramebufferTilePropertiesQCOM);
GETPROCADDR(GetDynamicRenderingTilePropertiesQCOM);
GETPROCADDR(CreateOpticalFlowSessionNV);
GETPROCADDR(DestroyOpticalFlowSessionNV);
GETPROCADDR(BindOpticalFlowSessionImageNV);
GETPROCADDR(CmdOpticalFlowExecuteNV);
GETPROCADDR(GetDeviceFaultInfoEXT);
GETPROCADDR(CmdSetDepthBias2EXT);
GETPROCADDR(ReleaseSwapchainImagesEXT);
GETPROCADDR(GetDeviceImageSubresourceLayoutKHR);
GETPROCADDR(MapMemory2KHR);
GETPROCADDR(UnmapMemory2KHR);
GETPROCADDR(CreateShadersEXT);
GETPROCADDR(DestroyShaderEXT);
GETPROCADDR(GetShaderBinaryDataEXT);
GETPROCADDR(CmdBindShadersEXT);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
GETPROCADDR(GetScreenBufferPropertiesQNX);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
GETPROCADDR(GetExecutionGraphPipelineScratchSizeAMDX);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
GETPROCADDR(GetExecutionGraphPipelineNodeIndexAMDX);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
GETPROCADDR(CreateExecutionGraphPipelinesAMDX);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
GETPROCADDR(CmdInitializeGraphScratchMemoryAMDX);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
GETPROCADDR(CmdDispatchGraphAMDX);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
GETPROCADDR(CmdDispatchGraphIndirectAMDX);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
GETPROCADDR(CmdDispatchGraphIndirectCountAMDX);
#endif
GETPROCADDR(CmdBindDescriptorSets2KHR);
GETPROCADDR(CmdPushConstants2KHR);
GETPROCADDR(CmdPushDescriptorSet2KHR);
GETPROCADDR(CmdPushDescriptorSetWithTemplate2KHR);
GETPROCADDR(CmdSetDescriptorBufferOffsets2EXT);
GETPROCADDR(CmdBindDescriptorBufferEmbeddedSamplers2EXT);
GETPROCADDR(SetLatencySleepModeNV);
GETPROCADDR(LatencySleepNV);
GETPROCADDR(SetLatencyMarkerNV);
GETPROCADDR(GetLatencyTimingsNV);
GETPROCADDR(QueueNotifyOutOfBandNV);
GETPROCADDR(ResetQueryPoolEXT);
GETPROCADDR(TrimCommandPoolKHR);
GETPROCADDR(GetDeviceGroupPeerMemoryFeaturesKHR);
GETPROCADDR(BindBufferMemory2KHR);
GETPROCADDR(BindImageMemory2KHR);
GETPROCADDR(CmdSetDeviceMaskKHR);
GETPROCADDR(CmdDispatchBaseKHR);
GETPROCADDR(CreateDescriptorUpdateTemplateKHR);
GETPROCADDR(DestroyDescriptorUpdateTemplateKHR);
GETPROCADDR(UpdateDescriptorSetWithTemplateKHR);
GETPROCADDR(GetBufferMemoryRequirements2KHR);
GETPROCADDR(GetImageMemoryRequirements2KHR);
GETPROCADDR(GetImageSparseMemoryRequirements2KHR);
GETPROCADDR(GetDeviceBufferMemoryRequirementsKHR);
GETPROCADDR(GetDeviceImageMemoryRequirementsKHR);
GETPROCADDR(GetDeviceImageSparseMemoryRequirementsKHR);
GETPROCADDR(CreateSamplerYcbcrConversionKHR);
GETPROCADDR(DestroySamplerYcbcrConversionKHR);
GETPROCADDR(GetDescriptorSetLayoutSupportKHR);
GETPROCADDR(GetCalibratedTimestampsEXT);
GETPROCADDR(CreateRenderPass2KHR);
GETPROCADDR(CmdBeginRenderPass2KHR);
GETPROCADDR(CmdNextSubpass2KHR);
GETPROCADDR(CmdEndRenderPass2KHR);
GETPROCADDR(GetSemaphoreCounterValueKHR);
GETPROCADDR(WaitSemaphoresKHR);
GETPROCADDR(SignalSemaphoreKHR);
GETPROCADDR(CmdDrawIndirectCountKHR);
GETPROCADDR(CmdDrawIndirectCountAMD);
GETPROCADDR(CmdDrawIndexedIndirectCountKHR);
GETPROCADDR(CmdDrawIndexedIndirectCountAMD);
GETPROCADDR(GetRayTracingShaderGroupHandlesNV);
GETPROCADDR(GetBufferOpaqueCaptureAddressKHR);
GETPROCADDR(GetBufferDeviceAddressKHR);
GETPROCADDR(GetBufferDeviceAddressEXT);
GETPROCADDR(GetDeviceMemoryOpaqueCaptureAddressKHR);
GETPROCADDR(CmdSetLineStippleEXT);
GETPROCADDR(CmdSetCullModeEXT);
GETPROCADDR(CmdSetFrontFaceEXT);
GETPROCADDR(CmdSetPrimitiveTopologyEXT);
GETPROCADDR(CmdSetViewportWithCountEXT);
GETPROCADDR(CmdSetScissorWithCountEXT);
GETPROCADDR(CmdBindVertexBuffers2EXT);
GETPROCADDR(CmdSetDepthTestEnableEXT);
GETPROCADDR(CmdSetDepthWriteEnableEXT);
GETPROCADDR(CmdSetDepthCompareOpEXT);
GETPROCADDR(CmdSetDepthBoundsTestEnableEXT);
GETPROCADDR(CmdSetStencilTestEnableEXT);
GETPROCADDR(CmdSetStencilOpEXT);
GETPROCADDR(CmdSetRasterizerDiscardEnableEXT);
GETPROCADDR(CmdSetDepthBiasEnableEXT);
GETPROCADDR(CmdSetPrimitiveRestartEnableEXT);
GETPROCADDR(CreatePrivateDataSlotEXT);
GETPROCADDR(DestroyPrivateDataSlotEXT);
GETPROCADDR(SetPrivateDataEXT);
GETPROCADDR(GetPrivateDataEXT);
GETPROCADDR(CmdCopyBuffer2KHR);
GETPROCADDR(CmdCopyImage2KHR);
GETPROCADDR(CmdBlitImage2KHR);
GETPROCADDR(CmdCopyBufferToImage2KHR);
GETPROCADDR(CmdCopyImageToBuffer2KHR);
GETPROCADDR(CmdResolveImage2KHR);
GETPROCADDR(CmdSetEvent2KHR);
GETPROCADDR(CmdResetEvent2KHR);
GETPROCADDR(CmdWaitEvents2KHR);
GETPROCADDR(CmdPipelineBarrier2KHR);
GETPROCADDR(QueueSubmit2KHR);
GETPROCADDR(CmdWriteTimestamp2KHR);
GETPROCADDR(CmdBeginRenderingKHR);
GETPROCADDR(CmdEndRenderingKHR);
GETPROCADDR(GetImageSubresourceLayout2EXT);
scoped_lock l(global_lock);
return instance_dispatch[GetKey(instance)].GetInstanceProcAddr(instance, pName);}

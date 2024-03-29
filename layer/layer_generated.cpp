#include "layer.hpp"
#include <mutex>
#include <ws2tcpip.h>
#include "../layergen/vk_layer_dispatch_table.h"
#include "../vklayer/vk_layer_table.h"
#include <iostream>

std::map<void*, VkLayerInstanceDispatchTable> instance_dispatch;
std::map<void*, VkLayerDispatchTable> device_dispatch;

std::mutex global_lock;
typedef std::lock_guard<std::mutex> scoped_lock;
template<typename DispatchableType>
void* GetKey(DispatchableType inst)
{ return *(void**)inst; }

void CreateDeviceDispatch(PFN_vkGetDeviceProcAddr gdpa, VkDevice* pDevice) 
{

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
struct CommandStats
{
    uint32_t drawCount = 0, instanceCount = 0, vertCount = 0;
};
std::map<VkCommandBuffer, CommandStats> commandbuffer_stats;

/* Layer init and shutdown */

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    #ifdef CREATEINSTANCE_EXISTS
    layer_CreateInstance(pCreateInfo, pAllocator, pInstance);
    #endif

    VkLayerInstanceCreateInfo* layerCreateInfo = (VkLayerInstanceCreateInfo*)pCreateInfo->pNext;
    /* step through the chain of pNext until we get to the link info */
    while (layerCreateInfo && (layerCreateInfo->sType != VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO ||
        layerCreateInfo->function != VK_LAYER_LINK_INFO))
    {
        layerCreateInfo = (VkLayerInstanceCreateInfo*)layerCreateInfo->pNext;
    }

    if (layerCreateInfo == NULL)
    {
        /* No loader instance create info */
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    PFN_vkGetInstanceProcAddr gpa = layerCreateInfo->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    layerCreateInfo->u.pLayerInfo = layerCreateInfo->u.pLayerInfo->pNext;
    PFN_vkCreateInstance createFunc = (PFN_vkCreateInstance)gpa(VK_NULL_HANDLE, "vkCreateInstance");

    /* fetch our own dispatch table for the functions we need, into the next layer */
    CreateInstanceDispatch(gpa, pInstance);

    VkResult ret = createFunc(pCreateInfo, pAllocator, pInstance);

    return ret;
}

VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator)
{
    scoped_lock l(global_lock);
    //winsockSendToUI(&ConnectSocket, "vkDestroyInstance");

    /* Stop communication */
    //layerWinsockExit(&ConnectSocket);
    instance_dispatch.erase(GetKey(instance));

}

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    VkLayerDeviceCreateInfo* layerCreateInfo = (VkLayerDeviceCreateInfo*)pCreateInfo->pNext;
    //winsockSendToUI(&ConnectSocket, "vkCreateDevice");

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

    VkResult ret = createFunc(physicalDevice, pCreateInfo, pAllocator, pDevice);

    /* fetch our own dispatch table for the functions we need, into the next layer */
    CreateDeviceDispatch(gdpa, pDevice);

    return VK_SUCCESS;
}

VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator)
{
    scoped_lock l(global_lock);
    //winsockSendToUI(&ConnectSocket, "vkDestroyDevice");
    device_dispatch.erase(GetKey(device));
}

/* Enumeration function */
VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumerateInstanceLayerProperties(uint32_t* pPropertyCount,
    VkLayerProperties* pProperties)
{
    if (pPropertyCount) *pPropertyCount = 1;
    //winsockSendToUI(&ConnectSocket, "vkEnumerateInstanceLayerProperties");

    if (pProperties)
    {
        strcpy_s(pProperties->layerName, "vkDetailsLayer");
        strcpy_s(pProperties->description, "https://github.com/xbilko03/ADT_VAPI");
        pProperties->implementationVersion = 1;
        pProperties->specVersion = VK_API_VERSION_1_0;
    }

    return VK_SUCCESS;
}

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumerateDeviceLayerProperties(
    VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    //winsockSendToUI(&ConnectSocket, "vkEnumerateDeviceLayerProperties");
    return DetailsLayer_EnumerateInstanceLayerProperties(pPropertyCount, pProperties);
}

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumerateInstanceExtensionProperties(
    const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    //winsockSendToUI(&ConnectSocket, "vkEnumerateInstanceExtensionProperties");
    if (pLayerName == NULL || strcmp(pLayerName, "VK_LAYER_SAMPLE_DetailsLayer"))
        return VK_ERROR_LAYER_NOT_PRESENT;

    /* don't expose any extensions */
    if (pPropertyCount) *pPropertyCount = 0;
    return VK_SUCCESS;
}

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumerateDeviceExtensionProperties(
    VkPhysicalDevice physicalDevice, const char* pLayerName,
    uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    //winsockSendToUI(&ConnectSocket, "vkEnumerateDeviceExtensionProperties");
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





 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices ) {
scoped_lock l(global_lock);
#ifdef ENUMERATEPHYSICALDEVICES_EXISTS
layer_EnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
#endif 
instance_dispatch[GetKey(instance)].EnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEPROPERTIES_EXISTS
layer_GetPhysicalDeviceProperties(physicalDevice, pProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceProperties(physicalDevice, pProperties);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEQUEUEFAMILYPROPERTIES_EXISTS
layer_GetPhysicalDeviceQueueFamilyProperties(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceQueueFamilyProperties(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEMEMORYPROPERTIES_EXISTS
layer_GetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEFEATURES_EXISTS
layer_GetPhysicalDeviceFeatures(physicalDevice, pFeatures);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceFeatures(physicalDevice, pFeatures);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEFORMATPROPERTIES_EXISTS
layer_GetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEIMAGEFORMATPROPERTIES_EXISTS
layer_GetPhysicalDeviceImageFormatProperties(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceImageFormatProperties(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICESPARSEIMAGEFORMATPROPERTIES_EXISTS
layer_GetPhysicalDeviceSparseImageFormatProperties(physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSparseImageFormatProperties(physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
}

#if defined(VK_USE_PLATFORM_ANDROID_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateAndroidSurfaceKHR(VkInstance instance, VkAndroidSurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface ) {
scoped_lock l(global_lock);
#ifdef CREATEANDROIDSURFACEKHR_EXISTS
layer_CreateAndroidSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
#endif 
instance_dispatch[GetKey(instance)].CreateAndroidSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
return VK_SUCCESS;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEDISPLAYPROPERTIESKHR_EXISTS
layer_GetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEDISPLAYPLANEPROPERTIESKHR_EXISTS
layer_GetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, pPropertyCount, pProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, pPropertyCount, pProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays ) {
scoped_lock l(global_lock);
#ifdef GETDISPLAYPLANESUPPORTEDDISPLAYSKHR_EXISTS
layer_GetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, pDisplayCount, pDisplays);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, pDisplayCount, pDisplays);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETDISPLAYMODEPROPERTIESKHR_EXISTS
layer_GetDisplayModePropertiesKHR(physicalDevice, display, pPropertyCount, pProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetDisplayModePropertiesKHR(physicalDevice, display, pPropertyCount, pProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, VkDisplayModeCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode ) {
scoped_lock l(global_lock);
#ifdef CREATEDISPLAYMODEKHR_EXISTS
layer_CreateDisplayModeKHR(physicalDevice, display, pCreateInfo, pAllocator, pMode);
#endif 
instance_dispatch[GetKey(physicalDevice)].CreateDisplayModeKHR(physicalDevice, display, pCreateInfo, pAllocator, pMode);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities ) {
scoped_lock l(global_lock);
#ifdef GETDISPLAYPLANECAPABILITIESKHR_EXISTS
layer_GetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDisplayPlaneSurfaceKHR(VkInstance instance, VkDisplaySurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface ) {
scoped_lock l(global_lock);
#ifdef CREATEDISPLAYPLANESURFACEKHR_EXISTS
layer_CreateDisplayPlaneSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
#endif 
instance_dispatch[GetKey(instance)].CreateDisplayPlaneSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYSURFACEKHR_EXISTS
layer_DestroySurfaceKHR(instance, surface, pAllocator);
#endif 
instance_dispatch[GetKey(instance)].DestroySurfaceKHR(instance, surface, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICESURFACESUPPORTKHR_EXISTS
layer_GetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICESURFACECAPABILITIESKHR_EXISTS
layer_GetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICESURFACEFORMATSKHR_EXISTS
layer_GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICESURFACEPRESENTMODESKHR_EXISTS
layer_GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
return VK_SUCCESS;
}

#if defined(VK_USE_PLATFORM_VI_NN)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateViSurfaceNN(VkInstance instance, VkViSurfaceCreateInfoNN* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface ) {
scoped_lock l(global_lock);
#ifdef CREATEVISURFACENN_EXISTS
layer_CreateViSurfaceNN(instance, pCreateInfo, pAllocator, pSurface);
#endif 
instance_dispatch[GetKey(instance)].CreateViSurfaceNN(instance, pCreateInfo, pAllocator, pSurface);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateWaylandSurfaceKHR(VkInstance instance, VkWaylandSurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface ) {
scoped_lock l(global_lock);
#ifdef CREATEWAYLANDSURFACEKHR_EXISTS
layer_CreateWaylandSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
#endif 
instance_dispatch[GetKey(instance)].CreateWaylandSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
 VK_LAYER_EXPORT VkBool32 VKAPI_CALL DetailsLayer_GetPhysicalDeviceWaylandPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, wl_display* display ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEWAYLANDPRESENTATIONSUPPORTKHR_EXISTS
layer_GetPhysicalDeviceWaylandPresentationSupportKHR(physicalDevice, queueFamilyIndex, display);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceWaylandPresentationSupportKHR(physicalDevice, queueFamilyIndex, display);
return VK_TRUE;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateWin32SurfaceKHR(VkInstance instance, VkWin32SurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface ) {
scoped_lock l(global_lock);
#ifdef CREATEWIN32SURFACEKHR_EXISTS
layer_CreateWin32SurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
#endif 
instance_dispatch[GetKey(instance)].CreateWin32SurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkBool32 VKAPI_CALL DetailsLayer_GetPhysicalDeviceWin32PresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEWIN32PRESENTATIONSUPPORTKHR_EXISTS
layer_GetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, queueFamilyIndex);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, queueFamilyIndex);
return VK_TRUE;
}

#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateXlibSurfaceKHR(VkInstance instance, VkXlibSurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface ) {
scoped_lock l(global_lock);
#ifdef CREATEXLIBSURFACEKHR_EXISTS
layer_CreateXlibSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
#endif 
instance_dispatch[GetKey(instance)].CreateXlibSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR)
 VK_LAYER_EXPORT VkBool32 VKAPI_CALL DetailsLayer_GetPhysicalDeviceXlibPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEXLIBPRESENTATIONSUPPORTKHR_EXISTS
layer_GetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice, queueFamilyIndex, dpy, visualID);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice, queueFamilyIndex, dpy, visualID);
return VK_TRUE;
}

#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateXcbSurfaceKHR(VkInstance instance, VkXcbSurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface ) {
scoped_lock l(global_lock);
#ifdef CREATEXCBSURFACEKHR_EXISTS
layer_CreateXcbSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
#endif 
instance_dispatch[GetKey(instance)].CreateXcbSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
 VK_LAYER_EXPORT VkBool32 VKAPI_CALL DetailsLayer_GetPhysicalDeviceXcbPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEXCBPRESENTATIONSUPPORTKHR_EXISTS
layer_GetPhysicalDeviceXcbPresentationSupportKHR(physicalDevice, queueFamilyIndex, connection, visual_id);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceXcbPresentationSupportKHR(physicalDevice, queueFamilyIndex, connection, visual_id);
return VK_TRUE;
}

#endif
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDirectFBSurfaceEXT(VkInstance instance, VkDirectFBSurfaceCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface ) {
scoped_lock l(global_lock);
#ifdef CREATEDIRECTFBSURFACEEXT_EXISTS
layer_CreateDirectFBSurfaceEXT(instance, pCreateInfo, pAllocator, pSurface);
#endif 
instance_dispatch[GetKey(instance)].CreateDirectFBSurfaceEXT(instance, pCreateInfo, pAllocator, pSurface);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
 VK_LAYER_EXPORT VkBool32 VKAPI_CALL DetailsLayer_GetPhysicalDeviceDirectFBPresentationSupportEXT(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEDIRECTFBPRESENTATIONSUPPORTEXT_EXISTS
layer_GetPhysicalDeviceDirectFBPresentationSupportEXT(physicalDevice, queueFamilyIndex, dfb);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceDirectFBPresentationSupportEXT(physicalDevice, queueFamilyIndex, dfb);
return VK_TRUE;
}

#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateImagePipeSurfaceFUCHSIA(VkInstance instance, VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface ) {
scoped_lock l(global_lock);
#ifdef CREATEIMAGEPIPESURFACEFUCHSIA_EXISTS
layer_CreateImagePipeSurfaceFUCHSIA(instance, pCreateInfo, pAllocator, pSurface);
#endif 
instance_dispatch[GetKey(instance)].CreateImagePipeSurfaceFUCHSIA(instance, pCreateInfo, pAllocator, pSurface);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_GGP)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateStreamDescriptorSurfaceGGP(VkInstance instance, VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface ) {
scoped_lock l(global_lock);
#ifdef CREATESTREAMDESCRIPTORSURFACEGGP_EXISTS
layer_CreateStreamDescriptorSurfaceGGP(instance, pCreateInfo, pAllocator, pSurface);
#endif 
instance_dispatch[GetKey(instance)].CreateStreamDescriptorSurfaceGGP(instance, pCreateInfo, pAllocator, pSurface);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateScreenSurfaceQNX(VkInstance instance, VkScreenSurfaceCreateInfoQNX* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface ) {
scoped_lock l(global_lock);
#ifdef CREATESCREENSURFACEQNX_EXISTS
layer_CreateScreenSurfaceQNX(instance, pCreateInfo, pAllocator, pSurface);
#endif 
instance_dispatch[GetKey(instance)].CreateScreenSurfaceQNX(instance, pCreateInfo, pAllocator, pSurface);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
 VK_LAYER_EXPORT VkBool32 VKAPI_CALL DetailsLayer_GetPhysicalDeviceScreenPresentationSupportQNX(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, _screen_window* window ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICESCREENPRESENTATIONSUPPORTQNX_EXISTS
layer_GetPhysicalDeviceScreenPresentationSupportQNX(physicalDevice, queueFamilyIndex, window);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceScreenPresentationSupportQNX(physicalDevice, queueFamilyIndex, window);
return VK_TRUE;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback ) {
scoped_lock l(global_lock);
#ifdef CREATEDEBUGREPORTCALLBACKEXT_EXISTS
layer_CreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
#endif 
instance_dispatch[GetKey(instance)].CreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYDEBUGREPORTCALLBACKEXT_EXISTS
layer_DestroyDebugReportCallbackEXT(instance, callback, pAllocator);
#endif 
instance_dispatch[GetKey(instance)].DestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, char* pLayerPrefix, char* pMessage ) {
scoped_lock l(global_lock);
#ifdef DEBUGREPORTMESSAGEEXT_EXISTS
layer_DebugReportMessageEXT(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
#endif 
instance_dispatch[GetKey(instance)].DebugReportMessageEXT(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEEXTERNALIMAGEFORMATPROPERTIESNV_EXISTS
layer_GetPhysicalDeviceExternalImageFormatPropertiesNV(physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceExternalImageFormatPropertiesNV(physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEFEATURES2_EXISTS
layer_GetPhysicalDeviceFeatures2(physicalDevice, pFeatures);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceFeatures2(physicalDevice, pFeatures);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEPROPERTIES2_EXISTS
layer_GetPhysicalDeviceProperties2(physicalDevice, pProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceProperties2(physicalDevice, pProperties);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEFORMATPROPERTIES2_EXISTS
layer_GetPhysicalDeviceFormatProperties2(physicalDevice, format, pFormatProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceFormatProperties2(physicalDevice, format, pFormatProperties);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEIMAGEFORMATPROPERTIES2_EXISTS
layer_GetPhysicalDeviceImageFormatProperties2(physicalDevice, pImageFormatInfo, pImageFormatProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceImageFormatProperties2(physicalDevice, pImageFormatInfo, pImageFormatProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEQUEUEFAMILYPROPERTIES2_EXISTS
layer_GetPhysicalDeviceQueueFamilyProperties2(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceQueueFamilyProperties2(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEMEMORYPROPERTIES2_EXISTS
layer_GetPhysicalDeviceMemoryProperties2(physicalDevice, pMemoryProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceMemoryProperties2(physicalDevice, pMemoryProperties);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICESPARSEIMAGEFORMATPROPERTIES2_EXISTS
layer_GetPhysicalDeviceSparseImageFormatProperties2(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSparseImageFormatProperties2(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEEXTERNALBUFFERPROPERTIES_EXISTS
layer_GetPhysicalDeviceExternalBufferProperties(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceExternalBufferProperties(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}

#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceExternalMemorySciBufPropertiesNV(VkPhysicalDevice physicalDevice, VkExternalMemoryHandleTypeFlagBits handleType, NvSciBufObj handle, VkMemorySciBufPropertiesNV* pMemorySciBufProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEEXTERNALMEMORYSCIBUFPROPERTIESNV_EXISTS
layer_GetPhysicalDeviceExternalMemorySciBufPropertiesNV(physicalDevice, handleType, handle, pMemorySciBufProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceExternalMemorySciBufPropertiesNV(physicalDevice, handleType, handle, pMemorySciBufProperties);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSciBufAttributesNV(VkPhysicalDevice physicalDevice, NvSciBufAttrList pAttributes ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICESCIBUFATTRIBUTESNV_EXISTS
layer_GetPhysicalDeviceSciBufAttributesNV(physicalDevice, pAttributes);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSciBufAttributesNV(physicalDevice, pAttributes);
return VK_SUCCESS;
}

#endif
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEEXTERNALSEMAPHOREPROPERTIES_EXISTS
layer_GetPhysicalDeviceExternalSemaphoreProperties(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceExternalSemaphoreProperties(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEEXTERNALFENCEPROPERTIES_EXISTS
layer_GetPhysicalDeviceExternalFenceProperties(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceExternalFenceProperties(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}

#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSciSyncAttributesNV(VkPhysicalDevice physicalDevice, VkSciSyncAttributesInfoNV* pSciSyncAttributesInfo, NvSciSyncAttrList pAttributes ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICESCISYNCATTRIBUTESNV_EXISTS
layer_GetPhysicalDeviceSciSyncAttributesNV(physicalDevice, pSciSyncAttributesInfo, pAttributes);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSciSyncAttributesNV(physicalDevice, pSciSyncAttributesInfo, pAttributes);
return VK_SUCCESS;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display ) {
scoped_lock l(global_lock);
#ifdef RELEASEDISPLAYEXT_EXISTS
layer_ReleaseDisplayEXT(physicalDevice, display);
#endif 
instance_dispatch[GetKey(physicalDevice)].ReleaseDisplayEXT(physicalDevice, display);
return VK_SUCCESS;
}

#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquireXlibDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display ) {
scoped_lock l(global_lock);
#ifdef ACQUIREXLIBDISPLAYEXT_EXISTS
layer_AcquireXlibDisplayEXT(physicalDevice, dpy, display);
#endif 
instance_dispatch[GetKey(physicalDevice)].AcquireXlibDisplayEXT(physicalDevice, dpy, display);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetRandROutputDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay ) {
scoped_lock l(global_lock);
#ifdef GETRANDROUTPUTDISPLAYEXT_EXISTS
layer_GetRandROutputDisplayEXT(physicalDevice, dpy, rrOutput, pDisplay);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetRandROutputDisplayEXT(physicalDevice, dpy, rrOutput, pDisplay);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display ) {
scoped_lock l(global_lock);
#ifdef ACQUIREWINRTDISPLAYNV_EXISTS
layer_AcquireWinrtDisplayNV(physicalDevice, display);
#endif 
instance_dispatch[GetKey(physicalDevice)].AcquireWinrtDisplayNV(physicalDevice, display);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay ) {
scoped_lock l(global_lock);
#ifdef GETWINRTDISPLAYNV_EXISTS
layer_GetWinrtDisplayNV(physicalDevice, deviceRelativeId, pDisplay);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetWinrtDisplayNV(physicalDevice, deviceRelativeId, pDisplay);
return VK_SUCCESS;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICESURFACECAPABILITIES2EXT_EXISTS
layer_GetPhysicalDeviceSurfaceCapabilities2EXT(physicalDevice, surface, pSurfaceCapabilities);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSurfaceCapabilities2EXT(physicalDevice, surface, pSurfaceCapabilities);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties ) {
scoped_lock l(global_lock);
#ifdef ENUMERATEPHYSICALDEVICEGROUPS_EXISTS
layer_EnumeratePhysicalDeviceGroups(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
#endif 
instance_dispatch[GetKey(instance)].EnumeratePhysicalDeviceGroups(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEPRESENTRECTANGLESKHR_EXISTS
layer_GetPhysicalDevicePresentRectanglesKHR(physicalDevice, surface, pRectCount, pRects);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDevicePresentRectanglesKHR(physicalDevice, surface, pRectCount, pRects);
return VK_SUCCESS;
}

#if defined(VK_USE_PLATFORM_IOS_MVK)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateIOSSurfaceMVK(VkInstance instance, VkIOSSurfaceCreateInfoMVK* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface ) {
scoped_lock l(global_lock);
#ifdef CREATEIOSSURFACEMVK_EXISTS
layer_CreateIOSSurfaceMVK(instance, pCreateInfo, pAllocator, pSurface);
#endif 
instance_dispatch[GetKey(instance)].CreateIOSSurfaceMVK(instance, pCreateInfo, pAllocator, pSurface);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_MACOS_MVK)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateMacOSSurfaceMVK(VkInstance instance, VkMacOSSurfaceCreateInfoMVK* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface ) {
scoped_lock l(global_lock);
#ifdef CREATEMACOSSURFACEMVK_EXISTS
layer_CreateMacOSSurfaceMVK(instance, pCreateInfo, pAllocator, pSurface);
#endif 
instance_dispatch[GetKey(instance)].CreateMacOSSurfaceMVK(instance, pCreateInfo, pAllocator, pSurface);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_METAL_EXT)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateMetalSurfaceEXT(VkInstance instance, VkMetalSurfaceCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface ) {
scoped_lock l(global_lock);
#ifdef CREATEMETALSURFACEEXT_EXISTS
layer_CreateMetalSurfaceEXT(instance, pCreateInfo, pAllocator, pSurface);
#endif 
instance_dispatch[GetKey(instance)].CreateMetalSurfaceEXT(instance, pCreateInfo, pAllocator, pSurface);
return VK_SUCCESS;
}

#endif
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEMULTISAMPLEPROPERTIESEXT_EXISTS
layer_GetPhysicalDeviceMultisamplePropertiesEXT(physicalDevice, samples, pMultisampleProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceMultisamplePropertiesEXT(physicalDevice, samples, pMultisampleProperties);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICESURFACECAPABILITIES2KHR_EXISTS
layer_GetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICESURFACEFORMATS2KHR_EXISTS
layer_GetPhysicalDeviceSurfaceFormats2KHR(physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSurfaceFormats2KHR(physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEDISPLAYPROPERTIES2KHR_EXISTS
layer_GetPhysicalDeviceDisplayProperties2KHR(physicalDevice, pPropertyCount, pProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceDisplayProperties2KHR(physicalDevice, pPropertyCount, pProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEDISPLAYPLANEPROPERTIES2KHR_EXISTS
layer_GetPhysicalDeviceDisplayPlaneProperties2KHR(physicalDevice, pPropertyCount, pProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceDisplayPlaneProperties2KHR(physicalDevice, pPropertyCount, pProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETDISPLAYMODEPROPERTIES2KHR_EXISTS
layer_GetDisplayModeProperties2KHR(physicalDevice, display, pPropertyCount, pProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetDisplayModeProperties2KHR(physicalDevice, display, pPropertyCount, pProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities ) {
scoped_lock l(global_lock);
#ifdef GETDISPLAYPLANECAPABILITIES2KHR_EXISTS
layer_GetDisplayPlaneCapabilities2KHR(physicalDevice, pDisplayPlaneInfo, pCapabilities);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetDisplayPlaneCapabilities2KHR(physicalDevice, pDisplayPlaneInfo, pCapabilities);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceCalibrateableTimeDomainsKHR(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICECALIBRATEABLETIMEDOMAINSKHR_EXISTS
layer_GetPhysicalDeviceCalibrateableTimeDomainsKHR(physicalDevice, pTimeDomainCount, pTimeDomains);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceCalibrateableTimeDomainsKHR(physicalDevice, pTimeDomainCount, pTimeDomains);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger ) {
scoped_lock l(global_lock);
#ifdef CREATEDEBUGUTILSMESSENGEREXT_EXISTS
layer_CreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
#endif 
instance_dispatch[GetKey(instance)].CreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYDEBUGUTILSMESSENGEREXT_EXISTS
layer_DestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
#endif 
instance_dispatch[GetKey(instance)].DestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_SubmitDebugUtilsMessageEXT(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, VkDebugUtilsMessengerCallbackDataEXT* pCallbackData ) {
scoped_lock l(global_lock);
#ifdef SUBMITDEBUGUTILSMESSAGEEXT_EXISTS
layer_SubmitDebugUtilsMessageEXT(instance, messageSeverity, messageTypes, pCallbackData);
#endif 
instance_dispatch[GetKey(instance)].SubmitDebugUtilsMessageEXT(instance, messageSeverity, messageTypes, pCallbackData);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICECOOPERATIVEMATRIXPROPERTIESNV_EXISTS
layer_GetPhysicalDeviceCooperativeMatrixPropertiesNV(physicalDevice, pPropertyCount, pProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceCooperativeMatrixPropertiesNV(physicalDevice, pPropertyCount, pProperties);
return VK_SUCCESS;
}

#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSurfacePresentModes2EXT(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICESURFACEPRESENTMODES2EXT_EXISTS
layer_GetPhysicalDeviceSurfacePresentModes2EXT(physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSurfacePresentModes2EXT(physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
return VK_SUCCESS;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions ) {
scoped_lock l(global_lock);
#ifdef ENUMERATEPHYSICALDEVICEQUEUEFAMILYPERFORMANCEQUERYCOUNTERSKHR_EXISTS
layer_EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions);
#endif 
instance_dispatch[GetKey(physicalDevice)].EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice, VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEQUEUEFAMILYPERFORMANCEQUERYPASSESKHR_EXISTS
layer_GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(physicalDevice, pPerformanceQueryCreateInfo, pNumPasses);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(physicalDevice, pPerformanceQueryCreateInfo, pNumPasses);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateHeadlessSurfaceEXT(VkInstance instance, VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface ) {
scoped_lock l(global_lock);
#ifdef CREATEHEADLESSSURFACEEXT_EXISTS
layer_CreateHeadlessSurfaceEXT(instance, pCreateInfo, pAllocator, pSurface);
#endif 
instance_dispatch[GetKey(instance)].CreateHeadlessSurfaceEXT(instance, pCreateInfo, pAllocator, pSurface);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICESUPPORTEDFRAMEBUFFERMIXEDSAMPLESCOMBINATIONSNV_EXISTS
layer_GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(physicalDevice, pCombinationCount, pCombinations);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(physicalDevice, pCombinationCount, pCombinations);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICETOOLPROPERTIES_EXISTS
layer_GetPhysicalDeviceToolProperties(physicalDevice, pToolCount, pToolProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceToolProperties(physicalDevice, pToolCount, pToolProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEFRAGMENTSHADINGRATESKHR_EXISTS
layer_GetPhysicalDeviceFragmentShadingRatesKHR(physicalDevice, pFragmentShadingRateCount, pFragmentShadingRates);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceFragmentShadingRatesKHR(physicalDevice, pFragmentShadingRateCount, pFragmentShadingRates);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceVideoCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEVIDEOCAPABILITIESKHR_EXISTS
layer_GetPhysicalDeviceVideoCapabilitiesKHR(physicalDevice, pVideoProfile, pCapabilities);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceVideoCapabilitiesKHR(physicalDevice, pVideoProfile, pCapabilities);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceVideoFormatPropertiesKHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEVIDEOFORMATPROPERTIESKHR_EXISTS
layer_GetPhysicalDeviceVideoFormatPropertiesKHR(physicalDevice, pVideoFormatInfo, pVideoFormatPropertyCount, pVideoFormatProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceVideoFormatPropertiesKHR(physicalDevice, pVideoFormatInfo, pVideoFormatPropertyCount, pVideoFormatProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR* pQualityLevelInfo, VkVideoEncodeQualityLevelPropertiesKHR* pQualityLevelProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEVIDEOENCODEQUALITYLEVELPROPERTIESKHR_EXISTS
layer_GetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(physicalDevice, pQualityLevelInfo, pQualityLevelProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(physicalDevice, pQualityLevelInfo, pQualityLevelProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display ) {
scoped_lock l(global_lock);
#ifdef ACQUIREDRMDISPLAYEXT_EXISTS
layer_AcquireDrmDisplayEXT(physicalDevice, drmFd, display);
#endif 
instance_dispatch[GetKey(physicalDevice)].AcquireDrmDisplayEXT(physicalDevice, drmFd, display);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display ) {
scoped_lock l(global_lock);
#ifdef GETDRMDISPLAYEXT_EXISTS
layer_GetDrmDisplayEXT(physicalDevice, drmFd, connectorId, display);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetDrmDisplayEXT(physicalDevice, drmFd, connectorId, display);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceOpticalFlowImageFormatsNV(VkPhysicalDevice physicalDevice, VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEOPTICALFLOWIMAGEFORMATSNV_EXISTS
layer_GetPhysicalDeviceOpticalFlowImageFormatsNV(physicalDevice, pOpticalFlowImageFormatInfo, pFormatCount, pImageFormatProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceOpticalFlowImageFormatsNV(physicalDevice, pOpticalFlowImageFormatInfo, pFormatCount, pImageFormatProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceCooperativeMatrixPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICECOOPERATIVEMATRIXPROPERTIESKHR_EXISTS
layer_GetPhysicalDeviceCooperativeMatrixPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceCooperativeMatrixPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEFEATURES2KHR_EXISTS
layer_GetPhysicalDeviceFeatures2KHR(physicalDevice, pFeatures);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceFeatures2KHR(physicalDevice, pFeatures);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEPROPERTIES2KHR_EXISTS
layer_GetPhysicalDeviceProperties2KHR(physicalDevice, pProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceProperties2KHR(physicalDevice, pProperties);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEFORMATPROPERTIES2KHR_EXISTS
layer_GetPhysicalDeviceFormatProperties2KHR(physicalDevice, format, pFormatProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceFormatProperties2KHR(physicalDevice, format, pFormatProperties);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEIMAGEFORMATPROPERTIES2KHR_EXISTS
layer_GetPhysicalDeviceImageFormatProperties2KHR(physicalDevice, pImageFormatInfo, pImageFormatProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceImageFormatProperties2KHR(physicalDevice, pImageFormatInfo, pImageFormatProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceQueueFamilyProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEQUEUEFAMILYPROPERTIES2KHR_EXISTS
layer_GetPhysicalDeviceQueueFamilyProperties2KHR(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceQueueFamilyProperties2KHR(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEMEMORYPROPERTIES2KHR_EXISTS
layer_GetPhysicalDeviceMemoryProperties2KHR(physicalDevice, pMemoryProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceMemoryProperties2KHR(physicalDevice, pMemoryProperties);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceSparseImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICESPARSEIMAGEFORMATPROPERTIES2KHR_EXISTS
layer_GetPhysicalDeviceSparseImageFormatProperties2KHR(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceSparseImageFormatProperties2KHR(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceExternalBufferPropertiesKHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEEXTERNALBUFFERPROPERTIESKHR_EXISTS
layer_GetPhysicalDeviceExternalBufferPropertiesKHR(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceExternalBufferPropertiesKHR(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceExternalSemaphorePropertiesKHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEEXTERNALSEMAPHOREPROPERTIESKHR_EXISTS
layer_GetPhysicalDeviceExternalSemaphorePropertiesKHR(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceExternalSemaphorePropertiesKHR(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceExternalFencePropertiesKHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICEEXTERNALFENCEPROPERTIESKHR_EXISTS
layer_GetPhysicalDeviceExternalFencePropertiesKHR(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceExternalFencePropertiesKHR(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumeratePhysicalDeviceGroupsKHR(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties ) {
scoped_lock l(global_lock);
#ifdef ENUMERATEPHYSICALDEVICEGROUPSKHR_EXISTS
layer_EnumeratePhysicalDeviceGroupsKHR(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
#endif 
instance_dispatch[GetKey(instance)].EnumeratePhysicalDeviceGroupsKHR(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICECALIBRATEABLETIMEDOMAINSEXT_EXISTS
layer_GetPhysicalDeviceCalibrateableTimeDomainsEXT(physicalDevice, pTimeDomainCount, pTimeDomains);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceCalibrateableTimeDomainsEXT(physicalDevice, pTimeDomainCount, pTimeDomains);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties ) {
scoped_lock l(global_lock);
#ifdef GETPHYSICALDEVICETOOLPROPERTIESEXT_EXISTS
layer_GetPhysicalDeviceToolPropertiesEXT(physicalDevice, pToolCount, pToolProperties);
#endif 
instance_dispatch[GetKey(physicalDevice)].GetPhysicalDeviceToolPropertiesEXT(physicalDevice, pToolCount, pToolProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEQUEUE_EXISTS
layer_GetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
#endif 
device_dispatch[GetKey(device)].GetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_QueueSubmit(VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence ) {
scoped_lock l(global_lock);
#ifdef QUEUESUBMIT_EXISTS
layer_QueueSubmit(queue, submitCount, pSubmits, fence);
#endif 
device_dispatch[GetKey(queue)].QueueSubmit(queue, submitCount, pSubmits, fence);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_QueueWaitIdle(VkQueue queue ) {
scoped_lock l(global_lock);
#ifdef QUEUEWAITIDLE_EXISTS
layer_QueueWaitIdle(queue);
#endif 
device_dispatch[GetKey(queue)].QueueWaitIdle(queue);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_DeviceWaitIdle(VkDevice device ) {
scoped_lock l(global_lock);
#ifdef DEVICEWAITIDLE_EXISTS
layer_DeviceWaitIdle(device);
#endif 
device_dispatch[GetKey(device)].DeviceWaitIdle(device);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AllocateMemory(VkDevice device, VkMemoryAllocateInfo* pAllocateInfo, VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory ) {
scoped_lock l(global_lock);
#ifdef ALLOCATEMEMORY_EXISTS
layer_AllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
#endif 
device_dispatch[GetKey(device)].AllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_FreeMemory(VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef FREEMEMORY_EXISTS
layer_FreeMemory(device, memory, pAllocator);
#endif 
device_dispatch[GetKey(device)].FreeMemory(device, memory, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData ) {
scoped_lock l(global_lock);
#ifdef MAPMEMORY_EXISTS
layer_MapMemory(device, memory, offset, size, flags, ppData);
#endif 
device_dispatch[GetKey(device)].MapMemory(device, memory, offset, size, flags, ppData);

std::cout << "data = " << ppData << std::endl;
std::cout << "datadog = " << *ppData << std::endl;

return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_UnmapMemory(VkDevice device, VkDeviceMemory memory ) {
scoped_lock l(global_lock);
#ifdef UNMAPMEMORY_EXISTS
layer_UnmapMemory(device, memory);
#endif 
device_dispatch[GetKey(device)].UnmapMemory(device, memory);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, VkMappedMemoryRange* pMemoryRanges ) {
scoped_lock l(global_lock);
#ifdef FLUSHMAPPEDMEMORYRANGES_EXISTS
layer_FlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
#endif 
device_dispatch[GetKey(device)].FlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, VkMappedMemoryRange* pMemoryRanges ) {
scoped_lock l(global_lock);
#ifdef INVALIDATEMAPPEDMEMORYRANGES_EXISTS
layer_InvalidateMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
#endif 
device_dispatch[GetKey(device)].InvalidateMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEMEMORYCOMMITMENT_EXISTS
layer_GetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);
#endif 
device_dispatch[GetKey(device)].GetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETBUFFERMEMORYREQUIREMENTS_EXISTS
layer_GetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset ) {
scoped_lock l(global_lock);
#ifdef BINDBUFFERMEMORY_EXISTS
layer_BindBufferMemory(device, buffer, memory, memoryOffset);
#endif 
device_dispatch[GetKey(device)].BindBufferMemory(device, buffer, memory, memoryOffset);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETIMAGEMEMORYREQUIREMENTS_EXISTS
layer_GetImageMemoryRequirements(device, image, pMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetImageMemoryRequirements(device, image, pMemoryRequirements);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset ) {
scoped_lock l(global_lock);
#ifdef BINDIMAGEMEMORY_EXISTS
layer_BindImageMemory(device, image, memory, memoryOffset);
#endif 
device_dispatch[GetKey(device)].BindImageMemory(device, image, memory, memoryOffset);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETIMAGESPARSEMEMORYREQUIREMENTS_EXISTS
layer_GetImageSparseMemoryRequirements(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetImageSparseMemoryRequirements(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, VkBindSparseInfo* pBindInfo, VkFence fence ) {
scoped_lock l(global_lock);
#ifdef QUEUEBINDSPARSE_EXISTS
layer_QueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
#endif 
device_dispatch[GetKey(queue)].QueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateFence(VkDevice device, VkFenceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkFence* pFence ) {
scoped_lock l(global_lock);
#ifdef CREATEFENCE_EXISTS
layer_CreateFence(device, pCreateInfo, pAllocator, pFence);
#endif 
device_dispatch[GetKey(device)].CreateFence(device, pCreateInfo, pAllocator, pFence);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyFence(VkDevice device, VkFence fence, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYFENCE_EXISTS
layer_DestroyFence(device, fence, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyFence(device, fence, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ResetFences(VkDevice device, uint32_t fenceCount, VkFence* pFences ) {
scoped_lock l(global_lock);
#ifdef RESETFENCES_EXISTS
layer_ResetFences(device, fenceCount, pFences);
#endif 
device_dispatch[GetKey(device)].ResetFences(device, fenceCount, pFences);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetFenceStatus(VkDevice device, VkFence fence ) {
scoped_lock l(global_lock);
#ifdef GETFENCESTATUS_EXISTS
layer_GetFenceStatus(device, fence);
#endif 
device_dispatch[GetKey(device)].GetFenceStatus(device, fence);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_WaitForFences(VkDevice device, uint32_t fenceCount, VkFence* pFences, VkBool32 waitAll, uint64_t timeout ) {
scoped_lock l(global_lock);
#ifdef WAITFORFENCES_EXISTS
layer_WaitForFences(device, fenceCount, pFences, waitAll, timeout);
#endif 
device_dispatch[GetKey(device)].WaitForFences(device, fenceCount, pFences, waitAll, timeout);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateSemaphore(VkDevice device, VkSemaphoreCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore ) {
scoped_lock l(global_lock);
#ifdef CREATESEMAPHORE_EXISTS
layer_CreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore);
#endif 
device_dispatch[GetKey(device)].CreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroySemaphore(VkDevice device, VkSemaphore semaphore, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYSEMAPHORE_EXISTS
layer_DestroySemaphore(device, semaphore, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroySemaphore(device, semaphore, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateEvent(VkDevice device, VkEventCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkEvent* pEvent ) {
scoped_lock l(global_lock);
#ifdef CREATEEVENT_EXISTS
layer_CreateEvent(device, pCreateInfo, pAllocator, pEvent);
#endif 
device_dispatch[GetKey(device)].CreateEvent(device, pCreateInfo, pAllocator, pEvent);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyEvent(VkDevice device, VkEvent event, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYEVENT_EXISTS
layer_DestroyEvent(device, event, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyEvent(device, event, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetEventStatus(VkDevice device, VkEvent event ) {
scoped_lock l(global_lock);
#ifdef GETEVENTSTATUS_EXISTS
layer_GetEventStatus(device, event);
#endif 
device_dispatch[GetKey(device)].GetEventStatus(device, event);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SetEvent(VkDevice device, VkEvent event ) {
scoped_lock l(global_lock);
#ifdef SETEVENT_EXISTS
layer_SetEvent(device, event);
#endif 
device_dispatch[GetKey(device)].SetEvent(device, event);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ResetEvent(VkDevice device, VkEvent event ) {
scoped_lock l(global_lock);
#ifdef RESETEVENT_EXISTS
layer_ResetEvent(device, event);
#endif 
device_dispatch[GetKey(device)].ResetEvent(device, event);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateQueryPool(VkDevice device, VkQueryPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool ) {
scoped_lock l(global_lock);
#ifdef CREATEQUERYPOOL_EXISTS
layer_CreateQueryPool(device, pCreateInfo, pAllocator, pQueryPool);
#endif 
device_dispatch[GetKey(device)].CreateQueryPool(device, pCreateInfo, pAllocator, pQueryPool);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyQueryPool(VkDevice device, VkQueryPool queryPool, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYQUERYPOOL_EXISTS
layer_DestroyQueryPool(device, queryPool, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyQueryPool(device, queryPool, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags ) {
scoped_lock l(global_lock);
#ifdef GETQUERYPOOLRESULTS_EXISTS
layer_GetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
#endif 
device_dispatch[GetKey(device)].GetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_ResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount ) {
scoped_lock l(global_lock);
#ifdef RESETQUERYPOOL_EXISTS
layer_ResetQueryPool(device, queryPool, firstQuery, queryCount);
#endif 
device_dispatch[GetKey(device)].ResetQueryPool(device, queryPool, firstQuery, queryCount);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateBuffer(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer ) {
scoped_lock l(global_lock);
#ifdef CREATEBUFFER_EXISTS
layer_CreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
#endif 
device_dispatch[GetKey(device)].CreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyBuffer(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYBUFFER_EXISTS
layer_DestroyBuffer(device, buffer, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyBuffer(device, buffer, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateBufferView(VkDevice device, VkBufferViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBufferView* pView ) {
scoped_lock l(global_lock);
#ifdef CREATEBUFFERVIEW_EXISTS
layer_CreateBufferView(device, pCreateInfo, pAllocator, pView);
#endif 
device_dispatch[GetKey(device)].CreateBufferView(device, pCreateInfo, pAllocator, pView);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyBufferView(VkDevice device, VkBufferView bufferView, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYBUFFERVIEW_EXISTS
layer_DestroyBufferView(device, bufferView, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyBufferView(device, bufferView, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateImage(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage ) {
scoped_lock l(global_lock);
#ifdef CREATEIMAGE_EXISTS
layer_CreateImage(device, pCreateInfo, pAllocator, pImage);
#endif 
device_dispatch[GetKey(device)].CreateImage(device, pCreateInfo, pAllocator, pImage);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyImage(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYIMAGE_EXISTS
layer_DestroyImage(device, image, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyImage(device, image, pAllocator);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageSubresourceLayout(VkDevice device, VkImage image, VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout ) {
scoped_lock l(global_lock);
#ifdef GETIMAGESUBRESOURCELAYOUT_EXISTS
layer_GetImageSubresourceLayout(device, image, pSubresource, pLayout);
#endif 
device_dispatch[GetKey(device)].GetImageSubresourceLayout(device, image, pSubresource, pLayout);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateImageView(VkDevice device, VkImageViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImageView* pView ) {
scoped_lock l(global_lock);
#ifdef CREATEIMAGEVIEW_EXISTS
layer_CreateImageView(device, pCreateInfo, pAllocator, pView);
#endif 
device_dispatch[GetKey(device)].CreateImageView(device, pCreateInfo, pAllocator, pView);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyImageView(VkDevice device, VkImageView imageView, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYIMAGEVIEW_EXISTS
layer_DestroyImageView(device, imageView, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyImageView(device, imageView, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateShaderModule(VkDevice device, VkShaderModuleCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule ) {
scoped_lock l(global_lock);
#ifdef CREATESHADERMODULE_EXISTS
layer_CreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule);
#endif 
device_dispatch[GetKey(device)].CreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYSHADERMODULE_EXISTS
layer_DestroyShaderModule(device, shaderModule, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyShaderModule(device, shaderModule, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreatePipelineCache(VkDevice device, VkPipelineCacheCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache ) {
scoped_lock l(global_lock);
#ifdef CREATEPIPELINECACHE_EXISTS
layer_CreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
#endif 
device_dispatch[GetKey(device)].CreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYPIPELINECACHE_EXISTS
layer_DestroyPipelineCache(device, pipelineCache, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyPipelineCache(device, pipelineCache, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData ) {
scoped_lock l(global_lock);
#ifdef GETPIPELINECACHEDATA_EXISTS
layer_GetPipelineCacheData(device, pipelineCache, pDataSize, pData);
#endif 
device_dispatch[GetKey(device)].GetPipelineCacheData(device, pipelineCache, pDataSize, pData);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, VkPipelineCache* pSrcCaches ) {
scoped_lock l(global_lock);
#ifdef MERGEPIPELINECACHES_EXISTS
layer_MergePipelineCaches(device, dstCache, srcCacheCount, pSrcCaches);
#endif 
device_dispatch[GetKey(device)].MergePipelineCaches(device, dstCache, srcCacheCount, pSrcCaches);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkGraphicsPipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines ) {
scoped_lock l(global_lock);
#ifdef CREATEGRAPHICSPIPELINES_EXISTS
layer_CreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
#endif 
device_dispatch[GetKey(device)].CreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkComputePipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines ) {
scoped_lock l(global_lock);
#ifdef CREATECOMPUTEPIPELINES_EXISTS
layer_CreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
#endif 
device_dispatch[GetKey(device)].CreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize ) {
scoped_lock l(global_lock);
#ifdef GETDEVICESUBPASSSHADINGMAXWORKGROUPSIZEHUAWEI_EXISTS
layer_GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(device, renderpass, pMaxWorkgroupSize);
#endif 
device_dispatch[GetKey(device)].GetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(device, renderpass, pMaxWorkgroupSize);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyPipeline(VkDevice device, VkPipeline pipeline, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYPIPELINE_EXISTS
layer_DestroyPipeline(device, pipeline, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyPipeline(device, pipeline, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreatePipelineLayout(VkDevice device, VkPipelineLayoutCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout ) {
scoped_lock l(global_lock);
#ifdef CREATEPIPELINELAYOUT_EXISTS
layer_CreatePipelineLayout(device, pCreateInfo, pAllocator, pPipelineLayout);
#endif 
device_dispatch[GetKey(device)].CreatePipelineLayout(device, pCreateInfo, pAllocator, pPipelineLayout);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYPIPELINELAYOUT_EXISTS
layer_DestroyPipelineLayout(device, pipelineLayout, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyPipelineLayout(device, pipelineLayout, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateSampler(VkDevice device, VkSamplerCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSampler* pSampler ) {
scoped_lock l(global_lock);
#ifdef CREATESAMPLER_EXISTS
layer_CreateSampler(device, pCreateInfo, pAllocator, pSampler);
#endif 
device_dispatch[GetKey(device)].CreateSampler(device, pCreateInfo, pAllocator, pSampler);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroySampler(VkDevice device, VkSampler sampler, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYSAMPLER_EXISTS
layer_DestroySampler(device, sampler, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroySampler(device, sampler, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDescriptorSetLayout(VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout ) {
scoped_lock l(global_lock);
#ifdef CREATEDESCRIPTORSETLAYOUT_EXISTS
layer_CreateDescriptorSetLayout(device, pCreateInfo, pAllocator, pSetLayout);
#endif 
device_dispatch[GetKey(device)].CreateDescriptorSetLayout(device, pCreateInfo, pAllocator, pSetLayout);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYDESCRIPTORSETLAYOUT_EXISTS
layer_DestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDescriptorPool(VkDevice device, VkDescriptorPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool ) {
scoped_lock l(global_lock);
#ifdef CREATEDESCRIPTORPOOL_EXISTS
layer_CreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
#endif 
device_dispatch[GetKey(device)].CreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYDESCRIPTORPOOL_EXISTS
layer_DestroyDescriptorPool(device, descriptorPool, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyDescriptorPool(device, descriptorPool, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags ) {
scoped_lock l(global_lock);
#ifdef RESETDESCRIPTORPOOL_EXISTS
layer_ResetDescriptorPool(device, descriptorPool, flags);
#endif 
device_dispatch[GetKey(device)].ResetDescriptorPool(device, descriptorPool, flags);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AllocateDescriptorSets(VkDevice device, VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets ) {
scoped_lock l(global_lock);
#ifdef ALLOCATEDESCRIPTORSETS_EXISTS
layer_AllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
#endif 
device_dispatch[GetKey(device)].AllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, VkDescriptorSet* pDescriptorSets ) {
scoped_lock l(global_lock);
#ifdef FREEDESCRIPTORSETS_EXISTS
layer_FreeDescriptorSets(device, descriptorPool, descriptorSetCount, pDescriptorSets);
#endif 
device_dispatch[GetKey(device)].FreeDescriptorSets(device, descriptorPool, descriptorSetCount, pDescriptorSets);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, VkCopyDescriptorSet* pDescriptorCopies ) {
scoped_lock l(global_lock);
#ifdef UPDATEDESCRIPTORSETS_EXISTS
layer_UpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
#endif 
device_dispatch[GetKey(device)].UpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateFramebuffer(VkDevice device, VkFramebufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer ) {
scoped_lock l(global_lock);
#ifdef CREATEFRAMEBUFFER_EXISTS
layer_CreateFramebuffer(device, pCreateInfo, pAllocator, pFramebuffer);
#endif 
device_dispatch[GetKey(device)].CreateFramebuffer(device, pCreateInfo, pAllocator, pFramebuffer);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYFRAMEBUFFER_EXISTS
layer_DestroyFramebuffer(device, framebuffer, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyFramebuffer(device, framebuffer, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateRenderPass(VkDevice device, VkRenderPassCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass ) {
scoped_lock l(global_lock);
#ifdef CREATERENDERPASS_EXISTS
layer_CreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
#endif 
device_dispatch[GetKey(device)].CreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyRenderPass(VkDevice device, VkRenderPass renderPass, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYRENDERPASS_EXISTS
layer_DestroyRenderPass(device, renderPass, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyRenderPass(device, renderPass, pAllocator);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity ) {
scoped_lock l(global_lock);
#ifdef GETRENDERAREAGRANULARITY_EXISTS
layer_GetRenderAreaGranularity(device, renderPass, pGranularity);
#endif 
device_dispatch[GetKey(device)].GetRenderAreaGranularity(device, renderPass, pGranularity);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetRenderingAreaGranularityKHR(VkDevice device, VkRenderingAreaInfoKHR* pRenderingAreaInfo, VkExtent2D* pGranularity ) {
scoped_lock l(global_lock);
#ifdef GETRENDERINGAREAGRANULARITYKHR_EXISTS
layer_GetRenderingAreaGranularityKHR(device, pRenderingAreaInfo, pGranularity);
#endif 
device_dispatch[GetKey(device)].GetRenderingAreaGranularityKHR(device, pRenderingAreaInfo, pGranularity);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateCommandPool(VkDevice device, VkCommandPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool ) {
scoped_lock l(global_lock);
#ifdef CREATECOMMANDPOOL_EXISTS
layer_CreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
#endif 
device_dispatch[GetKey(device)].CreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyCommandPool(VkDevice device, VkCommandPool commandPool, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYCOMMANDPOOL_EXISTS
layer_DestroyCommandPool(device, commandPool, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyCommandPool(device, commandPool, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags ) {
scoped_lock l(global_lock);
#ifdef RESETCOMMANDPOOL_EXISTS
layer_ResetCommandPool(device, commandPool, flags);
#endif 
device_dispatch[GetKey(device)].ResetCommandPool(device, commandPool, flags);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AllocateCommandBuffers(VkDevice device, VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers ) {
scoped_lock l(global_lock);
#ifdef ALLOCATECOMMANDBUFFERS_EXISTS
layer_AllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
#endif 
auto ret = device_dispatch[GetKey(device)].AllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
std::cout << "chad pCommandBuffers = " << *pCommandBuffers << std::endl;                                                                                  
return ret;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers ) {
scoped_lock l(global_lock);
#ifdef FREECOMMANDBUFFERS_EXISTS
layer_FreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
#endif 
device_dispatch[GetKey(device)].FreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BeginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo* pBeginInfo ) {
scoped_lock l(global_lock);
#ifdef BEGINCOMMANDBUFFER_EXISTS
layer_BeginCommandBuffer(commandBuffer, pBeginInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].BeginCommandBuffer(commandBuffer, pBeginInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EndCommandBuffer(VkCommandBuffer commandBuffer ) {
scoped_lock l(global_lock);
#ifdef ENDCOMMANDBUFFER_EXISTS
layer_EndCommandBuffer(commandBuffer);
#endif 
device_dispatch[GetKey(commandBuffer)].EndCommandBuffer(commandBuffer);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags ) {
scoped_lock l(global_lock);
#ifdef RESETCOMMANDBUFFER_EXISTS
layer_ResetCommandBuffer(commandBuffer, flags);
#endif 
device_dispatch[GetKey(commandBuffer)].ResetCommandBuffer(commandBuffer, flags);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline ) {
scoped_lock l(global_lock);
#ifdef CMDBINDPIPELINE_EXISTS
layer_CmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetAttachmentFeedbackLoopEnableEXT(VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask ) {
scoped_lock l(global_lock);
#ifdef CMDSETATTACHMENTFEEDBACKLOOPENABLEEXT_EXISTS
layer_CmdSetAttachmentFeedbackLoopEnableEXT(commandBuffer, aspectMask);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetAttachmentFeedbackLoopEnableEXT(commandBuffer, aspectMask);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewport* pViewports ) {
scoped_lock l(global_lock);
#ifdef CMDSETVIEWPORT_EXISTS
layer_CmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, VkRect2D* pScissors ) {
scoped_lock l(global_lock);
#ifdef CMDSETSCISSOR_EXISTS
layer_CmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth ) {
scoped_lock l(global_lock);
#ifdef CMDSETLINEWIDTH_EXISTS
layer_CmdSetLineWidth(commandBuffer, lineWidth);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetLineWidth(commandBuffer, lineWidth);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEPTHBIAS_EXISTS
layer_CmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetBlendConstants(VkCommandBuffer commandBuffer, float* blendConstants ) {
scoped_lock l(global_lock);
#ifdef CMDSETBLENDCONSTANTS_EXISTS
layer_CmdSetBlendConstants(commandBuffer, blendConstants);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetBlendConstants(commandBuffer, blendConstants);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEPTHBOUNDS_EXISTS
layer_CmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask ) {
scoped_lock l(global_lock);
#ifdef CMDSETSTENCILCOMPAREMASK_EXISTS
layer_CmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask ) {
scoped_lock l(global_lock);
#ifdef CMDSETSTENCILWRITEMASK_EXISTS
layer_CmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference ) {
scoped_lock l(global_lock);
#ifdef CMDSETSTENCILREFERENCE_EXISTS
layer_CmdSetStencilReference(commandBuffer, faceMask, reference);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetStencilReference(commandBuffer, faceMask, reference);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, uint32_t* pDynamicOffsets ) {
scoped_lock l(global_lock);
#ifdef CMDBINDDESCRIPTORSETS_EXISTS
layer_CmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType ) {
scoped_lock l(global_lock);
#ifdef CMDBINDINDEXBUFFER_EXISTS
layer_CmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets ) {
scoped_lock l(global_lock);
#ifdef CMDBINDVERTEXBUFFERS_EXISTS
layer_CmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance ) {
scoped_lock l(global_lock);

#ifdef CMDDRAW_EXISTS
layer_CmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
#endif 


device_dispatch[GetKey(commandBuffer)].CmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance ) {
scoped_lock l(global_lock);

#ifdef CMDDRAWINDEXED_EXISTS
layer_CmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
#endif 

device_dispatch[GetKey(commandBuffer)].CmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWMULTIEXT_EXISTS
layer_CmdDrawMultiEXT(commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawMultiEXT(commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, int32_t* pVertexOffset ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWMULTIINDEXEDEXT_EXISTS
layer_CmdDrawMultiIndexedEXT(commandBuffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawMultiIndexedEXT(commandBuffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWINDIRECT_EXISTS
layer_CmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWINDEXEDINDIRECT_EXISTS
layer_CmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ ) {
scoped_lock l(global_lock);
#ifdef CMDDISPATCH_EXISTS
layer_CmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset ) {
scoped_lock l(global_lock);
#ifdef CMDDISPATCHINDIRECT_EXISTS
layer_CmdDispatchIndirect(commandBuffer, buffer, offset);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDispatchIndirect(commandBuffer, buffer, offset);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer ) {
scoped_lock l(global_lock);
#ifdef CMDSUBPASSSHADINGHUAWEI_EXISTS
layer_CmdSubpassShadingHUAWEI(commandBuffer);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSubpassShadingHUAWEI(commandBuffer);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWCLUSTERHUAWEI_EXISTS
layer_CmdDrawClusterHUAWEI(commandBuffer, groupCountX, groupCountY, groupCountZ);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawClusterHUAWEI(commandBuffer, groupCountX, groupCountY, groupCountZ);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWCLUSTERINDIRECTHUAWEI_EXISTS
layer_CmdDrawClusterIndirectHUAWEI(commandBuffer, buffer, offset);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawClusterIndirectHUAWEI(commandBuffer, buffer, offset);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdUpdatePipelineIndirectBufferNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline ) {
scoped_lock l(global_lock);
#ifdef CMDUPDATEPIPELINEINDIRECTBUFFERNV_EXISTS
layer_CmdUpdatePipelineIndirectBufferNV(commandBuffer, pipelineBindPoint, pipeline);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdUpdatePipelineIndirectBufferNV(commandBuffer, pipelineBindPoint, pipeline);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy* pRegions ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYBUFFER_EXISTS
layer_CmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageCopy* pRegions ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYIMAGE_EXISTS
layer_CmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageBlit* pRegions, VkFilter filter ) {
scoped_lock l(global_lock);
#ifdef CMDBLITIMAGE_EXISTS
layer_CmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkBufferImageCopy* pRegions ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYBUFFERTOIMAGE_EXISTS
layer_CmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, VkBufferImageCopy* pRegions ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYIMAGETOBUFFER_EXISTS
layer_CmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYMEMORYINDIRECTNV_EXISTS
layer_CmdCopyMemoryIndirectNV(commandBuffer, copyBufferAddress, copyCount, stride);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyMemoryIndirectNV(commandBuffer, copyBufferAddress, copyCount, stride);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, VkImageSubresourceLayers* pImageSubresources ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYMEMORYTOIMAGEINDIRECTNV_EXISTS
layer_CmdCopyMemoryToImageIndirectNV(commandBuffer, copyBufferAddress, copyCount, stride, dstImage, dstImageLayout, pImageSubresources);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyMemoryToImageIndirectNV(commandBuffer, copyBufferAddress, copyCount, stride, dstImage, dstImageLayout, pImageSubresources);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, void* pData ) {
scoped_lock l(global_lock);
#ifdef CMDUPDATEBUFFER_EXISTS
layer_CmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data ) {
scoped_lock l(global_lock);
#ifdef CMDFILLBUFFER_EXISTS
layer_CmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearColorValue* pColor, uint32_t rangeCount, VkImageSubresourceRange* pRanges ) {
scoped_lock l(global_lock);
#ifdef CMDCLEARCOLORIMAGE_EXISTS
layer_CmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, VkImageSubresourceRange* pRanges ) {
scoped_lock l(global_lock);
#ifdef CMDCLEARDEPTHSTENCILIMAGE_EXISTS
layer_CmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, VkClearAttachment* pAttachments, uint32_t rectCount, VkClearRect* pRects ) {
scoped_lock l(global_lock);
#ifdef CMDCLEARATTACHMENTS_EXISTS
layer_CmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageResolve* pRegions ) {
scoped_lock l(global_lock);
#ifdef CMDRESOLVEIMAGE_EXISTS
layer_CmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask ) {
scoped_lock l(global_lock);
#ifdef CMDSETEVENT_EXISTS
layer_CmdSetEvent(commandBuffer, event, stageMask);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetEvent(commandBuffer, event, stageMask);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask ) {
scoped_lock l(global_lock);
#ifdef CMDRESETEVENT_EXISTS
layer_CmdResetEvent(commandBuffer, event, stageMask);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdResetEvent(commandBuffer, event, stageMask);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier* pImageMemoryBarriers ) {
scoped_lock l(global_lock);
#ifdef CMDWAITEVENTS_EXISTS
layer_CmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier* pImageMemoryBarriers ) {
scoped_lock l(global_lock);
#ifdef CMDPIPELINEBARRIER_EXISTS
layer_CmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags ) {
scoped_lock l(global_lock);
#ifdef CMDBEGINQUERY_EXISTS
layer_CmdBeginQuery(commandBuffer, queryPool, query, flags);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginQuery(commandBuffer, queryPool, query, flags);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query ) {
scoped_lock l(global_lock);
#ifdef CMDENDQUERY_EXISTS
layer_CmdEndQuery(commandBuffer, queryPool, query);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndQuery(commandBuffer, queryPool, query);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin ) {
scoped_lock l(global_lock);
#ifdef CMDBEGINCONDITIONALRENDERINGEXT_EXISTS
layer_CmdBeginConditionalRenderingEXT(commandBuffer, pConditionalRenderingBegin);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginConditionalRenderingEXT(commandBuffer, pConditionalRenderingBegin);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer ) {
scoped_lock l(global_lock);
#ifdef CMDENDCONDITIONALRENDERINGEXT_EXISTS
layer_CmdEndConditionalRenderingEXT(commandBuffer);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndConditionalRenderingEXT(commandBuffer);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount ) {
scoped_lock l(global_lock);
#ifdef CMDRESETQUERYPOOL_EXISTS
layer_CmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query ) {
scoped_lock l(global_lock);
#ifdef CMDWRITETIMESTAMP_EXISTS
layer_CmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYQUERYPOOLRESULTS_EXISTS
layer_CmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, void* pValues ) {
scoped_lock l(global_lock);
#ifdef CMDPUSHCONSTANTS_EXISTS
layer_CmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginRenderPass(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents ) {
scoped_lock l(global_lock);
#ifdef CMDBEGINRENDERPASS_EXISTS
layer_CmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents ) {
scoped_lock l(global_lock);
#ifdef CMDNEXTSUBPASS_EXISTS
layer_CmdNextSubpass(commandBuffer, contents);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdNextSubpass(commandBuffer, contents);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndRenderPass(VkCommandBuffer commandBuffer ) {
scoped_lock l(global_lock);
#ifdef CMDENDRENDERPASS_EXISTS
layer_CmdEndRenderPass(commandBuffer);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndRenderPass(commandBuffer);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers ) {
scoped_lock l(global_lock);
#ifdef CMDEXECUTECOMMANDS_EXISTS
layer_CmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, VkSwapchainCreateInfoKHR* pCreateInfos, VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains ) {
scoped_lock l(global_lock);
#ifdef CREATESHAREDSWAPCHAINSKHR_EXISTS
layer_CreateSharedSwapchainsKHR(device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
#endif 
device_dispatch[GetKey(device)].CreateSharedSwapchainsKHR(device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateSwapchainKHR(VkDevice device, VkSwapchainCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain ) {
scoped_lock l(global_lock);
#ifdef CREATESWAPCHAINKHR_EXISTS
layer_CreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
#endif 
device_dispatch[GetKey(device)].CreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYSWAPCHAINKHR_EXISTS
layer_DestroySwapchainKHR(device, swapchain, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroySwapchainKHR(device, swapchain, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages ) {
scoped_lock l(global_lock);
#ifdef GETSWAPCHAINIMAGESKHR_EXISTS
layer_GetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
#endif 
device_dispatch[GetKey(device)].GetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex ) {
scoped_lock l(global_lock);
#ifdef ACQUIRENEXTIMAGEKHR_EXISTS
layer_AcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
#endif 
device_dispatch[GetKey(device)].AcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_QueuePresentKHR(VkQueue queue, VkPresentInfoKHR* pPresentInfo ) {
scoped_lock l(global_lock);
#ifdef QUEUEPRESENTKHR_EXISTS
layer_QueuePresentKHR(queue, pPresentInfo);
#endif 
device_dispatch[GetKey(queue)].QueuePresentKHR(queue, pPresentInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_DebugMarkerSetObjectNameEXT(VkDevice device, VkDebugMarkerObjectNameInfoEXT* pNameInfo ) {
scoped_lock l(global_lock);
#ifdef DEBUGMARKERSETOBJECTNAMEEXT_EXISTS
layer_DebugMarkerSetObjectNameEXT(device, pNameInfo);
#endif 
device_dispatch[GetKey(device)].DebugMarkerSetObjectNameEXT(device, pNameInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_DebugMarkerSetObjectTagEXT(VkDevice device, VkDebugMarkerObjectTagInfoEXT* pTagInfo ) {
scoped_lock l(global_lock);
#ifdef DEBUGMARKERSETOBJECTTAGEXT_EXISTS
layer_DebugMarkerSetObjectTagEXT(device, pTagInfo);
#endif 
device_dispatch[GetKey(device)].DebugMarkerSetObjectTagEXT(device, pTagInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, VkDebugMarkerMarkerInfoEXT* pMarkerInfo ) {
scoped_lock l(global_lock);
#ifdef CMDDEBUGMARKERBEGINEXT_EXISTS
layer_CmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer ) {
scoped_lock l(global_lock);
#ifdef CMDDEBUGMARKERENDEXT_EXISTS
layer_CmdDebugMarkerEndEXT(commandBuffer);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDebugMarkerEndEXT(commandBuffer);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, VkDebugMarkerMarkerInfoEXT* pMarkerInfo ) {
scoped_lock l(global_lock);
#ifdef CMDDEBUGMARKERINSERTEXT_EXISTS
layer_CmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
}

#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle ) {
scoped_lock l(global_lock);
#ifdef GETMEMORYWIN32HANDLENV_EXISTS
layer_GetMemoryWin32HandleNV(device, memory, handleType, pHandle);
#endif 
device_dispatch[GetKey(device)].GetMemoryWin32HandleNV(device, memory, handleType, pHandle);
return VK_SUCCESS;
}

#endif
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo ) {
scoped_lock l(global_lock);
#ifdef CMDEXECUTEGENERATEDCOMMANDSNV_EXISTS
layer_CmdExecuteGeneratedCommandsNV(commandBuffer, isPreprocessed, pGeneratedCommandsInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdExecuteGeneratedCommandsNV(commandBuffer, isPreprocessed, pGeneratedCommandsInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo ) {
scoped_lock l(global_lock);
#ifdef CMDPREPROCESSGENERATEDCOMMANDSNV_EXISTS
layer_CmdPreprocessGeneratedCommandsNV(commandBuffer, pGeneratedCommandsInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPreprocessGeneratedCommandsNV(commandBuffer, pGeneratedCommandsInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex ) {
scoped_lock l(global_lock);
#ifdef CMDBINDPIPELINESHADERGROUPNV_EXISTS
layer_CmdBindPipelineShaderGroupNV(commandBuffer, pipelineBindPoint, pipeline, groupIndex);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindPipelineShaderGroupNV(commandBuffer, pipelineBindPoint, pipeline, groupIndex);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetGeneratedCommandsMemoryRequirementsNV(VkDevice device, VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETGENERATEDCOMMANDSMEMORYREQUIREMENTSNV_EXISTS
layer_GetGeneratedCommandsMemoryRequirementsNV(device, pInfo, pMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetGeneratedCommandsMemoryRequirementsNV(device, pInfo, pMemoryRequirements);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout ) {
scoped_lock l(global_lock);
#ifdef CREATEINDIRECTCOMMANDSLAYOUTNV_EXISTS
layer_CreateIndirectCommandsLayoutNV(device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
#endif 
device_dispatch[GetKey(device)].CreateIndirectCommandsLayoutNV(device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYINDIRECTCOMMANDSLAYOUTNV_EXISTS
layer_DestroyIndirectCommandsLayoutNV(device, indirectCommandsLayout, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyIndirectCommandsLayoutNV(device, indirectCommandsLayout, pAllocator);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, VkWriteDescriptorSet* pDescriptorWrites ) {
scoped_lock l(global_lock);
#ifdef CMDPUSHDESCRIPTORSETKHR_EXISTS
layer_CmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_TrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags ) {
scoped_lock l(global_lock);
#ifdef TRIMCOMMANDPOOL_EXISTS
layer_TrimCommandPool(device, commandPool, flags);
#endif 
device_dispatch[GetKey(device)].TrimCommandPool(device, commandPool, flags);
}

#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryWin32HandleKHR(VkDevice device, VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle ) {
scoped_lock l(global_lock);
#ifdef GETMEMORYWIN32HANDLEKHR_EXISTS
layer_GetMemoryWin32HandleKHR(device, pGetWin32HandleInfo, pHandle);
#endif 
device_dispatch[GetKey(device)].GetMemoryWin32HandleKHR(device, pGetWin32HandleInfo, pHandle);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties ) {
scoped_lock l(global_lock);
#ifdef GETMEMORYWIN32HANDLEPROPERTIESKHR_EXISTS
layer_GetMemoryWin32HandlePropertiesKHR(device, handleType, handle, pMemoryWin32HandleProperties);
#endif 
device_dispatch[GetKey(device)].GetMemoryWin32HandlePropertiesKHR(device, handleType, handle, pMemoryWin32HandleProperties);
return VK_SUCCESS;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryFdKHR(VkDevice device, VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd ) {
scoped_lock l(global_lock);
#ifdef GETMEMORYFDKHR_EXISTS
layer_GetMemoryFdKHR(device, pGetFdInfo, pFd);
#endif 
device_dispatch[GetKey(device)].GetMemoryFdKHR(device, pGetFdInfo, pFd);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties ) {
scoped_lock l(global_lock);
#ifdef GETMEMORYFDPROPERTIESKHR_EXISTS
layer_GetMemoryFdPropertiesKHR(device, handleType, fd, pMemoryFdProperties);
#endif 
device_dispatch[GetKey(device)].GetMemoryFdPropertiesKHR(device, handleType, fd, pMemoryFdProperties);
return VK_SUCCESS;
}

#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryZirconHandleFUCHSIA(VkDevice device, VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle ) {
scoped_lock l(global_lock);
#ifdef GETMEMORYZIRCONHANDLEFUCHSIA_EXISTS
layer_GetMemoryZirconHandleFUCHSIA(device, pGetZirconHandleInfo, pZirconHandle);
#endif 
device_dispatch[GetKey(device)].GetMemoryZirconHandleFUCHSIA(device, pGetZirconHandleInfo, pZirconHandle);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryZirconHandlePropertiesFUCHSIA(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties ) {
scoped_lock l(global_lock);
#ifdef GETMEMORYZIRCONHANDLEPROPERTIESFUCHSIA_EXISTS
layer_GetMemoryZirconHandlePropertiesFUCHSIA(device, handleType, zirconHandle, pMemoryZirconHandleProperties);
#endif 
device_dispatch[GetKey(device)].GetMemoryZirconHandlePropertiesFUCHSIA(device, handleType, zirconHandle, pMemoryZirconHandleProperties);
return VK_SUCCESS;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryRemoteAddressNV(VkDevice device, VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress ) {
scoped_lock l(global_lock);
#ifdef GETMEMORYREMOTEADDRESSNV_EXISTS
layer_GetMemoryRemoteAddressNV(device, pMemoryGetRemoteAddressInfo, pAddress);
#endif 
device_dispatch[GetKey(device)].GetMemoryRemoteAddressNV(device, pMemoryGetRemoteAddressInfo, pAddress);
return VK_SUCCESS;
}

#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemorySciBufNV(VkDevice device, VkMemoryGetSciBufInfoNV* pGetSciBufInfo, NvSciBufObj* pHandle ) {
scoped_lock l(global_lock);
#ifdef GETMEMORYSCIBUFNV_EXISTS
layer_GetMemorySciBufNV(device, pGetSciBufInfo, pHandle);
#endif 
device_dispatch[GetKey(device)].GetMemorySciBufNV(device, pGetSciBufInfo, pHandle);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSemaphoreWin32HandleKHR(VkDevice device, VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle ) {
scoped_lock l(global_lock);
#ifdef GETSEMAPHOREWIN32HANDLEKHR_EXISTS
layer_GetSemaphoreWin32HandleKHR(device, pGetWin32HandleInfo, pHandle);
#endif 
device_dispatch[GetKey(device)].GetSemaphoreWin32HandleKHR(device, pGetWin32HandleInfo, pHandle);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ImportSemaphoreWin32HandleKHR(VkDevice device, VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo ) {
scoped_lock l(global_lock);
#ifdef IMPORTSEMAPHOREWIN32HANDLEKHR_EXISTS
layer_ImportSemaphoreWin32HandleKHR(device, pImportSemaphoreWin32HandleInfo);
#endif 
device_dispatch[GetKey(device)].ImportSemaphoreWin32HandleKHR(device, pImportSemaphoreWin32HandleInfo);
return VK_SUCCESS;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSemaphoreFdKHR(VkDevice device, VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd ) {
scoped_lock l(global_lock);
#ifdef GETSEMAPHOREFDKHR_EXISTS
layer_GetSemaphoreFdKHR(device, pGetFdInfo, pFd);
#endif 
device_dispatch[GetKey(device)].GetSemaphoreFdKHR(device, pGetFdInfo, pFd);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ImportSemaphoreFdKHR(VkDevice device, VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo ) {
scoped_lock l(global_lock);
#ifdef IMPORTSEMAPHOREFDKHR_EXISTS
layer_ImportSemaphoreFdKHR(device, pImportSemaphoreFdInfo);
#endif 
device_dispatch[GetKey(device)].ImportSemaphoreFdKHR(device, pImportSemaphoreFdInfo);
return VK_SUCCESS;
}

#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSemaphoreZirconHandleFUCHSIA(VkDevice device, VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle ) {
scoped_lock l(global_lock);
#ifdef GETSEMAPHOREZIRCONHANDLEFUCHSIA_EXISTS
layer_GetSemaphoreZirconHandleFUCHSIA(device, pGetZirconHandleInfo, pZirconHandle);
#endif 
device_dispatch[GetKey(device)].GetSemaphoreZirconHandleFUCHSIA(device, pGetZirconHandleInfo, pZirconHandle);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ImportSemaphoreZirconHandleFUCHSIA(VkDevice device, VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo ) {
scoped_lock l(global_lock);
#ifdef IMPORTSEMAPHOREZIRCONHANDLEFUCHSIA_EXISTS
layer_ImportSemaphoreZirconHandleFUCHSIA(device, pImportSemaphoreZirconHandleInfo);
#endif 
device_dispatch[GetKey(device)].ImportSemaphoreZirconHandleFUCHSIA(device, pImportSemaphoreZirconHandleInfo);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetFenceWin32HandleKHR(VkDevice device, VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle ) {
scoped_lock l(global_lock);
#ifdef GETFENCEWIN32HANDLEKHR_EXISTS
layer_GetFenceWin32HandleKHR(device, pGetWin32HandleInfo, pHandle);
#endif 
device_dispatch[GetKey(device)].GetFenceWin32HandleKHR(device, pGetWin32HandleInfo, pHandle);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ImportFenceWin32HandleKHR(VkDevice device, VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo ) {
scoped_lock l(global_lock);
#ifdef IMPORTFENCEWIN32HANDLEKHR_EXISTS
layer_ImportFenceWin32HandleKHR(device, pImportFenceWin32HandleInfo);
#endif 
device_dispatch[GetKey(device)].ImportFenceWin32HandleKHR(device, pImportFenceWin32HandleInfo);
return VK_SUCCESS;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetFenceFdKHR(VkDevice device, VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd ) {
scoped_lock l(global_lock);
#ifdef GETFENCEFDKHR_EXISTS
layer_GetFenceFdKHR(device, pGetFdInfo, pFd);
#endif 
device_dispatch[GetKey(device)].GetFenceFdKHR(device, pGetFdInfo, pFd);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ImportFenceFdKHR(VkDevice device, VkImportFenceFdInfoKHR* pImportFenceFdInfo ) {
scoped_lock l(global_lock);
#ifdef IMPORTFENCEFDKHR_EXISTS
layer_ImportFenceFdKHR(device, pImportFenceFdInfo);
#endif 
device_dispatch[GetKey(device)].ImportFenceFdKHR(device, pImportFenceFdInfo);
return VK_SUCCESS;
}

#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetFenceSciSyncFenceNV(VkDevice device, VkFenceGetSciSyncInfoNV* pGetSciSyncHandleInfo, void* pHandle ) {
scoped_lock l(global_lock);
#ifdef GETFENCESCISYNCFENCENV_EXISTS
layer_GetFenceSciSyncFenceNV(device, pGetSciSyncHandleInfo, pHandle);
#endif 
device_dispatch[GetKey(device)].GetFenceSciSyncFenceNV(device, pGetSciSyncHandleInfo, pHandle);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetFenceSciSyncObjNV(VkDevice device, VkFenceGetSciSyncInfoNV* pGetSciSyncHandleInfo, void* pHandle ) {
scoped_lock l(global_lock);
#ifdef GETFENCESCISYNCOBJNV_EXISTS
layer_GetFenceSciSyncObjNV(device, pGetSciSyncHandleInfo, pHandle);
#endif 
device_dispatch[GetKey(device)].GetFenceSciSyncObjNV(device, pGetSciSyncHandleInfo, pHandle);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ImportFenceSciSyncFenceNV(VkDevice device, VkImportFenceSciSyncInfoNV* pImportFenceSciSyncInfo ) {
scoped_lock l(global_lock);
#ifdef IMPORTFENCESCISYNCFENCENV_EXISTS
layer_ImportFenceSciSyncFenceNV(device, pImportFenceSciSyncInfo);
#endif 
device_dispatch[GetKey(device)].ImportFenceSciSyncFenceNV(device, pImportFenceSciSyncInfo);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ImportFenceSciSyncObjNV(VkDevice device, VkImportFenceSciSyncInfoNV* pImportFenceSciSyncInfo ) {
scoped_lock l(global_lock);
#ifdef IMPORTFENCESCISYNCOBJNV_EXISTS
layer_ImportFenceSciSyncObjNV(device, pImportFenceSciSyncInfo);
#endif 
device_dispatch[GetKey(device)].ImportFenceSciSyncObjNV(device, pImportFenceSciSyncInfo);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSemaphoreSciSyncObjNV(VkDevice device, VkSemaphoreGetSciSyncInfoNV* pGetSciSyncInfo, void* pHandle ) {
scoped_lock l(global_lock);
#ifdef GETSEMAPHORESCISYNCOBJNV_EXISTS
layer_GetSemaphoreSciSyncObjNV(device, pGetSciSyncInfo, pHandle);
#endif 
device_dispatch[GetKey(device)].GetSemaphoreSciSyncObjNV(device, pGetSciSyncInfo, pHandle);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ImportSemaphoreSciSyncObjNV(VkDevice device, VkImportSemaphoreSciSyncInfoNV* pImportSemaphoreSciSyncInfo ) {
scoped_lock l(global_lock);
#ifdef IMPORTSEMAPHORESCISYNCOBJNV_EXISTS
layer_ImportSemaphoreSciSyncObjNV(device, pImportSemaphoreSciSyncInfo);
#endif 
device_dispatch[GetKey(device)].ImportSemaphoreSciSyncObjNV(device, pImportSemaphoreSciSyncInfo);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateSemaphoreSciSyncPoolNV(VkDevice device, VkSemaphoreSciSyncPoolCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSemaphoreSciSyncPoolNV* pSemaphorePool ) {
scoped_lock l(global_lock);
#ifdef CREATESEMAPHORESCISYNCPOOLNV_EXISTS
layer_CreateSemaphoreSciSyncPoolNV(device, pCreateInfo, pAllocator, pSemaphorePool);
#endif 
device_dispatch[GetKey(device)].CreateSemaphoreSciSyncPoolNV(device, pCreateInfo, pAllocator, pSemaphorePool);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_SCI)
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroySemaphoreSciSyncPoolNV(VkDevice device, VkSemaphoreSciSyncPoolNV semaphorePool, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYSEMAPHORESCISYNCPOOLNV_EXISTS
layer_DestroySemaphoreSciSyncPoolNV(device, semaphorePool, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroySemaphoreSciSyncPoolNV(device, semaphorePool, pAllocator);
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_DisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, VkDisplayPowerInfoEXT* pDisplayPowerInfo ) {
scoped_lock l(global_lock);
#ifdef DISPLAYPOWERCONTROLEXT_EXISTS
layer_DisplayPowerControlEXT(device, display, pDisplayPowerInfo);
#endif 
device_dispatch[GetKey(device)].DisplayPowerControlEXT(device, display, pDisplayPowerInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_RegisterDeviceEventEXT(VkDevice device, VkDeviceEventInfoEXT* pDeviceEventInfo, VkAllocationCallbacks* pAllocator, VkFence* pFence ) {
scoped_lock l(global_lock);
#ifdef REGISTERDEVICEEVENTEXT_EXISTS
layer_RegisterDeviceEventEXT(device, pDeviceEventInfo, pAllocator, pFence);
#endif 
device_dispatch[GetKey(device)].RegisterDeviceEventEXT(device, pDeviceEventInfo, pAllocator, pFence);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_RegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, VkDisplayEventInfoEXT* pDisplayEventInfo, VkAllocationCallbacks* pAllocator, VkFence* pFence ) {
scoped_lock l(global_lock);
#ifdef REGISTERDISPLAYEVENTEXT_EXISTS
layer_RegisterDisplayEventEXT(device, display, pDisplayEventInfo, pAllocator, pFence);
#endif 
device_dispatch[GetKey(device)].RegisterDisplayEventEXT(device, display, pDisplayEventInfo, pAllocator, pFence);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue ) {
scoped_lock l(global_lock);
#ifdef GETSWAPCHAINCOUNTEREXT_EXISTS
layer_GetSwapchainCounterEXT(device, swapchain, counter, pCounterValue);
#endif 
device_dispatch[GetKey(device)].GetSwapchainCounterEXT(device, swapchain, counter, pCounterValue);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEGROUPPEERMEMORYFEATURES_EXISTS
layer_GetDeviceGroupPeerMemoryFeatures(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
#endif 
device_dispatch[GetKey(device)].GetDeviceGroupPeerMemoryFeatures(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindBufferMemory2(VkDevice device, uint32_t bindInfoCount, VkBindBufferMemoryInfo* pBindInfos ) {
scoped_lock l(global_lock);
#ifdef BINDBUFFERMEMORY2_EXISTS
layer_BindBufferMemory2(device, bindInfoCount, pBindInfos);
#endif 
device_dispatch[GetKey(device)].BindBufferMemory2(device, bindInfoCount, pBindInfos);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindImageMemory2(VkDevice device, uint32_t bindInfoCount, VkBindImageMemoryInfo* pBindInfos ) {
scoped_lock l(global_lock);
#ifdef BINDIMAGEMEMORY2_EXISTS
layer_BindImageMemory2(device, bindInfoCount, pBindInfos);
#endif 
device_dispatch[GetKey(device)].BindImageMemory2(device, bindInfoCount, pBindInfos);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEVICEMASK_EXISTS
layer_CmdSetDeviceMask(commandBuffer, deviceMask);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDeviceMask(commandBuffer, deviceMask);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEGROUPPRESENTCAPABILITIESKHR_EXISTS
layer_GetDeviceGroupPresentCapabilitiesKHR(device, pDeviceGroupPresentCapabilities);
#endif 
device_dispatch[GetKey(device)].GetDeviceGroupPresentCapabilitiesKHR(device, pDeviceGroupPresentCapabilities);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEGROUPSURFACEPRESENTMODESKHR_EXISTS
layer_GetDeviceGroupSurfacePresentModesKHR(device, surface, pModes);
#endif 
device_dispatch[GetKey(device)].GetDeviceGroupSurfacePresentModesKHR(device, surface, pModes);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquireNextImage2KHR(VkDevice device, VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex ) {
scoped_lock l(global_lock);
#ifdef ACQUIRENEXTIMAGE2KHR_EXISTS
layer_AcquireNextImage2KHR(device, pAcquireInfo, pImageIndex);
#endif 
device_dispatch[GetKey(device)].AcquireNextImage2KHR(device, pAcquireInfo, pImageIndex);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ ) {
scoped_lock l(global_lock);
#ifdef CMDDISPATCHBASE_EXISTS
layer_CmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate ) {
scoped_lock l(global_lock);
#ifdef CREATEDESCRIPTORUPDATETEMPLATE_EXISTS
layer_CreateDescriptorUpdateTemplate(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
#endif 
device_dispatch[GetKey(device)].CreateDescriptorUpdateTemplate(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYDESCRIPTORUPDATETEMPLATE_EXISTS
layer_DestroyDescriptorUpdateTemplate(device, descriptorUpdateTemplate, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyDescriptorUpdateTemplate(device, descriptorUpdateTemplate, pAllocator);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_UpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, void* pData ) {
scoped_lock l(global_lock);
#ifdef UPDATEDESCRIPTORSETWITHTEMPLATE_EXISTS
layer_UpdateDescriptorSetWithTemplate(device, descriptorSet, descriptorUpdateTemplate, pData);
#endif 
device_dispatch[GetKey(device)].UpdateDescriptorSetWithTemplate(device, descriptorSet, descriptorUpdateTemplate, pData);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, void* pData ) {
scoped_lock l(global_lock);
#ifdef CMDPUSHDESCRIPTORSETWITHTEMPLATEKHR_EXISTS
layer_CmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_SetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, VkSwapchainKHR* pSwapchains, VkHdrMetadataEXT* pMetadata ) {
scoped_lock l(global_lock);
#ifdef SETHDRMETADATAEXT_EXISTS
layer_SetHdrMetadataEXT(device, swapchainCount, pSwapchains, pMetadata);
#endif 
device_dispatch[GetKey(device)].SetHdrMetadataEXT(device, swapchainCount, pSwapchains, pMetadata);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain ) {
scoped_lock l(global_lock);
#ifdef GETSWAPCHAINSTATUSKHR_EXISTS
layer_GetSwapchainStatusKHR(device, swapchain);
#endif 
device_dispatch[GetKey(device)].GetSwapchainStatusKHR(device, swapchain);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties ) {
scoped_lock l(global_lock);
#ifdef GETREFRESHCYCLEDURATIONGOOGLE_EXISTS
layer_GetRefreshCycleDurationGOOGLE(device, swapchain, pDisplayTimingProperties);
#endif 
device_dispatch[GetKey(device)].GetRefreshCycleDurationGOOGLE(device, swapchain, pDisplayTimingProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings ) {
scoped_lock l(global_lock);
#ifdef GETPASTPRESENTATIONTIMINGGOOGLE_EXISTS
layer_GetPastPresentationTimingGOOGLE(device, swapchain, pPresentationTimingCount, pPresentationTimings);
#endif 
device_dispatch[GetKey(device)].GetPastPresentationTimingGOOGLE(device, swapchain, pPresentationTimingCount, pPresentationTimings);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewportWScalingNV* pViewportWScalings ) {
scoped_lock l(global_lock);
#ifdef CMDSETVIEWPORTWSCALINGNV_EXISTS
layer_CmdSetViewportWScalingNV(commandBuffer, firstViewport, viewportCount, pViewportWScalings);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetViewportWScalingNV(commandBuffer, firstViewport, viewportCount, pViewportWScalings);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, VkRect2D* pDiscardRectangles ) {
scoped_lock l(global_lock);
#ifdef CMDSETDISCARDRECTANGLEEXT_EXISTS
layer_CmdSetDiscardRectangleEXT(commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDiscardRectangleEXT(commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETDISCARDRECTANGLEENABLEEXT_EXISTS
layer_CmdSetDiscardRectangleEnableEXT(commandBuffer, discardRectangleEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDiscardRectangleEnableEXT(commandBuffer, discardRectangleEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode ) {
scoped_lock l(global_lock);
#ifdef CMDSETDISCARDRECTANGLEMODEEXT_EXISTS
layer_CmdSetDiscardRectangleModeEXT(commandBuffer, discardRectangleMode);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDiscardRectangleModeEXT(commandBuffer, discardRectangleMode);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, VkSampleLocationsInfoEXT* pSampleLocationsInfo ) {
scoped_lock l(global_lock);
#ifdef CMDSETSAMPLELOCATIONSEXT_EXISTS
layer_CmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetBufferMemoryRequirements2(VkDevice device, VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETBUFFERMEMORYREQUIREMENTS2_EXISTS
layer_GetBufferMemoryRequirements2(device, pInfo, pMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetBufferMemoryRequirements2(device, pInfo, pMemoryRequirements);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageMemoryRequirements2(VkDevice device, VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETIMAGEMEMORYREQUIREMENTS2_EXISTS
layer_GetImageMemoryRequirements2(device, pInfo, pMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetImageMemoryRequirements2(device, pInfo, pMemoryRequirements);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageSparseMemoryRequirements2(VkDevice device, VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETIMAGESPARSEMEMORYREQUIREMENTS2_EXISTS
layer_GetImageSparseMemoryRequirements2(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetImageSparseMemoryRequirements2(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceBufferMemoryRequirements(VkDevice device, VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEBUFFERMEMORYREQUIREMENTS_EXISTS
layer_GetDeviceBufferMemoryRequirements(device, pInfo, pMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetDeviceBufferMemoryRequirements(device, pInfo, pMemoryRequirements);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceImageMemoryRequirements(VkDevice device, VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEIMAGEMEMORYREQUIREMENTS_EXISTS
layer_GetDeviceImageMemoryRequirements(device, pInfo, pMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetDeviceImageMemoryRequirements(device, pInfo, pMemoryRequirements);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceImageSparseMemoryRequirements(VkDevice device, VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEIMAGESPARSEMEMORYREQUIREMENTS_EXISTS
layer_GetDeviceImageSparseMemoryRequirements(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetDeviceImageSparseMemoryRequirements(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateSamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversionCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion ) {
scoped_lock l(global_lock);
#ifdef CREATESAMPLERYCBCRCONVERSION_EXISTS
layer_CreateSamplerYcbcrConversion(device, pCreateInfo, pAllocator, pYcbcrConversion);
#endif 
device_dispatch[GetKey(device)].CreateSamplerYcbcrConversion(device, pCreateInfo, pAllocator, pYcbcrConversion);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYSAMPLERYCBCRCONVERSION_EXISTS
layer_DestroySamplerYcbcrConversion(device, ycbcrConversion, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroySamplerYcbcrConversion(device, ycbcrConversion, pAllocator);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceQueue2(VkDevice device, VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEQUEUE2_EXISTS
layer_GetDeviceQueue2(device, pQueueInfo, pQueue);
#endif 
device_dispatch[GetKey(device)].GetDeviceQueue2(device, pQueueInfo, pQueue);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateValidationCacheEXT(VkDevice device, VkValidationCacheCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache ) {
scoped_lock l(global_lock);
#ifdef CREATEVALIDATIONCACHEEXT_EXISTS
layer_CreateValidationCacheEXT(device, pCreateInfo, pAllocator, pValidationCache);
#endif 
device_dispatch[GetKey(device)].CreateValidationCacheEXT(device, pCreateInfo, pAllocator, pValidationCache);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYVALIDATIONCACHEEXT_EXISTS
layer_DestroyValidationCacheEXT(device, validationCache, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyValidationCacheEXT(device, validationCache, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData ) {
scoped_lock l(global_lock);
#ifdef GETVALIDATIONCACHEDATAEXT_EXISTS
layer_GetValidationCacheDataEXT(device, validationCache, pDataSize, pData);
#endif 
device_dispatch[GetKey(device)].GetValidationCacheDataEXT(device, validationCache, pDataSize, pData);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_MergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, VkValidationCacheEXT* pSrcCaches ) {
scoped_lock l(global_lock);
#ifdef MERGEVALIDATIONCACHESEXT_EXISTS
layer_MergeValidationCachesEXT(device, dstCache, srcCacheCount, pSrcCaches);
#endif 
device_dispatch[GetKey(device)].MergeValidationCachesEXT(device, dstCache, srcCacheCount, pSrcCaches);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDescriptorSetLayoutSupport(VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport ) {
scoped_lock l(global_lock);
#ifdef GETDESCRIPTORSETLAYOUTSUPPORT_EXISTS
layer_GetDescriptorSetLayoutSupport(device, pCreateInfo, pSupport);
#endif 
device_dispatch[GetKey(device)].GetDescriptorSetLayoutSupport(device, pCreateInfo, pSupport);
}

#if defined(VK_USE_PLATFORM_ANDROID_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage ) {
scoped_lock l(global_lock);
#ifdef GETSWAPCHAINGRALLOCUSAGEANDROID_EXISTS
layer_GetSwapchainGrallocUsageANDROID(device, format, imageUsage, grallocUsage);
#endif 
device_dispatch[GetKey(device)].GetSwapchainGrallocUsageANDROID(device, format, imageUsage, grallocUsage);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage ) {
scoped_lock l(global_lock);
#ifdef GETSWAPCHAINGRALLOCUSAGE2ANDROID_EXISTS
layer_GetSwapchainGrallocUsage2ANDROID(device, format, imageUsage, swapchainImageUsage, grallocConsumerUsage, grallocProducerUsage);
#endif 
device_dispatch[GetKey(device)].GetSwapchainGrallocUsage2ANDROID(device, format, imageUsage, swapchainImageUsage, grallocConsumerUsage, grallocProducerUsage);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence ) {
scoped_lock l(global_lock);
#ifdef ACQUIREIMAGEANDROID_EXISTS
layer_AcquireImageANDROID(device, image, nativeFenceFd, semaphore, fence);
#endif 
device_dispatch[GetKey(device)].AcquireImageANDROID(device, image, nativeFenceFd, semaphore, fence);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_QueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd ) {
scoped_lock l(global_lock);
#ifdef QUEUESIGNALRELEASEIMAGEANDROID_EXISTS
layer_QueueSignalReleaseImageANDROID(queue, waitSemaphoreCount, pWaitSemaphores, image, pNativeFenceFd);
#endif 
device_dispatch[GetKey(queue)].QueueSignalReleaseImageANDROID(queue, waitSemaphoreCount, pWaitSemaphores, image, pNativeFenceFd);
return VK_SUCCESS;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo ) {
scoped_lock l(global_lock);
#ifdef GETSHADERINFOAMD_EXISTS
layer_GetShaderInfoAMD(device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
#endif 
device_dispatch[GetKey(device)].GetShaderInfoAMD(device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_SetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable ) {
scoped_lock l(global_lock);
#ifdef SETLOCALDIMMINGAMD_EXISTS
layer_SetLocalDimmingAMD(device, swapChain, localDimmingEnable);
#endif 
device_dispatch[GetKey(device)].SetLocalDimmingAMD(device, swapChain, localDimmingEnable);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetCalibratedTimestampsKHR(VkDevice device, uint32_t timestampCount, VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation ) {
scoped_lock l(global_lock);
#ifdef GETCALIBRATEDTIMESTAMPSKHR_EXISTS
layer_GetCalibratedTimestampsKHR(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
#endif 
device_dispatch[GetKey(device)].GetCalibratedTimestampsKHR(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SetDebugUtilsObjectNameEXT(VkDevice device, VkDebugUtilsObjectNameInfoEXT* pNameInfo ) {
scoped_lock l(global_lock);
#ifdef SETDEBUGUTILSOBJECTNAMEEXT_EXISTS
layer_SetDebugUtilsObjectNameEXT(device, pNameInfo);
#endif 
device_dispatch[GetKey(device)].SetDebugUtilsObjectNameEXT(device, pNameInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SetDebugUtilsObjectTagEXT(VkDevice device, VkDebugUtilsObjectTagInfoEXT* pTagInfo ) {
scoped_lock l(global_lock);
#ifdef SETDEBUGUTILSOBJECTTAGEXT_EXISTS
layer_SetDebugUtilsObjectTagEXT(device, pTagInfo);
#endif 
device_dispatch[GetKey(device)].SetDebugUtilsObjectTagEXT(device, pTagInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_QueueBeginDebugUtilsLabelEXT(VkQueue queue, VkDebugUtilsLabelEXT* pLabelInfo ) {
scoped_lock l(global_lock);
#ifdef QUEUEBEGINDEBUGUTILSLABELEXT_EXISTS
layer_QueueBeginDebugUtilsLabelEXT(queue, pLabelInfo);
#endif 
device_dispatch[GetKey(queue)].QueueBeginDebugUtilsLabelEXT(queue, pLabelInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_QueueEndDebugUtilsLabelEXT(VkQueue queue ) {
scoped_lock l(global_lock);
#ifdef QUEUEENDDEBUGUTILSLABELEXT_EXISTS
layer_QueueEndDebugUtilsLabelEXT(queue);
#endif 
device_dispatch[GetKey(queue)].QueueEndDebugUtilsLabelEXT(queue);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_QueueInsertDebugUtilsLabelEXT(VkQueue queue, VkDebugUtilsLabelEXT* pLabelInfo ) {
scoped_lock l(global_lock);
#ifdef QUEUEINSERTDEBUGUTILSLABELEXT_EXISTS
layer_QueueInsertDebugUtilsLabelEXT(queue, pLabelInfo);
#endif 
device_dispatch[GetKey(queue)].QueueInsertDebugUtilsLabelEXT(queue, pLabelInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, VkDebugUtilsLabelEXT* pLabelInfo ) {
scoped_lock l(global_lock);
#ifdef CMDBEGINDEBUGUTILSLABELEXT_EXISTS
layer_CmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer ) {
scoped_lock l(global_lock);
#ifdef CMDENDDEBUGUTILSLABELEXT_EXISTS
layer_CmdEndDebugUtilsLabelEXT(commandBuffer);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndDebugUtilsLabelEXT(commandBuffer);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, VkDebugUtilsLabelEXT* pLabelInfo ) {
scoped_lock l(global_lock);
#ifdef CMDINSERTDEBUGUTILSLABELEXT_EXISTS
layer_CmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties ) {
scoped_lock l(global_lock);
#ifdef GETMEMORYHOSTPOINTERPROPERTIESEXT_EXISTS
layer_GetMemoryHostPointerPropertiesEXT(device, handleType, pHostPointer, pMemoryHostPointerProperties);
#endif 
device_dispatch[GetKey(device)].GetMemoryHostPointerPropertiesEXT(device, handleType, pHostPointer, pMemoryHostPointerProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker ) {
scoped_lock l(global_lock);
#ifdef CMDWRITEBUFFERMARKERAMD_EXISTS
layer_CmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateRenderPass2(VkDevice device, VkRenderPassCreateInfo2* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass ) {
scoped_lock l(global_lock);
#ifdef CREATERENDERPASS2_EXISTS
layer_CreateRenderPass2(device, pCreateInfo, pAllocator, pRenderPass);
#endif 
device_dispatch[GetKey(device)].CreateRenderPass2(device, pCreateInfo, pAllocator, pRenderPass);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassBeginInfo* pSubpassBeginInfo ) {
scoped_lock l(global_lock);
#ifdef CMDBEGINRENDERPASS2_EXISTS
layer_CmdBeginRenderPass2(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginRenderPass2(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdNextSubpass2(VkCommandBuffer commandBuffer, VkSubpassBeginInfo* pSubpassBeginInfo, VkSubpassEndInfo* pSubpassEndInfo ) {
scoped_lock l(global_lock);
#ifdef CMDNEXTSUBPASS2_EXISTS
layer_CmdNextSubpass2(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdNextSubpass2(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndRenderPass2(VkCommandBuffer commandBuffer, VkSubpassEndInfo* pSubpassEndInfo ) {
scoped_lock l(global_lock);
#ifdef CMDENDRENDERPASS2_EXISTS
layer_CmdEndRenderPass2(commandBuffer, pSubpassEndInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndRenderPass2(commandBuffer, pSubpassEndInfo);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue ) {
scoped_lock l(global_lock);
#ifdef GETSEMAPHORECOUNTERVALUE_EXISTS
layer_GetSemaphoreCounterValue(device, semaphore, pValue);
#endif 
device_dispatch[GetKey(device)].GetSemaphoreCounterValue(device, semaphore, pValue);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_WaitSemaphores(VkDevice device, VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout ) {
scoped_lock l(global_lock);
#ifdef WAITSEMAPHORES_EXISTS
layer_WaitSemaphores(device, pWaitInfo, timeout);
#endif 
device_dispatch[GetKey(device)].WaitSemaphores(device, pWaitInfo, timeout);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SignalSemaphore(VkDevice device, VkSemaphoreSignalInfo* pSignalInfo ) {
scoped_lock l(global_lock);
#ifdef SIGNALSEMAPHORE_EXISTS
layer_SignalSemaphore(device, pSignalInfo);
#endif 
device_dispatch[GetKey(device)].SignalSemaphore(device, pSignalInfo);
return VK_SUCCESS;
}

#if defined(VK_USE_PLATFORM_ANDROID_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetAndroidHardwareBufferPropertiesANDROID(VkDevice device, AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETANDROIDHARDWAREBUFFERPROPERTIESANDROID_EXISTS
layer_GetAndroidHardwareBufferPropertiesANDROID(device, buffer, pProperties);
#endif 
device_dispatch[GetKey(device)].GetAndroidHardwareBufferPropertiesANDROID(device, buffer, pProperties);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetMemoryAndroidHardwareBufferANDROID(VkDevice device, VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, AHardwareBuffer* pBuffer ) {
scoped_lock l(global_lock);
#ifdef GETMEMORYANDROIDHARDWAREBUFFERANDROID_EXISTS
layer_GetMemoryAndroidHardwareBufferANDROID(device, pInfo, pBuffer);
#endif 
device_dispatch[GetKey(device)].GetMemoryAndroidHardwareBufferANDROID(device, pInfo, pBuffer);
return VK_SUCCESS;
}

#endif
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWINDIRECTCOUNT_EXISTS
layer_CmdDrawIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWINDEXEDINDIRECTCOUNT_EXISTS
layer_CmdDrawIndexedIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndexedIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCheckpointNV(VkCommandBuffer commandBuffer, void* pCheckpointMarker ) {
scoped_lock l(global_lock);
#ifdef CMDSETCHECKPOINTNV_EXISTS
layer_CmdSetCheckpointNV(commandBuffer, pCheckpointMarker);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCheckpointNV(commandBuffer, pCheckpointMarker);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData ) {
scoped_lock l(global_lock);
#ifdef GETQUEUECHECKPOINTDATANV_EXISTS
layer_GetQueueCheckpointDataNV(queue, pCheckpointDataCount, pCheckpointData);
#endif 
device_dispatch[GetKey(queue)].GetQueueCheckpointDataNV(queue, pCheckpointDataCount, pCheckpointData);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets, VkDeviceSize* pSizes ) {
scoped_lock l(global_lock);
#ifdef CMDBINDTRANSFORMFEEDBACKBUFFERSEXT_EXISTS
layer_CmdBindTransformFeedbackBuffersEXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindTransformFeedbackBuffersEXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, VkBuffer* pCounterBuffers, VkDeviceSize* pCounterBufferOffsets ) {
scoped_lock l(global_lock);
#ifdef CMDBEGINTRANSFORMFEEDBACKEXT_EXISTS
layer_CmdBeginTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, VkBuffer* pCounterBuffers, VkDeviceSize* pCounterBufferOffsets ) {
scoped_lock l(global_lock);
#ifdef CMDENDTRANSFORMFEEDBACKEXT_EXISTS
layer_CmdEndTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index ) {
scoped_lock l(global_lock);
#ifdef CMDBEGINQUERYINDEXEDEXT_EXISTS
layer_CmdBeginQueryIndexedEXT(commandBuffer, queryPool, query, flags, index);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginQueryIndexedEXT(commandBuffer, queryPool, query, flags, index);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index ) {
scoped_lock l(global_lock);
#ifdef CMDENDQUERYINDEXEDEXT_EXISTS
layer_CmdEndQueryIndexedEXT(commandBuffer, queryPool, query, index);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndQueryIndexedEXT(commandBuffer, queryPool, query, index);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWINDIRECTBYTECOUNTEXT_EXISTS
layer_CmdDrawIndirectByteCountEXT(commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndirectByteCountEXT(commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, VkRect2D* pExclusiveScissors ) {
scoped_lock l(global_lock);
#ifdef CMDSETEXCLUSIVESCISSORNV_EXISTS
layer_CmdSetExclusiveScissorNV(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetExclusiveScissorNV(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, VkBool32* pExclusiveScissorEnables ) {
scoped_lock l(global_lock);
#ifdef CMDSETEXCLUSIVESCISSORENABLENV_EXISTS
layer_CmdSetExclusiveScissorEnableNV(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissorEnables);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetExclusiveScissorEnableNV(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissorEnables);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout ) {
scoped_lock l(global_lock);
#ifdef CMDBINDSHADINGRATEIMAGENV_EXISTS
layer_CmdBindShadingRateImageNV(commandBuffer, imageView, imageLayout);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindShadingRateImageNV(commandBuffer, imageView, imageLayout);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkShadingRatePaletteNV* pShadingRatePalettes ) {
scoped_lock l(global_lock);
#ifdef CMDSETVIEWPORTSHADINGRATEPALETTENV_EXISTS
layer_CmdSetViewportShadingRatePaletteNV(commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetViewportShadingRatePaletteNV(commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, VkCoarseSampleOrderCustomNV* pCustomSampleOrders ) {
scoped_lock l(global_lock);
#ifdef CMDSETCOARSESAMPLEORDERNV_EXISTS
layer_CmdSetCoarseSampleOrderNV(commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCoarseSampleOrderNV(commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWMESHTASKSNV_EXISTS
layer_CmdDrawMeshTasksNV(commandBuffer, taskCount, firstTask);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawMeshTasksNV(commandBuffer, taskCount, firstTask);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWMESHTASKSINDIRECTNV_EXISTS
layer_CmdDrawMeshTasksIndirectNV(commandBuffer, buffer, offset, drawCount, stride);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawMeshTasksIndirectNV(commandBuffer, buffer, offset, drawCount, stride);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWMESHTASKSINDIRECTCOUNTNV_EXISTS
layer_CmdDrawMeshTasksIndirectCountNV(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawMeshTasksIndirectCountNV(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWMESHTASKSEXT_EXISTS
layer_CmdDrawMeshTasksEXT(commandBuffer, groupCountX, groupCountY, groupCountZ);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawMeshTasksEXT(commandBuffer, groupCountX, groupCountY, groupCountZ);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWMESHTASKSINDIRECTEXT_EXISTS
layer_CmdDrawMeshTasksIndirectEXT(commandBuffer, buffer, offset, drawCount, stride);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawMeshTasksIndirectEXT(commandBuffer, buffer, offset, drawCount, stride);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWMESHTASKSINDIRECTCOUNTEXT_EXISTS
layer_CmdDrawMeshTasksIndirectCountEXT(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawMeshTasksIndirectCountEXT(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader ) {
scoped_lock l(global_lock);
#ifdef COMPILEDEFERREDNV_EXISTS
layer_CompileDeferredNV(device, pipeline, shader);
#endif 
device_dispatch[GetKey(device)].CompileDeferredNV(device, pipeline, shader);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateAccelerationStructureNV(VkDevice device, VkAccelerationStructureCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure ) {
scoped_lock l(global_lock);
#ifdef CREATEACCELERATIONSTRUCTURENV_EXISTS
layer_CreateAccelerationStructureNV(device, pCreateInfo, pAllocator, pAccelerationStructure);
#endif 
device_dispatch[GetKey(device)].CreateAccelerationStructureNV(device, pCreateInfo, pAllocator, pAccelerationStructure);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout ) {
scoped_lock l(global_lock);
#ifdef CMDBINDINVOCATIONMASKHUAWEI_EXISTS
layer_CmdBindInvocationMaskHUAWEI(commandBuffer, imageView, imageLayout);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindInvocationMaskHUAWEI(commandBuffer, imageView, imageLayout);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYACCELERATIONSTRUCTUREKHR_EXISTS
layer_DestroyAccelerationStructureKHR(device, accelerationStructure, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyAccelerationStructureKHR(device, accelerationStructure, pAllocator);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYACCELERATIONSTRUCTURENV_EXISTS
layer_DestroyAccelerationStructureNV(device, accelerationStructure, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyAccelerationStructureNV(device, accelerationStructure, pAllocator);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetAccelerationStructureMemoryRequirementsNV(VkDevice device, VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETACCELERATIONSTRUCTUREMEMORYREQUIREMENTSNV_EXISTS
layer_GetAccelerationStructureMemoryRequirementsNV(device, pInfo, pMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetAccelerationStructureMemoryRequirementsNV(device, pInfo, pMemoryRequirements);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, VkBindAccelerationStructureMemoryInfoNV* pBindInfos ) {
scoped_lock l(global_lock);
#ifdef BINDACCELERATIONSTRUCTUREMEMORYNV_EXISTS
layer_BindAccelerationStructureMemoryNV(device, bindInfoCount, pBindInfos);
#endif 
device_dispatch[GetKey(device)].BindAccelerationStructureMemoryNV(device, bindInfoCount, pBindInfos);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYACCELERATIONSTRUCTURENV_EXISTS
layer_CmdCopyAccelerationStructureNV(commandBuffer, dst, src, mode);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyAccelerationStructureNV(commandBuffer, dst, src, mode);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, VkCopyAccelerationStructureInfoKHR* pInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYACCELERATIONSTRUCTUREKHR_EXISTS
layer_CmdCopyAccelerationStructureKHR(commandBuffer, pInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyAccelerationStructureKHR(commandBuffer, pInfo);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyAccelerationStructureInfoKHR* pInfo ) {
scoped_lock l(global_lock);
#ifdef COPYACCELERATIONSTRUCTUREKHR_EXISTS
layer_CopyAccelerationStructureKHR(device, deferredOperation, pInfo);
#endif 
device_dispatch[GetKey(device)].CopyAccelerationStructureKHR(device, deferredOperation, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, VkCopyAccelerationStructureToMemoryInfoKHR* pInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYACCELERATIONSTRUCTURETOMEMORYKHR_EXISTS
layer_CmdCopyAccelerationStructureToMemoryKHR(commandBuffer, pInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyAccelerationStructureToMemoryKHR(commandBuffer, pInfo);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyAccelerationStructureToMemoryInfoKHR* pInfo ) {
scoped_lock l(global_lock);
#ifdef COPYACCELERATIONSTRUCTURETOMEMORYKHR_EXISTS
layer_CopyAccelerationStructureToMemoryKHR(device, deferredOperation, pInfo);
#endif 
device_dispatch[GetKey(device)].CopyAccelerationStructureToMemoryKHR(device, deferredOperation, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, VkCopyMemoryToAccelerationStructureInfoKHR* pInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYMEMORYTOACCELERATIONSTRUCTUREKHR_EXISTS
layer_CmdCopyMemoryToAccelerationStructureKHR(commandBuffer, pInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyMemoryToAccelerationStructureKHR(commandBuffer, pInfo);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyMemoryToAccelerationStructureInfoKHR* pInfo ) {
scoped_lock l(global_lock);
#ifdef COPYMEMORYTOACCELERATIONSTRUCTUREKHR_EXISTS
layer_CopyMemoryToAccelerationStructureKHR(device, deferredOperation, pInfo);
#endif 
device_dispatch[GetKey(device)].CopyMemoryToAccelerationStructureKHR(device, deferredOperation, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery ) {
scoped_lock l(global_lock);
#ifdef CMDWRITEACCELERATIONSTRUCTURESPROPERTIESKHR_EXISTS
layer_CmdWriteAccelerationStructuresPropertiesKHR(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWriteAccelerationStructuresPropertiesKHR(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery ) {
scoped_lock l(global_lock);
#ifdef CMDWRITEACCELERATIONSTRUCTURESPROPERTIESNV_EXISTS
layer_CmdWriteAccelerationStructuresPropertiesNV(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWriteAccelerationStructuresPropertiesNV(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset ) {
scoped_lock l(global_lock);
#ifdef CMDBUILDACCELERATIONSTRUCTURENV_EXISTS
layer_CmdBuildAccelerationStructureNV(commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBuildAccelerationStructureNV(commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_WriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, size_t dataSize, void* pData, size_t stride ) {
scoped_lock l(global_lock);
#ifdef WRITEACCELERATIONSTRUCTURESPROPERTIESKHR_EXISTS
layer_WriteAccelerationStructuresPropertiesKHR(device, accelerationStructureCount, pAccelerationStructures, queryType, dataSize, pData, stride);
#endif 
device_dispatch[GetKey(device)].WriteAccelerationStructuresPropertiesKHR(device, accelerationStructureCount, pAccelerationStructures, queryType, dataSize, pData, stride);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdTraceRaysKHR(VkCommandBuffer commandBuffer, VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth ) {
scoped_lock l(global_lock);
#ifdef CMDTRACERAYSKHR_EXISTS
layer_CmdTraceRaysKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdTraceRaysKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth ) {
scoped_lock l(global_lock);
#ifdef CMDTRACERAYSNV_EXISTS
layer_CmdTraceRaysNV(commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdTraceRaysNV(commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData ) {
scoped_lock l(global_lock);
#ifdef GETRAYTRACINGSHADERGROUPHANDLESKHR_EXISTS
layer_GetRayTracingShaderGroupHandlesKHR(device, pipeline, firstGroup, groupCount, dataSize, pData);
#endif 
device_dispatch[GetKey(device)].GetRayTracingShaderGroupHandlesKHR(device, pipeline, firstGroup, groupCount, dataSize, pData);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData ) {
scoped_lock l(global_lock);
#ifdef GETRAYTRACINGCAPTUREREPLAYSHADERGROUPHANDLESKHR_EXISTS
layer_GetRayTracingCaptureReplayShaderGroupHandlesKHR(device, pipeline, firstGroup, groupCount, dataSize, pData);
#endif 
device_dispatch[GetKey(device)].GetRayTracingCaptureReplayShaderGroupHandlesKHR(device, pipeline, firstGroup, groupCount, dataSize, pData);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData ) {
scoped_lock l(global_lock);
#ifdef GETACCELERATIONSTRUCTUREHANDLENV_EXISTS
layer_GetAccelerationStructureHandleNV(device, accelerationStructure, dataSize, pData);
#endif 
device_dispatch[GetKey(device)].GetAccelerationStructureHandleNV(device, accelerationStructure, dataSize, pData);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkRayTracingPipelineCreateInfoNV* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines ) {
scoped_lock l(global_lock);
#ifdef CREATERAYTRACINGPIPELINESNV_EXISTS
layer_CreateRayTracingPipelinesNV(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
#endif 
device_dispatch[GetKey(device)].CreateRayTracingPipelinesNV(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkRayTracingPipelineCreateInfoKHR* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines ) {
scoped_lock l(global_lock);
#ifdef CREATERAYTRACINGPIPELINESKHR_EXISTS
layer_CreateRayTracingPipelinesKHR(device, deferredOperation, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
#endif 
device_dispatch[GetKey(device)].CreateRayTracingPipelinesKHR(device, deferredOperation, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress ) {
scoped_lock l(global_lock);
#ifdef CMDTRACERAYSINDIRECTKHR_EXISTS
layer_CmdTraceRaysIndirectKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdTraceRaysIndirectKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress ) {
scoped_lock l(global_lock);
#ifdef CMDTRACERAYSINDIRECT2KHR_EXISTS
layer_CmdTraceRaysIndirect2KHR(commandBuffer, indirectDeviceAddress);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdTraceRaysIndirect2KHR(commandBuffer, indirectDeviceAddress);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEACCELERATIONSTRUCTURECOMPATIBILITYKHR_EXISTS
layer_GetDeviceAccelerationStructureCompatibilityKHR(device, pVersionInfo, pCompatibility);
#endif 
device_dispatch[GetKey(device)].GetDeviceAccelerationStructureCompatibilityKHR(device, pVersionInfo, pCompatibility);
}

 VK_LAYER_EXPORT VkDeviceSize VKAPI_CALL DetailsLayer_GetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader ) {
scoped_lock l(global_lock);
#ifdef GETRAYTRACINGSHADERGROUPSTACKSIZEKHR_EXISTS
layer_GetRayTracingShaderGroupStackSizeKHR(device, pipeline, group, groupShader);
#endif 
device_dispatch[GetKey(device)].GetRayTracingShaderGroupStackSizeKHR(device, pipeline, group, groupShader);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize ) {
scoped_lock l(global_lock);
#ifdef CMDSETRAYTRACINGPIPELINESTACKSIZEKHR_EXISTS
layer_CmdSetRayTracingPipelineStackSizeKHR(commandBuffer, pipelineStackSize);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetRayTracingPipelineStackSizeKHR(commandBuffer, pipelineStackSize);
}

 VK_LAYER_EXPORT uint32_t VKAPI_CALL DetailsLayer_GetImageViewHandleNVX(VkDevice device, VkImageViewHandleInfoNVX* pInfo ) {
scoped_lock l(global_lock);
#ifdef GETIMAGEVIEWHANDLENVX_EXISTS
layer_GetImageViewHandleNVX(device, pInfo);
#endif 
device_dispatch[GetKey(device)].GetImageViewHandleNVX(device, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETIMAGEVIEWADDRESSNVX_EXISTS
layer_GetImageViewAddressNVX(device, imageView, pProperties);
#endif 
device_dispatch[GetKey(device)].GetImageViewAddressNVX(device, imageView, pProperties);
return VK_SUCCESS;
}

#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDeviceGroupSurfacePresentModes2EXT(VkDevice device, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEGROUPSURFACEPRESENTMODES2EXT_EXISTS
layer_GetDeviceGroupSurfacePresentModes2EXT(device, pSurfaceInfo, pModes);
#endif 
device_dispatch[GetKey(device)].GetDeviceGroupSurfacePresentModes2EXT(device, pSurfaceInfo, pModes);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain ) {
scoped_lock l(global_lock);
#ifdef ACQUIREFULLSCREENEXCLUSIVEMODEEXT_EXISTS
layer_AcquireFullScreenExclusiveModeEXT(device, swapchain);
#endif 
device_dispatch[GetKey(device)].AcquireFullScreenExclusiveModeEXT(device, swapchain);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain ) {
scoped_lock l(global_lock);
#ifdef RELEASEFULLSCREENEXCLUSIVEMODEEXT_EXISTS
layer_ReleaseFullScreenExclusiveModeEXT(device, swapchain);
#endif 
device_dispatch[GetKey(device)].ReleaseFullScreenExclusiveModeEXT(device, swapchain);
return VK_SUCCESS;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquireProfilingLockKHR(VkDevice device, VkAcquireProfilingLockInfoKHR* pInfo ) {
scoped_lock l(global_lock);
#ifdef ACQUIREPROFILINGLOCKKHR_EXISTS
layer_AcquireProfilingLockKHR(device, pInfo);
#endif 
device_dispatch[GetKey(device)].AcquireProfilingLockKHR(device, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_ReleaseProfilingLockKHR(VkDevice device ) {
scoped_lock l(global_lock);
#ifdef RELEASEPROFILINGLOCKKHR_EXISTS
layer_ReleaseProfilingLockKHR(device);
#endif 
device_dispatch[GetKey(device)].ReleaseProfilingLockKHR(device);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETIMAGEDRMFORMATMODIFIERPROPERTIESEXT_EXISTS
layer_GetImageDrmFormatModifierPropertiesEXT(device, image, pProperties);
#endif 
device_dispatch[GetKey(device)].GetImageDrmFormatModifierPropertiesEXT(device, image, pProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT uint64_t VKAPI_CALL DetailsLayer_GetBufferOpaqueCaptureAddress(VkDevice device, VkBufferDeviceAddressInfo* pInfo ) {
scoped_lock l(global_lock);
#ifdef GETBUFFEROPAQUECAPTUREADDRESS_EXISTS
layer_GetBufferOpaqueCaptureAddress(device, pInfo);
#endif 
device_dispatch[GetKey(device)].GetBufferOpaqueCaptureAddress(device, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkDeviceAddress VKAPI_CALL DetailsLayer_GetBufferDeviceAddress(VkDevice device, VkBufferDeviceAddressInfo* pInfo ) {
scoped_lock l(global_lock);
#ifdef GETBUFFERDEVICEADDRESS_EXISTS
layer_GetBufferDeviceAddress(device, pInfo);
#endif 
device_dispatch[GetKey(device)].GetBufferDeviceAddress(device, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_InitializePerformanceApiINTEL(VkDevice device, VkInitializePerformanceApiInfoINTEL* pInitializeInfo ) {
scoped_lock l(global_lock);
#ifdef INITIALIZEPERFORMANCEAPIINTEL_EXISTS
layer_InitializePerformanceApiINTEL(device, pInitializeInfo);
#endif 
device_dispatch[GetKey(device)].InitializePerformanceApiINTEL(device, pInitializeInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_UninitializePerformanceApiINTEL(VkDevice device ) {
scoped_lock l(global_lock);
#ifdef UNINITIALIZEPERFORMANCEAPIINTEL_EXISTS
layer_UninitializePerformanceApiINTEL(device);
#endif 
device_dispatch[GetKey(device)].UninitializePerformanceApiINTEL(device);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, VkPerformanceMarkerInfoINTEL* pMarkerInfo ) {
scoped_lock l(global_lock);
#ifdef CMDSETPERFORMANCEMARKERINTEL_EXISTS
layer_CmdSetPerformanceMarkerINTEL(commandBuffer, pMarkerInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetPerformanceMarkerINTEL(commandBuffer, pMarkerInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo ) {
scoped_lock l(global_lock);
#ifdef CMDSETPERFORMANCESTREAMMARKERINTEL_EXISTS
layer_CmdSetPerformanceStreamMarkerINTEL(commandBuffer, pMarkerInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetPerformanceStreamMarkerINTEL(commandBuffer, pMarkerInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, VkPerformanceOverrideInfoINTEL* pOverrideInfo ) {
scoped_lock l(global_lock);
#ifdef CMDSETPERFORMANCEOVERRIDEINTEL_EXISTS
layer_CmdSetPerformanceOverrideINTEL(commandBuffer, pOverrideInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetPerformanceOverrideINTEL(commandBuffer, pOverrideInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_AcquirePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration ) {
scoped_lock l(global_lock);
#ifdef ACQUIREPERFORMANCECONFIGURATIONINTEL_EXISTS
layer_AcquirePerformanceConfigurationINTEL(device, pAcquireInfo, pConfiguration);
#endif 
device_dispatch[GetKey(device)].AcquirePerformanceConfigurationINTEL(device, pAcquireInfo, pConfiguration);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration ) {
scoped_lock l(global_lock);
#ifdef RELEASEPERFORMANCECONFIGURATIONINTEL_EXISTS
layer_ReleasePerformanceConfigurationINTEL(device, configuration);
#endif 
device_dispatch[GetKey(device)].ReleasePerformanceConfigurationINTEL(device, configuration);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_QueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration ) {
scoped_lock l(global_lock);
#ifdef QUEUESETPERFORMANCECONFIGURATIONINTEL_EXISTS
layer_QueueSetPerformanceConfigurationINTEL(queue, configuration);
#endif 
device_dispatch[GetKey(queue)].QueueSetPerformanceConfigurationINTEL(queue, configuration);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue ) {
scoped_lock l(global_lock);
#ifdef GETPERFORMANCEPARAMETERINTEL_EXISTS
layer_GetPerformanceParameterINTEL(device, parameter, pValue);
#endif 
device_dispatch[GetKey(device)].GetPerformanceParameterINTEL(device, parameter, pValue);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT uint64_t VKAPI_CALL DetailsLayer_GetDeviceMemoryOpaqueCaptureAddress(VkDevice device, VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEMEMORYOPAQUECAPTUREADDRESS_EXISTS
layer_GetDeviceMemoryOpaqueCaptureAddress(device, pInfo);
#endif 
device_dispatch[GetKey(device)].GetDeviceMemoryOpaqueCaptureAddress(device, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPipelineExecutablePropertiesKHR(VkDevice device, VkPipelineInfoKHR* pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETPIPELINEEXECUTABLEPROPERTIESKHR_EXISTS
layer_GetPipelineExecutablePropertiesKHR(device, pPipelineInfo, pExecutableCount, pProperties);
#endif 
device_dispatch[GetKey(device)].GetPipelineExecutablePropertiesKHR(device, pPipelineInfo, pExecutableCount, pProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPipelineExecutableStatisticsKHR(VkDevice device, VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics ) {
scoped_lock l(global_lock);
#ifdef GETPIPELINEEXECUTABLESTATISTICSKHR_EXISTS
layer_GetPipelineExecutableStatisticsKHR(device, pExecutableInfo, pStatisticCount, pStatistics);
#endif 
device_dispatch[GetKey(device)].GetPipelineExecutableStatisticsKHR(device, pExecutableInfo, pStatisticCount, pStatistics);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPipelineExecutableInternalRepresentationsKHR(VkDevice device, VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations ) {
scoped_lock l(global_lock);
#ifdef GETPIPELINEEXECUTABLEINTERNALREPRESENTATIONSKHR_EXISTS
layer_GetPipelineExecutableInternalRepresentationsKHR(device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
#endif 
device_dispatch[GetKey(device)].GetPipelineExecutableInternalRepresentationsKHR(device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateAccelerationStructureKHR(VkDevice device, VkAccelerationStructureCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkAccelerationStructureKHR* pAccelerationStructure ) {
scoped_lock l(global_lock);
#ifdef CREATEACCELERATIONSTRUCTUREKHR_EXISTS
layer_CreateAccelerationStructureKHR(device, pCreateInfo, pAllocator, pAccelerationStructure);
#endif 
device_dispatch[GetKey(device)].CreateAccelerationStructureKHR(device, pCreateInfo, pAllocator, pAccelerationStructure);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBuildAccelerationStructuresKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, VkAccelerationStructureBuildGeometryInfoKHR* pInfos, VkAccelerationStructureBuildRangeInfoKHR** ppBuildRangeInfos ) {
scoped_lock l(global_lock);
#ifdef CMDBUILDACCELERATIONSTRUCTURESKHR_EXISTS
layer_CmdBuildAccelerationStructuresKHR(commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBuildAccelerationStructuresKHR(commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, VkAccelerationStructureBuildGeometryInfoKHR* pInfos, VkDeviceAddress* pIndirectDeviceAddresses, uint32_t* pIndirectStrides, uint32_t** ppMaxPrimitiveCounts ) {
scoped_lock l(global_lock);
#ifdef CMDBUILDACCELERATIONSTRUCTURESINDIRECTKHR_EXISTS
layer_CmdBuildAccelerationStructuresIndirectKHR(commandBuffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBuildAccelerationStructuresIndirectKHR(commandBuffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BuildAccelerationStructuresKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, VkAccelerationStructureBuildGeometryInfoKHR* pInfos, VkAccelerationStructureBuildRangeInfoKHR** ppBuildRangeInfos ) {
scoped_lock l(global_lock);
#ifdef BUILDACCELERATIONSTRUCTURESKHR_EXISTS
layer_BuildAccelerationStructuresKHR(device, deferredOperation, infoCount, pInfos, ppBuildRangeInfos);
#endif 
device_dispatch[GetKey(device)].BuildAccelerationStructuresKHR(device, deferredOperation, infoCount, pInfos, ppBuildRangeInfos);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkDeviceAddress VKAPI_CALL DetailsLayer_GetAccelerationStructureDeviceAddressKHR(VkDevice device, VkAccelerationStructureDeviceAddressInfoKHR* pInfo ) {
scoped_lock l(global_lock);
#ifdef GETACCELERATIONSTRUCTUREDEVICEADDRESSKHR_EXISTS
layer_GetAccelerationStructureDeviceAddressKHR(device, pInfo);
#endif 
device_dispatch[GetKey(device)].GetAccelerationStructureDeviceAddressKHR(device, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDeferredOperationKHR(VkDevice device, VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation ) {
scoped_lock l(global_lock);
#ifdef CREATEDEFERREDOPERATIONKHR_EXISTS
layer_CreateDeferredOperationKHR(device, pAllocator, pDeferredOperation);
#endif 
device_dispatch[GetKey(device)].CreateDeferredOperationKHR(device, pAllocator, pDeferredOperation);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYDEFERREDOPERATIONKHR_EXISTS
layer_DestroyDeferredOperationKHR(device, operation, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyDeferredOperationKHR(device, operation, pAllocator);
}

 VK_LAYER_EXPORT uint32_t VKAPI_CALL DetailsLayer_GetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation ) {
scoped_lock l(global_lock);
#ifdef GETDEFERREDOPERATIONMAXCONCURRENCYKHR_EXISTS
layer_GetDeferredOperationMaxConcurrencyKHR(device, operation);
#endif 
device_dispatch[GetKey(device)].GetDeferredOperationMaxConcurrencyKHR(device, operation);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation ) {
scoped_lock l(global_lock);
#ifdef GETDEFERREDOPERATIONRESULTKHR_EXISTS
layer_GetDeferredOperationResultKHR(device, operation);
#endif 
device_dispatch[GetKey(device)].GetDeferredOperationResultKHR(device, operation);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_DeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation ) {
scoped_lock l(global_lock);
#ifdef DEFERREDOPERATIONJOINKHR_EXISTS
layer_DeferredOperationJoinKHR(device, operation);
#endif 
device_dispatch[GetKey(device)].DeferredOperationJoinKHR(device, operation);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPipelineIndirectMemoryRequirementsNV(VkDevice device, VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETPIPELINEINDIRECTMEMORYREQUIREMENTSNV_EXISTS
layer_GetPipelineIndirectMemoryRequirementsNV(device, pCreateInfo, pMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetPipelineIndirectMemoryRequirementsNV(device, pCreateInfo, pMemoryRequirements);
}

 VK_LAYER_EXPORT VkDeviceAddress VKAPI_CALL DetailsLayer_GetPipelineIndirectDeviceAddressNV(VkDevice device, VkPipelineIndirectDeviceAddressInfoNV* pInfo ) {
scoped_lock l(global_lock);
#ifdef GETPIPELINEINDIRECTDEVICEADDRESSNV_EXISTS
layer_GetPipelineIndirectDeviceAddressNV(device, pInfo);
#endif 
device_dispatch[GetKey(device)].GetPipelineIndirectDeviceAddressNV(device, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode ) {
scoped_lock l(global_lock);
#ifdef CMDSETCULLMODE_EXISTS
layer_CmdSetCullMode(commandBuffer, cullMode);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCullMode(commandBuffer, cullMode);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace ) {
scoped_lock l(global_lock);
#ifdef CMDSETFRONTFACE_EXISTS
layer_CmdSetFrontFace(commandBuffer, frontFace);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetFrontFace(commandBuffer, frontFace);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology ) {
scoped_lock l(global_lock);
#ifdef CMDSETPRIMITIVETOPOLOGY_EXISTS
layer_CmdSetPrimitiveTopology(commandBuffer, primitiveTopology);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetPrimitiveTopology(commandBuffer, primitiveTopology);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, VkViewport* pViewports ) {
scoped_lock l(global_lock);
#ifdef CMDSETVIEWPORTWITHCOUNT_EXISTS
layer_CmdSetViewportWithCount(commandBuffer, viewportCount, pViewports);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetViewportWithCount(commandBuffer, viewportCount, pViewports);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, VkRect2D* pScissors ) {
scoped_lock l(global_lock);
#ifdef CMDSETSCISSORWITHCOUNT_EXISTS
layer_CmdSetScissorWithCount(commandBuffer, scissorCount, pScissors);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetScissorWithCount(commandBuffer, scissorCount, pScissors);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindIndexBuffer2KHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType ) {
scoped_lock l(global_lock);
#ifdef CMDBINDINDEXBUFFER2KHR_EXISTS
layer_CmdBindIndexBuffer2KHR(commandBuffer, buffer, offset, size, indexType);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindIndexBuffer2KHR(commandBuffer, buffer, offset, size, indexType);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets, VkDeviceSize* pSizes, VkDeviceSize* pStrides ) {
scoped_lock l(global_lock);
#ifdef CMDBINDVERTEXBUFFERS2_EXISTS
layer_CmdBindVertexBuffers2(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindVertexBuffers2(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEPTHTESTENABLE_EXISTS
layer_CmdSetDepthTestEnable(commandBuffer, depthTestEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthTestEnable(commandBuffer, depthTestEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEPTHWRITEENABLE_EXISTS
layer_CmdSetDepthWriteEnable(commandBuffer, depthWriteEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthWriteEnable(commandBuffer, depthWriteEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEPTHCOMPAREOP_EXISTS
layer_CmdSetDepthCompareOp(commandBuffer, depthCompareOp);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthCompareOp(commandBuffer, depthCompareOp);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEPTHBOUNDSTESTENABLE_EXISTS
layer_CmdSetDepthBoundsTestEnable(commandBuffer, depthBoundsTestEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthBoundsTestEnable(commandBuffer, depthBoundsTestEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETSTENCILTESTENABLE_EXISTS
layer_CmdSetStencilTestEnable(commandBuffer, stencilTestEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetStencilTestEnable(commandBuffer, stencilTestEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp ) {
scoped_lock l(global_lock);
#ifdef CMDSETSTENCILOP_EXISTS
layer_CmdSetStencilOp(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetStencilOp(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints ) {
scoped_lock l(global_lock);
#ifdef CMDSETPATCHCONTROLPOINTSEXT_EXISTS
layer_CmdSetPatchControlPointsEXT(commandBuffer, patchControlPoints);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetPatchControlPointsEXT(commandBuffer, patchControlPoints);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETRASTERIZERDISCARDENABLE_EXISTS
layer_CmdSetRasterizerDiscardEnable(commandBuffer, rasterizerDiscardEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetRasterizerDiscardEnable(commandBuffer, rasterizerDiscardEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEPTHBIASENABLE_EXISTS
layer_CmdSetDepthBiasEnable(commandBuffer, depthBiasEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthBiasEnable(commandBuffer, depthBiasEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp ) {
scoped_lock l(global_lock);
#ifdef CMDSETLOGICOPEXT_EXISTS
layer_CmdSetLogicOpEXT(commandBuffer, logicOp);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetLogicOpEXT(commandBuffer, logicOp);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETPRIMITIVERESTARTENABLE_EXISTS
layer_CmdSetPrimitiveRestartEnable(commandBuffer, primitiveRestartEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetPrimitiveRestartEnable(commandBuffer, primitiveRestartEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin ) {
scoped_lock l(global_lock);
#ifdef CMDSETTESSELLATIONDOMAINORIGINEXT_EXISTS
layer_CmdSetTessellationDomainOriginEXT(commandBuffer, domainOrigin);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetTessellationDomainOriginEXT(commandBuffer, domainOrigin);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEPTHCLAMPENABLEEXT_EXISTS
layer_CmdSetDepthClampEnableEXT(commandBuffer, depthClampEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthClampEnableEXT(commandBuffer, depthClampEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode ) {
scoped_lock l(global_lock);
#ifdef CMDSETPOLYGONMODEEXT_EXISTS
layer_CmdSetPolygonModeEXT(commandBuffer, polygonMode);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetPolygonModeEXT(commandBuffer, polygonMode);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits rasterizationSamples ) {
scoped_lock l(global_lock);
#ifdef CMDSETRASTERIZATIONSAMPLESEXT_EXISTS
layer_CmdSetRasterizationSamplesEXT(commandBuffer, rasterizationSamples);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetRasterizationSamplesEXT(commandBuffer, rasterizationSamples);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits samples, VkSampleMask* pSampleMask ) {
scoped_lock l(global_lock);
#ifdef CMDSETSAMPLEMASKEXT_EXISTS
layer_CmdSetSampleMaskEXT(commandBuffer, samples, pSampleMask);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetSampleMaskEXT(commandBuffer, samples, pSampleMask);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETALPHATOCOVERAGEENABLEEXT_EXISTS
layer_CmdSetAlphaToCoverageEnableEXT(commandBuffer, alphaToCoverageEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetAlphaToCoverageEnableEXT(commandBuffer, alphaToCoverageEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETALPHATOONEENABLEEXT_EXISTS
layer_CmdSetAlphaToOneEnableEXT(commandBuffer, alphaToOneEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetAlphaToOneEnableEXT(commandBuffer, alphaToOneEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETLOGICOPENABLEEXT_EXISTS
layer_CmdSetLogicOpEnableEXT(commandBuffer, logicOpEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetLogicOpEnableEXT(commandBuffer, logicOpEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, VkBool32* pColorBlendEnables ) {
scoped_lock l(global_lock);
#ifdef CMDSETCOLORBLENDENABLEEXT_EXISTS
layer_CmdSetColorBlendEnableEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendEnables);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetColorBlendEnableEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendEnables);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, VkColorBlendEquationEXT* pColorBlendEquations ) {
scoped_lock l(global_lock);
#ifdef CMDSETCOLORBLENDEQUATIONEXT_EXISTS
layer_CmdSetColorBlendEquationEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendEquations);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetColorBlendEquationEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendEquations);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, VkColorComponentFlags* pColorWriteMasks ) {
scoped_lock l(global_lock);
#ifdef CMDSETCOLORWRITEMASKEXT_EXISTS
layer_CmdSetColorWriteMaskEXT(commandBuffer, firstAttachment, attachmentCount, pColorWriteMasks);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetColorWriteMaskEXT(commandBuffer, firstAttachment, attachmentCount, pColorWriteMasks);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream ) {
scoped_lock l(global_lock);
#ifdef CMDSETRASTERIZATIONSTREAMEXT_EXISTS
layer_CmdSetRasterizationStreamEXT(commandBuffer, rasterizationStream);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetRasterizationStreamEXT(commandBuffer, rasterizationStream);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode ) {
scoped_lock l(global_lock);
#ifdef CMDSETCONSERVATIVERASTERIZATIONMODEEXT_EXISTS
layer_CmdSetConservativeRasterizationModeEXT(commandBuffer, conservativeRasterizationMode);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetConservativeRasterizationModeEXT(commandBuffer, conservativeRasterizationMode);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize ) {
scoped_lock l(global_lock);
#ifdef CMDSETEXTRAPRIMITIVEOVERESTIMATIONSIZEEXT_EXISTS
layer_CmdSetExtraPrimitiveOverestimationSizeEXT(commandBuffer, extraPrimitiveOverestimationSize);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetExtraPrimitiveOverestimationSizeEXT(commandBuffer, extraPrimitiveOverestimationSize);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEPTHCLIPENABLEEXT_EXISTS
layer_CmdSetDepthClipEnableEXT(commandBuffer, depthClipEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthClipEnableEXT(commandBuffer, depthClipEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETSAMPLELOCATIONSENABLEEXT_EXISTS
layer_CmdSetSampleLocationsEnableEXT(commandBuffer, sampleLocationsEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetSampleLocationsEnableEXT(commandBuffer, sampleLocationsEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, VkColorBlendAdvancedEXT* pColorBlendAdvanced ) {
scoped_lock l(global_lock);
#ifdef CMDSETCOLORBLENDADVANCEDEXT_EXISTS
layer_CmdSetColorBlendAdvancedEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendAdvanced);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetColorBlendAdvancedEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendAdvanced);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode ) {
scoped_lock l(global_lock);
#ifdef CMDSETPROVOKINGVERTEXMODEEXT_EXISTS
layer_CmdSetProvokingVertexModeEXT(commandBuffer, provokingVertexMode);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetProvokingVertexModeEXT(commandBuffer, provokingVertexMode);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode ) {
scoped_lock l(global_lock);
#ifdef CMDSETLINERASTERIZATIONMODEEXT_EXISTS
layer_CmdSetLineRasterizationModeEXT(commandBuffer, lineRasterizationMode);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetLineRasterizationModeEXT(commandBuffer, lineRasterizationMode);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETLINESTIPPLEENABLEEXT_EXISTS
layer_CmdSetLineStippleEnableEXT(commandBuffer, stippledLineEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetLineStippleEnableEXT(commandBuffer, stippledLineEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEPTHCLIPNEGATIVEONETOONEEXT_EXISTS
layer_CmdSetDepthClipNegativeOneToOneEXT(commandBuffer, negativeOneToOne);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthClipNegativeOneToOneEXT(commandBuffer, negativeOneToOne);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETVIEWPORTWSCALINGENABLENV_EXISTS
layer_CmdSetViewportWScalingEnableNV(commandBuffer, viewportWScalingEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetViewportWScalingEnableNV(commandBuffer, viewportWScalingEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewportSwizzleNV* pViewportSwizzles ) {
scoped_lock l(global_lock);
#ifdef CMDSETVIEWPORTSWIZZLENV_EXISTS
layer_CmdSetViewportSwizzleNV(commandBuffer, firstViewport, viewportCount, pViewportSwizzles);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetViewportSwizzleNV(commandBuffer, firstViewport, viewportCount, pViewportSwizzles);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETCOVERAGETOCOLORENABLENV_EXISTS
layer_CmdSetCoverageToColorEnableNV(commandBuffer, coverageToColorEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCoverageToColorEnableNV(commandBuffer, coverageToColorEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation ) {
scoped_lock l(global_lock);
#ifdef CMDSETCOVERAGETOCOLORLOCATIONNV_EXISTS
layer_CmdSetCoverageToColorLocationNV(commandBuffer, coverageToColorLocation);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCoverageToColorLocationNV(commandBuffer, coverageToColorLocation);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode ) {
scoped_lock l(global_lock);
#ifdef CMDSETCOVERAGEMODULATIONMODENV_EXISTS
layer_CmdSetCoverageModulationModeNV(commandBuffer, coverageModulationMode);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCoverageModulationModeNV(commandBuffer, coverageModulationMode);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETCOVERAGEMODULATIONTABLEENABLENV_EXISTS
layer_CmdSetCoverageModulationTableEnableNV(commandBuffer, coverageModulationTableEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCoverageModulationTableEnableNV(commandBuffer, coverageModulationTableEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, float* pCoverageModulationTable ) {
scoped_lock l(global_lock);
#ifdef CMDSETCOVERAGEMODULATIONTABLENV_EXISTS
layer_CmdSetCoverageModulationTableNV(commandBuffer, coverageModulationTableCount, pCoverageModulationTable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCoverageModulationTableNV(commandBuffer, coverageModulationTableCount, pCoverageModulationTable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETSHADINGRATEIMAGEENABLENV_EXISTS
layer_CmdSetShadingRateImageEnableNV(commandBuffer, shadingRateImageEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetShadingRateImageEnableNV(commandBuffer, shadingRateImageEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode ) {
scoped_lock l(global_lock);
#ifdef CMDSETCOVERAGEREDUCTIONMODENV_EXISTS
layer_CmdSetCoverageReductionModeNV(commandBuffer, coverageReductionMode);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCoverageReductionModeNV(commandBuffer, coverageReductionMode);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETREPRESENTATIVEFRAGMENTTESTENABLENV_EXISTS
layer_CmdSetRepresentativeFragmentTestEnableNV(commandBuffer, representativeFragmentTestEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetRepresentativeFragmentTestEnableNV(commandBuffer, representativeFragmentTestEnable);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreatePrivateDataSlot(VkDevice device, VkPrivateDataSlotCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot ) {
scoped_lock l(global_lock);
#ifdef CREATEPRIVATEDATASLOT_EXISTS
layer_CreatePrivateDataSlot(device, pCreateInfo, pAllocator, pPrivateDataSlot);
#endif 
device_dispatch[GetKey(device)].CreatePrivateDataSlot(device, pCreateInfo, pAllocator, pPrivateDataSlot);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYPRIVATEDATASLOT_EXISTS
layer_DestroyPrivateDataSlot(device, privateDataSlot, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyPrivateDataSlot(device, privateDataSlot, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data ) {
scoped_lock l(global_lock);
#ifdef SETPRIVATEDATA_EXISTS
layer_SetPrivateData(device, objectType, objectHandle, privateDataSlot, data);
#endif 
device_dispatch[GetKey(device)].SetPrivateData(device, objectType, objectHandle, privateDataSlot, data);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData ) {
scoped_lock l(global_lock);
#ifdef GETPRIVATEDATA_EXISTS
layer_GetPrivateData(device, objectType, objectHandle, privateDataSlot, pData);
#endif 
device_dispatch[GetKey(device)].GetPrivateData(device, objectType, objectHandle, privateDataSlot, pData);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyBuffer2(VkCommandBuffer commandBuffer, VkCopyBufferInfo2* pCopyBufferInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYBUFFER2_EXISTS
layer_CmdCopyBuffer2(commandBuffer, pCopyBufferInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyBuffer2(commandBuffer, pCopyBufferInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyImage2(VkCommandBuffer commandBuffer, VkCopyImageInfo2* pCopyImageInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYIMAGE2_EXISTS
layer_CmdCopyImage2(commandBuffer, pCopyImageInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyImage2(commandBuffer, pCopyImageInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBlitImage2(VkCommandBuffer commandBuffer, VkBlitImageInfo2* pBlitImageInfo ) {
scoped_lock l(global_lock);
#ifdef CMDBLITIMAGE2_EXISTS
layer_CmdBlitImage2(commandBuffer, pBlitImageInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBlitImage2(commandBuffer, pBlitImageInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyBufferToImage2(VkCommandBuffer commandBuffer, VkCopyBufferToImageInfo2* pCopyBufferToImageInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYBUFFERTOIMAGE2_EXISTS
layer_CmdCopyBufferToImage2(commandBuffer, pCopyBufferToImageInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyBufferToImage2(commandBuffer, pCopyBufferToImageInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, VkCopyImageToBufferInfo2* pCopyImageToBufferInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYIMAGETOBUFFER2_EXISTS
layer_CmdCopyImageToBuffer2(commandBuffer, pCopyImageToBufferInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyImageToBuffer2(commandBuffer, pCopyImageToBufferInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdResolveImage2(VkCommandBuffer commandBuffer, VkResolveImageInfo2* pResolveImageInfo ) {
scoped_lock l(global_lock);
#ifdef CMDRESOLVEIMAGE2_EXISTS
layer_CmdResolveImage2(commandBuffer, pResolveImageInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdResolveImage2(commandBuffer, pResolveImageInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetFragmentShadingRateKHR(VkCommandBuffer commandBuffer, VkExtent2D* pFragmentSize, VkFragmentShadingRateCombinerOpKHR* combinerOps ) {
scoped_lock l(global_lock);
#ifdef CMDSETFRAGMENTSHADINGRATEKHR_EXISTS
layer_CmdSetFragmentShadingRateKHR(commandBuffer, pFragmentSize, combinerOps);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetFragmentShadingRateKHR(commandBuffer, pFragmentSize, combinerOps);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetFragmentShadingRateEnumNV(VkCommandBuffer commandBuffer, VkFragmentShadingRateNV shadingRate, VkFragmentShadingRateCombinerOpKHR* combinerOps ) {
scoped_lock l(global_lock);
#ifdef CMDSETFRAGMENTSHADINGRATEENUMNV_EXISTS
layer_CmdSetFragmentShadingRateEnumNV(commandBuffer, shadingRate, combinerOps);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetFragmentShadingRateEnumNV(commandBuffer, shadingRate, combinerOps);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetAccelerationStructureBuildSizesKHR(VkDevice device, VkAccelerationStructureBuildTypeKHR buildType, VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo, uint32_t* pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo ) {
scoped_lock l(global_lock);
#ifdef GETACCELERATIONSTRUCTUREBUILDSIZESKHR_EXISTS
layer_GetAccelerationStructureBuildSizesKHR(device, buildType, pBuildInfo, pMaxPrimitiveCounts, pSizeInfo);
#endif 
device_dispatch[GetKey(device)].GetAccelerationStructureBuildSizesKHR(device, buildType, pBuildInfo, pMaxPrimitiveCounts, pSizeInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions ) {
scoped_lock l(global_lock);
#ifdef CMDSETVERTEXINPUTEXT_EXISTS
layer_CmdSetVertexInputEXT(commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetVertexInputEXT(commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetColorWriteEnableEXT(VkCommandBuffer commandBuffer, uint32_t attachmentCount, VkBool32* pColorWriteEnables ) {
scoped_lock l(global_lock);
#ifdef CMDSETCOLORWRITEENABLEEXT_EXISTS
layer_CmdSetColorWriteEnableEXT(commandBuffer, attachmentCount, pColorWriteEnables);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetColorWriteEnableEXT(commandBuffer, attachmentCount, pColorWriteEnables);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetEvent2(VkCommandBuffer commandBuffer, VkEvent event, VkDependencyInfo* pDependencyInfo ) {
scoped_lock l(global_lock);
#ifdef CMDSETEVENT2_EXISTS
layer_CmdSetEvent2(commandBuffer, event, pDependencyInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetEvent2(commandBuffer, event, pDependencyInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdResetEvent2(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask ) {
scoped_lock l(global_lock);
#ifdef CMDRESETEVENT2_EXISTS
layer_CmdResetEvent2(commandBuffer, event, stageMask);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdResetEvent2(commandBuffer, event, stageMask);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWaitEvents2(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent* pEvents, VkDependencyInfo* pDependencyInfos ) {
scoped_lock l(global_lock);
#ifdef CMDWAITEVENTS2_EXISTS
layer_CmdWaitEvents2(commandBuffer, eventCount, pEvents, pDependencyInfos);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWaitEvents2(commandBuffer, eventCount, pEvents, pDependencyInfos);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPipelineBarrier2(VkCommandBuffer commandBuffer, VkDependencyInfo* pDependencyInfo ) {
scoped_lock l(global_lock);
#ifdef CMDPIPELINEBARRIER2_EXISTS
layer_CmdPipelineBarrier2(commandBuffer, pDependencyInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPipelineBarrier2(commandBuffer, pDependencyInfo);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_QueueSubmit2(VkQueue queue, uint32_t submitCount, VkSubmitInfo2* pSubmits, VkFence fence ) {
scoped_lock l(global_lock);
#ifdef QUEUESUBMIT2_EXISTS
layer_QueueSubmit2(queue, submitCount, pSubmits, fence);
#endif 
device_dispatch[GetKey(queue)].QueueSubmit2(queue, submitCount, pSubmits, fence);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWriteTimestamp2(VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query ) {
scoped_lock l(global_lock);
#ifdef CMDWRITETIMESTAMP2_EXISTS
layer_CmdWriteTimestamp2(commandBuffer, stage, queryPool, query);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWriteTimestamp2(commandBuffer, stage, queryPool, query);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWriteBufferMarker2AMD(VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker ) {
scoped_lock l(global_lock);
#ifdef CMDWRITEBUFFERMARKER2AMD_EXISTS
layer_CmdWriteBufferMarker2AMD(commandBuffer, stage, dstBuffer, dstOffset, marker);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWriteBufferMarker2AMD(commandBuffer, stage, dstBuffer, dstOffset, marker);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData ) {
scoped_lock l(global_lock);
#ifdef GETQUEUECHECKPOINTDATA2NV_EXISTS
layer_GetQueueCheckpointData2NV(queue, pCheckpointDataCount, pCheckpointData);
#endif 
device_dispatch[GetKey(queue)].GetQueueCheckpointData2NV(queue, pCheckpointDataCount, pCheckpointData);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyMemoryToImageEXT(VkDevice device, VkCopyMemoryToImageInfoEXT* pCopyMemoryToImageInfo ) {
scoped_lock l(global_lock);
#ifdef COPYMEMORYTOIMAGEEXT_EXISTS
layer_CopyMemoryToImageEXT(device, pCopyMemoryToImageInfo);
#endif 
device_dispatch[GetKey(device)].CopyMemoryToImageEXT(device, pCopyMemoryToImageInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyImageToMemoryEXT(VkDevice device, VkCopyImageToMemoryInfoEXT* pCopyImageToMemoryInfo ) {
scoped_lock l(global_lock);
#ifdef COPYIMAGETOMEMORYEXT_EXISTS
layer_CopyImageToMemoryEXT(device, pCopyImageToMemoryInfo);
#endif 
device_dispatch[GetKey(device)].CopyImageToMemoryEXT(device, pCopyImageToMemoryInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyImageToImageEXT(VkDevice device, VkCopyImageToImageInfoEXT* pCopyImageToImageInfo ) {
scoped_lock l(global_lock);
#ifdef COPYIMAGETOIMAGEEXT_EXISTS
layer_CopyImageToImageEXT(device, pCopyImageToImageInfo);
#endif 
device_dispatch[GetKey(device)].CopyImageToImageEXT(device, pCopyImageToImageInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_TransitionImageLayoutEXT(VkDevice device, uint32_t transitionCount, VkHostImageLayoutTransitionInfoEXT* pTransitions ) {
scoped_lock l(global_lock);
#ifdef TRANSITIONIMAGELAYOUTEXT_EXISTS
layer_TransitionImageLayoutEXT(device, transitionCount, pTransitions);
#endif 
device_dispatch[GetKey(device)].TransitionImageLayoutEXT(device, transitionCount, pTransitions);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateVideoSessionKHR(VkDevice device, VkVideoSessionCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession ) {
scoped_lock l(global_lock);
#ifdef CREATEVIDEOSESSIONKHR_EXISTS
layer_CreateVideoSessionKHR(device, pCreateInfo, pAllocator, pVideoSession);
#endif 
device_dispatch[GetKey(device)].CreateVideoSessionKHR(device, pCreateInfo, pAllocator, pVideoSession);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYVIDEOSESSIONKHR_EXISTS
layer_DestroyVideoSessionKHR(device, videoSession, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyVideoSessionKHR(device, videoSession, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters ) {
scoped_lock l(global_lock);
#ifdef CREATEVIDEOSESSIONPARAMETERSKHR_EXISTS
layer_CreateVideoSessionParametersKHR(device, pCreateInfo, pAllocator, pVideoSessionParameters);
#endif 
device_dispatch[GetKey(device)].CreateVideoSessionParametersKHR(device, pCreateInfo, pAllocator, pVideoSessionParameters);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_UpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo ) {
scoped_lock l(global_lock);
#ifdef UPDATEVIDEOSESSIONPARAMETERSKHR_EXISTS
layer_UpdateVideoSessionParametersKHR(device, videoSessionParameters, pUpdateInfo);
#endif 
device_dispatch[GetKey(device)].UpdateVideoSessionParametersKHR(device, videoSessionParameters, pUpdateInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetEncodedVideoSessionParametersKHR(VkDevice device, VkVideoEncodeSessionParametersGetInfoKHR* pVideoSessionParametersInfo, VkVideoEncodeSessionParametersFeedbackInfoKHR* pFeedbackInfo, size_t* pDataSize, void* pData ) {
scoped_lock l(global_lock);
#ifdef GETENCODEDVIDEOSESSIONPARAMETERSKHR_EXISTS
layer_GetEncodedVideoSessionParametersKHR(device, pVideoSessionParametersInfo, pFeedbackInfo, pDataSize, pData);
#endif 
device_dispatch[GetKey(device)].GetEncodedVideoSessionParametersKHR(device, pVideoSessionParametersInfo, pFeedbackInfo, pDataSize, pData);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYVIDEOSESSIONPARAMETERSKHR_EXISTS
layer_DestroyVideoSessionParametersKHR(device, videoSessionParameters, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyVideoSessionParametersKHR(device, videoSessionParameters, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETVIDEOSESSIONMEMORYREQUIREMENTSKHR_EXISTS
layer_GetVideoSessionMemoryRequirementsKHR(device, videoSession, pMemoryRequirementsCount, pMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetVideoSessionMemoryRequirementsKHR(device, videoSession, pMemoryRequirementsCount, pMemoryRequirements);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos ) {
scoped_lock l(global_lock);
#ifdef BINDVIDEOSESSIONMEMORYKHR_EXISTS
layer_BindVideoSessionMemoryKHR(device, videoSession, bindSessionMemoryInfoCount, pBindSessionMemoryInfos);
#endif 
device_dispatch[GetKey(device)].BindVideoSessionMemoryKHR(device, videoSession, bindSessionMemoryInfoCount, pBindSessionMemoryInfos);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDecodeVideoKHR(VkCommandBuffer commandBuffer, VkVideoDecodeInfoKHR* pDecodeInfo ) {
scoped_lock l(global_lock);
#ifdef CMDDECODEVIDEOKHR_EXISTS
layer_CmdDecodeVideoKHR(commandBuffer, pDecodeInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDecodeVideoKHR(commandBuffer, pDecodeInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, VkVideoBeginCodingInfoKHR* pBeginInfo ) {
scoped_lock l(global_lock);
#ifdef CMDBEGINVIDEOCODINGKHR_EXISTS
layer_CmdBeginVideoCodingKHR(commandBuffer, pBeginInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginVideoCodingKHR(commandBuffer, pBeginInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, VkVideoCodingControlInfoKHR* pCodingControlInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCONTROLVIDEOCODINGKHR_EXISTS
layer_CmdControlVideoCodingKHR(commandBuffer, pCodingControlInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdControlVideoCodingKHR(commandBuffer, pCodingControlInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, VkVideoEndCodingInfoKHR* pEndCodingInfo ) {
scoped_lock l(global_lock);
#ifdef CMDENDVIDEOCODINGKHR_EXISTS
layer_CmdEndVideoCodingKHR(commandBuffer, pEndCodingInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndVideoCodingKHR(commandBuffer, pEndCodingInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEncodeVideoKHR(VkCommandBuffer commandBuffer, VkVideoEncodeInfoKHR* pEncodeInfo ) {
scoped_lock l(global_lock);
#ifdef CMDENCODEVIDEOKHR_EXISTS
layer_CmdEncodeVideoKHR(commandBuffer, pEncodeInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEncodeVideoKHR(commandBuffer, pEncodeInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, VkDecompressMemoryRegionNV* pDecompressMemoryRegions ) {
scoped_lock l(global_lock);
#ifdef CMDDECOMPRESSMEMORYNV_EXISTS
layer_CmdDecompressMemoryNV(commandBuffer, decompressRegionCount, pDecompressMemoryRegions);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDecompressMemoryNV(commandBuffer, decompressRegionCount, pDecompressMemoryRegions);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride ) {
scoped_lock l(global_lock);
#ifdef CMDDECOMPRESSMEMORYINDIRECTCOUNTNV_EXISTS
layer_CmdDecompressMemoryIndirectCountNV(commandBuffer, indirectCommandsAddress, indirectCommandsCountAddress, stride);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDecompressMemoryIndirectCountNV(commandBuffer, indirectCommandsAddress, indirectCommandsCountAddress, stride);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateCuModuleNVX(VkDevice device, VkCuModuleCreateInfoNVX* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule ) {
scoped_lock l(global_lock);
#ifdef CREATECUMODULENVX_EXISTS
layer_CreateCuModuleNVX(device, pCreateInfo, pAllocator, pModule);
#endif 
device_dispatch[GetKey(device)].CreateCuModuleNVX(device, pCreateInfo, pAllocator, pModule);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateCuFunctionNVX(VkDevice device, VkCuFunctionCreateInfoNVX* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction ) {
scoped_lock l(global_lock);
#ifdef CREATECUFUNCTIONNVX_EXISTS
layer_CreateCuFunctionNVX(device, pCreateInfo, pAllocator, pFunction);
#endif 
device_dispatch[GetKey(device)].CreateCuFunctionNVX(device, pCreateInfo, pAllocator, pFunction);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYCUMODULENVX_EXISTS
layer_DestroyCuModuleNVX(device, module, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyCuModuleNVX(device, module, pAllocator);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYCUFUNCTIONNVX_EXISTS
layer_DestroyCuFunctionNVX(device, function, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyCuFunctionNVX(device, function, pAllocator);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, VkCuLaunchInfoNVX* pLaunchInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCULAUNCHKERNELNVX_EXISTS
layer_CmdCuLaunchKernelNVX(commandBuffer, pLaunchInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCuLaunchKernelNVX(commandBuffer, pLaunchInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes ) {
scoped_lock l(global_lock);
#ifdef GETDESCRIPTORSETLAYOUTSIZEEXT_EXISTS
layer_GetDescriptorSetLayoutSizeEXT(device, layout, pLayoutSizeInBytes);
#endif 
device_dispatch[GetKey(device)].GetDescriptorSetLayoutSizeEXT(device, layout, pLayoutSizeInBytes);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset ) {
scoped_lock l(global_lock);
#ifdef GETDESCRIPTORSETLAYOUTBINDINGOFFSETEXT_EXISTS
layer_GetDescriptorSetLayoutBindingOffsetEXT(device, layout, binding, pOffset);
#endif 
device_dispatch[GetKey(device)].GetDescriptorSetLayoutBindingOffsetEXT(device, layout, binding, pOffset);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDescriptorEXT(VkDevice device, VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor ) {
scoped_lock l(global_lock);
#ifdef GETDESCRIPTOREXT_EXISTS
layer_GetDescriptorEXT(device, pDescriptorInfo, dataSize, pDescriptor);
#endif 
device_dispatch[GetKey(device)].GetDescriptorEXT(device, pDescriptorInfo, dataSize, pDescriptor);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, VkDescriptorBufferBindingInfoEXT* pBindingInfos ) {
scoped_lock l(global_lock);
#ifdef CMDBINDDESCRIPTORBUFFERSEXT_EXISTS
layer_CmdBindDescriptorBuffersEXT(commandBuffer, bufferCount, pBindingInfos);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindDescriptorBuffersEXT(commandBuffer, bufferCount, pBindingInfos);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, uint32_t* pBufferIndices, VkDeviceSize* pOffsets ) {
scoped_lock l(global_lock);
#ifdef CMDSETDESCRIPTORBUFFEROFFSETSEXT_EXISTS
layer_CmdSetDescriptorBufferOffsetsEXT(commandBuffer, pipelineBindPoint, layout, firstSet, setCount, pBufferIndices, pOffsets);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDescriptorBufferOffsetsEXT(commandBuffer, pipelineBindPoint, layout, firstSet, setCount, pBufferIndices, pOffsets);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set ) {
scoped_lock l(global_lock);
#ifdef CMDBINDDESCRIPTORBUFFEREMBEDDEDSAMPLERSEXT_EXISTS
layer_CmdBindDescriptorBufferEmbeddedSamplersEXT(commandBuffer, pipelineBindPoint, layout, set);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindDescriptorBufferEmbeddedSamplersEXT(commandBuffer, pipelineBindPoint, layout, set);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData ) {
scoped_lock l(global_lock);
#ifdef GETBUFFEROPAQUECAPTUREDESCRIPTORDATAEXT_EXISTS
layer_GetBufferOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
#endif 
device_dispatch[GetKey(device)].GetBufferOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData ) {
scoped_lock l(global_lock);
#ifdef GETIMAGEOPAQUECAPTUREDESCRIPTORDATAEXT_EXISTS
layer_GetImageOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
#endif 
device_dispatch[GetKey(device)].GetImageOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData ) {
scoped_lock l(global_lock);
#ifdef GETIMAGEVIEWOPAQUECAPTUREDESCRIPTORDATAEXT_EXISTS
layer_GetImageViewOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
#endif 
device_dispatch[GetKey(device)].GetImageViewOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData ) {
scoped_lock l(global_lock);
#ifdef GETSAMPLEROPAQUECAPTUREDESCRIPTORDATAEXT_EXISTS
layer_GetSamplerOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
#endif 
device_dispatch[GetKey(device)].GetSamplerOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData ) {
scoped_lock l(global_lock);
#ifdef GETACCELERATIONSTRUCTUREOPAQUECAPTUREDESCRIPTORDATAEXT_EXISTS
layer_GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
#endif 
device_dispatch[GetKey(device)].GetAccelerationStructureOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_SetDeviceMemoryPriorityEXT(VkDevice device, VkDeviceMemory memory, float priority ) {
scoped_lock l(global_lock);
#ifdef SETDEVICEMEMORYPRIORITYEXT_EXISTS
layer_SetDeviceMemoryPriorityEXT(device, memory, priority);
#endif 
device_dispatch[GetKey(device)].SetDeviceMemoryPriorityEXT(device, memory, priority);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_WaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout ) {
scoped_lock l(global_lock);
#ifdef WAITFORPRESENTKHR_EXISTS
layer_WaitForPresentKHR(device, swapchain, presentId, timeout);
#endif 
device_dispatch[GetKey(device)].WaitForPresentKHR(device, swapchain, presentId, timeout);
return VK_SUCCESS;
}

#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection ) {
scoped_lock l(global_lock);
#ifdef CREATEBUFFERCOLLECTIONFUCHSIA_EXISTS
layer_CreateBufferCollectionFUCHSIA(device, pCreateInfo, pAllocator, pCollection);
#endif 
device_dispatch[GetKey(device)].CreateBufferCollectionFUCHSIA(device, pCreateInfo, pAllocator, pCollection);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SetBufferCollectionBufferConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo ) {
scoped_lock l(global_lock);
#ifdef SETBUFFERCOLLECTIONBUFFERCONSTRAINTSFUCHSIA_EXISTS
layer_SetBufferCollectionBufferConstraintsFUCHSIA(device, collection, pBufferConstraintsInfo);
#endif 
device_dispatch[GetKey(device)].SetBufferCollectionBufferConstraintsFUCHSIA(device, collection, pBufferConstraintsInfo);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SetBufferCollectionImageConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo ) {
scoped_lock l(global_lock);
#ifdef SETBUFFERCOLLECTIONIMAGECONSTRAINTSFUCHSIA_EXISTS
layer_SetBufferCollectionImageConstraintsFUCHSIA(device, collection, pImageConstraintsInfo);
#endif 
device_dispatch[GetKey(device)].SetBufferCollectionImageConstraintsFUCHSIA(device, collection, pImageConstraintsInfo);
return VK_SUCCESS;
}

#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYBUFFERCOLLECTIONFUCHSIA_EXISTS
layer_DestroyBufferCollectionFUCHSIA(device, collection, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyBufferCollectionFUCHSIA(device, collection, pAllocator);
}

#endif
#if defined(VK_USE_PLATFORM_FUCHSIA)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetBufferCollectionPropertiesFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETBUFFERCOLLECTIONPROPERTIESFUCHSIA_EXISTS
layer_GetBufferCollectionPropertiesFUCHSIA(device, collection, pProperties);
#endif 
device_dispatch[GetKey(device)].GetBufferCollectionPropertiesFUCHSIA(device, collection, pProperties);
return VK_SUCCESS;
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateCudaModuleNV(VkDevice device, VkCudaModuleCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCudaModuleNV* pModule ) {
scoped_lock l(global_lock);
#ifdef CREATECUDAMODULENV_EXISTS
layer_CreateCudaModuleNV(device, pCreateInfo, pAllocator, pModule);
#endif 
device_dispatch[GetKey(device)].CreateCudaModuleNV(device, pCreateInfo, pAllocator, pModule);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetCudaModuleCacheNV(VkDevice device, VkCudaModuleNV module, size_t* pCacheSize, void* pCacheData ) {
scoped_lock l(global_lock);
#ifdef GETCUDAMODULECACHENV_EXISTS
layer_GetCudaModuleCacheNV(device, module, pCacheSize, pCacheData);
#endif 
device_dispatch[GetKey(device)].GetCudaModuleCacheNV(device, module, pCacheSize, pCacheData);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateCudaFunctionNV(VkDevice device, VkCudaFunctionCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCudaFunctionNV* pFunction ) {
scoped_lock l(global_lock);
#ifdef CREATECUDAFUNCTIONNV_EXISTS
layer_CreateCudaFunctionNV(device, pCreateInfo, pAllocator, pFunction);
#endif 
device_dispatch[GetKey(device)].CreateCudaFunctionNV(device, pCreateInfo, pAllocator, pFunction);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyCudaModuleNV(VkDevice device, VkCudaModuleNV module, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYCUDAMODULENV_EXISTS
layer_DestroyCudaModuleNV(device, module, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyCudaModuleNV(device, module, pAllocator);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyCudaFunctionNV(VkDevice device, VkCudaFunctionNV function, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYCUDAFUNCTIONNV_EXISTS
layer_DestroyCudaFunctionNV(device, function, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyCudaFunctionNV(device, function, pAllocator);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCudaLaunchKernelNV(VkCommandBuffer commandBuffer, VkCudaLaunchInfoNV* pLaunchInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCUDALAUNCHKERNELNV_EXISTS
layer_CmdCudaLaunchKernelNV(commandBuffer, pLaunchInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCudaLaunchKernelNV(commandBuffer, pLaunchInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginRendering(VkCommandBuffer commandBuffer, VkRenderingInfo* pRenderingInfo ) {
scoped_lock l(global_lock);
#ifdef CMDBEGINRENDERING_EXISTS
layer_CmdBeginRendering(commandBuffer, pRenderingInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginRendering(commandBuffer, pRenderingInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndRendering(VkCommandBuffer commandBuffer ) {
scoped_lock l(global_lock);
#ifdef CMDENDRENDERING_EXISTS
layer_CmdEndRendering(commandBuffer);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndRendering(commandBuffer);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping ) {
scoped_lock l(global_lock);
#ifdef GETDESCRIPTORSETLAYOUTHOSTMAPPINGINFOVALVE_EXISTS
layer_GetDescriptorSetLayoutHostMappingInfoVALVE(device, pBindingReference, pHostMapping);
#endif 
device_dispatch[GetKey(device)].GetDescriptorSetLayoutHostMappingInfoVALVE(device, pBindingReference, pHostMapping);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void** ppData ) {
scoped_lock l(global_lock);
#ifdef GETDESCRIPTORSETHOSTMAPPINGVALVE_EXISTS
layer_GetDescriptorSetHostMappingVALVE(device, descriptorSet, ppData);
#endif 
device_dispatch[GetKey(device)].GetDescriptorSetHostMappingVALVE(device, descriptorSet, ppData);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateMicromapEXT(VkDevice device, VkMicromapCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkMicromapEXT* pMicromap ) {
scoped_lock l(global_lock);
#ifdef CREATEMICROMAPEXT_EXISTS
layer_CreateMicromapEXT(device, pCreateInfo, pAllocator, pMicromap);
#endif 
device_dispatch[GetKey(device)].CreateMicromapEXT(device, pCreateInfo, pAllocator, pMicromap);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBuildMicromapsEXT(VkCommandBuffer commandBuffer, uint32_t infoCount, VkMicromapBuildInfoEXT* pInfos ) {
scoped_lock l(global_lock);
#ifdef CMDBUILDMICROMAPSEXT_EXISTS
layer_CmdBuildMicromapsEXT(commandBuffer, infoCount, pInfos);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBuildMicromapsEXT(commandBuffer, infoCount, pInfos);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BuildMicromapsEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, VkMicromapBuildInfoEXT* pInfos ) {
scoped_lock l(global_lock);
#ifdef BUILDMICROMAPSEXT_EXISTS
layer_BuildMicromapsEXT(device, deferredOperation, infoCount, pInfos);
#endif 
device_dispatch[GetKey(device)].BuildMicromapsEXT(device, deferredOperation, infoCount, pInfos);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYMICROMAPEXT_EXISTS
layer_DestroyMicromapEXT(device, micromap, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyMicromapEXT(device, micromap, pAllocator);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyMicromapEXT(VkCommandBuffer commandBuffer, VkCopyMicromapInfoEXT* pInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYMICROMAPEXT_EXISTS
layer_CmdCopyMicromapEXT(commandBuffer, pInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyMicromapEXT(commandBuffer, pInfo);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyMicromapInfoEXT* pInfo ) {
scoped_lock l(global_lock);
#ifdef COPYMICROMAPEXT_EXISTS
layer_CopyMicromapEXT(device, deferredOperation, pInfo);
#endif 
device_dispatch[GetKey(device)].CopyMicromapEXT(device, deferredOperation, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, VkCopyMicromapToMemoryInfoEXT* pInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYMICROMAPTOMEMORYEXT_EXISTS
layer_CmdCopyMicromapToMemoryEXT(commandBuffer, pInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyMicromapToMemoryEXT(commandBuffer, pInfo);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyMicromapToMemoryInfoEXT* pInfo ) {
scoped_lock l(global_lock);
#ifdef COPYMICROMAPTOMEMORYEXT_EXISTS
layer_CopyMicromapToMemoryEXT(device, deferredOperation, pInfo);
#endif 
device_dispatch[GetKey(device)].CopyMicromapToMemoryEXT(device, deferredOperation, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, VkCopyMemoryToMicromapInfoEXT* pInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYMEMORYTOMICROMAPEXT_EXISTS
layer_CmdCopyMemoryToMicromapEXT(commandBuffer, pInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyMemoryToMicromapEXT(commandBuffer, pInfo);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyMemoryToMicromapInfoEXT* pInfo ) {
scoped_lock l(global_lock);
#ifdef COPYMEMORYTOMICROMAPEXT_EXISTS
layer_CopyMemoryToMicromapEXT(device, deferredOperation, pInfo);
#endif 
device_dispatch[GetKey(device)].CopyMemoryToMicromapEXT(device, deferredOperation, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery ) {
scoped_lock l(global_lock);
#ifdef CMDWRITEMICROMAPSPROPERTIESEXT_EXISTS
layer_CmdWriteMicromapsPropertiesEXT(commandBuffer, micromapCount, pMicromaps, queryType, queryPool, firstQuery);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWriteMicromapsPropertiesEXT(commandBuffer, micromapCount, pMicromaps, queryType, queryPool, firstQuery);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_WriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, VkMicromapEXT* pMicromaps, VkQueryType queryType, size_t dataSize, void* pData, size_t stride ) {
scoped_lock l(global_lock);
#ifdef WRITEMICROMAPSPROPERTIESEXT_EXISTS
layer_WriteMicromapsPropertiesEXT(device, micromapCount, pMicromaps, queryType, dataSize, pData, stride);
#endif 
device_dispatch[GetKey(device)].WriteMicromapsPropertiesEXT(device, micromapCount, pMicromaps, queryType, dataSize, pData, stride);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceMicromapCompatibilityEXT(VkDevice device, VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEMICROMAPCOMPATIBILITYEXT_EXISTS
layer_GetDeviceMicromapCompatibilityEXT(device, pVersionInfo, pCompatibility);
#endif 
device_dispatch[GetKey(device)].GetDeviceMicromapCompatibilityEXT(device, pVersionInfo, pCompatibility);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetMicromapBuildSizesEXT(VkDevice device, VkAccelerationStructureBuildTypeKHR buildType, VkMicromapBuildInfoEXT* pBuildInfo, VkMicromapBuildSizesInfoEXT* pSizeInfo ) {
scoped_lock l(global_lock);
#ifdef GETMICROMAPBUILDSIZESEXT_EXISTS
layer_GetMicromapBuildSizesEXT(device, buildType, pBuildInfo, pSizeInfo);
#endif 
device_dispatch[GetKey(device)].GetMicromapBuildSizesEXT(device, buildType, pBuildInfo, pSizeInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier ) {
scoped_lock l(global_lock);
#ifdef GETSHADERMODULEIDENTIFIEREXT_EXISTS
layer_GetShaderModuleIdentifierEXT(device, shaderModule, pIdentifier);
#endif 
device_dispatch[GetKey(device)].GetShaderModuleIdentifierEXT(device, shaderModule, pIdentifier);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetShaderModuleCreateInfoIdentifierEXT(VkDevice device, VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier ) {
scoped_lock l(global_lock);
#ifdef GETSHADERMODULECREATEINFOIDENTIFIEREXT_EXISTS
layer_GetShaderModuleCreateInfoIdentifierEXT(device, pCreateInfo, pIdentifier);
#endif 
device_dispatch[GetKey(device)].GetShaderModuleCreateInfoIdentifierEXT(device, pCreateInfo, pIdentifier);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageSubresourceLayout2KHR(VkDevice device, VkImage image, VkImageSubresource2KHR* pSubresource, VkSubresourceLayout2KHR* pLayout ) {
scoped_lock l(global_lock);
#ifdef GETIMAGESUBRESOURCELAYOUT2KHR_EXISTS
layer_GetImageSubresourceLayout2KHR(device, image, pSubresource, pLayout);
#endif 
device_dispatch[GetKey(device)].GetImageSubresourceLayout2KHR(device, image, pSubresource, pLayout);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetPipelinePropertiesEXT(VkDevice device, VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties ) {
scoped_lock l(global_lock);
#ifdef GETPIPELINEPROPERTIESEXT_EXISTS
layer_GetPipelinePropertiesEXT(device, pPipelineInfo, pPipelineProperties);
#endif 
device_dispatch[GetKey(device)].GetPipelinePropertiesEXT(device, pPipelineInfo, pPipelineProperties);
return VK_SUCCESS;
}

#if defined(VK_USE_PLATFORM_METAL_EXT)
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_ExportMetalObjectsEXT(VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo ) {
scoped_lock l(global_lock);
#ifdef EXPORTMETALOBJECTSEXT_EXISTS
layer_ExportMetalObjectsEXT(device, pMetalObjectsInfo);
#endif 
device_dispatch[GetKey(device)].ExportMetalObjectsEXT(device, pMetalObjectsInfo);
}

#endif
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETFRAMEBUFFERTILEPROPERTIESQCOM_EXISTS
layer_GetFramebufferTilePropertiesQCOM(device, framebuffer, pPropertiesCount, pProperties);
#endif 
device_dispatch[GetKey(device)].GetFramebufferTilePropertiesQCOM(device, framebuffer, pPropertiesCount, pProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDynamicRenderingTilePropertiesQCOM(VkDevice device, VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETDYNAMICRENDERINGTILEPROPERTIESQCOM_EXISTS
layer_GetDynamicRenderingTilePropertiesQCOM(device, pRenderingInfo, pProperties);
#endif 
device_dispatch[GetKey(device)].GetDynamicRenderingTilePropertiesQCOM(device, pRenderingInfo, pProperties);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession ) {
scoped_lock l(global_lock);
#ifdef CREATEOPTICALFLOWSESSIONNV_EXISTS
layer_CreateOpticalFlowSessionNV(device, pCreateInfo, pAllocator, pSession);
#endif 
device_dispatch[GetKey(device)].CreateOpticalFlowSessionNV(device, pCreateInfo, pAllocator, pSession);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYOPTICALFLOWSESSIONNV_EXISTS
layer_DestroyOpticalFlowSessionNV(device, session, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyOpticalFlowSessionNV(device, session, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout ) {
scoped_lock l(global_lock);
#ifdef BINDOPTICALFLOWSESSIONIMAGENV_EXISTS
layer_BindOpticalFlowSessionImageNV(device, session, bindingPoint, view, layout);
#endif 
device_dispatch[GetKey(device)].BindOpticalFlowSessionImageNV(device, session, bindingPoint, view, layout);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, VkOpticalFlowExecuteInfoNV* pExecuteInfo ) {
scoped_lock l(global_lock);
#ifdef CMDOPTICALFLOWEXECUTENV_EXISTS
layer_CmdOpticalFlowExecuteNV(commandBuffer, session, pExecuteInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdOpticalFlowExecuteNV(commandBuffer, session, pExecuteInfo);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEFAULTINFOEXT_EXISTS
layer_GetDeviceFaultInfoEXT(device, pFaultCounts, pFaultInfo);
#endif 
device_dispatch[GetKey(device)].GetDeviceFaultInfoEXT(device, pFaultCounts, pFaultInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthBias2EXT(VkCommandBuffer commandBuffer, VkDepthBiasInfoEXT* pDepthBiasInfo ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEPTHBIAS2EXT_EXISTS
layer_CmdSetDepthBias2EXT(commandBuffer, pDepthBiasInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthBias2EXT(commandBuffer, pDepthBiasInfo);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_ReleaseSwapchainImagesEXT(VkDevice device, VkReleaseSwapchainImagesInfoEXT* pReleaseInfo ) {
scoped_lock l(global_lock);
#ifdef RELEASESWAPCHAINIMAGESEXT_EXISTS
layer_ReleaseSwapchainImagesEXT(device, pReleaseInfo);
#endif 
device_dispatch[GetKey(device)].ReleaseSwapchainImagesEXT(device, pReleaseInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceImageSubresourceLayoutKHR(VkDevice device, VkDeviceImageSubresourceInfoKHR* pInfo, VkSubresourceLayout2KHR* pLayout ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEIMAGESUBRESOURCELAYOUTKHR_EXISTS
layer_GetDeviceImageSubresourceLayoutKHR(device, pInfo, pLayout);
#endif 
device_dispatch[GetKey(device)].GetDeviceImageSubresourceLayoutKHR(device, pInfo, pLayout);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_MapMemory2KHR(VkDevice device, VkMemoryMapInfoKHR* pMemoryMapInfo, void** ppData ) {
scoped_lock l(global_lock);
#ifdef MAPMEMORY2KHR_EXISTS
layer_MapMemory2KHR(device, pMemoryMapInfo, ppData);
#endif 
device_dispatch[GetKey(device)].MapMemory2KHR(device, pMemoryMapInfo, ppData);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_UnmapMemory2KHR(VkDevice device, VkMemoryUnmapInfoKHR* pMemoryUnmapInfo ) {
scoped_lock l(global_lock);
#ifdef UNMAPMEMORY2KHR_EXISTS
layer_UnmapMemory2KHR(device, pMemoryUnmapInfo);
#endif 
device_dispatch[GetKey(device)].UnmapMemory2KHR(device, pMemoryUnmapInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateShadersEXT(VkDevice device, uint32_t createInfoCount, VkShaderCreateInfoEXT* pCreateInfos, VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders ) {
scoped_lock l(global_lock);
#ifdef CREATESHADERSEXT_EXISTS
layer_CreateShadersEXT(device, createInfoCount, pCreateInfos, pAllocator, pShaders);
#endif 
device_dispatch[GetKey(device)].CreateShadersEXT(device, createInfoCount, pCreateInfos, pAllocator, pShaders);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyShaderEXT(VkDevice device, VkShaderEXT shader, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYSHADEREXT_EXISTS
layer_DestroyShaderEXT(device, shader, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyShaderEXT(device, shader, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData ) {
scoped_lock l(global_lock);
#ifdef GETSHADERBINARYDATAEXT_EXISTS
layer_GetShaderBinaryDataEXT(device, shader, pDataSize, pData);
#endif 
device_dispatch[GetKey(device)].GetShaderBinaryDataEXT(device, shader, pDataSize, pData);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, VkShaderStageFlagBits* pStages, VkShaderEXT* pShaders ) {
scoped_lock l(global_lock);
#ifdef CMDBINDSHADERSEXT_EXISTS
layer_CmdBindShadersEXT(commandBuffer, stageCount, pStages, pShaders);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindShadersEXT(commandBuffer, stageCount, pStages, pShaders);
}

#if defined(VK_USE_PLATFORM_SCREEN_QNX)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetScreenBufferPropertiesQNX(VkDevice device, _screen_buffer* buffer, VkScreenBufferPropertiesQNX* pProperties ) {
scoped_lock l(global_lock);
#ifdef GETSCREENBUFFERPROPERTIESQNX_EXISTS
layer_GetScreenBufferPropertiesQNX(device, buffer, pProperties);
#endif 
device_dispatch[GetKey(device)].GetScreenBufferPropertiesQNX(device, buffer, pProperties);
return VK_SUCCESS;
}

#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetExecutionGraphPipelineScratchSizeAMDX(VkDevice device, VkPipeline executionGraph, VkExecutionGraphPipelineScratchSizeAMDX* pSizeInfo ) {
scoped_lock l(global_lock);
#ifdef GETEXECUTIONGRAPHPIPELINESCRATCHSIZEAMDX_EXISTS
layer_GetExecutionGraphPipelineScratchSizeAMDX(device, executionGraph, pSizeInfo);
#endif 
device_dispatch[GetKey(device)].GetExecutionGraphPipelineScratchSizeAMDX(device, executionGraph, pSizeInfo);
return VK_SUCCESS;
}

#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetExecutionGraphPipelineNodeIndexAMDX(VkDevice device, VkPipeline executionGraph, VkPipelineShaderStageNodeCreateInfoAMDX* pNodeInfo, uint32_t* pNodeIndex ) {
scoped_lock l(global_lock);
#ifdef GETEXECUTIONGRAPHPIPELINENODEINDEXAMDX_EXISTS
layer_GetExecutionGraphPipelineNodeIndexAMDX(device, executionGraph, pNodeInfo, pNodeIndex);
#endif 
device_dispatch[GetKey(device)].GetExecutionGraphPipelineNodeIndexAMDX(device, executionGraph, pNodeInfo, pNodeIndex);
return VK_SUCCESS;
}

#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateExecutionGraphPipelinesAMDX(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkExecutionGraphPipelineCreateInfoAMDX* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines ) {
scoped_lock l(global_lock);
#ifdef CREATEEXECUTIONGRAPHPIPELINESAMDX_EXISTS
layer_CreateExecutionGraphPipelinesAMDX(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
#endif 
device_dispatch[GetKey(device)].CreateExecutionGraphPipelinesAMDX(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
return VK_SUCCESS;
}

#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdInitializeGraphScratchMemoryAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch ) {
scoped_lock l(global_lock);
#ifdef CMDINITIALIZEGRAPHSCRATCHMEMORYAMDX_EXISTS
layer_CmdInitializeGraphScratchMemoryAMDX(commandBuffer, scratch);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdInitializeGraphScratchMemoryAMDX(commandBuffer, scratch);
}

#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDispatchGraphAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDispatchGraphCountInfoAMDX* pCountInfo ) {
scoped_lock l(global_lock);
#ifdef CMDDISPATCHGRAPHAMDX_EXISTS
layer_CmdDispatchGraphAMDX(commandBuffer, scratch, pCountInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDispatchGraphAMDX(commandBuffer, scratch, pCountInfo);
}

#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDispatchGraphIndirectAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDispatchGraphCountInfoAMDX* pCountInfo ) {
scoped_lock l(global_lock);
#ifdef CMDDISPATCHGRAPHINDIRECTAMDX_EXISTS
layer_CmdDispatchGraphIndirectAMDX(commandBuffer, scratch, pCountInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDispatchGraphIndirectAMDX(commandBuffer, scratch, pCountInfo);
}

#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS)
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDispatchGraphIndirectCountAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDeviceAddress countInfo ) {
scoped_lock l(global_lock);
#ifdef CMDDISPATCHGRAPHINDIRECTCOUNTAMDX_EXISTS
layer_CmdDispatchGraphIndirectCountAMDX(commandBuffer, scratch, countInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDispatchGraphIndirectCountAMDX(commandBuffer, scratch, countInfo);
}

#endif
 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindDescriptorSets2KHR(VkCommandBuffer commandBuffer, VkBindDescriptorSetsInfoKHR* pBindDescriptorSetsInfo ) {
scoped_lock l(global_lock);
#ifdef CMDBINDDESCRIPTORSETS2KHR_EXISTS
layer_CmdBindDescriptorSets2KHR(commandBuffer, pBindDescriptorSetsInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindDescriptorSets2KHR(commandBuffer, pBindDescriptorSetsInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPushConstants2KHR(VkCommandBuffer commandBuffer, VkPushConstantsInfoKHR* pPushConstantsInfo ) {
scoped_lock l(global_lock);
#ifdef CMDPUSHCONSTANTS2KHR_EXISTS
layer_CmdPushConstants2KHR(commandBuffer, pPushConstantsInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPushConstants2KHR(commandBuffer, pPushConstantsInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPushDescriptorSet2KHR(VkCommandBuffer commandBuffer, VkPushDescriptorSetInfoKHR* pPushDescriptorSetInfo ) {
scoped_lock l(global_lock);
#ifdef CMDPUSHDESCRIPTORSET2KHR_EXISTS
layer_CmdPushDescriptorSet2KHR(commandBuffer, pPushDescriptorSetInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPushDescriptorSet2KHR(commandBuffer, pPushDescriptorSetInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPushDescriptorSetWithTemplate2KHR(VkCommandBuffer commandBuffer, VkPushDescriptorSetWithTemplateInfoKHR* pPushDescriptorSetWithTemplateInfo ) {
scoped_lock l(global_lock);
#ifdef CMDPUSHDESCRIPTORSETWITHTEMPLATE2KHR_EXISTS
layer_CmdPushDescriptorSetWithTemplate2KHR(commandBuffer, pPushDescriptorSetWithTemplateInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPushDescriptorSetWithTemplate2KHR(commandBuffer, pPushDescriptorSetWithTemplateInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDescriptorBufferOffsets2EXT(VkCommandBuffer commandBuffer, VkSetDescriptorBufferOffsetsInfoEXT* pSetDescriptorBufferOffsetsInfo ) {
scoped_lock l(global_lock);
#ifdef CMDSETDESCRIPTORBUFFEROFFSETS2EXT_EXISTS
layer_CmdSetDescriptorBufferOffsets2EXT(commandBuffer, pSetDescriptorBufferOffsetsInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDescriptorBufferOffsets2EXT(commandBuffer, pSetDescriptorBufferOffsetsInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindDescriptorBufferEmbeddedSamplers2EXT(VkCommandBuffer commandBuffer, VkBindDescriptorBufferEmbeddedSamplersInfoEXT* pBindDescriptorBufferEmbeddedSamplersInfo ) {
scoped_lock l(global_lock);
#ifdef CMDBINDDESCRIPTORBUFFEREMBEDDEDSAMPLERS2EXT_EXISTS
layer_CmdBindDescriptorBufferEmbeddedSamplers2EXT(commandBuffer, pBindDescriptorBufferEmbeddedSamplersInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindDescriptorBufferEmbeddedSamplers2EXT(commandBuffer, pBindDescriptorBufferEmbeddedSamplersInfo);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SetLatencySleepModeNV(VkDevice device, VkSwapchainKHR swapchain, VkLatencySleepModeInfoNV* pSleepModeInfo ) {
scoped_lock l(global_lock);
#ifdef SETLATENCYSLEEPMODENV_EXISTS
layer_SetLatencySleepModeNV(device, swapchain, pSleepModeInfo);
#endif 
device_dispatch[GetKey(device)].SetLatencySleepModeNV(device, swapchain, pSleepModeInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_LatencySleepNV(VkDevice device, VkSwapchainKHR swapchain, VkLatencySleepInfoNV* pSleepInfo ) {
scoped_lock l(global_lock);
#ifdef LATENCYSLEEPNV_EXISTS
layer_LatencySleepNV(device, swapchain, pSleepInfo);
#endif 
device_dispatch[GetKey(device)].LatencySleepNV(device, swapchain, pSleepInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_SetLatencyMarkerNV(VkDevice device, VkSwapchainKHR swapchain, VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo ) {
scoped_lock l(global_lock);
#ifdef SETLATENCYMARKERNV_EXISTS
layer_SetLatencyMarkerNV(device, swapchain, pLatencyMarkerInfo);
#endif 
device_dispatch[GetKey(device)].SetLatencyMarkerNV(device, swapchain, pLatencyMarkerInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetLatencyTimingsNV(VkDevice device, VkSwapchainKHR swapchain, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo ) {
scoped_lock l(global_lock);
#ifdef GETLATENCYTIMINGSNV_EXISTS
layer_GetLatencyTimingsNV(device, swapchain, pLatencyMarkerInfo);
#endif 
device_dispatch[GetKey(device)].GetLatencyTimingsNV(device, swapchain, pLatencyMarkerInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_QueueNotifyOutOfBandNV(VkQueue queue, VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo ) {
scoped_lock l(global_lock);
#ifdef QUEUENOTIFYOUTOFBANDNV_EXISTS
layer_QueueNotifyOutOfBandNV(queue, pQueueTypeInfo);
#endif 
device_dispatch[GetKey(queue)].QueueNotifyOutOfBandNV(queue, pQueueTypeInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_ResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount ) {
scoped_lock l(global_lock);
#ifdef RESETQUERYPOOLEXT_EXISTS
layer_ResetQueryPoolEXT(device, queryPool, firstQuery, queryCount);
#endif 
device_dispatch[GetKey(device)].ResetQueryPoolEXT(device, queryPool, firstQuery, queryCount);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_TrimCommandPoolKHR(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags ) {
scoped_lock l(global_lock);
#ifdef TRIMCOMMANDPOOLKHR_EXISTS
layer_TrimCommandPoolKHR(device, commandPool, flags);
#endif 
device_dispatch[GetKey(device)].TrimCommandPoolKHR(device, commandPool, flags);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceGroupPeerMemoryFeaturesKHR(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEGROUPPEERMEMORYFEATURESKHR_EXISTS
layer_GetDeviceGroupPeerMemoryFeaturesKHR(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
#endif 
device_dispatch[GetKey(device)].GetDeviceGroupPeerMemoryFeaturesKHR(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, VkBindBufferMemoryInfo* pBindInfos ) {
scoped_lock l(global_lock);
#ifdef BINDBUFFERMEMORY2KHR_EXISTS
layer_BindBufferMemory2KHR(device, bindInfoCount, pBindInfos);
#endif 
device_dispatch[GetKey(device)].BindBufferMemory2KHR(device, bindInfoCount, pBindInfos);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_BindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, VkBindImageMemoryInfo* pBindInfos ) {
scoped_lock l(global_lock);
#ifdef BINDIMAGEMEMORY2KHR_EXISTS
layer_BindImageMemory2KHR(device, bindInfoCount, pBindInfos);
#endif 
device_dispatch[GetKey(device)].BindImageMemory2KHR(device, bindInfoCount, pBindInfos);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEVICEMASKKHR_EXISTS
layer_CmdSetDeviceMaskKHR(commandBuffer, deviceMask);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDeviceMaskKHR(commandBuffer, deviceMask);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ ) {
scoped_lock l(global_lock);
#ifdef CMDDISPATCHBASEKHR_EXISTS
layer_CmdDispatchBaseKHR(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDispatchBaseKHR(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate ) {
scoped_lock l(global_lock);
#ifdef CREATEDESCRIPTORUPDATETEMPLATEKHR_EXISTS
layer_CreateDescriptorUpdateTemplateKHR(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
#endif 
device_dispatch[GetKey(device)].CreateDescriptorUpdateTemplateKHR(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYDESCRIPTORUPDATETEMPLATEKHR_EXISTS
layer_DestroyDescriptorUpdateTemplateKHR(device, descriptorUpdateTemplate, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyDescriptorUpdateTemplateKHR(device, descriptorUpdateTemplate, pAllocator);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_UpdateDescriptorSetWithTemplateKHR(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, void* pData ) {
scoped_lock l(global_lock);
#ifdef UPDATEDESCRIPTORSETWITHTEMPLATEKHR_EXISTS
layer_UpdateDescriptorSetWithTemplateKHR(device, descriptorSet, descriptorUpdateTemplate, pData);
#endif 
device_dispatch[GetKey(device)].UpdateDescriptorSetWithTemplateKHR(device, descriptorSet, descriptorUpdateTemplate, pData);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetBufferMemoryRequirements2KHR(VkDevice device, VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETBUFFERMEMORYREQUIREMENTS2KHR_EXISTS
layer_GetBufferMemoryRequirements2KHR(device, pInfo, pMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetBufferMemoryRequirements2KHR(device, pInfo, pMemoryRequirements);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageMemoryRequirements2KHR(VkDevice device, VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETIMAGEMEMORYREQUIREMENTS2KHR_EXISTS
layer_GetImageMemoryRequirements2KHR(device, pInfo, pMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetImageMemoryRequirements2KHR(device, pInfo, pMemoryRequirements);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageSparseMemoryRequirements2KHR(VkDevice device, VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETIMAGESPARSEMEMORYREQUIREMENTS2KHR_EXISTS
layer_GetImageSparseMemoryRequirements2KHR(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetImageSparseMemoryRequirements2KHR(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceBufferMemoryRequirementsKHR(VkDevice device, VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEBUFFERMEMORYREQUIREMENTSKHR_EXISTS
layer_GetDeviceBufferMemoryRequirementsKHR(device, pInfo, pMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetDeviceBufferMemoryRequirementsKHR(device, pInfo, pMemoryRequirements);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceImageMemoryRequirementsKHR(VkDevice device, VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEIMAGEMEMORYREQUIREMENTSKHR_EXISTS
layer_GetDeviceImageMemoryRequirementsKHR(device, pInfo, pMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetDeviceImageMemoryRequirementsKHR(device, pInfo, pMemoryRequirements);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDeviceImageSparseMemoryRequirementsKHR(VkDevice device, VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEIMAGESPARSEMEMORYREQUIREMENTSKHR_EXISTS
layer_GetDeviceImageSparseMemoryRequirementsKHR(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
#endif 
device_dispatch[GetKey(device)].GetDeviceImageSparseMemoryRequirementsKHR(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateSamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversionCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion ) {
scoped_lock l(global_lock);
#ifdef CREATESAMPLERYCBCRCONVERSIONKHR_EXISTS
layer_CreateSamplerYcbcrConversionKHR(device, pCreateInfo, pAllocator, pYcbcrConversion);
#endif 
device_dispatch[GetKey(device)].CreateSamplerYcbcrConversionKHR(device, pCreateInfo, pAllocator, pYcbcrConversion);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroySamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYSAMPLERYCBCRCONVERSIONKHR_EXISTS
layer_DestroySamplerYcbcrConversionKHR(device, ycbcrConversion, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroySamplerYcbcrConversionKHR(device, ycbcrConversion, pAllocator);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetDescriptorSetLayoutSupportKHR(VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport ) {
scoped_lock l(global_lock);
#ifdef GETDESCRIPTORSETLAYOUTSUPPORTKHR_EXISTS
layer_GetDescriptorSetLayoutSupportKHR(device, pCreateInfo, pSupport);
#endif 
device_dispatch[GetKey(device)].GetDescriptorSetLayoutSupportKHR(device, pCreateInfo, pSupport);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation ) {
scoped_lock l(global_lock);
#ifdef GETCALIBRATEDTIMESTAMPSEXT_EXISTS
layer_GetCalibratedTimestampsEXT(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
#endif 
device_dispatch[GetKey(device)].GetCalibratedTimestampsEXT(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateRenderPass2KHR(VkDevice device, VkRenderPassCreateInfo2* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass ) {
scoped_lock l(global_lock);
#ifdef CREATERENDERPASS2KHR_EXISTS
layer_CreateRenderPass2KHR(device, pCreateInfo, pAllocator, pRenderPass);
#endif 
device_dispatch[GetKey(device)].CreateRenderPass2KHR(device, pCreateInfo, pAllocator, pRenderPass);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassBeginInfo* pSubpassBeginInfo ) {
scoped_lock l(global_lock);
#ifdef CMDBEGINRENDERPASS2KHR_EXISTS
layer_CmdBeginRenderPass2KHR(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginRenderPass2KHR(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, VkSubpassBeginInfo* pSubpassBeginInfo, VkSubpassEndInfo* pSubpassEndInfo ) {
scoped_lock l(global_lock);
#ifdef CMDNEXTSUBPASS2KHR_EXISTS
layer_CmdNextSubpass2KHR(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdNextSubpass2KHR(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, VkSubpassEndInfo* pSubpassEndInfo ) {
scoped_lock l(global_lock);
#ifdef CMDENDRENDERPASS2KHR_EXISTS
layer_CmdEndRenderPass2KHR(commandBuffer, pSubpassEndInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndRenderPass2KHR(commandBuffer, pSubpassEndInfo);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetSemaphoreCounterValueKHR(VkDevice device, VkSemaphore semaphore, uint64_t* pValue ) {
scoped_lock l(global_lock);
#ifdef GETSEMAPHORECOUNTERVALUEKHR_EXISTS
layer_GetSemaphoreCounterValueKHR(device, semaphore, pValue);
#endif 
device_dispatch[GetKey(device)].GetSemaphoreCounterValueKHR(device, semaphore, pValue);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_WaitSemaphoresKHR(VkDevice device, VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout ) {
scoped_lock l(global_lock);
#ifdef WAITSEMAPHORESKHR_EXISTS
layer_WaitSemaphoresKHR(device, pWaitInfo, timeout);
#endif 
device_dispatch[GetKey(device)].WaitSemaphoresKHR(device, pWaitInfo, timeout);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SignalSemaphoreKHR(VkDevice device, VkSemaphoreSignalInfo* pSignalInfo ) {
scoped_lock l(global_lock);
#ifdef SIGNALSEMAPHOREKHR_EXISTS
layer_SignalSemaphoreKHR(device, pSignalInfo);
#endif 
device_dispatch[GetKey(device)].SignalSemaphoreKHR(device, pSignalInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWINDIRECTCOUNTKHR_EXISTS
layer_CmdDrawIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWINDIRECTCOUNTAMD_EXISTS
layer_CmdDrawIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWINDEXEDINDIRECTCOUNTKHR_EXISTS
layer_CmdDrawIndexedIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndexedIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride ) {
scoped_lock l(global_lock);
#ifdef CMDDRAWINDEXEDINDIRECTCOUNTAMD_EXISTS
layer_CmdDrawIndexedIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdDrawIndexedIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_GetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData ) {
scoped_lock l(global_lock);
#ifdef GETRAYTRACINGSHADERGROUPHANDLESNV_EXISTS
layer_GetRayTracingShaderGroupHandlesNV(device, pipeline, firstGroup, groupCount, dataSize, pData);
#endif 
device_dispatch[GetKey(device)].GetRayTracingShaderGroupHandlesNV(device, pipeline, firstGroup, groupCount, dataSize, pData);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT uint64_t VKAPI_CALL DetailsLayer_GetBufferOpaqueCaptureAddressKHR(VkDevice device, VkBufferDeviceAddressInfo* pInfo ) {
scoped_lock l(global_lock);
#ifdef GETBUFFEROPAQUECAPTUREADDRESSKHR_EXISTS
layer_GetBufferOpaqueCaptureAddressKHR(device, pInfo);
#endif 
device_dispatch[GetKey(device)].GetBufferOpaqueCaptureAddressKHR(device, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkDeviceAddress VKAPI_CALL DetailsLayer_GetBufferDeviceAddressKHR(VkDevice device, VkBufferDeviceAddressInfo* pInfo ) {
scoped_lock l(global_lock);
#ifdef GETBUFFERDEVICEADDRESSKHR_EXISTS
layer_GetBufferDeviceAddressKHR(device, pInfo);
#endif 
device_dispatch[GetKey(device)].GetBufferDeviceAddressKHR(device, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT VkDeviceAddress VKAPI_CALL DetailsLayer_GetBufferDeviceAddressEXT(VkDevice device, VkBufferDeviceAddressInfo* pInfo ) {
scoped_lock l(global_lock);
#ifdef GETBUFFERDEVICEADDRESSEXT_EXISTS
layer_GetBufferDeviceAddressEXT(device, pInfo);
#endif 
device_dispatch[GetKey(device)].GetBufferDeviceAddressEXT(device, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT uint64_t VKAPI_CALL DetailsLayer_GetDeviceMemoryOpaqueCaptureAddressKHR(VkDevice device, VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo ) {
scoped_lock l(global_lock);
#ifdef GETDEVICEMEMORYOPAQUECAPTUREADDRESSKHR_EXISTS
layer_GetDeviceMemoryOpaqueCaptureAddressKHR(device, pInfo);
#endif 
device_dispatch[GetKey(device)].GetDeviceMemoryOpaqueCaptureAddressKHR(device, pInfo);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern ) {
scoped_lock l(global_lock);
#ifdef CMDSETLINESTIPPLEEXT_EXISTS
layer_CmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode ) {
scoped_lock l(global_lock);
#ifdef CMDSETCULLMODEEXT_EXISTS
layer_CmdSetCullModeEXT(commandBuffer, cullMode);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetCullModeEXT(commandBuffer, cullMode);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace ) {
scoped_lock l(global_lock);
#ifdef CMDSETFRONTFACEEXT_EXISTS
layer_CmdSetFrontFaceEXT(commandBuffer, frontFace);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetFrontFaceEXT(commandBuffer, frontFace);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology ) {
scoped_lock l(global_lock);
#ifdef CMDSETPRIMITIVETOPOLOGYEXT_EXISTS
layer_CmdSetPrimitiveTopologyEXT(commandBuffer, primitiveTopology);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetPrimitiveTopologyEXT(commandBuffer, primitiveTopology);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, VkViewport* pViewports ) {
scoped_lock l(global_lock);
#ifdef CMDSETVIEWPORTWITHCOUNTEXT_EXISTS
layer_CmdSetViewportWithCountEXT(commandBuffer, viewportCount, pViewports);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetViewportWithCountEXT(commandBuffer, viewportCount, pViewports);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, VkRect2D* pScissors ) {
scoped_lock l(global_lock);
#ifdef CMDSETSCISSORWITHCOUNTEXT_EXISTS
layer_CmdSetScissorWithCountEXT(commandBuffer, scissorCount, pScissors);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetScissorWithCountEXT(commandBuffer, scissorCount, pScissors);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets, VkDeviceSize* pSizes, VkDeviceSize* pStrides ) {
scoped_lock l(global_lock);
#ifdef CMDBINDVERTEXBUFFERS2EXT_EXISTS
layer_CmdBindVertexBuffers2EXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBindVertexBuffers2EXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEPTHTESTENABLEEXT_EXISTS
layer_CmdSetDepthTestEnableEXT(commandBuffer, depthTestEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthTestEnableEXT(commandBuffer, depthTestEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEPTHWRITEENABLEEXT_EXISTS
layer_CmdSetDepthWriteEnableEXT(commandBuffer, depthWriteEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthWriteEnableEXT(commandBuffer, depthWriteEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEPTHCOMPAREOPEXT_EXISTS
layer_CmdSetDepthCompareOpEXT(commandBuffer, depthCompareOp);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthCompareOpEXT(commandBuffer, depthCompareOp);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEPTHBOUNDSTESTENABLEEXT_EXISTS
layer_CmdSetDepthBoundsTestEnableEXT(commandBuffer, depthBoundsTestEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthBoundsTestEnableEXT(commandBuffer, depthBoundsTestEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETSTENCILTESTENABLEEXT_EXISTS
layer_CmdSetStencilTestEnableEXT(commandBuffer, stencilTestEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetStencilTestEnableEXT(commandBuffer, stencilTestEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp ) {
scoped_lock l(global_lock);
#ifdef CMDSETSTENCILOPEXT_EXISTS
layer_CmdSetStencilOpEXT(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetStencilOpEXT(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETRASTERIZERDISCARDENABLEEXT_EXISTS
layer_CmdSetRasterizerDiscardEnableEXT(commandBuffer, rasterizerDiscardEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetRasterizerDiscardEnableEXT(commandBuffer, rasterizerDiscardEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETDEPTHBIASENABLEEXT_EXISTS
layer_CmdSetDepthBiasEnableEXT(commandBuffer, depthBiasEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetDepthBiasEnableEXT(commandBuffer, depthBiasEnable);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable ) {
scoped_lock l(global_lock);
#ifdef CMDSETPRIMITIVERESTARTENABLEEXT_EXISTS
layer_CmdSetPrimitiveRestartEnableEXT(commandBuffer, primitiveRestartEnable);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetPrimitiveRestartEnableEXT(commandBuffer, primitiveRestartEnable);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreatePrivateDataSlotEXT(VkDevice device, VkPrivateDataSlotCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot ) {
scoped_lock l(global_lock);
#ifdef CREATEPRIVATEDATASLOTEXT_EXISTS
layer_CreatePrivateDataSlotEXT(device, pCreateInfo, pAllocator, pPrivateDataSlot);
#endif 
device_dispatch[GetKey(device)].CreatePrivateDataSlotEXT(device, pCreateInfo, pAllocator, pPrivateDataSlot);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyPrivateDataSlotEXT(VkDevice device, VkPrivateDataSlot privateDataSlot, VkAllocationCallbacks* pAllocator ) {
scoped_lock l(global_lock);
#ifdef DESTROYPRIVATEDATASLOTEXT_EXISTS
layer_DestroyPrivateDataSlotEXT(device, privateDataSlot, pAllocator);
#endif 
device_dispatch[GetKey(device)].DestroyPrivateDataSlotEXT(device, privateDataSlot, pAllocator);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_SetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data ) {
scoped_lock l(global_lock);
#ifdef SETPRIVATEDATAEXT_EXISTS
layer_SetPrivateDataEXT(device, objectType, objectHandle, privateDataSlot, data);
#endif 
device_dispatch[GetKey(device)].SetPrivateDataEXT(device, objectType, objectHandle, privateDataSlot, data);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData ) {
scoped_lock l(global_lock);
#ifdef GETPRIVATEDATAEXT_EXISTS
layer_GetPrivateDataEXT(device, objectType, objectHandle, privateDataSlot, pData);
#endif 
device_dispatch[GetKey(device)].GetPrivateDataEXT(device, objectType, objectHandle, privateDataSlot, pData);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, VkCopyBufferInfo2* pCopyBufferInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYBUFFER2KHR_EXISTS
layer_CmdCopyBuffer2KHR(commandBuffer, pCopyBufferInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyBuffer2KHR(commandBuffer, pCopyBufferInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyImage2KHR(VkCommandBuffer commandBuffer, VkCopyImageInfo2* pCopyImageInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYIMAGE2KHR_EXISTS
layer_CmdCopyImage2KHR(commandBuffer, pCopyImageInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyImage2KHR(commandBuffer, pCopyImageInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBlitImage2KHR(VkCommandBuffer commandBuffer, VkBlitImageInfo2* pBlitImageInfo ) {
scoped_lock l(global_lock);
#ifdef CMDBLITIMAGE2KHR_EXISTS
layer_CmdBlitImage2KHR(commandBuffer, pBlitImageInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBlitImage2KHR(commandBuffer, pBlitImageInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, VkCopyBufferToImageInfo2* pCopyBufferToImageInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYBUFFERTOIMAGE2KHR_EXISTS
layer_CmdCopyBufferToImage2KHR(commandBuffer, pCopyBufferToImageInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyBufferToImage2KHR(commandBuffer, pCopyBufferToImageInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, VkCopyImageToBufferInfo2* pCopyImageToBufferInfo ) {
scoped_lock l(global_lock);
#ifdef CMDCOPYIMAGETOBUFFER2KHR_EXISTS
layer_CmdCopyImageToBuffer2KHR(commandBuffer, pCopyImageToBufferInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdCopyImageToBuffer2KHR(commandBuffer, pCopyImageToBufferInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdResolveImage2KHR(VkCommandBuffer commandBuffer, VkResolveImageInfo2* pResolveImageInfo ) {
scoped_lock l(global_lock);
#ifdef CMDRESOLVEIMAGE2KHR_EXISTS
layer_CmdResolveImage2KHR(commandBuffer, pResolveImageInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdResolveImage2KHR(commandBuffer, pResolveImageInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdSetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, VkDependencyInfo* pDependencyInfo ) {
scoped_lock l(global_lock);
#ifdef CMDSETEVENT2KHR_EXISTS
layer_CmdSetEvent2KHR(commandBuffer, event, pDependencyInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdSetEvent2KHR(commandBuffer, event, pDependencyInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdResetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask ) {
scoped_lock l(global_lock);
#ifdef CMDRESETEVENT2KHR_EXISTS
layer_CmdResetEvent2KHR(commandBuffer, event, stageMask);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdResetEvent2KHR(commandBuffer, event, stageMask);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWaitEvents2KHR(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent* pEvents, VkDependencyInfo* pDependencyInfos ) {
scoped_lock l(global_lock);
#ifdef CMDWAITEVENTS2KHR_EXISTS
layer_CmdWaitEvents2KHR(commandBuffer, eventCount, pEvents, pDependencyInfos);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWaitEvents2KHR(commandBuffer, eventCount, pEvents, pDependencyInfos);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdPipelineBarrier2KHR(VkCommandBuffer commandBuffer, VkDependencyInfo* pDependencyInfo ) {
scoped_lock l(global_lock);
#ifdef CMDPIPELINEBARRIER2KHR_EXISTS
layer_CmdPipelineBarrier2KHR(commandBuffer, pDependencyInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdPipelineBarrier2KHR(commandBuffer, pDependencyInfo);
}

 VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_QueueSubmit2KHR(VkQueue queue, uint32_t submitCount, VkSubmitInfo2* pSubmits, VkFence fence ) {
scoped_lock l(global_lock);
#ifdef QUEUESUBMIT2KHR_EXISTS
layer_QueueSubmit2KHR(queue, submitCount, pSubmits, fence);
#endif 
device_dispatch[GetKey(queue)].QueueSubmit2KHR(queue, submitCount, pSubmits, fence);
return VK_SUCCESS;
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdWriteTimestamp2KHR(VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query ) {
scoped_lock l(global_lock);
#ifdef CMDWRITETIMESTAMP2KHR_EXISTS
layer_CmdWriteTimestamp2KHR(commandBuffer, stage, queryPool, query);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdWriteTimestamp2KHR(commandBuffer, stage, queryPool, query);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdBeginRenderingKHR(VkCommandBuffer commandBuffer, VkRenderingInfo* pRenderingInfo ) {
scoped_lock l(global_lock);
#ifdef CMDBEGINRENDERINGKHR_EXISTS
layer_CmdBeginRenderingKHR(commandBuffer, pRenderingInfo);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdBeginRenderingKHR(commandBuffer, pRenderingInfo);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_CmdEndRenderingKHR(VkCommandBuffer commandBuffer ) {
scoped_lock l(global_lock);
#ifdef CMDENDRENDERINGKHR_EXISTS
layer_CmdEndRenderingKHR(commandBuffer);
#endif 
device_dispatch[GetKey(commandBuffer)].CmdEndRenderingKHR(commandBuffer);
}

 VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetImageSubresourceLayout2EXT(VkDevice device, VkImage image, VkImageSubresource2KHR* pSubresource, VkSubresourceLayout2KHR* pLayout ) {
scoped_lock l(global_lock);
#ifdef GETIMAGESUBRESOURCELAYOUT2EXT_EXISTS
layer_GetImageSubresourceLayout2EXT(device, image, pSubresource, pLayout);
#endif 
device_dispatch[GetKey(device)].GetImageSubresourceLayout2EXT(device, image, pSubresource, pLayout);
}
#define GETPROCADDR(func) if(!strcmp(pName, "vk" #func)) return (PFN_vkVoidFunction)&DetailsLayer_##func;
VK_LAYER_EXPORT PFN_vkVoidFunction VKAPI_CALL DetailsLayer_GetDeviceProcAddr(VkDevice device, const char* pName) {
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

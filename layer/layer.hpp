#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vk_layer.h>
#include <winsock2.h>
#include <string>
#include <map>

//#include "../appUI/winsock.h"

#define CREATEINSTANCE_EXISTS
void layer_CreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);

#define CREATEBUFFER_EXISTS
void layer_CreateBuffer(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer);

#define CMDDRAW_EXISTS
void layer_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);

#define MAPMEMORY_EXISTS
void layer_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData);

#define DESTROYBUFFER_EXISTS
void layer_DestroyBuffer(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator);

#define UNMAPMEMORY_EXISTS
void layer_UnmapMemory(VkDevice device, VkDeviceMemory memory);

#define BINDBUFFERMEMORY_EXISTS
void layer_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);

#define CREATEIMAGE_EXISTS
void layer_CreateImage(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage);

#define BINDIMAGEMEMORY_EXISTS
void layer_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);

#define DESTROYIMAGE_EXISTS
void layer_DestroyImage(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator);

#define ALLOCATECOMMANDBUFFERS_EXISTS
void layer_AllocateCommandBuffers(VkDevice device, VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);

#define FREECOMMANDBUFFERS_EXISTS
void layer_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers);

#define BEGINCOMMANDBUFFER_EXISTS
void layer_BeginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo* pBeginInfo);

#define ENDCOMMANDBUFFER_EXISTS
void layer_EndCommandBuffer(VkCommandBuffer commandBuffer);

#define RESETCOMMANDBUFFER_EXISTS
void layer_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags);

#define QUEUESUBMIT_EXISTS
void layer_QueueSubmit(VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence);

//#pragma comment(lib, "Ws2_32.lib")
#define WINDOW_NAME "VK_DEBUGGER"
#define DEFAULT_BUFLEN 512
#define VK_LAYER_EXPORT extern "C" __declspec(dllexport)
#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vk_layer.h>
#include <winsock2.h>
#include "../appUI/winsock.h"
#include <string>
#include <map>
#include <thread>
#include <windows.h>
#include <filesystem>


extern SOCKET ConnectSocket;
extern bool connected;


//#include "../appUI/winsock.h"

std::string GetWindowName();

#define CREATEINSTANCE_BEFORE_EXEC_EXISTS
void layer_CreateInstance_before(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);

#define CREATEBUFFER_BEFORE_EXEC_EXISTS
void layer_CreateBuffer_before(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer);

//#define CMDDRAW_EXISTS
void layer_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);

#define MAPMEMORY_AFTER_EXEC_EXISTS
void layer_MapMemory_after(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData);

#define UNMAPMEMORY_BEFORE_EXEC_EXISTS
void layer_UnmapMemory_before(VkDevice device, VkDeviceMemory memory);

//#define DESTROYBUFFER_EXISTS
void layer_DestroyBuffer(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator);

//#define BINDBUFFERMEMORY_EXISTS
void layer_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);

#define CREATEIMAGE_BEFORE_EXEC_EXISTS
void layer_CreateImage_before(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage);

//#define BINDIMAGEMEMORY_EXISTS
void layer_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);

//#define DESTROYIMAGE_EXISTS
void layer_DestroyImage(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator);

#define ALLOCATECOMMANDBUFFERS_BEFORE_EXEC_EXISTS
void layer_AllocateCommandBuffers_before(VkDevice device, VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);

//#define FREECOMMANDBUFFERS_EXISTS
void layer_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers);

//#define BEGINCOMMANDBUFFER_EXISTS
void layer_BeginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo* pBeginInfo);

//#define ENDCOMMANDBUFFER_EXISTS
void layer_EndCommandBuffer(VkCommandBuffer commandBuffer);

//#define RESETCOMMANDBUFFER_EXISTS
void layer_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags);

//#define QUEUESUBMIT_EXISTS
void layer_QueueSubmit(VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence);

//#pragma comment(lib, "Ws2_32.lib")
#define WINDOW_NAME "VK_DEBUGGER"
#define DEFAULT_BUFLEN 512
#define VK_LAYER_EXPORT extern "C" __declspec(dllexport)
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

std::string GetWindowName();

#define FREEMEMORY_BEFORE_EXEC_EXISTS
void layer_FreeMemory_before(VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks* pAllocator);

#define BINDIMAGEMEMORY_AFTER_EXEC_EXISTS
void layer_BindImageMemory_after(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);

#define BINDBUFFERMEMORY_AFTER_EXEC_EXISTS
void layer_BindBufferMemory_after(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);

#define ALLOCATEMEMORY_AFTER_EXEC_EXISTS
void layer_AllocateMemory_after(VkDevice device, VkMemoryAllocateInfo* pAllocateInfo, VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory);

#define CREATEBUFFER_AFTER_EXEC_EXISTS
void layer_CreateBuffer_after(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer);
#define DESTROYBUFFER_BEFORE_EXEC_EXISTS
void layer_DestroyBuffer_before(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator);

#define MAPMEMORY_AFTER_EXEC_EXISTS
void layer_MapMemory_after(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData);

#define UNMAPMEMORY_BEFORE_EXEC_EXISTS
void layer_UnmapMemory_before(VkDevice device, VkDeviceMemory memory);

#define CREATEIMAGE_AFTER_EXEC_EXISTS
void layer_CreateImage_after(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage);
#define DESTROYIMAGE_BEFORE_EXEC_EXISTS
void layer_DestroyImage_before(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator);

#define CMDCOPYBUFFERTOIMAGE_BEFORE_EXEC_EXISTS
void layer_CmdCopyBufferToImage_before(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkBufferImageCopy* pRegions);

#define CMDCOPYIMAGETOBUFFER_BEFORE_EXEC_EXISTS
void layer_CmdCopyImageToBuffer_before(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, VkBufferImageCopy* pRegions);

#define CMDCOPYBUFFER_BEFORE_EXEC_EXISTS
void layer_CmdCopyBuffer_before(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy* pRegions);

#define CMDCOPYIMAGE_BEFORE_EXEC_EXISTS
void layer_CmdCopyImage_before(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageCopy* pRegions);


//#pragma comment(lib, "Ws2_32.lib")
#define WINDOW_NAME "VK_DEBUGGER"
#define DEFAULT_BUFLEN 512
#define VK_LAYER_EXPORT extern "C" __declspec(dllexport)
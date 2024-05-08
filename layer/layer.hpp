/*
* Name		    : layer_data.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file to store the current state of the Vulkan constructs and perform operations on them
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vk_layer.h>
#include "../appUI/winsock.h"
#include <string>
#include <map>
#include <filesystem>
#include <windows.h>
#include "layer_messages.h"

extern SOCKET ConnectSocket;
extern bool connected;
extern bool skipLock;
extern bool callAtBreak;
extern bool callEveryBreak;

void layer_newCall();

std::string GetWindowName();
void layer_SetEnvVariables();

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

#define CREATEINSTANCE_AFTER_EXEC_EXISTS
void layer_CreateInstance_after(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);

#define ACQUIRENEXTIMAGEKHR_BEFORE_EXEC_EXISTS
void layer_AcquireNextImageKHR_before(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex);

#define QUEUESUBMIT_AFTER_EXEC_EXISTS
void layer_QueueSubmit_after(VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence);

#define GETPHYSICALDEVICESURFACESUPPORTKHR_BEFORE_EXEC_EXISTS
void layer_GetPhysicalDeviceSurfaceSupportKHR_before(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);

#define CREATECOMMANDPOOL_AFTER_EXEC_EXISTS
void layer_CreateCommandPool_after(VkDevice device, VkCommandPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool);

#define CREATEDEVICE_AFTER_EXEC_EXISTS
void layer_CreateDevice_after(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);

#define CMDDRAW_AFTER_EXEC_EXISTS
void layer_CmdDraw_after(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);

void layer_AppStarter(std::string configContent);

//#pragma comment(lib, "Ws2_32.lib")
#define WINDOW_NAME "VK_DEBUGGER"
#define DEFAULT_BUFLEN 512
#define VK_LAYER_EXPORT extern "C" __declspec(dllexport)
/*
* Name		    : layer.h
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file to implement extra behavior to a set of chosen Vulkan functions
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
#include <iostream>
#include <cstring>
#include "layer_messages.h"


#ifdef __linux__
  
#elifdef __WIN32__
  #include <windows.h>
#endif

/* this layer's socket */
extern SOCKET ConnectSocket;
/* connected bool and skipLock makes it possible to use Vulkan functions right on the layer */
extern bool connected;
extern bool skipLock;
/* enables the counting of Vulkan calls to ensure the breakpoint */
extern bool callAtBreak;
extern bool callEveryBreak;
/* breakpoint based on the counting of Vulkan function calls */
void layer_newCall();
/* get current's application name */
std::string GetWindowName();
/* extracts the starting parameters out of 'VkConfig' */
void layer_SetEnvVariables();

#define WINDOW_NAME "VkDebugger.exe"
/* starts the new VkDebuggerApp in case the .exe is found */
void layer_AppStarter(std::string configContent);


#ifdef __linux__
  #define VK_LAYER_EXPORT extern "C" __attribute((visibility("default")))
  #define VKAPI_CALL
  #define strcpy_s strcpy
  #define MAX_PATH 1024
#elifdef __WIN32__
  /* macro to ensure the export of these functions to a .dll */
  #define VK_LAYER_EXPORT extern "C" __declspec(dllexport)
#endif


/* checks for the frame warn and breakpoint conditions */
void frameWarnBreak();
/* 
* corresponding define and function call enable the extra functionality for that chosen Vulkan function 
* before_exec_exists and after_exec_exists decide whether this behavior is on the way down the dispatch chain
* of up back to the source program
*/
#define FREEMEMORY_BEFORE_EXEC_EXISTS
/* free VkDeviceMemory, invalidate its reference, inform the VkDebuggerApp about it too */
void layer_FreeMemory_before(VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks* pAllocator);

#define BINDIMAGEMEMORY_AFTER_EXEC_EXISTS
/* change state of the relationship between the VkImage and VkDeviceMemory, inform the VkDebuggerApp about it too */
void layer_BindImageMemory_after(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);

#define BINDBUFFERMEMORY_AFTER_EXEC_EXISTS
/* change state of the relationship between the VkBuffer and VkDeviceMemory, inform the VkDebuggerApp about it too */
void layer_BindBufferMemory_after(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);

#define ALLOCATEMEMORY_AFTER_EXEC_EXISTS
/* create a new VkDeviceMemory, save if as an VkMemoryObject and inform VkDebuggerApp */
void layer_AllocateMemory_after(VkDevice device, VkMemoryAllocateInfo* pAllocateInfo, VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory);

#define CREATEBUFFER_AFTER_EXEC_EXISTS
/* create a new VkBuffer, save if as an VkBufferObject and inform VkDebuggerApp */
void layer_CreateBuffer_after(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer);

#define CREATEIMAGE_AFTER_EXEC_EXISTS
/* create a new VkImage, save if as an VkImageObject and inform VkDebuggerApp */
void layer_CreateImage_after(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage);

#define DESTROYBUFFER_BEFORE_EXEC_EXISTS
/* free VkBuffer, invalidate its reference, inform the VkDebuggerApp about it too */
void layer_DestroyBuffer_before(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator);

#define DESTROYIMAGE_BEFORE_EXEC_EXISTS
/* free VkImage, invalidate its reference, inform the VkDebuggerApp about it too */
void layer_DestroyImage_before(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator);

#define MAPMEMORY_AFTER_EXEC_EXISTS
/* maps the CPU ppData to the VkDeviceMemory, set this relationship between these objects along with size and offset */
void layer_MapMemory_after(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData);

#define UNMAPMEMORY_BEFORE_EXEC_EXISTS
/* unmaps the CPU ppData to the VkDeviceMemory, data read from ppData is no longer relevant and shouldn't be read from */
void layer_UnmapMemory_before(VkDevice device, VkDeviceMemory memory);

#define CMDCOPYBUFFERTOIMAGE_BEFORE_EXEC_EXISTS
/* set this relationship between VkImage, VkBuffer and copying the data in the VkDebuggerApp */
void layer_CmdCopyBufferToImage_before(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkBufferImageCopy* pRegions);

#define CMDCOPYIMAGETOBUFFER_BEFORE_EXEC_EXISTS
/* set this relationship between VkImage, VkBuffer and copying the data in the VkDebuggerApp */
void layer_CmdCopyImageToBuffer_before(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, VkBufferImageCopy* pRegions);

#define CMDCOPYBUFFER_BEFORE_EXEC_EXISTS
/* set this relationship between them and copying the data in the VkDebuggerApp */
void layer_CmdCopyBuffer_before(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy* pRegions);

#define CMDCOPYIMAGE_BEFORE_EXEC_EXISTS
/* set this relationship between them and copying the data in the VkDebuggerApp */
void layer_CmdCopyImage_before(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageCopy* pRegions);

#define CREATEINSTANCE_AFTER_EXEC_EXISTS
/* creates a new data object, stores the instance for use later */
void layer_CreateInstance_after(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);

#define CREATEDEVICE_AFTER_EXEC_EXISTS
/* gets the reference to currently used device so it can be used inside the image/buffer data reader */
void layer_CreateDevice_after(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);

#define ACQUIRENEXTIMAGEKHR_BEFORE_EXEC_EXISTS
/* potential frame delimiter function, ensure the breakpoints and warnings are counted in case it is a frame delimiter function */
void layer_AcquireNextImageKHR_before(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex);

#define QUEUESUBMIT_AFTER_EXEC_EXISTS
/* potential frame delimiter function, ensure the breakpoints and warnings are counted in case it is a frame delimiter function */
void layer_QueueSubmit_after(VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence);

#define QUEUEPRESENTKHR_BEFORE_EXEC_EXISTS
/* 
* potential frame delimiter function, ensure the breakpoints and warnings are counted in case it is a frame delimiter function,
* also sends the VkMemory data of their corresponding bound VkBuffer and VkImage objects
*/
void layer_QueuePresentKHR_before(VkQueue queue, VkPresentInfoKHR* pPresentInfo);

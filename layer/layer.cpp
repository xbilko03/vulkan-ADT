/*
* Name		: appUI.cpp
* Project	: A Debugging Tool for Vulkan API
*
* Based on  : Baldur Karlsson (baldurk) and Johannes Kuhlmann's (jkuhlmann) sample_layer
* https://github.com/baldurk/sample_layer/blob/master
* Minor changes : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/

#include "layer.hpp"
#include "../appUI/winsock.h"

#include <windows.h>
#include <iostream>

#include <assert.h>
#include <filesystem>
#include <array>

#include <stdlib.h> //atexit
#include <mutex> //scoped lock
#include <chrono> //sleep
#include <thread>

SOCKET ConnectSocket = INVALID_SOCKET;
bool connected = false;

void layer_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
{
    if (connected)
    {
        //std::cout << "mapping ID -> [memory]: " << memory << std::endl;
        //winsockSendToUI(&ConnectSocket, "VkDeviceSize:" + std::to_string(size) + s);
    }
}

void layer_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    if (connected) 
    {
        //std::cout << std::endl;
        //winsockSendToUI(&ConnectSocket, "CmdDraw");
    }
}

void layer_UnmapMemory(VkDevice device, VkDeviceMemory memory) 
{
    if (connected)
    {
        //std::cout << "unmapping ID -> [memory]: " << memory << std::endl;
    }
}


std::mutex test_lock;
typedef std::lock_guard<std::mutex> scoped_lock;

/* VkImage */
void layer_CreateImage(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
    if (connected)
    {
        /* VkImageCreateInfo
        std::cout << pCreateInfo->arrayLayers << std::endl;
        std::cout << pCreateInfo->extent.width << std::endl;
        std::cout << pCreateInfo->extent.height << std::endl;
        std::cout << pCreateInfo->extent.depth << std::endl;
        std::cout << pCreateInfo->flags << std::endl;
        std::cout << pCreateInfo->format << std::endl;
        std::cout << pCreateInfo->imageType << std::endl;
        std::cout << pCreateInfo->initialLayout << std::endl;
        std::cout << pCreateInfo->mipLevels << std::endl;
        std::cout << pCreateInfo->pNext << std::endl;
        std::cout << pCreateInfo->pQueueFamilyIndices << std::endl;
        std::cout << pCreateInfo->queueFamilyIndexCount << std::endl;
        std::cout << pCreateInfo->samples << std::endl;
        std::cout << pCreateInfo->sharingMode << std::endl;
        std::cout << pCreateInfo->sType << std::endl;
        std::cout << pCreateInfo->tiling << std::endl;
        std::cout << pCreateInfo->usage << std::endl;
        */
        //std::cout << "create image -> [VkImage*] " << pImage << std::endl;
    }
}
void layer_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    if (connected)
    {
        //std::cout << "bind image -> [VkImage] " << image << std::endl;
        //std::cout << "bind image -> [VkDeviceMemory] " << memory << std::endl;
    }
}
void layer_DestroyImage(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator)
{
    if (connected)
    {
        //std::cout << "destroy image -> [VkImage] " << image << std::endl;
    }
}
/* VkImage */

/* VkCommandBuffer */
void layer_QueueSubmit(VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence)
{
    if (connected)
    {
        //std::cout << "submit cmdBuff -> [pCommandBuffers*]" << std::hex << *(pSubmits->pCommandBuffers) << std::endl;
    }
}
void layer_AllocateCommandBuffers(VkDevice device, VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{
    if (connected)
    {
        //std::cout << pAllocateInfo->commandBufferCount << std::endl;
        /*
        std::cout << pAllocateInfo->commandPool << std::endl;
        std::cout << pAllocateInfo->level << std::endl;
        std::cout << pAllocateInfo->pNext << std::endl;
        std::cout << pAllocateInfo->sType << std::endl;
        */
        //std::cout << "allocate before cmdBuff -> [VkCommandBuffer*]" << *pCommandBuffers << std::endl;
    }
}
void layer_BeginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo* pBeginInfo)
{
    if (connected)
    {
        //std::cout << "begin cmdBuff -> [VkCommandBuffer]" << commandBuffer << std::endl;
    }
}
void layer_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags)
{
    if (connected)
    {
        //std::cout << "reset cmdBuff -> [VkCommandBuffer]" << commandBuffer << std::endl;
    }
}
void layer_EndCommandBuffer(VkCommandBuffer commandBuffer)
{
    if (connected)
    {
        //std::cout << "end cmdBuff -> [VkCommandBuffer]" << commandBuffer << std::endl;
    }
}
void layer_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers)
{
    if (connected)
    {
        //std::cout << "free cmdBuff -> [VkCommandBuffer*]" << *pCommandBuffers << std::endl;
        //std::cout << "free cmdBuff -> [VkCommandPool]" << commandPool << std::endl;
    }
}
/* VkCommandBuffer */





/* VkBuffer */
void layer_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    if (connected)
    {
        //std::cout << "binding ID -> [buffer]:" << buffer << std::endl;
        //std::cout << "binding ID -> [memory]:" << memory << std::endl;
    }
}
void layer_CreateBuffer(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
    if (connected)
    {
        //std::cout << "createBuffer ID -> [buffer]: " << pBuffer << std::endl;

        /* create info
        std::cout << pCreateInfo->sType << std::endl;
        std::cout << pCreateInfo->pNext << std::endl;
        std::cout << pCreateInfo->flags << std::endl;
        std::cout << pCreateInfo->size << std::endl;
        std::cout << pCreateInfo->usage << std::endl;
        std::cout << pCreateInfo->sharingMode << std::endl;
        std::cout << pCreateInfo->queueFamilyIndexCount << std::endl;
        std::cout << pCreateInfo->pQueueFamilyIndices << std::endl;
        */

        //winsockSendToUI(&ConnectSocket, "created buffer");
    }
}
void layer_DestroyBuffer(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator)
{
    if (connected)
    {
        //std::cout << "DestroyBuffer -> buffer ID -> [buffer]: " << buffer << std::endl << std::endl;
    }
}
/* VkBuffer */




void layer_CreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    /* Start UI */
    /* If there is a running window already, do nothing. */

    STARTUPINFO info = { sizeof(info) };
    PROCESS_INFORMATION processInfo;

    /* windows only: */
    char buf[MAX_PATH];
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    std::filesystem::path progPath(buf);
    std::string filename = progPath.filename().string();

    if (strcmp(filename.c_str(), "vkDetails.exe") != 0)
    {
        /* prevent opening vkDetails on appUI startup */
        CreateProcess("C:\\Users\\jozef\\Desktop\\vk details\\out\\build\\x64-debug\\vkDetails.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &info, &processInfo);
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);

        /* Connect to UI */
        if (layerWinsockInit(&ConnectSocket) == 0)
        {
            connected = true;
        }
        else
            connected = false;

        if (connected)
        {
            //winsockSendToUI(&ConnectSocket, "vkCreateInstance");
        }
    }
    /* windows only */
}
/*
* Name		: appUI.cpp
* Project	: A Debugging Tool for Vulkan API
*
* Based on  : Baldur Karlsson (baldurk) and Johannes Kuhlmann's (jkuhlmann) sample_layer
* https://github.com/baldurk/sample_layer/blob/master
* Minor changes : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/

#include "layer.hpp"
#include <iostream>
#include <assert.h>
#include <array>
#include <mutex> //scoped lock
#include <sstream> //scoped lock

SOCKET ConnectSocket = INVALID_SOCKET;
void* map;
uint64_t image_size;
void layer_MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
{
        std::cout << "mapping ID -> [memory]: " << memory << std::endl;
        map = *ppData;
        image_size = size;
        // data is 0 after the command is called
        
        //winsockSendToUI(&ConnectSocket, "VkDeviceSize:" + std::to_string(size) + s);
}

void layer_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
        //std::cout << std::endl;
        //winsockSendToUI(&ConnectSocket, "CmdDraw");
}

void layer_UnmapMemory(VkDevice device, VkDeviceMemory memory) 
{
        int i = 0;
        unsigned char* c = (unsigned char*)map;

        while (i != image_size)
            printf("%02x ", c[i++]);
        //std::cout << "unmapping ID -> [memory]: " << memory << std::endl;
}

/* VkImage */
void layer_CreateImage(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage)
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
void layer_BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
        //std::cout << "bind image -> [VkImage] " << image << std::endl;
        //std::cout << "bind image -> [VkDeviceMemory] " << memory << std::endl;
}
void layer_DestroyImage(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator)
{
        //std::cout << "destroy image -> [VkImage] " << image << std::endl;
}
/* VkImage */

/* VkCommandBuffer */
void layer_QueueSubmit(VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence)
{
        //std::cout << "submit cmdBuff -> [pCommandBuffers*]" << std::hex << *(pSubmits->pCommandBuffers) << std::endl;
}
void layer_AllocateCommandBuffers(VkDevice device, VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
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
void layer_BeginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo* pBeginInfo)
{
        //std::cout << "begin cmdBuff -> [VkCommandBuffer]" << commandBuffer << std::endl;
}
void layer_ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags)
{
        //std::cout << "reset cmdBuff -> [VkCommandBuffer]" << commandBuffer << std::endl;
}
void layer_EndCommandBuffer(VkCommandBuffer commandBuffer)
{
        //std::cout << "end cmdBuff -> [VkCommandBuffer]" << commandBuffer << std::endl;
}
void layer_FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers)
{
        //std::cout << "free cmdBuff -> [VkCommandBuffer*]" << *pCommandBuffers << std::endl;
        //std::cout << "free cmdBuff -> [VkCommandPool]" << commandPool << std::endl;
}
/* VkCommandBuffer */



std::string ptrToString(auto* input)
{
    std::stringstream s;
    s << *input;
    return s.str();
}

/* VkBuffer */
void layer_BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
        //std::cout << "binding ID -> [buffer]:" << buffer << std::endl;
        //std::cout << "binding ID -> [memory]:" << memory << std::endl;
}
void layer_CreateBuffer_before(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
    std::string output = "bufferID=" + ptrToString(&pBuffer) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "sType=" + std::to_string(pCreateInfo->sType) + '!';
    winsockSendToUI(&ConnectSocket, output);
    
    output = "pNext=" + ptrToString(&(pCreateInfo->pNext)) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "flags=" + std::to_string(pCreateInfo->flags) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "size=" + std::to_string(pCreateInfo->size) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "usage=" + std::to_string(pCreateInfo->usage) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "sharingMode=" + std::to_string(pCreateInfo->sharingMode) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "queueFamilyIndexCount=" + std::to_string(pCreateInfo->queueFamilyIndexCount) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "pQueueFamilyIndices=" + ptrToString(&(pCreateInfo->pQueueFamilyIndices)) + '!';
    winsockSendToUI(&ConnectSocket, output);
}
void layer_DestroyBuffer(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator)
{
        //std::cout << "DestroyBuffer -> buffer ID -> [buffer]: " << buffer << std::endl << std::endl;
}
/* VkBuffer */




void layer_CreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    /* windows only */
}
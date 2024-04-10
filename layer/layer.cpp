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
#include <sstream> //string stream
#include <map> //map

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

SOCKET ConnectSocket = INVALID_SOCKET;
void* map;
uint64_t image_size;

/* Helper Functions */
std::string ptrToString(auto* input)
{
    if (input == NULL)
        return "NULL";
    std::stringstream s;
    s << *input;
    return s.str();
}

/* VkMemory */
typedef struct memoryObj {
    VkDeviceSize size;
    VkBuffer boundBuffer;
    VkImage boundImage;
    void** data;
};
std::map<VkDeviceMemory, memoryObj> memoryMap;
void layer_AllocateMemory_after(VkDevice device, VkMemoryAllocateInfo* pAllocateInfo, VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
{
    std::string output = "pMemory=" + ptrToString(pMemory) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "sType=" + std::to_string((*pAllocateInfo).sType) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "pNext=" + ptrToString(&(*pAllocateInfo).pNext) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "allocationSize=" + std::to_string((*pAllocateInfo).allocationSize) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "memoryTypeIndex=" + std::to_string((*pAllocateInfo).memoryTypeIndex) + '!';
    winsockSendToUI(&ConnectSocket, output);

    /* map memory to memoryObj */
    memoryMap[*pMemory] = {};
    memoryMap[*pMemory] = memoryObj((*pAllocateInfo).allocationSize, 0, 0, 0);

}
void layer_BindBufferMemory_after(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    auto tarObject = memoryMap[memory];
    tarObject.boundBuffer = buffer;

    std::string output = "memory=" + ptrToString(&memory) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "buffer=" + ptrToString(&buffer) + '!';
    winsockSendToUI(&ConnectSocket, output);
}
void layer_BindImageMemory_after(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    auto tarObject = memoryMap[memory];
    tarObject.boundImage = image;

    std::string output = "memory=" + ptrToString(&memory) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "image=" + ptrToString(&image) + '!';
    winsockSendToUI(&ConnectSocket, output);
}

void layer_MapMemory_after(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
{
    auto tarObject = memoryMap[memory];
    if (tarObject.size > size)
        tarObject.size = size;
    tarObject.data = ppData;
    memoryMap[memory] = tarObject;
}

#include <fstream>


std::string ToHex(const std::string& s, bool upper_case /* = true */)
{
    std::ostringstream ret;

    for (std::string::size_type i = 0; i < s.length(); ++i)
        ret << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << (int)s[i];

    return ret.str();
}
void layer_UnmapMemory_before(VkDevice device, VkDeviceMemory memory)
{
    auto tarObject = memoryMap[memory];
    //void* cpuData = (void*)malloc(tarObject.size);
    //memcpy(cpuData, *tarObject.data, tarObject.size);

    /*
    std::ofstream file;
    file.open("C:\\Users\\jozef\\Desktop\\" + ptrToString(&cpuData), std::ios_base::binary);
    assert(file.is_open());
    file.write((const char*)cpuData, tarObject.size);
    file.close();
    */

    

    //std::string s(static_cast<char*>(*tarObject.data), static_cast<char*>(*tarObject.data) + tarObject.size);
    //std::cout << "outputData = '" << s << "'" << std::endl;
    //return;
    int* cpuData = (int*)*tarObject.data;
    std::string output = "";
    for (unsigned i = 0; i < tarObject.size / sizeof(int); i++)
    {
        output += std::to_string((cpuData[i]));
        output += '!';
    }

    std::string dataMessage = "data";
    dataMessage += std::to_string(output.size());
    //dataMessage += std::to_string();
    dataMessage += '!';
    winsockSendToUI(&ConnectSocket, dataMessage);
    winsockSendToUI(&ConnectSocket, output);

    /* Catch data before they are invalidated */
    //std::cout << "raw data try = " << tarObject.data << std::endl;
}


/* Init */
void layer_CreateInstance_before(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    std::string output = "pid=" + std::to_string(getpid()) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "windowName=" + GetWindowName() + '!';
    winsockSendToUI(&ConnectSocket, output);
}

/* VkImage */
void layer_CreateImage_before(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
    std::string output = "arrayLayers=" + std::to_string((*pCreateInfo).arrayLayers) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "extent.width=" + std::to_string((*pCreateInfo).extent.width) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "extent.height=" + std::to_string((*pCreateInfo).extent.height) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "extent.depth=" + std::to_string((*pCreateInfo).extent.depth) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "flags=" + std::to_string((*pCreateInfo).flags) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "format=" + std::to_string((*pCreateInfo).format) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "imageType=" + std::to_string((*pCreateInfo).imageType) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "initialLayout=" + std::to_string((*pCreateInfo).initialLayout) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "mipLevels=" + std::to_string((*pCreateInfo).mipLevels) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "pNext=" + ptrToString(&((*pCreateInfo).pNext)) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "queueFamilyIndexCount=" + std::to_string((*pCreateInfo).queueFamilyIndexCount) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "pQueueFamilyIndices=" + ptrToString(&((*pCreateInfo).pQueueFamilyIndices)) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "samples=" + std::to_string((*pCreateInfo).samples) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "sharingMode=" + std::to_string((*pCreateInfo).sharingMode) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "sType=" + std::to_string((*pCreateInfo).sType) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "tiling=" + std::to_string((*pCreateInfo).tiling) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "usage=" + std::to_string((*pCreateInfo).usage) + '!';
    winsockSendToUI(&ConnectSocket, output);
}

/* VkCommandBuffer */
void layer_AllocateCommandBuffers_after(VkDevice device, VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{
    std::string output = "pCommandBuffers=" + ptrToString(pCommandBuffers) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "commandBufferCount=" + std::to_string((*pAllocateInfo).commandBufferCount) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "arrayLayers=" + std::to_string((*pAllocateInfo).commandBufferCount) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "commandPool=" + ptrToString(&((*pAllocateInfo).commandPool)) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "level=" + std::to_string((*pAllocateInfo).level) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "sType=" + std::to_string((*pAllocateInfo).sType) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "pNext=" + ptrToString(&((*pAllocateInfo).pNext)) + '!';
    winsockSendToUI(&ConnectSocket, output);
}

/* VkBuffer */
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

/* Unused */
void layer_QueueSubmit(VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence)
{
    //std::cout << "submit cmdBuff -> [pCommandBuffers*]" << std::hex << *(pSubmits->pCommandBuffers) << std::endl;
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
void layer_DestroyBuffer(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator)
{
        //std::cout << "DestroyBuffer -> buffer ID -> [buffer]: " << buffer << std::endl << std::endl;
}
void layer_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    //std::cout << std::endl;
    //winsockSendToUI(&ConnectSocket, "CmdDraw");
}
void layer_DestroyImage(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator)
{
    //std::cout << "destroy image -> [VkImage] " << image << std::endl;
}


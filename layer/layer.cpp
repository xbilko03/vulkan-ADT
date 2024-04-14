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

#define CUSTOM_PARAM_PREFIX "layer_"
#define CUSTOM_DATA_PREFIX "data_"


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
std::string addrToString(auto input)
{
    if (input == NULL)
        return "NULL";
    std::stringstream s;
    s << input;
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
/* Create memory object */
void layer_AllocateMemory_after(VkDevice device, VkMemoryAllocateInfo* pAllocateInfo, VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
{
    /* map memory to memoryObj */
    memoryMap[*pMemory] = {};
    memoryMap[*pMemory] = memoryObj((*pAllocateInfo).allocationSize, 0, 0, 0);
    std::string output = CUSTOM_PARAM_PREFIX;
    output += "ptr=";
    output += ptrToString(pMemory);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);
}
/* Free memory object */
void layer_FreeMemory_before(VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks* pAllocator)
{
    /* map memory to memoryObj */
    memoryMap[memory] = {};
    std::string output = CUSTOM_PARAM_PREFIX;
    output += "ptr=";
    output += addrToString(memory);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);

}

void layer_BindBufferMemory_after(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    auto tarObject = memoryMap[memory];
    tarObject.boundBuffer = buffer;

    std::string output = CUSTOM_PARAM_PREFIX;
    output += "memPtr=";
    output += addrToString(memory);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);

    output = CUSTOM_PARAM_PREFIX;
    output += "bufPtr=";
    output += addrToString(buffer);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);
}
void layer_BindImageMemory_after(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    auto tarObject = memoryMap[memory];
    tarObject.boundImage = image;

    std::string output = CUSTOM_PARAM_PREFIX;
    output += "memPtr=";
    output += addrToString(memory);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);

    output = CUSTOM_PARAM_PREFIX;
    output += "imgPtr=";
    output += addrToString(image);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);
}
/* Get local pointer to memory data */
void layer_MapMemory_after(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
{
    auto tarObject = memoryMap[memory];
    if (tarObject.size > size)
        tarObject.size = size;
    tarObject.data = ppData;
    memoryMap[memory] = tarObject;
}
/* Get data from the local pointer */
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
    std::string memAddrSend = CUSTOM_PARAM_PREFIX;
    memAddrSend += "ptr=";
    memAddrSend += addrToString(memory);
    memAddrSend += '!';
    winsockSendToUI(&ConnectSocket, memAddrSend);
        
    std::string dataMessage = "data";
    dataMessage += std::to_string(output.size());
    dataMessage += '!';
    winsockSendToUI(&ConnectSocket, dataMessage);
    winsockSendToUI(&ConnectSocket, output);
    //winsockSendToUIraw(&ConnectSocket, (char*)*tarObject.data, tarObject.size);

    /* Catch data before they are invalidated */
    //std::cout << "raw data try = " << tarObject.data << std::endl;
}
/* Create Image object */
void layer_CreateImage_after(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
    std::string output = CUSTOM_PARAM_PREFIX;
    output += "ptr=";
    output += ptrToString(pImage);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);
}
/* Create buffer object */
void layer_CreateBuffer_after(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
    std::string output = CUSTOM_PARAM_PREFIX;
    output += "ptr=";
    output += ptrToString(pBuffer);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);
}
/* Free image object */
void layer_DestroyImage_before(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator)
{
    std::string output = CUSTOM_PARAM_PREFIX;
    output += "ptr=";
    output += addrToString(image);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);
}
/* Free buffer object */
void layer_DestroyBuffer_before(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator)
{
    std::string output = CUSTOM_PARAM_PREFIX;
    output += "ptr=";
    output += addrToString(buffer);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);
}
/* copy image data to buffer */
void layer_CmdCopyImageToBuffer_before(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, VkBufferImageCopy* pRegions)
{
    std::string output = CUSTOM_PARAM_PREFIX;
    output += "dstBuf=";
    output += addrToString(dstBuffer);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);

    output = CUSTOM_PARAM_PREFIX;
    output += "srcImg=";
    output += addrToString(srcImage);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);
}
/* copy buffer data to image */
void layer_CmdCopyBufferToImage_before(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkBufferImageCopy* pRegions)
{
    std::string output = CUSTOM_PARAM_PREFIX;
    output += "srcBuf=";
    output += addrToString(srcBuffer);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);

    output = CUSTOM_PARAM_PREFIX;
    output += "dstImg=";
    output += addrToString(dstImage);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);
}
/* copy buffer data to buffer */
void layer_CmdCopyBuffer_before(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy* pRegions)
{
    std::string output = CUSTOM_PARAM_PREFIX;
    output += "srcBuf=";
    output += addrToString(srcBuffer);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);

    output = CUSTOM_PARAM_PREFIX;
    output += "dstBuf=";
    output += addrToString(dstBuffer);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);
}
/* copy image data to image */
void layer_CmdCopyImage_before(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageCopy* pRegions)
{
    std::string output = CUSTOM_PARAM_PREFIX;
    output += "srcImg=";
    output += addrToString(srcImage);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);

    output = CUSTOM_PARAM_PREFIX;
    output += "dstImg=";
    output += addrToString(dstImage);
    output += '!';
    winsockSendToUI(&ConnectSocket, output);
}
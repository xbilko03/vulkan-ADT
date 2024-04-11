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
    /* map memory to memoryObj */
    memoryMap[*pMemory] = {};
    memoryMap[*pMemory] = memoryObj((*pAllocateInfo).allocationSize, 0, 0, 0);

}
void layer_BindBufferMemory_after(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    auto tarObject = memoryMap[memory];
    tarObject.boundBuffer = buffer;
}
void layer_BindImageMemory_after(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    auto tarObject = memoryMap[memory];
    tarObject.boundImage = image;

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

    std::string dataMessage = CUSTOM_DATA_PREFIX;
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
    /*
    std::string output = "pid=" + std::to_string(getpid()) + '!';
    winsockSendToUI(&ConnectSocket, output);

    output = "windowName=" + GetWindowName() + '!';
    winsockSendToUI(&ConnectSocket, output);
    */
}

/* VkImage */
void layer_CreateImage_before(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage)
{

}

/* VkCommandBuffer */
void layer_AllocateCommandBuffers_after(VkDevice device, VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{


}

/* VkBuffer */
void layer_CreateBuffer_before(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
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


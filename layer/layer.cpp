/*
* Name		: appUI.cpp
* Project	: A Debugging Tool for Vulkan API
*
* Based on  : Baldur Karlsson (baldurk) and Johannes Kuhlmann's (jkuhlmann) sample_layer
* https://github.com/baldurk/sample_layer/blob/master
* Minor changes : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/

#include "layer.hpp"
#include "layer_data.hpp"

#define CUSTOM_PARAM_PREFIX "layer_"
#define CUSTOM_DATA_PREFIX "data_"

SOCKET ConnectSocket = INVALID_SOCKET;
void* map;
uint64_t image_size;


#include <iostream>
/* Enviroment Variables */

std::map <std::string, std::string> envVar;

bool warnings = false;
bool frameEveryOption = false;
unsigned long long frameEveryValue;
void SetMemoryVariables(std::string configContent)
{
    std::string token;
    std::string s = configContent;

    int pos;
    while ((pos = s.find("\n")) != std::string::npos) {
        token = s.substr(0, pos);
        s.erase(0, pos + 1);

        if (token[0] == '#' || token[0] == '\0')
            continue;

        
        auto pos1 = token.find(".");
        auto pos2 = token.substr(pos1 + 1, token.size()).find("=");
        std::string t1 = token.substr(pos1 + 1, pos2 - 1);


        pos2 = token.find("=");
        std::string t2 = token.substr(pos2 + 2, token.size());

        envVar[t1] = t2;
    }

    /*
    * default variables settings
    * 
    * vut_detailslayer.app_path = default
    * vut_detailslayer.enable_message_logging = false
    * vut_detailslayer.output_path = none
    * vut_detailslayer.enable_warnings = false
    * vut_detailslayer.frame_every = 0
    * vut_detailslayer.frame_at = 0
    * vut_detailslayer.fps_below = 0
    * vut_detailslayer.fps_every = 0
    * vut_detailslayer.triangles_over = 0
    * vut_detailslayer.triangles_every = 0
    */

    /*
    * for example, envVar["app_path"] is 'detault' - correspondes to vut_detailslayer.app_path = default
    */
    if (envVar["enable_warnings"] == "true")
    {
        warnings = true;
    }
    if (std::stol(envVar["frame_every"]) > 0 && warnings == true)
    {
        frameEveryOption = true;
        frameEveryValue = std::stol(envVar["frame_every"]);
    }
}


details::layerData* data;
bool readerReady = false;

void layer_AllocateMemory_after(VkDevice device, VkMemoryAllocateInfo* pAllocateInfo, VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
{
    data->newMemoryObj(*pMemory, (*pAllocateInfo).allocationSize);

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", ptrToString((void**)pMemory)));
}
void layer_FreeMemory_before(VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks* pAllocator)
{
    data->freeMemoryObj(memory);

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", addrToString((void*)memory)));
}
void layer_CreateInstance_after(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    data = new details::layerData();
    data->newInstance(*pInstance);

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "appName=", GetWindowName()));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "proccessID=", std::to_string(GetCurrentProcessId())));
}
void layer_BindBufferMemory_after(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    data->newBinding(memory, buffer);
    data->setState(buffer, "bound");

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "memPtr=", addrToString((void*)memory)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "bufPtr=", addrToString((void*)buffer)));
}
void layer_BindImageMemory_after(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    data->newBinding(memory, image);
    data->setState(image, "bound");

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "memPtr=", addrToString((void*)memory)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "imgPtr=", addrToString((void*)image)));
}
void layer_MapMemory_after(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
{
    data->memoryMapping(memory, size, ppData);

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", ptrToString((void**)memory)));
}
void layer_GetPhysicalDeviceSurfaceSupportKHR_before(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
{
    // empty 
}
void layer_CreateCommandPool_after(VkDevice device, VkCommandPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
{
    // empty
}
void layer_UnmapMemory_before(VkDevice device, VkDeviceMemory memory)
{
    data->memoryUnMapping(memory);
    
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", addrToString((void*)memory)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "size=", std::to_string(data->getSize(memory))));
    
    std::string dataMessage = "data";
    dataMessage += std::to_string(data->getSize(memory));
    dataMessage += '!';
    
    winsockSendToUI(&ConnectSocket, dataMessage);
    winsockSendToUIraw(&ConnectSocket, reinterpret_cast<char*>(*(data->getData(memory))), data->getSize(memory));
}
void layer_CreateImage_after(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
    data->newImage(*pImage, pCreateInfo->initialLayout, pCreateInfo->extent);

    if (readerReady == false)
    {
        connected = false;
        data->initReader(); // value.size, value.boundImage
        connected = true;
        readerReady = true;
    }

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", ptrToString((void**)pImage)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "width=", std::to_string(pCreateInfo->extent.width)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "height=", std::to_string(pCreateInfo->extent.height)));
}
void layer_CreateBuffer_after(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{

    if (readerReady == false)
    {
        connected = false;
        data->initReader(); // value.size, value.boundImage
        connected = true;
        readerReady = true;
    }

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", ptrToString((void**)pBuffer)));
}
void layer_DestroyImage_before(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator)
{
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", addrToString((void*)image)));
}
void layer_DestroyBuffer_before(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator)
{
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", addrToString((void*)buffer)));
}
void layer_CmdCopyImageToBuffer_before(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, VkBufferImageCopy* pRegions)
{
    //data->setState(dstBuffer, "copied");
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "dstBuf=", addrToString((void*)dstBuffer)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "srcImg=", addrToString((void*)srcImage)));
}
void layer_CmdCopyBufferToImage_before(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkBufferImageCopy* pRegions)
{
    //data->setState(dstImage, "copied");
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "srcBuf=", addrToString((void*)srcBuffer)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "dstImg=", addrToString((void*)dstImage)));
}
void layer_CmdCopyBuffer_before(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy* pRegions)
{
    //data->setState(dstBuffer, "copied");
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "srcBuf=", addrToString((void*)srcBuffer)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "dstBuf=", addrToString((void*)dstBuffer)));
}
void layer_CmdCopyImage_before(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageCopy* pRegions)
{
    //data->setState(dstImage, "copied");
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "srcImg=", addrToString((void*)srcImage)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "dstImg=", addrToString((void*)dstImage)));
}
void layer_CreateDevice_after(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    data->newDevice(*pDevice);
}
void layer_QueueSubmit_after(VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence)
{
    //waitdeviceidle
    for (const auto& [key, value] : *(data->getMemoryMap())) {
        if (value.status == "mapped")
        {
            auto tarObject = value;
            if (value.data == NULL)
                continue;
            winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", addrToString((void*)key)));
            winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "size=", std::to_string(tarObject.size)));

            std::string dataMessage = "data";
            dataMessage += std::to_string(tarObject.size);
            dataMessage += '!';

            winsockSendToUI(&ConnectSocket, dataMessage);
            winsockSendToUIraw(&ConnectSocket, reinterpret_cast<char*>(*tarObject.data), tarObject.size);
        }
        else if(value.status == "bound")
        {
            if (value.boundTo == "image")
            {
                connected = false;
                data->mapImageToBuffer(key);
                connected = true;
            }
            else if (value.boundTo == "buffer")
            {
                connected = false;
                data->mapBufferToBuffer(key);
                connected = true;
            }
            winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", addrToString((void*)key)));
            winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "size=", std::to_string(value.size)));

            std::string dataMessage = "data";
            dataMessage += std::to_string(value.size);
            dataMessage += '!';

            char* limitedData = (char*)malloc(value.size);
            memcpy(limitedData, *data->getBufferData(), value.size);

            winsockSendToUI(&ConnectSocket, dataMessage);
            winsockSendToUIraw(&ConnectSocket, (limitedData), value.size);

            free(limitedData);
        }
    }
}

/* before new frame hits */
unsigned long long frameCount = 0;
void layer_AcquireNextImageKHR_before(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
{
    frameCount++;

    if (frameEveryOption)
    {
        if (frameCount % frameEveryValue == 0)
        {
            winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "warning_frameCount=", std::to_string(frameCount)));
        }
    }
}

void layer_CreateImageView_after(VkDevice device, VkImageViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImageView* pView)
{
    //empty
}
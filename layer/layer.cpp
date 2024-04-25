/*
* Name		: appUI.cpp
* Project	: A Debugging Tool for Vulkan API
*
* Based on  : Baldur Karlsson (baldurk) and Johannes Kuhlmann's (jkuhlmann) sample_layer
* https://github.com/baldurk/sample_layer/blob/master
* Minor changes : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/

#include "layer.hpp"

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

/* VkMemory */
typedef struct memoryObj {
    VkDeviceSize size;
    VkBuffer boundBuffer;
    VkImage boundImage;
    std::string status;
    std::string boundTo;

    void** data;
};
std::map<VkDeviceMemory, memoryObj> memoryMap;
std::map<VkImage, VkExtent3D> extentMap;
std::map<VkImage, VkImageLayout> layoutMap;

/* Create memory object */
void layer_AllocateMemory_after(VkDevice device, VkMemoryAllocateInfo* pAllocateInfo, VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
{
    /* map memory to memoryObj */
    memoryMap[*pMemory] = {};
    memoryMap[*pMemory] = memoryObj((*pAllocateInfo).allocationSize, nullptr, nullptr, "allocated", "nothing", NULL);

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", ptrToString((void**)pMemory)));
}
/* Free memory object */
void layer_FreeMemory_before(VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks* pAllocator)
{
    /* map memory to memoryObj */
    memoryMap[memory] = {};
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", addrToString((void*)memory)));
}

VkInstance instance;

void layer_CreateInstance_after(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    instance = *pInstance;

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "appName=", GetWindowName()));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "proccessID=", std::to_string(GetCurrentProcessId())));
}

void layer_BindBufferMemory_after(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    memoryMap[memory].boundBuffer = buffer;
    memoryMap[memory].boundTo = "image";

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "memPtr=", addrToString((void*)memory)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "bufPtr=", addrToString((void*)buffer)));
}
void layer_BindImageMemory_after(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{




    memoryMap[memory].boundImage = image;
    memoryMap[memory].boundTo = "image";

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "memPtr=", addrToString((void*)memory)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "imgPtr=", addrToString((void*)image)));
}

VkDevice deviceGl;

/* Get local pointer to memory data */
void layer_MapMemory_after(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
{
    deviceGl = device;

    auto tarObject = memoryMap[memory];
    if (tarObject.size > size)
        tarObject.size = size;
    tarObject.data = ppData;
    tarObject.status = "mapped";
    memoryMap[memory] = tarObject;
}
/* Get data from the local pointer */
#define VK_USE_PLATFORM_WIN32_KHR

/* filled by vkGetPhysicalDeviceSurfaceSupportKHR */
VkSurfaceKHR surfaceNew;
VkPhysicalDevice physicalDeviceNew;

void layer_GetPhysicalDeviceSurfaceSupportKHR_before(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
{
    surfaceNew = surface;
}

VkCommandPool commandPool;
void layer_CreateCommandPool_after(VkDevice device, VkCommandPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
{
    commandPool = *pCommandPool;
}



void test(VkDevice device, VkDeviceSize size, VkImage image)
{
    uint32_t pPhysicalDeviceCount;
    std::cout << "in " << instance << std::endl;
    vkEnumeratePhysicalDevices(instance, &pPhysicalDeviceCount, nullptr);
    

    std::vector<VkPhysicalDevice> devices(pPhysicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &pPhysicalDeviceCount, devices.data());

    int devCounter = 0;
    int i = 0;
    for (const auto& device : devices) {
        if (i == devCounter) {
            physicalDeviceNew = device;
            break;
        }
        i++;
    }

    std::cout << "dev" << physicalDeviceNew << std::endl;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDeviceNew, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDeviceNew, &queueFamilyCount, queueFamilies.data());

    uint32_t queueFamilyIndex = 0;
    i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueFamilyIndex = i;
            break;
        }
        i++;
    }





    VkCommandPool commandPool;

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndex;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }

    std::cout << "new pool " << commandPool << std::endl;

    VkCommandBuffer commandBuffer;
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    std::cout << "new cmdBuff " << commandBuffer << std::endl;

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }


    std::cout << "recording " << commandBuffer << std::endl;

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

    std::cout << "done " << commandBuffer << std::endl;

    VkQueue graphicsQueue;
    vkGetDeviceQueue(device, queueFamilyIndex, 0, &graphicsQueue);

    std::cout << "done queue " << graphicsQueue << std::endl;

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    std::cout << "done submit " << graphicsQueue << std::endl;

    VkDeviceMemory bufferMemory;
    VkBuffer buffer;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }
    std::cout << "done buffer " << buffer << std::endl;

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfoMem{};
    allocInfoMem.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfoMem.allocationSize = memRequirements.size;


    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDeviceNew, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((memRequirements.memoryTypeBits & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
            allocInfoMem.memoryTypeIndex = i;
        }
    }

    if (vkAllocateMemory(device, &allocInfoMem, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    std::cout << "done memory " << bufferMemory << std::endl;
    vkBindBufferMemory(device, buffer, bufferMemory, 0);


    std::cout << "done memory bound and everything ready " << bufferMemory << std::endl;

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = extentMap[image];

    vkCmdCopyImageToBuffer(commandBuffer,image, layoutMap[image], buffer, 1, &region);
    std::cout << "image is copied " << buffer << std::endl;

    void* data;
    vkMapMemory(device, bufferMemory, 0, size, 0, &data);
    vkDeviceWaitIdle(device);
    
    std::cout << "data = " << (char*)data << " of size " << size << std::endl;

    //vkDestroyCommandPool(device, commandPool, nullptr);
    exit(0);
}



void layer_UnmapMemory_before(VkDevice device, VkDeviceMemory memory)
{
    auto tarObject = memoryMap[memory];
    tarObject.status = "unmapped";
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", addrToString((void*)memory)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "size=", std::to_string(tarObject.size)));
    
    std::string dataMessage = "data";
    dataMessage += std::to_string(tarObject.size);
    dataMessage += '!';
    
    winsockSendToUI(&ConnectSocket, dataMessage);
    winsockSendToUIraw(&ConnectSocket, reinterpret_cast<char*>(*tarObject.data), tarObject.size);
}
/* Create Image object */
void layer_CreateImage_after(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
    layoutMap[*pImage] = pCreateInfo->initialLayout;
    extentMap[*pImage] = pCreateInfo->extent;

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", ptrToString((void**)pImage)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "width=", std::to_string(pCreateInfo->extent.width)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "height=", std::to_string(pCreateInfo->extent.height)));
}
/* Create buffer object */
void layer_CreateBuffer_after(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", ptrToString((void**)pBuffer)));
}
/* Free image object */
void layer_DestroyImage_before(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator)
{
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", addrToString((void*)image)));
}
/* Free buffer object */
void layer_DestroyBuffer_before(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator)
{
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", addrToString((void*)buffer)));
}
/* copy image data to buffer */
void layer_CmdCopyImageToBuffer_before(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, VkBufferImageCopy* pRegions)
{
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "dstBuf=", addrToString((void*)dstBuffer)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "srcImg=", addrToString((void*)srcImage)));
}
/* copy buffer data to image */
void layer_CmdCopyBufferToImage_before(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkBufferImageCopy* pRegions)
{
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "srcBuf=", addrToString((void*)srcBuffer)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "dstImg=", addrToString((void*)dstImage)));
}
/* copy buffer data to buffer */
void layer_CmdCopyBuffer_before(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy* pRegions)
{
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "srcBuf=", addrToString((void*)srcBuffer)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "dstBuf=", addrToString((void*)dstBuffer)));
}
/* copy image data to image */
void layer_CmdCopyImage_before(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageCopy* pRegions)
{
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "srcImg=", addrToString((void*)srcImage)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "dstImg=", addrToString((void*)dstImage)));
}

void layer_QueueSubmit_after(VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence)
{
    /* just mapped buffers data send */
    for (const auto& [key, value] : memoryMap) {
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
        else if(value.status == "allocated")
        {
            auto tarObject = value;

            connected = false;
            skipLock = true;

            if(tarObject.boundTo == "image")
                test(deviceGl,tarObject.size, tarObject.boundImage);

            skipLock = false;
            connected = true;
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
    return;
    /* find corresponding image */
    for (const auto& [key, value] : memoryMap) {
        if (value.boundImage == pCreateInfo->image)
        {
            /* pause layer functionality with these booleans */
            connected = false;
            skipLock = true;


            exit(0);
            skipLock = false;
            connected = true;
            break;
        }
    }
}
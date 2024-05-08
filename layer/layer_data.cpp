/*
* Name		    : layer_data.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file to store the current state of the Vulkan constructs and perform operations on them
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#include "layer_data.hpp"
#include "layer.hpp"

namespace details {

    /* initializes the GPU to CPU memory reader mechanism */
    void layerData::initReader()
    {
        /*
        * skipLock is to ensure deadlock wouldn't occur during runtime, it needs to be called everytime the layer uses Vulkan functions
        * since they are passed into the loader first and the arrive back here, also, in case it is the same command,
        * e.g. vkQueueSubmit and then on the layer of this call is called vkQueueSubmit, it creates an indefinite loop 
        * that is not desired for the purposes of this application
        */
        uint32_t pPhysicalDeviceCount;
        skipLock = true;
        vkEnumeratePhysicalDevices(instanceAddr, &pPhysicalDeviceCount, nullptr);
        skipLock = false;

        std::vector<VkPhysicalDevice> devices(pPhysicalDeviceCount);
        skipLock = true;
        vkEnumeratePhysicalDevices(instanceAddr, &pPhysicalDeviceCount, devices.data());
        skipLock = false;

        int devCounter = 0;
        int i = 0;
        /* get the first physical device available */
        for (auto device : devices) {
            if (i == devCounter) {
                physicalDeviceNew = device;
                break;
            }
            i++;
        }

        uint32_t queueFamilyCount = 0;
        skipLock = true;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDeviceNew, &queueFamilyCount, nullptr);
        skipLock = false;

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        skipLock = true;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDeviceNew, &queueFamilyCount, queueFamilies.data());
        skipLock = false;

        /* get queueFamilyIndex */
        uint32_t queueFamilyIndex = 0;
        i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                queueFamilyIndex = i;
                break;
            }
            i++;
        }

        /* create command pool to create command buffers */
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndex;

        VkCommandPool commandPool;
        skipLock = true;
        if (vkCreateCommandPool(deviceAddr, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }
        skipLock = false;

        /* create command buffers to record commands that will be executed on GPU  */
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        skipLock = true;
        if (vkAllocateCommandBuffers(deviceAddr, &allocInfo, &commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
        skipLock = false;

        /* create a device queue */
        skipLock = true;
        vkGetDeviceQueue(deviceAddr, queueFamilyIndex, 0, &graphicsQueue);
        skipLock = false;

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = MAXBUFFERSIZE;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        /* create buffer */
        skipLock = true;
        if (vkCreateBuffer(deviceAddr, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }
        skipLock = false;

        /* get memory reqs and allocate a new memory */
        VkMemoryRequirements memRequirements;
        skipLock = true;
        vkGetBufferMemoryRequirements(deviceAddr, buffer, &memRequirements);
        skipLock = false;

        VkMemoryAllocateInfo allocInfoMem{};
        allocInfoMem.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfoMem.allocationSize = memRequirements.size;

        VkPhysicalDeviceMemoryProperties memProperties;
        skipLock = true;
        vkGetPhysicalDeviceMemoryProperties(physicalDeviceNew, &memProperties);
        skipLock = false;

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((memRequirements.memoryTypeBits & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
                allocInfoMem.memoryTypeIndex = i;
            }
        }

        skipLock = true;
        if (vkAllocateMemory(deviceAddr, &allocInfoMem, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }
        skipLock = false;

        /* bind copy buffer to the CPU readable memory */
        skipLock = true;
        vkBindBufferMemory(deviceAddr, buffer, bufferMemory, 0);
        skipLock = false;
    }
    /* copies image data and saves them */
    void layerData::mapImageToBuffer(VkDeviceMemory memory)
    {
        /* waits until GPU is no longer busy so the data wouldn't get biased somehow */
        skipLock = true;
        vkDeviceWaitIdle(deviceAddr);
        skipLock = false;

        VkImage inputImage = memoryMap[memory].boundImage;

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = imageMap[inputImage].extent;

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        skipLock = true;
        vkMapMemory(deviceAddr, bufferMemory, 0, memoryMap[memory].size, 0, &bufferData);
        skipLock = false;

        /* start command buffer recording */
        skipLock = true;
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }
        skipLock = false;

        /* copy to CPU readable buffer */
        skipLock = true;
        vkCmdCopyImageToBuffer(commandBuffer, inputImage, VK_IMAGE_LAYOUT_GENERAL, buffer, 1, &region);
        skipLock = false;

        /* ends command buffer recording */
        skipLock = true;
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
        skipLock = false;

        /* submit recorded calls to GPU */
        skipLock = true;
        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        skipLock = false;

        skipLock = true;
        vkUnmapMemory(deviceAddr, bufferMemory);
        skipLock = false;

        /* wait until the submit operation is finished */
        skipLock = true;
        vkDeviceWaitIdle(deviceAddr);
        skipLock = false;
        /* now the data at 'bufferData' are data from the copied image */
    }
    /* copies buffer data and saves them */
    void layerData::mapBufferToBuffer(VkDeviceMemory memory)
    {
        /* waits until GPU is no longer busy so the data wouldn't get biased somehow */
        skipLock = true;
        vkDeviceWaitIdle(deviceAddr);
        skipLock = false;

        VkBuffer inputBuffer = memoryMap[memory].boundBuffer;

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        VkBufferCopy region{};
        region.srcOffset = 0;
        region.dstOffset = 0;
        region.size = memoryMap[memory].size;

        skipLock = true;
        vkMapMemory(deviceAddr, bufferMemory, 0, memoryMap[memory].size, 0, &bufferData);
        skipLock = false;

        /* start of the command buffer recording */
        skipLock = true;
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }
        skipLock = false;

        /* copy to cpu readable buffer */
        skipLock = true;
        vkCmdCopyBuffer(commandBuffer, inputBuffer, buffer, 1, &region);
        skipLock = false;

        /* end of the command buffer recording */
        skipLock = true;
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
        skipLock = false;

        skipLock = true;
        vkUnmapMemory(deviceAddr, bufferMemory);
        skipLock = false;

        /* submit recorded calls to GPU */
        skipLock = true;
        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        skipLock = false;

        /* wait until the submit operation is finished */
        skipLock = true;
        vkDeviceWaitIdle(deviceAddr);
        skipLock = false;
        /* now the data at 'bufferData' are data from the copied buffer */
    }
    /* allocates new memory of certain size and map it */
    void layerData::newMemoryObj(VkDeviceMemory memory, VkDeviceSize size)
    {
        memoryMap[memory] = {};
        memoryMap[memory] = memoryObj(size, nullptr, nullptr, "allocated", "nothing", NULL);
    }
    /* creates saves a new image with the initial data */
    void layerData::newImage(VkImage image, VkImageLayout initialLayout, VkExtent3D extent)
    {
        imageMap[image] = {};
        imageMap[image] = imageObj(initialLayout, extent, nullptr);
    }
    /* creates and saves a new buffer */
    void layerData::newBuffer(VkBuffer buffer)
    {
        bufferMap[buffer] = {};
        bufferMap[buffer] = bufferObj(nullptr);
    }
    /* frees the memory, rendering its handle free to use again */
    void layerData::freeMemoryObj(VkDeviceMemory memory)
    {
        memoryMap[memory] = {};
    }
    /* saves the reference to the instance in use */
    void layerData::newInstance(VkInstance instance)
    {
        instanceAddr = instance;
    }
    /* saves the reference ot the device in use */
    void layerData::newDevice(VkDevice device)
    {
        deviceAddr = device;
    }
    /* associates a memory with a buffer */
    void layerData::newBinding(VkDeviceMemory memory, VkBuffer buffer)
    {
        memoryMap[memory].boundBuffer = buffer;
        memoryMap[memory].boundTo = "buffer";
        bufferMap[buffer].boundTo = memory;

    }
    /* associates a memory with an image */
    void layerData::newBinding(VkDeviceMemory memory, VkImage image)
    {
        memoryMap[memory].boundImage = image;
        memoryMap[memory].boundTo = "image";
        imageMap[image].boundTo = memory;
    }
    /* associates a GPU with CPU memory */
    void layerData::memoryMapping(VkDeviceMemory memory, VkDeviceSize size, void** data)
    {
        if (memoryMap[memory].size > size)
            memoryMap[memory].size = size;

        memoryMap[memory].data = data;
        memoryMap[memory].status = "mapped";
    }
    /* frees the memory mapping */
    void layerData::memoryUnMapping(VkDeviceMemory memory)
    {
        memoryMap[memory].status = "unmapped";
    }
}
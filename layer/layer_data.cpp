#include "layer_data.hpp"



namespace details {
    VkInstance instanceAddr;
    VkDevice deviceAddr;
    VkPhysicalDevice physicalDeviceNew;
    #define MAXBUFFERSIZE 5000000

    VkCommandBuffer commandBuffer;
    VkDeviceMemory bufferMemory;
    VkBuffer buffer;

    void layerData::initReader()
    {
        uint32_t pPhysicalDeviceCount;
        std::cout << "in " << instanceAddr << std::endl;
        vkEnumeratePhysicalDevices(instanceAddr, &pPhysicalDeviceCount, nullptr);

        std::vector<VkPhysicalDevice> devices(pPhysicalDeviceCount);
        vkEnumeratePhysicalDevices(instanceAddr, &pPhysicalDeviceCount, devices.data());

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

        if (vkCreateCommandPool(deviceAddr, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }

        std::cout << "new pool " << commandPool << std::endl;

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(deviceAddr, &allocInfo, &commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        std::cout << "new cmdBuff " << commandBuffer << std::endl;

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }


        std::cout << "recording " << commandBuffer << std::endl;

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

        std::cout << "done " << commandBuffer << std::endl;

        VkQueue graphicsQueue;
        vkGetDeviceQueue(deviceAddr, queueFamilyIndex, 0, &graphicsQueue);

        std::cout << "done queue " << graphicsQueue << std::endl;

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(graphicsQueue);

        std::cout << "done submit " << graphicsQueue << std::endl;

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = MAXBUFFERSIZE;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(deviceAddr, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }
        std::cout << "done buffer " << buffer << std::endl;

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(deviceAddr, buffer, &memRequirements);

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

        if (vkAllocateMemory(deviceAddr, &allocInfoMem, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        std::cout << "done memory " << bufferMemory << std::endl;
        vkBindBufferMemory(deviceAddr, buffer, bufferMemory, 0);

        std::cout << "done memory bound and everything ready " << bufferMemory << std::endl;

        //vkDestroyCommandPool(device, commandPool, nullptr);
    }
    void layerData::mapImageToBuffer(VkDeviceMemory memory)
    {
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

        vkCmdCopyImageToBuffer(commandBuffer, inputImage, imageMap[inputImage].layout, buffer, 1, &region);

        vkMapMemory(deviceAddr, bufferMemory, 0, memoryMap[memory].size, 0, &bufferData);
        vkDeviceWaitIdle(deviceAddr);
    }
    void layerData::mapBufferToBuffer(VkDeviceMemory memory)
    {
        VkBuffer inputBuffer = memoryMap[memory].boundBuffer;

        VkBufferCopy region{};
        region.srcOffset = 0;
        region.dstOffset = 0;
        region.size = memoryMap[memory].size;

        vkCmdCopyBuffer(commandBuffer, inputBuffer, buffer, 1, &region);

        vkMapMemory(deviceAddr, bufferMemory, 0, memoryMap[memory].size, 0, &bufferData);
        vkDeviceWaitIdle(deviceAddr);
    }
    void layerData::newMemoryObj(VkDeviceMemory memory, VkDeviceSize size)
    {
        memoryMap[memory] = {};
        memoryMap[memory] = memoryObj(size, nullptr, nullptr, "allocated", "nothing", NULL);
    }
    void layerData::newImage(VkImage image, VkImageLayout initialLayout, VkExtent3D extent)
    {
        imageMap[image] = {};
        imageMap[image] = imageObj(initialLayout, extent, nullptr);
    }
    void layerData::newBuffer(VkBuffer buffer)
    {
        bufferMap[buffer] = {};
        bufferMap[buffer] = bufferObj(nullptr);
    }
    void layerData::freeMemoryObj(VkDeviceMemory memory)
    {
        memoryMap[memory] = {};
    }
    void layerData::newInstance(VkInstance instance)
    {
        instanceAddr = instance;
    }
    void layerData::newDevice(VkDevice device)
    {
        deviceAddr = device;
    }
    void layerData::newBinding(VkDeviceMemory memory, VkBuffer buffer)
    {
        memoryMap[memory].boundBuffer = buffer;
        memoryMap[memory].boundTo = "buffer";
        bufferMap[buffer].boundTo = memory;

    }
    void layerData::newBinding(VkDeviceMemory memory, VkImage image)
    {
        memoryMap[memory].boundImage = image;
        memoryMap[memory].boundTo = "image";
        imageMap[image].boundTo = memory;
    }
    void layerData::memoryMapping(VkDeviceMemory memory, VkDeviceSize size, void** data)
    {
        if (memoryMap[memory].size > size)
            memoryMap[memory].size = size;

        memoryMap[memory].data = data;
        memoryMap[memory].status = "mapped";
    }
    void layerData::memoryUnMapping(VkDeviceMemory memory)
    {
        memoryMap[memory].status = "unmapped";
    }
}
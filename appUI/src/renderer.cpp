/*
* Name		    : renderer.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file of the Vulkan window and image rendering VkDebuggerApp
*
* This template is available online by: Omar Cornut (ocornut) : MIT license
* https://github.com/ocornut/imgui/tree/master/examples/example_glfw_vulkan
* Minor adjustments: Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#include "../include/renderer.hpp"
#include "../include/communication/events.hpp"
#include <stdexcept>

/* moved here from callbacks.cpp (2026-10-16): both are used only by this file */
/* function called when an glfw error occurs */
static void glfwErrorCallback(int err, const char* text)
{
    throw std::runtime_error("GLFW - " + std::to_string(err) + " - " + text);
}
/* function called when Vulkan error occurs */
static void vkErrorCallback(VkResult err)
{
    if (err == 0)
        return;
    throw std::runtime_error("Vulkan - VkResult - " + std::to_string(err));
    if (err < 0)
        abort();
}

namespace vkdebug::ui {
    uint32_t renderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(pPhysicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;

        return 0xFFFFFFFF; // Unable to find memoryType
    }
    bool renderer::loadImageTexture(unsigned long long ID, int width, int height, int channels, void* imageData)
    {
        /* free last texture if exists */
        if (loadedImages.count(ID))
        {
            removeTexture(&loadedImages[ID]);
        }

        currentTexture.channels = 4;
        currentTexture.height = height;
        currentTexture.width = width;

        void* imagePixels = imageData;

        if (imagePixels == NULL)
        {
            return false;
        }

        /* calculate allocation size (in number of bytes) */
        size_t imageSize = currentTexture.width * currentTexture.height * currentTexture.channels;

        {
            /* create the Vulkan image */
            VkImageCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            info.imageType = VK_IMAGE_TYPE_2D;
            info.format = VK_FORMAT_R8G8B8A8_UNORM;
            /* AI-Assisted: Claude Opus 5.5: syntax */
            info.extent.width = (uint32_t)currentTexture.width;
            info.extent.height = (uint32_t)currentTexture.height;
            /* AI-Assisted: end */
            info.extent.depth = 1;
            info.mipLevels = 1;
            info.arrayLayers = 1;
            info.samples = VK_SAMPLE_COUNT_1_BIT;
            info.tiling = VK_IMAGE_TILING_OPTIMAL;
            info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            if (vkCreateImage(pDevice, &info, pAllocator, &currentTexture.image) != VK_SUCCESS)
            {
                return false;
            }
            VkMemoryRequirements req;
            vkGetImageMemoryRequirements(pDevice, currentTexture.image, &req);
            VkMemoryAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = req.size;
            allocInfo.memoryTypeIndex = findMemoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            if (vkAllocateMemory(pDevice, &allocInfo, pAllocator, &currentTexture.imageMemory) != VK_SUCCESS)
            {
                return false;
            }
            if (vkBindImageMemory(pDevice, currentTexture.image, currentTexture.imageMemory, 0) != VK_SUCCESS)
            {
                return false;
            }
        }

        {
            /* create the Image View */
            VkImageViewCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            info.image = currentTexture.image;
            info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            info.format = VK_FORMAT_R8G8B8A8_UNORM;
            info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            info.subresourceRange.levelCount = 1;
            info.subresourceRange.layerCount = 1;
            if (vkCreateImageView(pDevice, &info, pAllocator, &currentTexture.imageView) != VK_SUCCESS)
            {
                return false;
            }
        }

        {
            /* Create sampler */
            VkSamplerCreateInfo samplerInfo{};
            samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.magFilter = VK_FILTER_LINEAR;
            samplerInfo.minFilter = VK_FILTER_LINEAR;
            samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.minLod = -1000;
            samplerInfo.maxLod = 1000;
            samplerInfo.maxAnisotropy = 1.0f;
            if (vkCreateSampler(pDevice, &samplerInfo, pAllocator, &currentTexture.sampler) != VK_SUCCESS)
            {
                return false;
            }
        }

        /* create Descriptor Set using ImGUI's implementation */
        currentTexture.DS = ImGui_ImplVulkan_AddTexture(currentTexture.sampler, currentTexture.imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        {
            /* create Upload Buffer */
            VkBufferCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = imageSize;
            bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            if (vkCreateBuffer(pDevice, &bufferInfo, pAllocator, &currentTexture.uploadBuffer) != VK_SUCCESS)
            {
                return false;
            }
            VkMemoryRequirements req;
            vkGetBufferMemoryRequirements(pDevice, currentTexture.uploadBuffer, &req);
            VkMemoryAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = req.size;
            allocInfo.memoryTypeIndex = findMemoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            if (vkAllocateMemory(pDevice, &allocInfo, pAllocator, &currentTexture.uploadBufferMemory) != VK_SUCCESS)
            {
                return false;
            }
            if (vkBindBufferMemory(pDevice, currentTexture.uploadBuffer, currentTexture.uploadBufferMemory, 0) != VK_SUCCESS)
            {
                return false;
            }
        }

        {
            /* upload data to Buffer */
            void* map = NULL;

            if (vkMapMemory(pDevice, currentTexture.uploadBufferMemory, 0, imageSize, 0, &map) != VK_SUCCESS)
            {
                return false;
            }
            memcpy(map, imagePixels, imageSize);
            VkMappedMemoryRange range[1] = {};
            range[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
            range[0].memory = currentTexture.uploadBufferMemory;
            range[0].size = imageSize;
            if (vkFlushMappedMemoryRanges(pDevice, 1, range) != VK_SUCCESS)
            {
                return false;
            }
            vkUnmapMemory(pDevice, currentTexture.uploadBufferMemory);
        }

        VkCommandPool commandPool = pMainWindowData.Frames[pMainWindowData.FrameIndex].CommandPool;
        VkCommandBuffer commandBuffer;
        {
            /* create a command buffer that will perform following steps when hit in the command queue */
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = commandPool;
            allocInfo.commandBufferCount = 1;

            if (vkAllocateCommandBuffers(pDevice, &allocInfo, &commandBuffer) != VK_SUCCESS)
            {
                return false;
            }

            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
            {
                return false;
            }
        }

        {
            /* copy to Image */
            VkImageMemoryBarrier copyBarrier[1] = {};
            copyBarrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            copyBarrier[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            copyBarrier[0].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            copyBarrier[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            copyBarrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            copyBarrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            copyBarrier[0].image = currentTexture.image;
            copyBarrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copyBarrier[0].subresourceRange.levelCount = 1;
            copyBarrier[0].subresourceRange.layerCount = 1;
            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, copyBarrier);

            VkBufferImageCopy region = {};
            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.layerCount = 1;
            /* AI-Assisted: Claude Opus 5.5: syntax */
            region.imageExtent.width = (uint32_t)currentTexture.width;
            region.imageExtent.height = (uint32_t)currentTexture.height;
            /* AI-Assisted: end */
            region.imageExtent.depth = 1;
            vkCmdCopyBufferToImage(commandBuffer, currentTexture.uploadBuffer, currentTexture.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

            VkImageMemoryBarrier useBarrier[1] = {};
            useBarrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            useBarrier[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            useBarrier[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            useBarrier[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            useBarrier[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            useBarrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            useBarrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            useBarrier[0].image = currentTexture.image;
            useBarrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            useBarrier[0].subresourceRange.levelCount = 1;
            useBarrier[0].subresourceRange.layerCount = 1;
            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, useBarrier);
        }

        {
            /* end command buffer */
            VkSubmitInfo endInfo = {};
            endInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            endInfo.commandBufferCount = 1;
            endInfo.pCommandBuffers = &commandBuffer;
            if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
            {
                return false;
            }
            if (vkQueueSubmit(pQueue, 1, &endInfo, VK_NULL_HANDLE) != VK_SUCCESS)
            {
                return false;
            }
            if (vkDeviceWaitIdle(pDevice) != VK_SUCCESS)
            {
                return false;
            }
        }

        loadedImages[ID] = currentTexture;
        return true;
    }

    void renderer::removeTexture(textureData* texData)
    {
        vkFreeMemory(pDevice, texData->uploadBufferMemory, nullptr);
        vkDestroyBuffer(pDevice, texData->uploadBuffer, nullptr);
        vkDestroySampler(pDevice, texData->sampler, nullptr);
        vkDestroyImageView(pDevice, texData->imageView, nullptr);
        vkDestroyImage(pDevice, texData->image, nullptr);
        vkFreeMemory(pDevice, texData->imageMemory, nullptr);
        ImGui_ImplVulkan_RemoveTexture(texData->DS);
    }
    
    void renderer::renderNewFrame()
    {
        ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();
        const bool isMinimized = (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);
        if (!isMinimized)
        {
            wd->ClearValue.color.float32[0] = clearColor.x * clearColor.w;
            wd->ClearValue.color.float32[1] = clearColor.y * clearColor.w;
            wd->ClearValue.color.float32[2] = clearColor.z * clearColor.w;
            wd->ClearValue.color.float32[3] = clearColor.w;
            frameRender(wd, drawData);
            framePresent(wd);
        }
    }

    void renderer::startNewFrame()
    {
        glfwPollEvents();

        if (pSwapChainRebuild)
        {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            if (width > 0 && height > 0)
            {
                ImGui_ImplVulkan_SetMinImageCount(pMinImageCount);
                ImGui_ImplVulkanH_CreateOrResizeWindow(pInstance, pPhysicalDevice, pDevice, &pMainWindowData, pQueueFamily, pAllocator, width, height, pMinImageCount);
                pMainWindowData.FrameIndex = 0;
                pSwapChainRebuild = false;
            }
        }

        /* start the Dear ImGui frame */
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void renderer::cleanupImGui()
    {
        if (vkDeviceWaitIdle(pDevice) != VK_SUCCESS)
        {
            throw std::runtime_error("ImGui: failed to cleanup");
            return;
        }
        removeTexture(&currentTexture);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void renderer::cleanupGLFW()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void renderer::imGuiInit()
    {
        /* setup Dear ImGui context */
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        /* Enable Keyboard Controls */
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        
        /* setup Dear ImGui style */
        ImGui::StyleColorsDark();

        /* setup Platform/Renderer backends */
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = pInstance;
        initInfo.PhysicalDevice = pPhysicalDevice;
        initInfo.Device = pDevice;
        initInfo.QueueFamily = pQueueFamily;
        initInfo.Queue = pQueue;
        initInfo.PipelineCache = pPipelineCache;
        initInfo.DescriptorPool = pDescriptorPool;
        initInfo.Subpass = 0;
        initInfo.MinImageCount = pMinImageCount;
        initInfo.ImageCount = wd->ImageCount;
        initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        initInfo.Allocator = pAllocator;
        initInfo.CheckVkResultFn = vkErrorCallback;
        ImGui_ImplVulkan_Init(&initInfo, wd->RenderPass);

    }

    void renderer::framebufferInit()
    {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        wd = &pMainWindowData;
        setupVulkanWindow(wd, surface, w, h);
    }

    void renderer::surfaceInit()
    {
        if (glfwCreateWindowSurface(pInstance, window, pAllocator, &surface) != VK_SUCCESS)
        {
            throw std::runtime_error("GLFW: failed to initialize");
            return;
        }
    }

    void renderer::glfwWindowInit()
    {
        glfwSetErrorCallback(glfwErrorCallback);
        if (!glfwInit())
        {
            throw std::runtime_error("GLFW: failed to initialize");
        }

        /* create window with Vulkan context */
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(1280, 720, WINDOW_NAME, nullptr, nullptr);
        if (!glfwVulkanSupported())
        {
            throw std::runtime_error("GLFW: vulkan not supported");
        }
    }

    void renderer::dataInit()
    {
        pAllocator = nullptr;
        pInstance = VK_NULL_HANDLE;
        pPhysicalDevice = VK_NULL_HANDLE;
        pDevice = VK_NULL_HANDLE;
        pQueueFamily = (uint32_t)-1;
        pQueue = VK_NULL_HANDLE;
        pDebugReport = VK_NULL_HANDLE;
        pPipelineCache = VK_NULL_HANDLE;
        pDescriptorPool = VK_NULL_HANDLE;
        pMinImageCount = 2;
        pSwapChainRebuild = false;
        clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    }

    bool renderer::isExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension)
    {
        for (const VkExtensionProperties& p : properties)
            if (strcmp(p.extensionName, extension) == 0)
                return true;
        return false;
    }

    VkPhysicalDevice renderer::setupVulkan_SelectPhysicalDevice()
    {
        /* prioritize discrete GPU, if not found, select integrated one, if there are none, return VK_NULL_HANDLE */
        if (vkEnumeratePhysicalDevices(pInstance, &physicalDeviceCount, nullptr) != VK_SUCCESS)
        {
            throw std::runtime_error("Vulkan: failed to get physical device list");
            return nullptr;
        }
        IM_ASSERT(physicalDeviceCount > 0);

        ImVector<VkPhysicalDevice> gpus;
        gpus.resize(physicalDeviceCount);
        if (vkEnumeratePhysicalDevices(pInstance, &physicalDeviceCount, gpus.Data) != VK_SUCCESS)
        {
            throw std::runtime_error("Vulkan: failed to get physical device list");
            return nullptr;
        }

        VkPhysicalDevice chosenDevice = nullptr;
        for (VkPhysicalDevice& device : gpus)
        {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(device, &properties);

            /* extract physical devices data */
            physicalDevicePropertiesList.push_back(properties);

            if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                chosenDevice = device;
        }

        if (chosenDevice == nullptr)
        {
            /* in case no discrete GPUs are available */
            if(physicalDeviceCount > 0)
                chosenDevice = gpus[0];
        }
        if (physicalDeviceCount > 0)
            return chosenDevice;
        else
            /* no suitable device found */
            return VK_NULL_HANDLE;
    }

    void renderer::setupVulkan()
    {
        uint32_t extensionsCount = 0;
        ImVector<const char*> reqInstanceExtensions;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionsCount);
        for (uint32_t i = 0; i < extensionsCount; i++)
            reqInstanceExtensions.push_back(glfwExtensions[i]);

        VkResult err;

        /* create Vulkan Instance */
        {
            VkInstanceCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

            /* enumerate available extensions */
            uint32_t propertiesCount;
            vkEnumerateInstanceExtensionProperties(nullptr, &propertiesCount, nullptr);
            instanceExtensions.resize(propertiesCount);
            err = vkEnumerateInstanceExtensionProperties(nullptr, &propertiesCount, instanceExtensions.Data);
            if (err != VK_SUCCESS)
            {
                throw std::runtime_error("Vulkan: failed to get instance extension list");
                return;
            }

            /* enable required extensions */
            if (isExtensionAvailable(instanceExtensions, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
                reqInstanceExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
            #ifdef VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
            if (isExtensionAvailable(instanceExtensions, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME))
            {
                reqInstanceExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
                createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
            }
            #endif
            // Create Vulkan Instance
            createInfo.enabledExtensionCount = (uint32_t)reqInstanceExtensions.Size;
            createInfo.ppEnabledExtensionNames = reqInstanceExtensions.Data;
            if (vkCreateInstance(&createInfo, pAllocator, &pInstance) != VK_SUCCESS)
            {
                throw std::runtime_error("Vulkan: failed to create an instance");
                return;
            }
    }

        /* select Physical Device (GPU) */
        pPhysicalDevice = setupVulkan_SelectPhysicalDevice();

        /* select graphics queue family */
        {
            uint32_t count;
            vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevice, &count, nullptr);
            VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
            vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevice, &count, queues);
            for (uint32_t i = 0; i < count; i++)
                if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    pQueueFamily = i;
                    break;
                }
            free(queues);
            IM_ASSERT(pQueueFamily != (uint32_t)-1);
        }

        {
            /* create Logical Device (with a single queue) */
            ImVector<const char*> deviceExt;
            deviceExt.push_back("VK_KHR_swapchain");

            /* enumerate physical device extension */
            uint32_t propertiesCount;
            vkEnumerateDeviceExtensionProperties(pPhysicalDevice, nullptr, &propertiesCount, nullptr);
            deviceExtensions.resize(propertiesCount);
            vkEnumerateDeviceExtensionProperties(pPhysicalDevice, nullptr, &propertiesCount, deviceExtensions.Data);
            
            const float queuePriority[] = { 1.0f };
            VkDeviceQueueCreateInfo queueInfo[1] = {};
            queueInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo[0].queueFamilyIndex = pQueueFamily;
            queueInfo[0].queueCount = 1;
            queueInfo[0].pQueuePriorities = queuePriority;
            VkDeviceCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createInfo.queueCreateInfoCount = sizeof(queueInfo) / sizeof(queueInfo[0]);
            createInfo.pQueueCreateInfos = queueInfo;
            createInfo.enabledExtensionCount = (uint32_t)deviceExt.Size;
            createInfo.ppEnabledExtensionNames = deviceExt.Data;
            if (vkCreateDevice(pPhysicalDevice, &createInfo, pAllocator, &pDevice) != VK_SUCCESS)
            {
                throw std::runtime_error("Vulkan: failed to createa a physical device");
                return;
            }
            vkGetDeviceQueue(pDevice, pQueueFamily, 0, &pQueue);
        }

        {
            /* create Descriptor Pool */
            VkDescriptorPoolSize poolSizes[] =
            {
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2 },
            };
            VkDescriptorPoolCreateInfo poolInfo = {};
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            poolInfo.maxSets = 4;
            poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
            poolInfo.pPoolSizes = poolSizes;
            if (vkCreateDescriptorPool(pDevice, &poolInfo, pAllocator, &pDescriptorPool) != VK_SUCCESS)
            {
                throw std::runtime_error("Vulkan: failed to create descriptor pool");
                return;
            }
        }
    }

    void renderer::setupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height)
    {
        wd->Surface = surface;

        /* Check for WSI support */
        VkBool32 res;
        vkGetPhysicalDeviceSurfaceSupportKHR(pPhysicalDevice, pQueueFamily, wd->Surface, &res);
        if (res != VK_TRUE)
        {
            fprintf(stderr, "Error no WSI support on physical device 0\n");
            exit(-1);
        }

        /* select Surface Format */
        const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
        const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(pPhysicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

        /* present mode */
        VkPresentModeKHR presentModes[] = { VK_PRESENT_MODE_FIFO_KHR };
        wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(pPhysicalDevice, wd->Surface, &presentModes[0], IM_ARRAYSIZE(presentModes));
        
        /* create SwapChain, RenderPass, Framebuffer, etc. */
        IM_ASSERT(pMinImageCount >= 2);
        ImGui_ImplVulkanH_CreateOrResizeWindow(pInstance, pPhysicalDevice, pDevice, wd, pQueueFamily, pAllocator, width, height, pMinImageCount);
    }

    void renderer::cleanupVulkan()
    {
        vkDestroyDescriptorPool(pDevice, pDescriptorPool, pAllocator);
        vkDestroyDevice(pDevice, pAllocator);
        vkDestroyInstance(pInstance, pAllocator);
    }

    void renderer::cleanupVulkanWindow()
    {
        ImGui_ImplVulkanH_DestroyWindow(pInstance, pDevice, &pMainWindowData, pAllocator);
    }

    void renderer::frameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* drawData)
    {
        /* start a new frame */
        VkSemaphore imageAcquiredSemaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
        VkSemaphore renderCompleteSemaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
        VkResult err = vkAcquireNextImageKHR(pDevice, wd->Swapchain, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE, &wd->FrameIndex);
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            pSwapChainRebuild = true;
            return;
        }
        if (err != VK_SUCCESS)
        {
            throw std::runtime_error("Vulkan: failed to get next image");
            return;
        }

        ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
        {
            /* wait indefinitely for the operation's completion instead of periodically checking */
            if (vkWaitForFences(pDevice, 1, &fd->Fence, VK_TRUE, UINT64_MAX) != VK_SUCCESS)
            {
                throw std::runtime_error("Vulkan: failed wait for fences operation");
                return;
            }
            if (vkResetFences(pDevice, 1, &fd->Fence) != VK_SUCCESS)
            {
                throw std::runtime_error("Vulkan: failed to reset fences");
                return;
            }
        }
        {
            /* reuse command pool */
            if (vkResetCommandPool(pDevice, fd->CommandPool, 0) != VK_SUCCESS)
            {
                throw std::runtime_error("Vulkan: failed to reset command pool");
                return;
            }
            VkCommandBufferBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            /* begins the command buffer recording */
            if (vkBeginCommandBuffer(fd->CommandBuffer, &info) != VK_SUCCESS)
            {
                throw std::runtime_error("Vulkan: failed to reset command buffer");
                return;
            }
        }
        {
            VkRenderPassBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass = wd->RenderPass;
            info.framebuffer = fd->Framebuffer;
            info.renderArea.extent.width = wd->Width;
            info.renderArea.extent.height = wd->Height;
            info.clearValueCount = 1;
            info.pClearValues = &wd->ClearValue;
            vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
        }

        /* record dear imgui primitives into command buffer */
        ImGui_ImplVulkan_RenderDrawData(drawData, fd->CommandBuffer);

        /* submit command buffer */
        vkCmdEndRenderPass(fd->CommandBuffer);
        {
            VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            VkSubmitInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            info.waitSemaphoreCount = 1;
            info.pWaitSemaphores = &imageAcquiredSemaphore;
            info.pWaitDstStageMask = &waitStage;
            info.commandBufferCount = 1;
            info.pCommandBuffers = &fd->CommandBuffer;
            info.signalSemaphoreCount = 1;
            info.pSignalSemaphores = &renderCompleteSemaphore;

            /* end command buffer recording and submit it */
            if (vkEndCommandBuffer(fd->CommandBuffer) != VK_SUCCESS)
            {
                throw std::runtime_error("Vulkan: failed end command buffer");
                return;
            }
            if (vkQueueSubmit(pQueue, 1, &info, fd->Fence) != VK_SUCCESS)
            {
                throw std::runtime_error("Vulkan: failed to submit a queue");
                return;
            }
        }
    }

    void renderer::framePresent(ImGui_ImplVulkanH_Window* wd)
    {
        if (pSwapChainRebuild)
            return;
        VkSemaphore renderCompleteSemaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
        VkPresentInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &renderCompleteSemaphore;
        info.swapchainCount = 1;
        info.pSwapchains = &wd->Swapchain;
        info.pImageIndices = &wd->FrameIndex;
        
        VkResult err = vkQueuePresentKHR(pQueue, &info);
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            pSwapChainRebuild = true;
            return;
        }
        if (err != VK_SUCCESS)
        {
            throw std::runtime_error("Vulkan: failed to present queue");
            return;
        }
        /* use the next set of semapthores */
        wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount;
    }
}

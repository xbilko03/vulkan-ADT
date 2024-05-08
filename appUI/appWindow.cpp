/*
* Name		    : appWindow.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file of the Vulkan window and image rendering VkDebuggerApp
*
* This template is available online by: Omar Cornut (ocornut) : MIT license
* https://github.com/ocornut/imgui/tree/master/examples/example_glfw_vulkan
* Minor adjustments: Jozef Bilko (xbilko03), supervised by Ing. Ján Peciva Ph.D.
*/
#include "appWindow.hpp"
#include "callbacks.h"
#include "events.hpp"

namespace details {
    /* helper function to find Vulkan memory type bits.See ImGui_ImplVulkan_MemoryType() in imgui_impl_vulkan.cpp */
    uint32_t appWindow::findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties mem_properties;
        vkGetPhysicalDeviceMemoryProperties(pPhysicalDevice, &mem_properties);

        for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
            if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;

        return 0xFFFFFFFF; // Unable to find memoryType
    }
    /* loads an image texture and renders it using imGui and Vulkan */
    bool appWindow::loadImageTexture(unsigned long long ID, int width, int height, int channels, void* imageData)
    {
        /* free last texture if exists */
        if (loadedImages.count(ID))
        {
            removeTexture(&loadedImages[ID]);
        }

        currentTexture.channels = 4;
        currentTexture.height = height;
        currentTexture.width = width;

        void* image_data = imageData;

        if (image_data == NULL)
        {
            return false;
        }

        /* calculate allocation size (in number of bytes) */
        size_t image_size = currentTexture.width * currentTexture.height * currentTexture.channels;

        {
            /* create the Vulkan image */
            VkImageCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            info.imageType = VK_IMAGE_TYPE_2D;
            info.format = VK_FORMAT_R8G8B8A8_UNORM;
            info.extent.width = currentTexture.width;
            info.extent.height = currentTexture.height;
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
            VkMemoryAllocateInfo alloc_info = {};
            alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            alloc_info.allocationSize = req.size;
            alloc_info.memoryTypeIndex = findMemoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            if (vkAllocateMemory(pDevice, &alloc_info, pAllocator, &currentTexture.imageMemory) != VK_SUCCESS)
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
            VkSamplerCreateInfo sampler_info{};
            sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            sampler_info.magFilter = VK_FILTER_LINEAR;
            sampler_info.minFilter = VK_FILTER_LINEAR;
            sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            /* outside image bounds just use border color */
            sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            sampler_info.minLod = -1000;
            sampler_info.maxLod = 1000;
            sampler_info.maxAnisotropy = 1.0f;
            if (vkCreateSampler(pDevice, &sampler_info, pAllocator, &currentTexture.sampler) != VK_SUCCESS)
            {
                return false;
            }
        }

        /* create Descriptor Set using ImGUI's implementation */
        currentTexture.DS = ImGui_ImplVulkan_AddTexture(currentTexture.sampler, currentTexture.imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        {
            /* create Upload Buffer */
            VkBufferCreateInfo buffer_info = {};
            buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            buffer_info.size = image_size;
            buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            if (vkCreateBuffer(pDevice, &buffer_info, pAllocator, &currentTexture.uploadBuffer) != VK_SUCCESS)
            {
                return false;
            }
            VkMemoryRequirements req;
            vkGetBufferMemoryRequirements(pDevice, currentTexture.uploadBuffer, &req);
            VkMemoryAllocateInfo alloc_info = {};
            alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            alloc_info.allocationSize = req.size;
            alloc_info.memoryTypeIndex = findMemoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            if (vkAllocateMemory(pDevice, &alloc_info, pAllocator, &currentTexture.uploadBufferMemory) != VK_SUCCESS)
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

            if (vkMapMemory(pDevice, currentTexture.uploadBufferMemory, 0, image_size, 0, &map) != VK_SUCCESS)
            {
                return false;
            }
            memcpy(map, image_data, image_size);
            VkMappedMemoryRange range[1] = {};
            range[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
            range[0].memory = currentTexture.uploadBufferMemory;
            range[0].size = image_size;
            if (vkFlushMappedMemoryRanges(pDevice, 1, range) != VK_SUCCESS)
            {
                return false;
            }
            vkUnmapMemory(pDevice, currentTexture.uploadBufferMemory);
        }

        VkCommandPool command_pool = pMainWindowData.Frames[pMainWindowData.FrameIndex].CommandPool;
        VkCommandBuffer command_buffer;
        {
            /* create a command buffer that will perform following steps when hit in the command queue */
            VkCommandBufferAllocateInfo alloc_info{};
            alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            alloc_info.commandPool = command_pool;
            alloc_info.commandBufferCount = 1;

            if (vkAllocateCommandBuffers(pDevice, &alloc_info, &command_buffer) != VK_SUCCESS)
            {
                return false;
            }

            VkCommandBufferBeginInfo begin_info = {};
            begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS)
            {
                return false;
            }
        }

        {
            /* copy to Image */
            VkImageMemoryBarrier copy_barrier[1] = {};
            copy_barrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            copy_barrier[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            copy_barrier[0].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            copy_barrier[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            copy_barrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            copy_barrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            copy_barrier[0].image = currentTexture.image;
            copy_barrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copy_barrier[0].subresourceRange.levelCount = 1;
            copy_barrier[0].subresourceRange.layerCount = 1;
            vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, copy_barrier);

            VkBufferImageCopy region = {};
            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.layerCount = 1;
            region.imageExtent.width = currentTexture.width;
            region.imageExtent.height = currentTexture.height;
            region.imageExtent.depth = 1;
            vkCmdCopyBufferToImage(command_buffer, currentTexture.uploadBuffer, currentTexture.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

            VkImageMemoryBarrier use_barrier[1] = {};
            use_barrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            use_barrier[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            use_barrier[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            use_barrier[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            use_barrier[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            use_barrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            use_barrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            use_barrier[0].image = currentTexture.image;
            use_barrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            use_barrier[0].subresourceRange.levelCount = 1;
            use_barrier[0].subresourceRange.layerCount = 1;
            vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, use_barrier);
        }

        {
            /* end command buffer */
            VkSubmitInfo end_info = {};
            end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            end_info.commandBufferCount = 1;
            end_info.pCommandBuffers = &command_buffer;
            if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS)
            {
                return false;
            }
            if (vkQueueSubmit(pQueue, 1, &end_info, VK_NULL_HANDLE) != VK_SUCCESS)
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

    /* helper function to cleanup an image loaded */
    void appWindow::removeTexture(textureData* tex_data)
    {
        vkFreeMemory(pDevice, tex_data->uploadBufferMemory, nullptr);
        vkDestroyBuffer(pDevice, tex_data->uploadBuffer, nullptr);
        vkDestroySampler(pDevice, tex_data->sampler, nullptr);
        vkDestroyImageView(pDevice, tex_data->imageView, nullptr);
        vkDestroyImage(pDevice, tex_data->image, nullptr);
        vkFreeMemory(pDevice, tex_data->imageMemory, nullptr);
        ImGui_ImplVulkan_RemoveTexture(tex_data->DS);
    }
    
    /* render frame */
    void appWindow::renderNewFrame()
    {
        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();
        const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
        if (!is_minimized)
        {
            wd->ClearValue.color.float32[0] = clearColor.x * clearColor.w;
            wd->ClearValue.color.float32[1] = clearColor.y * clearColor.w;
            wd->ClearValue.color.float32[2] = clearColor.z * clearColor.w;
            wd->ClearValue.color.float32[3] = clearColor.w;
            frameRender(wd, draw_data);
            framePresent(wd);
        }
    }

    /* start frame */
    void appWindow::startNewFrame()
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

    /* free imGui resources */
    void appWindow::cleanupImGui()
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

    /* free glfw resources */
    void appWindow::cleanupGLFW()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    /* initilize ImGui */
    void appWindow::imGuiInit()
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
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = pInstance;
        init_info.PhysicalDevice = pPhysicalDevice;
        init_info.Device = pDevice;
        init_info.QueueFamily = pQueueFamily;
        init_info.Queue = pQueue;
        init_info.PipelineCache = pPipelineCache;
        init_info.DescriptorPool = pDescriptorPool;
        init_info.Subpass = 0;
        init_info.MinImageCount = pMinImageCount;
        init_info.ImageCount = wd->ImageCount;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = pAllocator;
        init_info.CheckVkResultFn = vkErrorCallback;
        ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);

    }

    /* initialize framebuffer */
    void appWindow::framebufferInit()
    {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        wd = &pMainWindowData;
        setupVulkanWindow(wd, surface, w, h);
    }

    /* initialize surface */    
    void appWindow::surfaceInit()
    {
        if (glfwCreateWindowSurface(pInstance, window, pAllocator, &surface) != VK_SUCCESS)
        {
            throw std::runtime_error("GLFW: failed to initialize");
            return;
        }
    }

    /* initialize window */
    void appWindow::glfwWindowInit()
    {
        glfwSetErrorCallback(glfwErrorCallback);
        if (!glfwInit())
        {
            throw std::runtime_error("GLFW: failed to initialize");
        }

        /* create window with Vulkan context */
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(1280, 720, WINDOWNAME, nullptr, nullptr);
        if (!glfwVulkanSupported())
        {
            throw std::runtime_error("GLFW: vulkan not supported");
        }
    }

    /* initialize data */
    void appWindow::dataInit()
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

    /* returns true if the extension is available */
    bool appWindow::isExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension)
    {
        for (const VkExtensionProperties& p : properties)
            if (strcmp(p.extensionName, extension) == 0)
                return true;
        return false;
    }

    /* initializes the physicalDev setup and returns the handle to it */
    VkPhysicalDevice appWindow::setupVulkan_SelectPhysicalDevice()
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

    /* initializes Vulkan */
    void appWindow::setupVulkan()
    {
        uint32_t extensions_count = 0;
        ImVector<const char*> reqInstanceExtensions;
        const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
        for (uint32_t i = 0; i < extensions_count; i++)
            reqInstanceExtensions.push_back(glfw_extensions[i]);

        VkResult err;

        /* create Vulkan Instance */
        {
            VkInstanceCreateInfo create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

            /* enumerate available extensions */
            uint32_t properties_count;
            vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, nullptr);
            instanceExtensions.resize(properties_count);
            err = vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, instanceExtensions.Data);
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
                create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
            }
            #endif
            // Create Vulkan Instance
            create_info.enabledExtensionCount = (uint32_t)reqInstanceExtensions.Size;
            create_info.ppEnabledExtensionNames = reqInstanceExtensions.Data;
            if (vkCreateInstance(&create_info, pAllocator, &pInstance) != VK_SUCCESS)
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
            uint32_t properties_count;
            vkEnumerateDeviceExtensionProperties(pPhysicalDevice, nullptr, &properties_count, nullptr);
            deviceExtensions.resize(properties_count);
            vkEnumerateDeviceExtensionProperties(pPhysicalDevice, nullptr, &properties_count, deviceExtensions.Data);
            
            const float queue_priority[] = { 1.0f };
            VkDeviceQueueCreateInfo queue_info[1] = {};
            queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_info[0].queueFamilyIndex = pQueueFamily;
            queue_info[0].queueCount = 1;
            queue_info[0].pQueuePriorities = queue_priority;
            VkDeviceCreateInfo create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
            create_info.pQueueCreateInfos = queue_info;
            create_info.enabledExtensionCount = (uint32_t)deviceExt.Size;
            create_info.ppEnabledExtensionNames = deviceExt.Data;
            if (vkCreateDevice(pPhysicalDevice, &create_info, pAllocator, &pDevice) != VK_SUCCESS)
            {
                throw std::runtime_error("Vulkan: failed to createa a physical device");
                return;
            }
            vkGetDeviceQueue(pDevice, pQueueFamily, 0, &pQueue);
        }

        {
            /* create Descriptor Pool */
            VkDescriptorPoolSize pool_sizes[] =
            {
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
            };
            VkDescriptorPoolCreateInfo pool_info = {};
            pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            /* there can be only so many textures rendered at the same time */
            pool_info.maxSets = MAX_TEXTURES_COUNT;
            pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
            pool_info.pPoolSizes = pool_sizes;
            if (vkCreateDescriptorPool(pDevice, &pool_info, pAllocator, &pDescriptorPool) != VK_SUCCESS)
            {
                throw std::runtime_error("Vulkan: failed to create descriptor pool");
                return;
            }
        }
    }

    /* initializes the Vulkan window */
    void appWindow::setupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height)
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
        VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
        wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(pPhysicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
        
        /* create SwapChain, RenderPass, Framebuffer, etc. */
        IM_ASSERT(pMinImageCount >= 2);
        ImGui_ImplVulkanH_CreateOrResizeWindow(pInstance, pPhysicalDevice, pDevice, wd, pQueueFamily, pAllocator, width, height, pMinImageCount);
    }

    /* free Vulkan resources */
    void appWindow::cleanupVulkan()
    {
        vkDestroyDescriptorPool(pDevice, pDescriptorPool, pAllocator);
        vkDestroyDevice(pDevice, pAllocator);
        vkDestroyInstance(pInstance, pAllocator);
    }

    /* free Vulkan window resources */
    void appWindow::cleanupVulkanWindow()
    {
        ImGui_ImplVulkanH_DestroyWindow(pInstance, pDevice, &pMainWindowData, pAllocator);
    }

    /* renders frame and prepares it for the presenting */
    void appWindow::frameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data)
    {
        /* start a new frame */
        VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
        VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
        VkResult err = vkAcquireNextImageKHR(pDevice, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
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
        ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

        /* submit command buffer */
        vkCmdEndRenderPass(fd->CommandBuffer);
        {
            VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            VkSubmitInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            info.waitSemaphoreCount = 1;
            info.pWaitSemaphores = &image_acquired_semaphore;
            info.pWaitDstStageMask = &wait_stage;
            info.commandBufferCount = 1;
            info.pCommandBuffers = &fd->CommandBuffer;
            info.signalSemaphoreCount = 1;
            info.pSignalSemaphores = &render_complete_semaphore;

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

    /* presents the previously rendered frame */
    void appWindow::framePresent(ImGui_ImplVulkanH_Window* wd)
    {
        if (pSwapChainRebuild)
            return;
        VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
        VkPresentInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &render_complete_semaphore;
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

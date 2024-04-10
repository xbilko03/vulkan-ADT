#include "appWindow.hpp"
#include "callbacks.h"
#include "events.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" //texture test
#define MAX_TEXTURES_COUNT 800

#include <fstream> // texture test
#include <sstream> // texture test

namespace details {
    /* [Texture] Helper function to find Vulkan memory type bits.See ImGui_ImplVulkan_MemoryType() in imgui_impl_vulkan.cpp */
    uint32_t appWindow::findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties mem_properties;
        vkGetPhysicalDeviceMemoryProperties(g_PhysicalDevice, &mem_properties);

        for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
            if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;

        return 0xFFFFFFFF; // Unable to find memoryType
    }

    /* [Texture] Helper function to load an image with common settings and return a MyTextureData with a VkDescriptorSet as a sort of Vulkan pointer */
    bool appWindow::LoadTextureFromFile(const char* filename, MyTextureData* tex_data)
    {
        // Specifying 4 channels forces stb to load the image in RGBA which is an easy format for Vulkan
        tex_data->Channels = 4;
        //unsigned char* image_data = stbi_load(filename, &tex_data->Width, &tex_data->Height, 0, tex_data->Channels);

        tex_data->Height = 1024;
        tex_data->Width = 1024;
        std::ifstream file;

        file.open("C:\\Users\\jozef\\Desktop\\binary2", std::ios_base::binary);
        assert(file.is_open());
        std::stringstream buffer;
        buffer << file.rdbuf();
        
        //char* image_data = (char*)malloc(4194304);
        std::string str = buffer.str();
        unsigned char*  image_data = (unsigned char*)str.c_str();
        

        if (image_data == NULL)
            return false;

        // Calculate allocation size (in number of bytes)
        size_t image_size = tex_data->Width * tex_data->Height * tex_data->Channels;

        VkResult err;
        // Create the Vulkan image.
        {
            VkImageCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            info.imageType = VK_IMAGE_TYPE_2D;
            info.format = VK_FORMAT_R8G8B8A8_UNORM;
            info.extent.width = tex_data->Width;
            info.extent.height = tex_data->Height;
            info.extent.depth = 1;
            info.mipLevels = 1;
            info.arrayLayers = 1;
            info.samples = VK_SAMPLE_COUNT_1_BIT;
            info.tiling = VK_IMAGE_TILING_OPTIMAL;
            info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            err = vkCreateImage(g_Device, &info, g_Allocator, &tex_data->Image);
            check_vk_result(err);
            VkMemoryRequirements req;
            vkGetImageMemoryRequirements(g_Device, tex_data->Image, &req);
            VkMemoryAllocateInfo alloc_info = {};
            alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            alloc_info.allocationSize = req.size;
            alloc_info.memoryTypeIndex = findMemoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            err = vkAllocateMemory(g_Device, &alloc_info, g_Allocator, &tex_data->ImageMemory);
            check_vk_result(err);
            err = vkBindImageMemory(g_Device, tex_data->Image, tex_data->ImageMemory, 0);
            check_vk_result(err);
        }

        // Create the Image View
        {
            VkImageViewCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            info.image = tex_data->Image;
            info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            info.format = VK_FORMAT_R8G8B8A8_UNORM;
            info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            info.subresourceRange.levelCount = 1;
            info.subresourceRange.layerCount = 1;
            err = vkCreateImageView(g_Device, &info, g_Allocator, &tex_data->ImageView);
            check_vk_result(err);
        }

        // Create Sampler
        {
            VkSamplerCreateInfo sampler_info{};
            sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            sampler_info.magFilter = VK_FILTER_LINEAR;
            sampler_info.minFilter = VK_FILTER_LINEAR;
            sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT; // outside image bounds just use border color
            sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            sampler_info.minLod = -1000;
            sampler_info.maxLod = 1000;
            sampler_info.maxAnisotropy = 1.0f;
            err = vkCreateSampler(g_Device, &sampler_info, g_Allocator, &tex_data->Sampler);
            check_vk_result(err);
        }

        // Create Descriptor Set using ImGUI's implementation
        tex_data->DS = ImGui_ImplVulkan_AddTexture(tex_data->Sampler, tex_data->ImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        // Create Upload Buffer
        {
            VkBufferCreateInfo buffer_info = {};
            buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            buffer_info.size = image_size;
            buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            err = vkCreateBuffer(g_Device, &buffer_info, g_Allocator, &tex_data->UploadBuffer);
            check_vk_result(err);
            VkMemoryRequirements req;
            vkGetBufferMemoryRequirements(g_Device, tex_data->UploadBuffer, &req);
            VkMemoryAllocateInfo alloc_info = {};
            alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            alloc_info.allocationSize = req.size;
            alloc_info.memoryTypeIndex = findMemoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            err = vkAllocateMemory(g_Device, &alloc_info, g_Allocator, &tex_data->UploadBufferMemory);
            check_vk_result(err);
            err = vkBindBufferMemory(g_Device, tex_data->UploadBuffer, tex_data->UploadBufferMemory, 0);
            check_vk_result(err);
        }

        // Upload to Buffer:
        {
            void* map = NULL;

            err = vkMapMemory(g_Device, tex_data->UploadBufferMemory, 0, image_size, 0, &map);

            std::cout << map << std::endl;

            check_vk_result(err);
            memcpy(map, image_data, image_size);
            VkMappedMemoryRange range[1] = {};
            range[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
            range[0].memory = tex_data->UploadBufferMemory;
            range[0].size = image_size;
            err = vkFlushMappedMemoryRanges(g_Device, 1, range);
            check_vk_result(err);
            vkUnmapMemory(g_Device, tex_data->UploadBufferMemory);
        }

        // Release image memory using stb
        //stbi_image_free(image_data);

        // Create a command buffer that will perform following steps when hit in the command queue.
        // TODO: this works in the example, but may need input if this is an acceptable way to access the pool/create the command buffer.
        VkCommandPool command_pool = g_MainWindowData.Frames[g_MainWindowData.FrameIndex].CommandPool;
        VkCommandBuffer command_buffer;
        {
            VkCommandBufferAllocateInfo alloc_info{};
            alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            alloc_info.commandPool = command_pool;
            alloc_info.commandBufferCount = 1;

            err = vkAllocateCommandBuffers(g_Device, &alloc_info, &command_buffer);
            check_vk_result(err);

            VkCommandBufferBeginInfo begin_info = {};
            begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            err = vkBeginCommandBuffer(command_buffer, &begin_info);
            check_vk_result(err);
        }

        // Copy to Image
        {
            VkImageMemoryBarrier copy_barrier[1] = {};
            copy_barrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            copy_barrier[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            copy_barrier[0].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            copy_barrier[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            copy_barrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            copy_barrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            copy_barrier[0].image = tex_data->Image;
            copy_barrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copy_barrier[0].subresourceRange.levelCount = 1;
            copy_barrier[0].subresourceRange.layerCount = 1;
            vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, copy_barrier);

            VkBufferImageCopy region = {};
            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.layerCount = 1;
            region.imageExtent.width = tex_data->Width;
            region.imageExtent.height = tex_data->Height;
            region.imageExtent.depth = 1;
            vkCmdCopyBufferToImage(command_buffer, tex_data->UploadBuffer, tex_data->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

            VkImageMemoryBarrier use_barrier[1] = {};
            use_barrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            use_barrier[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            use_barrier[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            use_barrier[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            use_barrier[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            use_barrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            use_barrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            use_barrier[0].image = tex_data->Image;
            use_barrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            use_barrier[0].subresourceRange.levelCount = 1;
            use_barrier[0].subresourceRange.layerCount = 1;
            vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, use_barrier);
        }

        // End command buffer
        {
            VkSubmitInfo end_info = {};
            end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            end_info.commandBufferCount = 1;
            end_info.pCommandBuffers = &command_buffer;
            err = vkEndCommandBuffer(command_buffer);
            check_vk_result(err);
            err = vkQueueSubmit(g_Queue, 1, &end_info, VK_NULL_HANDLE);
            check_vk_result(err);
            err = vkDeviceWaitIdle(g_Device);
            check_vk_result(err);
        }

        return true;
    }

    /* [Texture] Helper function to cleanup an image loaded with LoadTextureFromFile */
    void appWindow::RemoveTexture(MyTextureData* tex_data)
    {
        vkFreeMemory(g_Device, tex_data->UploadBufferMemory, nullptr);
        vkDestroyBuffer(g_Device, tex_data->UploadBuffer, nullptr);
        vkDestroySampler(g_Device, tex_data->Sampler, nullptr);
        vkDestroyImageView(g_Device, tex_data->ImageView, nullptr);
        vkDestroyImage(g_Device, tex_data->Image, nullptr);
        vkFreeMemory(g_Device, tex_data->ImageMemory, nullptr);
        ImGui_ImplVulkan_RemoveTexture(tex_data->DS);
    }
    
    /* [Texture] texture test */
    void appWindow::CreateTexture()
    {
        bool ret = LoadTextureFromFile("C:\\Users\\jozef\\Desktop\\viking_room.png", &my_texture);
        IM_ASSERT(ret);
    }

    /* [Texture] texture test */
    void appWindow::ShowTexture()
    {
        ImGui::Begin("Vulkan Texture Test");
        ImGui::Text("pointer = %p", my_texture.DS);
        ImGui::Text("size = %d x %d", my_texture.Width, my_texture.Height);
        ImGui::Image((ImTextureID)my_texture.DS, ImVec2(my_texture.Width, my_texture.Height));
        ImGui::End();
    }

    /* render frame */
    void appWindow::renderNewFrame()
    {
        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();
        const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
        if (!is_minimized)
        {
            wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
            wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
            wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
            wd->ClearValue.color.float32[3] = clear_color.w;
            FrameRender(wd, draw_data);
            FramePresent(wd);
        }
    }

    /* start frame */
    void appWindow::startNewFrame()
    {
        glfwPollEvents();

        if (g_SwapChainRebuild)
        {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            if (width > 0 && height > 0)
            {
                ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
                ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, &g_MainWindowData, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
                g_MainWindowData.FrameIndex = 0;
                g_SwapChainRebuild = false;
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    
    /* free imGui */
    void appWindow::cleanupImGui()
    {
        auto err = vkDeviceWaitIdle(g_Device);
        check_vk_result(err);
        RemoveTexture(&my_texture);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    
    /* free glfw */
    void appWindow::cleanupGLFW()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    
    void appWindow::imGuiInit()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;        
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = g_Instance;
        init_info.PhysicalDevice = g_PhysicalDevice;
        init_info.Device = g_Device;
        init_info.QueueFamily = g_QueueFamily;
        init_info.Queue = g_Queue;
        init_info.PipelineCache = g_PipelineCache;
        init_info.DescriptorPool = g_DescriptorPool;
        init_info.Subpass = 0;
        init_info.MinImageCount = g_MinImageCount;
        init_info.ImageCount = wd->ImageCount;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = g_Allocator;
        init_info.CheckVkResultFn = check_vk_result;
        ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);

    }
    void appWindow::framebufferInit()
    {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        wd = &g_MainWindowData;
        SetupVulkanWindow(wd, surface, w, h);
    }
    void appWindow::surfaceInit()
    {
        VkResult err = glfwCreateWindowSurface(g_Instance, window, g_Allocator, &surface);
        check_vk_result(err);
    }

    void appWindow::glfwWindowInit()
    {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
        {
            throw std::runtime_error("GLFW: failed to initialize");
        }

        // Create window with Vulkan context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(1280, 720, WINDOWNAME, nullptr, nullptr);
        if (!glfwVulkanSupported())
        {
            throw std::runtime_error("GLFW: vulkan not supported");
        }
    }

    void appWindow::dataInit()
    {
        g_Allocator = nullptr;
        g_Instance = VK_NULL_HANDLE;
        g_PhysicalDevice = VK_NULL_HANDLE;
        g_Device = VK_NULL_HANDLE;
        g_QueueFamily = (uint32_t)-1;
        g_Queue = VK_NULL_HANDLE;
        g_DebugReport = VK_NULL_HANDLE;
        g_PipelineCache = VK_NULL_HANDLE;
        g_DescriptorPool = VK_NULL_HANDLE;
        g_MinImageCount = 2;
        g_SwapChainRebuild = false;
        clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    }

    bool appWindow::IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension)
    {
        for (const VkExtensionProperties& p : properties)
            if (strcmp(p.extensionName, extension) == 0)
                return true;
        return false;
    }

    /* prioritize discrete GPU, if not found, select integrated one, if there are none, return VK_NULL_HANDLE */
    VkPhysicalDevice appWindow::SetupVulkan_SelectPhysicalDevice()
    {
        VkResult err = vkEnumeratePhysicalDevices(g_Instance, &physicalDeviceCount, nullptr);
        check_vk_result(err);
        IM_ASSERT(physicalDeviceCount > 0);

        ImVector<VkPhysicalDevice> gpus;
        gpus.resize(physicalDeviceCount);
        err = vkEnumeratePhysicalDevices(g_Instance, &physicalDeviceCount, gpus.Data);
        check_vk_result(err);

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
            return VK_NULL_HANDLE; // no suitable device found
    }

    void appWindow::setupVulkan()
    {
        uint32_t extensions_count = 0;
        ImVector<const char*> reqInstanceExtensions;
        const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
        for (uint32_t i = 0; i < extensions_count; i++)
            reqInstanceExtensions.push_back(glfw_extensions[i]);

        VkResult err;

        // Create Vulkan Instance
        {
            VkInstanceCreateInfo create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

            // Enumerate available extensions
            uint32_t properties_count;
            vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, nullptr);
            instanceExtensions.resize(properties_count);
            err = vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, instanceExtensions.Data);
            check_vk_result(err);

            // Enable required extensions
            if (IsExtensionAvailable(instanceExtensions, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
                reqInstanceExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
            #ifdef VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
            if (IsExtensionAvailable(instanceExtensions, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME))
            {
                reqInstanceExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
                create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
            }
            #endif
            // Create Vulkan Instance
            create_info.enabledExtensionCount = (uint32_t)reqInstanceExtensions.Size;
            create_info.ppEnabledExtensionNames = reqInstanceExtensions.Data;
            err = vkCreateInstance(&create_info, g_Allocator, &g_Instance);
            check_vk_result(err);
    }

        // Select Physical Device (GPU)
        g_PhysicalDevice = SetupVulkan_SelectPhysicalDevice();

        // Select graphics queue family
        {
            uint32_t count;
            vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, nullptr);
            VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
            vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, queues);
            for (uint32_t i = 0; i < count; i++)
                if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    g_QueueFamily = i;
                    break;
                }
            free(queues);
            IM_ASSERT(g_QueueFamily != (uint32_t)-1);
        }

        // Create Logical Device (with 1 queue)
        {
            ImVector<const char*> deviceExt;
            deviceExt.push_back("VK_KHR_swapchain");

            // Enumerate physical device extension
            uint32_t properties_count;
            vkEnumerateDeviceExtensionProperties(g_PhysicalDevice, nullptr, &properties_count, nullptr);
            deviceExtensions.resize(properties_count);
            vkEnumerateDeviceExtensionProperties(g_PhysicalDevice, nullptr, &properties_count, deviceExtensions.Data);
            
            const float queue_priority[] = { 1.0f };
            VkDeviceQueueCreateInfo queue_info[1] = {};
            queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_info[0].queueFamilyIndex = g_QueueFamily;
            queue_info[0].queueCount = 1;
            queue_info[0].pQueuePriorities = queue_priority;
            VkDeviceCreateInfo create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
            create_info.pQueueCreateInfos = queue_info;
            create_info.enabledExtensionCount = (uint32_t)deviceExt.Size;
            create_info.ppEnabledExtensionNames = deviceExt.Data;
            err = vkCreateDevice(g_PhysicalDevice, &create_info, g_Allocator, &g_Device);
            check_vk_result(err);
            vkGetDeviceQueue(g_Device, g_QueueFamily, 0, &g_Queue);
        }

        // Create Descriptor Pool
        // The example only requires a single combined image sampler descriptor for the font image and only uses one descriptor set (for that)
        // If you wish to load e.g. additional textures you may need to alter pools sizes.
        {
            VkDescriptorPoolSize pool_sizes[] =
            {
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
            };
            VkDescriptorPoolCreateInfo pool_info = {};
            pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            pool_info.maxSets = MAX_TEXTURES_COUNT;
            pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
            pool_info.pPoolSizes = pool_sizes;
            err = vkCreateDescriptorPool(g_Device, &pool_info, g_Allocator, &g_DescriptorPool);
            check_vk_result(err);
        }
}

    void appWindow::SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height)
    {
        wd->Surface = surface;

        // Check for WSI support
        VkBool32 res;
        vkGetPhysicalDeviceSurfaceSupportKHR(g_PhysicalDevice, g_QueueFamily, wd->Surface, &res);
        if (res != VK_TRUE)
        {
            fprintf(stderr, "Error no WSI support on physical device 0\n");
            exit(-1);
        }

        // Select Surface Format
        const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
        const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(g_PhysicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

        // Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
        VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
        VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
        wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(g_PhysicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
        //printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

        // Create SwapChain, RenderPass, Framebuffer, etc.
        IM_ASSERT(g_MinImageCount >= 2);
        ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, wd, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
    }

    void appWindow::cleanupVulkan()
    {
        vkDestroyDescriptorPool(g_Device, g_DescriptorPool, g_Allocator);

#ifdef IMGUI_VULKAN_DEBUG_REPORT
        // Remove the debug report callback
        auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkDestroyDebugReportCallbackEXT");
        vkDestroyDebugReportCallbackEXT(g_Instance, g_DebugReport, g_Allocator);
#endif // IMGUI_VULKAN_DEBUG_REPORT

        vkDestroyDevice(g_Device, g_Allocator);
        vkDestroyInstance(g_Instance, g_Allocator);
    }

    void appWindow::cleanupVulkanWindow()
    {
        ImGui_ImplVulkanH_DestroyWindow(g_Instance, g_Device, &g_MainWindowData, g_Allocator);
    }

    void appWindow::FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data)
    {
        VkResult err;

        VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
        VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
        err = vkAcquireNextImageKHR(g_Device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            g_SwapChainRebuild = true;
            return;
        }
        check_vk_result(err);

        ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
        {
            err = vkWaitForFences(g_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
            check_vk_result(err);

            err = vkResetFences(g_Device, 1, &fd->Fence);
            check_vk_result(err);
        }
        {
            err = vkResetCommandPool(g_Device, fd->CommandPool, 0);
            check_vk_result(err);
            VkCommandBufferBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
            check_vk_result(err);
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

        // Record dear imgui primitives into command buffer
        ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

        // Submit command buffer
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

            err = vkEndCommandBuffer(fd->CommandBuffer);
            check_vk_result(err);
            err = vkQueueSubmit(g_Queue, 1, &info, fd->Fence);
            check_vk_result(err);
        }
    }

    void appWindow::FramePresent(ImGui_ImplVulkanH_Window* wd)
    {
        if (g_SwapChainRebuild)
            return;
        VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
        VkPresentInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &render_complete_semaphore;
        info.swapchainCount = 1;
        info.pSwapchains = &wd->Swapchain;
        info.pImageIndices = &wd->FrameIndex;
        VkResult err = vkQueuePresentKHR(g_Queue, &info);
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            g_SwapChainRebuild = true;
            return;
        }
        check_vk_result(err);
        wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount; // Now we can use the next set of semaphores
    }

}

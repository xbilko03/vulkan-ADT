#pragma once

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <list>

#include "winsock.h"


namespace details {
	class appWindow {
	public:
        appWindow()
        {

        }
        /* some of these might be just private */
        void imGuiInit();   
        void framebufferInit();
        void surfaceInit();
        void glfwWindowInit();
        void dataInit();
        bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);
        VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();
        void setupVulkan();
        void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);

        void cleanupVulkan();
        void cleanupVulkanWindow();
        void cleanupImGui();
        void cleanupGLFW();

        void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
        void FramePresent(ImGui_ImplVulkanH_Window* wd);

        void startNewFrame();
        void renderNewFrame();

        GLFWwindow* window;
        ImVec4 clear_color;

        void CreateTexture();
        void ShowTexture();

        uint32_t GetPhysicalDeviceCount() { return physicalDeviceCount; }
        std::list<VkPhysicalDeviceProperties> GetPhysicalDeviceProperties() { return physicalDevicePropertiesList; }
        ImVector<VkExtensionProperties> GetInstanceExtensions() { return instanceExtensions; }
        ImVector<VkExtensionProperties> GetDeviceExtensions() { return deviceExtensions; }
	private:
        // A struct to manage data related to one image in vulkan
        struct MyTextureData
        {
            VkDescriptorSet DS;         // Descriptor set: this is what you'll pass to Image()
            int             Width;
            int             Height;
            int             Channels;

            // Need to keep track of these to properly cleanup
            VkImageView     ImageView;
            VkImage         Image;
            VkDeviceMemory  ImageMemory;
            VkSampler       Sampler;
            VkBuffer        UploadBuffer;
            VkDeviceMemory  UploadBufferMemory;

            MyTextureData() { memset(this, 0, sizeof(*this)); }
        };
        MyTextureData my_texture;

        uint32_t findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties);
        bool LoadTextureFromFile(const char* filename, MyTextureData* tex_data);
        void RemoveTexture(MyTextureData* tex_data);
        ImVector<VkExtensionProperties> instanceExtensions;
        ImVector<VkExtensionProperties> deviceExtensions;


        #define WINDOWNAME "vkDetails"
        VkAllocationCallbacks* g_Allocator;
        VkInstance g_Instance;
        VkPhysicalDevice g_PhysicalDevice;
        VkDevice g_Device;
        uint32_t g_QueueFamily;
        VkQueue g_Queue;
        VkDebugReportCallbackEXT g_DebugReport;
        VkPipelineCache g_PipelineCache;
        VkDescriptorPool g_DescriptorPool;
        ImGui_ImplVulkanH_Window g_MainWindowData;
        int g_MinImageCount;
        bool g_SwapChainRebuild;
        VkSurfaceKHR surface;
        ImGui_ImplVulkanH_Window* wd;


        /* Physical devices */
        uint32_t physicalDeviceCount;
        std::list<VkPhysicalDeviceProperties> physicalDevicePropertiesList;

        DWORD __stdcall layerReceiver(LPVOID lpParameter);
    };
}
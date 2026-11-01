/*
* Name		    : renderer.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file of the Vulkan window and image rendering VkDebuggerApp
* 
* This template is available online by: Omar Cornut (ocornut) : MIT license
* https://github.com/ocornut/imgui/tree/master/examples/example_glfw_vulkan
* Minor adjustments: Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#pragma once

/* AI-Assisted: Claude Opus 5.5: syntax */
#include "communication/winsock.hpp"
/* AI-Assisted: end */
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
#include <map>

namespace vkdebug::ui {
	class renderer {
	public:
        /* init */
        /* initilize ImGui */
        void imGuiInit();
        /* initialize framebuffer */
        void framebufferInit();
        /* initialize surface */
        void surfaceInit();
        /* initialize window */
        void glfwWindowInit();
        /* initialize data */
        void dataInit();
        /* initializes the physicalDev setup and returns the handle to it */
        VkPhysicalDevice setupVulkan_SelectPhysicalDevice();
        /* initializes Vulkan */
        void setupVulkan();
        /* initializes the Vulkan window */
        void setupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);
        /* returns true if the extension is available */
        bool isExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);

        /* free Vulkan resources */
        void cleanupVulkan();
        /* free Vulkan window resources */
        void cleanupVulkanWindow();
        /* free imGui resources */
        void cleanupImGui();
        /* free glfw resources */
        void cleanupGLFW();

        /* renders frame and prepares it for the presenting */
        void frameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* drawData);
        /* presents the previously rendered frame */
        void framePresent(ImGui_ImplVulkanH_Window* wd);

        /* start frame */
        void startNewFrame();
        /* render frame */
        void renderNewFrame();

        GLFWwindow* window;
        ImVec4 clearColor;

        /* export properties about Vulkan */
        std::list<VkPhysicalDeviceProperties> getPhysicalDeviceProperties() { return physicalDevicePropertiesList; }
        ImVector<VkExtensionProperties> getInstanceExtensions() { return instanceExtensions; }
        ImVector<VkExtensionProperties> getDeviceExtensions() { return deviceExtensions; }

        /* loads an image texture and renders it using imGui and Vulkan */
        bool loadImageTexture(unsigned long long ID, int width, int height, int channels, void* imageData);

        /* return properties about texture */
        VkDescriptorSet getImageDS(unsigned long long ID) { return loadedImages[ID].DS; }
        unsigned long long getImageWidth(unsigned long long ID) { return loadedImages[ID].width; }
        unsigned long long getImageHeight(unsigned long long ID) { return loadedImages[ID].height; }

	private:
        /* a struct to manage data related to one image in vulkan */
        struct textureData
        {
            /* texture descriptor */
            VkDescriptorSet DS;
            unsigned long long width;
            unsigned long long height;
            unsigned long long channels;

            /* need to keep track of these to properly cleanup */
            VkImageView     imageView;
            VkImage         image;
            VkDeviceMemory  imageMemory;
            VkSampler       sampler;
            VkBuffer        uploadBuffer;
            VkDeviceMemory  uploadBufferMemory;

            textureData() { memset(this, 0, sizeof(*this)); }
        };

        /* current texture is after rendering mapped to the particular imageID */
        textureData currentTexture;
        std::map<unsigned long long, textureData> loadedImages;

        /* helper function to find Vulkan memory type bits.See ImGui_ImplVulkan_MemoryType() in imgui_impl_vulkan.cpp */
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        /* helper function to cleanup an image loaded */
        void removeTexture(textureData* texData);

        /* info about Vulkan */
        ImVector<VkExtensionProperties> instanceExtensions;
        ImVector<VkExtensionProperties> deviceExtensions;

        /* app settings */
        #define WINDOW_NAME "VkDebugger"

        /* settings to help with the VkDebuggerApp rendering */
        VkAllocationCallbacks* pAllocator;
        VkInstance pInstance;
        VkPhysicalDevice pPhysicalDevice;
        VkDevice pDevice;
        uint32_t pQueueFamily;
        VkQueue pQueue;
        VkDebugReportCallbackEXT pDebugReport;
        VkPipelineCache pPipelineCache;
        VkDescriptorPool pDescriptorPool;
        ImGui_ImplVulkanH_Window pMainWindowData;
        int pMinImageCount;
        bool pSwapChainRebuild;
        VkSurfaceKHR surface;
        ImGui_ImplVulkanH_Window* wd;

        /* physical devices */
        uint32_t physicalDeviceCount;
        std::list<VkPhysicalDeviceProperties> physicalDevicePropertiesList;
    };
}
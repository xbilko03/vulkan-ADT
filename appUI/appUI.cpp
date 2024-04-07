/*
* Name		: appUI.cpp
* Project	: A Debugging Tool for Vulkan API
* Director  : Ing. Ján Pečiva Ph.D.
* Author	: Jozef Bilko (xbilko03) {excluding the explicitly marked parts of the code}
* Licence   : MIT
*/
#include "appUI.hpp"
#include <iostream>

namespace details {

    void appUI::ShowImages(details::appWindow* window, details::events* dataObject)
    {
        ImGui::Begin("Images");

        auto imageList = (*dataObject).getImages();
        size_t imageCount = imageList.size();

        for (size_t i = 0; i < imageCount; i++)
        {
            std::string headerName = "vkImage #" + std::to_string(i) + '\0';
            if (ImGui::CollapsingHeader(headerName.c_str()))
            {
                std::list<events::vkImageStr>::iterator iterator = imageList.begin();
                std::advance(iterator, i);
                for (auto param : iterator->parameters)
                {
                    ImGui::Text(param.c_str());
                }
            }
        }

        ImGui::End();
    }
    void appUI::ShowCommandBuffers(details::appWindow* window, details::events* dataObject)
    {
        ImGui::Begin("CommandBuffers");

        auto cmdBuffList = (*dataObject).getCommandBuffers();
        size_t cmdBuffCount = cmdBuffList.size();

        for (size_t i = 0; i < cmdBuffCount; i++)
        {
            std::string headerName = "CommandBuffers #" + std::to_string(i) + '\0';
            if (ImGui::CollapsingHeader(headerName.c_str()))
            {
                std::list<events::vkCommandBuffersStr>::iterator iterator = cmdBuffList.begin();
                std::advance(iterator, i);
                for (auto param : iterator->parameters)
                {
                    ImGui::Text(param.c_str());
                }
            }
        }

        ImGui::End();
    }
    void appUI::ShowAppInfo(details::appWindow* window, details::events* dataObject)
    {
        ImGui::Begin("Vulkan Info");

        auto appInfo = (*dataObject).getAppInfo();
        for (auto item : appInfo.parameters)
            ImGui::Text((item).c_str());

        ImGui::End();
    }
    void appUI::ShowVulkanInfo(details::appWindow* window, details::events* dataObject)
    {
        ImGui::Begin("Vulkan Info");

        if (ImGui::CollapsingHeader("Available Physical Devices"))
        {
            uint32_t test = (*window).GetPhysicalDeviceCount();
            ImGui::Text(("Available physical devices: " + std::to_string(test)).c_str());

            auto devices = (*window).GetPhysicalDeviceProperties();

            for (auto& dev : devices)
            {
                if (ImGui::CollapsingHeader(dev.deviceName))
                {
                    ImGui::Text(("apiVersion: " + std::to_string(dev.apiVersion)).c_str());
                    ImGui::Text(("driverVersion: " + std::to_string(dev.driverVersion)).c_str());
                    ImGui::Text(("vendorID: " + std::to_string(dev.vendorID)).c_str());
                    ImGui::Text(("deviceID: " + std::to_string(dev.deviceID)).c_str());
                    ImGui::Text(("deviceType: " + std::to_string(dev.deviceType)).c_str());
                }
                /* INCOMPLETE ... */
            }
        }

        if (ImGui::CollapsingHeader("Available Instance Extensions"))
        {
            auto instanceExtensions = (*window).GetInstanceExtensions();
            uint32_t i = 0;
            for (auto item : instanceExtensions)
            {
                ImGui::Text((std::to_string(++i) + " extension: " + item.extensionName).c_str());
            }
        }

        if (ImGui::CollapsingHeader("Available Device Extensions"))
        {
            auto deviceExtensions = (*window).GetDeviceExtensions();
            uint32_t i = 0;
            for (auto item : deviceExtensions)
            {
                ImGui::Text((std::to_string(++i) + " extension: " + item.extensionName).c_str());
            }
        }
        
        ImGui::End();
    }
    void appUI::ShowBuffers(details::appWindow* window, details::events* dataObject)
    {
        ImGui::Begin("Vk_Buffers");
        auto bufferList = (*dataObject).getBuffers();
        size_t bufferCount = bufferList.size();

        for (size_t i = 0; i < bufferCount; i++)
        {
            std::string headerName = "vkBuffer #" + std::to_string(i) + '\0';
            if (ImGui::CollapsingHeader(headerName.c_str()))
            {
                std::list<events::vkBufferStr>::iterator iterator = bufferList.begin();
                std::advance(iterator, i);
                for (auto param : iterator->parameters)
                {
                    ImGui::Text(param.c_str());
                }
            }
        }

        ImGui::End();
    }
    void appUI::ShowApiCalls(details::appWindow *window, details::events *dataObject)
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Vk_API_Calls");


        auto callsList = (*dataObject).getApiCalls();
        size_t apiCount = callsList.size();
        size_t apiGroupCount = 1000;

        for (size_t i = 0; i < apiCount; i += apiGroupCount)
        {
            size_t range = i + apiGroupCount;
            std::string headerName = std::to_string(i) + "-" + std::to_string(range) + '\0';

            if (ImGui::CollapsingHeader(headerName.c_str()))
            {
                std::list<std::string>::iterator iterator = callsList.begin();
                std::advance(iterator, i);
                //34
                size_t commandsInBracket = apiCount - i;
                //4317
                if (commandsInBracket > apiGroupCount)
                {
                    commandsInBracket = apiGroupCount;
                }



                for (int row = 0; row < commandsInBracket; row++)
                {
                    std::string out = std::to_string(i + row) + ": " + iterator++->c_str() + '\0';
                    ImGui::Text(out.c_str());
                }
            }
        }
        ImGui::End();
    }

    void appUI::run()
    {
        details::appWindow window;
        details::events eventManager;

        eventManager.connectToLayer();
        
        /* init App */
        window.dataInit();
        window.glfwWindowInit();
        window.setupVulkan();
        window.surfaceInit();
        window.framebufferInit();
        window.imGuiInit();
        window.CreateTexture();

        bool apiCalls = false;
        bool buffers = false;
        bool demo = false;
        bool texture = false;
        bool vulkanInfo = false;
        bool appInfo = false;
        bool commandBuffers = false;
        bool images = false;
        /* window render loop */
        while (!glfwWindowShouldClose(window.window))
        {
            glfwWaitEvents();

            window.startNewFrame();

            /* imGui menu */            
            {
                ImGui::Begin("Menu");
                ImGui::Checkbox("Vulkan_Info", &vulkanInfo);
                ImGui::Checkbox("Api_Calls", &apiCalls);
                ImGui::Checkbox("Vk_Buffers", &buffers);
                ImGui::Checkbox("Demo_Window", &demo);
                ImGui::Checkbox("Texture_Test", &texture);
                ImGui::Checkbox("App_Info", &appInfo);
                ImGui::Checkbox("Vk_Command_Buffers", &commandBuffers);
                ImGui::Checkbox("Vk_Images", &images);
                ImGui::End();
            }

            if (vulkanInfo == true)
                ShowVulkanInfo(&window, &eventManager);
            if(apiCalls == true)
                ShowApiCalls(&window, &eventManager);
            if(buffers == true)
                ShowBuffers(&window, &eventManager);
            if(demo == true)
                ImGui::ShowDemoWindow(&demo);
            if(texture == true)
                window.ShowTexture();
            if (appInfo == true)
                ShowAppInfo(&window, &eventManager);
            if (commandBuffers == true)
                ShowCommandBuffers(&window, &eventManager);
            if (images == true)
                ShowImages(&window, &eventManager);

            window.renderNewFrame();
        }

        /* clean */
        window.cleanupImGui();
        window.cleanupGLFW();
        window.cleanupVulkanWindow();
        window.cleanupVulkan();

        return;
    }
}

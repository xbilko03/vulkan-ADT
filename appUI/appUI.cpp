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

    void appUI::ShowMemories(details::appWindow* window, details::events* dataObject)
    {
        ImGui::Begin("Memory");
        /*
        auto memoryList = (*dataObject).getMemories();
        size_t memoryCount = memoryList.size();

        for (size_t i = 0; i < memoryCount; i++)
        {
            std::string headerName = "vkMemory #" + std::to_string(i) + '\0';
            if (ImGui::CollapsingHeader(headerName.c_str()))
            {
                std::list<events::vkMemoryStr>::iterator iterator = memoryList.begin();
                std::advance(iterator, i);
                for (auto param : iterator->parameters)
                {
                    ImGui::Text(param.c_str());
                }
            }
        }
        */
        ImGui::End();
    }
    void appUI::ShowImages(details::appWindow* window, details::events* dataObject)
    {
        ImGui::Begin("Images");
        /*
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
        */
        ImGui::End();
    }
    void appUI::ShowCommandBuffers(details::appWindow* window, details::events* dataObject)
    {
        ImGui::Begin("CommandBuffers");
        /*
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
        */
        ImGui::End();
    }
    void appUI::ShowAppInfo(details::appWindow* window, details::events* dataObject)
    {
        ImGui::Begin("Vulkan Info");
        /*
        auto appInfo = (*dataObject).getAppInfo();
        for (auto item : appInfo.parameters)
            ImGui::Text((item).c_str());
        */
        ImGui::End();
    }
    void appUI::ShowVulkanInfo(details::appWindow* window, details::events* dataObject)
    {
        ImGui::Begin("Vulkan Info");
        /*
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
        
        */
        ImGui::End();
    }
    void appUI::ShowBuffers(details::appWindow* window, details::events* dataObject)
    {
        ImGui::Begin("Vk_Buffers");
        /*
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
        */
        ImGui::End();
    }
    void appUI::ShowApiCalls(details::appWindow *window, details::events *dataObject)
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Vk_API_Calls");

        auto calls = (*dataObject).getFrameCount();

        std::cout << calls << " is count. " << std::endl;


        auto callis = (*dataObject).getFrameCalls(0);
        for (auto item : callis)
        {
            std::cout << item.getName() << " is subject. " << std::endl;
        }




        static ImGuiTableFlags flags =
            ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
            | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
            | ImGuiTableFlags_ScrollY;
        //PushStyleCompact();
        //ImGui::CheckboxFlags("ImGuiTableFlags_SortMulti", &flags, ImGuiTableFlags_SortMulti);
        //ImGui::SameLine(); HelpMarker("When sorting is enabled: hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1).");
        //ImGui::CheckboxFlags("ImGuiTableFlags_SortTristate", &flags, ImGuiTableFlags_SortTristate);
        //ImGui::SameLine(); HelpMarker("When sorting is enabled: allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0).");
        //PopStyleCompact();
        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
        int MyItemColumnID_ID = 5;
        int MyItemColumnID_Name = 10;
        int MyItemColumnID_Action = 15;
        int MyItemColumnID_Quantity = 20;
        if (ImGui::BeginTable("table_sorting", 4, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 15), 0.0f))
        {            
            ImGui::TableSetupColumn("Call_ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_ID++);
            ImGui::TableSetupColumn("Vk_Call", ImGuiTableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Name++);
            ImGui::TableSetupColumn("Detail", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Action++);
            ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
            ImGui::TableHeadersRow();

            // Sort our data if sort specs have been changed!
            if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs())
                if (sort_specs->SpecsDirty)
                {
                    //MyItem::SortWithSortSpecs(sort_specs, items.Data, items.Size);
                    sort_specs->SpecsDirty = false;
                }

            // Demonstrate using clipper for large vertical lists
            ImGuiListClipper clipper;
            clipper.Begin(5); //size
            while (clipper.Step())
                for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
                {
                    // Display a data item
                    ImGui::PushID("item->ID");
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%04d", "item->ID");
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted("item->Name");
                    ImGui::TableNextColumn();
                    ImGui::SmallButton("None");
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", "item->Quantity");
                    ImGui::PopID();
                }
            ImGui::EndTable();
        }
        /*
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
        */
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
        bool memories = false;
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
                ImGui::Checkbox("Vk_Memory", &memories);
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
            if (memories == true)
                ShowMemories(&window, &eventManager);
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

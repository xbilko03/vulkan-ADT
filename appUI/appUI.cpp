/*
* Name		: appUI.cpp
* Project	: A Debugging Tool for Vulkan API
* Director  : Ing. Ján Pečiva Ph.D.
* Author	: Jozef Bilko (xbilko03) {excluding the explicitly marked parts of the code}
* Licence   : MIT
*/
#include "appUI.hpp"
#include <iostream>
#include <map>

namespace details {
    #define MAX_TABLE_SIZE 10

    std::map < std::string, bool> apiDetails;
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

        ImGui::Begin("Vk_API_Calls");

        unsigned long long frameRangesCount;
        unsigned long long framesCount = (*dataObject).getFrameCount();
        unsigned long long groupFrameSingleCount = 1000;
        frameRangesCount = framesCount / groupFrameSingleCount + 1;

        for (unsigned long long i = 0; i < frameRangesCount; i++)
        {
            std::string headerName = "frames " + std::to_string(i * groupFrameSingleCount) + "-" + std::to_string((i + 1) * groupFrameSingleCount) + '\0';
            if (ImGui::CollapsingHeader(headerName.c_str()))
            {
                unsigned long long framesPerThisRange;
                if (framesCount / ((i + 1) * groupFrameSingleCount) >= 1)
                {
                    framesPerThisRange = groupFrameSingleCount;
                }
                else
                {
                    framesPerThisRange = framesCount % groupFrameSingleCount;
                }
                for (unsigned long long index = 0; index < framesPerThisRange; index++)
                {
                    if (i + index == 0)
                    {
                        headerName = "before frames" + '\0';
                    }
                    else
                    {
                        headerName = "frame #" + std::to_string(i * groupFrameSingleCount + index) + '\0';
                    }
                    if (ImGui::CollapsingHeader(headerName.c_str()))
                    {
                        auto frame = (*dataObject).getFrameCalls(i * groupFrameSingleCount + index);

                        int tableSize = frame.size();
                        if (MAX_TABLE_SIZE < tableSize)
                            tableSize = MAX_TABLE_SIZE;
                        if (ImGui::BeginTable("table_sorting", 3, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * tableSize), 0.0f))
                        {
                            ImGui::TableSetupColumn("Call_ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
                            ImGui::TableSetupColumn("Vk_Call", ImGuiTableColumnFlags_WidthFixed, 0.0f, 1);
                            ImGui::TableSetupColumn("Parameters", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 2);
                            ImGui::TableHeadersRow();

                            /*
                            // Sort our data if sort specs have been changed!
                            if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs())
                                if (sort_specs->SpecsDirty)
                                {
                                    item::SortWithSortSpecs(sort_specs, frame.Data, frame.Size);
                                    sort_specs->SpecsDirty = false;
                                }
                            */

                            int tableItemID = 0;
                            for (auto item : frame)
                            {
                                // Display a data item
                                ImGui::PushID(tableItemID++);
                                ImGui::TableNextRow();
                                ImGui::TableNextColumn();
                                ImGui::Text("%llu", item.getID());
                                ImGui::TableNextColumn();
                                ImGui::TextUnformatted(item.getName().c_str());
                                ImGui::TableNextColumn();

                                std::string IdStr = ("vk_Call [ID=" + std::to_string(item.getID()) + "]");
                                ImGui::Checkbox("Detail", &(apiDetails[IdStr]));
                                if(apiDetails[IdStr])
                                {
                                    bool p_open;
                                    p_open = true;
                                    ImGui::Begin(IdStr.c_str(), &p_open, 0);
                                    ImGui::Text(IdStr.c_str());
                                    if (ImGui::BeginTable((IdStr + "table").c_str(), 2, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * tableSize), 0.0f))
                                    {
                                        ImGui::TableSetupColumn("Parameter_Name", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
                                        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, 0.0f, 1);
                                        ImGui::TableHeadersRow();
                                        for (auto itm : item.getParameters())
                                        {
                                            ImGui::TableNextRow();
                                            ImGui::TableNextColumn();
                                            ImGui::Text(itm.name.c_str());
                                            ImGui::TableNextColumn();
                                            ImGui::Text(itm.value.c_str());
                                        }
                                        ImGui::EndTable();
                                    }
                                    ImGui::End();
                                }
                                ImGui::PopID();
                            }
                            
                            ImGui::EndTable();
                        }
                    }
                }
            }
        }
            
        
        /*
        auto callis = (*dataObject).getFrameCalls(0);
        for (auto item : callis)
        {
            std::cout << item.getName() << " is subject. " << std::endl;
        }
        */



        
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

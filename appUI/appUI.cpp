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


    void appUI::ShowWarnings(details::appWindow* window, details::events* dataObject)
    {
        ImGui::Begin("Warnings");

        auto warningsList = (*dataObject).getWarningsList();

        for (auto item : *warningsList)
        {
            ImGui::Text(item.c_str());
        }

        ImGui::End();
    }
    void appUI::ShowMemories(details::appWindow* window, details::events* dataObject)
    {
        ImGui::Begin("Memory");
        
        unsigned long long memoryCount = (*dataObject).getMemoriesCount();

        for (unsigned long long i = 0; i < memoryCount;i++)
        {
            std::string hdrName = "vkMemory #" + std::to_string(i);
            std::string output = hdrName + "[" + (*dataObject).getMemoryState(i) + "]";
            if(ImGui::CollapsingHeader(output.c_str()))
            {
                output = (*dataObject).getMemoryPointer(i);
                ImGui::Text(output.c_str());

                output = std::to_string((*dataObject).getMemoryCulpritID(i));
                output += ":";
                output += (*dataObject).getMemoryCulpritName(i);
                ImGui::Text(output.c_str());

                ImGui::Text((*dataObject).getMemoryData(i).c_str());
            }
        }

        ImGui::End();
    }
    void appUI::ShowImages(details::appWindow* window, details::events* dataObject)
    {
        ImGui::Begin("Images");

        unsigned long long imageCount = (*dataObject).getImagesCount();

        ImGuiIO& io = ImGui::GetIO();
        for (unsigned long long i = 0; i < imageCount;i++)
        {
            std::string hdrName = "vkImage #" + std::to_string(i);
            std::string output = hdrName + "[" + (*dataObject).getImageState(i) + "]";
            if (ImGui::CollapsingHeader(output.c_str()))
            {
                output = (*dataObject).getImagePointer(i);
                ImGui::Text(output.c_str());

                output = std::to_string((*dataObject).getImageCulpritID(i));
                output += ":";
                output += (*dataObject).getImageCulpritName(i);
                ImGui::Text(output.c_str());

                ImGui::Text((*dataObject).getImageData(i).c_str());
                ImGui::Text("pointer = %p", (*window).getImageDS(i));
                ImGui::Text("size = %d x %d", (*window).getImageWidth(i), (*window).getImageHeight(i));

                if (ImGui::Button("Load Texture"))
                    (*dataObject).loadTexture(i);
                if (ImGui::CollapsingHeader("inspect texture"))
                {
                    ImTextureID my_tex_id = (ImTextureID)(*window).getImageDS(i);
                    float my_tex_w = (*window).getImageWidth(i);
                    float my_tex_h = (*window).getImageHeight(i);

                    ImVec2 pos = ImGui::GetCursorScreenPos();
                    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                    ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);

                    ImGui::Image((ImTextureID)(*window).getImageDS(i), ImVec2((*window).getImageWidth(i), (*window).getImageHeight(i)));
                    if (ImGui::BeginItemTooltip())
                    {
                        float region_sz = 32.0f;
                        float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
                        float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
                        float zoom = 4.0f;
                        if (region_x < 0.0f) { region_x = 0.0f; }
                        else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
                        if (region_y < 0.0f) { region_y = 0.0f; }
                        else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
                        ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
                        ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
                        ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
                        ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
                        ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
                        ImGui::EndTooltip();
                    }
                }
            }
        }
        ImGui::End();
    }
    void appUI::ShowBuffers(details::appWindow* window, details::events* dataObject)
    {
        ImGui::Begin("Vk_Buffers");

        unsigned long long bufferCount = (*dataObject).getBufferCount();

        for (unsigned long long i = 0; i < bufferCount;i++)
        {
            std::string hdrName = "vkBuffer #" + std::to_string(i);
            std::string output = hdrName + "[" + (*dataObject).getBufferState(i) + "]";
            if (ImGui::CollapsingHeader(output.c_str()))
            {
                output = (*dataObject).getBufferPointer(i);
                ImGui::Text(output.c_str());

                output = std::to_string((*dataObject).getBufferCulpritID(i));
                output += ":";
                output += (*dataObject).getBufferCulpritName(i);
                ImGui::Text(output.c_str());

                ImGui::Text((*dataObject).getBufferData(i).c_str());
            }
        }

        ImGui::End();
    }
    void appUI::ShowAppInfo(details::appWindow* window, details::events* dataObject)
    {
        ImGui::Begin("Vulkan Info");

        if (ImGui::CollapsingHeader("appInfo"))
        {
            auto appInfo = (*dataObject).getAppInfo();
            for (auto item : appInfo)
                ImGui::Text((item).c_str());
        }

        if (ImGui::CollapsingHeader("vkInfo"))
        {
            auto vkInfo = (*dataObject).getVkInfo();
            for (auto item : vkInfo)
                ImGui::Text((item).c_str());
        }
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
                        if (ImGui::BeginTable("table_sorting", 4, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * tableSize), 0.0f))
                        {
                            ImGui::TableSetupColumn("Call_ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
                            ImGui::TableSetupColumn("Vk_Call", ImGuiTableColumnFlags_WidthFixed, 0.0f, 1);
                            ImGui::TableSetupColumn("vkResult", ImGuiTableColumnFlags_WidthFixed, 0.0f, 2);
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
                                ImGui::Text("%llu", item->getID());
                                ImGui::TableNextColumn();
                                ImGui::TextUnformatted(item->getName().c_str());
                                ImGui::TableNextColumn();
                                if (item->getRetVal())
                                {
                                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "VK_SUCCESS");
                                }
                                else
                                {
                                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "NOT SUCCESS");
                                }
                                ImGui::TableNextColumn();

                                std::string IdStr = ("vk_Call [ID=" + std::to_string(item->getID()) + "]");
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
                                        for (auto itm : item->getParameters())
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
        ImGui::End();
    }

    void appUI::run()
    {
        details::appWindow window;
        details::events eventManager;

        eventManager.connectToLayer(&window);
        
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
        bool appInfo = false;
        bool images = false;
        bool memories = false;
        bool warnings = false;
        /* window render loop */
        while (!glfwWindowShouldClose(window.window))
        {
            glfwWaitEvents();

            window.startNewFrame();

            /* imGui menu */            
            {
                ImGui::Begin("Menu");
                ImGui::Checkbox("Api_Calls", &apiCalls);
                ImGui::Checkbox("Vk_Buffers", &buffers);
                ImGui::Checkbox("Vk_Images", &images);
                ImGui::Checkbox("Vk_Memory", &memories);

                ImGui::Checkbox("Warnings", &warnings);

                ImGui::Checkbox("App_Info", &appInfo);
                ImGui::Checkbox("Demo_Window", &demo);
                ImGui::End();
            }

            if(apiCalls == true)
                ShowApiCalls(&window, &eventManager);
            if(buffers == true)
                ShowBuffers(&window, &eventManager);
            if(demo == true)
                ImGui::ShowDemoWindow(&demo);
            if (warnings == true)
                ShowWarnings(&window, &eventManager);
            if (appInfo == true)
                ShowAppInfo(&window, &eventManager);
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

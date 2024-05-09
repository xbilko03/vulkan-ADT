/*
* Name		    : appUI.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file of the UI application
*
* Author: Jozef Bilko (xbilko03), supervised by Ing. Ján Peciva Ph.D.
*/
#include "appUI.hpp"
#include <iostream>
#include <map>

namespace details {
    /* render a window that shows history of warnings */
    void appUI::ShowWarnings(details::appWindow* window, details::events* dataObject)
    {
        /* if the window has been opened for the first time, adjust it for a better use */
        ImGui::SetNextWindowSize(ImVec2(512, 768), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(192, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Warnings", &warnings, ImGuiWindowFlags_NoCollapse);

        auto warningsList = (*dataObject).getWarningsList();
        for (auto item : *warningsList)
        {
            ImGui::Text(item.c_str());
        }

        ImGui::End();
    }
    /* render a window that shows history of breakpoint occurances */
    void appUI::ShowBreaks(details::appWindow* window, details::events* dataObject)
    {
        /* if the window has been opened for the first time, adjust it for a better use */
        ImGui::SetNextWindowSize(ImVec2(512, 768), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(192, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Breakpoints", &breaks, ImGuiWindowFlags_NoCollapse);

        auto breaksList = (*dataObject).getBreaksList();
        for (auto item : *breaksList)
        {
            ImGui::Text(item.c_str());
        }

        ImGui::End();
    }
    /* render a window that shows the state of the VkMemory */
    void appUI::ShowMemories(details::appWindow* window, details::events* dataObject)
    {
        /* if the window has been opened for the first time, adjust it for a better use */
        ImGui::SetNextWindowSize(ImVec2(512, 768), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(192, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("VkMemory", &memories, ImGuiWindowFlags_NoCollapse);
        
        unsigned long long memoryCount = (*dataObject).getMemoriesCount();
        /* make each memory expandable for detailed view */
        for (unsigned long long i = 0; i < memoryCount;i++)
        {
            std::string hdrName = "vkMemory #" + std::to_string(i);
            std::string output = hdrName + "[" + (*dataObject).getMemoryState(i) + "]";
            if(ImGui::CollapsingHeader(output.c_str()))
            {
                /* make each buffer expandable for detailed view */
                output = (*dataObject).getMemoryPointer(i);
                ImGui::SeparatorText("VkMemory details");
                output = (*dataObject).getBufferPointer(i);
                ImGui::Text(("VkMemory handle = " + output).c_str());
                output = std::to_string((*dataObject).getMemoryDataSize(i));
                ImGui::Text(("Size = " + output).c_str());

                if (dataObject->getCallsSettings() && output != "new")
                {
                    ImGui::SeparatorText("Relations");
                    output = std::to_string((*dataObject).getMemoryCulpritID(i));
                    output += ":";
                    output += (*dataObject).getMemoryCulpritName(i);
                    ImGui::Text(output.c_str());
                    output = (*dataObject).getMemoryBound(i);
                    if(output != "")
                        ImGui::Text(( "Bound VkObject Handle = " + output).c_str());
                }

                /* show data if there are any */
                ImGui::SeparatorText("Data");
                ImGui::Text((*dataObject).getMemoryData(i).c_str());
            }
        }
        ImGui::End();
    }
    /* render a window that shows the state of the VkImages */
    void appUI::ShowImages(details::appWindow* window, details::events* dataObject)
    {
        /* if the window has been opened for the first time, adjust it for a better use */
        ImGui::SetNextWindowSize(ImVec2(512, 768), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(192, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("VkImages", &images, ImGuiWindowFlags_NoCollapse);

        unsigned long long imageCount = (*dataObject).getImagesCount();

        ImGuiIO& io = ImGui::GetIO();
        for (unsigned long long i = 0; i < imageCount;i++)
        {
            std::string hdrName = "vkImage #" + std::to_string(i);
            std::string output = hdrName + "[" + (*dataObject).getImageState(i) + "]";
            if (ImGui::CollapsingHeader(output.c_str()))
            {
                ImGui::SeparatorText("VkImage details");
                output = (*dataObject).getImagePointer(i);
                ImGui::Text(("VkImage handle = " + output).c_str());
                output = std::to_string((*dataObject).getImageDataSize(i));
                ImGui::Text(("Size = " + output).c_str());

                if (dataObject->getCallsSettings() && output != "")
                {
                    ImGui::SeparatorText("Relations");
                    output = std::to_string((*dataObject).getImageCulpritID(i));
                    output += ":";
                    output += (*dataObject).getImageCulpritName(i);
                    ImGui::Text(output.c_str());
                    output = (*dataObject).getImageBound(i);
                    ImGui::Text(("VkMemory Handle = " + output).c_str());
                }

                /* show data if there are any */
                if ((*dataObject).getImageData(i) != "")
                {
                    ImGui::SeparatorText("Data");
                    ImGui::Text((*dataObject).getImageData(i).c_str());
                }

                /* give an option to render image */
                if ((*dataObject).getImageData(i) != "")
                {   
                    ImGui::SeparatorText("Image");
                    std::string buttonName = "Load Image Data #" + std::to_string(i);
                    if (ImGui::Button(buttonName.c_str()))
                    {
                        (*dataObject).loadTexture(i);
                        texLoaded[i] = true;
                    }
                    ImGui::Text("Dimensions = %d x %d", (*window).getImageWidth(i), (*window).getImageHeight(i));

                    buttonName = "Show in native size #" + std::to_string(i);
                    if (ImGui::Button(buttonName.c_str()))
                        texDetails[i] = true;

                    ImVec2 pos = ImGui::GetCursorScreenPos();
                    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                    ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);

                    ImTextureID my_tex_id = (ImTextureID)(*window).getImageDS(i);
                    float width = (*window).getImageWidth(i);
                    float height = (*window).getImageHeight(i);
                    if (texLoaded[i] == true)
                    {
                        if (width >= height)
                        {
                            double scale = IMAGE_REVIEW_SIZE / (double)width;
                            width *= scale;
                            height *= scale;
                        }
                        else
                        {
                            double scale = IMAGE_REVIEW_SIZE / (double)height;
                            width *= scale;
                            height *= scale;
                        }
                        ImGui::Image((ImTextureID)(*window).getImageDS(i), ImVec2(width, height));
                    }

                    if (texDetails[i])
                    {
                        /* show native size and give the option to zoom in */
                        std::string detailsStr = "texDetails #" + std::to_string(i);
                        ImGui::Begin(detailsStr.c_str(), &texDetails[i], ImGuiWindowFlags_HorizontalScrollbar);

                        ImGui::DragFloat("zoom", &zoom, IMAGE_ZOOM_SPEED, 1.0f, MAX_IMAGE_ZOOM);
                        float width = (*window).getImageWidth(i);
                        float height = (*window).getImageHeight(i);
                        width *= zoom;
                        height *= zoom;
                        ImGui::Image((ImTextureID)(*window).getImageDS(i), ImVec2(width, height));

                        ImGui::End();
                    }
                }
            }
        }
        ImGui::End();
    }
    /* render a window that shows the state of the VkBuffers */
    void appUI::ShowBuffers(details::appWindow* window, details::events* dataObject)
    {
        /* if the window has been opened for the first time, adjust it for a better use */
        ImGui::SetNextWindowSize(ImVec2(512, 768), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(192, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("VkBuffers", &buffers, ImGuiWindowFlags_NoCollapse);

        unsigned long long bufferCount = (*dataObject).getBufferCount();
        /* make each buffer expandable for detailed view */
        for (unsigned long long i = 0; i < bufferCount;i++)
        {
            std::string hdrName = "vkBuffer #" + std::to_string(i);
            std::string output = hdrName + "[" + (*dataObject).getBufferState(i) + "]";
            if (ImGui::CollapsingHeader(output.c_str()))
            {
                /* further details */
                ImGui::SeparatorText("VkBuffer details");
                output = (*dataObject).getBufferPointer(i);
                ImGui::Text(("VkBuffer handle = " + output).c_str());
                output = std::to_string((*dataObject).getBufferDataSize(i));
                ImGui::Text(("Size = " + output).c_str());

                if (dataObject->getCallsSettings() && output != "new")
                {
                    ImGui::SeparatorText("Relations");
                    output = std::to_string((*dataObject).getBufferCulpritID(i));
                    output += ":";
                    output += (*dataObject).getBufferCulpritName(i);
                    ImGui::Text(output.c_str());
                }

                /* show data if there are any */
                ImGui::SeparatorText("Data");
                ImGui::Text((*dataObject).getBufferData(i).c_str());
            }
        }

        ImGui::End();
    }
    /* render a window that shows details about Vulkan on the current device */
    void appUI::ShowAppInfo(details::appWindow* window, details::events* dataObject)
    {
        /* if the window has been opened for the first time, adjust it for a better use */
        ImGui::SetNextWindowSize(ImVec2(512, 768), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(192, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Vulkan Info", &appInfo, ImGuiWindowFlags_NoCollapse);

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
    /* render a window that shows the collected apiCalls */
    void appUI::ShowApiCalls(details::appWindow *window, details::events *dataObject)
    {
        /* if the window has been opened for the first time, adjust it for a better use */
        ImGui::SetNextWindowSize(ImVec2(512, 768), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(192, 0), ImGuiCond_FirstUseEver);
        
        /* table style and feature flags */
        static ImGuiTableFlags flags =
            ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable |
            ImGuiTableFlags_SortMulti | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | 
            ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollY || ImGuiTableFlags_SizingStretchProp;

        /* table will be sized based on text height */
        const float textHeight = ImGui::GetTextLineHeightWithSpacing();

        /* starts a new window */
        ImGui::Begin("Vk_API_Calls", &apiCalls, ImGuiWindowFlags_NoCollapse);

        /* to prevent clutter and performance issues, api calls are separated into frames which are separated into groups of frames */
        unsigned long long frameGroupsCount;
        unsigned long long framesCount = (*dataObject).getFrameCount();
        unsigned long long groupFrameSingleCount = MAX_FRAMES_IN_GROUP;
        frameGroupsCount = framesCount / groupFrameSingleCount + 1;

        /* for each frame group, */
        for (unsigned long long i = 0; i < frameGroupsCount; i++)
        {
            std::string headerName = "frames " + std::to_string(i * groupFrameSingleCount) + "-" + std::to_string((i + 1) * groupFrameSingleCount) + '\0';

            /* allow each frame group to be expanded for inspection */
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(7.0f, 7.0f));
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
                /* make each frame expandable in this group */
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Header, (ImVec4)ImColor::HSV(0.5f, 1.0f, 0.2f));
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
                    /* expand this particular frame */
                    if (ImGui::CollapsingHeader(headerName.c_str()))
                    {
                        /* frame details */
                        auto frame = (*dataObject).getFrameCalls(i * groupFrameSingleCount + index);

                        int tableSize = frame.size();
                        if (MAX_TABLE_SIZE < tableSize)
                            tableSize = MAX_TABLE_SIZE;
                        /* details about API calls in this frame */
                        if (ImGui::BeginTable("callTable", 4, flags, ImVec2(0.0f, textHeight * tableSize), 0.0f))
                        {
                            ImGui::TableSetupColumn("Call_ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
                            ImGui::TableSetupColumn("Vk_Call", ImGuiTableColumnFlags_WidthFixed, 0.0f, 1);
                            ImGui::TableSetupColumn("vkResult", ImGuiTableColumnFlags_WidthFixed, 0.0f, 2);
                            ImGui::TableSetupColumn("Detail", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 2);
                            ImGui::TableHeadersRow();

                            int tableItemID = 0;
                            /* put for each API call from this frame into this table */
                            for (auto item : frame)
                            {
                                /* table item ID */
                                ImGui::PushID(tableItemID++);
                                ImGui::TableNextRow();
                                ImGui::TableNextColumn();
                                /* call ID */
                                ImGui::Text("%llu", item->getID());
                                ImGui::TableNextColumn();
                                /* call name */
                                ImGui::TextUnformatted(item->getName().c_str());
                                ImGui::TableNextColumn();
                                /* call ret val */
                                if (item->getRetVal() == 0)
                                {
                                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "VK_SUCCESS");
                                }
                                else
                                {
                                    std::string out = "NOT SUCCESS [VK_RESULT=" + std::to_string(item->getRetVal()) + "]";
                                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), out.c_str() );
                                }
                                ImGui::TableNextColumn();
                                /* detailed call view */
                                std::string IdStr = ("vk_Call [ID=" + std::to_string(item->getID()) + "]");
                                if (ImGui::Button("",ImVec2(ImGui::GetContentRegionAvail().x,20)))
                                {
                                    apiDetails[IdStr] = true;
                                }
                                if(apiDetails[IdStr])
                                {
                                    /* detailed call view window */
                                    ImGui::Begin(IdStr.c_str(), &apiDetails[IdStr], 0);
                                    ImGui::Text(item->getName().c_str());
                                    /* for each parameter, put a type and its value into a table */
                                    if (ImGui::BeginTable((IdStr + "table").c_str(), 2, flags, ImVec2(0.0f, textHeight * tableSize), 0.0f))
                                    {
                                        ImGui::TableSetupColumn("Parameter_Name", ImGuiTableColumnFlags_DefaultSort, 0.0f, 0);
                                        ImGui::TableSetupColumn("Value", NULL, 0.0f, 1);
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
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
            }
            ImGui::PopStyleVar();
            

        }
        ImGui::End();
    }

    void appUI::run()
    {
        /* window object to handle rendering */
        details::appWindow window;
        /* event manager to handle data */
        details::events eventManager;

        /* connect to VkDebuggerLayer */
        eventManager.connectToLayer(&window);
        
        /* init VkDebuggerApp */
        window.dataInit();
        window.glfwWindowInit();
        window.setupVulkan();
        window.surfaceInit();
        window.framebufferInit();
        window.imGuiInit();

        /* main menu window flags */
        ImGuiWindowFlags mainWindowFlags = 0;
        mainWindowFlags |= ImGuiWindowFlags_NoMove;
        mainWindowFlags |= ImGuiWindowFlags_NoResize;
        mainWindowFlags |= ImGuiWindowFlags_NoCollapse;
        mainWindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        mainWindowFlags |= ImGuiWindowFlags_NoTitleBar;

        /* main menu and general colors */
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 1.0f, 0.4f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(0.5f, 1.0f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(0.5f, 1.0f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_CheckMark, (ImVec4)ImColor::HSV(0.0f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5f, 1.0f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.5f, 1.0f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.5f, 1.0f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_TitleBg, (ImVec4)ImColor::HSV(0.5f, 1.0f, 0.4f));
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, (ImVec4)ImColor::HSV(0.5f, 1.0f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_Header, (ImVec4)ImColor::HSV(0.5f, 1.0f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, (ImVec4)ImColor::HSV(0.5f, 1.0f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, (ImVec4)ImColor::HSV(0.5f, 1.0f, 0.7f));

        /* table colors */
        ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, (ImVec4)ImColor::HSV(0.5f, 1.0f, 0.4f));

        /* main menu window */
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 20.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(8.0f, 0.0f));

        /* window render loop */
        while (!glfwWindowShouldClose(window.window))
        {
            glfwWaitEvents();

            window.startNewFrame();

            /* imGui menu */            
            {
                /* resize */
                int width, height;
                glfwGetWindowSize(window.window, &width, &height);
                ImGui::SetNextWindowSize({ (float)width, (float)height }, 0);
                ImGui::SetNextWindowPos({0,0}, 0);

                ImGui::Begin("Menu", NULL, mainWindowFlags);

                /* enable the button for a feature in case it is activate */
                if(eventManager.getCallsSettings())
                    ImGui::Checkbox("Api_Calls", &apiCalls);
                if (eventManager.getBuffersSettings())
                    ImGui::Checkbox("Vk_Buffers", &buffers);
                if (eventManager.getImagesSettings())
                    ImGui::Checkbox("Vk_Images", &images);
                if (eventManager.getMemorySettings())
                    ImGui::Checkbox("Vk_Memory", &memories);
                if (eventManager.getWarningsSettings())
                    ImGui::Checkbox("Warnings", &warnings);
                if (eventManager.getBreaksSettings())
                    ImGui::Checkbox("Breakpoints", &breaks);

                ImGui::Checkbox("App_Info", &appInfo);

                if (ImGui::Button("Exit", ImVec2(128,64)))
                {
                    break;
                }

                ImGui::End();
            }

            /* global styles for all VkDebuggerApp windows */
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(8.0f, 0.0f));

            /* show a window containing said feature */
            if (apiCalls == true)
                ShowApiCalls(&window, &eventManager);
            if(buffers == true)
                ShowBuffers(&window, &eventManager);
            if(demo == true)
                ImGui::ShowDemoWindow(&demo);
            if (warnings == true)
                ShowWarnings(&window, &eventManager);
            if (breaks == true)
                ShowBreaks(&window, &eventManager);
            if (appInfo == true)
                ShowAppInfo(&window, &eventManager);
            if (images == true)
                ShowImages(&window, &eventManager);
            if (memories == true)
                ShowMemories(&window, &eventManager);

            ImGui::PopStyleVar(4);

            window.renderNewFrame();
        }

        ImGui::PopStyleVar(4);
        ImGui::PopStyleColor(12);
        ImGui::PopStyleColor(1);

        /* clean */
        window.cleanupImGui();
        window.cleanupGLFW();
        window.cleanupVulkanWindow();
        window.cleanupVulkan();

        return;
    }
}

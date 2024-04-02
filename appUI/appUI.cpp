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
    /* App thread */
    void appUI::ShowMenu(details::appWindow *window, details::events *dataObject)
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Vk_API_Calls");
        bool show_demo_window = true;
        ImGui::ShowDemoWindow(&show_demo_window);


        auto callsList = (*dataObject).getApiCalls();
        std::list<std::string>::iterator iterator = callsList.begin();
        size_t apiCount = callsList.size();
        size_t apiGroupCount = 10000;

        for (size_t i = 0; i < apiCount; i += apiGroupCount)
        {
            size_t range = i + apiGroupCount;
            std::string headerName = std::to_string(i) + "-" + std::to_string(range) + '\0';

            if (ImGui::CollapsingHeader(headerName.c_str()))
            {
                ImGuiListClipper clipper;
                clipper.Begin(apiGroupCount);
                while (clipper.Step())
                {
                    for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                    {
                        ImGui::Text(iterator++->c_str());
                    }
                }
            }
        }



        /*
        for (auto item : )
        {
            ImGui::Text(item.c_str());
        }
        */

        //ImGui::ColorEdit3("clear color", (float*)&(*window).clear_color); // Edit 3 floats representing a color


        /* cursors for later */
        /*
        if (ImGui::TreeNode("Mouse Cursors"))
        {
            const char* mouse_cursors_names[] = { "Arrow", "TextInput", "ResizeAll", "ResizeNS", "ResizeEW", "ResizeNESW", "ResizeNWSE", "Hand", "NotAllowed" };
            IM_ASSERT(IM_ARRAYSIZE(mouse_cursors_names) == ImGuiMouseCursor_COUNT);

            ImGuiMouseCursor current = ImGui::GetMouseCursor();
            ImGui::Text("Current mouse cursor = %d: %s", current, mouse_cursors_names[current]);
            ImGui::BeginDisabled(true);
            //ImGui::CheckboxFlags("io.BackendFlags: HasMouseCursors", &window.io.BackendFlags, ImGuiBackendFlags_HasMouseCursors);
            ImGui::EndDisabled();

            ImGui::Text("Hover to see mouse cursors:");

            for (int i = 0; i < ImGuiMouseCursor_COUNT; i++)
            {
                char label[32];
                sprintf(label, "Mouse cursor %d: %s", i, mouse_cursors_names[i]);
                ImGui::Bullet(); ImGui::Selectable(label, false);
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(i);
            }
            ImGui::TreePop();
        }

        */

        ImGui::End();
    }

    void appUI::run()
    {
        details::appWindow window;
        details::events eventManager;

        eventManager.connectToLayer();
        
        /* init UI */
        window.dataInit();
        window.glfwWindowInit();
        window.setupVulkan();
        window.surfaceInit();
        window.framebufferInit();
        window.imGuiInit();
        window.CreateTexture();


        /* window render loop */
        while (!glfwWindowShouldClose(window.window))
        {
            glfwWaitEvents();

            window.startNewFrame();

            /* imGui */
            ShowMenu(&window, &eventManager);
            window.ShowTexture();

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

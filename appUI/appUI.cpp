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
    void appUI::ShowApiCalls(details::appWindow *window, details::events *dataObject)
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Vk_API_Calls");
        bool show_demo_window = true;
        ImGui::ShowDemoWindow(&show_demo_window);


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
            ShowApiCalls(&window, &eventManager);
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

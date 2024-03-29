/*
* Name		: appUI.cpp
* Project	: A Debugging Tool for Vulkan API
* Director  : Ing. Ján Pečiva Ph.D.
* Author	: Jozef Bilko (xbilko03) {excluding the explicitly marked parts of the code}
* Licence   : MIT
*/
//#include <winsock2.h>
//#include "winsock.h"
//#include "events.h"

#include "appUI.hpp"
#include "appWindow.hpp"

#define DEFAULT_BUFLEN 500

namespace details {

    void appUI::newInfo(const char* input)
    {
        std::string inp = input;
        std::replace(inp.begin(), inp.end(), '!', '\0');
        testList.push_back(inp);
    }
    DWORD WINAPI appUI::listenForData(__in LPVOID lpParameter)
    {
        /* Create socket */
        int ret;
        SOCKET ClientSocket = INVALID_SOCKET;
        char recvbuf[DEFAULT_BUFLEN];
        uiWinsockInit(&ClientSocket);

        // Receive until the peer shuts down the connection
        do {
            ret = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
            if (ret > 0)
            {
                /* Parse data */
                newInfo(recvbuf);
                /* New data from layer received */
                glfwPostEmptyEvent();
            }
            else if (ret == 0)
            {
                /* closing connection */
            }
            else
            {
                /* recv failed */
                closesocket(ClientSocket);
                WSACleanup();
                //throw runtime error
            }
        } while (ret > 0);

        /* Destroy socket */
        uiWinsockExit(&ClientSocket);
        return 0;
    }
    void appUI::ShowMenu(details::appWindow *window)
    {
        static float f = 0.0f;
        static int counter = 0;


        ImGui::Begin("Layer receiver");
        /*
        bool show_demo_window = true;
        ImGui::ShowDemoWindow(&show_demo_window);
        */
        for (auto item : testList)
        {
            ImGui::Text(item.c_str());
        }



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
        DWORD mythreadId;
        /* Collect data from socket continuously */
        CreateThread(0, 0, appUI::listenForData, 0, 0, &mythreadId);

        details::appWindow window;
        
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
            //ShowMenu(&window);
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

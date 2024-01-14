/*
* Name		: appUI.cpp
* Project	: A Debugging Tool for Vulkan API
* Director  : Ing. Ján Pečiva Ph.D.
* Author	: Jozef Bilko (xbilko03)
*/
#include <stdio.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <GLFW/glfw3.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "winsock.h"
#include "events.h"

#define DEFAULT_BUFLEN 512

/*
* based on: ImGUI Omar Cornut's (ocornut) template
* more examples can be found in libs/imgui/examples or at
* https://github.com/ocornut/imgui/tree/master/examples
*/
#pragma comment(lib, "Ws2_32.lib")
struct addrinfo* result = NULL, * ptr = NULL, hints;
#define WINDOW_NAME "VK_DEBUGGER"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

void ShowTable(const char* dataType);

/* 
* based on: ImGUI Omar Cornut's template
* continuously render window and the UI within
*/
int RunUI()
{
    /* Create UI */
    if (!glfwInit())
        return 1;

    /* Create window with graphics context */
    GLFWwindow* window = glfwCreateWindow(1280, 720, WINDOW_NAME, nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    /* vsync = 1 */
    glfwSwapInterval(1);

    /* Setup Dear ImGui context */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    /* Setup Platform/Renderer backends */
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    /* UI state */
    bool show_demo_window = false;
    bool show_api_calls = false;
    bool show_api_calls_statistics = true;
    bool show_api_calls_history = false;

    bool show_command_buffer = false;
    bool show_command_buffer_statistics_total = true;
    bool show_command_buffer_statistics_current = false;

    bool show_another_window = false;
    ImVec4 background_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    /* Continuous UI updating */
    while (!glfwWindowShouldClose(window))
    {
        /* Sleep */
        glfwWaitEvents();
        /* Start the Dear ImGui frame */
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Menu");

        /*
        * demo window (for inspiration)
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        ImGui::Checkbox("Demo Window", &show_demo_window);
        */

        ImGui::Checkbox("API Calls", &show_api_calls);

        if (show_api_calls)
        {
            ImGui::Begin("API Calls");
            ImGui::Checkbox("Statistics", &show_api_calls_statistics);
            if (show_api_calls_statistics)
                ShowTable("api_statistics");
            ImGui::Checkbox("History", &show_api_calls_history);
            if (show_api_calls_history)
                ShowTable("api_history");
            ImGui::End();
        }
        ImGui::Checkbox("Command Buffer", &show_command_buffer);
        if (show_command_buffer)
        {
            ImGui::Begin("Command Buffer");
            ImGui::Checkbox("Total", &show_command_buffer_statistics_total);
            if (show_command_buffer_statistics_total)
                ShowTable("show_command_buffer_statistics_total");
            ImGui::Checkbox("Current", &show_command_buffer_statistics_current);
            if (show_command_buffer_statistics_current)
                ShowTable("show_command_buffer_statistics_current");
            ImGui::End();
        }

        if (ImGui::Button("Exit"))
            break;
        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(background_color.x * background_color.w, background_color.y * background_color.w, background_color.z * background_color.w, background_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    /* Destroy UI */
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    /* Destroy window */
    glfwDestroyWindow(window);
    glfwTerminate();

}
/* [THREAD] open connection, create socket, listen for data, then wake up the glfw window so it updates the received values */
DWORD WINAPI layerReceiver(__in LPVOID lpParameter)
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
            newLayerEvent(recvbuf);
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
            return 1;
        }
    } while (ret > 0);

    /* Destroy socket */
    uiWinsockExit(&ClientSocket);
    return 0;
}
/* create item in the table with 'value' value */
void AddTableItem(const char* value)
{
    char buf[64];
    sprintf(buf, "%s", value);
    ImGui::TableNextColumn();
    ImGui::Text(buf, ImVec2(-FLT_MIN, 0.0f));
}
/* create item in the table with 'value' value */
void AddTableItem(const int value)
{
    char buf[64];
    sprintf(buf, "%d", value);
    ImGui::TableNextColumn();
    ImGui::Text(buf, ImVec2(-FLT_MIN, 0.0f));
}
/* creates a table based on the dataType */
void ShowTable(const char* dataType)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild(dataType, ImVec2(0, 260), ImGuiChildFlags_Border, window_flags);

    if (strcmp(dataType, "api_statistics") == 0)
    {
        if (ImGui::BeginTable("split", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
        {
                for (int i = 0; i < GetApiStatsLen(); i++)
                {
                    AddTableItem(GetApiName(i).c_str());
                    AddTableItem(GetApiCount(i));
                }
            ImGui::EndTable();
        }
    }
    else if (strcmp(dataType, "api_history") == 0)
    {
        if (ImGui::BeginTable("split", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
        {
            int counter = 0;
            for (auto i : GetApiCallHistory())
            {
                AddTableItem(counter);
                AddTableItem(GetApiEventName(i).c_str());
                counter++;
            }
            ImGui::EndTable();
        }
    }
    else if (strcmp(dataType, "show_command_buffer_statistics_total") == 0)
    {
        if (ImGui::BeginTable("split", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
        {
            AddTableItem("Draw Calls");
            AddTableItem(GetCmdBuffVal(0));
            AddTableItem("Instances");
            AddTableItem(GetCmdBuffVal(1));
            AddTableItem("Vertices");
            AddTableItem(GetCmdBuffVal(2));
            ImGui::EndTable();
        }
    }
    else if (strcmp(dataType, "show_command_buffer_statistics_current") == 0)
    {
        if (ImGui::BeginTable("split", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
        {
            AddTableItem("Draw Calls");
            AddTableItem(GetCmdBuffVal(3));
            AddTableItem("Instances");
            AddTableItem(GetCmdBuffVal(4));
            AddTableItem("Vertices");
            AddTableItem(GetCmdBuffVal(5));
            ImGui::EndTable();
        }
    }

    ImGui::EndChild();
    ImGui::PopStyleVar();
}
/* create a new thread to collect data continuously while running the UI at the same time */
int main(int, char**)
{
    DWORD mythreadId;
    /* Collect data from socket continuously */
    CreateThread(0, 0, layerReceiver, 0, 0, &mythreadId);

    /* Open UI and show data continuosly until the user closes the window */
    if (RunUI() != 0)
        return 1;

    return 0;
}

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

#pragma comment(lib, "Ws2_32.lib")
struct addrinfo* result = NULL, * ptr = NULL, hints;
#define WINDOW_NAME "VK_DEBUGGER"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
DWORD WINAPI mythread(__in LPVOID lpParameter)
{
    /* Create socket */
    int iResult;
    SOCKET ClientSocket = INVALID_SOCKET;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    INIT_UI_WINSOCK(&ClientSocket);

    // Receive until the peer shuts down the connection
    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            /* Parse data */
            layer_event(recvbuf);
            /* New data from layer received */
            glfwPostEmptyEvent();
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed with error [server]: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
    } while (iResult > 0);

    /* Destroy socket */
    EXIT_UI_WINSOCK(&ClientSocket);
    printf("socket destroyed\n");

    return 0;
}
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
                    char buf[64];
                    sprintf(buf, "%s", GetApiName(i).c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text(buf, ImVec2(-FLT_MIN, 0.0f));
                    sprintf(buf, "%d", GetApiCount(i));
                    ImGui::TableNextColumn();
                    ImGui::Text(buf, ImVec2(-FLT_MIN, 0.0f));
                }
            ImGui::EndTable();
        }
    }
    else if (strcmp(dataType, "api_history") == 0)
    {
        if (ImGui::BeginTable("split", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
        {
            for (int i = 0; i < 100; i++)
            {
                char buf[64];
                sprintf(buf, "%d",i);
                ImGui::TableNextColumn();
                ImGui::Text(buf, ImVec2(-FLT_MIN, 0.0f));
                sprintf(buf, "call name");
                ImGui::TableNextColumn();
                ImGui::Text(buf, ImVec2(-FLT_MIN, 0.0f));
            }
            ImGui::EndTable();
        }
    }

    ImGui::EndChild();
    ImGui::PopStyleVar();
}
int main(int, char**)
{
    DWORD mythreadid;
    /* Collect data from socket continuously */
    CreateThread(0, 0, mythread, 0, 0, &mythreadid);

    /* Create UI */
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, WINDOW_NAME, nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    // Our state
    bool show_demo_window = false;
    bool show_api_calls = false;
    bool show_api_calls_statistics = true;
    bool show_api_calls_history = false;

    bool show_another_window = false;
    ImVec4 background_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        /* Create base UI */

        /* Refresh UI */

        glfwWaitEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::Begin("Menu");
        ImGui::Checkbox("Demo Window", &show_demo_window);

        ImGui::Checkbox("API Calls", &show_api_calls);

        if (show_api_calls)
        {
            ImGui::Begin("API Calls");
            ImGui::Checkbox("Statistics", &show_api_calls_statistics);
            if (show_api_calls_statistics)
            {
                ShowTable("api_statistics");
            }
            ImGui::Checkbox("History", &show_api_calls_history);
            if (show_api_calls_history)
            {
                ShowTable("api_history");
            }
            ImGui::End();
        }

        if (ImGui::Button("Exit"))
            break;
        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(background_color.x * background_color.w, background_color.y * background_color.w, background_color.z * background_color.w, background_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        // If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
        // you may need to backup/reset/restore other state, e.g. for current shader using the commented lines below.
        //GLint last_program;
        //glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
        //glUseProgram(0);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        //glUseProgram(last_program);

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

    return 0;
}

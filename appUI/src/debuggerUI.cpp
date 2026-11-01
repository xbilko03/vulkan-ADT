/*
* Name		    : debuggerUI.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file of the UI application
*
* Author: Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#include "../include/debuggerUI.hpp"
#include <iostream>
/* AI-Assisted: Claude Sonnet 5: syntax */
#include <fstream>
/* AI-Assisted: end */
#include <map>
/* AI-Assisted: Claude Sonnet 5: syntax */
#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <windows.h>
#include <commdlg.h>
/* AI-Assisted: end */

namespace vkdebug::ui {
    /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
    /* [xbilko03] get path to vkdebugger_recent_targets.txt using path of VkDebugger.exe */
    static std::filesystem::path recentTargetsFilePath() {
        char selfPath[MAX_PATH];
        GetModuleFileNameA(nullptr, selfPath, MAX_PATH);
        return std::filesystem::path(selfPath).parent_path() / "vkdebugger_recent_targets.txt";
    }

#define MAX_RECENT_TARGETS 5

    void debuggerUI::loadRecentTargets() {
        recentTargets.clear();
        std::ifstream file(recentTargetsFilePath());
        std::string line;
        while (std::getline(file, line) && recentTargets.size() < MAX_RECENT_TARGETS) {
            if (!line.empty())
                recentTargets.push_back(line);
        }
        recentTargetsLoaded = true;
    }

    void debuggerUI::saveRecentTargets() {
        std::ofstream file(recentTargetsFilePath(), std::ios::trunc);
        for (const std::string &path: recentTargets)
            file << path << "\n";
    }

    void debuggerUI::addRecentTarget(const std::string &path) {
        /* [xbilko03] copy given path to a targeted program and place it on top of the list of recent targets */
        std::string newPath = path;
        std::vector<std::string> updated;
        updated.push_back(newPath);
        /* [xbilko03] put earlier paths used into the list */
        for (size_t i = 0; i < recentTargets.size(); i++) {
            if (recentTargets[i] != newPath)
                updated.push_back(recentTargets[i]);
        }
        /* [xbilko03] check if updated list does not exceed the MAX_RECENT_TARGETS, if so, cut entries count to MAX_RECENT_TARGETS */
        if (updated.size() > MAX_RECENT_TARGETS)
            updated.resize(MAX_RECENT_TARGETS);
        recentTargets = updated;
        saveRecentTargets();
    }

    /* [xbilko03] frame delimiter options offered before launch, key written to the layer, label recognized by UI */
    struct frameDelimChoice {
        const char *key;
        const char *label;
    };

    static const frameDelimChoice frameDelims[] = {
        {"delim_vkAcquireNextImageKHR", "vkAcquireNextImageKHR"},
        {"delim_vkQueueSubmit", "vkQueueSubmit"},
        {"delim_vkQueuePresentKHR", "vkQueuePresentKHR"},
    };

    static void writeLine(std::ofstream &file, const std::string &key, const std::string &value) {
        file << "VK_LAYER_VUT_DebuggerLayer." << key << " = " << value << "\n";
    }

    /* [xbilko03] convert bool to text */
    static std::string boolText(bool value) {
        if (value)
            return "true";
        return "false";
    }

    void debuggerUI::writeLayerSettings(const std::string &settingsPath) {
        std::ofstream file(settingsPath, std::ios::trunc);

        char selfPath[MAX_PATH];
        GetModuleFileNameA(nullptr, selfPath, MAX_PATH);
        writeLine(file, "app_path", std::filesystem::path(selfPath).generic_string());
        writeLine(file, "frameless", boolText(cfgFrameless));
        writeLine(file, "frame_delim", frameDelims[cfgFrameDelim].key);
        writeLine(file, "log_calls", "true");
        writeLine(file, "log_buffers", "true");
        writeLine(file, "log_images", "true");
        writeLine(file, "log_memory", "true");
        writeLine(file, "enable_warnings", "false");
        writeLine(file, "fps_below", "0");
        writeLine(file, "fps_every", "0");

        writeLine(file, "enable_breaks", boolText(cfgEnableBreaks));
        writeLine(file, "break_fps_below", std::to_string(cfgBreakFpsBelow));
        writeLine(file, "break_frame_at", std::to_string(cfgBreakFrameAt));
        writeLine(file, "break_frame_every", std::to_string(cfgBreakFrameEvery));
        writeLine(file, "break_call_at", std::to_string(cfgBreakCallAt));
        writeLine(file, "break_call_every", std::to_string(cfgBreakCallEvery));
        writeLine(file, "break_memory_at", boolText(cfgBreakMemoryNew));
        writeLine(file, "break_buffer_at", boolText(cfgBreakBufferNew));
        writeLine(file, "break_image_at", boolText(cfgBreakImageNew));
    }

    void debuggerUI::launchTargetPath(const std::string &path) {
        char selfPath[MAX_PATH];
        GetModuleFileNameA(nullptr, selfPath, MAX_PATH);
        std::filesystem::path selfDir = std::filesystem::path(selfPath).parent_path();

        SetEnvironmentVariableA("VK_LAYER_PATH", selfDir.string().c_str());
        SetEnvironmentVariableA("VK_INSTANCE_LAYERS", "VK_LAYER_VUT_DebuggerLayer");
        /* [xbilko03] tell layer the UI is already running, so the layer does not start another VkDebugger instance */
        SetEnvironmentVariableA("VKDBG_MANUAL_LAUNCH", "1");

        std::string settingsPath = (selfDir / "vkdebugger_layer_settings.txt").string();
        writeLayerSettings(settingsPath);
        SetEnvironmentVariableA("VKDBG_SETTINGS_PATH", settingsPath.c_str());

        std::filesystem::path targetPath(path);
        std::string workingDir = targetPath.parent_path().string();
        /* [xbilko03] no folder in the path = work in current directory */
        const char *workingDirectory = nullptr;
        if (!workingDir.empty())
            workingDirectory = workingDir.c_str();

        STARTUPINFOA startupInfo{};
        startupInfo.cb = sizeof(startupInfo);
        PROCESS_INFORMATION processInfo{};

        BOOL launched = CreateProcessA(
            path.c_str(), nullptr, nullptr, nullptr, FALSE, 0, nullptr,
            workingDirectory,
            &startupInfo, &processInfo);

        /* AI-Assisted: end */
        /* AI-Assisted: Claude Opus 5.5: syntax */
        if (launched != FALSE)
        /* AI-Assisted: end */
        /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
        {
            CloseHandle(processInfo.hProcess);
            CloseHandle(processInfo.hThread);
            targetLaunched = true;
            /* [xbilko03] launch is successful, open breakpoints tab */
            requestedTab = TabBreakpoints;
        } else {
            std::cerr << "VkDebugger: failed to launch target at " << path << std::endl;
        }
    }

    void debuggerUI::launchTarget() {
        char filePath[MAX_PATH] = "";

        OPENFILENAMEA ofn{};
        ofn.lStructSize = sizeof(ofn);
        ofn.lpstrFilter = "Executable Files\0*.exe\0All Files\0*.*\0";
        ofn.lpstrFile = filePath;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

        if (!GetOpenFileNameA(&ofn))
            return;

        launchTargetPath(filePath);
        addRecentTarget(filePath);
    }

    /* AI-Assisted: end */
    /* AI-Assisted: Claude Opus 5.5: syntax */
    /* [xbilko03] render grid containing history of warnings|breakpoints */
    static void showTimedGrid(const char *id, std::vector<vkdebug::ui::events::timedRow> &rows) {
        ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
                                ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_ScrollY;
        if (ImGui::BeginTable(id, 2, flags, ImVec2(0.0f, ImGui::GetContentRegionAvail().y))) {
            ImGui::TableSetupColumn("Source", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, 120.0f);
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableHeadersRow();
            for (size_t i = 0; i < rows.size(); i++) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(rows[i].source.c_str());
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(rows[i].time.c_str());
            }
            ImGui::EndTable();
        }
    }

    /* AI-Assisted: end */
    void debuggerUI::showWarnings(vkdebug::ui::renderer *window, vkdebug::ui::events *dataObject) {
        /* AI-Assisted: Claude Opus 5.5: syntax */
        ImGui::BeginChild("Warnings");
        dataObject->getWarningRows(warningRows);
        showTimedGrid("warningsGrid", warningRows);
        ImGui::EndChild();
        /* AI-Assisted: end */
    }

    void debuggerUI::showBreaks(vkdebug::ui::renderer *window, vkdebug::ui::events *dataObject) {
        /* AI-Assisted: Claude Opus 5.5: syntax */
        ImGui::BeginChild("Breakpoints");
        /* AI-Assisted: end */

        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] status of GPU work that might get stuck (case of timeline semaphores) */
        std::vector<std::string> *timelineWaits = dataObject->getTimelineWaits();
        if (dataObject->getPausedState() && !timelineWaits->empty()) {
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f),
                               "GPU may be stuck: these submits wait on a timeline semaphore value that no queue signal or vkSignalSemaphore has provided yet.\n"
                               "If the thread that should provide it is paused, the GPU work will not finish until Continue.");
            for (std::string &wait: *timelineWaits)
                ImGui::BulletText("%s", wait.c_str());
            ImGui::Separator();
        }

        /* [xbilko03] grid of all breakpoint hits and user pauses */
        dataObject->getBreakRows(breakRows);
        showTimedGrid("breaksGrid", breakRows);

        ImGui::EndChild();
        /* AI-Assisted: end */
    }

    /* AI-Assisted: Claude Opus 5.5: syntax */
    void debuggerUI::showStatusBar(vkdebug::ui::events *dataObject) {
        int state = dataObject->getProgramState();
        bool isPaused = (state == vkdebug::ui::events::ProgramPaused);
        bool isRunning = (state == vkdebug::ui::events::ProgramRunning);
        const char *stateText = "Waiting for the application";
        ImVec4 barColor = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
        if (isRunning) {
            stateText = "Running";
            barColor = ImVec4(0.16f, 0.51f, 0.24f, 1.0f);
        } else if (isPaused) {
            stateText = "Paused - Unstable";
            if (dataObject->getStableState())
                stateText = "Paused - Stable";
            else if (dataObject->getStabilizingState())
                stateText = "Paused - Stabilizing";
            barColor = ImVec4(0.80f, 0.40f, 0.20f, 1.0f);
        } else if (state == vkdebug::ui::events::ProgramLost) {
            stateText = "Lost - application ended or crashed";
            barColor = ImVec4(0.67f, 0.16f, 0.16f, 1.0f);
        }

        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - STATUS_BAR_HEIGHT));
        ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, STATUS_BAR_HEIGHT));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, barColor);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(STATUS_BAR_PADDING, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 0.0f));
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoSavedSettings |
                                 ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoFocusOnAppearing;
        ImGui::Begin("##StatusBar", NULL, flags);

        /* buttons with a thin white frame and a faint fill so they read as buttons, a light shade when the mouse is over them; STATUS_BUTTON_INSET above and below so the frame does not touch the bar edges */
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.08f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.22f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 1.0f, 1.0f, 0.35f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.7f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 0.0f));
        float buttonHeight = STATUS_BAR_HEIGHT - 2.0f * STATUS_BUTTON_INSET;
        ImGui::SetCursorPosY(STATUS_BUTTON_INSET);

        ImGui::BeginDisabled(!isRunning || dataObject->getPauseRequestedState());
        if (ImGui::Button("Pause", ImVec2(0.0f, buttonHeight)))
            dataObject->pauseExecution();
        ImGui::EndDisabled();
        if (isPaused) {
            ImGui::SameLine();
            if (ImGui::Button("Continue", ImVec2(0.0f, buttonHeight)))
                dataObject->resumeExecution();
            ImGui::SameLine();
            ImGui::BeginDisabled(dataObject->getStableState() || dataObject->getStabilizingState());
            if (ImGui::Button("Stabilize", ImVec2(0.0f, buttonHeight)))
                dataObject->stabilizeExecution();
            ImGui::EndDisabled();
        }
        ImGui::PopStyleVar(3);

        /* state text at the right end, in the middle of the bar height, left of the 'i' hint button */
        float hintSize = STATUS_BAR_HEIGHT - 2.0f * STATUS_BUTTON_INSET;
        ImGui::SameLine();
        float textWidth = ImGui::CalcTextSize(stateText).x;
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - STATUS_BAR_PADDING - hintSize - 8.0f - textWidth);
        ImGui::SetCursorPosY((STATUS_BAR_HEIGHT - ImGui::GetTextLineHeight()) / 2.0f);
        ImGui::TextUnformatted(stateText);

        /* [xbilko03] hint popup for runtime states the target program may be in */
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - STATUS_BAR_PADDING - hintSize);
        ImGui::SetCursorPosY(STATUS_BUTTON_INSET);
        ImVec2 hintPos = ImGui::GetCursorScreenPos();
        if (ImGui::InvisibleButton("##stateHint", ImVec2(hintSize, hintSize)))
            ImGui::OpenPopup("stateHintPopup");
        ImVec2 center = ImVec2(hintPos.x + hintSize / 2.0f, hintPos.y + hintSize / 2.0f);
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        if (ImGui::IsItemHovered())
            drawList->AddCircleFilled(center, hintSize / 2.0f, IM_COL32(255, 255, 255, 56));
        drawList->AddCircle(center, hintSize / 2.0f - 0.5f, IM_COL32(255, 255, 255, 200), 0, 1.0f);
        ImVec2 iSize = ImGui::CalcTextSize("i");
        drawList->AddText(ImVec2(center.x - iSize.x / 2.0f, center.y - iSize.y / 2.0f), IM_COL32(255, 255, 255, 255),
                          "i");

        /* popup above the bar at the right edge; the line of the current state is white, the others grey */
        ImGui::PopStyleColor(5);
        ImGui::PopStyleVar(5);
        ImGui::PopStyleColor();
        ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x + viewport->Size.x - STATUS_BAR_PADDING,
                                       viewport->Pos.y + viewport->Size.y - STATUS_BAR_HEIGHT - 4.0f), ImGuiCond_Always,
                                ImVec2(1.0f, 1.0f));
        if (ImGui::BeginPopup("stateHintPopup")) {
            const char *hintNames[6] = {
                "Waiting for the application", "Running", "Paused - Unstable", "Paused - Stabilizing",
                "Paused - Stable", "Lost - application ended or crashed"
            };
            const char *hintTexts[6] = {
                "the UI waits until the target application starts and the layer connects.",
                "the application runs; Pause stops all its threads.",
                "some thread was stopped while it was changing an object or allocation (shown in the Threads tab), "
                "data shown for that object may be half-written. Stabilize lets these threads finish that call.",
                "the threads are finishing the call they were in the middle of, then they stop again.",
                "no thread is in the middle of changing an object or allocation, the shown data is consistent.",
                "the connection to the application ended: it exited, crashed or was closed."
            };
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 40.0f);
            for (int i = 0; i < 6; i++) {
                ImVec4 color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
                if (strcmp(hintNames[i], stateText) == 0)
                    color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                ImGui::TextColored(color, "%s - %s", hintNames[i], hintTexts[i]);
            }
            ImGui::PopTextWrapPos();
            ImGui::EndPopup();
        }
        ImGui::End();
    }

    void debuggerUI::showThreads(vkdebug::ui::renderer *window, vkdebug::ui::events *dataObject) {
        ImGui::BeginChild("Threads");

        bool isPaused = dataObject->getPausedState();
        if (isPaused)
            ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.0f, 1.0f), "Paused - state from the layer.");
        else
            ImGui::TextDisabled("Running - state while paused is shown after Pause or a breakpoint.");

        dataObject->getThreadTabRows(threadTabRows);
        ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
                                ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_ScrollY;
        float height = ImGui::GetContentRegionAvail().y;
        if (isPaused)
            height -= ImGui::GetTextLineHeightWithSpacing();
        if (ImGui::BeginTable("threadsGrid", 7, flags, ImVec2(0.0f, height))) {
            ImGui::TableSetupColumn("Thread ID", ImGuiTableColumnFlags_WidthFixed, 90.0f);
            ImGui::TableSetupColumn("State", ImGuiTableColumnFlags_WidthFixed, 230.0f);
            ImGui::TableSetupColumn("Last call", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Call ID", ImGuiTableColumnFlags_WidthFixed, 90.0f);
            ImGui::TableSetupColumn("Inside call", ImGuiTableColumnFlags_WidthFixed, 90.0f);
            ImGui::TableSetupColumn("Finished calls", ImGuiTableColumnFlags_WidthFixed, 110.0f);
            ImGui::TableSetupColumn("May be changing", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableHeadersRow();
            for (size_t i = 0; i < threadTabRows.size(); i++) {
                vkdebug::ui::events::threadTabRow &row = threadTabRows[i];
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(row.id.c_str());
                ImGui::TableNextColumn();
                if (row.state == "running")
                    ImGui::TextUnformatted(row.state.c_str());
                else
                    ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.0f, 1.0f), "%s", row.state.c_str());
                ImGui::TableNextColumn();
                /* while paused the layer's current call wins (the agent data can be behind) */
                if (!row.current.empty())
                    ImGui::TextUnformatted(row.current.c_str());
                else
                    ImGui::TextUnformatted(row.lastCall.c_str());
                ImGui::TableNextColumn();
                if (!row.lastCall.empty())
                    ImGui::Text("%llu", row.lastCallID);
                ImGui::TableNextColumn();
                if (!row.lastCall.empty()) {
                    if (row.inCall)
                        ImGui::TextUnformatted("yes");
                    else
                        ImGui::TextUnformatted("no");
                }
                ImGui::TableNextColumn();
                ImGui::Text("%llu", row.callCount);
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(row.objects.c_str());
            }
            ImGui::EndTable();
        }
        if (isPaused)
            ImGui::Text("Threads not calling Vulkan: %llu (paused)", dataObject->getOtherThreadCount());

        ImGui::EndChild();
    }

    /* [xbilko03] find if filter's desired value is substring of text, return true if it matches (regardless of upper/lower case) */
    static bool callFilterMatches(const std::string &text, const char *filter) {
        if (filter[0] == '\0')
            return true;
        std::string lowerText = text;
        for (size_t i = 0; i < lowerText.size(); i++)
            lowerText[i] = (char) tolower((unsigned char) lowerText[i]);
        std::string lowerFilter = filter;
        for (size_t i = 0; i < lowerFilter.size(); i++)
            lowerFilter[i] = (char) tolower((unsigned char) lowerFilter[i]);
        return lowerText.find(lowerFilter) != std::string::npos;
    }

    /* [xbilko03] memory | buffer | image grids */
    static void showObjectGrid(const char *id, std::vector<objectRow> &rows,
                               char filters[OBJECT_COLUMN_COUNT][CALL_FILTER_SIZE],
                               int &page, std::map<unsigned long long, bool> &opened, const char *boundName) {
        std::vector<objectRow *> shown;
        for (size_t i = 0; i < rows.size(); i++) {
            objectRow &row = rows[i];
            if (!callFilterMatches(std::to_string(row.index), filters[0]) || !callFilterMatches(row.handle, filters[1])
                ||
                !callFilterMatches(row.state, filters[2]) || !callFilterMatches(row.size, filters[3]) ||
                !callFilterMatches(row.bound, filters[4]) || !callFilterMatches(row.createdBy, filters[5]))
                continue;
            shown.push_back(&row);
        }
        int pageCount = (int) ((shown.size() + CALLS_PER_PAGE - 1) / CALLS_PER_PAGE);
        if (pageCount < 1)
            pageCount = 1;
        if (page > pageCount - 1)
            page = pageCount - 1;
        if (page < 0)
            page = 0;
        size_t first = (size_t) page * CALLS_PER_PAGE;
        size_t last = first + CALLS_PER_PAGE;
        if (last > shown.size())
            last = shown.size();

        ImGui::PushID(id);
        ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter |
                                ImGuiTableFlags_BordersV | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp;
        float gridHeight = ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing();
        if (ImGui::BeginTable("grid", OBJECT_COLUMN_COUNT, flags, ImVec2(0.0f, gridHeight))) {
            ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 60.0f);
            ImGui::TableSetupColumn("Handle", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("State", ImGuiTableColumnFlags_WidthFixed, 110.0f);
            ImGui::TableSetupColumn("Data size", ImGuiTableColumnFlags_WidthFixed, 110.0f);
            ImGui::TableSetupColumn(boundName, ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Created by", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupScrollFreeze(0, 2);

            /* header row as tall as the filter row, same as the Vulkan API Calls grid */
            ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.5f));
            ImGui::TableNextRow(ImGuiTableRowFlags_Headers, ImGui::GetFrameHeight());
            for (int column = 0; column < OBJECT_COLUMN_COUNT; column++) {
                if (!ImGui::TableSetColumnIndex(column))
                    continue;
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);
                ImGui::TableHeader(ImGui::TableGetColumnName(column));
            }
            ImGui::PopStyleColor();

            ImGui::TableNextRow();
            for (int column = 0; column < OBJECT_COLUMN_COUNT; column++) {
                ImGui::TableSetColumnIndex(column);
                ImGui::PushID(column);
                ImGui::SetNextItemWidth(-FLT_MIN);
                if (ImGui::InputTextWithHint("##filter", "filter", filters[column], CALL_FILTER_SIZE))
                    page = 0;
                ImGui::PopID();
            }

            for (size_t i = first; i < last; i++) {
                objectRow &row = *shown[i];
                ImGui::PushID((int) row.index);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                bool isOpen = opened.count(row.index) != 0 && opened[row.index];
                if (ImGui::Selectable(std::to_string(row.index).c_str(), isOpen, ImGuiSelectableFlags_SpanAllColumns))
                    opened[row.index] = !isOpen;
                ImGui::TableSetColumnIndex(1);
                ImGui::TextUnformatted(row.handle.c_str());
                ImGui::TableSetColumnIndex(2);
                ImGui::TextUnformatted(row.state.c_str());
                ImGui::TableSetColumnIndex(3);
                ImGui::TextUnformatted(row.size.c_str());
                ImGui::TableSetColumnIndex(4);
                ImGui::TextUnformatted(row.bound.c_str());
                ImGui::TableSetColumnIndex(5);
                ImGui::TextUnformatted(row.createdBy.c_str());
                ImGui::PopID();
            }
            ImGui::EndTable();
        }

        ImGui::BeginDisabled(page <= 0);
        if (ImGui::Button("Prev"))
            page--;
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Page");
        ImGui::SameLine();
        int pageInput = page + 1;
        ImGui::SetNextItemWidth(80.0f);
        if (ImGui::InputInt("##page", &pageInput, 0, 0)) {
            if (pageInput < 1)
                pageInput = 1;
            if (pageInput > pageCount)
                pageInput = pageCount;
            page = pageInput - 1;
        }
        ImGui::SameLine();
        ImGui::Text("of %d (%llu objects)", pageCount, (unsigned long long) shown.size());
        ImGui::SameLine();
        ImGui::BeginDisabled(page >= pageCount - 1);
        if (ImGui::Button("Next"))
            page++;
        ImGui::EndDisabled();
        ImGui::PopID();
    }

    /* AI-Assisted: end */
    void debuggerUI::showMemories(vkdebug::ui::renderer *window, vkdebug::ui::events *dataObject) {
        /* AI-Assisted: Claude Opus 5.5: syntax */
        ImGui::BeginChild("VkMemory");
        /* [xbilko03] grid of all memory objects */
        unsigned long long memoryCount = (*dataObject).getMemoriesCount();
        objectRows.clear();
        for (unsigned long long i = 0; i < memoryCount; i++) {
            objectRow row;
            row.index = i;
            row.handle = (*dataObject).getMemoryPointer(i);
            row.state = (*dataObject).getMemoryState(i);
            row.size = std::to_string((*dataObject).getMemoryDataSize(i));
            row.bound = (*dataObject).getMemoryBound(i);
            row.createdBy = "";
            if (dataObject->getCallsSettings() && dataObject->getMemoryHasCulprit(i))
                row.createdBy = std::to_string((*dataObject).getMemoryCulpritID(i)) + ":" + (*dataObject).
                                getMemoryCulpritName(i);
            objectRows.push_back(row);
        }
        showObjectGrid("memoryGrid", objectRows, memoryFilters, memoryPage, memoryDetails, "Bound object");
        ImGui::EndChild();

        /* [xbilko03] details of opened memory objects in a window */
        for (std::map<unsigned long long, bool>::iterator openWindow = memoryDetails.begin();
             openWindow != memoryDetails.end(); openWindow++) {
            if (!openWindow->second)
                continue;
            unsigned long long i = openWindow->first;
            std::string hdrName = "vkMemory #" + std::to_string(i);
            std::string output = hdrName;
            ImGui::Begin(hdrName.c_str(), &openWindow->second, ImGuiWindowFlags_NoCollapse);
            /* AI-Assisted: end */
            {
                ImGui::SeparatorText("VkMemory details");
                output = (*dataObject).getMemoryPointer(i);
                ImGui::Text(("VkMemory handle = " + output).c_str());
                /* AI-Assisted: Claude Opus 5.5: syntax */
                /* [xbilko03] warning when another thread was frozen while working with this object (data might not be accurate anymore) */
                {
                    std::string note = dataObject->getChangingNote(output);
                    if (!note.empty())
                        ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.0f, 1.0f), "May be in the middle of a change: %s",
                                           note.c_str());
                }
                /* AI-Assisted: end */
                output = std::to_string((*dataObject).getMemoryDataSize(i));
                ImGui::Text(("Size = " + output).c_str());

                /* AI-Assisted: Claude Opus 5.5: syntax */
                if (dataObject->getCallsSettings() && output != "new" && dataObject->getMemoryHasCulprit(i)) {
                    ImGui::SeparatorText("Relations");
                    output = std::to_string((*dataObject).getMemoryCulpritID(i));
                    output += ":";
                    output += (*dataObject).getMemoryCulpritName(i);
                    ImGui::Text(output.c_str());
                    output = (*dataObject).getMemoryBound(i);
                    if (output != "")
                        ImGui::Text(("Bound VkObject Handle = " + output).c_str());
                }
                /* AI-Assisted: end */

                /* [xbilko03] show data if there are any */
                ImGui::SeparatorText("Data");
                ImGui::Text((*dataObject).getMemoryData(i).c_str());
            }
            /* AI-Assisted: Claude Opus 5.5: syntax */
            ImGui::End();
            /* AI-Assisted: end */
        }
    }

    void debuggerUI::showImages(vkdebug::ui::renderer *window, vkdebug::ui::events *dataObject) {
        /* AI-Assisted: Claude Opus 5.5: syntax */
        ImGui::BeginChild("VkImages");
        /* [xbilko03] grid of image objects */
        unsigned long long imageCount = (*dataObject).getImagesCount();
        objectRows.clear();
        for (unsigned long long i = 0; i < imageCount; i++) {
            objectRow row;
            row.index = i;
            row.handle = (*dataObject).getImagePointer(i);
            row.state = (*dataObject).getImageState(i);
            row.size = std::to_string((*dataObject).getImageDataSize(i));
            row.bound = (*dataObject).getImageBound(i);
            row.createdBy = "";
            if (dataObject->getCallsSettings() && dataObject->getImageHasCulprit(i))
                row.createdBy = std::to_string((*dataObject).getImageCulpritID(i)) + ":" + (*dataObject).
                                getImageCulpritName(i);
            objectRows.push_back(row);
        }
        showObjectGrid("imageGrid", objectRows, imageFilters, imagePage, imageDetails, "Bound memory");
        ImGui::EndChild();

        /* [xbilko03] details of every opened image in a window */
        for (std::map<unsigned long long, bool>::iterator openWindow = imageDetails.begin();
             openWindow != imageDetails.end(); openWindow++) {
            if (!openWindow->second)
                continue;
            unsigned long long i = openWindow->first;
            std::string hdrName = "vkImage #" + std::to_string(i);
            std::string output = hdrName;
            ImGui::Begin(hdrName.c_str(), &openWindow->second, ImGuiWindowFlags_NoCollapse);
            /* AI-Assisted: end */
            {
                ImGui::SeparatorText("VkImage details");
                output = (*dataObject).getImagePointer(i);
                ImGui::Text(("VkImage handle = " + output).c_str());
                /* AI-Assisted: Claude Opus 5.5: syntax */
                /* [xbilko03] warning for memory (data might be in middle of a change) */
                {
                    std::string note = dataObject->getChangingNote(output);
                    if (!note.empty())
                        ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.0f, 1.0f), "May be in the middle of a change: %s",
                                           note.c_str());
                }
                /* AI-Assisted: end */
                output = std::to_string((*dataObject).getImageDataSize(i));
                ImGui::Text(("Size = " + output).c_str());

                /* AI-Assisted: Claude Opus 5.5: syntax */
                if (dataObject->getCallsSettings() && output != "" && dataObject->getImageHasCulprit(i)) {
                    ImGui::SeparatorText("Relations");
                    output = std::to_string((*dataObject).getImageCulpritID(i));
                    output += ":";
                    output += (*dataObject).getImageCulpritName(i);
                    ImGui::Text(output.c_str());
                    output = (*dataObject).getImageBound(i);
                    ImGui::Text(("VkMemory Handle = " + output).c_str());
                }
                /* AI-Assisted: end */

                /* [xbilko03] show data if there are any */
                if ((*dataObject).getImageData(i) != "") {
                    ImGui::SeparatorText("Data");
                    ImGui::Text((*dataObject).getImageData(i).c_str());
                }

                /* [xbilko03] give an option to render image */
                if ((*dataObject).getImageData(i) != "") {
                    ImGui::SeparatorText("Image");
                    std::string buttonName = "Load Image Data #" + std::to_string(i);
                    if (ImGui::Button(buttonName.c_str())) {
                        (*dataObject).loadTexture(i);
                        texLoaded[i] = true;
                    }
                    ImGui::Text("Dimensions = %d x %d", (*window).getImageWidth(i), (*window).getImageHeight(i));

                    buttonName = "Show in native size #" + std::to_string(i);
                    if (ImGui::Button(buttonName.c_str())) {
                        /* AI-Assisted: Claude Sonnet 5: syntax */
                        if (!texLoaded[i]) {
                            (*dataObject).loadTexture(i);
                            texLoaded[i] = true;
                        }
                        /* AI-Assisted: end */
                        texDetails[i] = true;
                    }

                    /* AI-Assisted: Claude Opus 5.5: syntax */
                    float width = (float) (*window).getImageWidth(i);
                    float height = (float) (*window).getImageHeight(i);
                    /* AI-Assisted: end */
                    if (texLoaded[i] == true) {
                        if (width >= height) {
                            /* AI-Assisted: Claude Opus 5.5: syntax */
                            float scale = (float) IMAGE_REVIEW_SIZE / width;
                            /* AI-Assisted: end */
                            width *= scale;
                            height *= scale;
                        } else {
                            /* AI-Assisted: Claude Opus 5.5: syntax */
                            float scale = (float) IMAGE_REVIEW_SIZE / height;
                            /* AI-Assisted: end */
                            width *= scale;
                            height *= scale;
                        }
                        ImGui::Image((ImTextureID) (*window).getImageDS(i), ImVec2(width, height));
                    }

                    /* AI-Assisted: Claude Sonnet 5: syntax */
                    if (texDetails[i] && texLoaded[i])
                    /* AI-Assisted: end */
                    {
                        /* show native size and give the option to zoom in */
                        std::string detailsStr = "texDetails #" + std::to_string(i);
                        ImGui::Begin(detailsStr.c_str(), &texDetails[i], ImGuiWindowFlags_HorizontalScrollbar);

                        ImGui::DragFloat("zoom", &zoom, IMAGE_ZOOM_SPEED, 1.0f, MAX_IMAGE_ZOOM);
                        /* AI-Assisted: Claude Opus 5.5: syntax */
                        float width = (float) (*window).getImageWidth(i);
                        float height = (float) (*window).getImageHeight(i);
                        /* AI-Assisted: end */
                        width *= zoom;
                        height *= zoom;
                        ImGui::Image((ImTextureID) (*window).getImageDS(i), ImVec2(width, height));

                        ImGui::End();
                    }
                }
            }
            /* AI-Assisted: Claude Opus 5.5: syntax */
            ImGui::End();
            /* AI-Assisted: end */
        }
    }

    void debuggerUI::showBuffers(vkdebug::ui::renderer *window, vkdebug::ui::events *dataObject) {
        /* AI-Assisted: Claude Opus 5.5: syntax */
        ImGui::BeginChild("VkBuffers");
        /* [xbilko03] grid of all buffer objects */
        unsigned long long bufferCount = (*dataObject).getBufferCount();
        objectRows.clear();
        for (unsigned long long i = 0; i < bufferCount; i++) {
            objectRow row;
            row.index = i;
            row.handle = (*dataObject).getBufferPointer(i);
            row.state = (*dataObject).getBufferState(i);
            row.size = std::to_string((*dataObject).getBufferDataSize(i));
            row.bound = (*dataObject).getBufferBound(i);
            row.createdBy = "";
            if (dataObject->getCallsSettings() && dataObject->getBufferHasCulprit(i))
                row.createdBy = std::to_string((*dataObject).getBufferCulpritID(i)) + ":" + (*dataObject).
                                getBufferCulpritName(i);
            objectRows.push_back(row);
        }
        showObjectGrid("bufferGrid", objectRows, bufferFilters, bufferPage, bufferDetails, "Bound memory");
        ImGui::EndChild();

        /* [xbilko03] details of every opened buffer in a window */
        for (std::map<unsigned long long, bool>::iterator openWindow = bufferDetails.begin();
             openWindow != bufferDetails.end(); openWindow++) {
            if (!openWindow->second)
                continue;
            unsigned long long i = openWindow->first;
            std::string hdrName = "vkBuffer #" + std::to_string(i);
            std::string output = hdrName;
            ImGui::Begin(hdrName.c_str(), &openWindow->second, ImGuiWindowFlags_NoCollapse);
            /* AI-Assisted: end */
            {
                /* further details */
                ImGui::SeparatorText("VkBuffer details");
                output = (*dataObject).getBufferPointer(i);
                ImGui::Text(("VkBuffer handle = " + output).c_str());
                /* AI-Assisted: Claude Opus 5.5: syntax */
                /* [xbilko03] warning that data might be in middle of a change */
                {
                    std::string note = dataObject->getChangingNote(output);
                    if (!note.empty())
                        ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.0f, 1.0f), "May be in the middle of a change: %s",
                                           note.c_str());
                }
                /* AI-Assisted: end */
                output = std::to_string((*dataObject).getBufferDataSize(i));
                ImGui::Text(("Size = " + output).c_str());

                /* AI-Assisted: Claude Opus 5.5: syntax */
                if (dataObject->getCallsSettings() && output != "new" && dataObject->getBufferHasCulprit(i)) {
                    ImGui::SeparatorText("Relations");
                    output = std::to_string((*dataObject).getBufferCulpritID(i));
                    output += ":";
                    output += (*dataObject).getBufferCulpritName(i);
                    ImGui::Text(output.c_str());
                }
                /* AI-Assisted: end */

                /* [xbilko03] show data if there are any */
                ImGui::SeparatorText("Data");
                ImGui::Text((*dataObject).getBufferData(i).c_str());
            }
            /* AI-Assisted: Claude Opus 5.5: syntax */
            ImGui::End();
            /* AI-Assisted: end */
        }
    }

    /* AI-Assisted: Claude Opus 5.5: syntax */
    /* [xbilko03] grid of vulkan information */
    static void showInfoGrid(const char *id, std::vector<infoRow> &rows, char filters[2][CALL_FILTER_SIZE],
                             float height) {
        ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
                                ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_ScrollY;
        if (!ImGui::BeginTable(id, 2, flags, ImVec2(0.0f, height)))
            return;
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupScrollFreeze(0, 2);
        ImGui::TableHeadersRow();

        ImGui::TableNextRow();
        for (int column = 0; column < 2; column++) {
            ImGui::TableSetColumnIndex(column);
            ImGui::PushID(column);
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputTextWithHint("##filter", "filter", filters[column], CALL_FILTER_SIZE);
            ImGui::PopID();
        }

        for (size_t i = 0; i < rows.size(); i++) {
            if (!callFilterMatches(rows[i].number, filters[0]) || !callFilterMatches(rows[i].name, filters[1]))
                continue;
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(rows[i].number.c_str());
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(rows[i].name.c_str());
        }
        ImGui::EndTable();
    }

    /* AI-Assisted: end */
    void debuggerUI::showAppInfo(vkdebug::ui::renderer *window, vkdebug::ui::events *dataObject) {
        /* AI-Assisted: Claude Opus 5.5: syntax */
        ImGui::BeginChild("Vulkan Info", ImVec2(0.0f, 0.0f), ImGuiChildFlags_None,
                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        /* AI-Assisted: end */

        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] two grids with two columns */
        ImGuiTableFlags infoFlags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
                                    ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp;
        ImGui::SeparatorText("Application Information");
        if (ImGui::BeginTable("appInfoGrid", 2, infoFlags)) {
            ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 200.0f);
            ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();
            std::list<std::string> appInfo = (*dataObject).getAppInfo();
            for (std::string &item: appInfo) {
                std::string property = item;
                std::string value = "";
                size_t equals = item.find('=');
                if (equals != std::string::npos) {
                    property = item.substr(0, equals);
                    value = item.substr(equals + 1);
                }
                if (property.substr(0, 6) == "layer_")
                    property = property.substr(6);
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(property.c_str());
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(value.c_str());
            }
            ImGui::EndTable();
        }

        /* [xbilko03] vkInfo information about extensions|physical devices */
        std::vector<infoRow> deviceExtensions;
        std::vector<infoRow> instanceExtensions;
        std::vector<infoRow> physicalDevices;
        std::list<std::string> vkInfo = (*dataObject).getVkInfo();
        for (std::string &item: vkInfo) {
            size_t hash = item.find('#');
            if (hash == std::string::npos)
                continue;
            size_t space = item.find(' ', hash);
            if (space == std::string::npos)
                continue;
            infoRow row;
            row.number = item.substr(hash + 1, space - hash - 1);
            row.name = item.substr(space + 1);
            std::string kind = item.substr(0, hash);
            if (kind == "device extension ")
                deviceExtensions.push_back(row);
            else if (kind == "instance extension ")
                instanceExtensions.push_back(row);
            else if (kind == "physical device ")
                physicalDevices.push_back(row);
        }

        /* [xbilko03] grid's height scaling with number of entries within physical devices grid */
        ImGuiStyle &style = ImGui::GetStyle();
        float textRow = ImGui::GetTextLineHeight() + style.CellPadding.y * 2.0f;
        float filterRow = ImGui::GetFrameHeight() + style.CellPadding.y * 2.0f;
        float separatorHeight = ImGui::GetTextLineHeight() + style.SeparatorTextPadding.y * 2.0f + style.ItemSpacing.y;
        float devicesHeight = textRow * (float) (physicalDevices.size() + 1) + filterRow + 2.0f;
        float extensionsHeight = (ImGui::GetContentRegionAvail().y - devicesHeight - 3.0f * separatorHeight - 3.0f *
                                  style.ItemSpacing.y) / 2.0f;
        extensionsHeight = (float) (int) extensionsHeight - 1.0f;
        if (extensionsHeight < textRow * 3.0f + filterRow)
            extensionsHeight = textRow * 3.0f + filterRow;

        ImGui::SeparatorText("Device Extensions");
        showInfoGrid("deviceExtensionsGrid", deviceExtensions, vkInfoFilters[0], extensionsHeight);
        ImGui::SeparatorText("Instance Extensions");
        showInfoGrid("instanceExtensionsGrid", instanceExtensions, vkInfoFilters[1], extensionsHeight);
        ImGui::SeparatorText("Physical Devices");
        showInfoGrid("physicalDevicesGrid", physicalDevices, vkInfoFilters[2], devicesHeight);

        ImGui::EndChild();
        /* AI-Assisted: end */
    }

    /* AI-Assisted: Claude Opus 5.5: syntax */
    /* [xbilko03] text of the frame column */
    static std::string callFrameText(const events::callRow &row) {
        if (!row.hasFrame)
            return "";
        if (row.frame == 0)
            return "before frames";
        return std::to_string(row.frame);
    }

    /* [xbilko03] text of the result column */
    static std::string callResultText(const events::callRow &row) {
        if (row.beforeExecution)
            return "caught before execution";
        if (row.retVal == 0)
            return "VK_SUCCESS";
        return "NOT SUCCESS [VK_RESULT=" + std::to_string(row.retVal) + "]";
    }

    /* [xbilko03] options for texts in the result selector filter */
    static const char *const callResultFilterNames[CallResultCount] = {
        "All", "VK_SUCCESS", "NOT SUCCESS", "caught before execution"
    };
    /* [xbilko03] selected column for sorting */
    static int callSortColumn = CallColumnId;
    static bool callSortAscending = true;
    /* [xbilko03] asc order of two rows by sort column */
    static bool callRowLessAscending(const events::callRow &a, const events::callRow &b) {
        if (callSortColumn == CallColumnFrame && a.frame != b.frame)
            return a.frame < b.frame;
        if (callSortColumn == CallColumnFunction && a.name != b.name)
            return a.name < b.name;
        if (callSortColumn == CallColumnResult) {
            if (a.beforeExecution != b.beforeExecution)
                return b.beforeExecution;
            if (a.retVal != b.retVal)
                return a.retVal < b.retVal;
        }
        return a.id < b.id;
    }

    static bool callRowLess(const events::callRow &a, const events::callRow &b) {
        if (callSortAscending)
            return callRowLessAscending(a, b);
        return callRowLessAscending(b, a);
    }

    /* AI-Assisted: end */
    void debuggerUI::showApiCalls(vkdebug::ui::renderer *window, vkdebug::ui::events *dataObject) {
        /* table style and feature flags */
        static ImGuiTableFlags flags =
                ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable |
                ImGuiTableFlags_Sortable |
                ImGuiTableFlags_SortMulti | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter |
                ImGuiTableFlags_BordersV |
                ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollY || ImGuiTableFlags_SizingStretchProp;

        /* table will be sized based on text height */
        const float textHeight = ImGui::GetTextLineHeightWithSpacing();

        /* AI-Assisted: Claude Opus 5.5: syntax */
        ImGui::BeginChild("Vk_API_Calls");

        /* [xbilko03] grid of all vulkan api calls */
        bool frameless = dataObject->isFrameless();
        ImGuiTableColumnFlags frameColumnFlags = ImGuiTableColumnFlags_WidthFixed;
        if (frameless)
            frameColumnFlags |= ImGuiTableColumnFlags_Disabled;
        ImGuiTableFlags gridFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable |
                                    ImGuiTableFlags_Sortable | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter |
                                    ImGuiTableFlags_BordersV |
                                    ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp;
        float gridHeight = ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing();
        unsigned long long shownCount = 0;
        int pageCount = 1;

        if (ImGui::BeginTable("callGrid", CallColumnCount, gridFlags, ImVec2(0.0f, gridHeight))) {
            ImGui::TableSetupColumn("Call ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed,
                                    90.0f, CallColumnId);
            ImGui::TableSetupColumn("Frame", frameColumnFlags, 110.0f, CallColumnFrame);
            ImGui::TableSetupColumn("Function", ImGuiTableColumnFlags_WidthStretch, 0.0f, CallColumnFunction);
            ImGui::TableSetupColumn("Result", ImGuiTableColumnFlags_WidthFixed, 230.0f, CallColumnResult);
            /* header row and filter row stay on top when scrolling */
            ImGui::TableSetupScrollFreeze(0, 2);
            /* [xbilko03] styling for header */
            ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.5f));
            ImGui::TableNextRow(ImGuiTableRowFlags_Headers, ImGui::GetFrameHeight());
            for (int column = 0; column < CallColumnCount; column++) {
                if (!ImGui::TableSetColumnIndex(column))
                    continue;
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);
                ImGui::TableHeader(ImGui::TableGetColumnName(column));
            }
            ImGui::PopStyleColor();

            /* filter row: a new filter text starts again at the first page */
            ImGui::TableNextRow();
            for (int column = 0; column < CallColumnCount; column++) {
                ImGui::TableSetColumnIndex(column);
                ImGui::PushID(column);
                ImGui::SetNextItemWidth(-FLT_MIN);
                /* [xbilko03] result filter as a selector */
                if (column == CallColumnResult) {
                    if (ImGui::BeginCombo("##filter", callResultFilterNames[callResultFilter])) {
                        for (int option = 0; option < CallResultCount; option++) {
                            if (ImGui::Selectable(callResultFilterNames[option], option == callResultFilter)) {
                                callResultFilter = option;
                                callsPage = 0;
                                callRowsDirty = true;
                            }
                        }
                        ImGui::EndCombo();
                    }
                } else if (ImGui::InputTextWithHint("##filter", "filter", callFilters[column], CALL_FILTER_SIZE)) {
                    callsPage = 0;
                    callRowsDirty = true;
                }
                ImGui::PopID();
            }

            /* sort by the clicked header */
            ImGuiTableSortSpecs *sortSpecs = ImGui::TableGetSortSpecs();
            if (sortSpecs != NULL && sortSpecs->SpecsDirty) {
                if (sortSpecs->SpecsCount > 0) {
                    callSortColumn = (int) sortSpecs->Specs[0].ColumnUserID;
                    callSortAscending = (sortSpecs->Specs[0].SortDirection == ImGuiSortDirection_Ascending);
                }
                sortSpecs->SpecsDirty = false;
                callRowsDirty = true;
            }

            double now = glfwGetTime();
            /* [xbilko03] if filters|sorting in grid changed or after CALL_GRID_REFRESH_SECONDS, get the rows */
            if (callRowsDirty || now - callRowsTime >= CALL_GRID_REFRESH_SECONDS) {
                if (dataObject->getCallRows(callRows, callRowsVersion))
                    callRowsDirty = true;
            } else if (dataObject->getCallsVersion() != callRowsVersion)
                glfwPostEmptyEvent();
            if (callRowsDirty) {
                callRowsDirty = false;
                callRowsTime = now;

                /* name and result of every row are needed only to filter or sort by them */
                bool needNames = callFilters[CallColumnFunction][0] != '\0' || callResultFilter != CallResultAll ||
                                 callSortColumn == CallColumnFunction || callSortColumn == CallColumnResult;
                if (needNames)
                    dataObject->fillCallRows(callRows, 0, callRows.size());

                /* rows that pass all filters, the text of a column is made only when its filter is not empty */
                shownCallRows.clear();
                for (size_t i = 0; i < callRows.size(); i++) {
                    events::callRow &row = callRows[i];
                    if (callFilters[CallColumnId][0] != '\0' && !callFilterMatches(
                            std::to_string(row.id), callFilters[CallColumnId]))
                        continue;
                    if (!frameless && callFilters[CallColumnFrame][0] != '\0' && !callFilterMatches(
                            callFrameText(row), callFilters[CallColumnFrame]))
                        continue;
                    if (callFilters[CallColumnFunction][0] != '\0' && !callFilterMatches(
                            row.name, callFilters[CallColumnFunction]))
                        continue;
                    if (callResultFilter == CallResultSuccess && (row.beforeExecution || row.retVal != 0))
                        continue;
                    if (callResultFilter == CallResultNotSuccess && (row.beforeExecution || row.retVal == 0))
                        continue;
                    if (callResultFilter == CallResultBeforeExecution && !row.beforeExecution)
                        continue;
                    shownCallRows.push_back(row);
                }

                /* sorting by Call ID: rows usually come already in ascending ID order, then they are only reversed for descending order */
                bool inOrder = false;
                if (callSortColumn == CallColumnId) {
                    inOrder = true;
                    for (size_t i = 1; i < shownCallRows.size(); i++) {
                        if (shownCallRows[i - 1].id > shownCallRows[i].id) {
                            inOrder = false;
                            break;
                        }
                    }
                }
                if (!inOrder)
                    std::sort(shownCallRows.begin(), shownCallRows.end(), callRowLess);
                else if (!callSortAscending)
                    std::reverse(shownCallRows.begin(), shownCallRows.end());
            }

            /* current page */
            shownCount = shownCallRows.size();
            pageCount = (int) ((shownCount + CALLS_PER_PAGE - 1) / CALLS_PER_PAGE);
            if (pageCount < 1)
                pageCount = 1;
            if (callsPage > pageCount - 1)
                callsPage = pageCount - 1;
            if (callsPage < 0)
                callsPage = 0;
            size_t first = (size_t) callsPage * CALLS_PER_PAGE;
            size_t last = first + CALLS_PER_PAGE;
            if (last > shownCallRows.size())
                last = shownCallRows.size();
            /* name and result only for the rows of this page */
            dataObject->fillCallRows(shownCallRows, first, last);

            for (size_t i = first; i < last; i++) {
                events::callRow &row = shownCallRows[i];
                ImGui::PushID((void *) row.call);
                ImGui::TableNextRow();

                /* the whole row is one selectable: a click opens the details window of this call, a click on an open row closes it */
                ImGui::TableSetColumnIndex(CallColumnId);
                std::string idStr = ("vk_Call [ID=" + std::to_string(row.id) + "]");
                bool detailsOpen = apiDetails.count(idStr) != 0 && apiDetails[idStr];
                if (ImGui::Selectable(std::to_string(row.id).c_str(), detailsOpen,
                                      ImGuiSelectableFlags_SpanAllColumns)) {
                    apiDetails[idStr] = !detailsOpen;
                    apiDetailsCall[idStr] = row.call;
                }

                ImGui::TableSetColumnIndex(CallColumnFrame);
                ImGui::TextUnformatted(callFrameText(row).c_str());

                ImGui::TableSetColumnIndex(CallColumnFunction);
                ImGui::TextUnformatted(row.name.c_str());

                /* call-end message has not arrived yet: the call was caught before execution (or never returned) */
                ImGui::TableSetColumnIndex(CallColumnResult);
                ImVec4 resultColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                if (row.beforeExecution)
                    resultColor = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
                else if (row.retVal == 0)
                    resultColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
                ImGui::TextColored(resultColor, "%s", callResultText(row).c_str());
                ImGui::PopID();
            }
            ImGui::EndTable();
        }

        /* pager below the grid */
        ImGui::BeginDisabled(callsPage <= 0);
        if (ImGui::Button("Prev"))
            callsPage--;
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Page");
        ImGui::SameLine();
        /* [xbilko03] current grid's page number */
        int pageInput = callsPage + 1;
        ImGui::SetNextItemWidth(80.0f);
        if (ImGui::InputInt("##page", &pageInput, 0, 0)) {
            if (pageInput < 1)
                pageInput = 1;
            if (pageInput > pageCount)
                pageInput = pageCount;
            callsPage = pageInput - 1;
        }
        ImGui::SameLine();
        ImGui::Text("of %d (%llu calls)", pageCount, shownCount);
        ImGui::SameLine();
        ImGui::BeginDisabled(callsPage >= pageCount - 1);
        if (ImGui::Button("Next"))
            callsPage++;
        ImGui::EndDisabled();

        /* [xbilko03] rows opened details in windows stay persistently on screen regardless of current grid page */
        for (std::map<std::string, bool>::iterator openWindow = apiDetails.begin(); openWindow != apiDetails.end();
             openWindow++) {
            if (!openWindow->second || apiDetailsCall.count(openWindow->first) == 0)
                continue;
            std::string idStr = openWindow->first;
            vkdebug::ui::apiCall *item = apiDetailsCall[idStr];
            int tableSize = (int) item->getParameters().size() + 1;
            if (MAX_TABLE_SIZE < tableSize)
                tableSize = MAX_TABLE_SIZE;
            /* AI-Assisted: end */

            /* detailed call view window */
            /* AI-Assisted: Claude Opus 5.5: syntax - NoCollapse */
            ImGui::Begin(idStr.c_str(), &openWindow->second, ImGuiWindowFlags_NoCollapse);
            /* AI-Assisted: end */
            ImGui::Text(item->getName().c_str());
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (item->getBeforeExecution())
                ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.0f, 1.0f),
                                   "caught before execution: only input parameters are known");
            /* AI-Assisted: end */
            /* for each parameter, put a type and its value into a table */
            if (ImGui::BeginTable((idStr + "table").c_str(), 2, flags, ImVec2(0.0f, textHeight * tableSize), 0.0f)) {
                ImGui::TableSetupColumn("Parameter_Name", ImGuiTableColumnFlags_DefaultSort, 0.0f, 0);
                ImGui::TableSetupColumn("Value", NULL, 0.0f, 1);
                ImGui::TableHeadersRow();
                for (auto itm: item->getParameters()) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text(itm.name.c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text(itm.value.c_str());
                }
                ImGui::EndTable();
            }
            ImGui::End();
            /* AI-Assisted: Claude Opus 5.5: syntax */
        }

        ImGui::EndChild();
        /* AI-Assisted: end */
    }

    void debuggerUI::run() {
        /* window object to handle rendering */
        vkdebug::ui::renderer window;
        /* event manager to handle data */
        vkdebug::ui::events eventManager;

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
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.4f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_CheckMark, (ImVec4) ImColor::HSV(0.0f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_TitleBg, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.4f));
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_Header, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.7f));

        /* table colors */
        ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.4f));

        /* main menu window */
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 20.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(8.0f, 0.0f));

        /* window render loop */
        while (!glfwWindowShouldClose(window.window)) {
            glfwWaitEvents();

            window.startNewFrame();

            /* imGui menu */
            {
                /* resize */
                int width, height;
                glfwGetWindowSize(window.window, &width, &height);
                ImGui::SetNextWindowSize({(float) width, (float) height}, 0);
                ImGui::SetNextWindowPos({0, 0}, 0);

                ImGui::Begin("Menu", NULL, mainWindowFlags);

                /* AI-Assisted: Claude Opus 5.5: syntax */
                bool exitClicked = false;
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(TAB_BAR_PADDING_X, TAB_BAR_PADDING_Y));
                float barHeight = ImGui::GetFrameHeight();
                /* AI-Assisted: end */

                /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
                if (!recentTargetsLoaded)
                    loadRecentTargets();

                /* [xbilko03] start screen for layer configuration before any target runs */
                if (!targetLaunched && !startedByLayer) {
                    ImVec2 topLeft = ImGui::GetCursorPos();
                    ImGui::SetCursorPosX(
                        ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - EXIT_BUTTON_WIDTH);
                    if (ImGui::Button("Exit", ImVec2(EXIT_BUTTON_WIDTH, barHeight)))
                        exitClicked = true;
                    ImGui::SetCursorPos(topLeft);
                    ImGui::PopStyleVar();

                    ImGui::Text("No application selected.");
                    ImGui::Text("Launch a target application to start debugging.");
                    ImGui::NewLine();

                    ImGui::SeparatorText("Frames (applies to the next launch)");
                    ImGui::Checkbox("Frameless app", &cfgFrameless);
                    ImGui::BeginDisabled(cfgFrameless);
                    if (ImGui::BeginCombo("Frame delimiter", frameDelims[cfgFrameDelim].label)) {
                        for (int d = 0; d < (int) (sizeof(frameDelims) / sizeof(frameDelims[0])); d++)
                            if (ImGui::Selectable(frameDelims[d].label, d == cfgFrameDelim))
                                cfgFrameDelim = d;
                        ImGui::EndCombo();
                    }
                    ImGui::EndDisabled();

                    ImGui::SeparatorText("Breakpoints (applies to the next launch)");
                    ImGui::Checkbox("Enable breakpoints", &cfgEnableBreaks);
                    if (cfgEnableBreaks) {
                        ImGui::InputInt("Break at Xth API call", &cfgBreakCallAt);
                        ImGui::InputInt("Break every Xth API call", &cfgBreakCallEvery);
                        ImGui::BeginDisabled(cfgFrameless);
                        ImGui::InputInt("Break at Xth frame", &cfgBreakFrameAt);
                        ImGui::InputInt("Break every Xth frame", &cfgBreakFrameEvery);
                        ImGui::InputInt("Break when FPS below", &cfgBreakFpsBelow);
                        ImGui::EndDisabled();
                        ImGui::Checkbox("Break on new VkMemory allocation", &cfgBreakMemoryNew);
                        ImGui::Checkbox("Break on new VkBuffer creation", &cfgBreakBufferNew);
                        ImGui::Checkbox("Break on new VkImage creation", &cfgBreakImageNew);

                        if (cfgBreakCallAt < 0) cfgBreakCallAt = 0;
                        if (cfgBreakCallEvery < 0) cfgBreakCallEvery = 0;
                        if (cfgBreakFrameAt < 0) cfgBreakFrameAt = 0;
                        if (cfgBreakFrameEvery < 0) cfgBreakFrameEvery = 0;
                        if (cfgBreakFpsBelow < 0) cfgBreakFpsBelow = 0;
                    }
                    ImGui::NewLine();

                    if (ImGui::Button("Launch Target...", ImVec2(160, 64))) {
                        launchTarget();
                    }

                    if (!recentTargets.empty()) {
                        ImGui::Text("Recent:");
                        ImGui::SameLine();
                        for (size_t i = 0; i < recentTargets.size(); ++i) {
                            std::string label = std::filesystem::path(recentTargets[i]).filename().string();
                            ImGui::PushID((int) i);
                            if (ImGui::Button(label.c_str())) {
                                launchTargetPath(recentTargets[i]);
                                addRecentTarget(recentTargets[i]);
                            }
                            ImGui::PopID();
                            ImGui::SameLine();
                        }
                        ImGui::NewLine();
                    }
                }
                /* AI-Assisted: end */
                /* AI-Assisted: Claude Opus 5.5: syntax */
                /* [xbilko03] when target runs, top bar with tabs is shown, chosen tab fills the screen */
                else {
                    /* [xbilko03] tab bar with its own child region, exit button fits on that same row */
                    float barWidth = ImGui::GetContentRegionAvail().x - EXIT_BUTTON_WIDTH - ImGui::GetStyle().
                                     ItemSpacing.x;
                    ImGui::BeginChild("TabBarRow", ImVec2(barWidth, barHeight + 2.0f), ImGuiChildFlags_None,
                                      ImGuiWindowFlags_NoScrollbar);
                    /* [xbilko03] tabs colors */
                    ImGui::PushStyleColor(ImGuiCol_Tab, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.4f));
                    ImGui::PushStyleColor(ImGuiCol_TabHovered, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.6f));
                    ImGui::PushStyleColor(ImGuiCol_TabActive, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.5f));
                    ImGui::PushStyleColor(ImGuiCol_TabUnfocused, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.4f));
                    ImGui::PushStyleColor(ImGuiCol_TabUnfocusedActive, (ImVec4) ImColor::HSV(0.5f, 1.0f, 0.5f));
                    if (ImGui::BeginTabBar("MainTabs")) {
                        const char *tabNames[TabCount] = {
                            "Target Application", "Threads", "API Calls", "Buffers", "Images",
                            "Memory", "Warnings", "Breakpoints", "Shaders"
                        };
                        for (int tab = 0; tab < TabCount; tab++) {
                            ImGuiTabItemFlags tabFlags = ImGuiTabItemFlags_None;
                            if (tab == requestedTab)
                                tabFlags = ImGuiTabItemFlags_SetSelected;
                            if (ImGui::BeginTabItem(tabNames[tab], NULL, tabFlags)) {
                                selectedTab = tab;
                                ImGui::EndTabItem();
                            }
                        }
                        requestedTab = -1;
                        ImGui::EndTabBar();
                    }
                    ImGui::PopStyleColor(5);
                    ImGui::EndChild();

                    ImGui::SameLine();
                    if (ImGui::Button("Exit", ImVec2(EXIT_BUTTON_WIDTH, barHeight)))
                        exitClicked = true;
                    ImGui::PopStyleVar();

                    /* [xbilko03] global styles for all VkDebuggerApp windows */
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(8.0f, 0.0f));

                    /* [xbilko03] tab fills the screen down to a small gap above the status bar */
                    float bottomPadding = ImGui::GetWindowSize().y - ImGui::GetWindowContentRegionMax().y;
                    float statusReserve = STATUS_BAR_HEIGHT + ImGui::GetStyle().ItemSpacing.y - bottomPadding;
                    if (statusReserve < 0.0f)
                        statusReserve = 0.0f;
                    ImGui::BeginChild("TabContent", ImVec2(0.0f, -statusReserve));

                    /* show the chosen tab, Shaders is empty for now */
                    if (selectedTab == TabTargetApplication)
                        showAppInfo(&window, &eventManager);
                    else if (selectedTab == TabThreads)
                        showThreads(&window, &eventManager);
                    else if (selectedTab == TabApiCalls)
                        showApiCalls(&window, &eventManager);
                    else if (selectedTab == TabBuffers)
                        showBuffers(&window, &eventManager);
                    else if (selectedTab == TabImages)
                        showImages(&window, &eventManager);
                    else if (selectedTab == TabMemory)
                        showMemories(&window, &eventManager);
                    else if (selectedTab == TabWarnings)
                        showWarnings(&window, &eventManager);
                    else if (selectedTab == TabBreakpoints)
                        showBreaks(&window, &eventManager);
                    ImGui::EndChild();

                    showStatusBar(&eventManager);

                    ImGui::PopStyleVar(4);
                }
                /* AI-Assisted: end */

                ImGui::End();

                /* AI-Assisted: Claude Opus 5.5: syntax */
                if (exitClicked)
                    break;
                /* AI-Assisted: end */
            }

            /* global styles for all VkDebuggerApp windows */
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(8.0f, 0.0f));

            if (demo == true)
                ImGui::ShowDemoWindow(&demo);

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

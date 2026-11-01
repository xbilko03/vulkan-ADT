/*
* Name		    : debuggerUI.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file of the UI application (main loop and all windows)
*
* Author: Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#pragma once

#include "renderer.hpp"
#include "communication/events.hpp"
/* AI-Assisted: Claude Sonnet 5: syntax */
#include <string>
#include <vector>
/* AI-Assisted: end */

namespace vkdebug::ui {
    /* AI-Assisted: Claude Opus 5.5: syntax */
    /* [xbilko03] columns of vulkan api calls grid */
    enum callColumn {
        CallColumnId,
        CallColumnFrame,
        CallColumnFunction,
        CallColumnResult,
        CallColumnCount
    };

    /* [xbilko03] one row of Memory | Buffer | Image grids */
    struct objectRow {
        unsigned long long index;
        std::string handle;
        std::string state;
        std::string size;
        std::string bound;
        std::string createdBy;
    };

#define OBJECT_COLUMN_COUNT 6

    /* [xbilko03] one row of vulkan information grid */
    struct infoRow {
        std::string number;
        std::string name;
    };

    /* [xbilko03] options of the result filter selector in grid */
    enum callResultOption {
        CallResultAll,
        CallResultSuccess,
        CallResultNotSuccess,
        CallResultBeforeExecution,
        CallResultCount
    };

    /* AI-Assisted: end */

    class debuggerUI {
    public:
        void run();

        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] set at the start of program if UI was started by the layer */
        bool startedByLayer = false;
        /* AI-Assisted: end */
    private:
        /* render a tab that shows the collected apiCalls */
        void showApiCalls(vkdebug::ui::renderer *window, vkdebug::ui::events *dataObject);

        /* render a tab that shows the state of the VkMemory */
        void showMemories(vkdebug::ui::renderer *window, vkdebug::ui::events *dataObject);

        /* render a tab that shows the state of the VkBuffers */
        void showBuffers(vkdebug::ui::renderer *window, vkdebug::ui::events *dataObject);

        /* render a tab that shows the state of the VkImages */
        void showImages(vkdebug::ui::renderer *window, vkdebug::ui::events *dataObject);

        /* render a tab that shows details about Vulkan on the current device */
        void showAppInfo(vkdebug::ui::renderer *window, vkdebug::ui::events *dataObject);

        /* render a tab that shows history of warnings */
        void showWarnings(vkdebug::ui::renderer *window, vkdebug::ui::events *dataObject);

        /* render a tab that shows history of breakpoint occurances */
        void showBreaks(vkdebug::ui::renderer *window, vkdebug::ui::events *dataObject);

        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* render a tab that shows the app threads that call Vulkan (grid) */
        void showThreads(vkdebug::ui::renderer *window, vkdebug::ui::events *dataObject);

        std::vector<vkdebug::ui::events::threadTabRow> threadTabRows;
        std::vector<vkdebug::ui::events::timedRow> breakRows;
        std::vector<vkdebug::ui::events::timedRow> warningRows;

        /* render the status bar along the bottom edge, colored by the program state: Pause (+ Continue, Stabilize while paused) left, state right */
        void showStatusBar(vkdebug::ui::events *dataObject);

#define STATUS_BAR_HEIGHT 26.0f
#define STATUS_BAR_PADDING 8.0f
#define STATUS_BUTTON_INSET 3.0f
        /* AI-Assisted: end */
        /* AI-Assisted: Claude Sonnet 5: syntax */
        /* [xbilko03] launch target app via UI */
        void launchTarget();

        void launchTargetPath(const std::string &path);

        /* [xbilko03] list recently launched executables */
        void loadRecentTargets();

        void saveRecentTargets();

        void addRecentTarget(const std::string &path);

        std::vector<std::string> recentTargets;
        bool recentTargetsLoaded = false;
        bool targetLaunched = false;

        /* [xbilko03] breakpoint settings chosen in the UI, written to layer settings file after */
        bool cfgEnableBreaks = false;
        int cfgBreakCallAt = 0;
        int cfgBreakCallEvery = 0;
        int cfgBreakFrameAt = 0;
        int cfgBreakFrameEvery = 0;
        int cfgBreakFpsBelow = 0;
        bool cfgBreakMemoryNew = false;
        bool cfgBreakBufferNew = false;
        bool cfgBreakImageNew = false;
        /* AI-Assisted: end */
        /* AI-Assisted: Claude Opus 5.5: syntax */
        bool cfgFrameless = false;
        int cfgFrameDelim = 0;
        /* AI-Assisted: end */
        /* AI-Assisted: Claude Sonnet 5: syntax */
        /* [xbilko03] write the setting file the layer reads at start */
        void writeLayerSettings(const std::string &settingsPath);

        /* AI-Assisted: end */

        /* max items that can render in a single table at once */
#define MAX_TABLE_SIZE 32
#define IMAGE_REVIEW_SIZE 512
#define IMAGE_ZOOM_SPEED 0.10f
#define	MAX_IMAGE_ZOOM 256.0f

        /* call details associated with their name to allow multiple windows of that kind can stay opened at the same time */
        std::map<std::string, bool> apiDetails;
        /* textures associated with their IDs so multiple windows of that kind can stay opened at the same time  */
        std::map<unsigned long long, bool> texDetails;
        std::map<unsigned long long, bool> texLoaded;
        /* global zoom option for the textures */
        float zoom = 1.0f;

        /* features */
        bool demo = false;

        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] navigation tabs */
        enum mainTab {
            TabTargetApplication,
            TabThreads,
            TabApiCalls,
            TabBuffers,
            TabImages,
            TabMemory,
            TabWarnings,
            TabBreakpoints,
            TabShaders,
            TabCount
        };

        /* [xbilko03] current tab */
        int selectedTab = TabTargetApplication;
        /* [xbilko03] scheduled tab to be switched for the user by program */
        int requestedTab = -1;

        /* [xbilko03] size of the tabs|exit button on top of the bar */
#define TAB_BAR_PADDING_X 16.0f
#define TAB_BAR_PADDING_Y 10.0f
#define EXIT_BUTTON_WIDTH 128.0f

        /* [xbilko03] vulkan api calls grid data limits */
#define CALLS_PER_PAGE 50
#define CALL_FILTER_SIZE 64
        char callFilters[CallColumnCount][CALL_FILTER_SIZE] = {};
        int callResultFilter = CallResultAll;
        /* [xbilko03] filters of vulkan information grids */
        char vkInfoFilters[3][2][CALL_FILTER_SIZE] = {};
        /* [xbilko03] filters for memory rows within grid */
        char memoryFilters[OBJECT_COLUMN_COUNT][CALL_FILTER_SIZE] = {};
        /* [xbilko03] filters for buffer rows within grid */
        char bufferFilters[OBJECT_COLUMN_COUNT][CALL_FILTER_SIZE] = {};
        /* [xbilko03] filters for image rows within grid */
        char imageFilters[OBJECT_COLUMN_COUNT][CALL_FILTER_SIZE] = {};
        int memoryPage = 0;
        int bufferPage = 0;
        int imagePage = 0;
        std::map<unsigned long long, bool> memoryDetails;
        std::map<unsigned long long, bool> bufferDetails;
        std::map<unsigned long long, bool> imageDetails;
        std::vector<objectRow> objectRows;
        int callsPage = 0;
        /* [xbilko03] a vector to keep entries of vulkan calls in between UI draws */
        std::vector<vkdebug::ui::events::callRow> callRows;
        /* [xbilko03] a vector to keep entries of vulkan calls that currently fit grid's filters */
        std::vector<vkdebug::ui::events::callRow> shownCallRows;
        /* [xbilko03] shownCallRows is built only when calls (callsVersion), filter value or sorting changed */
        unsigned long long callRowsVersion = (unsigned long long) -1;
        bool callRowsDirty = true;
        /* [xbilko03] as massive amounts of calls can keep coming in, rows are build only at this interval to prevent lag */
#define CALL_GRID_REFRESH_SECONDS 0.25
        double callRowsTime = 0.0;
        /* [xbilko03] call of each opened details window */
        std::map<std::string, vkdebug::ui::apiCall *> apiDetailsCall;
        /* AI-Assisted: end */
    };
}

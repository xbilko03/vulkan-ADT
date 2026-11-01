/*
* Name		    : events.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file for data reception, message parsing and data processing
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#pragma once
#include <list>
#include <string>
#include <map>
/* AI-Assisted: Claude Opus 5.5: syntax */
#include <vector>
#include <mutex>
#include <cstdint>
/* AI-Assisted: end */
/* AI-Assisted: Claude Opus 5.5: syntax */
#include "winsock.hpp"
/* AI-Assisted: end */
#include <GLFW/glfw3.h>

#include "../data/memoryManager.hpp"
#include "../data/bufferManager.hpp"
#include "../data/imageManager.hpp"
#include "../data/systemManager.hpp"
#include "../data/apiCallsManager.hpp"
#include "../renderer.hpp"

namespace vkdebug::ui {
    class events {
    public:
        /* establish connection between VkDebuggerApp and VkDebuggerLayer */
        void connectToLayer(vkdebug::ui::renderer *windowManager);

        /* get data about program that is being debugged */
        /* about API calls */
        unsigned long long getFrameCount() { return frames.size(); }
        /* AI-Assisted: Claude Opus 5.5: syntax */
        bool isFrameless() { return frameless; }
#define FRAMELESS_GROUP_SIZE 1000ull
        /* AI-Assisted: end */
        unsigned long long getFrameCallsCount(unsigned long long frameID) { return frames[frameID].size(); }
        std::list<apiCall *> getFrameCalls(unsigned long long frameID) { return frames[frameID]; }
        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] one row of vulkan api calls grid */
        struct callRow {
            apiCall *call;
            unsigned long long id;
            bool hasFrame;
            unsigned long long frame;
            std::string name;
            int retVal;
            bool beforeExecution;
        };

        /* [xbilko03] if something changed (new version), get all calls of all frames, otherwise return false (no changes) */
        bool getCallRows(std::vector<callRow> &rows, unsigned long long &version);

        /* [xbilko03] fill name, retval and beforeExecution of rows on current grid page or for all entries when sorting|filtering in name|retval column */
        void fillCallRows(std::vector<callRow> &rows, size_t first, size_t last);

        /* [xbilko03] get current version of the call entries */
        unsigned long long getCallsVersion() {
            /* [xbilko03] parseMutex because the version is written by receiving threads and read by the UI */
            std::lock_guard<std::mutex> lock(parseMutex);
            return callsVersion;
        }

        /* AI-Assisted: end */
        /* about VkDeviceMemory */
        /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
        unsigned long long getMemoriesCount() {
            if (memMan == nullptr)
                return 0;
            return memMan->getMemoryCount();
        }

        /* AI-Assisted: end */
        std::string getMemoryPointer(unsigned long long ID) { return memMan->getPointer(ID); }
        std::string getMemoryState(unsigned long long ID) { return memMan->getState(ID); }
        std::string getMemoryData(unsigned long long ID) { return memMan->getDataReadable(ID); }
        std::string getMemoryBound(unsigned long long ID) { return memMan->getBoundObj(ID); }
        unsigned long long getMemoryDataSize(unsigned long long ID) { return memMan->getDataSize(ID); }
        /* about VkBuffer */
        /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
        unsigned long long getBufferCount() {
            if (bufMan == nullptr)
                return 0;
            return bufMan->getBufferCount();
        }

        /* AI-Assisted: end */
        std::string getBufferPointer(unsigned long long ID) { return bufMan->getPointer(ID); }
        std::string getBufferState(unsigned long long ID) { return bufMan->getState(ID); }
        std::string getBufferData(unsigned long long ID) { return bufMan->getDataReadable(ID); }
        std::string getBufferBound(unsigned long long ID) { return bufMan->getBoundObj(ID); }
        unsigned long long getBufferDataSize(unsigned long long ID) { return bufMan->getDataSize(ID); }
        /* about VkImage */
        /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
        unsigned long long getImagesCount() {
            if (imgMan == nullptr)
                return 0;
            return imgMan->getBufferCount();
        }

        /* AI-Assisted: end */
        std::string getImagePointer(unsigned long long ID) { return imgMan->getPointer(ID); }
        std::string getImageState(unsigned long long ID) { return imgMan->getState(ID); }
        std::string getImageData(unsigned long long ID) { return imgMan->getDataReadable(ID); }
        std::string getImageBound(unsigned long long ID) { return imgMan->getBoundObj(ID); }
        unsigned long long getImageDataSize(unsigned long long ID) { return imgMan->getDataSize(ID); }
        /* get data about Vulkan object */
        /* about VkDeviceMemory */
        std::string getMemoryCulpritName(unsigned long long ID) { return memMan->getCulprit(ID)->getName(); }
        unsigned long long getMemoryCulpritID(unsigned long long ID) { return memMan->getCulprit(ID)->getID(); }
        /* about VkBuffer */
        std::string getBufferCulpritName(unsigned long long ID) { return bufMan->getCulprit(ID)->getName(); }
        unsigned long long getBufferCulpritID(unsigned long long ID) { return bufMan->getCulprit(ID)->getID(); }
        /* about VkImage */
        std::string getImageCulpritName(unsigned long long ID) { return imgMan->getCulprit(ID)->getName(); }
        unsigned long long getImageCulpritID(unsigned long long ID) { return imgMan->getCulprit(ID)->getID(); }
        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] false until vk command that created this object finishes execution (out of driver) and arrives as a message */
        bool getMemoryHasCulprit(unsigned long long ID) { return memMan->getCulprit(ID) != NULL; }
        /* [xbilko03] false until vk command that created this object finishes execution (out of driver) and arrives as a message */
        bool getBufferHasCulprit(unsigned long long ID) { return bufMan->getCulprit(ID) != NULL; }
        /* [xbilko03] false until vk command that created this object finishes execution (out of driver) and arrives as a message */
        bool getImageHasCulprit(unsigned long long ID) { return imgMan->getCulprit(ID) != NULL; }
        /* AI-Assisted: end */

        /* get layer enabled features list */
        bool getCallsSettings() { return logCalls; }

        /* render a new Vulkan image in ImGui */
        void loadTexture(unsigned long long ID);

        /* AI-Assisted: Claude Sonnet 5: syntax */
        bool getPausedState() { return layerPaused; }
        /* AI-Assisted: end */
        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] state of target application shown in the UI at the bottom right */
        enum programState {
            ProgramWaiting,
            ProgramRunning,
            ProgramPaused,
            ProgramLost
        };

        int getProgramState() {
            std::lock_guard<std::mutex> lock(parseMutex);
            if (layerLost)
                return ProgramLost;
            if (!layerConnected)
                return ProgramWaiting;
            if (layerPaused)
                return ProgramPaused;
            return ProgramRunning;
        }

        /* AI-Assisted: end */
        /* AI-Assisted: Claude Sonnet 5: syntax */
        void resumeExecution();

        /* AI-Assisted: end */
        /* AI-Assisted: Claude Opus 5.5: syntax */
        bool getStableState() { return layerStable; }
        bool getStabilizingState() { return stabilizing; }

        void stabilizeExecution();

        bool getPauseRequestedState() { return pauseRequested; }

        void pauseExecution();

        /* [xbilko03] one row of threads from the LAYER's thread list (sent during a pause)
         * objects = objects that thread works with
         */
        struct threadRow {
            std::string id;
            std::string state;
            std::string call;
            std::vector<std::string> objects;
        };

        /* [xbilko03] get information about which thread may be changing given object (handle) */
        std::string getChangingNote(const std::string &handle);

        /* AI-Assisted: end */
        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] one row of the thread's TAB in UI = UI information about given layer's thread */
        struct threadTabRow {
            std::string id;
            std::string state;
            std::string lastCall;
            unsigned long long lastCallID;
            bool inCall;
            unsigned long long callCount;
            std::string current;
            std::string objects;
        };

        void getThreadTabRows(std::vector<threadTabRow> &rows);

        /* AI-Assisted: end */
        /* AI-Assisted: Claude Opus 5.5: syntax */
        unsigned long long getOtherThreadCount() { return otherThreadCount; }
        /* [xbilko03] hint text when submitted timeline semaphore has not received value (sent by layer while target app is paused) */
        std::vector<std::string> *getTimelineWaits() { return &timelineWaits; }
        /* AI-Assisted: end */

        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] one row of breakpoints/warnings grid */
        struct timedRow {
            std::string source;
            std::string time;
        };

        void getBreakRows(std::vector<timedRow> &rows);

        void getWarningRows(std::vector<timedRow> &rows);

        /* AI-Assisted: end */
        /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
        std::list<std::string> getAppInfo() {
            if (sysMan == nullptr)
                return std::list<std::string>();
            return sysMan->getAppInfo();
        }

        std::list<std::string> getVkInfo() {
            if (sysMan == nullptr)
                return std::list<std::string>();
            return sysMan->getVkInfo();
        }

        /* AI-Assisted: end */
    private:
        /* bools to omit unused features */
        /* AI-Assisted: Claude Sonnet 5: syntax */
        bool logCalls = false;
        bool logBuffers = false;
        bool logImages = false;
        bool logMemory = false;
        bool logWarnings = false;
        bool logBreaks = false;
        bool layerPaused = false;
        /* AI-Assisted: end */
        /* AI-Assisted: Claude Opus 5.5: syntax */
        bool layerStable = false;
        bool stabilizing = false;
        bool pauseRequested = false;
        /* AI-Assisted: end */

        /* socket and layer communication */
        SOCKET connectSocket = INVALID_SOCKET;

        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] socket and agent communication */
        SOCKET agentSocket = INVALID_SOCKET;
        uint16_t agentPort = 0;

        /* [xbilko03] receiving thread for the control connection (layer) */
        static DWORD WINAPI listenForData(__in LPVOID lpParameter);

        /* [xbilko03] receiving thread for the data connection (agent) */
        static DWORD WINAPI listenForAgent(__in LPVOID lpParameter);

        /* [xbilko03] handle one control command from the layer */
        void handleControl(uint8_t command, const uint8_t *payload, size_t size);

        /* [xbilko03] send command from UI to the layer */
        void sendCommand(uint8_t command);

        /* [xbilko03] receive buffer per target's application thread
         * bytes = fragments of one message received so far
         * openCall = finished messages of the call that has not ended yet (call which has not sent message about being finished in vulkan driver)
         */
        struct threadStream {
            std::vector<uint8_t> bytes;
            std::vector<std::vector<uint8_t> > openCall;
            uint32_t threadId = 0;
            std::string lastCall;
            unsigned long long lastCallID = 0;
            bool inCall = false;
            unsigned long long callCount = 0;
        };

        std::map<std::pair<uint32_t, uint64_t>, threadStream> threadStreams;

        /* [xbilko03] split a finished chunk from the agent into fragments, then join fragments into corresponding messages */
        void handleSegment(uint32_t threadId, uint64_t threadCreationTime, uint32_t flags, const uint8_t *bytes,
                           size_t size);

        /* [xbilko03] process one message according to the type */
        void handleMessage(threadStream &stream, const uint8_t *message, size_t size);

        /* [xbilko03] for every finished message in stream, decode and save it to UI's data */
        void applyOpenCall(threadStream &stream);

        /* [xbilko03] show a vulkan call when MessageCallBegin message arrives */
        void showCallBegin(const uint8_t *message, size_t size);

        /* [xbilko03] decode the input parameters of the vulkan call, add label and add it to existing vulkan call row in the UI */
        void showCallInputs(const uint8_t *message, size_t size);

        /* [xbilko03] vulkan calls shown before in-vulkan-driver execution until their MessageCallEnd message arrives
         * pendingCalls = api calls
         * pendingCallFrames = frame that api call is in
         */
        std::map<unsigned long long, apiCall *> pendingCalls;
        std::map<unsigned long long, unsigned long long> pendingCallFrames;

        /* [xbilko03] decode and save one message to UI's data storage by its type */
        void applyMessage(const uint8_t *message, size_t size);

        /* [xbilko03] guard for when both receiving threads rewrite the same data */
        std::mutex parseMutex;
        /* [xbilko03] tracker of whether a new vulkan call is added or an existing call receives new information */
        unsigned long long callsVersion = 0;
        /* [xbilko03] layer connection status */
        bool layerConnected = false;
        bool layerLost = false;
        /* [xbilko03] call number of the message being processed */
        unsigned long long currentMessageCall = 0;
        /* AI-Assisted: end */

        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] find or create api call of function, starts a new frame if it's a delimiter function, creates a new memory|buffer|image entry for create calls */
        void callBegin(uint16_t function);

        /* [xbilko03] add one parameter to the current call */
        void callParameter(const std::string &name, const std::string &value);

        /* [xbilko03] save finished call into its frame, for create calls set it as creator of said object, increase callsVersion */
        void callEnd(uint16_t function);

        /* [xbilko03] process special information from the layer such as breakpoint triggers */
        void layerInfo(uint8_t infoType, uint64_t number, const std::string &text);

        /* [xbilko03] process content of memory data sent by the layer */
        void memoryData(uint64_t handle, uint64_t bytes, const uint8_t *data);

        /* [xbilko03] save the arrival time of the new break|warning entries, refresh UI so that new entries might be shown */
        void messageDone();

        /* AI-Assisted: end */
        /* initialize objects that take care of data */
        void createDataManagers();

        /* AI-Assisted: Claude Sonnet 5: syntax */
        vkMemoryManager *memMan = nullptr;
        vkBufferManager *bufMan = nullptr;
        vkImageManager *imgMan = nullptr;
        vkSystemManager *sysMan = nullptr;
        /* AI-Assisted: end */

        /* lists to hold additional data */
        std::list<std::string> warningsList;
        std::list<std::string> breaksList;
        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] timestamps of each breakpoint and warning entries */
        std::vector<std::string> breaksTimes;
        std::vector<std::string> warningsTimes;
        /* AI-Assisted: end */
        /* AI-Assisted: Claude Opus 5.5: syntax */
        std::vector<threadRow> threadRows;
        unsigned long long otherThreadCount = 0;
        std::vector<std::string> timelineWaits;
        /* AI-Assisted: end */
        std::map<unsigned long long, std::list<apiCall *> > frames;

        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] number of vulkan function whose call is in progress */
        uint16_t receptionFunction = 0;
        /* AI-Assisted: end */
        /* keep track of the current Api Call in the single command iteration */
        apiCall *currentCall;
        /* list to save all the Api Calls data */
        std::list<apiCall *> currentCallList;
        /* IDs to assign them to the corresponding objects */
        unsigned long long frameID = 0;
        unsigned long long memoryID = 0;
        unsigned long long bufferID = 0;
        unsigned long long imageID = 0;
        /* index 0 for source, 1 for destination used in data parsing operations */
        unsigned long long modifiedMemoryID[2];
        unsigned long long lastMemorySize;
        unsigned long long modifiedBufferID[2];
        unsigned long long modifiedImageID[2];
        /* window object for image rendering */
        vkdebug::ui::renderer *winMan;
        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] number of function which starts a new frame */
        uint16_t delimFunction = 0;
        /* AI-Assisted: end */
        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] if there is no delimiting function */
        bool frameless = false;
        /* AI-Assisted: end */
    };
}

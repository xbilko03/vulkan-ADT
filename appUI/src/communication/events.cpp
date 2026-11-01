/*
* Name		    : events.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file for data reception, message parsing and data processing
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#include "../../include/communication/events.hpp"
/* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
#include "../../include/communication/vkdbgDecoder.hpp"
#include "../../../transport/include/vkdbgFormat.hpp"
#include "../../../transport/include/vkdbgFunctionsGenerated.hpp"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <sstream>
/* AI-Assisted: end */

namespace vkdebug::ui {
    void events::createDataManagers() {
        memMan = new vkMemoryManager();
        bufMan = new vkBufferManager();
        imgMan = new vkImageManager();
        sysMan = new vkSystemManager(winMan);
    }

    /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
    void events::resumeExecution() {
        if (!layerPaused)
            return;
        layerPaused = false;
        sendCommand(vkdebug::ControlResume);
    }

    void events::pauseExecution() {
        if (layerPaused || pauseRequested)
            return;
        pauseRequested = true;
        sendCommand(vkdebug::ControlPause);
    }

    /* [xbilko03] normalize handles (different message can return same handle in different format) */
    static std::string normalizeHandle(const std::string &handle) {
        std::string out;
        size_t i = 0;
        if (handle.size() > 2 && handle[0] == '0' && (handle[1] == 'x' || handle[1] == 'X'))
            i = 2;
        while (i < handle.size() && handle[i] == '0')
            i++;
        for (; i < handle.size(); i++)
            out += (char) toupper((unsigned char) handle[i]);
        return out;
    }

    /* [xbilko03] insert vulkan call into the list so it's in the correct position so grid stays sorted */
    static void insertSortedByID(std::list<apiCall *> &target, apiCall *call) {
        std::list<apiCall *>::iterator position = target.end();
        while (position != target.begin()) {
            position--;
            if ((*position)->getID() <= call->getID()) {
                position++;
                break;
            }
        }
        target.insert(position, call);
    }

    bool events::getCallRows(std::vector<callRow> &rows, unsigned long long &version) {
        /* [xbilko03] read data on mutex so that they don't get changed by receiving threads mid-read */
        std::lock_guard<std::mutex> lock(parseMutex);
        /* [xbilko03] to prevent copying same data, continue only if something has changed */
        if (version == callsVersion)
            return false;
        version = callsVersion;
        rows.clear();
        for (std::map<unsigned long long, std::list<apiCall *> >::iterator frame = frames.begin(); frame != frames.end()
             ; frame++) {
            for (apiCall *call: frame->second) {
                callRow row;
                row.call = call;
                row.id = call->getID();
                row.hasFrame = !frameless;
                row.frame = frame->first;
                row.retVal = 0;
                row.beforeExecution = false;
                rows.push_back(row);
            }
        }
        return true;
    }

    /* [xbilko03] copy information from layer's thread list into thread grid's row */
    static void copyLayerThreadInfo(events::threadTabRow &row, events::threadRow &layerRow) {
        row.state = layerRow.state;
        if (layerRow.call != "-")
            row.current = layerRow.call;
        for (size_t o = 0; o < layerRow.objects.size(); o++) {
            if (!row.objects.empty())
                row.objects += ", ";
            row.objects += layerRow.objects[o];
        }
    }

    void events::getThreadTabRows(std::vector<threadTabRow> &rows) {
        /* [xbilko03] mutex to prevent crash while reading */
        std::lock_guard<std::mutex> lock(parseMutex);
        rows.clear();
        /* [xbilko03] create a row for each vulkan-calling thread */
        for (std::map<std::pair<uint32_t, uint64_t>, threadStream>::iterator item = threadStreams.begin();
             item != threadStreams.end(); item++) {
            threadStream &stream = item->second;
            threadTabRow row;
            row.id = std::to_string(stream.threadId);
            row.state = "running";
            if (layerPaused)
                row.state = "?";
            row.lastCall = stream.lastCall;
            row.lastCallID = stream.lastCallID;
            row.inCall = stream.inCall;
            row.callCount = stream.callCount;
            row.current = "";
            row.objects = "";
            /* [xbilko03] when paused merge the data of what UI knows about given threa and what layer knows about it */
            if (layerPaused) {
                for (size_t i = 0; i < threadRows.size(); i++) {
                    if (threadRows[i].id == row.id)
                        copyLayerThreadInfo(row, threadRows[i]);
                }
            }
            rows.push_back(row);
        }

        /* [xbilko03] when paused, layer sends data about threads, some thread may not have sent anything just yet, so make rows for them too */
        if (!layerPaused)
            return;
        for (size_t i = 0; i < threadRows.size(); i++) {
            bool listed = false;
            for (size_t r = 0; r < rows.size(); r++) {
                if (rows[r].id == threadRows[i].id)
                    listed = true;
            }
            if (listed)
                continue;
            threadTabRow row;
            row.id = threadRows[i].id;
            row.lastCall = "";
            row.lastCallID = 0;
            row.inCall = false;
            row.callCount = 0;
            row.current = "";
            row.objects = "";
            copyLayerThreadInfo(row, threadRows[i]);
            rows.push_back(row);
        }
    }

    /* [xbilko03] local time HH:MM:SS.mmm */
    static std::string currentTimeText() {
        SYSTEMTIME now;
        GetLocalTime(&now);
        char text[16];
        snprintf(text, sizeof(text), "%02u:%02u:%02u.%03u", now.wHour, now.wMinute, now.wSecond, now.wMilliseconds);
        return text;
    }

    /* [xbilko03] pair every entry of the list with its time */
    static void makeTimedRows(std::vector<events::timedRow> &rows, std::list<std::string> &entries,
                              std::vector<std::string> &times) {
        rows.clear();
        size_t i = 0;
        for (std::string &item: entries) {
            events::timedRow row;
            row.source = item;
            row.time = "";
            if (i < times.size())
                row.time = times[i];
            rows.push_back(row);
            i++;
        }
    }

    void events::getBreakRows(std::vector<timedRow> &rows) {
        /* [xbilko03] mutex, receiving thread may add new entries */
        std::lock_guard<std::mutex> lock(parseMutex);
        makeTimedRows(rows, breaksList, breaksTimes);
    }

    void events::getWarningRows(std::vector<timedRow> &rows) {
        std::lock_guard<std::mutex> lock(parseMutex);
        makeTimedRows(rows, warningsList, warningsTimes);
    }

    void events::fillCallRows(std::vector<callRow> &rows, size_t first, size_t last) {
        /* [xbilko03] mutex, receiving thread may add new entries */
        std::lock_guard<std::mutex> lock(parseMutex);
        for (size_t i = first; i < last && i < rows.size(); i++) {
            rows[i].name = rows[i].call->getName();
            rows[i].retVal = rows[i].call->getRetVal();
            rows[i].beforeExecution = rows[i].call->getBeforeExecution();
        }
    }

    std::string events::getChangingNote(const std::string &handle) {
        if (!layerPaused || layerStable)
            return "";
        std::string wanted = normalizeHandle(handle);
        if (wanted.empty())
            return "";
        for (threadRow &row: threadRows)
            for (std::string &object: row.objects)
                if (normalizeHandle(object) == wanted)
                    return "thread " + row.id + " (" + row.call + ")";
        return "";
    }

    void events::stabilizeExecution() {
        if (!layerPaused || layerStable || stabilizing)
            return;
        stabilizing = true;
        sendCommand(vkdebug::ControlStabilize);
    }

    /* AI-Assisted: end */
    void events::loadTexture(unsigned long long ID) {
        if (imgMan->getData(ID) != NULL)
            /* AI-Assisted: Claude Opus 5.5: syntax */
            winMan->loadImageTexture(ID, (int) imgMan->getWidth(ID), (int) imgMan->getHeight(ID), 4,
                                     imgMan->getMemory(ID)->memoryData);
        /* AI-Assisted: end */
    }

    /* AI-Assisted: Claude Opus 5.5: syntax */
    static std::string handleText(uint64_t handle) {
        std::stringstream s;
        s << (void *) ((uintptr_t) (handle));
        return s.str();
    }

    void events::messageDone() {
        while (breaksTimes.size() < breaksList.size())
            breaksTimes.push_back(currentTimeText());
        while (warningsTimes.size() < warningsList.size())
            warningsTimes.push_back(currentTimeText());

        /* [xbilko03] refresh window when the new data is received */
        glfwPostEmptyEvent();
    }

    void events::callBegin(uint16_t function) {
        receptionFunction = function;
        if (logCalls) {
            /* [xbilko03] if showCallBegin has already created the call, use it, otherwise create a new one */
            if (pendingCalls.count(currentMessageCall) != 0)
                currentCall = pendingCalls[currentMessageCall];
            else
                currentCall = new apiCall(currentMessageCall);
            currentCall->assignName(std::string("begin_") + functionName(function));
            currentCall->assignRetVal("0");
        }
        if (function == delimFunction) {
            /* [xbilko03] delimiter function has been called, start a new frame */
            frameID++;
            if (logCalls)
                currentCallList = {};
        }
        /* [xbilko03] new objects of a certain type, expect data about them */
        else if (function == VKDBG_FN_vkAllocateMemory) {
            memMan->newMemory(memoryID);
        } else if (function == VKDBG_FN_vkCreateImage) {
            imgMan->newBuffer(imageID);
        } else if (function == VKDBG_FN_vkCreateBuffer) {
            bufMan->newBuffer(bufferID);
        }
    }

    void events::callParameter(const std::string &name, const std::string &value) {
        if (!logCalls)
            return;
        if (name == "vkResult")
            currentCall->assignRetVal(value);
        currentCall->assignParameter(name + "=" + value);
    }

    void events::callEnd(uint16_t function) {
        /* [xbilko03] set that no call in progress anymore */
        receptionFunction = VKDBG_FN_NONE;
        /* [xbilko03] finish reading and save call */
        if (logCalls) {
            /* [xbilko03] if MessageCallBegin arrived before and current MessageCallEnd belongs to it, mark that entry as finished */
            bool listed = false;
            unsigned long long pendingID = currentCall->getID();
            if (pendingCalls.count(pendingID) != 0) {
                currentCall->assignBeforeExecution(false);

            /* [xbilko03] if call belongs to this frame or app does not track frames, then consider it done, else remove it from the old frame */
                if (frameless || pendingCallFrames[pendingID] == frameID)
                    listed = true;
                else
                    frames[pendingCallFrames[pendingID]].remove(currentCall);
                pendingCallFrames.erase(pendingID);
                pendingCalls.erase(pendingID);
            }
            /* [xbilko03]
             * if MessageCallBegin has not arrived (or is removed from the old frame)
             * make this call a new entry
             */
            if (!listed) {
                if (frameless)
                    insertSortedByID(frames[currentCall->getID() / FRAMELESS_GROUP_SIZE], currentCall);
                else
                    insertSortedByID(currentCallList, currentCall);
            }
            if (!frameless)
                frames[frameID] = currentCallList;
            /* [xbilko03] calls changed, inform about that fact in callsVersion */
            callsVersion++;
        }
        /* [xbilko03] this object's data is now complete, prepare to expect a new one in the future */
        if (function == VKDBG_FN_vkAllocateMemory) {
            if (logCalls)
                memMan->assignCulprit(memoryID, currentCall);
            memoryID++;
        } else if (function == VKDBG_FN_vkCreateImage) {
            if (logCalls)
                imgMan->assignCulprit(imageID, currentCall);
            imageID++;
        } else if (function == VKDBG_FN_vkCreateBuffer) {
            if (logCalls)
                bufMan->assignCulprit(bufferID, currentCall);
            bufferID++;
        }
    }

    void events::layerInfo(uint8_t infoType, uint64_t number, const std::string &text) {
        if (infoType == vkdebug::InfoCallAtBreakTrigger) {
            breaksList.push_back("Breakpoint: triggered at Xth API call - call[" + text + "]");
        } else if (infoType == vkdebug::InfoCallEveryBreakTrigger) {
            breaksList.push_back("Breakpoint: triggered every Xth API call - call[" + text + "]");
        } else if (infoType == vkdebug::InfoImageNewBreak) {
            breaksList.push_back(
                "Breakpoint: triggered at new VkImage creation - call[" + std::to_string(currentMessageCall) + "]");
        } else if (infoType == vkdebug::InfoBufferNewBreak) {
            breaksList.push_back(
                "Breakpoint: triggered at new VkBuffer creation - call[" + std::to_string(currentMessageCall) + "]");
        } else if (infoType == vkdebug::InfoMemoryNewBreak) {
            breaksList.push_back(
                "Breakpoint: triggered at new VkMemory allocation - call[" + std::to_string(currentMessageCall) + "]");
        } else if (infoType == vkdebug::InfoFpsBelowOptionBreak) {
            breaksList.push_back(
                "Breakpoint: triggered when below X fps - FPS[" + text + "] - call[" + std::to_string(
                    currentMessageCall)
                + "]");
        } else if (infoType == vkdebug::InfoFrameAtBreakTrigger) {
            breaksList.push_back(
                "Breakpoint: triggered when at frame - frame[" + std::to_string(frameID) + "] - call[" + std::to_string(
                    currentMessageCall) + "]");
        } else if (infoType == vkdebug::InfoFrameEveryBreakTrigger) {
            breaksList.push_back(
                "Breakpoint: triggered every Xth frame - frame[" + std::to_string(frameID) + "] - call[" +
                std::to_string(currentMessageCall) + "]");
        }
        /* [xbilko03] add information about which thread hit the breakpoint to last breakpoint entry */
        else if (infoType == vkdebug::InfoBreakThread) {
            if (!breaksList.empty())
                breaksList.back() += " - thread[" + text + "]";
        } else if (infoType == vkdebug::InfoFpsEveryTrigger) {
            warningsList.push_back(
                "Warning: FPS report every Xth frame - FPS[" + text + "] - frame[" + std::to_string(frameID) + "]");
        } else if (infoType == vkdebug::InfoFpsBelowTrigger) {
            warningsList.push_back(
                "Warning: FPS report below FPS - FPS[" + text + "] - frame[" + std::to_string(frameID) + "]");
        } else if (infoType == vkdebug::InfoLogCalls) {
            logCalls = (number != 0);
        } else if (infoType == vkdebug::InfoLogBuffers) {
            logBuffers = (number != 0);
        } else if (infoType == vkdebug::InfoLogImages) {
            logImages = (number != 0);
        } else if (infoType == vkdebug::InfoLogMemory) {
            logMemory = (number != 0);
        } else if (infoType == vkdebug::InfoWarnings) {
            logWarnings = (number != 0);
        } else if (infoType == vkdebug::InfoBreaks) {
            logBreaks = (number != 0);
        } else if (infoType == vkdebug::InfoDelim) {
            if (text == "vkAcquireNextImageKHR")
                delimFunction = VKDBG_FN_vkAcquireNextImageKHR;
            else if (text == "vkQueueSubmit")
                delimFunction = VKDBG_FN_vkQueueSubmit;
            else if (text == "vkCmdDraw")
                delimFunction = VKDBG_FN_vkCmdDraw;
            else if (text == "frameless")
                frameless = true;
        }

        /* [xbilko03] state of Vulkan objects changes */
        if (receptionFunction == VKDBG_FN_vkUnmapMemory) {
            if (infoType == vkdebug::InfoPtr) {
                modifiedMemoryID[0] = memMan->getFromPointerID(text);
                memMan->setState(modifiedMemoryID[0], "unmapped");
            } else if (infoType == vkdebug::InfoSize) {
                lastMemorySize = number;
            }
        } else if (receptionFunction == VKDBG_FN_vkMapMemory) {
            if (infoType == vkdebug::InfoPtr) {
                modifiedMemoryID[1] = memMan->getFromPointerID(text);
                memMan->setState(modifiedMemoryID[1], "mapped");
            }
        } else if (receptionFunction == VKDBG_FN_vkQueueSubmit) {
            if (infoType == vkdebug::InfoPtr) {
                modifiedMemoryID[0] = memMan->getFromPointerID(text);
            } else if (infoType == vkdebug::InfoSize) {
                lastMemorySize = number;
            }
        } else if (receptionFunction == VKDBG_FN_vkAllocateMemory) {
            memMan->assignPointer(memoryID, text);
        } else if (receptionFunction == VKDBG_FN_vkFreeMemory) {
            memMan->setState(memMan->getFromPointerID(text), "freed");
        } else if (receptionFunction == VKDBG_FN_vkCreateImage) {
            if (infoType == vkdebug::InfoPtr) {
                imgMan->assignPointer(imageID, text);
            } else if (infoType == vkdebug::InfoWidth) {
                imgMan->setWidth(imageID, text);
            } else if (infoType == vkdebug::InfoHeight) {
                imgMan->setHeight(imageID, text);
            }
        } else if (receptionFunction == VKDBG_FN_vkDestroyImage) {
            modifiedImageID[1] = imgMan->getFromPointerID(text);
            if (modifiedImageID[1] != -1)
                imgMan->setState(modifiedImageID[1], "freed");
        } else if (receptionFunction == VKDBG_FN_vkCreateBuffer) {
            bufMan->assignPointer(bufferID, text);
        } else if (receptionFunction == VKDBG_FN_vkDestroyBuffer) {
            modifiedBufferID[1] = bufMan->getFromPointerID(text);
            if (modifiedBufferID[1] != -1)
                bufMan->setState(modifiedBufferID[1], "freed");
        } else if (receptionFunction == VKDBG_FN_vkBindBufferMemory) {
            if (infoType == vkdebug::InfoMemPtr) {
                /* [xbilko03] get source memory ID */
                modifiedMemoryID[0] = memMan->getFromPointerID(text);
            } else if (infoType == vkdebug::InfoBufPtr) {
                /* [xbilko03] if source memory exists, attempt to bind it to the destination buffer */
                if (modifiedMemoryID[0] != -1) {
                    /* [xbilko03] get destination buffer ID */
                    modifiedBufferID[1] = bufMan->getFromPointerID(text);

                    /* [xbilko03] bind memoryPtr to bufferPtr */
                    bufMan->assignBoundObj(modifiedBufferID[1], memMan->getPointer(modifiedMemoryID[0]));
                    bufMan->setState(modifiedBufferID[1], "bound");

                    /* [xbilko03] bind bufferPtr to memoryPtr */
                    memMan->assignBoundObj(modifiedMemoryID[0], text);
                    memMan->setState(modifiedMemoryID[0], "bound");

                    /* [xbilko03] assign vkMemory to vkBuffer */
                    bufMan->assignMemory(modifiedBufferID[1], memMan->getMemory(modifiedMemoryID[0]));
                }
            }
        } else if (receptionFunction == VKDBG_FN_vkBindImageMemory) {
            if (infoType == vkdebug::InfoMemPtr) {
                /* [xbilko03] get source memory ID */
                modifiedMemoryID[0] = memMan->getFromPointerID(text);
            } else if (infoType == vkdebug::InfoImgPtr) {
                /* [xbilko03] if source memory exists, attempt to bind it to the destination image */
                if (modifiedMemoryID[0] != -1) {
                    /* [xbilko03] get destination image ID */
                    modifiedImageID[1] = imgMan->getFromPointerID(text);

                    /* [xbilko03] bind memoryPtr to imagePtr */
                    imgMan->assignBoundObj(modifiedImageID[1], memMan->getPointer(modifiedMemoryID[0]));
                    imgMan->setState(modifiedImageID[1], "bound");

                    /* [xbilko03] bind imagePtr to memoryPtr */
                    memMan->assignBoundObj(modifiedMemoryID[0], text);
                    memMan->setState(modifiedMemoryID[0], "bound");

                    /* [xbilko03] assign vkMemory to image's vkBuffer */
                    imgMan->assignMemory(modifiedImageID[1], memMan->getMemory(modifiedMemoryID[0]));
                    /* [xbilko03] load texture only when image's memory data have already arrived */
                    vkdebug::ui::vkMemoryManager::vkMemory *boundMem = imgMan->getMemory(modifiedImageID[1]);
                    if (boundMem != nullptr && boundMem->getDataPtr() != nullptr)
                        winMan->loadImageTexture(modifiedImageID[1], (int) imgMan->getWidth(modifiedImageID[1]),
                                                 (int) imgMan->getHeight(modifiedImageID[1]), 4,
                                                 boundMem->getDataPtr());
                }
            }
        } else if (receptionFunction == VKDBG_FN_vkCmdCopyImageToBuffer) {
            if (infoType == vkdebug::InfoSrcImg) {
                modifiedImageID[0] = imgMan->getFromPointerID(text);
            } else if (infoType == vkdebug::InfoDstBuf) {
                /* [xbilko03] if source image exists, attempt to copy the data */
                if (modifiedImageID[0] != -1) {
                    /* [xbilko03] get destination buffer ID */
                    modifiedBufferID[1] = bufMan->getFromPointerID(text);
                    bufMan->setState(modifiedBufferID[1], "copied");

                    /* [xbilko03] get source memory ID */
                    modifiedMemoryID[0] = memMan->getFromPointerID(imgMan->getBoundObj(modifiedImageID[0]));

                    /* [xbilko03] get destination memory ID */
                    modifiedMemoryID[1] = memMan->getFromPointerID(bufMan->getBoundObj(modifiedBufferID[1]));

                    /* [xbilko03] copy data between bound memory objects */
                    memMan->assignData(modifiedMemoryID[1], memMan->getData(modifiedMemoryID[0]),
                                       memMan->getDataSize(modifiedMemoryID[0]));
                }
            }
        } else if (receptionFunction == VKDBG_FN_vkCmdCopyBufferToImage) {
            if (infoType == vkdebug::InfoSrcBuf) {
                /* [xbilko03] get source buffer ID */
                modifiedBufferID[0] = bufMan->getFromPointerID(text);
            } else if (infoType == vkdebug::InfoDstImg) {
                /* [xbilko03] if source buffer exists, attempt to copy data */
                if (modifiedBufferID[0] != -1) {
                    /* [xbilko03] get destination image ID */
                    modifiedImageID[1] = imgMan->getFromPointerID(text);
                    imgMan->setState(modifiedImageID[1], "copied");

                    /* [xbilko03] get source memory ID */
                    modifiedMemoryID[0] = memMan->getFromPointerID(bufMan->getBoundObj(modifiedBufferID[0]));

                    /* [xbilko03] get destination memory ID */
                    modifiedMemoryID[1] = memMan->getFromPointerID(imgMan->getBoundObj(modifiedImageID[1]));

                    /* [xbilko03] copy data between bound memory objects */
                    memMan->assignData(modifiedMemoryID[1], memMan->getData(modifiedMemoryID[0]),
                                       memMan->getDataSize(modifiedMemoryID[0]));
                }
            }
        } else if (receptionFunction == VKDBG_FN_vkCmdCopyImage) {
            if (infoType == vkdebug::InfoSrcImg) {
                /* [xbilko03] get source image ID */
                modifiedImageID[0] = imgMan->getFromPointerID(text);
            } else if (infoType == vkdebug::InfoDstImg) {
                /* [xbilko03] if source image exists, attempt to copy the data */
                if (modifiedMemoryID[0] != -1) {
                    /* [xbilko03] get destination image ID */
                    modifiedImageID[1] = imgMan->getFromPointerID(text);
                    imgMan->setState(modifiedImageID[1], "copied");

                    /* [xbilko03] get source memory ID */
                    modifiedMemoryID[0] = memMan->getFromPointerID(imgMan->getBoundObj(modifiedImageID[0]));

                    /* [xbilko03] get destination memory ID */
                    modifiedMemoryID[1] = memMan->getFromPointerID(imgMan->getBoundObj(modifiedImageID[1]));

                    memMan->assignData(modifiedMemoryID[1], memMan->getData(modifiedMemoryID[0]),
                                       memMan->getDataSize(modifiedMemoryID[0]));
                }
            }
        } else if (receptionFunction == VKDBG_FN_vkCmdCopyBuffer) {
            if (infoType == vkdebug::InfoSrcBuf) {
                /* [xbilko03] get source buffer ID */
                modifiedBufferID[0] = bufMan->getFromPointerID(text);
            } else if (infoType == vkdebug::InfoDstBuf) {
                /* [xbilko03] if source buffer exists, attempt to copy data */
                if (modifiedBufferID[0] != -1) {
                    /* [xbilko03] get destination buffer ID */
                    modifiedBufferID[1] = bufMan->getFromPointerID(text);
                    bufMan->setState(modifiedBufferID[1], "copied");

                    /* [xbilko03] get source memory ID */
                    modifiedMemoryID[0] = memMan->getFromPointerID(bufMan->getBoundObj(modifiedBufferID[0]));

                    /* [xbilko03] get destination memory ID */
                    modifiedMemoryID[1] = memMan->getFromPointerID(bufMan->getBoundObj(modifiedBufferID[1]));

                    /* [xbilko03] assign source memory data to destination memory */
                    memMan->assignData(modifiedMemoryID[1], memMan->getData(modifiedMemoryID[0]),
                                       memMan->getDataSize(modifiedMemoryID[0]));
                }
            }
        }
        /* [xbilko03] app information shown in the UI */
        else if (receptionFunction == VKDBG_FN_vkCreateInstance) {
            if (infoType == vkdebug::InfoProcessId) {
                sysMan->newAppInfo("proccessID=" + text);
            } else if (infoType == vkdebug::InfoAppName) {
                sysMan->newAppInfo("appName=" + text);
            } else if (infoType == vkdebug::InfoAppPath) {
                sysMan->newAppInfo("appPath=" + text);
            }
        }
    }

    void events::memoryData(uint64_t handle, uint64_t bytes, const uint8_t *data) {
        layerInfo(vkdebug::InfoPtr, handle, handleText(handle));
        layerInfo(vkdebug::InfoSize, bytes, std::to_string(bytes));
        if (receptionFunction != VKDBG_FN_vkUnmapMemory && receptionFunction != VKDBG_FN_vkQueueSubmit)
            return;

        size_t copySize = (size_t) (bytes);
        /* [xbilko03]
         * layer might send data of length 0
         * malloc(0) might return NULL or a valid pointer, if it would be NULL, there would be no way to differentiate OUT OF MEMORY or just empty data from the layer
         * copySize = 1 ensures the data shown in the UI get changed to reflect current state of that memory object
         */
        if (copySize == 0)
            copySize = 1;
        char *copy = (char *) (malloc(copySize));
        if (!copy)
            return;
        memcpy(copy, data, (size_t) (bytes));

        /* [xbilko03] if destination memory already has data, free them and attempt to replace them with the new data */
        if (memMan->getData(modifiedMemoryID[0]) != NULL)
            free(memMan->getData(modifiedMemoryID[0]));

        memMan->assignData(modifiedMemoryID[0], copy, lastMemorySize);
        modifiedBufferID[1] = bufMan->getFromPointerID(memMan->getBoundObj(modifiedMemoryID[0]));
        modifiedImageID[1] = imgMan->getFromPointerID(memMan->getBoundObj(modifiedMemoryID[0]));
    }

    /* AI-Assisted: end */

    /* AI-Assisted: Claude Opus 5.5: syntax */
    static bool recvExact(SOCKET socket, void *out, size_t size) {
        char *bytes = (char *) (out);
        while (size > 0) {
            /* [xbilko03] 1GB limit for one recv, bigger data received in parts */
            size_t part = size;
            if (part > 0x40000000)
                part = 0x40000000;
            int received = recv(socket, bytes, (int) (part), 0);
            if (received <= 0)
                return false;
            bytes += received;
            size -= (size_t) (received);
        }
        return true;
    }

    /* [xbilko03] messages that belong to the call being processed and must wait until call is complete (images, memory, buffers, ...) */
    static bool needsCallContext(uint8_t infoType) {
        switch (infoType) {
            case vkdebug::InfoAppName:
            case vkdebug::InfoProcessId:
            case vkdebug::InfoAppPath:
            case vkdebug::InfoPtr:
            case vkdebug::InfoSize:
            case vkdebug::InfoMemPtr:
            case vkdebug::InfoBufPtr:
            case vkdebug::InfoImgPtr:
            case vkdebug::InfoSrcBuf:
            case vkdebug::InfoDstBuf:
            case vkdebug::InfoSrcImg:
            case vkdebug::InfoDstImg:
            case vkdebug::InfoWidth:
            case vkdebug::InfoHeight:
                return true;
            default:
                return false;
        }
    }

    /* [xbilko03] text for each threadState */
    static const char *const threadStateNames[vkdebug::ThreadStateCount] = {
        "running", "breakpoint", "dead", "frozen while writing the maps", "frozen inside the driver",
        "frozen in a blocking wait", "waiting", "frozen", "in a blocking wait", "still finishing"
    };

    void events::sendCommand(uint8_t command) {
        uint8_t commandBytes[3] = {1, 0, command};
        winsockSendRaw(&connectSocket, (const char *) (commandBytes), sizeof(commandBytes));
    }

    void events::handleControl(uint8_t command, const uint8_t *payload, size_t size) {
        std::lock_guard<std::mutex> lock(parseMutex);
        switch (command) {
            /* [xbilko03] pause state from the layer = switch state in UI */
            case vkdebug::ControlPaused:
                if (size >= 1) {
                    if (payload[0]) {
                        layerPaused = true;
                        pauseRequested = false;
                    } else {
                        layerPaused = false;
                        layerStable = false;
                        stabilizing = false;
                    }
                }
                break;
            case vkdebug::ControlStable:
                if (size >= 1) {
                    layerStable = (payload[0] != 0);
                    if (layerStable)
                        stabilizing = false;
                }
                break;
            case vkdebug::ControlUserPause:
                breaksList.push_back("Paused by the user (Pause button)");
                break;
            case vkdebug::ControlThreadsBegin:
                threadRows.clear();
                otherThreadCount = 0;
                timelineWaits.clear();
                break;
            case vkdebug::ControlThreadsOther: {
                uint32_t count = 0;
                if (size >= 4)
                    memcpy(&count, payload, 4);
                otherThreadCount = count;
                break;
            }
            case vkdebug::ControlThreadInfo: {
                size_t offset = 0;
                uint32_t id = 0;
                uint8_t state = 0;
                uint16_t nameLength = 0;
                if (size < 7)
                    break;
                memcpy(&id, payload, 4);
                state = payload[4];
                memcpy(&nameLength, payload + 5, 2);
                offset = 7;
                if (offset + nameLength + 1 > size)
                    break;
                std::string call((const char *) (payload + offset), nameLength);
                offset += nameLength;
                uint8_t count = payload[offset++];
                threadRow row;
                row.id = std::to_string(id);
                for (uint8_t i = 0; i < count && offset + 8 <= size; i++, offset += 8) {
                    uint64_t object = 0;
                    memcpy(&object, payload + offset, 8);
                    row.objects.push_back(handleText(object));
                }
                /* [xbilko03] unknown state */
                row.state = "?";
                if (state < vkdebug::ThreadStateCount)
                    row.state = threadStateNames[state];
                row.call = call;
                if (row.call.empty())
                    /* [xbilko03] no call */
                    row.call = "-";
                threadRows.push_back(row);
                break;
            }
            case vkdebug::ControlTimelineWait: {
                if (size < 44)
                    break;
                uint64_t submit = 0, queue = 0, semaphore = 0, value = 0, provided = 0;
                uint32_t batch = 0;
                memcpy(&submit, payload, 8);
                memcpy(&batch, payload + 8, 4);
                memcpy(&queue, payload + 12, 8);
                memcpy(&semaphore, payload + 20, 8);
                memcpy(&value, payload + 28, 8);
                memcpy(&provided, payload + 36, 8);
                std::string text = "submit #" + std::to_string(submit) + " (batch " + std::to_string(batch) +
                                   ") on queue " +
                                   handleText(queue) + " waits on timeline semaphore " + handleText(semaphore) +
                                   " value " + std::to_string(value) +
                                   ", highest value provided so far " + std::to_string(provided);
                timelineWaits.push_back(text);
                break;
            }
            case vkdebug::ControlAgentPort:
                if (size >= 2 && agentSocket == INVALID_SOCKET && agentPort == 0) {
                    memcpy(&agentPort, payload, 2);
                    DWORD threadId;
                    CreateThread(0, 0, events::listenForAgent, this, 0, &threadId);
                }
                break;
            default:
                break;
        }
        messageDone();
    }

    void events::applyMessage(const uint8_t *message, size_t size) {
        vkdebug::messageHeader header;
        memcpy(&header, message, sizeof(header));
        const uint8_t *payload = message + sizeof(header);
        size_t payloadSize = size - sizeof(header);
        currentMessageCall = header.call;

        switch (header.messageType) {
            case vkdebug::MessageCallBegin:
                callBegin(header.function);
                break;

            case vkdebug::MessageCallEnd: {
                decodedFields fields;
                decodeCallEnd(header.function, payload, payloadSize, fields);
                for (decodedField &field: fields)
                    callParameter(field.name, field.value);
                callEnd(header.function);
                break;
            }

            case vkdebug::MessageLayerInfo: {
                if (payloadSize < 2 || payload[0] >= vkdebug::InfoCount)
                    break;
                uint8_t kind = payload[1];
                const uint8_t *value = payload + 2;
                size_t valueSize = payloadSize - 2;
                uint64_t number = 0;
                std::string text;
                if ((kind == vkdebug::ValueU64 || kind == vkdebug::ValueHandle || kind == vkdebug::ValueDouble) &&
                    valueSize >= 8) {
                    memcpy(&number, value, 8);
                    if (kind == vkdebug::ValueU64)
                        /* [xbilko03] ValueU64: number, text "1024" */
                        text = std::to_string(number);
                    else if (kind == vkdebug::ValueHandle) {
                        /* [xbilko03] ValueHandle: number, text "0x00AB12…"*/
                        text = handleText(number);
                    } else {
                        /* [xbilko03] ValueDouble: same 8 bytes as the number, text "1.500000" */
                        double real = 0;
                        memcpy(&real, value, 8);
                        text = std::to_string(real);
                    }
                }
                /* [xbilko03] ValueBool: number 0/1, text "true"/"false" */
                else if (kind == vkdebug::ValueBool && valueSize >= 1) {
                    number = value[0];
                    if (value[0])
                        text = "true";
                    else
                        text = "false";
                }
                /* [xbilko03] ValueString: text only, number stays 0 */
                else if (kind == vkdebug::ValueString && valueSize >= 2) {
                    uint16_t length = 0;
                    memcpy(&length, value, 2);
                    if (2 + size_t(length) <= valueSize)
                        text.assign((const char *) (value + 2), length);
                }
                layerInfo(payload[0], number, text);
                break;
            }

            case vkdebug::MessageMemoryData: {
                if (payloadSize < 24)
                    break;
                uint64_t handle = 0, offset = 0, bytes = 0;
                memcpy(&handle, payload, 8);
                memcpy(&offset, payload + 8, 8);
                memcpy(&bytes, payload + 16, 8);
                if (24 + bytes > payloadSize)
                    break;
                /* [xbilko03] large memory is sent in parts */
                if (offset != 0)
                    break;
                memoryData(handle, bytes, payload + 24);
                break;
            }
            default:
                break;
        }
        messageDone();
    }

    void events::applyOpenCall(threadStream &stream) {
        for (std::vector<uint8_t> &message: stream.openCall)
            applyMessage(message.data(), message.size());

        /* [xbilko03] all whole messages from stream.openCall are processed, clear them */
        stream.openCall.clear();
    }

    void events::showCallBegin(const uint8_t *message, size_t size) {
        if (!logCalls)
            return;
        vkdebug::messageHeader header;
        memcpy(&header, message, sizeof(header));

        apiCall *call = new apiCall(header.call);
        call->assignName(std::string("begin_") + functionName(header.function));
        call->assignRetVal("0");
        call->assignBeforeExecution(true);

        if (frameless)
            insertSortedByID(frames[header.call / FRAMELESS_GROUP_SIZE], call);
        else
            insertSortedByID(currentCallList, call);
        if (!frameless)
            frames[frameID] = currentCallList;

        pendingCalls[header.call] = call;
        pendingCallFrames[header.call] = frameID;
        callsVersion++;
    }

    void events::showCallInputs(const uint8_t *message, size_t size) {
        if (!logCalls)
            return;
        vkdebug::messageHeader header;
        memcpy(&header, message, sizeof(header));
        if (pendingCalls.count(header.call) == 0)
            return;
        apiCall *pending = pendingCalls[header.call];
        decodedFields fields;
        decodeCallInputs(header.function, message + sizeof(header), size - sizeof(header), fields);
        for (decodedField &field: fields)
            pending->assignParameter(field.name + "=" + field.value);
    }

    void events::handleMessage(threadStream &stream, const uint8_t *message, size_t size) {
        vkdebug::messageHeader header;
        /* [xbilko03] copy header of the message from receive buffer */
        memcpy(&header, message, sizeof(header));

        switch (header.messageType) {
            case vkdebug::MessageCallBegin:
                /* [xbilko03] process previous call if anything was left there */
                applyOpenCall(stream);
                /* [xbilko03] save a copy of this message to openCall */
                stream.openCall.push_back(std::vector<uint8_t>(message, message + size));
                /* [xbilko03] show MessageCallBegin type of message to inform UI that such call was being made */
                showCallBegin(message, size);
                stream.lastCall = functionName(header.function);
                stream.lastCallID = header.call;
                stream.inCall = true;
                break;
            case vkdebug::MessageCallInputs:
                /* [xbilko03] save a copy of this message to openCall */
                stream.openCall.push_back(std::vector<uint8_t>(message, message + size));
                /* [xbilko03] add input parameters into the row */
                showCallInputs(message, size);
                break;
            case vkdebug::MessageCallEnd:
                /* [xbilko03] save a copy of this message to openCall */
                stream.openCall.push_back(std::vector<uint8_t>(message, message + size));
                /* [xbilko03] decode all data about this call and save it to UI's data */
                applyOpenCall(stream);

                stream.inCall = false;
                stream.callCount++;
                break;
            case vkdebug::MessageLayerInfo:
                /* [xbilko03] if message is part of this call (needsCallContext), store it in stream.openCall, else apply it to UI now */
                if (stream.openCall.empty() || size < sizeof(header) + 1 || !needsCallContext(message[sizeof(header)]))
                    applyMessage(message, size);
                else
                    stream.openCall.push_back(std::vector<uint8_t>(message, message + size));
                break;
            default:
                /* [xbilko03] if a call is not being processed, apply the message to UI now, otherwise it has to be a part of a call, store it in stream.openCall */
                if (stream.openCall.empty())
                    applyMessage(message, size);
                else
                    stream.openCall.push_back(std::vector<uint8_t>(message, message + size));
                break;
        }
    }

    void events::handleSegment(uint32_t threadId, uint64_t threadCreationTime, uint32_t flags, const uint8_t *bytes,
                               size_t size) {
        std::lock_guard<std::mutex> lock(parseMutex);
        std::pair<uint32_t, uint64_t> threadKey(threadId, threadCreationTime);
        threadStream &stream = threadStreams[threadKey];
        stream.threadId = threadId;

        size_t offset = 0;
        bool first = true;
        while (size - offset >= VKDBG_FRAGMENT_HEADER_SIZE) {
            uint32_t fragmentSize = 0;
            memcpy(&fragmentSize, bytes + offset, sizeof(fragmentSize));
            offset += VKDBG_FRAGMENT_HEADER_SIZE;
            if (fragmentSize > size - offset)
                break;
            bool continuesPrevious = first && (flags & vkdebug::ChunkFirstContinues) != 0;
            bool continuesNext = offset + fragmentSize == size && (flags & vkdebug::ChunkLastContinues) != 0;

            /* [xbilko03] fragment starts a new message, drop unfinished leftovers of previous message (if there are any)
             * safety guard: a message is cut only when layer fails to open next chunk (agent gone/not responding)
             * wrongly cut message like this must not be falsely a part of the next one
             */
            if (!continuesPrevious)
                stream.bytes.clear();
            /* [xbilko03] collect fragments into the stream.bytes */
            stream.bytes.insert(stream.bytes.end(), bytes + offset, bytes + offset + fragmentSize);
            /* [xbilko03] if fragment does not continue into the next chunk, handle the message and clear the stream.bytes */
            if (!continuesNext) {
                if (stream.bytes.size() >= sizeof(vkdebug::messageHeader))
                    handleMessage(stream, stream.bytes.data(), stream.bytes.size());
                stream.bytes.clear();
            }
            offset += fragmentSize;
            first = false;
        }
        glfwPostEmptyEvent();
    }

    /* AI-Assisted: end */

    DWORD WINAPI events::listenForData(__in LPVOID lpParameter) {
        /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
        /* [xbilko03] create socket */
        SOCKET clientSocket = INVALID_SOCKET;
        uiWinsockInit(&clientSocket);

        vkdebug::ui::events *myObj = (vkdebug::ui::events *) lpParameter;
        myObj->connectSocket = clientSocket;
        (*myObj).createDataManagers();
        {
            std::lock_guard<std::mutex> lock(myObj->parseMutex);
            myObj->layerConnected = true;
        }
        glfwPostEmptyEvent();

        std::vector<uint8_t> payload;
        while (true) {
            uint16_t length = 0;
            if (!recvExact(clientSocket, &length, 2) || length == 0)
                break;
            payload.resize(length);
            if (!recvExact(clientSocket, payload.data(), length))
                break;
            /* [xbilko03] an error in one message must not end the receiving thread, write crashes into "vkdebugger_crash_diag.log" */
            try {
                myObj->handleControl(payload[0], payload.data() + 1, length - 1u);
            } catch (const std::exception &e) {
                std::ofstream log("vkdebugger_crash_diag.log", std::ios::app);
                log << "listenForData: handleControl threw: " << e.what() << std::endl;
            }
        }

        {
            /* [xbilko03] layer closed connection|target app is dead */
            std::lock_guard<std::mutex> lock(myObj->parseMutex);
            myObj->layerLost = true;
        }
        glfwPostEmptyEvent();

        /* [xbilko03] Destroy socket */
        /* AI-Assisted: end */
        uiWinsockExit(&clientSocket);
        return 0;
    }

    /* AI-Assisted: Claude Opus 5.5: syntax */
    DWORD WINAPI events::listenForAgent(__in LPVOID lpParameter) {
        vkdebug::ui::events *myObj = (vkdebug::ui::events *) lpParameter;

        SOCKET agent = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        address.sin_port = htons(myObj->agentPort);
        if (agent == INVALID_SOCKET || connect(agent, (sockaddr *) (&address), sizeof(address)) == SOCKET_ERROR)
            return 0;
        myObj->agentSocket = agent;

        std::vector<uint8_t> bytes;
        while (true) {
            vkdebug::resultChunkHeader segment;
            if (!recvExact(agent, &segment, sizeof(segment)))
                break;
            bytes.resize(segment.length);
            if (!recvExact(agent, bytes.data(), segment.length))
                break;
            try {
                myObj->handleSegment(segment.threadId, segment.threadCreationTime, segment.flags, bytes.data(),
                                     bytes.size());
            } catch (const std::exception &e) {
                std::ofstream log("vkdebugger_crash_diag.log", std::ios::app);
                log << "listenForAgent: handleSegment threw: " << e.what() << std::endl;
            }
        }
        closesocket(agent);
        return 0;
    }

    /* AI-Assisted: end */

    void events::connectToLayer(vkdebug::ui::renderer *windowManager) {
        /* receive window object reference from debuggerUI.cpp */
        winMan = windowManager;
        DWORD mythreadId;
        /* Collect data from socket continuously */
        CreateThread(0, 0, events::listenForData, this, 0, &mythreadId);
    }
}

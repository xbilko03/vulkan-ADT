/*
* Name		    : memoryManager.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file for data class that handles the VkDeviceMemory state and its data
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#pragma once
#include <list>
#include <string>
#include <map>
/* AI-Assisted: Claude Sonnet 5: syntax */
#include <mutex>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstring>
/* AI-Assisted: end */
#include "apiCallsManager.hpp"

namespace vkdebug::ui {
    /* abort the operation in case the user is trying to access something that does not exist */
#define MEMORY_BOUNDARY_CHECK if (memoryMap.count(inputID) == 0) return

    class vkMemoryManager {
    public:
        /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
        /* [xbilko03] create a new vkMemory */
        void newMemory(unsigned long long inputID) { memoryMap[inputID].memoryState = "allocated"; }
        /* [xbilko03] get memory state */
        std::string getState(unsigned long long inputID) {
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (memoryMap.count(inputID) == 0)
                return "";
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            std::lock_guard<std::mutex> lock(memoryMap[inputID].dataMutex);
            return memoryMap[inputID].memoryState;
        }

        void setState(unsigned long long inputID, std::string input) {
            MEMORY_BOUNDARY_CHECK;
            std::lock_guard<std::mutex> lock(memoryMap[inputID].dataMutex);
            memoryMap[inputID].memoryState = input;
        }

        /* get the total allocated memory objects */
        unsigned long long getMemoryCount() { return memoryMap.size(); }
        /* pointer to the VkDeviceMemory handle */
        void assignPointer(unsigned long long inputID, std::string inputPtr) {
            MEMORY_BOUNDARY_CHECK;
            std::lock_guard<std::mutex> lock(memoryMap[inputID].dataMutex);
            memoryMap[inputID].localPointer = inputPtr;
        }

        std::string getPointer(unsigned long long inputID) {
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (memoryMap.count(inputID) == 0)
                return "";
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            std::lock_guard<std::mutex> lock(memoryMap[inputID].dataMutex);
            return memoryMap[inputID].localPointer;
        }

        /* call related to the memory object */
        void assignCulprit(unsigned long long inputID, vkdebug::ui::apiCall *inputCulprit) {
            MEMORY_BOUNDARY_CHECK;
            std::lock_guard<std::mutex> lock(memoryMap[inputID].dataMutex);
            memoryMap[inputID].culprit = inputCulprit;
        }

        vkdebug::ui::apiCall *getCulprit(unsigned long long inputID) {
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (memoryMap.count(inputID) == 0)
                return NULL;
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            std::lock_guard<std::mutex> lock(memoryMap[inputID].dataMutex);
            return memoryMap[inputID].culprit;
        }

        /* binary data that this object contains */
        void assignData(unsigned long long inputID, char *inputData, unsigned long long size) {
            MEMORY_BOUNDARY_CHECK;
            std::lock_guard<std::mutex> lock(memoryMap[inputID].dataMutex);
            memoryMap[inputID].memoryData = inputData;
            memoryMap[inputID].dataSize = size;
            convertToReadable(inputID, inputData, size);
        }

        char *getData(unsigned long long inputID) {
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (memoryMap.count(inputID) == 0)
                return NULL;
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            std::lock_guard<std::mutex> lock(memoryMap[inputID].dataMutex);
            return memoryMap[inputID].memoryData;
        }

        unsigned long long getDataSize(unsigned long long inputID) {
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (memoryMap.count(inputID) == 0)
                return 0;
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            std::lock_guard<std::mutex> lock(memoryMap[inputID].dataMutex);
            return memoryMap[inputID].dataSize;
        }

        /* hexadecimal data */
        std::string getDataReadable(unsigned long long inputID) {
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (memoryMap.count(inputID) == 0)
                return "";
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            std::lock_guard<std::mutex> lock(memoryMap[inputID].dataMutex);
            return memoryMap[inputID].readableMemoryData;
        }

        /* relation between memory and either VkImage and VkBuffer */
        void assignBoundObj(unsigned long long inputID, std::string inputData) {
            MEMORY_BOUNDARY_CHECK;
            std::lock_guard<std::mutex> lock(memoryMap[inputID].dataMutex);
            memoryMap[inputID].boundObjPtr = inputData;
        }

        std::string getBoundObj(unsigned long long inputID) {
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (memoryMap.count(inputID) == 0)
                return "";
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            std::lock_guard<std::mutex> lock(memoryMap[inputID].dataMutex);
            return memoryMap[inputID].boundObjPtr;
        }

        /* AI-Assisted: end */
        /* look for an memoryID based on the pointer */
        unsigned long long getFromPointerID(std::string message);

        /* VkMemoryObject */
        struct vkMemory;

        vkMemory *getMemory(unsigned long long inputID) {
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (memoryMap.count(inputID) == 0)
                return NULL;
            /* AI-Assisted: end */
            return &memoryMap[inputID];
        }

        struct vkMemory {
            unsigned long long ID;
            std::string localPointer;
            std::string memoryState;
            std::string boundObjPtr;
            std::string readableMemoryData;
            /* AI-Assisted: Claude Sonnet 5: syntax */
            char *memoryData = nullptr;
            unsigned long long dataSize = 0;
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            vkdebug::ui::apiCall *culprit = NULL;
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            /* [xbilko03] data guard: input data is writen by receiving thread and read by render thread */
            std::mutex dataMutex;

            std::string getReadable() {
                std::lock_guard<std::mutex> lock(dataMutex);
                return readableMemoryData;
            }

            char *getDataPtr() {
                std::lock_guard<std::mutex> lock(dataMutex);
                return memoryData;
            }

            unsigned long long getSize() {
                std::lock_guard<std::mutex> lock(dataMutex);
                return dataSize;
            }

            /* AI-Assisted: end */
        };

    private:
        /* map that has the saved data */
        std::map<unsigned long long, vkMemory> memoryMap;

        /* convert binary data to hex format */
        void convertToReadable(unsigned long long inputID, char *inputData, unsigned long long size);
    };
}

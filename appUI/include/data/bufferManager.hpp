/*
* Name		    : bufferManager.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file for data class that handles the VkBuffer state and its data
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#pragma once
#include <list>
#include <string>
/* AI-Assisted: Claude Sonnet 5: syntax */
#include <vector>
#include <mutex>
/* AI-Assisted: end */
#include "memoryManager.hpp"

#include <iostream>

#define BUFFER_BOUNDARY_CHECK if (bufferMap.count(inputID) == 0) return

namespace vkdebug::ui {
    class vkBufferManager {
    public:
        /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
        /* [xbilko03] create a new vkBuffer */
        void newBuffer(unsigned long long inputID) {
            bufferCount++;
            bufferMap[inputID].bufferState = "allocated";
        }

        /* [xbilko03] set vkBuffer state */
        void setState(unsigned long long inputID, std::string input) {
            BUFFER_BOUNDARY_CHECK;
            std::lock_guard<std::mutex> lock(bufferMap[inputID].dataMutex);
            bufferMap[inputID].bufferState = input;
        }

        std::string getState(unsigned long long inputID) {
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (bufferMap.count(inputID) == 0)
                return "";
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            std::lock_guard<std::mutex> lock(bufferMap[inputID].dataMutex);
            return bufferMap[inputID].bufferState;
        }

        /* get the total allocated BufferObjects */
        unsigned long long getBufferCount() { return bufferCount; }
        /* pointer operations */
        /* look for an bufferID based on the pointer */
        unsigned long long getFromPointerID(std::string message);

        std::string getPointer(unsigned long long inputID) {
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (bufferMap.count(inputID) == 0)
                return "";
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            std::lock_guard<std::mutex> lock(bufferMap[inputID].dataMutex);
            return bufferMap[inputID].localPointer;
        }

        /* return the pointer to the bound MemoryObject*/
        std::string getBoundObj(unsigned long long inputID) {
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (bufferMap.count(inputID) == 0)
                return "";
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            std::lock_guard<std::mutex> lock(bufferMap[inputID].dataMutex);
            return bufferMap[inputID].attachedMemoryPtr;
        }

        /* new data & state change */
        void assignPointer(unsigned long long inputID, std::string ptr) {
            BUFFER_BOUNDARY_CHECK;
            std::lock_guard<std::mutex> lock(bufferMap[inputID].dataMutex);
            bufferMap[inputID].localPointer = ptr;
            bufferMap[inputID].bufferState = "created";
        }

        void assignBoundObj(unsigned long long inputID, std::string inputData) {
            BUFFER_BOUNDARY_CHECK;
            std::lock_guard<std::mutex> lock(bufferMap[inputID].dataMutex);
            bufferMap[inputID].attachedMemoryPtr = inputData;
        }

        void assignMemory(unsigned long long inputID, vkMemoryManager::vkMemory *ptr) {
            BUFFER_BOUNDARY_CHECK;
            std::lock_guard<std::mutex> lock(bufferMap[inputID].dataMutex);
            bufferMap[inputID].attachedMemory = ptr;
        }

        void assignCulprit(unsigned long long inputID, vkdebug::ui::apiCall *inputCulprit) {
            BUFFER_BOUNDARY_CHECK;
            std::lock_guard<std::mutex> lock(bufferMap[inputID].dataMutex);
            bufferMap[inputID].culprit = inputCulprit;
        }

        /* data operations */
        char *getData(unsigned long long inputID) {
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (bufferMap.count(inputID) == 0)
                return NULL;
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            std::lock_guard<std::mutex> lock(bufferMap[inputID].dataMutex);
            if (bufferMap[inputID].attachedMemory == NULL)
                return NULL;
            else
                return bufferMap[inputID].attachedMemory->getDataPtr();
        }

        unsigned long long getDataSize(unsigned long long inputID) {
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (bufferMap.count(inputID) == 0)
                return 0;
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            std::lock_guard<std::mutex> lock(bufferMap[inputID].dataMutex);
            if (bufferMap[inputID].attachedMemory == NULL)
                return NULL;
            else
                return bufferMap[inputID].attachedMemory->getSize();
        }

        std::string getDataReadable(unsigned long long inputID) {
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (bufferMap.count(inputID) == 0)
                return "";
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            std::lock_guard<std::mutex> lock(bufferMap[inputID].dataMutex);
            if (bufferMap[inputID].attachedMemory == NULL)
                return "";
            else
                return bufferMap[inputID].attachedMemory->getReadable();
        }

        /* get BufferObject related data */
        vkMemoryManager::vkMemory *getMemory(unsigned long long inputID) {
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (bufferMap.count(inputID) == 0)
                return NULL;
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            std::lock_guard<std::mutex> lock(bufferMap[inputID].dataMutex);
            return bufferMap[inputID].attachedMemory;
        }

        vkdebug::ui::apiCall *getCulprit(unsigned long long inputID) {
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            if (bufferMap.count(inputID) == 0)
                return NULL;
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            std::lock_guard<std::mutex> lock(bufferMap[inputID].dataMutex);
            return bufferMap[inputID].culprit;
        }

        /* AI-Assisted: end */

        /* saved data */
        struct vkBuffer {
            std::string localPointer;
            std::string bufferState;
            std::string attachedMemoryPtr;
            /* AI-Assisted: Claude Sonnet 5: syntax */
            vkMemoryManager::vkMemory *attachedMemory = nullptr;
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Opus 5.5: syntax */
            vkdebug::ui::apiCall *culprit = NULL;
            /* AI-Assisted: end */
            /* AI-Assisted: Claude Sonnet 5: syntax */
            /* [xbilko03] data guard: input data is writen by receiving thread and read by render thread */
            std::mutex dataMutex;
            /* AI-Assisted: end */
        };

        std::map<unsigned long long, vkBuffer> bufferMap;

    private:
        unsigned long long bufferCount = 0;
    };
}

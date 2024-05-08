/*
* Name		    : class_buffer.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file for data class that handles the VkBuffer state and its data
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#pragma once
#include <list>
#include <string>
#include "class_memory.hpp"

#include <iostream>

#define BOUNDARYCHECK if (bufferMap.count(inputID) == 0) return
namespace details {
    class vkBufferManager {
    public:
        /* create a new BufferObject */
        void newBuffer(unsigned long long inputID) { bufferCount++; bufferMap[inputID] = vkBuffer(); bufferMap[inputID].bufferState = "allocated"; }
        /* get BufferObject state */
        void setState(unsigned long long inputID, std::string input) { BOUNDARYCHECK; bufferMap[inputID].bufferState = input; }
        std::string getState(unsigned long long inputID) { return bufferMap[inputID].bufferState; }
        /* get the total allocated BufferObjects */
        unsigned long long getBufferCount() { return bufferCount; }
        /* pointer operations */
        /* look for an bufferID based on the pointer */
        unsigned long long getFromPointerID(std::string message);
        std::string getPointer(unsigned long long inputID) { return bufferMap[inputID].localPointer; }
        /* return the pointer to the bound MemoryObject*/
        std::string getBoundObj(unsigned long long inputID) { return bufferMap[inputID].attachedMemoryPtr; }
        
        /* new data & state change */
        void assignPointer(unsigned long long inputID, std::string ptr) { BOUNDARYCHECK; bufferMap[inputID].localPointer = ptr; this->bufferMap[inputID].bufferState = "created"; }
        void assignBoundObj(unsigned long long inputID, std::string inputData) { BOUNDARYCHECK; bufferMap[inputID].attachedMemoryPtr = inputData; }
        void assignMemory(unsigned long long inputID, vkMemoryManager::vkMemory* ptr) { BOUNDARYCHECK; bufferMap[inputID].attachedMemory = ptr; }
        void assignCulprit(unsigned long long inputID, details::apiCall* inputCulprit) { BOUNDARYCHECK; bufferMap[inputID].culprit = inputCulprit; }

        /* data operations */
        char* getData(unsigned long long inputID) { if (bufferMap[inputID].attachedMemory == NULL) return NULL; else return bufferMap[inputID].attachedMemory->memoryData; }
        unsigned long long getDataSize(unsigned long long inputID) { if (bufferMap[inputID].attachedMemory == NULL) return NULL; else return bufferMap[inputID].attachedMemory->dataSize; };
        std::string getDataReadable(unsigned long long inputID) { if (bufferMap[inputID].attachedMemory == NULL) return ""; else return bufferMap[inputID].attachedMemory->readableMemoryData; }
        /* get BufferObject related data */
        vkMemoryManager::vkMemory* getMemory(unsigned long long inputID) { if (bufferMap[inputID].attachedMemory == NULL) return NULL; else return bufferMap[inputID].attachedMemory; }
        details::apiCall* getCulprit(unsigned long long inputID) { return bufferMap[inputID].culprit; }

        /* saved data */
        struct vkBuffer {
            std::string localPointer;
            std::string bufferState;
            std::string attachedMemoryPtr;
            vkMemoryManager::vkMemory* attachedMemory;
            details::apiCall* culprit;
        };
        std::map<unsigned long long, vkBuffer> bufferMap;
    private:
        unsigned long long bufferCount = 0;
    };
}
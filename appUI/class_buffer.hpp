#pragma once
#include <list>
#include <string>
#include "class_memory.hpp"

#include <iostream>

#define BOUNDARYCHECK if (bufferMap.count(inputID) == 0) return
namespace details {
    class vkBufferManager {
    public:
        /* state change */
        void newBuffer(unsigned long long inputID) { bufferCount++; bufferMap[inputID] = vkBuffer(); bufferMap[inputID].bufferState = "allocated"; }
        void FreeBuffer(unsigned long long inputID) { BOUNDARYCHECK; bufferMap[inputID].bufferState = "freed"; }

        /* getInfo */
        std::string GetState(unsigned long long inputID) { return bufferMap[inputID].bufferState; }
        unsigned long long GetBufferCount() { return bufferCount; }
        unsigned long long GetFromPointerID(std::string message);

        std::string GetPointer(unsigned long long inputID) { return bufferMap[inputID].localPointer; }
        std::string GetBoundObj(unsigned long long inputID) { return bufferMap[inputID].attachedMemoryPtr; }
        
        /* new data*/
        void AssignPointer(unsigned long long inputID, std::string ptr) { BOUNDARYCHECK; bufferMap[inputID].localPointer = ptr; this->bufferMap[inputID].bufferState = "created"; }
        void AssignBoundObj(unsigned long long inputID, std::string inputData) { BOUNDARYCHECK; bufferMap[inputID].attachedMemoryPtr = inputData; }
        void AssignMemory(unsigned long long inputID, vkMemoryManager::vkMemory* ptr) { BOUNDARYCHECK; bufferMap[inputID].attachedMemory = ptr; }
        void AssignCulprit(unsigned long long inputID, details::apiCall* inputCulprit) { BOUNDARYCHECK; bufferMap[inputID].culprit = inputCulprit; }

        char* GetData(unsigned long long inputID) { if (bufferMap[inputID].attachedMemory == NULL) return NULL; else return bufferMap[inputID].attachedMemory->memoryData; }
        unsigned long long GetDataSize(unsigned long long inputID) { if (bufferMap[inputID].attachedMemory == NULL) return NULL; else return bufferMap[inputID].attachedMemory->dataSize; };
        std::string GetDataReadable(unsigned long long inputID) { if (bufferMap[inputID].attachedMemory == NULL) return ""; else return bufferMap[inputID].attachedMemory->readableMemoryData; }
        vkMemoryManager::vkMemory* GetMemory(unsigned long long inputID) { if (bufferMap[inputID].attachedMemory == NULL) return NULL; else return bufferMap[inputID].attachedMemory; }
        details::apiCall* GetCulprit(unsigned long long inputID) { return bufferMap[inputID].culprit; }

        

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
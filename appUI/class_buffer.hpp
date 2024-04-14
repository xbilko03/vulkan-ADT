#pragma once
#include <list>
#include <string>
#include "class_memory.hpp"

#include <iostream>

#define BOUNDARYCHECK if (inputID == -1) return
namespace details {
    class vkBufferManager {
    public:
        unsigned long long GetBufferCount() { return bufferCount; }
        std::string GetState(unsigned long long inputID) { return bufferMap[inputID].bufferState; }

        void newBuffer(unsigned long long inputID) { bufferCount++; bufferMap[inputID] = vkBuffer(); bufferMap[inputID].bufferState = "allocated"; }
        void FreeBuffer(unsigned long long inputID) { BOUNDARYCHECK; bufferMap[inputID].bufferState = "freed"; };

        std::string GetPointer(unsigned long long inputID) { return bufferMap[inputID].localPointer; }
        void AssignPointer(unsigned long long inputID, std::string ptr) { BOUNDARYCHECK; bufferMap[inputID].localPointer = ptr; this->bufferMap[inputID].bufferState = "created"; }
        
        void AssignData(unsigned long long inputID, char* inputData) { BOUNDARYCHECK; bufferMap[inputID].memoryData = inputData; }
        char* GetData(unsigned long long inputID) { return bufferMap[inputID].memoryData; }
        
        void AssignBoundObj(unsigned long long inputID, std::string inputData) { BOUNDARYCHECK; bufferMap[inputID].attachedMemoryPtr = inputData; }
        std::string GetBoundObj(unsigned long long inputID) { return bufferMap[inputID].attachedMemoryPtr; }        

        unsigned long long GetFromPointerID(std::string message);

        void FreeMemory(unsigned long long inputID) { BOUNDARYCHECK; bufferMap[inputID].bufferState = "freed"; }

        void AssignDataSize(unsigned long long inputID, unsigned long long size) { BOUNDARYCHECK; bufferMap[inputID].dataSize = size; }
        unsigned long long GetDataSize(unsigned long long inputID) { return bufferMap[inputID].dataSize; };

        struct vkBuffer {
            std::string localPointer;
            std::string bufferState;
            std::string attachedMemoryPtr;
            char* memoryData;
            unsigned long long dataSize;
        };
        std::map<unsigned long long, vkBuffer> bufferMap;
    private:
        unsigned long long bufferCount = 0;
    };
}
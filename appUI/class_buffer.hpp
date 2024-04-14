#pragma once
#include <list>
#include <string>
#include "class_memory.hpp"

#include <iostream>
namespace details {
    class vkBufferManager {
    public:
        unsigned long long GetBufferCount() { return this->bufferMap.size(); }
        std::string GetState(unsigned long long inputID) { return this->bufferMap[inputID].bufferState; }

        void newBuffer(unsigned long long inputID) { this->bufferMap[inputID] = vkBuffer(); bufferMap[inputID].bufferState = "allocated"; }
        void FreeBuffer(unsigned long long inputID) { this->bufferMap[inputID].bufferState = "freed"; };

        std::string GetPointer(unsigned long long inputID) { return this->bufferMap[inputID].localPointer; }
        void AssignPointer(unsigned long long inputID, std::string ptr) { this->bufferMap[inputID].localPointer = ptr; this->bufferMap[inputID].bufferState = "created"; }
        
        void AssignData(unsigned long long inputID, char* inputData) { this->bufferMap[inputID].memoryData = inputData; }
        char* GetData(unsigned long long inputID) { return this->bufferMap[inputID].memoryData; }
        
        void AssignBoundObj(unsigned long long inputID, std::string inputData) { this->bufferMap[inputID].attachedMemoryPtr = inputData; }
        std::string GetBoundObj(unsigned long long inputID) { return this->bufferMap[inputID].attachedMemoryPtr; }        

        unsigned long long GetFromPointerID(std::string message);
        std::string GetConvertRaw(unsigned long long inputID);

        void FreeMemory(unsigned long long inputID) { this->bufferMap[inputID].bufferState = "freed"; }
        char* GetDataRaw(unsigned long long inputID) { return this->bufferMap[inputID].memoryData; };
        unsigned long long GetDataSize(unsigned long long inputID) { return this->bufferMap[inputID].dataSize; };

        struct vkBuffer {
            std::string localPointer;
            std::string bufferState;
            std::string attachedMemoryPtr;
            char* memoryData;
            unsigned long long dataSize;
        };
        std::map<unsigned long long, vkBuffer> bufferMap;
    private:
    };
}
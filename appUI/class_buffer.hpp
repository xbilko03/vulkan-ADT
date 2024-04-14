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
        
        void AssignData(unsigned long long inputID, std::string inputData) { this->bufferMap[inputID].attachedMemoryData = inputData; }
        std::string GetData(unsigned long long inputID) { return this->bufferMap[inputID].attachedMemoryData; }
        
        void AssignBoundObj(unsigned long long inputID, std::string inputData) { this->bufferMap[inputID].attachedMemoryPtr = inputData; }
        std::string GetBoundObj(unsigned long long inputID) { return this->bufferMap[inputID].attachedMemoryPtr; }        

        unsigned long long GetFromPointerID(std::string message);
        std::string GetConvertRaw(unsigned long long inputID);

        void FreeMemory(unsigned long long inputID) { this->bufferMap[inputID].bufferState = "freed"; }
        char* GetDataRaw(unsigned long long inputID) { return this->bufferMap[inputID].attachedMemoryDataRaw; };

        struct vkBuffer {
            std::string localPointer;
            std::string bufferState;
            std::string attachedMemoryPtr;
            std::string attachedMemoryData;
            char* attachedMemoryDataRaw;
        };
        std::map<unsigned long long, vkBuffer> bufferMap;
    private:
    };
}
#pragma once
#include <list>
#include <string>
#include <map>
#include "class_apiCall.hpp"

namespace details {

    #define BOUNDARYCHECK if (memoryMap.count(inputID) == 0) return

    class vkMemoryManager {
    public:
        void newMemory(unsigned long long inputID) { memoryMap[inputID] = vkMemory(); memoryMap[inputID].memoryState = "allocated"; }
        std::string GetState(unsigned long long inputID) { return memoryMap[inputID].memoryState; }
        unsigned long long GetMemoryCount() { return memoryMap.size(); }

        void AssignPointer(unsigned long long inputID, std::string inputPtr) { BOUNDARYCHECK; memoryMap[inputID].localPointer = inputPtr; }
        std::string GetPointer(unsigned long long inputID) { return memoryMap[inputID].localPointer; }

        void AssignCulprit(unsigned long long inputID, details::apiCall* inputCulprit) { BOUNDARYCHECK; memoryMap[inputID].culprit = inputCulprit; }
        void AssignData(unsigned long long inputID, char* inputData, unsigned long long size) { BOUNDARYCHECK; memoryMap[inputID].memoryData = inputData; memoryMap[inputID].dataSize = size;ConvertToReadable(inputID, inputData, size); }
        char* GetData(unsigned long long inputID) { return memoryMap[inputID].memoryData; }
        std::string GetDataReadable(unsigned long long inputID) { return memoryMap[inputID].readableMemoryData; }
        unsigned long long GetDataSize(unsigned long long inputID) { return memoryMap[inputID].dataSize; }

        void AssignBoundObj(unsigned long long inputID, std::string inputData) { BOUNDARYCHECK; memoryMap[inputID].boundObjPtr = inputData; }
        std::string GetBoundObj(unsigned long long inputID) { return memoryMap[inputID].boundObjPtr; }
        details::apiCall* GetCulprit(unsigned long long inputID){ return memoryMap[inputID].culprit; }

        unsigned long long GetFromPointerID(std::string message);

        void SetState(unsigned long long inputID, std::string input) { BOUNDARYCHECK; memoryMap[inputID].memoryState = input; }

        struct vkMemory;
        vkMemory* GetMemory(unsigned long long inputID) { return &memoryMap[inputID]; }

        struct vkMemory {
            unsigned long long ID;
            std::string localPointer;
            std::string memoryState;
            std::string boundObjPtr;

            std::string readableMemoryData;
            char* memoryData;
            unsigned long long dataSize;

            details::apiCall* culprit;
        };
    private:
        std::map<unsigned long long, vkMemory> memoryMap;
        void ConvertToReadable(unsigned long long inputID, char* inputData, unsigned long long size);
    };
}
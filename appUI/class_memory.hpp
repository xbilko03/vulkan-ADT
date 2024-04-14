#pragma once
#include <list>
#include <string>
#include <map>

namespace details {
    class vkMemoryManager {
    public:
        void newMemory(unsigned long long inputID) { memoryMap[inputID] = vkMemory(); memoryMap[inputID].memoryState = "allocated"; }
        std::string GetState(unsigned long long inputID) { return memoryMap[inputID].memoryState; }
        unsigned long long GetMemoryCount() { return memoryMap.size(); }

        void AssignPointer(unsigned long long inputID, std::string inputPtr) { memoryMap[inputID].localPointer = inputPtr; }
        std::string GetPointer(unsigned long long inputID) { return memoryMap[inputID].localPointer; }

        void AssignData(unsigned long long inputID, std::string inputData) { memoryMap[inputID].memoryData = inputData; }
        std::string GetData(unsigned long long inputID) { return memoryMap[inputID].memoryData; }

        void AssignBoundObj(unsigned long long inputID, std::string inputData) { memoryMap[inputID].boundObjPtr = inputData; }
        std::string GetBoundObj(unsigned long long inputID) { return memoryMap[inputID].boundObjPtr; }

        unsigned long long GetFromPointerID(std::string message);

        void FreeMemory(unsigned long long inputID) { memoryMap[inputID].memoryState = "freed"; }

        struct vkMemory {
            unsigned long long ID;
            std::string localPointer;
            std::string memoryData;
            std::string memoryState;
            std::string boundObjPtr;
        };
    private:
        std::map<unsigned long long, vkMemory> memoryMap;
    };
}
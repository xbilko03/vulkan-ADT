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

        void AssignPointer(std::string inputPtr, unsigned long long inputID) { memoryMap[inputID].localPointer = inputPtr; }
        std::string GetPointer(unsigned long long inputID) { return memoryMap[inputID].localPointer; }

        void AssignData(std::string inputData, unsigned long long inputID) { memoryMap[inputID].memoryData = inputData; }
        std::string GetData(unsigned long long inputID) { return memoryMap[inputID].memoryData; }

        unsigned long long GetFromPointerID(std::string message);

        void FreeMemory(unsigned long long inputID) { memoryMap[inputID].memoryState = "freed"; }

    private:
        struct vkMemory{
        unsigned long long ID;
        std::string localPointer;
        std::string memoryData;
        std::string memoryState;
        };
        std::map<unsigned long long, vkMemory> memoryMap;
    };
}
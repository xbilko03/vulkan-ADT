/*
* Name		    : class_memory.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file for data class that handles the VkDeviceMemory state and its data
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#pragma once
#include <list>
#include <string>
#include <map>
#include "class_apiCall.hpp"

namespace details {
    /* abort the operation in case the user is trying to access something that does not exist */
    #define BOUNDARYCHECK if (memoryMap.count(inputID) == 0) return

    class vkMemoryManager {
    public:
        /* create a new MemoryObject */
        void newMemory(unsigned long long inputID) { memoryMap[inputID] = vkMemory(); memoryMap[inputID].memoryState = "allocated"; }
        /* get memory state */
        std::string getState(unsigned long long inputID) { return memoryMap[inputID].memoryState; }
        void setState(unsigned long long inputID, std::string input) { BOUNDARYCHECK; memoryMap[inputID].memoryState = input; }
        /* get the total allocated memory objects */
        unsigned long long getMemoryCount() { return memoryMap.size(); }
        /* pointer to the VkDeviceMemory handle */
        void assignPointer(unsigned long long inputID, std::string inputPtr) { BOUNDARYCHECK; memoryMap[inputID].localPointer = inputPtr; }
        std::string getPointer(unsigned long long inputID) { return memoryMap[inputID].localPointer; }
        /* call related to the memory object */
        void assignCulprit(unsigned long long inputID, details::apiCall* inputCulprit) { BOUNDARYCHECK; memoryMap[inputID].culprit = inputCulprit; }
        details::apiCall* getCulprit(unsigned long long inputID) { return memoryMap[inputID].culprit; }
        /* binary data that this object contains */
        void assignData(unsigned long long inputID, char* inputData, unsigned long long size) { BOUNDARYCHECK; memoryMap[inputID].memoryData = inputData; memoryMap[inputID].dataSize = size; convertToReadable(inputID, inputData, size); }
        char* getData(unsigned long long inputID) { return memoryMap[inputID].memoryData; }
        unsigned long long getDataSize(unsigned long long inputID) { return memoryMap[inputID].dataSize; }
        /* hexadecimal data */
        std::string getDataReadable(unsigned long long inputID) { return memoryMap[inputID].readableMemoryData; }
        /* relation between memory and either VkImage and VkBuffer */
        void assignBoundObj(unsigned long long inputID, std::string inputData) { BOUNDARYCHECK; memoryMap[inputID].boundObjPtr = inputData; }
        std::string getBoundObj(unsigned long long inputID) { return memoryMap[inputID].boundObjPtr; }
        /* look for an memoryID based on the pointer */
        unsigned long long getFromPointerID(std::string message);
        /* VkMemoryObject */
        struct vkMemory;
        vkMemory* getMemory(unsigned long long inputID) { return &memoryMap[inputID]; }
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
        /* map that has the saved data */
        std::map<unsigned long long, vkMemory> memoryMap;
        /* convert binary data to hex format */
        void convertToReadable(unsigned long long inputID, char* inputData, unsigned long long size);
    };
}
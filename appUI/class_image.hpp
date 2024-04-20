#pragma once
#include <list>
#include <string>
#include "class_buffer.hpp"

namespace details {
    class vkImageManager : public vkBufferManager {
    public:
        void SetWidth(unsigned long long inputID, std::string inputWidth) { bufferMap[inputID].width = std::stoul(inputWidth); }
        unsigned long long GetWidth(unsigned long long inputID) { return bufferMap[inputID].width; }
        
        void SetHeight(unsigned long long inputID, std::string inputHeight) { bufferMap[inputID].height = std::stoul(inputHeight); }
        unsigned long long GetHeight(unsigned long long inputID) { return bufferMap[inputID].height; }

        struct vkBuffer {
            std::string localPointer;
            std::string bufferState;
            std::string attachedMemoryPtr;
            char* memoryData;
            unsigned long long dataSize;
            unsigned long long width;
            unsigned long long height;
        };
        std::map<unsigned long long, vkBuffer> bufferMap;
    private:
    };
}
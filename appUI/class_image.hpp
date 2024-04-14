#pragma once
#include <list>
#include <string>
#include "class_buffer.hpp"

namespace details {
    class vkImageManager : public vkBufferManager {
    public:
        void AssignData(unsigned long long inputID, std::string inputData) {
            this->bufferMap[inputID].attachedMemoryData = inputData; this->bufferMap[inputID].attachedMemoryRawData = GetConvertRaw(inputID);
        }
        std::string GetDataRaw(unsigned long long inputID) { return this->bufferMap[inputID].attachedMemoryRawData; };
    private:
    };
}
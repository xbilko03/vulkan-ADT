#pragma once
#include <list>
#include <string>
#include "class_buffer.hpp"

namespace details {
    class vkImageManager : public vkBufferManager {
    public:
        void AssignDataRaw(unsigned long long inputID, std::string inputData);
    private:
    };
}
#include "class_buffer.hpp"

#include <iostream>

namespace details {
    unsigned long long vkBufferManager::GetFromPointerID(std::string message)
    {
        for (auto& [key, value] : bufferMap) {
            if (value.bufferState == "freed")
                continue;
            if (message == value.localPointer)
                return key;
        }
        return -1;
    }
}
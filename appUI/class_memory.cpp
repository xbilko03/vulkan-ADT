#include "class_memory.hpp"


#include <iostream>

namespace details {
    unsigned long long vkMemoryManager::GetFromPointerID(std::string message)
    {
        /* input format 'layer_ptr=' */
        for (auto& [key, value] : memoryMap) {
            if (value.memoryState == "freed")
                continue;
            if (message == value.localPointer)
                return key;
        }
        return -1;
    }
}
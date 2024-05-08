/*
* Name		    : class_buffer.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file for data class that handles the VkBuffer state and its data
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#include "class_buffer.hpp"

#include <iostream>
#include <sstream>

namespace details {
    /* look for an bufferID based on the pointer */
    unsigned long long vkBufferManager::getFromPointerID(std::string message)
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
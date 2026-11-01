/*
* Name		    : bufferManager.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file for data class that handles the VkBuffer state and its data
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#include "../../include/data/bufferManager.hpp"

#include <iostream>
#include <sstream>

namespace vkdebug::ui {
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
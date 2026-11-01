/*
* Name		    : memoryManager.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file for data class that handles the VkDeviceMemory state and its data
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#include "../../include/data/memoryManager.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
/* AI-Assisted: Claude Sonnet 5: syntax */
#include <cstring>
/* AI-Assisted: end */

namespace vkdebug::ui {
    unsigned long long vkMemoryManager::getFromPointerID(std::string message)
    {
        for (auto& [key, value] : memoryMap) {
            if (value.memoryState == "freed")
                continue;
            if (message == value.localPointer)
                return key;
        }
        return -1;
    }
    /* max buffer inspect length */
    #define MAX_READABLE_SIZE 100 * sizeof(int)

    void vkMemoryManager::convertToReadable(unsigned long long inputID, char* inputData, unsigned long long size)
    {
        std::string output = "";
        std::stringstream stream;

        unsigned long long dataSize = MAX_READABLE_SIZE;
        if (dataSize > size)
            dataSize = size;

        int rows = 4;
        /* address offset for the given data */
        int address = 0x0000;

        for (unsigned long long i = 0; i < dataSize; i += sizeof(int))
        {

            if (i % (sizeof(int) * rows) == 0)
            {
                stream << "0x" << std::setfill('0') << std::setw(4) << std::uppercase << std::hex << address;
                output += stream.str() + " : ";
                address += sizeof(int) * rows;
                stream = {};
            }

            /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */
            unsigned int value = 0;
            unsigned long long bytesAvailable = size - i;
            if (bytesAvailable > sizeof(int))
                bytesAvailable = sizeof(int);
            std::memcpy(&value, inputData + i, (size_t)(bytesAvailable));
            stream << std::uppercase << std::hex << std::setfill('0') << std::setw(8) << value;
            /* AI-Assisted: end */
            output += stream.str();
            if ((i / sizeof(int) + 1) % rows == 0)
                output += "\n";
            else
                output += " ";
            stream.str("");
            stream.clear();
        }
        memoryMap[inputID].readableMemoryData = output;
    }
}
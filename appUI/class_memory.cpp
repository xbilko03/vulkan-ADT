#include "class_memory.hpp"


#include <iostream>
#include <sstream>
#include  <iomanip>

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


    /* max buffer inspect length */
#define MAXREADABLESIZE 100 * sizeof(int)

    void vkMemoryManager::ConvertToReadable(unsigned long long inputID, char* inputData, unsigned long long size)
    {
        std::string output = "";
        std::stringstream stream;

        unsigned long long dataSize = MAXREADABLESIZE;
        if (dataSize > size)
            dataSize = size;

        int rows = 4;
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

            stream << std::uppercase << std::hex << (int)(inputData + i);
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
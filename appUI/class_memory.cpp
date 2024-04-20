#include "class_memory.hpp"


#include <iostream>
#include <sstream>

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

        for (unsigned long long i = 0; i < dataSize;i += sizeof(int))
        {
            stream << std::hex << (int)(inputData + i);
            output += stream.str();
            if ((i / sizeof(int) + 1) % 5 == 0)
                output += "\n";
            else
                output += " ";
            stream.str("");
            stream.clear();
        }
        memoryMap[inputID].readableMemoryData = output;
    }
}
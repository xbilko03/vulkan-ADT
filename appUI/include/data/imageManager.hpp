/*
* Name		    : imageManager.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file for data class that handles the VkImage state and its data
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#pragma once
#include <list>
#include <string>
#include "bufferManager.hpp"

namespace vkdebug::ui {
    class vkImageManager : public vkBufferManager {
    public:
        /* VkExtend3D. It is exclusive to VkImage compared to VkBuffer */
        void setWidth(unsigned long long inputID, std::string inputWidth) {
            imageMap[inputID].width = std::stoul(inputWidth);
        }

        unsigned long long getWidth(unsigned long long inputID) { return imageMap[inputID].width; }

        void setHeight(unsigned long long inputID, std::string inputHeight) {
            imageMap[inputID].height = std::stoul(inputHeight);
        }

        unsigned long long getHeight(unsigned long long inputID) { return imageMap[inputID].height; }

        /* additional vkImage data exclusive to VkImage */
        struct vkImage {
            std::string localPointer;
            std::string bufferState;
            std::string attachedMemoryPtr;
            char *memoryData;
            unsigned long long dataSize;
            unsigned long long width;
            unsigned long long height;
        };

        std::map<unsigned long long, vkImage> imageMap;

    private:
    };
}

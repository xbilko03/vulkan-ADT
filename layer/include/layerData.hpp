/*
* Name		    : layerData.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file to store the current state of the Vulkan constructs and perform operations on them
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#pragma once

#include <vulkan/vulkan.h>
#include <map>
#include <vector>
#include <string>

namespace vkdebug::layer {
    class layerData {
    public:
        /* initializes the GPU to CPU memory reader mechanism */
        void initReader();

        /* [xbilko03] saves information of a new allocated memory */
        void newMemoryObj(VkDeviceMemory memory, VkDeviceSize size);

        /* frees the memory, rendering its handle free to use again */
        void freeMemoryObj(VkDeviceMemory memory);

        /* saves the reference to the instance in use */
        void newInstance(VkInstance instance);

        /* saves the reference ot the device in use */
        void newDevice(VkDevice device);

        /* associates a memory with a buffer */
        void newBinding(VkDeviceMemory memory, VkBuffer buffer);

        /* associates a memory with an image */
        void newBinding(VkDeviceMemory memory, VkImage image);

        /* associates a GPU with CPU memory */
        void memoryMapping(VkDeviceMemory memory, VkDeviceSize size, void **data);

        /* frees the memory mapping */
        void memoryUnMapping(VkDeviceMemory memory);

        /* creates saves a new image with the initial data */
        void newImage(VkImage image, VkImageLayout initialLayout, VkExtent3D extent);

        /* creates and saves a new buffer */
        void newBuffer(VkBuffer buffer);

        /* copies image data and saves them */
        void mapImageToBuffer(VkDeviceMemory memory);

        /* copies buffer data and saves them */
        void mapBufferToBuffer(VkDeviceMemory memory);

        /* sets the image state manually */
        void setState(VkImage image, std::string input) { memoryMap[imageMap[image].boundTo].status = input; }
        /* sets the buffer state manually */
        void setState(VkBuffer buffer, std::string input) { memoryMap[bufferMap[buffer].boundTo].status = input; }
        /* returns the memorySize */
        VkDeviceSize getSize(VkDeviceMemory memory) { return memoryMap[memory].size; }
        /* returns the data of a certain memory */
        void **getData(VkDeviceMemory memory) { return memoryMap[memory].data; }
        /* returns the last copied data */
        void **getBufferData() { return &bufferData; };
        /* AI-Assisted: Claude Sonnet 5 + Claude Opus 5.5: syntax */

        /* [xbilko03] keeps track of which layout is each image in */
        bool isImageTracked(VkImage image) { return imageMap.count(image) != 0; }
        VkImageLayout getImageLayout(VkImage image) { return imageMap[image].layout; }

        void updateImageLayout(VkImage image, VkImageLayout newLayout) {
            if (imageMap.count(image) != 0)
                imageMap[image].layout = newLayout;
        }

        /* AI-Assisted: end */
        /* structure representing VkMemory */
        typedef struct memoryObj {
            VkDeviceSize size;
            VkBuffer boundBuffer;
            VkImage boundImage;
            std::string status;
            std::string boundTo;
            void **data;
        };

        /* structure representing VkImage */
        typedef struct imageObj {
            VkImageLayout layout;
            VkExtent3D extent;
            VkDeviceMemory boundTo;
        };

        /* structure representing VkBuffer*/
        typedef struct bufferObj {
            VkDeviceMemory boundTo;
        };

        /* map to remember VkMemoryObjects */
        std::map<VkDeviceMemory, memoryObj> memoryMap;
        /* return the address of VkMemoryObjects */
        std::map<VkDeviceMemory, memoryObj> *getMemoryMap() { return &memoryMap; }

    private:
        /* limit on maximum copy capacity */
#define MAX_BUFFER_SIZE 5000000
        /* map to remember VkImageObjects */
        std::map<VkImage, imageObj> imageMap;
        /* map to remember VkBufferObjects */
        std::map<VkBuffer, bufferObj> bufferMap;
        /* last copied data */
        void *bufferData;
        /* current instance */
        VkInstance instanceAddr;
        /* current device */
        VkDevice deviceAddr;
        /* current physicalDevice*/
        VkPhysicalDevice physicalDeviceNew;
        /* command buffer to help copy the data */
        VkCommandBuffer commandBuffer;
        /* destination VkMemory which is instantiated in CPU readable format */
        VkDeviceMemory bufferMemory;
        /* destination VkBuffer which is instantiated in CPU readable format */
        VkBuffer buffer;
        /* graphics queue to help copy the data */
        VkQueue graphicsQueue;
    };
}

/* AI-Assisted: Claude Sonnet 5: syntax */
/* [xbilko03] object for layer to keep track of vulkan objects/other data -- shared with other layer files */
extern vkdebug::layer::layerData *data;
/* AI-Assisted: end */

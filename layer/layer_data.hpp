#pragma once

#include <vulkan/vulkan.h>
#include <iostream>
#include <map>
#include <vector>


namespace details {
    class layerData {
    public:
        void initReader();
        
        void newMemoryObj(VkDeviceMemory memory, VkDeviceSize size);
        void freeMemoryObj(VkDeviceMemory memory);

        void newInstance(VkInstance instance);
        void newDevice(VkDevice device);

        void newBinding(VkDeviceMemory memory, VkBuffer buffer);
        void newBinding(VkDeviceMemory memory, VkImage image);

        void memoryMapping(VkDeviceMemory memory, VkDeviceSize size, void** data);
        void memoryUnMapping(VkDeviceMemory memory);

        void newImage(VkImage image, VkImageLayout initialLayout, VkExtent3D extent);

        VkDeviceSize getSize(VkDeviceMemory memory) { return memoryMap[memory].size; }
        void** getData(VkDeviceMemory memory) { return memoryMap[memory].data; }



        typedef struct memoryObj {
            VkDeviceSize size;
            VkBuffer boundBuffer;
            VkImage boundImage;
            std::string status;
            std::string boundTo;

            void** data;
        };
        typedef struct imageObj {
            VkImageLayout layout;
            VkExtent3D extent;
        };

        std::map<VkDeviceMemory, memoryObj> memoryMap;

        std::map<VkDeviceMemory, memoryObj>* getMemoryMap() { return &memoryMap; }

    private:
        /* VkMemory */
        std::map<VkImage, imageObj> imageMap;
    };
}
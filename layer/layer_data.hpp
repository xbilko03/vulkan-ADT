#pragma once

#include <vulkan/vulkan.h>
#include <map>
#include <vector>
#include <string>

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
        void newBuffer(VkBuffer buffer);

        void mapImageToBuffer(VkDeviceMemory memory);
        void mapBufferToBuffer(VkDeviceMemory memory);

        void setState(VkImage image, std::string input) { memoryMap[imageMap[image].boundTo].status = input; }
        void setState(VkBuffer buffer, std::string input) { memoryMap[bufferMap[buffer].boundTo].status = input; }

        VkDeviceSize getSize(VkDeviceMemory memory) { return memoryMap[memory].size; }
        void** getData(VkDeviceMemory memory) { return memoryMap[memory].data; }
        void** getBufferData() { return &bufferData;  };

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
            VkDeviceMemory boundTo;
        };
        typedef struct bufferObj {
            VkDeviceMemory boundTo;
        };


        std::map<VkDeviceMemory, memoryObj> memoryMap;

        std::map<VkDeviceMemory, memoryObj>* getMemoryMap() { return &memoryMap; }

    private:
        std::map<VkImage, imageObj> imageMap;
        std::map<VkBuffer, bufferObj> bufferMap;
        void* bufferData;
    };
}
/*
* Name		    : systemManager.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file for data class that handles the Vulkan details
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#include "../../include/data/systemManager.hpp"

namespace vkdebug::ui {
    vkSystemManager::vkSystemManager(vkdebug::ui::renderer * winMan) {
        
        long long unsigned counter = 1;
        for (auto& item : winMan->getDeviceExtensions())
        {
            this->newVkInfo("device extension #" + std::to_string(counter++) + " " + item.extensionName);
        }
        counter = 1;
        for (auto& item : winMan->getInstanceExtensions())
        {
            this->newVkInfo("instance extension #" + std::to_string(counter++) + " " + item.extensionName);
        }
        counter = 1;
        for (auto& item : winMan->getPhysicalDeviceProperties())
        {
            this->newVkInfo("physical device #" + std::to_string(counter++) + " " + item.deviceName);
        }
    }
}
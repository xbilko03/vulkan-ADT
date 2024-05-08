/*
* Name		    : class_system.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file for data class that handles the Vulkan details
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#include "class_system.hpp"

namespace details {
    /* get information about Vulkan API */
    vkSystemManager::vkSystemManager(details::appWindow * winMan) {
        
        long long unsigned counter = 1;
        for (auto& item : winMan->GetDeviceExtensions())
        {
            this->newVkInfo("device extension #" + std::to_string(counter++) + " " + item.extensionName);
        }
        counter = 1;
        for (auto& item : winMan->GetInstanceExtensions())
        {
            this->newVkInfo("instance extension #" + std::to_string(counter++) + " " + item.extensionName);
        }
        counter = 1;
        for (auto& item : winMan->GetPhysicalDeviceProperties())
        {
            this->newVkInfo("physical device #" + std::to_string(counter++) + " " + item.deviceName);
        }
    }
}
/*
* Name		    : class_system.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file for data class that handles the Vulkan details
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#pragma once
#include <list>
#include <string>
#include "appWindow.hpp"

namespace details {
    class vkSystemManager {
    public:
        /* get information about Vulkan API */
        vkSystemManager(details::appWindow* winMan);
        /* save a single detail about it */
        void newVkInfo(std::string parameter) { vkInfo.push_back(parameter); }
        void newAppInfo(std::string parameter) { appInfo.push_back(parameter); }
        /* return the required data */
        std::list<std::string> getVkInfo() { return vkInfo; }
        std::list<std::string> getAppInfo() { return appInfo; }
    private:
        /* a list of Vulkan details */
        std::list<std::string> vkInfo;
        std::list<std::string> appInfo;
    };
}
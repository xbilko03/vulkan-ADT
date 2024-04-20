#pragma once
#include <list>
#include <string>
#include "appWindow.hpp"

namespace details {
    class vkSystemManager {
    public:
        vkSystemManager(details::appWindow* winMan) {

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
        void newVkInfo(std::string parameter) { vkInfo.push_back(parameter); }
        void newAppInfo(std::string parameter) { appInfo.push_back(parameter); }
        std::list<std::string> getVkInfo() { return vkInfo; }
        std::list<std::string> getAppInfo() { return appInfo; }
    private:
        std::list<std::string> vkInfo;
        std::list<std::string> appInfo;
    };
}
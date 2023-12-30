#include <stdio.h>
#include <list>
#include <string>

#include "events.h"

struct api_call
{
    std::string name;
    int count;
};

api_call apiArray[512] =
{
    api_call{"vkCreateInstance",0},
    api_call{"vkDestroyInstance",0},
    api_call{"vkCreateDevice",0},
    api_call{"vkDestroyDevice",0},
    api_call{"vkBeginCommandBuffer",0},
    api_call{"vkCmdDraw",0},
    api_call{"vkCmdDrawIndexed",0},
    api_call{"vkEndCommandBuffer",0},
    api_call{"vkEnumerateInstanceLayerProperties",0},
    api_call{"vkEnumerateDeviceLayerProperties",0},
    api_call{"vkEnumerateInstanceExtensionProperties",0},
    api_call{"vkEnumerateDeviceExtensionProperties",0},
    api_call{"vkGetDeviceProcAddr",0},
    api_call{"vkGetInstanceProcAddr",0}
};
enum api_call_enum {
    vkCreateInstance,
    vkDestroyInstance,
    vkCreateDevice,
    vkDestroyDevice,
    vkBeginCommandBuffer,
    vkCmdDraw,
    vkCmdDrawIndexed,
    vkEndCommandBuffer,
    vkEnumerateInstanceLayerProperties,
    vkEnumerateDeviceLayerProperties,
    vkEnumerateInstanceExtensionProperties,
    vkEnumerateDeviceExtensionProperties,
    vkGetDeviceProcAddr,
    vkGetInstanceProcAddr
};

void layer_event(const char* input)
{
    /* Trace api calls */
    if (input == "vkCreateInstance")
        apiArray[vkCreateInstance].count++;

    else if (input == "vkDestroyInstance")
        apiArray[vkDestroyInstance].count++;

    else if (input == "vkCreateDevice")
        apiArray[vkCreateDevice].count++;

    else if (input == "vkDestroyDevice")
        apiArray[vkDestroyDevice].count++;

    else if (input == "vkBeginCommandBuffer")
        apiArray[vkBeginCommandBuffer].count++;

    else if (input == "vkCmdDraw")
        apiArray[vkCmdDraw].count++;

    else if (input == "vkCmdDrawIndexed")
        apiArray[vkCmdDrawIndexed].count++;

    else if (input == "vkEndCommandBuffer")
        apiArray[vkEndCommandBuffer].count++;

    else if (input == "vkEnumerateInstanceLayerProperties")
        apiArray[vkEnumerateInstanceLayerProperties].count++;

    else if (input == "vkEnumerateDeviceLayerProperties")
        apiArray[vkEnumerateDeviceLayerProperties].count++;

    else if (input == "vkEnumerateInstanceExtensionProperties")
        apiArray[vkEnumerateInstanceExtensionProperties].count++;

    else if (input == "vkEnumerateDeviceExtensionProperties")
        apiArray[vkEnumerateDeviceExtensionProperties].count++;

    else if (input == "vkGetDeviceProcAddr")
        apiArray[vkGetDeviceProcAddr].count++;

    else if (input == "vkGetInstanceProcAddr")
        apiArray[vkGetInstanceProcAddr].count++;
}
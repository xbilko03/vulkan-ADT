#include <stdio.h>
#include <list>

#include "events.h"
#define TRACKED_API_COUNT 14

struct api_call
{
    std::string name;
    int count;
};
std::array <api_call, ARRAYLEN> apiArray =
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

bool cmp_input(const char* str1, const char* str2)
{
    if(strncmp(str1,str2, strlen(str2)) == 0)
        return true;
    
    return false;
}

void layer_event(const char* input)
{
    /* Trace api calls */
    if (cmp_input(input, "vkCreateInstance"))
        apiArray[vkCreateInstance].count++;

    else if (cmp_input(input, "vkDestroyInstance"))
        apiArray[vkDestroyInstance].count++;

    else if (cmp_input(input, "vkCreateDevice"))
        apiArray[vkCreateDevice].count++;

    else if (cmp_input(input, "vkDestroyDevice"))
        apiArray[vkDestroyDevice].count++;

    else if (cmp_input(input, "vkBeginCommandBuffer"))
        apiArray[vkBeginCommandBuffer].count++;

    else if (cmp_input(input, "vkCmdDraw"))
        apiArray[vkCmdDraw].count++;

    else if (cmp_input(input, "vkCmdDrawIndexed"))
        apiArray[vkCmdDrawIndexed].count++;

    else if (cmp_input(input, "vkEndCommandBuffer"))
        apiArray[vkEndCommandBuffer].count++;

    else if (cmp_input(input, "vkEnumerateInstanceLayerProperties"))
        apiArray[vkEnumerateInstanceLayerProperties].count++;

    else if (cmp_input(input, "vkEnumerateDeviceLayerProperties"))
        apiArray[vkEnumerateDeviceLayerProperties].count++;

    else if (cmp_input(input, "vkEnumerateInstanceExtensionProperties"))
        apiArray[vkEnumerateInstanceExtensionProperties].count++;

    else if (cmp_input(input, "vkEnumerateDeviceExtensionProperties"))
        apiArray[vkEnumerateDeviceExtensionProperties].count++;

    else if (cmp_input(input, "vkGetDeviceProcAddr"))
        apiArray[vkGetDeviceProcAddr].count++;

    else if (cmp_input(input, "vkGetInstanceProcAddr"))
        apiArray[vkGetInstanceProcAddr].count++;
}
int GetApiStatsLen()
{
    return TRACKED_API_COUNT;
}
std::string GetApiName(int index)
{
    return apiArray[index].name;
}
int GetApiCount(int index)
{
    return apiArray[index].count;
}
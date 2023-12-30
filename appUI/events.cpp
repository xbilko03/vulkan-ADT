#include <stdio.h>

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

std::list <int> api_call_history;

void new_call(int ID)
{
    apiArray[ID].count++;
    api_call_history.push_back(ID);
}
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
        new_call(vkCreateInstance);
    else if (cmp_input(input, "vkDestroyInstance"))
        new_call(vkDestroyInstance);
    else if (cmp_input(input, "vkCreateDevice"))
        new_call(vkCreateDevice);
    else if (cmp_input(input, "vkDestroyDevice"))
        new_call(vkDestroyDevice);
    else if (cmp_input(input, "vkBeginCommandBuffer"))
        new_call(vkBeginCommandBuffer);
    else if (cmp_input(input, "vkCmdDraw"))
        new_call(vkCmdDraw);
    else if (cmp_input(input, "vkCmdDrawIndexed"))
        new_call(vkCmdDrawIndexed);
    else if (cmp_input(input, "vkEndCommandBuffer"))
        new_call(vkEndCommandBuffer);
    else if (cmp_input(input, "vkEnumerateInstanceLayerProperties"))
        new_call(vkEnumerateInstanceLayerProperties);
    else if (cmp_input(input, "vkEnumerateDeviceLayerProperties"))
        new_call(vkEnumerateDeviceLayerProperties);
    else if (cmp_input(input, "vkEnumerateInstanceExtensionProperties"))
        new_call(vkEnumerateInstanceExtensionProperties);
    else if (cmp_input(input, "vkEnumerateDeviceExtensionProperties"))
        new_call(vkEnumerateDeviceExtensionProperties);
    else if (cmp_input(input, "vkGetDeviceProcAddr"))
        new_call(vkGetDeviceProcAddr);
    else if (cmp_input(input, "vkGetInstanceProcAddr"))
        new_call(vkGetInstanceProcAddr);
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
std::string GetApiEventName(int ID)
{
    switch (ID)
    {
    case vkCreateInstance:
        return "vkCreateInstance";
        break;
    case vkDestroyInstance:
        return "vkDestroyInstance";
        break;
    case vkCreateDevice:
        return "vkCreateDevice";
        break;
    case vkDestroyDevice:
        return "vkDestroyDevice";
        break;
    case vkBeginCommandBuffer:
        return "vkBeginCommandBuffer";
        break;
    case vkCmdDraw:
        return "vkCmdDraw";
        break;
    case vkCmdDrawIndexed:
        return "vkCmdDrawIndexed";
        break;
    case vkEndCommandBuffer:
        return "vkEndCommandBuffer";
        break;
    case vkEnumerateInstanceLayerProperties:
        return "vkEnumerateInstanceLayerProperties";
        break;
    case vkEnumerateDeviceLayerProperties:
        return "vkEnumerateDeviceLayerProperties";
        break;
    case vkEnumerateInstanceExtensionProperties:
        return "vkEnumerateInstanceExtensionProperties";
        break;
    case vkEnumerateDeviceExtensionProperties:
        return "vkEnumerateDeviceExtensionProperties";
        break;
    case vkGetDeviceProcAddr:
        return "vkGetDeviceProcAddr";
        break;
    case vkGetInstanceProcAddr:
        return "vkGetInstanceProcAddr";
        break;
    }
    return "";
}
std::list <int> GetApiCallHistory()
{
    return api_call_history;
}
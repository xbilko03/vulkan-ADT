#include <stdio.h>

#include "events.h"
#define TRACKED_API_COUNT 14
#define CMD_BUFFER_COUNT 6

struct pair_val
{
    std::string name;
    int value;
};
std::array <pair_val, ARRAYLEN> apiArray =
{
    pair_val{"vkCreateInstance",0},
    pair_val{"vkDestroyInstance",0},
    pair_val{"vkCreateDevice",0},
    pair_val{"vkDestroyDevice",0},
    pair_val{"vkBeginCommandBuffer",0},
    pair_val{"vkCmdDraw",0},
    pair_val{"vkCmdDrawIndexed",0},
    pair_val{"vkEndCommandBuffer",0},
    pair_val{"vkEnumerateInstanceLayerProperties",0},
    pair_val{"vkEnumerateDeviceLayerProperties",0},
    pair_val{"vkEnumerateInstanceExtensionProperties",0},
    pair_val{"vkEnumerateDeviceExtensionProperties",0},
    pair_val{"vkGetDeviceProcAddr",0},
    pair_val{"vkGetInstanceProcAddr",0}
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

std::array <pair_val, CMD_BUFFER_COUNT> cmdbuffArray =
{
    pair_val{"DrawCalls",0},
    pair_val{"Instances",0},
    pair_val{"Vertices",0},
    pair_val{"cDrawCalls",0},
    pair_val{"cInstances",0},
    pair_val{"cVertices",0}
};
enum cmd_buff_enum {
    cmdbuffDrawCalls,
    cmdbuffInstances,
    cmdbuffVertices,
    cmdbuffcDrawCalls,
    cmdbuffcInstances,
    cmdbuffcVertices
};

std::list <int> api_call_history;

void new_call(int ID)
{
    apiArray[ID].value++;
    api_call_history.push_back(ID);
}
bool cmp_input(const char* str1, const char* str2)
{
    if (strncmp(str1, str2, strlen(str2)) == 0)
        return true;

    return false;
}
std::string GetNextArg(std::string args)
{
    if(args.find(",") == std::string::npos)
        return args;
    return args.substr(0, args.find(","));
}
void parse_arguments(const char* input)
{
    if (cmp_input(input, "vkCmdDraw") || cmp_input(input, "vkCmdDrawIndexed"))
    {
        cmdbuffArray[cmdbuffcDrawCalls].value++;
        cmdbuffArray[cmdbuffDrawCalls].value++;
        /* vk_CmdDraw(x,y) */
        std::string args;
        if(cmp_input(input, "vkCmdDraw"))
            args = input + strlen("vkCmdDraw(");
        else
            args = input + strlen("vkCmdDrawIndexed(");

        args = args.substr(0, args.find(")"));
        /* x,y */
        /* x - Instance Count */
        std::string x = GetNextArg(args);
        cmdbuffArray[cmdbuffInstances].value += stoi(x);
        cmdbuffArray[cmdbuffcInstances].value += stoi(x);

        /* Consume argument */
        args = args.substr(args.find(",") + 1, args.back());
        /* y - Vertex Count */
        std::string y = GetNextArg(args);
        cmdbuffArray[cmdbuffVertices].value += stoi(y) * stoi(x);
        cmdbuffArray[cmdbuffcVertices].value += stoi(y) * stoi(x);
    }
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
    {
        new_call(vkBeginCommandBuffer);
        cmdbuffArray[cmdbuffcDrawCalls].value = 0;
        cmdbuffArray[cmdbuffcInstances].value = 0;
        cmdbuffArray[cmdbuffcVertices].value = 0;
    }
    else if (cmp_input(input, "vkCmdDraw"))
    {
        new_call(vkCmdDraw);
        parse_arguments(input);
    }
    else if (cmp_input(input, "vkCmdDrawIndexed"))
    {
        new_call(vkCmdDrawIndexed);
        parse_arguments(input);
    }
    else if (cmp_input(input, "vkEndCommandBuffer"))
    {
        new_call(vkEndCommandBuffer);

        cmdbuffArray[cmdbuffcDrawCalls].value = 0;
        cmdbuffArray[cmdbuffcInstances].value = 0;
        cmdbuffArray[cmdbuffcVertices].value = 0;
    }
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
    return apiArray[index].value;
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
int GetCmdBuffVal(int ID)
{
    return cmdbuffArray[ID].value;
}
/*
* Name		: events.cpp
* Project	: A Debugging Tool for Vulkan API
* Director  : Ing. Ján Pečiva Ph.D.
* Author	: Jozef Bilko (xbilko03)
*/
#include <stdio.h>
#include "events.h"
#include <iostream>
#define TRACKED_API_COUNT 14
#define CMD_BUFFER_COUNT 6

/* structure to map a name to an integer value */
struct pair_val
{
    std::string name;
    int value;
};
/* create a new array made of the vkApi calls, most likely a subject to change */
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
/* enumerate vkApi calls, most likely a subject to change */
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
/* create a new structure to hold command buffer contents, most likely a subject to change */
std::array <pair_val, CMD_BUFFER_COUNT> cmdbuffArray =
{
    pair_val{"DrawCalls",0},
    pair_val{"Instances",0},
    pair_val{"Vertices",0},
    pair_val{"cDrawCalls",0},
    pair_val{"cInstances",0},
    pair_val{"cVertices",0}
};
/* enumerate command buffer contents, most likely a subject to change */
enum cmd_buff_enum {
    cmdbuffDrawCalls,
    cmdbuffInstances,
    cmdbuffVertices,
    cmdbuffcDrawCalls,
    cmdbuffcInstances,
    cmdbuffcVertices
};

/* create a new list where the api calls history should be saved to */
std::list <int> api_call_history;

void parseArguments(const char* input);
void newCall(int ID);
bool cmpInput(const char* str1, const char* str2);
/* 
* whenever something important happened at the layer, call this function
* it also handles the information provided by the layer 'input'
*/
void newLayerEvent(const char* input)
{
    std::string inp = input;
    std::cout << "[events.cpp] new input = " << inp << std::endl;
    //testBuffer.push_back(inp);
}
/* returns the number of how many vk api calls we are currently tracking */
int GetApiStatsLen()
{
    return TRACKED_API_COUNT;
}
/* returns the name of the vk api call */
std::string GetApiName(int index)
{
    return apiArray[index].name;
}
/* returns the value associated with a given ID of an vk api call */
int GetApiCount(int index)
{
    return apiArray[index].value;
}
/* returns the string value of a given vk api call according to its ID, this is definitely a subject to change */
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
/* returns the list containing the traced vk api calls */
std::list <int> GetApiCallHistory()
{
    return api_call_history;
}
/* returns the nth value of the command buffer structure */
int GetCmdBuffVal(int n)
{
    return cmdbuffArray[n].value;
}
/* stores the new vk api call to the list and increments the corresponding statistic */
void newCall(int ID)
{
    apiArray[ID].value++;
    api_call_history.push_back(ID);
}
/* compares two strings, likely a subject to change */
bool cmpInput(const char* str1, const char* str2)
{
    if (strncmp(str1, str2, strlen(str2)) == 0)
        return true;

    return false;
}
/* parses the arguments from a given string of arguments*/
std::string GetNextArg(std::string args)
{
    if (args.find(",") == std::string::npos)
        return args;
    return args.substr(0, args.find(","));
}
/* parse the arguments provided by the string which was send by the layer, likely a subject to change */
void parseArguments(const char* input)
{
    if (cmpInput(input, "vkCmdDraw") || cmpInput(input, "vkCmdDrawIndexed"))
    {
        cmdbuffArray[cmdbuffcDrawCalls].value++;
        cmdbuffArray[cmdbuffDrawCalls].value++;
        /* vk_CmdDraw(x,y) */
        std::string args;
        if (cmpInput(input, "vkCmdDraw"))
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
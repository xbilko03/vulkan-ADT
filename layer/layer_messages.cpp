/*
* Name		    : layer_messages.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file of the layer containing helper functions to transform layer data into readable string format
*
* Author : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#include "layer_messages.h"

std::string ptrToString(void** input)
{
    if (input == NULL)
        return "NULL";
    std::stringstream s;
    s << *input;
    return s.str();
}
std::string addrToString(void* input)
{
    if (input == NULL)
        return "NULL";
    std::stringstream s;
    s << input;
    return s.str();
}
std::string formulateMessage(std::string prefix, std::string paramName, std::string paramValue)
{
    std::string output = prefix;
    output += paramName;
    output += paramValue;
    output += '!';
    return output;
}
std::string charToString(char* input)
{
    if (input == NULL)
        return "NULL";
    std::stringstream s;
    s << *input;
    return s.str();
}
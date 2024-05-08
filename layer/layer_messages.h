/*
* Name		    : layer_messages.h
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file of the layer containing helper functions to transform layer data into readable string format
*
* Author : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#pragma once
#include <string>
#include <sstream>

std::string ptrToString(void** input);
std::string addrToString(void* input);
std::string charToString(char* input);
std::string formulateMessage(std::string prefix, std::string paramName, std::string paramValue);
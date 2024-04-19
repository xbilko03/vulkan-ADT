#pragma once
#include <string>
#include <sstream>

std::string ptrToString(void** input);
std::string addrToString(void* input);
std::string charToString(char* input);
std::string formulateMessage(std::string prefix, std::string paramName, std::string paramValue);
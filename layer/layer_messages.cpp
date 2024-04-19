#include "layer_messages.h"

/* Helper Functions */
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
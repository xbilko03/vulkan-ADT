/*
* Name		    : layerStrings.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file of the layer containing helper functions to transform layer data into readable string format
*
* Author : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#include "../include/layerStrings.hpp"

/* AI-Assisted: Claude Opus 5.5: syntax */
namespace vkdebug::layer {
/* AI-Assisted: end */

std::string addrToString(void* input)
{
    if (input == NULL)
        return "NULL";
    std::stringstream s;
    s << input;
    return s.str();
}
/* AI-Assisted: Claude Opus 5.5: syntax */
}
/* AI-Assisted: end */

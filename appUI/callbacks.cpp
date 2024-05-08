/*
* Name		    : callbacks.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file for data class that represents a single Vulkan function call
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#include "callbacks.h"

void glfw_error_callback(int error, const char* description)
{
    throw std::runtime_error("GLFW: Error " + std::to_string(error) + ": " + description);
}
void check_vk_result(VkResult err)
{
    if (err == 0)
        return;
    throw std::runtime_error("Vulkan Error: VkResult = " + std::to_string(err));
    if (err < 0)
        abort();
}
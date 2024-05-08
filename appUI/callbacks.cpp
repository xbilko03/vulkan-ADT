/*
* Name		    : callbacks.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file for callback functions used by appWindow.cpp
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#include "callbacks.h"
/* function called when an glfw error occurs */
void glfwErrorCallback(int err, const char* text)
{
    throw std::runtime_error("GLFW - " + std::to_string(err) + " - " + text);
}
/* function called when Vulkan error occurs */
void vkErrorCallback(VkResult err)
{
    if (err == 0)
        return;
    throw std::runtime_error("Vulkan - VkResult - " + std::to_string(err));
    if (err < 0)
        abort();
}
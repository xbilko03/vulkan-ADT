/*
* Name		    : callbacks.h
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file for callback functions used by appWindow.cpp
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#pragma once

#include <vulkan/vulkan.h>
#include <stdio.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

void glfwErrorCallback(int err, const char* text);
void vkErrorCallback(VkResult err);
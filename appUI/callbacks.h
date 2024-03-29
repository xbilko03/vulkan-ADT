#pragma once

#include <vulkan/vulkan.h>
#include <stdio.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

void glfw_error_callback(int error, const char* description);
void check_vk_result(VkResult err);
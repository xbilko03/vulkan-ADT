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
/*
* Name		    : main.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Entry point for the layer generator
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/

#include "../include/layerGenerator.hpp"
#include <iostream>
#include <stdexcept>

int main() {
    vkdebug::generator::layerGenerator app{};

    try {
        app.run();
    } catch (std::exception e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

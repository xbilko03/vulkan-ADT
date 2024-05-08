/*
* Name		    : main.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Entry point for VkDebuggerApp
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#include "appUI.hpp"

#include <iostream>
#include <stdexcept>

int main() {
    details::appUI app{};

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
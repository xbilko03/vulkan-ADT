/*
* Name		    : main.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Entry point for VkDebuggerApp
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#include "../include/debuggerUI.hpp"

#include <iostream>
#include <stdexcept>

/* AI-Assisted: Claude Opus 5.5: syntax */
int main(int argc, char **argv) {
    /* AI-Assisted: end */
    vkdebug::ui::debuggerUI app{};

    /* AI-Assisted: Claude Opus 5.5: syntax */
    /* [xbilko03] save vkDebugger's start method (starting by the layer skips in-app layer configuration) */
    for (int i = 1; i < argc; i++)
        if (std::string(argv[i]) == "--started-by-layer")
            app.startedByLayer = true;
    /* AI-Assisted: end */

    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

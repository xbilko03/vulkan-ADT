/*
* Name		    : appUI.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file of the Vulkan window and image rendering VkDebuggerApp
*
* This template is available online by: Omar Cornut (ocornut)
* https://github.com/ocornut/imgui/tree/master/examples/example_glfw_vulkan
* Minor adjustments: Jozef Bilko (xbilko03), supervised by Ing. Ján Peciva Ph.D.
*/
#pragma once

#include "appWindow.hpp"
#include "events.hpp"

namespace details {
	class appUI {
	public:
		void run();
	private:

		void ShowApiCalls(details::appWindow* window, details::events *dataObject);
		void ShowBuffers(details::appWindow* window, details::events* dataObject);
		void ShowAppInfo(details::appWindow* window, details::events* dataObject);
		void ShowImages(details::appWindow* window, details::events* dataObject);
		void ShowMemories(details::appWindow* window, details::events* dataObject);
		void ShowWarnings(details::appWindow* window, details::events* dataObject);
		void ShowBreaks(details::appWindow* window, details::events* dataObject);
	};
}
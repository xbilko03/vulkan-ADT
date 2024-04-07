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
		void ShowVulkanInfo(details::appWindow* window, details::events* dataObject);
		void ShowAppInfo(details::appWindow* window, details::events* dataObject);
		void ShowCommandBuffers(details::appWindow* window, details::events* dataObject);
		void ShowImages(details::appWindow* window, details::events* dataObject);
	};
}
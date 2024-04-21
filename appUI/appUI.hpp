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
	};
}
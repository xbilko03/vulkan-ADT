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

	};
}
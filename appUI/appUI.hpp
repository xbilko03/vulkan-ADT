#pragma once

#include "appWindow.hpp"
#include "events.hpp"

namespace details {
	class appUI {
	public:
		void run();
	private:

		void ShowMenu(details::appWindow* window, details::events *dataObject);

	};
}
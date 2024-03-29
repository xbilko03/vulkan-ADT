#pragma once

#include "appWindow.hpp"
#include "layerReceiver.hpp"
#include "events.h"

#include <algorithm> //std::replace

static std::list<std::string> testList;
namespace details {
	class appUI {
	public:
		void run();
	private:

		void ShowMenu(details::appWindow* window);

		static DWORD WINAPI listenForData(__in LPVOID lpParameter);
		static void newInfo(const char* input);

		SOCKET ConnectSocket = INVALID_SOCKET;
		#define DEFAULT_BUFLEN 500
	};
}
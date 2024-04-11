/*
* Name		: events.h
* Project	: A Debugging Tool for Vulkan API
* Director  : Ing. Ján Pečiva Ph.D.
* Author	: Jozef Bilko (xbilko03)
*/
#pragma once
#include <list>
#include <string>
#include <winsock2.h>
#pragma once
#include "winsock.h"
#include "class_apiCall.hpp"
#include <map>

namespace details {
	class events {
	public:
		void connectToLayer();
		unsigned long long getFrameCount() { return frames.size(); }
		unsigned long long getFrameCallsCount(unsigned long long frameID) { return frames[frameID].size(); }
		std::list<apiCall> getFrameCalls(unsigned long long frameID) { return frames[frameID]; }

	private:
		SOCKET ConnectSocket = INVALID_SOCKET;
		#define DEFAULT_BUFLEN 500

		static DWORD WINAPI listenForData(__in LPVOID lpParameter);

		void newInfo(const char* input, size_t index);
		void parseMessage(std::string* input);

		std::map<unsigned long long, std::list<apiCall>> frames;
	};
}
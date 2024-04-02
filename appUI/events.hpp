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
#include "winsock.h"


namespace details {
	class events {
	public:
		void connectToLayer();
		std::list<std::string> getApiCalls() { return apiCalls; };

		struct vkBufferStr
		{
			std::list<std::string> parameters;
		};
		std::list<vkBufferStr> getBuffers() { return Buffers; };
	private:
		static DWORD WINAPI listenForData(__in LPVOID lpParameter);

		void newInfo(const char* input, size_t index);
		void parseMessage(std::string* input);



		SOCKET ConnectSocket = INVALID_SOCKET;
		#define DEFAULT_BUFLEN 500

		std::list<std::string> apiCalls;
		std::list<vkBufferStr> Buffers;
		vkBufferStr Buffer;
	};
}
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
#include "class_memory.hpp"
#include "class_buffer.hpp"
#include "class_image.hpp"
#include "class_apiCall.hpp"
#include <map>

namespace details {
	class events {
	public:
		void connectToLayer();
		unsigned long long getFrameCount() { return frames.size(); }
		unsigned long long getFrameCallsCount(unsigned long long frameID) { return frames[frameID].size(); }
		std::list<apiCall> getFrameCalls(unsigned long long frameID) { return frames[frameID]; }

		unsigned long long getMemoriesCount() { return memMan->GetMemoryCount(); }
		std::string getMemoryPointer(unsigned long long ID) { return memMan->GetPointer(ID); }
		std::string getMemoryState(unsigned long long ID) { return memMan->GetState(ID); }
		std::string getMemoryData(unsigned long long ID) { return memMan->GetData(ID); }

		unsigned long long getBufferCount() { return bufMan->GetBufferCount(); }
		std::string getBufferPointer(unsigned long long ID) { return bufMan->GetPointer(ID); }
		std::string getBufferState(unsigned long long ID) { return bufMan->GetState(ID); }
		std::string getBufferData(unsigned long long ID) { return bufMan->GetData(ID); }

		unsigned long long getImagesCount() { return imgMan->GetBufferCount(); }
		std::string getImagePointer(unsigned long long ID) { return imgMan->GetPointer(ID); }
		std::string getImageState(unsigned long long ID) { return imgMan->GetState(ID); }
		std::string getImageData(unsigned long long ID) { return imgMan->GetData(ID); }
		std::string getImageDataRaw(unsigned long long ID) { return imgMan->GetDataRaw(ID); }
	private:
		SOCKET ConnectSocket = INVALID_SOCKET;
		#define DEFAULT_BUFLEN 500

		static DWORD WINAPI listenForData(__in LPVOID lpParameter);

		void newInfo(const char* input, size_t index);
		void parseMessage(std::string* input);
		void createDataManagers();
		std::string omitMessage(std::string input);
		std::string omitValue(std::string input);

		std::map<unsigned long long, std::list<apiCall>> frames;
		vkMemoryManager* memMan;
		vkBufferManager* bufMan;
		vkImageManager* imgMan;
	};
}
/*
* Name		    : events.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file for data reception, message parsing and data processing
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#pragma once
#include <list>
#include <string>
#include <map>
#include <GLFW/glfw3.h>

#include "winsock.h"
#include "class_memory.hpp"
#include "class_buffer.hpp"
#include "class_image.hpp"
#include "class_system.hpp"
#include "class_apiCall.hpp"
#include "appWindow.hpp"

namespace details {
	class events {
	public:
		/* establish connection between VkDebuggerApp and VkDebuggerLayer */
		void connectToLayer(details::appWindow* windowManager);

		/* get data about program that is being debugged */
		/* about API calls */
		unsigned long long getFrameCount() { return frames.size(); }
		unsigned long long getFrameCallsCount(unsigned long long frameID) { return frames[frameID].size(); }
		std::list<apiCall*> getFrameCalls(unsigned long long frameID) { return frames[frameID]; }
		/* about VkDeviceMemory */
		unsigned long long getMemoriesCount() { return memMan->getMemoryCount(); }
		std::string getMemoryPointer(unsigned long long ID) { return memMan->getPointer(ID); }
		std::string getMemoryState(unsigned long long ID) { return memMan->getState(ID); }
		std::string getMemoryData(unsigned long long ID) { return memMan->getDataReadable(ID); }
		std::string getMemoryBound(unsigned long long ID) { return memMan->getBoundObj(ID); }
		unsigned long long getMemoryDataSize(unsigned long long ID) { return memMan->getDataSize(ID); }
		/* about VkBuffer */
		unsigned long long getBufferCount() { return bufMan->getBufferCount(); }
		std::string getBufferPointer(unsigned long long ID) { return bufMan->getPointer(ID); }
		std::string getBufferState(unsigned long long ID) { return bufMan->getState(ID); }
		std::string getBufferData(unsigned long long ID) { return bufMan->getDataReadable(ID); }
		std::string getBufferBound(unsigned long long ID) { return bufMan->getBoundObj(ID); }
		unsigned long long getBufferDataSize(unsigned long long ID) { return bufMan->getDataSize(ID); }
		/* about VkImage */
		unsigned long long getImagesCount() { return imgMan->getBufferCount(); }
		std::string getImagePointer(unsigned long long ID) { return imgMan->getPointer(ID); }
		std::string getImageState(unsigned long long ID) { return imgMan->getState(ID); }
		std::string getImageData(unsigned long long ID) { return imgMan->getDataReadable(ID); }
		std::string getImageBound(unsigned long long ID) { return imgMan->getBoundObj(ID); }
		unsigned long long getImageDataSize(unsigned long long ID) { return imgMan->getDataSize(ID); }
		/* get data about Vulkan object */
		/* about VkDeviceMemory */
		std::string getMemoryCulpritName(unsigned long long ID) { return memMan->getCulprit(ID)->getName(); }
		unsigned long long getMemoryCulpritID(unsigned long long ID) { return memMan->getCulprit(ID)->getID(); }
		/* about VkBuffer */
		std::string getBufferCulpritName(unsigned long long ID) { return bufMan->getCulprit(ID)->getName(); }
		unsigned long long getBufferCulpritID(unsigned long long ID) { return bufMan->getCulprit(ID)->getID(); }
		/* about VkImage */
		std::string getImageCulpritName(unsigned long long ID) { return imgMan->getCulprit(ID)->getName(); }
		unsigned long long getImageCulpritID(unsigned long long ID) { return imgMan->getCulprit(ID)->getID(); }

		/* get layer enabled features list */
		bool getCallsSettings() { return logCalls; }
		bool getBuffersSettings() { return logBuffers; }
		bool getImagesSettings() { return logImages; }
		bool getMemorySettings() { return logMemory; }
		bool getWarningsSettings() { return logWarnings; }
		bool getBreaksSettings() { return logBreaks; }

		/* render a new Vulkan image in ImGui */
		void loadTexture(unsigned long long ID);

		/* get data about following constructs */
		std::list<std::string>* getWarningsList() { return &warningsList; }
		std::list<std::string>* getBreaksList() { return &breaksList; }
		std::list<std::string> getAppInfo() { return sysMan->getAppInfo(); }
		std::list<std::string> getVkInfo() { return sysMan->getVkInfo(); }
	private:
		/* bools to omit unused features */
		bool logCalls;
		bool logBuffers;
		bool logImages;
		bool logMemory;
		bool logWarnings;
		bool logBreaks;

		/* socket and layer communication */
		SOCKET ConnectSocket = INVALID_SOCKET;
		#define DEFAULT_BUFLEN 500

		/* thread to catch layer data */
                #ifdef __linux__
                  static void listenForData();
                #elif __WIN32__
		  static DWORD WINAPI listenForData(__in LPVOID lpParameter);
                #endif
            
		/* receive potentially incomplete, meshed data and break them into comprehensive messages */
		void newInfo(const char* input, int index);
		/* based on the input message, manipulate the currently debugged application state */
		void parseMessage(const char* inputChar);
		/* initialize objects that take care of data */
		void createDataManagers();
		vkMemoryManager* memMan;
		vkBufferManager* bufMan;
		vkImageManager* imgMan;
		vkSystemManager* sysMan;

		/* returns the value of the input message */
		std::string omitMessage(std::string input);
		/* returns the type of the input message */
		std::string omitValue(std::string input);

		/* lists to hold additional data */
		std::list<std::string> warningsList;
		std::list<std::string> breaksList;
		std::map<unsigned long long, std::list<apiCall*>> frames;

		/* parser FSM state */
		std::string receptionState = "";
		bool dataLoad = false;
		/* data concatenation string in case the input is longer than 500 chars or if not the entire data has been accepted */
		std::string remainder;
		std::string newData;
		char* allocatedData;
		size_t dataSize = 0;
		size_t currentDataSize = 0;
		/* keep track of the current Api Call in the single command iteration */
		apiCall* currentCall;
		/* list to save all the Api Calls data */
		std::list<apiCall*> currentCallList;
		/* IDs to assign them to the corresponding objects */
		unsigned long long callID = 0;
		unsigned long long frameID = 0;
		unsigned long long memoryID = 0;
		unsigned long long bufferID = 0;
		unsigned long long imageID = 0;
		/* index 0 for source, 1 for destination used in data parsing operations */
		unsigned long long modifiedMemoryID[2];
		unsigned long long lastMemorySize;
		unsigned long long modifiedBufferID[2];
		unsigned long long modifiedImageID[2];
		/* window object for image rendering */
		details::appWindow* winMan;
		/* frame delimiter name */
		std::string delimStr = "XXX";
	};
}

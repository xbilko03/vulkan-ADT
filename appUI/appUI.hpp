/*
* Name		    : appUI.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file of the UI application
*
* Author: Jozef Bilko (xbilko03), supervised by Ing. Ján Peciva Ph.D.
*/
#pragma once

#include "appWindow.hpp"
#include "events.hpp"

namespace details {
	class appUI {
	public:
		void run();
	private:
		/* render a window that shows the collected apiCalls */
		void ShowApiCalls(details::appWindow* window, details::events *dataObject);
		/* render a window that shows the state of the VkMemory */
		void ShowMemories(details::appWindow* window, details::events* dataObject);
		/* render a window that shows the state of the VkBuffers */
		void ShowBuffers(details::appWindow* window, details::events* dataObject);
		/* render a window that shows the state of the VkImages */
		void ShowImages(details::appWindow* window, details::events* dataObject);
		/* render a window that shows details about Vulkan on the current device */
		void ShowAppInfo(details::appWindow* window, details::events* dataObject);
		/* render a window that shows history of warnings */
		void ShowWarnings(details::appWindow* window, details::events* dataObject);
		/* render a window that shows history of breakpoint occurances */
		void ShowBreaks(details::appWindow* window, details::events* dataObject);

		/* max items that can render in a single table at once */
		#define MAX_TABLE_SIZE 32
		#define IMAGE_REVIEW_SIZE 512
		#define IMAGE_ZOOM_SPEED 0.10f
		#define	MAX_IMAGE_ZOOM 256.0f

		/* call details associated with their name to allow multiple windows of that kind can stay opened at the same time */
		std::map < std::string, bool> apiDetails;
		/* textures associated with their IDs so multiple windows of that kind can stay opened at the same time  */
		std::map < unsigned long long, bool> texDetails;
		std::map < unsigned long long, bool> texLoaded;
		/* global zoom option for the textures */
		float zoom = 1.0f;
		
		/* features */
		bool apiCalls = false;
		bool buffers = false;
		bool demo = false;
		bool texture = false;
		bool appInfo = false;
		bool images = false;
		bool memories = false;
		bool warnings = false;
		bool breaks = false;
	};
}
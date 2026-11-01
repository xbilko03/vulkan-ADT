/*
* Name		    : winsock.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file to ensure the communication between VkDebuggerLayer and VkDebuggerApp
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>

/* initializes and establishes the CLIENT, try to connect to the SERVER */
int layerWinsockInit(SOCKET *connectSocket);

/* initializes and establishes the SERVER, wait for the CLIENT to connect */
int uiWinsockInit(SOCKET *connectSocket);

/* AI-Assisted: Claude Opus 5.5: syntax */
/* [xbilko03] send all bytes (send() might send just a part of data, so it repeats until all is sent) */
int winsockSendRaw(SOCKET *connectSocket, const char *data, size_t length);

/* AI-Assisted: end */

/* shuts down connection from the CLIENT side */
int layerWinsockExit(SOCKET *connectSocket);

/* shuts down connection from the SERVER side */
int uiWinsockExit(SOCKET *connectSocket);

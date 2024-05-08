/*
* Name		    : winsock.h
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file to ensure the communication between VkDebuggerLayer and VkDebuggerApp
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>

/* initializes and establishes the CLIENT, continuously try to connect to the SERVER */
int layerWinsockInit(SOCKET* ConnectSocket);

/* initializes and establishes the SERVER, wait for the CLIENT to connect */
int uiWinsockInit(SOCKET* ConnectSocket);

/* sends the 'sendbuf' array of chars from the CLIENT to the SERVER */
int winsockSendToUI(SOCKET* ConnectSocket, std::string sendbuf);

/* sends the 'sendbuf' binary data with a fixed size from the CLIENT to the SERVER */
int winsockSendToUIraw(SOCKET* ConnectSocket, const char* sendbuf, unsigned long long size);

/* shuts down connection from the CLIENT side */
int layerWinsockExit(SOCKET* ConnectSocket);

/* shuts down connection from the SERVER side */
int uiWinsockExit(SOCKET* ConnectSocket);
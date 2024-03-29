/*
* Name		: winsock.h
* Project	: A Debugging Tool for Vulkan API
* Director  : Ing. Ján Pečiva Ph.D.
* Author	: Jozef Bilko (xbilko03)
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

/* shuts down connection from the CLIENT side */
int layerWinsockExit(SOCKET* ConnectSocket);

/* shuts down connection from the SERVER side */
int uiWinsockExit(SOCKET* ConnectSocket);
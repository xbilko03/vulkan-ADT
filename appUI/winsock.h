#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

int INIT_LAYER_WINSOCK(SOCKET* ConnectSocket);
int SEND_TO_UI(SOCKET* ConnectSocket, char* sendbuf);
int EXIT_WINSOCK(SOCKET* ConnectSocket);
int INIT_UI_WINSOCK(SOCKET* ConnectSocket);
int EXIT_UI_WINSOCK(SOCKET* ConnectSocket);
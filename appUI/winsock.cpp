/*
* Name		: winsock.cpp
* Project	: A Debugging Tool for Vulkan API
* Director  : Ing. Ján Pečiva Ph.D.
* Author	: Jozef Bilko (xbilko03)
*/
#include "winsock.h"

#define DEFAULT_PORT "27015"

/* initializes and establishes the CLIENT, continuously try to connect to the SERVER */
int layerWinsockInit(SOCKET* ConnectSocket)
{
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    WSADATA wsaData;
    int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (ret != 0)
        return 1;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    /* resolve the server address and the port */
    ret = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
    if (ret != 0) {
        WSACleanup();
        return 1;
    }
    *ConnectSocket = INVALID_SOCKET;

    /* try to connect */
    ptr = result;

    /* try to connect again until the proper connection is found */
    for (ptr = result; ptr != NULL;ptr = ptr->ai_next) {
        *ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (*ConnectSocket == INVALID_SOCKET) {
            WSACleanup();
            return 1;
        }

        /* connect */
        ret = connect(*ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (ret == SOCKET_ERROR) {
            closesocket(*ConnectSocket);
            *ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);

    /* cannot connect */
    if (*ConnectSocket == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    /* connection established successfully*/
    return 0;
}
/* initializes and establishes the SERVER, wait for the CLIENT to connect */
int uiWinsockInit(SOCKET* ConnectSocket)
{
    SOCKET ListenSocket = INVALID_SOCKET;
    WSADATA wsaData;
    int ret;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    /* init winsock */
    ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (ret != 0)
        return 1;


    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    /* resolve the server address and the port */
    ret = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (ret != 0) {
        WSACleanup();
        return 1;
    }

    /* create new socket */
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    /* socket setup [TCP] / bind socket */
    ret = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (ret == SOCKET_ERROR) {
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(result);

    ret = listen(ListenSocket, SOMAXCONN);
    if (ret == SOCKET_ERROR) {
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    /* accept client socket */
    *ConnectSocket = accept(ListenSocket, NULL, NULL);
    if (*ConnectSocket == INVALID_SOCKET) {
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    closesocket(ListenSocket);
    return 0;
}

/* sends the 'sendbuf' array of chars from the CLIENT to the SERVER */
int winsockSendToUI(SOCKET* ConnectSocket, std::string sendbuf)
{
    sendbuf += '!';
    /* Send 'sendbuf' data */
    int ret = send(*ConnectSocket, sendbuf.c_str(), strlen(sendbuf.c_str()), 0);
    if (ret == SOCKET_ERROR) {
        closesocket(*ConnectSocket);
        WSACleanup();
        return 1;
    }
    return 0;
}

/* shuts down connection from the CLIENT side */
int layerWinsockExit(SOCKET* ConnectSocket)
{
    /* shut down the connection */
    int ret = shutdown(*ConnectSocket, SD_SEND);
    if (ret == SOCKET_ERROR) {
        closesocket(*ConnectSocket);
        WSACleanup();
        exit(1);
    }
    closesocket(*ConnectSocket);
    WSACleanup();
}
/* shuts down connection from the SERVER side */
int uiWinsockExit(SOCKET* ConnectSocket)
{
    /* shut down the connection */
    int ret = shutdown(*ConnectSocket, SD_SEND);
    if (ret == SOCKET_ERROR) {
        closesocket(*ConnectSocket);
        WSACleanup();
        return 1;
    }
    closesocket(*ConnectSocket);
    WSACleanup();
    return 0;
}

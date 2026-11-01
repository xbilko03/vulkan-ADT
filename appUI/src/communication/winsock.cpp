/*
* Name		    : winsock.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file to ensure the communication between VkDebuggerLayer and VkDebuggerApp
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#include "../../include/communication/winsock.hpp"

#define DEFAULT_PORT "27015"

int layerWinsockInit(SOCKET *connectSocket) {
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    WSADATA data;
    int ret = WSAStartup(MAKEWORD(2, 2), &data);
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
    *connectSocket = INVALID_SOCKET;

    /* [xbilko03] try to connect */
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        *connectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                                ptr->ai_protocol);
        if (*connectSocket == INVALID_SOCKET) {
            /* AI-Assisted: Claude Opus 5.5: syntax */
            freeaddrinfo(result);
            /* AI-Assisted: end */
            WSACleanup();
            return 1;
        }

        /* connect */
        ret = connect(*connectSocket, ptr->ai_addr, (int) ptr->ai_addrlen);
        if (ret == SOCKET_ERROR) {
            closesocket(*connectSocket);
            *connectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);

    /* cannot connect */
    if (*connectSocket == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    /* connection established successfully*/
    return 0;
}

int uiWinsockInit(SOCKET *connectSocket) {
    SOCKET listenSocket = INVALID_SOCKET;
    WSADATA data;
    int ret;
    struct addrinfo *result = NULL;
    struct addrinfo hints;

    /* init winsock */
    ret = WSAStartup(MAKEWORD(2, 2), &data);
    if (ret != 0)
        return 1;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    /* AI-Assisted: Claude Opus 5.5: syntax */
    /* [xbilko03] UI listens to localhost, functionality may grant remote communication in the future */
    /* [xbilko03] resolve the server address and the port */
    ret = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
    /* AI-Assisted: end */
    if (ret != 0) {
        WSACleanup();
        return 1;
    }

    /* create new socket */
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    /* socket setup [TCP] / bind socket */
    ret = bind(listenSocket, result->ai_addr, (int) result->ai_addrlen);
    if (ret == SOCKET_ERROR) {
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(result);

    ret = listen(listenSocket, SOMAXCONN);
    if (ret == SOCKET_ERROR) {
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    /* accept client socket */
    *connectSocket = accept(listenSocket, NULL, NULL);
    if (*connectSocket == INVALID_SOCKET) {
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    closesocket(listenSocket);
    return 0;
}

/* AI-Assisted: Claude Sonnet 5: syntax */
static bool sendAll(SOCKET *sock, const char *data, size_t len) {
    size_t totalSent = 0;
    while (totalSent < len) {
        int sent = send(*sock, data + totalSent, (int) (len - totalSent), 0);
        if (sent == SOCKET_ERROR)
            return false;
        totalSent += (size_t) sent;
    }
    return true;
}

/* AI-Assisted: end */
/* AI-Assisted: Claude Opus 5.5: syntax */
int winsockSendRaw(SOCKET *connectSocket, const char *data, size_t length) {
    if (!sendAll(connectSocket, data, length)) {
        closesocket(*connectSocket);
        WSACleanup();
        return 1;
    }
    return 0;
}

/* AI-Assisted: end */
int layerWinsockExit(SOCKET *connectSocket) {
    /* shut down the connection */
    /* AI-Assisted: Claude Opus 5.5: syntax */
    int ret = shutdown(*connectSocket, SD_SEND);
    if (ret == SOCKET_ERROR) {
        closesocket(*connectSocket);
        WSACleanup();
        return 1;
    }
    closesocket(*connectSocket);
    WSACleanup();
    return 0;
    /* AI-Assisted: end */
}

int uiWinsockExit(SOCKET *connectSocket) {
    /* shut down the connection */
    int ret = shutdown(*connectSocket, SD_SEND);
    if (ret == SOCKET_ERROR) {
        closesocket(*connectSocket);
        WSACleanup();
        return 1;
    }
    closesocket(*connectSocket);
    WSACleanup();
    return 0;
}

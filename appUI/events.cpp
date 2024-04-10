/*
* Name		: events.cpp
* Project	: A Debugging Tool for Vulkan API
* Director  : Ing. Ján Pečiva Ph.D.
* Author	: Jozef Bilko (xbilko03)
*/
#include "events.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <mutex>

namespace details {

    std::string receptionState = "";
    std::string remainder = "";
    std::string currentMemoryPtr = "";

    apiCall* currentCall;
    std::list<apiCall> currentCallList;
    unsigned callID = 0;
    
    void events::parseMessage(std::string *input)
    {
        /* begin of an api call */
        if ((*input).substr(0, 6) == "begin_")
        {
            currentCall = new apiCall(callID);
            /* api command begin message */
            (*currentCall).assignName(*input);
            //std::cout << "new call = " << currentCall->getName() << std::endl;
            //std::cout << "new call = " << apiCallObjectList.size() << std::endl;
            /* refresh window */
            //glfwPostEmptyEvent();
            receptionState = *input;
        }
        /* end of an api call */
        else if ((*input).substr(0, 4) == "end_")
        {
            receptionState = *input;
            /* finish reading and save call */
            currentCallList.push_back(*currentCall);
            frames[callID++] = currentCallList;
        }
        /* other data in between api calls */
        else if ((*input).substr(0, 5) == "data_")
        {
            
        }
        else
        {
            (*currentCall).assignParameter(*input);
        }
    }
    
    unsigned long dataSize = 0;
    unsigned long currentDataSize = 0;
    std::string newData;
    void events::newInfo(const char* input, size_t index)
    {
        std::string s = input;
        s = s.substr(0,index);

        if (currentDataSize < dataSize)
        {
            dataHandler:
            /* catch data */
            currentDataSize += s.size();
            if (currentDataSize >= dataSize)
            {
                unsigned long remainderIndex = currentDataSize - dataSize;
                remainderIndex = s.size() - remainderIndex;
                remainder = s.substr(remainderIndex, s.size());
                newData += s.substr(0, remainderIndex);
                newData = "data_" + newData;
                parseMessage(&newData);
                currentDataSize = 0;
                dataSize = 0;
            }
            else
            {
                newData += s;
            }
            return;
        }
        s = remainder + s;

        std::string token;
        size_t pos = 0;

        while ((pos = s.find("!")) != std::string::npos) {
            token = s.substr(0, pos);
            s.erase(0, pos + 1);
            if (token.substr(0, 4) == "data")
            {
                /* Get size */
                auto dataLen = token.substr(4, pos);
                dataSize = stoul(dataLen, nullptr, 10);
                currentDataSize = 0;
                newData = "";
                goto dataHandler;
            }
            else
            {
                parseMessage(&token);
            }
        }
        if (s.size() > 0)
            remainder = s;
        else
            remainder = "";
    }

    /* Receiver thread */
    DWORD WINAPI events::listenForData(__in LPVOID lpParameter)
    {
        /* Create socket */
        int ret;
        SOCKET ClientSocket = INVALID_SOCKET;
        char recvbuf[DEFAULT_BUFLEN];
        uiWinsockInit(&ClientSocket);

        // Receive until the peer shuts down the connection
        details::events* myObj = (details::events*)lpParameter;
        do {
            //std::fill(recvbuf, recvbuf + DEFAULT_BUFLEN - 1, 0);
            ret = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
            if (ret > 0)
            {
                /* Parse data */
                (*myObj).newInfo(recvbuf, ret);
            }
            else if (ret == 0)
            {
                /* closing connection */
            }
            else
            {
                /* recv failed */
                closesocket(ClientSocket);
                WSACleanup();
                //throw runtime error
            }
        } while (ret > 0);

        /* Destroy socket */
        uiWinsockExit(&ClientSocket);
        return 0;
    }
    void events::connectToLayer()
    {
        DWORD mythreadId;
        /* Collect data from socket continuously */
        CreateThread(0, 0, events::listenForData, this, 0, &mythreadId);
    }
}
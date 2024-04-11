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
    unsigned long long callID = 0;
    unsigned long long frameID = 0;
    unsigned long long memoryID = 0;
    unsigned long long modifiedMemoryID;
    
    void events::createDataManagers()
    {
        memMan = new vkMemoryManager();
    }

    void events::parseMessage(std::string *input)
    {
        /* refresh window */
        //glfwPostEmptyEvent();

        //std::cout << "new dataIN." << *input << std::endl;
        /* begin of an api call */
        if ((*input).substr(0, 6) == "begin_")
        {
            receptionState = *input;
            currentCall = new apiCall(callID++);
            /* api command begin message */
            (*currentCall).assignName(*input);
            if (receptionState == "begin_vkAcquireNextImageKHR")
            {
                frameID++;
                currentCallList = {};
            }
            else if (receptionState == "begin_vkAllocateMemory")
            {
                memMan->newMemory(memoryID);
            }
        }
        /* end of an api call */
        else if ((*input).substr(0, 4) == "end_")
        {
            receptionState = *input;
            /* finish reading and save call */
            currentCallList.push_back(*currentCall);
            frames[frameID] = currentCallList;
        if (receptionState == "end_vkAllocateMemory")
        {
            memoryID++;
        }
        }
        /* other data in between api calls */
        else if ((*input).substr(0, 5) == "data_")
        {
            if (receptionState == "begin_vkUnmapMemory")
            {
                std::cout << "new data at " << modifiedMemoryID << std::endl;
                memMan->AssignData((*input), modifiedMemoryID);
            }
            //std::cout << "new data." << std::endl;
        }
        else if ((*input).substr(0, 6) == "layer_")
        {
            if (receptionState == "begin_vkUnmapMemory")
            {
                /* get ID by pointer */
                modifiedMemoryID = memMan->GetFromPointerID(*input);
            }
            else if (receptionState == "begin_vkAllocateMemory")
            {
                memMan->AssignPointer((*input), memoryID);
            }
            else if (receptionState == "begin_vkFreeMemory")
            {
                memMan->FreeMemory(memMan->GetFromPointerID(*input));
            }
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
        (*myObj).createDataManagers();
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
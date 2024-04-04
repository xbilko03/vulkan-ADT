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

    void events::parseMessage(std::string *input)
    {
        if ((*input).size() < 6)
            return;
        
        if ((*input).substr(0, 6) == "begin_")
        {
            /* api command begin message */
            apiCalls.push_back((*input).substr(6, (*input).size()));
            /* refresh window */
            //glfwPostEmptyEvent();
            if ((*input) == "begin_vkCreateBuffer")
            {   
                receptionState = "begin_vkCreateBuffer";
            }
        }
        else if ((*input).substr(0, 4) == "end_")
        {
            if ((*input) == "end_vkCreateBuffer")
            {
                receptionState = "";
                Buffers.push_back(Buffer);
                /* reset buffer struct */
                Buffer = {};
            }
            /* api command end message */
        }
        else if ((*input).substr(0, 5) == "data_")
        {
            /* api command data message */
        }
        else
        {
            if (receptionState == "begin_vkCreateBuffer")
            {
                Buffer.parameters.push_back(*input);
            }
            else
            {
                std::cout << "different message = " << *input << std::endl;
            }
        }
    }

    std::string remainder = "";

    void events::newInfo(const char* input, size_t index)
    {
        /*
        * received data example
        * apicall1()!apicall2()!\0XXXXXXXXXXXX
        * 
        * apicall3()!apical\0XXXXXXXXXXXX
        * l4()!\0XXXXXXXXXXXX
        */
        std::string s = input;
        s = s.substr(0,index);
        s = remainder + s;

        std::string token;
        size_t pos = 0;
        while ((pos = s.find("!")) != std::string::npos) {
            token = s.substr(0, pos);
            parseMessage(&token);
            s.erase(0, pos + 1);
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
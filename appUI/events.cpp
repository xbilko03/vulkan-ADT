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

    /* FSM state */
    std::string receptionState = "";
    /* Data concatenation string in case the input is longer than 500 chars or if not the entire data has been accepted */
    std::string remainder = "";
    std::string newData;
    char* allocatedData;
    unsigned long dataSize = 0;
    unsigned long currentDataSize = 0;
    /* Keep track of the current Api Call in the single command iteration */
    apiCall* currentCall;
    /* List to save all the Api Calls data */
    std::list<apiCall*> currentCallList;
    /* IDs to assign them to the corresponding objects */
    unsigned long long callID = 0;
    unsigned long long frameID = 0;
    unsigned long long memoryID = 0;
    unsigned long long bufferID = 0;
    unsigned long long imageID = 0;
    /* Memory ID in case we need to backtrack it after the memory allocation */
    unsigned long long modifiedMemoryID;
    unsigned long long lastMemorySize;
    unsigned long long modifiedBufferID;
    unsigned long long modifiedImageID;
    /* Window for image rendering */
    details::appWindow* winMan;

    /* Init objects that hold the data */
    void events::createDataManagers()
    {
        memMan = new vkMemoryManager();
        bufMan = new vkBufferManager();
        imgMan = new vkImageManager();
        sysMan = new vkSystemManager(winMan);
    }
    /* Remove the message prefix before = */
    std::string events::omitMessage(std::string input)
    {
        auto pos = input.find("=");
        if (pos == std::string::npos)
            return "NULL";

        return input.substr(pos + 1, input.size());
    }
    std::string events::omitValue(std::string input)
    {
        auto pos = input.find("=");
        if (pos == std::string::npos)
            return "NULL";

        return input.substr(0, pos);
    }
    /* Decipher received input message */
    void events::parseMessage(const char* inputChar)
    {
        std::string input = inputChar;
        /* refresh window */
        //glfwPostEmptyEvent();
        /* handle data input */
        if (receptionState == "data_loadMemory")
        {
            memMan->AssignData(modifiedMemoryID, (char*)inputChar, lastMemorySize);
            modifiedBufferID = bufMan->GetFromPointerID(memMan->GetBoundObj(modifiedMemoryID));
            modifiedImageID = imgMan->GetFromPointerID(memMan->GetBoundObj(modifiedMemoryID));

            if (modifiedBufferID != -1)
            {
                bufMan->AssignMemory(modifiedBufferID, memMan->GetMemory(modifiedMemoryID));
            }
            if (modifiedImageID != -1)
            {
                imgMan->AssignMemory(modifiedBufferID, memMan->GetMemory(modifiedMemoryID));
                winMan->LoadImageTexture(modifiedImageID,1024,1024,4,(char*)inputChar);
            }
            receptionState = "begin_vkUnmapMemory";
            return;
        }

        /* begin of an api call */
        if (input.substr(0, 6) == "begin_")
        {
            receptionState = input;
            currentCall = new apiCall(callID++);
            /* api command begin message */
            currentCall->assignName(input);
            if (receptionState == "begin_vkAcquireNextImageKHR")
            {
                frameID++;
                currentCallList = {};
            }
            else if (receptionState == "begin_vkAllocateMemory")
            {
                memMan->newMemory(memoryID);
            }
            else if (receptionState == "begin_vkCreateImage")
            {
                imgMan->newBuffer(imageID);
            }
            else if (receptionState == "begin_vkCreateBuffer")
            {
                bufMan->newBuffer(bufferID);
            }
        }
        /* end of an api call */
        else if (input.substr(0, 4) == "end_")
        {
            receptionState = input;
            /* finish reading and save call */
            currentCallList.push_back(currentCall);
            frames[frameID] = currentCallList;

            if (receptionState == "end_vkAllocateMemory")
            {
                memMan->AssignCulprit(memoryID,currentCall);
                memoryID++;
            }
            else if (receptionState == "end_vkCreateImage")
            {
                imgMan->AssignCulprit(imageID, currentCall);
                imageID++;
            }
            else if (receptionState == "end_vkCreateBuffer")
            {
                bufMan->AssignCulprit(bufferID, currentCall);
                bufferID++;
            }
        }
        else if (input.substr(0, 6) == "layer_")
        {
            /* instructions sent by layer.cpp */

            if (input.substr(0, 14) == "layer_warning_")
            {
                warningsList.push_back(input);
            }

            if (receptionState == "begin_vkUnmapMemory")
            {
                if (input.substr(0, 11) == "layer_data=")
                {
                    receptionState = "data_loadMemory";
                }
                else if (omitValue(input) == "layer_ptr")
                {
                    modifiedMemoryID = memMan->GetFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_size")
                {
                    lastMemorySize = stoul(omitMessage(input));
                }
            }
            else if (receptionState == "begin_vkAllocateMemory")
            {
                memMan->AssignPointer(memoryID, omitMessage(input));
            }
            else if (receptionState == "begin_vkFreeMemory")
            {
                memMan->FreeMemory(memMan->GetFromPointerID(omitMessage(input)));
            }
            else if (receptionState == "begin_vkCreateImage")
            {
                if (omitValue(input) == "layer_ptr")
                {
                    imgMan->AssignPointer(imageID, omitMessage(input));
                }
                else if (omitValue(input) == "layer_width")
                {
                    imgMan->SetWidth(imageID, omitMessage(input));
                }
                else if (omitValue(input) == "layer_height")
                {
                    imgMan->SetHeight(imageID, omitMessage(input));
                }
            }
            else if (receptionState == "begin_vkDestroyImage")
            {
                modifiedImageID = imgMan->GetFromPointerID(omitMessage(input));
                if (modifiedImageID != -1)
                {
                    imgMan->FreeBuffer(modifiedImageID);
                }
            }
            else if (receptionState == "begin_vkCreateBuffer")
            {
                bufMan->AssignPointer(bufferID, omitMessage(input));
            }
            else if (receptionState == "begin_vkDestroyBuffer")
            {
                modifiedBufferID = bufMan->GetFromPointerID(omitMessage(input));
                if (modifiedBufferID != -1)
                    bufMan->FreeBuffer(modifiedBufferID);
            }
            else if (receptionState == "begin_vkBindBufferMemory")
            {
                if (omitValue(input) == "layer_memPtr")
                {
                    modifiedMemoryID = memMan->GetFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_bufPtr")
                {
                    modifiedBufferID = bufMan->GetFromPointerID(omitMessage(input));
                    if (modifiedBufferID != -1)
                        bufMan->AssignBoundObj(modifiedBufferID, memMan->GetPointer(modifiedMemoryID));
                    if (modifiedMemoryID != -1)
                        memMan->AssignBoundObj(modifiedMemoryID, omitMessage(input));
                    if (modifiedBufferID != -1)
                    {
                        bufMan->AssignMemory(modifiedBufferID, memMan->GetMemory(modifiedMemoryID));
                    }
                }
            }
            else if (receptionState == "begin_vkBindImageMemory")
            {
                if (omitValue(input) == "layer_memPtr")
                {
                    modifiedMemoryID = memMan->GetFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_imgPtr")
                {
                    modifiedImageID = imgMan->GetFromPointerID(omitMessage(input));
                    if (modifiedImageID != -1)
                        imgMan->AssignBoundObj(modifiedImageID, memMan->GetPointer(modifiedMemoryID));
                    if (modifiedMemoryID != -1)
                        memMan->AssignBoundObj(modifiedMemoryID, omitMessage(input));
                    if (modifiedImageID != -1)
                    {
                        imgMan->AssignMemory(modifiedImageID, memMan->GetMemory(modifiedMemoryID));
                        winMan->LoadImageTexture(modifiedImageID,1024, 1024, 4, memMan->GetData(modifiedMemoryID));
                    }
                }
            }
            else if (receptionState == "begin_vkCmdCopyImageToBuffer")
            {
                if (omitValue(input) == "layer_dstBuf")
                {
                    if (modifiedImageID != -1)
                    {
                        modifiedBufferID =  bufMan->GetFromPointerID(omitMessage(input));
                        bufMan->AssignMemory(modifiedBufferID, imgMan->GetMemory(modifiedImageID));
                    }
                }
                else if (omitValue(input) == "layer_srcImg")
                {
                    modifiedImageID = imgMan->GetFromPointerID(omitMessage(input));
                }
            }
            else if (receptionState == "begin_vkCmdCopyBufferToImage")
            {
                if (omitValue(input) == "layer_srcBuf")
                {
                    modifiedBufferID = bufMan->GetFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_dstImg")
                {
                    modifiedImageID = imgMan->GetFromPointerID(omitMessage(input));
                    if (modifiedImageID != -1)
                    {
                        imgMan->AssignMemory(modifiedImageID, imgMan->GetMemory(modifiedBufferID));
                        winMan->LoadImageTexture(modifiedImageID,1024, 1024, 4, bufMan->GetData(modifiedBufferID));
                    }
                }
            }
            else if (receptionState == "begin_vkCmdCopyImage")
            {

                if (omitValue(input) == "layer_srcImg")
                {
                    modifiedImageID = imgMan->GetFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_dstImg")
                {
                    modifiedMemoryID = imgMan->GetFromPointerID(omitMessage(input));
                    if (modifiedMemoryID != -1)
                    {
                        imgMan->AssignMemory(modifiedMemoryID, imgMan->GetMemory(modifiedImageID));
                    }
                }
            }
            else if (receptionState == "begin_vkCmdCopyBuffer")
            {
                if (omitValue(input) == "layer_srcBuf")
                {
                    modifiedBufferID = bufMan->GetFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_dstBuf")
                {
                    modifiedMemoryID = bufMan->GetFromPointerID(omitMessage(input));
                    if (modifiedMemoryID != -1)
                    {
                        bufMan->AssignMemory(modifiedMemoryID, imgMan->GetMemory(modifiedBufferID));
                    }
                }
            }
            else if (receptionState == "begin_vkCreateInstance")
            {
                if (omitValue(input) == "layer_proccessID")
                {
                    sysMan->newAppInfo(input);
                }
                else if (omitValue(input) == "layer_appName")
                {
                    sysMan->newAppInfo(input);
                }
            }
        }
        else
        {
            if (omitValue(input) == "result")
                currentCall->assignRetVal(omitMessage(input));
            currentCall->assignParameter(input);
        }
    }

    /* Receive new message from the thread, send it to be parsed after it has been constructed successfully */
    void events::newInfo(const char* input, size_t index)
    {
        std::string s = input;
        s = s.substr(0, index);

        if (currentDataSize < dataSize)
        {
            /* catch data */
            currentDataSize += index;
            if (currentDataSize >= dataSize)
            {
                unsigned long remainderIndex = currentDataSize - dataSize;
                remainderIndex = index - remainderIndex;
                /*
                * in some cases, input may look like this
                * data112!XXXXXcommand1!command2!
                * therefore you need to catch just the XXXXX data
                * we do this by copying data (XXXXX) from the memory to allocatedData
                * and then copying command1!command2! to the remainder string,
                * which will be handled in the next call of the function
                */
                memcpy(allocatedData + currentDataSize - index, input, remainderIndex);
                char* temp = (char*)malloc(currentDataSize - dataSize);
                memcpy(temp, input + remainderIndex, currentDataSize - dataSize);

                remainder = temp;
                remainder = remainder.substr(0, currentDataSize - dataSize);

                std::string invokeData = "layer_data=" + std::to_string(dataSize);
                parseMessage(invokeData.c_str());
                parseMessage(allocatedData);
                currentDataSize = 0;
                dataSize = 0;
            }
            else
            {
                memcpy(allocatedData + currentDataSize - index, input, index);
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
                allocatedData = (char*)malloc(dataSize);
                std::string posString = input;
                size_t dataIndex = posString.find(token);
                newInfo(input + dataIndex + token.size() + 1, index - dataIndex - token.size() - 1);
                return;
            }
            else
            {
                parseMessage(token.c_str());
            }
        }
        if (s.size() > 0)
            remainder = s;
        else
            remainder = "";
    }

    /* Message receiver thread, reads socket, then sends the partial data to newInfo function to be concatenated into comprehensive messages */
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

    /* Creates new thread to collect data on the layer so that the user may interact with UI without delay */
    void events::connectToLayer(details::appWindow* windowManager)
    {
        winMan = windowManager;
        DWORD mythreadId;
        /* Collect data from socket continuously */
        CreateThread(0, 0, events::listenForData, this, 0, &mythreadId);
    }
}
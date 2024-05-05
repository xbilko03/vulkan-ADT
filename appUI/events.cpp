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
    std::string remainder;
    std::string newData;
    char* allocatedData;
    size_t dataSize = 0;
    size_t currentDataSize = 0;
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
    /* index 0 for source, 1 for destination */
    unsigned long long modifiedMemoryID[2];
    unsigned long long lastMemorySize;
    unsigned long long modifiedBufferID[2];
    unsigned long long modifiedImageID[2];
    /* Window for image rendering */
    details::appWindow* winMan;
    bool dataLoad = false;


    /* Init objects that hold the data */
    void events::createDataManagers()
    {
        memMan = new vkMemoryManager();
        bufMan = new vkBufferManager();
        imgMan = new vkImageManager();
        sysMan = new vkSystemManager(winMan);
    }
    void events::loadTexture(unsigned long long ID )
    { 
        if(imgMan->GetData(ID) != NULL)
            winMan->LoadImageTexture(ID, imgMan->GetWidth(ID), imgMan->GetHeight(ID), 4, imgMan->GetMemory(ID)->memoryData); 
    }

    /* Remove the message prefix before = */
    std::string delimStr = "XXX";
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
        if (dataLoad)
        {
            /* if destination memory already has data, free them and attempt to replace them with the new data */
            
            if (memMan->GetData(modifiedMemoryID[0]) != NULL)
                free(memMan->GetData(modifiedMemoryID[0]));

            memMan->AssignData(modifiedMemoryID[0], (char*)inputChar, lastMemorySize);
            modifiedBufferID[1] = bufMan->GetFromPointerID(memMan->GetBoundObj(modifiedMemoryID[0]));
            modifiedImageID[1] = imgMan->GetFromPointerID(memMan->GetBoundObj(modifiedMemoryID[0]));

            dataLoad = false;
            return;
        }

        /* beginning of an api call */
        if (input.substr(0, 6) == "begin_")
        {
            receptionState = input;
            if (logCalls)
            {
                currentCall = new apiCall(callID++);
                currentCall->assignName(input);
            }
            if (receptionState == "begin_" + delimStr)
            {
                frameID++;
                if(logCalls)
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
            if (logCalls)
            {
                currentCallList.push_back(currentCall);
                frames[frameID] = currentCallList;
            }

            if (receptionState == "end_vkAllocateMemory")
            {
                if(logCalls)
                    memMan->AssignCulprit(memoryID,currentCall);
                memoryID++;
            }
            else if (receptionState == "end_vkCreateImage")
            {
                if(logCalls)
                    imgMan->AssignCulprit(imageID, currentCall);
                imageID++;
            }
            else if (receptionState == "end_vkCreateBuffer")
            {
                if(logCalls)
                    bufMan->AssignCulprit(bufferID, currentCall);
                bufferID++;
            }
        }
        else if (input.substr(0, 6) == "layer_")
        {
            /* instructions sent by layer.cpp */

            if (omitValue(input) == "layer_callAtBreakTrigger")
            {
                breaksList.push_back("Breakpoint: triggered at Xth API call - call[" + omitMessage(input) + "]");
            }

            else if (omitValue(input) == "layer_callEveryBreakTrigger")
            {
                breaksList.push_back("Breakpoint: triggered every Xth API call - call[" + omitMessage(input) + "]");
            }

            else if (omitValue(input) == "layer_imageNewBreak")
            {
                breaksList.push_back("Breakpoint: triggered at new VkImage creation - call[" + std::to_string(callID) + "]");
            }

            else if (omitValue(input) == "layer_bufferNewBreak")
            {
                breaksList.push_back("Breakpoint: triggered at new VkBuffer creation - call[" + std::to_string(callID) + "]");
            }

            else if (omitValue(input) == "layer_memoryNewBreak")
            {
                breaksList.push_back("Breakpoint: triggered at new VkMemory allocation - call[" + std::to_string(callID) + "]");
            }

            else if (omitValue(input) == "layer_fpsBelowOptionBreak")
            {
                breaksList.push_back("Breakpoint: triggered when below X fps - FPS[" + omitMessage(input) + "] - call[" + std::to_string(callID) + "]");
            }

            else if (omitValue(input) == "layer_frameAtBreakTrigger")
            {
                breaksList.push_back("Breakpoint: triggered when at frame - frame[" + std::to_string(frameID) + "] - call[" + std::to_string(callID) + "]");
            }

            else if (omitValue(input) == "layer_frameEveryBreakTrigger")
            {
                breaksList.push_back("Breakpoint: triggered every Xth frame - frame[" + std::to_string(frameID) + "] - call[" + std::to_string(callID) + "]");
            }

            else if (omitValue(input) == "layer_fpsEveryTrigger")
            {
                warningsList.push_back("Warning: FPS report every Xth frame - FPS[" + omitMessage(input) + "] - frame[" + std::to_string(frameID) + "]");
            }

            else if (omitValue(input) == "layer_fpsBelowTrigger")
            {
                warningsList.push_back("Warning: FPS report below FPS - FPS[" + omitMessage(input) + "] - frame[" + std::to_string(frameID) + "]");
            }

            else if (omitValue(input) == "layer_log_calls")
            {
                if (omitMessage(input) == "false")
                    logCalls = false;
                else if (omitMessage(input) == "true")
                    logCalls = true;
            }

            else if (omitValue(input) == "layer_log_buffers")
            {
                if (omitMessage(input) == "false")
                    logBuffers = false;
                else if (omitMessage(input) == "true")
                    logBuffers = true;
            }

            else if (omitValue(input) == "layer_log_images")
            {
                if (omitMessage(input) == "false")
                    logImages = false;
                else if (omitMessage(input) == "true")
                    logImages = true;
            }

            else if (omitValue(input) == "layer_log_memory")
            {
                if (omitMessage(input) == "false")
                    logMemory = false;
                else if (omitMessage(input) == "true")
                    logMemory = true;
            }

            else if (omitValue(input) == "layer_warnings")
            {
                if (omitMessage(input) == "false")
                    logWarnings = false;
                else if (omitMessage(input) == "true")
                    logWarnings = true;
            }

            else if (omitValue(input) == "layer_breaks")
            {
                if (omitMessage(input) == "false")
                    logBreaks = false;
                else if (omitMessage(input) == "true")
                    logBreaks = true;
            }

            else if (omitValue(input) == "layer_delim")
            {
                if (omitMessage(input) == "vkAcquireNextImageKHR")
                    delimStr = "vkAcquireNextImageKHR";
                else if (omitMessage(input) == "vkQueueSubmit")
                    delimStr = "vkQueueSubmit";
                else if (omitMessage(input) == "vkCmdDraw")
                    delimStr = "vkCmdDraw";
            }

            if (input.substr(0, 14) == "layer_warning_")
            {
                warningsList.push_back(input);
            }

            if (receptionState == "begin_vkUnmapMemory")
            {
                if (input.substr(0, 11) == "layer_data=")
                {
                    dataLoad = true;
                }
                else if (omitValue(input) == "layer_ptr")
                {
                    modifiedMemoryID[0] = memMan->GetFromPointerID(omitMessage(input));
                    memMan->SetState(modifiedMemoryID[0], "unmapped");
                }
                else if (omitValue(input) == "layer_size")
                {
                    lastMemorySize = stoul(omitMessage(input));
                }
            }
            else if (receptionState == "begin_vkMapMemory")
            {
                if (omitValue(input) == "layer_ptr")
                {
                    modifiedMemoryID[1] = memMan->GetFromPointerID(omitMessage(input));
                    memMan->SetState(modifiedMemoryID[1], "mapped");
                }
            }
            else if (receptionState == "begin_vkQueueSubmit")
            {
                if (input.substr(0, 11) == "layer_data=")
                {
                    dataLoad = true;
                }
                else if (omitValue(input) == "layer_ptr")
                {
                    modifiedMemoryID[0] = memMan->GetFromPointerID(omitMessage(input));
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
                memMan->SetState(memMan->GetFromPointerID(omitMessage(input)), "freed");
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
                modifiedImageID[1] = imgMan->GetFromPointerID(omitMessage(input));
                if (modifiedImageID[1] != -1)
                    imgMan->SetState(modifiedImageID[1], "freed");
            }
            else if (receptionState == "begin_vkCreateBuffer")
            {
                bufMan->AssignPointer(bufferID, omitMessage(input));
            }
            else if (receptionState == "begin_vkDestroyBuffer")
            {
                modifiedBufferID[1] = bufMan->GetFromPointerID(omitMessage(input));
                if (modifiedBufferID[1] != -1)
                    bufMan->SetState(modifiedBufferID[1], "freed");
            }
            else if (receptionState == "begin_vkBindBufferMemory")
            {
                if (omitValue(input) == "layer_memPtr")
                {
                    /* get source memory ID */
                    modifiedMemoryID[0] = memMan->GetFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_bufPtr")
                {
                    /* if source memory exists, attempt to bind it to the destination buffer */
                    if (modifiedMemoryID[0] != -1)
                    {
                        /* get destination buffer ID */
                        modifiedBufferID[1] = bufMan->GetFromPointerID(omitMessage(input));

                        /* bind memoryPtr to bufferPtr */
                        bufMan->AssignBoundObj(modifiedBufferID[1], memMan->GetPointer(modifiedMemoryID[0]));
                        bufMan->SetState(modifiedBufferID[1], "bound");

                        /* bind bufferPtr to memoryPtr */
                        memMan->AssignBoundObj(modifiedMemoryID[0], omitMessage(input));
                        memMan->SetState(modifiedMemoryID[0], "bound");

                        /* assign memoryObj to bufferObj */
                        bufMan->AssignMemory(modifiedBufferID[1], memMan->GetMemory(modifiedMemoryID[0]));
                    }
                }
            }
            else if (receptionState == "begin_vkBindImageMemory")
            {
                if (omitValue(input) == "layer_memPtr")
                {
                    /* get source memory ID */
                    modifiedMemoryID[0] = memMan->GetFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_imgPtr")
                {
                    /* if source memory exists, attempt to bind it to the destination image */
                    if (modifiedMemoryID[0] != -1)
                    {
                        /* get destination image ID */
                        modifiedImageID[1] = imgMan->GetFromPointerID(omitMessage(input));

                        /* bind memoryPtr to imagePtr */
                        imgMan->AssignBoundObj(modifiedImageID[1], memMan->GetPointer(modifiedMemoryID[0]));
                        imgMan->SetState(modifiedImageID[1], "bound");

                        /* bind imagePtr to memoryPtr */
                        memMan->AssignBoundObj(modifiedMemoryID[0], omitMessage(input));
                        memMan->SetState(modifiedMemoryID[0], "bound");

                        /* assign memoryObj to imageObj */
                        imgMan->AssignMemory(modifiedImageID[1], memMan->GetMemory(modifiedMemoryID[0]));
                        winMan->LoadImageTexture(modifiedImageID[1], imgMan->GetWidth(modifiedImageID[1]), imgMan->GetHeight(modifiedImageID[1]), 4, imgMan->GetMemory(modifiedImageID[1])->memoryData);
                    }
                }
            }
            else if (receptionState == "begin_vkCmdCopyImageToBuffer")
            {
                if (omitValue(input) == "layer_srcImg")
                {
                    modifiedImageID[0] = imgMan->GetFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_dstBuf")
                {
                    /* if source image exists, attempt to copy the data */
                    if (modifiedImageID[0] != -1)
                    {
                        /* get destination buffer ID */
                        modifiedBufferID[1] = bufMan->GetFromPointerID(omitMessage(input));
                        bufMan->SetState(modifiedBufferID[1], "copied");

                        /* get source memory ID */
                        modifiedMemoryID[0] = memMan->GetFromPointerID(imgMan->GetBoundObj(modifiedImageID[0]));

                        /* get destination memory ID */
                        modifiedMemoryID[1] = memMan->GetFromPointerID(bufMan->GetBoundObj(modifiedBufferID[1]));

                        /* copy data between bound memory objects */
                        memMan->AssignData(modifiedMemoryID[1], memMan->GetData(modifiedMemoryID[0]), memMan->GetDataSize(modifiedMemoryID[0]));
                    }
                }
            }
            else if (receptionState == "begin_vkCmdCopyBufferToImage")
            {
                if (omitValue(input) == "layer_srcBuf")
                {
                    /* get source buffer ID */
                    modifiedBufferID[0] = bufMan->GetFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_dstImg")
                {
                    /* if source buffer exists, attempt to copy data */
                    if (modifiedBufferID[0] != -1)
                    {
                        /* get destination image ID */
                        modifiedImageID[1] = imgMan->GetFromPointerID(omitMessage(input));
                        imgMan->SetState(modifiedImageID[1], "copied");
                        
                        /* get source memory ID */
                        modifiedMemoryID[0] = memMan->GetFromPointerID(bufMan->GetBoundObj(modifiedBufferID[0]));

                        /* get destination memory ID */
                        modifiedMemoryID[1] = memMan->GetFromPointerID(imgMan->GetBoundObj(modifiedImageID[1]));

                        /* copy data between bound memory objects */
                        memMan->AssignData(modifiedMemoryID[1], memMan->GetData(modifiedMemoryID[0]), memMan->GetDataSize(modifiedMemoryID[0]));
                        //winMan->LoadImageTexture(modifiedImageID[1], imgMan->GetWidth(modifiedImageID[1]), imgMan->GetHeight(modifiedImageID[1]), 4, imgMan->GetMemory(modifiedImageID[1])->memoryData);
                    }
                }
            }
            else if (receptionState == "begin_vkCmdCopyImage")
            {
                if (omitValue(input) == "layer_srcImg")
                {
                    /* get source image ID */
                    modifiedImageID[0] = imgMan->GetFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_dstImg")
                {
                    /* if source image exists, attempt to copy the data */
                    if (modifiedMemoryID[0] != -1)
                    {
                        /* get destination image ID */
                        modifiedImageID[1] = imgMan->GetFromPointerID(omitMessage(input));
                        imgMan->SetState(modifiedImageID[1], "copied");

                        /* get source memory ID */
                        modifiedMemoryID[0] = memMan->GetFromPointerID(imgMan->GetBoundObj(modifiedImageID[0]));

                        /* get destination memory ID */
                        modifiedMemoryID[1] = memMan->GetFromPointerID(imgMan->GetBoundObj(modifiedImageID[1]));

                        memMan->AssignData(modifiedMemoryID[1], memMan->GetData(modifiedMemoryID[0]), memMan->GetDataSize(modifiedMemoryID[0]));
                        //winMan->LoadImageTexture(modifiedImageID[1], imgMan->GetWidth(modifiedImageID[1]), imgMan->GetHeight(modifiedImageID[1]), 4, imgMan->GetMemory(modifiedImageID[1])->memoryData);
                    }
                }
            }
            else if (receptionState == "begin_vkCmdCopyBuffer")
            {
                if (omitValue(input) == "layer_srcBuf")
                {
                    /* get source buffer ID */
                    modifiedBufferID[0] = bufMan->GetFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_dstBuf")
                {
                    /* if source buffer exists, attempt to copy data */
                    if (modifiedBufferID[0] != -1)
                    {
                        /* get destination buffer ID */
                        modifiedBufferID[1] = bufMan->GetFromPointerID(omitMessage(input));
                        bufMan->SetState(modifiedBufferID[1], "copied");

                        /* get source memory ID */
                        modifiedMemoryID[0] = memMan->GetFromPointerID(bufMan->GetBoundObj(modifiedBufferID[0]));

                        /* get destination memory ID */
                        modifiedMemoryID[1] = memMan->GetFromPointerID(bufMan->GetBoundObj(modifiedBufferID[1]));

                        /* assign source memory data to destination memory */
                        memMan->AssignData(modifiedMemoryID[1], memMan->GetData(modifiedMemoryID[0]), memMan->GetDataSize(modifiedMemoryID[0]));
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
        else if(logCalls)
        {
            if (omitValue(input) == "result")
                currentCall->assignRetVal(omitMessage(input));
            currentCall->assignParameter(input);
        }
    }

    /* Receive new message from the thread, send it to be parsed after it has been constructed successfully */
    void events::newInfo(const char* input, int index)
    {
        if (index <= 0)
            return;
        std::string s = input;

        s = s.substr(0, index);

        if (currentDataSize < dataSize)
        {
            /* catch data */
            currentDataSize += index;
            if (currentDataSize >= dataSize)
            {
                size_t remainderIndex = currentDataSize - dataSize;
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
                /*
                char* temp = (char*)malloc(currentDataSize - dataSize);
                memcpy(temp, input + remainderIndex, currentDataSize - dataSize);

                remainder = temp;
                remainder = remainder.substr(0, currentDataSize - dataSize);
                */

                std::string invokeData = "layer_data=" + std::to_string(dataSize);
                parseMessage(invokeData.c_str());
                parseMessage(allocatedData);
                int offset = currentDataSize - dataSize;
                currentDataSize = 0;
                dataSize = 0;
                remainder = "";
                newInfo(input + remainderIndex, offset);
                return;
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
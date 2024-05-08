/*
* Name		    : events.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file for data reception, message parsing and data processing
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#include "events.hpp"

namespace details {

    /* initialize objects that take care of data */
    void events::createDataManagers()
    {
        memMan = new vkMemoryManager();
        bufMan = new vkBufferManager();
        imgMan = new vkImageManager();
        sysMan = new vkSystemManager(winMan);
    }
    /* render a new Vulkan image in ImGui */
    void events::loadTexture(unsigned long long ID )
    { 
        if(imgMan->getData(ID) != NULL)
            winMan->LoadImageTexture(ID, imgMan->GetWidth(ID), imgMan->GetHeight(ID), 4, imgMan->getMemory(ID)->memoryData); 
    }
    /* returns the value of the input message */
    std::string events::omitMessage(std::string input)
    {
        auto pos = input.find("=");
        if (pos == std::string::npos)
            return "NULL";

        return input.substr(pos + 1, input.size());
    }
    /* returns the type of the input message */
    std::string events::omitValue(std::string input)
    {
        auto pos = input.find("=");
        if (pos == std::string::npos)
            return "NULL";

        return input.substr(0, pos);
    }
    /* based on the input message, manipulate the currently debugged application state */
    void events::parseMessage(const char* inputChar)
    {
        /* inputChar is always a singular message */
        /* handle binary data input */
        if (dataLoad)
        {
            /* if destination memory already has data, free them and attempt to replace them with the new data */
            if (memMan->getData(modifiedMemoryID[0]) != NULL)
                free(memMan->getData(modifiedMemoryID[0]));

            memMan->assignData(modifiedMemoryID[0], (char*)inputChar, lastMemorySize);
            modifiedBufferID[1] = bufMan->getFromPointerID(memMan->getBoundObj(modifiedMemoryID[0]));
            modifiedImageID[1] = imgMan->getFromPointerID(memMan->getBoundObj(modifiedMemoryID[0]));

            dataLoad = false;
            return;
        }
        std::string input = inputChar;

        /* beginning of an api call */
        if (input.substr(0, 6) == "begin_")
        {
            receptionState = input;
            if (logCalls)
            {
                /* new API call has been started, the following messages without proper prefix will be considered as parameters to this call */
                currentCall = new apiCall(callID++);
                currentCall->assignName(input);
            }
            if (receptionState == "begin_" + delimStr)
            {
                /* delimiter function has been called, start a new frame */
                frameID++;
                if(logCalls)
                    currentCallList = {};
            }
            /* new objects of a certain type, expect data about them */
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
            /* this object's data is now complete, prepare to expect a new one in the future */
            if (receptionState == "end_vkAllocateMemory")
            {
                if(logCalls)
                    memMan->assignCulprit(memoryID,currentCall);
                memoryID++;
            }
            else if (receptionState == "end_vkCreateImage")
            {
                if(logCalls)
                    imgMan->assignCulprit(imageID, currentCall);
                imageID++;
            }
            else if (receptionState == "end_vkCreateBuffer")
            {
                if(logCalls)
                    bufMan->assignCulprit(bufferID, currentCall);
                bufferID++;
            }
        }
        else if (input.substr(0, 6) == "layer_")
        {
            /* instructions sent by layer.cpp */

            /* information about layer initialization parameters */
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

            /* received a new warning */
            if (input.substr(0, 14) == "layer_warning_")
            {
                warningsList.push_back(input);
            }

            /* state of Vulkan objects changes */
            if (receptionState == "begin_vkUnmapMemory")
            {
                if (input.substr(0, 11) == "layer_data=")
                {
                    dataLoad = true;
                }
                else if (omitValue(input) == "layer_ptr")
                {
                    modifiedMemoryID[0] = memMan->getFromPointerID(omitMessage(input));
                    memMan->setState(modifiedMemoryID[0], "unmapped");
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
                    modifiedMemoryID[1] = memMan->getFromPointerID(omitMessage(input));
                    memMan->setState(modifiedMemoryID[1], "mapped");
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
                    modifiedMemoryID[0] = memMan->getFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_size")
                {
                    lastMemorySize = stoul(omitMessage(input));
                }
            }
            else if (receptionState == "begin_vkAllocateMemory")
            {
                memMan->assignPointer(memoryID, omitMessage(input));
            }
            else if (receptionState == "begin_vkFreeMemory")
            {
                memMan->setState(memMan->getFromPointerID(omitMessage(input)), "freed");
            }
            else if (receptionState == "begin_vkCreateImage")
            {
                if (omitValue(input) == "layer_ptr")
                {
                    imgMan->assignPointer(imageID, omitMessage(input));
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
                modifiedImageID[1] = imgMan->getFromPointerID(omitMessage(input));
                if (modifiedImageID[1] != -1)
                    imgMan->setState(modifiedImageID[1], "freed");
            }
            else if (receptionState == "begin_vkCreateBuffer")
            {
                bufMan->assignPointer(bufferID, omitMessage(input));
            }
            else if (receptionState == "begin_vkDestroyBuffer")
            {
                modifiedBufferID[1] = bufMan->getFromPointerID(omitMessage(input));
                if (modifiedBufferID[1] != -1)
                    bufMan->setState(modifiedBufferID[1], "freed");
            }
            else if (receptionState == "begin_vkBindBufferMemory")
            {
                if (omitValue(input) == "layer_memPtr")
                {
                    /* get source memory ID */
                    modifiedMemoryID[0] = memMan->getFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_bufPtr")
                {
                    /* if source memory exists, attempt to bind it to the destination buffer */
                    if (modifiedMemoryID[0] != -1)
                    {
                        /* get destination buffer ID */
                        modifiedBufferID[1] = bufMan->getFromPointerID(omitMessage(input));

                        /* bind memoryPtr to bufferPtr */
                        bufMan->assignBoundObj(modifiedBufferID[1], memMan->getPointer(modifiedMemoryID[0]));
                        bufMan->setState(modifiedBufferID[1], "bound");

                        /* bind bufferPtr to memoryPtr */
                        memMan->assignBoundObj(modifiedMemoryID[0], omitMessage(input));
                        memMan->setState(modifiedMemoryID[0], "bound");

                        /* assign memoryObj to bufferObj */
                        bufMan->assignMemory(modifiedBufferID[1], memMan->getMemory(modifiedMemoryID[0]));
                    }
                }
            }
            else if (receptionState == "begin_vkBindImageMemory")
            {
                if (omitValue(input) == "layer_memPtr")
                {
                    /* get source memory ID */
                    modifiedMemoryID[0] = memMan->getFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_imgPtr")
                {
                    /* if source memory exists, attempt to bind it to the destination image */
                    if (modifiedMemoryID[0] != -1)
                    {
                        /* get destination image ID */
                        modifiedImageID[1] = imgMan->getFromPointerID(omitMessage(input));

                        /* bind memoryPtr to imagePtr */
                        imgMan->assignBoundObj(modifiedImageID[1], memMan->getPointer(modifiedMemoryID[0]));
                        imgMan->setState(modifiedImageID[1], "bound");

                        /* bind imagePtr to memoryPtr */
                        memMan->assignBoundObj(modifiedMemoryID[0], omitMessage(input));
                        memMan->setState(modifiedMemoryID[0], "bound");

                        /* assign memoryObj to imageObj */
                        imgMan->assignMemory(modifiedImageID[1], memMan->getMemory(modifiedMemoryID[0]));
                        winMan->LoadImageTexture(modifiedImageID[1], imgMan->GetWidth(modifiedImageID[1]), imgMan->GetHeight(modifiedImageID[1]), 4, imgMan->getMemory(modifiedImageID[1])->memoryData);
                    }
                }
            }
            else if (receptionState == "begin_vkCmdCopyImageToBuffer")
            {
                if (omitValue(input) == "layer_srcImg")
                {
                    modifiedImageID[0] = imgMan->getFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_dstBuf")
                {
                    /* if source image exists, attempt to copy the data */
                    if (modifiedImageID[0] != -1)
                    {
                        /* get destination buffer ID */
                        modifiedBufferID[1] = bufMan->getFromPointerID(omitMessage(input));
                        bufMan->setState(modifiedBufferID[1], "copied");

                        /* get source memory ID */
                        modifiedMemoryID[0] = memMan->getFromPointerID(imgMan->getBoundObj(modifiedImageID[0]));

                        /* get destination memory ID */
                        modifiedMemoryID[1] = memMan->getFromPointerID(bufMan->getBoundObj(modifiedBufferID[1]));

                        /* copy data between bound memory objects */
                        memMan->assignData(modifiedMemoryID[1], memMan->getData(modifiedMemoryID[0]), memMan->getDataSize(modifiedMemoryID[0]));
                    }
                }
            }
            else if (receptionState == "begin_vkCmdCopyBufferToImage")
            {
                if (omitValue(input) == "layer_srcBuf")
                {
                    /* get source buffer ID */
                    modifiedBufferID[0] = bufMan->getFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_dstImg")
                {
                    /* if source buffer exists, attempt to copy data */
                    if (modifiedBufferID[0] != -1)
                    {
                        /* get destination image ID */
                        modifiedImageID[1] = imgMan->getFromPointerID(omitMessage(input));
                        imgMan->setState(modifiedImageID[1], "copied");
                        
                        /* get source memory ID */
                        modifiedMemoryID[0] = memMan->getFromPointerID(bufMan->getBoundObj(modifiedBufferID[0]));

                        /* get destination memory ID */
                        modifiedMemoryID[1] = memMan->getFromPointerID(imgMan->getBoundObj(modifiedImageID[1]));

                        /* copy data between bound memory objects */
                        memMan->assignData(modifiedMemoryID[1], memMan->getData(modifiedMemoryID[0]), memMan->getDataSize(modifiedMemoryID[0]));
                        //winMan->LoadImageTexture(modifiedImageID[1], imgMan->GetWidth(modifiedImageID[1]), imgMan->GetHeight(modifiedImageID[1]), 4, imgMan->getMemory(modifiedImageID[1])->memoryData);
                    }
                }
            }
            else if (receptionState == "begin_vkCmdCopyImage")
            {
                if (omitValue(input) == "layer_srcImg")
                {
                    /* get source image ID */
                    modifiedImageID[0] = imgMan->getFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_dstImg")
                {
                    /* if source image exists, attempt to copy the data */
                    if (modifiedMemoryID[0] != -1)
                    {
                        /* get destination image ID */
                        modifiedImageID[1] = imgMan->getFromPointerID(omitMessage(input));
                        imgMan->setState(modifiedImageID[1], "copied");

                        /* get source memory ID */
                        modifiedMemoryID[0] = memMan->getFromPointerID(imgMan->getBoundObj(modifiedImageID[0]));

                        /* get destination memory ID */
                        modifiedMemoryID[1] = memMan->getFromPointerID(imgMan->getBoundObj(modifiedImageID[1]));

                        memMan->assignData(modifiedMemoryID[1], memMan->getData(modifiedMemoryID[0]), memMan->getDataSize(modifiedMemoryID[0]));
                        //winMan->LoadImageTexture(modifiedImageID[1], imgMan->GetWidth(modifiedImageID[1]), imgMan->GetHeight(modifiedImageID[1]), 4, imgMan->getMemory(modifiedImageID[1])->memoryData);
                    }
                }
            }
            else if (receptionState == "begin_vkCmdCopyBuffer")
            {
                if (omitValue(input) == "layer_srcBuf")
                {
                    /* get source buffer ID */
                    modifiedBufferID[0] = bufMan->getFromPointerID(omitMessage(input));
                }
                else if (omitValue(input) == "layer_dstBuf")
                {
                    /* if source buffer exists, attempt to copy data */
                    if (modifiedBufferID[0] != -1)
                    {
                        /* get destination buffer ID */
                        modifiedBufferID[1] = bufMan->getFromPointerID(omitMessage(input));
                        bufMan->setState(modifiedBufferID[1], "copied");

                        /* get source memory ID */
                        modifiedMemoryID[0] = memMan->getFromPointerID(bufMan->getBoundObj(modifiedBufferID[0]));

                        /* get destination memory ID */
                        modifiedMemoryID[1] = memMan->getFromPointerID(bufMan->getBoundObj(modifiedBufferID[1]));

                        /* assign source memory data to destination memory */
                        memMan->assignData(modifiedMemoryID[1], memMan->getData(modifiedMemoryID[0]), memMan->getDataSize(modifiedMemoryID[0]));
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

        /* refresh window when the new data is received */
        glfwPostEmptyEvent();
    }

    /* receive potentially incomplete, meshed data and break them into comprehensive messages */
    void events::newInfo(const char* input, int index)
    {
        /* if there are no viable data, don't look for a message in the input buffer */
        if (index <= 0)
            return;
        std::string s = input;
        /* viable data cut out of input buffer */
        s = s.substr(0, index);

        /* if we are receiving binary data, ignore the message delimiters and save it as a whole */
        if (currentDataSize < dataSize)
        {
            /* catch data */
            currentDataSize += index;
            /* exeeded the given length, call the newInfo function on the remainder so the information is never lost */
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

                /* binary data size parse */
                std::string invokeData = "layer_data=" + std::to_string(dataSize);
                parseMessage(invokeData.c_str());
                /* binary data parse */
                parseMessage(allocatedData);
                int offset = currentDataSize - dataSize;
                currentDataSize = 0;
                dataSize = 0;
                remainder = "";
                /* break the remainder into comprehensive messages */
                newInfo(input + remainderIndex, offset);
                return;
            }
            else
            {
                /* concatenate input data */
                memcpy(allocatedData + currentDataSize - index, input, index);
            }
            return;
        }
        s = remainder + s;

        std::string token;
        size_t pos = 0;
        /* break buffer by delimiters into messages */
        while ((pos = s.find("!")) != std::string::npos) {
            token = s.substr(0, pos);
            s.erase(0, pos + 1);

            /* this indicates the start of the binary data is right after this message, so prepare this function to save binary data */
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
                /* single message found */
                parseMessage(token.c_str());
            }
        }
        /* in case the buffer came incomplete, e.g. vkCreateIn....XXX -- handle it in the next call -- VkCreateInstance!Message2!Mesage3!.... */
        if (s.size() > 0)
            remainder = s;
        else
            remainder = "";
    }

    /* thread to catch layer data */
    DWORD WINAPI events::listenForData(__in LPVOID lpParameter)
    {
        /* create socket */
        int ret;
        SOCKET ClientSocket = INVALID_SOCKET;
        char recvbuf[DEFAULT_BUFLEN];
        uiWinsockInit(&ClientSocket);

        /* receive data unless the other end is disconnected */
        details::events* myObj = (details::events*)lpParameter;
        (*myObj).createDataManagers();
        do {
            /* ret is the length of successfully received data */
            ret = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
            if (ret > 0)
            {
                /* Parse data */
                (*myObj).newInfo(recvbuf, ret);
            }
            else if (ret == 0)
            {
                /* closing connection */
                break;
            }
            else
            {
                /* recv failed */
                closesocket(ClientSocket);
                WSACleanup();
                break;
            }
        } while (ret > 0);

        /* Destroy socket */
        uiWinsockExit(&ClientSocket);
        return 0;
    }

    /* establish connection between VkDebuggerApp and VkDebuggerLayer */
    void events::connectToLayer(details::appWindow* windowManager)
    {
        /* receive window object reference from appUI.cpp */
        winMan = windowManager;
        DWORD mythreadId;
        /* Collect data from socket continuously */
        CreateThread(0, 0, events::listenForData, this, 0, &mythreadId);
    }
}
/*
* Name		    : layer_data.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file to store the current state of the Vulkan constructs and perform operations on them
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/

#include "layer.hpp"
#include "layer_data.hpp"
#include <chrono>

#define CUSTOM_PARAM_PREFIX "layer_"
#define CUSTOM_DATA_PREFIX "data_"

SOCKET ConnectSocket = INVALID_SOCKET;
void* map;
uint64_t image_size;

#include <iostream>
/* Enviroment Variables */

std::map <std::string, std::string> envVar;

bool warnings = false;
bool fpsBelowOption = false;
bool fpsEveryOption = false;
unsigned long long fpsBelowTrigger;
unsigned long long fpsEveryTrigger;

bool breaks = false;
bool fpsBelowOptionBreak = false;
bool frameAtBreak = false;
bool frameEveryBreak = false;
bool callAtBreak = false;
bool callEveryBreak = false;
bool memoryNewBreak = false;
bool bufferNewBreak = false;
bool imageNewBreak = false;
std::string appPath;
unsigned long long fpsBelowBreakTrigger;
unsigned long long frameEveryBreakTrigger;
unsigned long long frameAtBreakTrigger;
unsigned long long callEveryBreakTrigger;
unsigned long long callAtBreakTrigger;
#include <filesystem>
bool logBuffers;
bool logImages;
bool logMemory;

int frameDelim;
enum delim
{
    enum_vkAcquireNextImageKHR = 0,
    enum_vkQueueSubmit = 1,
    enum_vkCmdDraw = 2
};

unsigned long long callCount = 0;
void layer_newCall()
{
    callCount++;
    if (callAtBreak)
    {
        if (callCount == callAtBreakTrigger)
        {
            winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "callAtBreakTrigger=", std::to_string(callCount)));
            system("pause");
        }
    }
    if (callEveryBreak)
    {
        if (callCount % callEveryBreakTrigger == 0)
        {
            winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "callEveryBreakTrigger=", std::to_string(callCount)));
            system("pause");
        }
    }
}

void layer_SetEnvVariables()
{
    if (envVar["enable_warnings"] == "true")
    {
        warnings = true;
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "warnings=", "true"));
    }
    else
    {
        warnings = false;
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "warnings=", "false"));
    }
    if (envVar["log_calls"] == "true")
    {
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "log_calls=", "true"));
    }
    else
    {
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "log_calls=", "false"));
    }
    if (envVar["log_buffers"] == "true")
    {
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "log_buffers=", "true"));
        logBuffers = true;
    }
    else
    {
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "log_buffers=", "false"));
        logBuffers = false;
    }
    if (envVar["log_images"] == "true")
    {
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "log_images=", "true"));
        logImages = true;
    }
    else
    {
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "log_images=", "false"));
        logImages = false;
    }
    if (envVar["log_memory"] == "true")
    {
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "log_memory=", "true"));
        logMemory = true;
    }
    else
    {
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "log_memory=", "false"));
        logMemory = false;
    }
    if (envVar["enable_breaks"] == "true")
    {
        breaks = true;
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "breaks=", "true"));
    }
    else
    {
        breaks = false;
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "breaks=", "false"));
    }
    if (breaks == true)
    {
        if (std::stol(envVar["break_fps_below"]) > 0)
        {
            fpsBelowOptionBreak = true;
            fpsBelowBreakTrigger = std::stol(envVar["break_fps_below"]);
        }
    }
    if (breaks == true)
    {
        if (std::stol(envVar["break_frame_at"]) > 0)
        {
            frameAtBreak = true;
            frameAtBreakTrigger = std::stol(envVar["break_frame_at"]);
        }
    }
    if (breaks == true)
    {
        if (std::stol(envVar["break_frame_every"]) > 0)
        {
            frameEveryBreak = true;
            frameEveryBreakTrigger = std::stol(envVar["break_frame_every"]);
        }
    }
    if (breaks == true)
    {
        if (std::stol(envVar["break_call_at"]) > 0)
        {
            callAtBreak = true;
            callAtBreakTrigger = std::stol(envVar["break_call_at"]);
        }
    }
    if (breaks == true)
    {
        if (std::stol(envVar["break_call_every"]) > 0)
        {
            callEveryBreak = true;
            callEveryBreakTrigger = std::stol(envVar["break_call_every"]);
        }
    }

    if (envVar["break_memory_at"] == "true" && breaks == true)
    {
        memoryNewBreak = true;
    }
    if (envVar["break_buffer_at"] == "true" && breaks == true)
    {
        bufferNewBreak = true;
    }
    if (envVar["break_image_at"] == "true" && breaks == true)
    {
        imageNewBreak = true;
    }

    if (envVar["frame_delim"] == "delim_vkAcquireNextImageKHR")
    {
        frameDelim = enum_vkAcquireNextImageKHR;
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "delim=", "vkAcquireNextImageKHR"));
    }
    else if (envVar["frame_delim"] == "delim_vkQueueSubmit")
    {
        frameDelim = enum_vkQueueSubmit;
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "delim=", "vkQueueSubmit"));
    }
    else if (envVar["frame_delim"] == "delim_vkCmdDraw")
    {
        frameDelim = enum_vkCmdDraw;
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "delim=", "vkCmdDraw"));
    }

    if (warnings == true)
    {
        if (std::stol(envVar["fps_below"]) > 0)
        {
            fpsBelowOption = true;
            fpsBelowTrigger = std::stol(envVar["fps_below"]);
        }
    }
    if (warnings == true)
    {
        if (std::stol(envVar["fps_every"]) > 0)
        {
            fpsEveryOption = true;
            fpsEveryTrigger = std::stol(envVar["fps_every"]);
        }
    }
}

void layer_AppStarter(std::string configContent)
{
    std::string token;
    std::string s = configContent;
    

    int pos;
    while ((pos = s.find("\n")) != std::string::npos) {
        token = s.substr(0, pos);
        s.erase(0, pos + 1);

        if (token[0] == '#' || token[0] == '\0')
            continue;

        auto pos1 = token.find(".");
        auto pos2 = token.substr(pos1 + 1, token.size()).find("=");
        std::string t1 = token.substr(pos1 + 1, pos2 - 1);

        pos2 = token.find("=");
        std::string t2 = token.substr(pos2 + 2, token.size());

        envVar[t1] = t2;
    }

    std::string appPath;
    if (envVar["app_path"] == "default")
    {
        appPath = std::filesystem::current_path().string() + "\\";
        appPath += "VkDebugger.exe";
    }
    else
    {
        appPath = envVar["app_path"];
        std::string s = appPath;
        std::string result = "";
        size_t index;
        while ((index = s.find("/")) != std::string::npos)
        {
            result += s.substr(0,index);
            s.erase(0, index+1);
            result += "\\\\";
        }
        /* file name */
        result += s;
        appPath = result;

        if (s != "VkDebugger.exe")
            std::cerr << "VkDebugger: could not open " << appPath << " it is not a VkDebugger.exe application" << std::endl;
    }

    if (!std::filesystem::exists(appPath))
        std::cerr << "VkDebugger: VkDebugger.exe could not be found at " << appPath << std::endl;

    /* create new process */
    STARTUPINFO info = { sizeof(info) };
    PROCESS_INFORMATION processInfo;

    /* open new vkDetails window */
    CreateProcess(appPath.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &info, &processInfo);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
}

details::layerData* data;
bool readerReady = false;

void layer_AllocateMemory_after(VkDevice device, VkMemoryAllocateInfo* pAllocateInfo, VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
{
    if (logMemory == false)
        return;
    if (memoryNewBreak)
    {
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "memoryNewBreak=", "new"));
        system("pause");
    }

    data->newMemoryObj(*pMemory, (*pAllocateInfo).allocationSize);

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", ptrToString((void**)pMemory)));

}
void layer_FreeMemory_before(VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks* pAllocator)
{
    if (logMemory == false)
        return;
    data->freeMemoryObj(memory);

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", addrToString((void*)memory)));
}
void layer_CreateInstance_after(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    data = new details::layerData();
    data->newInstance(*pInstance);

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "appName=", GetWindowName()));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "proccessID=", std::to_string(GetCurrentProcessId())));
}
void layer_BindBufferMemory_after(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    if (logBuffers == false)
        return;

    if (logMemory)
    {
        data->newBinding(memory, buffer);
        data->setState(buffer, "bound");
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "memPtr=", addrToString((void*)memory)));
    }
    else
    {
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "memPtr=", "Logging VkMemory is disabled."));
    }

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "bufPtr=", addrToString((void*)buffer)));
}
void layer_BindImageMemory_after(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    if (logImages == false)
        return;


    if (logMemory)
    {
        data->newBinding(memory, image);
        data->setState(image, "bound");
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "memPtr=", addrToString((void*)memory)));
    }
    else
    {
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "memPtr=", "Logging VkMemory is disabled."));
    }

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "imgPtr=", addrToString((void*)image)));
}
void layer_MapMemory_after(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
{
    if (logMemory == false)
        return;
    data->memoryMapping(memory, size, ppData);

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", ptrToString((void**)memory)));
}
void layer_GetPhysicalDeviceSurfaceSupportKHR_before(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
{
    // empty 
}
void layer_CreateCommandPool_after(VkDevice device, VkCommandPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
{
    // empty
}
void layer_UnmapMemory_before(VkDevice device, VkDeviceMemory memory)
{
    if (logMemory == false)
        return;
    data->memoryUnMapping(memory);
    
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", addrToString((void*)memory)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "size=", std::to_string(data->getSize(memory))));
    
    std::string dataMessage = "data";
    dataMessage += std::to_string(data->getSize(memory));
    dataMessage += '!';
    
    winsockSendToUI(&ConnectSocket, dataMessage);
    winsockSendToUIraw(&ConnectSocket, reinterpret_cast<char*>(*(data->getData(memory))), data->getSize(memory));
}
void layer_CreateImage_after(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
    if (logImages == false)
        return;
    if (imageNewBreak)
    {
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "imageNewBreak=", "new"));
        system("pause");
    }
    data->newImage(*pImage, pCreateInfo->initialLayout, pCreateInfo->extent);

    if (readerReady == false)
    {
        connected = false;
        data->initReader(); // value.size, value.boundImage
        connected = true;
        readerReady = true;
    }

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", ptrToString((void**)pImage)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "width=", std::to_string(pCreateInfo->extent.width)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "height=", std::to_string(pCreateInfo->extent.height)));

}
void layer_CreateBuffer_after(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
    if (logBuffers == false)
        return;
    if (bufferNewBreak)
    {
        winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "bufferNewBreak=", "new"));
        system("pause");
    }

    if (readerReady == false)
    {
        connected = false;
        data->initReader(); // value.size, value.boundImage
        connected = true;
        readerReady = true;
    }

    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", ptrToString((void**)pBuffer)));

}
void layer_DestroyImage_before(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator)
{
    if (logImages == false)
        return;
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", addrToString((void*)image)));
}
void layer_DestroyBuffer_before(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator)
{
    if (logBuffers == false)
        return;
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", addrToString((void*)buffer)));
}
void layer_CmdCopyImageToBuffer_before(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, VkBufferImageCopy* pRegions)
{
    if (logImages == false)
        return;
    if (logBuffers == false)
        return;
    //data->setState(dstBuffer, "copied");
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "dstBuf=", addrToString((void*)dstBuffer)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "srcImg=", addrToString((void*)srcImage)));
}
void layer_CmdCopyBufferToImage_before(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkBufferImageCopy* pRegions)
{
    if (logImages == false)
        return;
    if (logBuffers == false)
        return;
    //data->setState(dstImage, "copied");
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "srcBuf=", addrToString((void*)srcBuffer)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "dstImg=", addrToString((void*)dstImage)));
}
void layer_CmdCopyBuffer_before(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy* pRegions)
{
    if (logBuffers == false)
        return;
    //data->setState(dstBuffer, "copied");
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "srcBuf=", addrToString((void*)srcBuffer)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "dstBuf=", addrToString((void*)dstBuffer)));
}
void layer_CmdCopyImage_before(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageCopy* pRegions)
{
    if (logImages == false)
        return;
    //data->setState(dstImage, "copied");
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "srcImg=", addrToString((void*)srcImage)));
    winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "dstImg=", addrToString((void*)dstImage)));
}
void layer_CreateDevice_after(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    data->newDevice(*pDevice);
}


/* before new frame hits */
typedef std::chrono::steady_clock FPSclock;
typedef std::chrono::duration<float, std::milli> FPSduration;

unsigned long long frameCount = 0;
unsigned long long unbiasedFrameCount = 0;

FPSduration startTime;
FPSduration lastTime;

std::chrono::nanoseconds timeTotal = {};
std::chrono::nanoseconds timeDiff;
double FPS;

void frameWarnBreak()
{
    if (frameAtBreak || frameEveryBreak)
    {
        unbiasedFrameCount++;
        if (frameAtBreak && unbiasedFrameCount == frameAtBreakTrigger)
        {
            winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "frameAtBreakTrigger=", std::to_string(frameAtBreakTrigger)));
            system("pause");
        }
        if (frameEveryBreak && unbiasedFrameCount % (frameEveryBreakTrigger) == 0)
        {
            winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "frameEveryBreakTrigger=", std::to_string(frameEveryBreakTrigger)));
            system("pause");
        }
    }
    if (fpsEveryOption || fpsBelowOption || fpsBelowOptionBreak)
    {
        startTime = FPSclock::now().time_since_epoch();
        frameCount++;
    }

    if ((fpsEveryOption || fpsBelowOption || fpsBelowOptionBreak) && lastTime != std::chrono::nanoseconds::zero())
    {
        timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(startTime - lastTime);
        timeTotal += timeDiff;

        double sec = (double)std::chrono::duration_cast<std::chrono::milliseconds>(timeTotal).count() / 1000;
        FPS = (double)frameCount / sec;

        if (fpsEveryOption && frameCount >= fpsEveryTrigger)
        {
            winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "fpsEveryTrigger=", std::to_string(FPS)));
            timeTotal = {};
            frameCount = 0;
        }
        if (fpsBelowOption && FPS < fpsBelowTrigger)
        {
            winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "fpsBelowTrigger=", std::to_string(FPS)));
            if (fpsEveryOption == false)
            {
                timeTotal = {};
                frameCount = 0;
            }
        }
        if (fpsBelowOptionBreak && FPS < fpsBelowBreakTrigger)
        {
            winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "fpsBelowOptionBreak=", std::to_string(FPS)));
            system("pause");
            if (fpsEveryOption == false)
            {
                timeTotal = {};
                frameCount = 0;
            }
        }
    }

    if (fpsEveryOption || fpsBelowOption || fpsBelowOptionBreak)
        lastTime = FPSclock::now().time_since_epoch();
}

void layer_AcquireNextImageKHR_before(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
{
    if(frameDelim == enum_vkAcquireNextImageKHR)
        frameWarnBreak();
}

void layer_CmdDraw_after(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    if (frameDelim == enum_vkCmdDraw)
        frameWarnBreak();
}
void layer_QueueSubmit_after(VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence)
{
    for (const auto& [key, value] : *(data->getMemoryMap())) {
        if (value.status == "mapped")
        {
            auto tarObject = value;
            if (value.data == NULL)
                continue;
            winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", addrToString((void*)key)));
            winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "size=", std::to_string(tarObject.size)));

            std::string dataMessage = "data";
            dataMessage += std::to_string(tarObject.size);
            dataMessage += '!';

            winsockSendToUI(&ConnectSocket, dataMessage);
            winsockSendToUIraw(&ConnectSocket, reinterpret_cast<char*>(*tarObject.data), tarObject.size);
        }
        else if (value.status == "bound")
        {
            if (value.boundTo == "image")
            {
                connected = false;
                data->mapImageToBuffer(key);
                connected = true;
            }
            else if (value.boundTo == "buffer")
            {
                connected = false;
                data->mapBufferToBuffer(key);
                connected = true;
            }
            winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "ptr=", addrToString((void*)key)));
            winsockSendToUI(&ConnectSocket, formulateMessage(CUSTOM_PARAM_PREFIX, "size=", std::to_string(value.size)));

            std::string dataMessage = "data";
            dataMessage += std::to_string(value.size);
            dataMessage += '!';

            void* limitedData = (void*)malloc(value.size);
            memcpy(limitedData, *data->getBufferData(), value.size);

            winsockSendToUI(&ConnectSocket, dataMessage);
            winsockSendToUIraw(&ConnectSocket, (reinterpret_cast<char*>(limitedData)), value.size);

            free(limitedData);
        }
    }

    if (frameDelim == enum_vkQueueSubmit)
        frameWarnBreak();
}
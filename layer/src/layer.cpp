/*
* Name		    : layer.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file to implement extra behavior to a set of chosen Vulkan functions
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/

#include "../include/layer.hpp"
#include "../include/layerData.hpp"
#include <chrono>
/* AI-Assisted: Claude Opus 5.5: syntax */
#include <cstring>
#include <sstream>
#include <vector>
/* AI-Assisted: end */

/* AI-Assisted: Claude Opus 5.5: syntax */
namespace vkdebug::layer {
    /* AI-Assisted: end */

    /* list of variables used only in this particular program */

    /* associates the input parameter key with its value, use them to initialize this layer's functionality */
    std::map<std::string, std::string> envVar;
    /* this layer's socket */
    SOCKET connectSocket = INVALID_SOCKET;
    /* AI-Assisted: Claude Opus 5.5: syntax */
    std::string globalUiPath;
    /* AI-Assisted: end */
    bool readerReady = false;
    /* object that takes care of handling data */
    vkdebug::layer::layerData *data;
    /* DebuggerLayer basic functionality */
    bool logBuffers;
    bool logImages;
    bool logMemory;
    bool warnings = false;
    bool breaks = false;
    /* warning related triggers */
    bool fpsBelowOption = false;
    bool fpsEveryOption = false;
    unsigned long long fpsBelowTrigger;
    unsigned long long fpsEveryTrigger;
    /* breakpoint related triggers*/
    bool fpsBelowOptionBreak = false;
    bool frameAtBreak = false;
    bool frameEveryBreak = false;
    bool callAtBreak = false;
    bool callEveryBreak = false;
    bool memoryNewBreak = false;
    bool bufferNewBreak = false;
    bool imageNewBreak = false;
    unsigned long long fpsBelowBreakTrigger;
    unsigned long long frameEveryBreakTrigger;
    unsigned long long frameAtBreakTrigger;
    unsigned long long callEveryBreakTrigger;
    unsigned long long callAtBreakTrigger;
    /* frame delimiter options */
    int frameDelim;

    enum delim {
        DelimVkAcquireNextImageKHR = 0,
        DelimVkQueueSubmit = 1,
        /* AI-Assisted: Claude Opus 5.5: syntax */
        DelimVkQueuePresentKHR = 2,
        DelimFrameless = 3
        /* AI-Assisted: end */
    };

    /* FPS counter, warning and breakpoint related */
    typedef std::chrono::steady_clock fpsClock;
    typedef std::chrono::duration<float, std::milli> fpsDuration;
    unsigned long long frameCount = 0;
    unsigned long long unbiasedFrameCount = 0;
    fpsDuration startTime;
    fpsDuration lastTime;
    std::chrono::nanoseconds timeTotal = {};
    std::chrono::nanoseconds timeDiff;
    double fps;
    /* count of Vulkan functions caught by this layer so far, breakpoint related */
    unsigned long long callCount = 0;
    extern bool callAtBreak;
    extern bool callEveryBreak;

    /* AI-Assisted: Claude Sonnet 5: syntax */
    long envVarAsInt(const std::string &key) {
        try {
            return std::stol(envVar[key]);
        } catch (const std::exception &) {
            return 0;
        }
    }

    bool layer_RecvExact(char *buf, int n) {
        int total = 0;
        while (total < n) {
            int r = recv(connectSocket, buf + total, n - total, 0);
            if (r == 0)
                return false;
            if (r < 0) {
                int err = WSAGetLastError();
                if (err == WSAEINTR || err == WSAEWOULDBLOCK)
                    continue;
                return false;
            }
            total += r;
        }
        return true;
    }

    /* AI-Assisted: end */

    /* checks the VkDebuggerApp filename, attempt to start it */
    void layer_AppStarter(std::string configContent) {
        std::string token;
        std::string s = configContent;
        int pos;
        /* extract the input parameters from the layer config file and save them into a map */
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
        /* if default path is chosen, look for the VkDebuggerApp the started app folder */
        if (envVar["app_path"] == "default") {
            appPath = std::filesystem::current_path().string() + "\\";
            appPath += WINDOW_NAME;
        }
        /* if VkConfig was used to set the path, it needs to have the path separator altered and replaced since we are on windows */
        else {
            appPath = envVar["app_path"];
            std::string s = appPath;
            std::string result = "";
            size_t index;
            while ((index = s.find("/")) != std::string::npos) {
                result += s.substr(0, index);
                s.erase(0, index + 1);
                result += "\\\\";
            }
            /* file name */
            result += s;
            appPath = result;

            if (s != WINDOW_NAME)
                std::cerr << "VkDebugger: could not open " << appPath << " it is not a " << WINDOW_NAME <<
                        " application" << std::endl;
        }

        /* AI-Assisted: Claude Opus 5.5: syntax */
        globalUiPath = appPath;
        /* AI-Assisted: end */

        /* AI-Assisted: Claude Sonnet 5: syntax */
        /* [xbilko03] when starting target app via UI (not VulkanConfigurator), don't launch another UI */
        char manualLaunchFlag[8] = "";
        if (GetEnvironmentVariableA("VKDBG_MANUAL_LAUNCH", manualLaunchFlag, sizeof(manualLaunchFlag)) > 0)
            return;
        /* AI-Assisted: end */

        if (!std::filesystem::exists(appPath))
            std::cerr << "VkDebugger: " << WINDOW_NAME << " could not be found at " << appPath << std::endl;

        /* create new process */
        STARTUPINFO info = {sizeof(info)};
        PROCESS_INFORMATION processInfo;

        /* open new VkDebuggerApp window */
        /* AI-Assisted: Claude Opus 5.5: syntax */
        std::string commandLine = "\"" + appPath + "\" --started-by-layer";
        CreateProcess(appPath.c_str(), commandLine.data(), NULL, NULL, FALSE, 0, NULL, NULL, &info, &processInfo);
        /* AI-Assisted: end */

        /* close process handles */
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
    }

    void layer_SetEnvVariables() {
        /* AI-Assisted: Claude Opus 5.5 + Claude Sonnet 5: syntax */
        /* [xbilko03] set basic features */
        if (envVar["enable_warnings"] == "true") {
            warnings = true;
            layer_CreateLayerInfoBool(vkdebug::InfoWarnings, true);
        } else {
            warnings = false;
            layer_CreateLayerInfoBool(vkdebug::InfoWarnings, false);
        }
        if (envVar["log_calls"] == "true") {
            layer_CreateLayerInfoBool(vkdebug::InfoLogCalls, true);
        } else {
            layer_CreateLayerInfoBool(vkdebug::InfoLogCalls, false);
        }
        if (envVar["log_buffers"] == "true") {
            layer_CreateLayerInfoBool(vkdebug::InfoLogBuffers, true);
            logBuffers = true;
        } else {
            layer_CreateLayerInfoBool(vkdebug::InfoLogBuffers, false);
            logBuffers = false;
        }
        if (envVar["log_images"] == "true") {
            layer_CreateLayerInfoBool(vkdebug::InfoLogImages, true);
            logImages = true;
        } else {
            layer_CreateLayerInfoBool(vkdebug::InfoLogImages, false);
            logImages = false;
        }
        if (envVar["log_memory"] == "true") {
            layer_CreateLayerInfoBool(vkdebug::InfoLogMemory, true);
            logMemory = true;
        } else {
            layer_CreateLayerInfoBool(vkdebug::InfoLogMemory, false);
            logMemory = false;
        }
        if (envVar["enable_breaks"] == "true") {
            breaks = true;
            layer_CreateLayerInfoBool(vkdebug::InfoBreaks, true);
        } else {
            breaks = false;
            layer_CreateLayerInfoBool(vkdebug::InfoBreaks, false);
        }

        /* [xbilko03] set breakpoints */
        if (breaks == true) {
            if (envVarAsInt("break_fps_below") > 0) {
                fpsBelowOptionBreak = true;
                fpsBelowBreakTrigger = envVarAsInt("break_fps_below");
            }
        }
        if (breaks == true) {
            if (envVarAsInt("break_frame_at") > 0) {
                frameAtBreak = true;
                frameAtBreakTrigger = envVarAsInt("break_frame_at");
            }
        }
        if (breaks == true) {
            if (envVarAsInt("break_frame_every") > 0) {
                frameEveryBreak = true;
                frameEveryBreakTrigger = envVarAsInt("break_frame_every");
            }
        }
        if (breaks == true) {
            if (envVarAsInt("break_call_at") > 0) {
                callAtBreak = true;
                callAtBreakTrigger = envVarAsInt("break_call_at");
            }
        }
        if (breaks == true) {
            if (envVarAsInt("break_call_every") > 0) {
                callEveryBreak = true;
                callEveryBreakTrigger = envVarAsInt("break_call_every");
            }
        }

        /* [xbilko03] set special breakpoints */
        if (envVar["break_memory_at"] == "true" && breaks == true) {
            memoryNewBreak = true;
        }
        if (envVar["break_buffer_at"] == "true" && breaks == true) {
            bufferNewBreak = true;
        }
        if (envVar["break_image_at"] == "true" && breaks == true) {
            imageNewBreak = true;
        }

        /* [xbilko03] set frame delimiter */
        if (envVar["frameless"] == "true") {
            frameDelim = DelimFrameless;
            layer_CreateLayerInfoString(vkdebug::InfoDelim, "frameless");
        } else if (envVar["frame_delim"] == "delim_vkAcquireNextImageKHR") {
            frameDelim = DelimVkAcquireNextImageKHR;
            layer_CreateLayerInfoString(vkdebug::InfoDelim, "vkAcquireNextImageKHR");
        } else if (envVar["frame_delim"] == "delim_vkQueueSubmit") {
            frameDelim = DelimVkQueueSubmit;
            layer_CreateLayerInfoString(vkdebug::InfoDelim, "vkQueueSubmit");
        } else if (envVar["frame_delim"] == "delim_vkQueuePresentKHR") {
            frameDelim = DelimVkQueuePresentKHR;
            layer_CreateLayerInfoString(vkdebug::InfoDelim, "vkQueuePresentKHR");
        }

        /* [xbilko03] set warnings */
        if (warnings == true) {
            if (envVarAsInt("fps_below") > 0) {
                fpsBelowOption = true;
                fpsBelowTrigger = envVarAsInt("fps_below");
            }
        }
        if (warnings == true) {
            if (envVarAsInt("fps_every") > 0) {
                fpsEveryOption = true;
                fpsEveryTrigger = envVarAsInt("fps_every");
            }
        }
        /* AI-Assisted: end */
    }

    void layer_NewCall() {
        callCount++;
        /* AI-Assisted: Claude Opus 5.5: syntax */
        layer_SetCurrentCall(callCount);
        if (callAtBreak) {
            if (callCount == callAtBreakTrigger) {
                /* [xbilko03] inform the user and stop the program */
                layer_CreateLayerInfoU64(vkdebug::InfoCallAtBreakTrigger, callCount);
                layer_RequestBreak();
            }
        }
        if (callEveryBreak) {
            if (callCount % callEveryBreakTrigger == 0) {
                /* [xbilko03] inform the user and stop the program */
                layer_CreateLayerInfoU64(vkdebug::InfoCallEveryBreakTrigger, callCount);
                layer_RequestBreak();
            }
        }
        /* AI-Assisted: end */
    }

    /* AI-Assisted: Claude Opus 5.5: syntax */
    /* [xbilko03] after FPS breakpoint, the counter (frame clock) gets broken and needs to be flagged for correction */
    static thread_local bool threadFpsBreakResync = false;

    void layer_AfterBreakResumed() {
        if (!threadFpsBreakResync)
            return;
        threadFpsBreakResync = false;
        mapsLock l(globalLock);
        lastTime = fpsClock::now().time_since_epoch();
    }

    /* AI-Assisted: end */

    void frameWarnBreak() {
        /* FPS calculated by the time difference between each of the frames */
        if (frameAtBreak || frameEveryBreak) {
            unbiasedFrameCount++;
            if (frameAtBreak && unbiasedFrameCount == frameAtBreakTrigger) {
                /* inform the user and stop the program */
                /* AI-Assisted: Claude Opus 5.5: syntax */
                layer_CreateLayerInfoU64(vkdebug::InfoFrameAtBreakTrigger, frameAtBreakTrigger);
                layer_RequestBreak();
                /* AI-Assisted: end */
            }
            if (frameEveryBreak && unbiasedFrameCount % (frameEveryBreakTrigger) == 0) {
                /* inform the user and stop the program */
                /* AI-Assisted: Claude Opus 5.5: syntax */
                layer_CreateLayerInfoU64(vkdebug::InfoFrameEveryBreakTrigger, frameEveryBreakTrigger);
                layer_RequestBreak();
                /* AI-Assisted: end */
            }
        }
        if (fpsEveryOption || fpsBelowOption || fpsBelowOptionBreak) {
            /* start time of the new frame */
            startTime = fpsClock::now().time_since_epoch();
            frameCount++;
        }

        if ((fpsEveryOption || fpsBelowOption || fpsBelowOptionBreak) && lastTime != std::chrono::nanoseconds::zero()) {
            timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(startTime - lastTime);
            timeTotal += timeDiff;

            double sec = (double) std::chrono::duration_cast<std::chrono::milliseconds>(timeTotal).count() / 1000;
            fps = (double) frameCount / sec;

            if (fpsEveryOption && frameCount >= fpsEveryTrigger) {
                /* warn the user */
                /* AI-Assisted: Claude Opus 5.5: syntax */
                layer_CreateLayerInfoDouble(vkdebug::InfoFpsEveryTrigger, fps);
                /* AI-Assisted: end */
                timeTotal = {};
                frameCount = 0;
            }
            if (fpsBelowOption && fps < fpsBelowTrigger) {
                /* warn the user */
                /* AI-Assisted: Claude Opus 5.5: syntax */
                layer_CreateLayerInfoDouble(vkdebug::InfoFpsBelowTrigger, fps);
                /* AI-Assisted: end */
                if (fpsEveryOption == false) {
                    timeTotal = {};
                    frameCount = 0;
                }
            }
            if (fpsBelowOptionBreak && fps < fpsBelowBreakTrigger) {
                /* inform the user and stop the program */
                /* AI-Assisted: Claude Opus 5.5: syntax */
                layer_CreateLayerInfoDouble(vkdebug::InfoFpsBelowOptionBreak, fps);
                layer_RequestBreak();
                threadFpsBreakResync = true;
                /* AI-Assisted: end */
                if (fpsEveryOption == false) {
                    timeTotal = {};
                    frameCount = 0;
                }
            }
        }
        /* end time of this frame */
        if (fpsEveryOption || fpsBelowOption || fpsBelowOptionBreak)
            lastTime = fpsClock::now().time_since_epoch();
    }

    void layer_AllocateMemory_after(VkDevice device, VkMemoryAllocateInfo *pAllocateInfo,
                                    VkAllocationCallbacks *pAllocator, VkDeviceMemory *pMemory) {
        /* no point in saving this since the user is not interested in VkDeviceMemory */
        if (logMemory == false)
            return;
        if (memoryNewBreak) {
            /* AI-Assisted: Claude Opus 5.5: syntax */
            layer_CreateLayerInfoString(vkdebug::InfoMemoryNewBreak, "new");
            layer_RequestBreak();
            /* AI-Assisted: end */
        }
        /* save as VkMemoryObject*/
        data->newMemoryObj(*pMemory, (*pAllocateInfo).allocationSize);
        /* AI-Assisted: Claude Opus 5.5: syntax */
        layer_CreateLayerInfoHandle(vkdebug::InfoPtr, (uint64_t) *pMemory);
        /* AI-Assisted: end */
    }

    /* when a memory is freed, invalidate its reference, inform the VkDebuggerApp about it too */
    void layer_FreeMemory_before(VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks *pAllocator) {
        /* no point in dealing with this since the user is not interested in VkDeviceMemory */
        if (logMemory == false)
            return;
        data->freeMemoryObj(memory);

        /* AI-Assisted: Claude Opus 5.5: syntax */
        layer_CreateLayerInfoHandle(vkdebug::InfoPtr, (uint64_t) memory);
        /* AI-Assisted: end */
    }

    void layer_CreateInstance_after(const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator,
                                    VkInstance *pInstance) {
        data = new vkdebug::layer::layerData();
        data->newInstance(*pInstance);
        /* AI-Assisted: Claude Opus 5.5: syntax */
        layer_CreateLayerInfoString(vkdebug::InfoAppName, getWindowName());
        layer_CreateLayerInfoU64(vkdebug::InfoProcessId, GetCurrentProcessId());
        /* AI-Assisted: end */
        /* AI-Assisted: Claude Sonnet 5: syntax */
        char targetAppPath[MAX_PATH];
        GetModuleFileNameA(nullptr, targetAppPath, MAX_PATH);
        layer_CreateLayerInfoString(vkdebug::InfoAppPath, std::string(targetAppPath));
        /* AI-Assisted: end */
    }

    void layer_BindBufferMemory_after(VkDevice device, VkBuffer buffer, VkDeviceMemory memory,
                                      VkDeviceSize memoryOffset) {
        /* no point in saving this since the user is not interested in VkBuffer */
        if (logBuffers == false)
            return;

        /* AI-Assisted: Claude Opus 5.5: syntax */
        if (logMemory) {
            data->newBinding(memory, buffer);
            data->setState(buffer, "bound");
            layer_CreateLayerInfoHandle(vkdebug::InfoMemPtr, (uint64_t) memory);
        } else {
            layer_CreateLayerInfoString(vkdebug::InfoMemPtr, "Logging VkMemory is disabled.");
        }
        layer_CreateLayerInfoHandle(vkdebug::InfoBufPtr, (uint64_t) buffer);
        /* AI-Assisted: end */
    }

    void layer_BindImageMemory_after(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) {
        /* no point in saving this since the user is not interested in VkImages */
        if (logImages == false)
            return;

        /* AI-Assisted: Claude Opus 5.5: syntax */
        if (logMemory) {
            data->newBinding(memory, image);
            data->setState(image, "bound");
            layer_CreateLayerInfoHandle(vkdebug::InfoMemPtr, (uint64_t) memory);
        } else {
            layer_CreateLayerInfoString(vkdebug::InfoMemPtr, "Logging VkMemory is disabled.");
        }
        layer_CreateLayerInfoHandle(vkdebug::InfoImgPtr, (uint64_t) image);
        /* AI-Assisted: end */
    }

    void layer_MapMemory_after(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size,
                               VkMemoryMapFlags flags, void **ppData) {
        if (logMemory == false)
            return;
        data->memoryMapping(memory, size, ppData);
        /* AI-Assisted: Claude Opus 5.5: syntax */
        layer_CreateLayerInfoHandle(vkdebug::InfoPtr, (uint64_t) memory);
        /* AI-Assisted: end */
    }

    void layer_UnmapMemory_before(VkDevice device, VkDeviceMemory memory) {
        if (logMemory == false)
            return;
        data->memoryUnMapping(memory);

        /* AI-Assisted: Claude Opus 5.5: syntax */
        layer_CreateMemoryData((uint64_t) memory, *(data->getData(memory)), data->getSize(memory));
        /* AI-Assisted: end */
    }

    void layer_CreateImage_after(VkDevice device, VkImageCreateInfo *pCreateInfo, VkAllocationCallbacks *pAllocator,
                                 VkImage *pImage) {
        if (logImages == false)
            return;
        /* special image breakpoint trigger */
        if (imageNewBreak) {
            /* AI-Assisted: Claude Opus 5.5: syntax */
            layer_CreateLayerInfoString(vkdebug::InfoImageNewBreak, "new");
            layer_RequestBreak();
            /* AI-Assisted: end */
        }
        /* new VkImageOject */
        data->newImage(*pImage, pCreateInfo->initialLayout, pCreateInfo->extent);

        /* when there are images, user will most likely want to read its contents, pepare this in advance */
        if (readerReady == false) {
            connected = false;
            data->initReader();
            connected = true;
            readerReady = true;
        }

        /* AI-Assisted: Claude Opus 5.5: syntax */
        layer_CreateLayerInfoHandle(vkdebug::InfoPtr, (uint64_t) *pImage);
        layer_CreateLayerInfoU64(vkdebug::InfoWidth, pCreateInfo->extent.width);
        layer_CreateLayerInfoU64(vkdebug::InfoHeight, pCreateInfo->extent.height);
        /* AI-Assisted: end */
    }

    void layer_CreateBuffer_after(VkDevice device, VkBufferCreateInfo *pCreateInfo, VkAllocationCallbacks *pAllocator,
                                  VkBuffer *pBuffer) {
        if (logBuffers == false)
            return;
        if (bufferNewBreak) {
            /* AI-Assisted: Claude Opus 5.5: syntax */
            layer_CreateLayerInfoString(vkdebug::InfoBufferNewBreak, "new");
            layer_RequestBreak();
            /* AI-Assisted: end */
        }

        /* when there are images, user will most likely want to read its contents, pepare this in advance */
        if (readerReady == false) {
            connected = false;
            data->initReader();
            connected = true;
            readerReady = true;
        }

        /* AI-Assisted: Claude Opus 5.5: syntax */
        layer_CreateLayerInfoHandle(vkdebug::InfoPtr, (uint64_t) *pBuffer);
        /* AI-Assisted: end */
    }

    void layer_DestroyBuffer_before(VkDevice device, VkBuffer buffer, VkAllocationCallbacks *pAllocator) {
        if (logBuffers == false)
            return;
        /* AI-Assisted: Claude Opus 5.5: syntax */
        layer_CreateLayerInfoHandle(vkdebug::InfoPtr, (uint64_t) buffer);
        /* AI-Assisted: end */
    }

    void layer_DestroyImage_before(VkDevice device, VkImage image, VkAllocationCallbacks *pAllocator) {
        if (logImages == false)
            return;
        /* AI-Assisted: Claude Opus 5.5: syntax */
        layer_CreateLayerInfoHandle(vkdebug::InfoPtr, (uint64_t) image);
        /* AI-Assisted: end */
    }

    /* AI-Assisted: Claude Sonnet 5: syntax */
    /* [xbilko03] compare layout app passes into function with layout that layer tracks, when it does not match, note that information and track that layout for image's object instead */
    void checkImageLayoutClaim(VkImage image, VkImageLayout claimedLayout, const char *callName) {
        if (!data->isImageTracked(image))
            return;
        VkImageLayout tracked = data->getImageLayout(image);
        if (tracked != claimedLayout) {
            layer_CreateLayerInfoString(vkdebug::InfoLayoutMismatch,
                                      std::string(callName) + " claims image " + addrToString((void *) image) +
                                      " is in layout " +
                                      std::to_string((int) claimedLayout) + ", tracker had " + std::to_string(
                                          (int) tracked));
            data->updateImageLayout(image, claimedLayout);
        }
    }

    /* AI-Assisted: end */
    void layer_CmdCopyImageToBuffer_before(VkCommandBuffer commandBuffer, VkImage srcImage,
                                           VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount,
                                           VkBufferImageCopy *pRegions) {
        /* AI-Assisted: Claude Sonnet 5: syntax */
        checkImageLayoutClaim(srcImage, srcImageLayout, "vkCmdCopyImageToBuffer");
        /* AI-Assisted: end */
        if (logImages == false)
            return;
        if (logBuffers == false)
            return;
        /* AI-Assisted: Claude Opus 5.5: syntax */
        layer_CreateLayerInfoHandle(vkdebug::InfoDstBuf, (uint64_t) dstBuffer);
        layer_CreateLayerInfoHandle(vkdebug::InfoSrcImg, (uint64_t) srcImage);
        /* AI-Assisted: end */
    }

    void layer_CmdCopyBufferToImage_before(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage,
                                           VkImageLayout dstImageLayout, uint32_t regionCount,
                                           VkBufferImageCopy *pRegions) {
        /* AI-Assisted: Claude Sonnet 5: syntax */
        checkImageLayoutClaim(dstImage, dstImageLayout, "vkCmdCopyBufferToImage");
        /* AI-Assisted: end */
        if (logImages == false)
            return;
        if (logBuffers == false)
            return;
        /* AI-Assisted: Claude Opus 5.5: syntax */
        layer_CreateLayerInfoHandle(vkdebug::InfoSrcBuf, (uint64_t) srcBuffer);
        layer_CreateLayerInfoHandle(vkdebug::InfoDstImg, (uint64_t) dstImage);
        /* AI-Assisted: end */
    }

    void layer_CmdCopyBuffer_before(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer,
                                    uint32_t regionCount, VkBufferCopy *pRegions) {
        if (logBuffers == false)
            return;
        /* AI-Assisted: Claude Opus 5.5: syntax */
        layer_CreateLayerInfoHandle(vkdebug::InfoSrcBuf, (uint64_t) srcBuffer);
        layer_CreateLayerInfoHandle(vkdebug::InfoDstBuf, (uint64_t) dstBuffer);
        /* AI-Assisted: end */
    }

    void layer_CmdCopyImage_before(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout,
                                   VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                                   VkImageCopy *pRegions) {
        /* AI-Assisted: Claude Sonnet 5: syntax */
        checkImageLayoutClaim(srcImage, srcImageLayout, "vkCmdCopyImage(src)");
        checkImageLayoutClaim(dstImage, dstImageLayout, "vkCmdCopyImage(dst)");
        /* AI-Assisted: end */
        if (logImages == false)
            return;
        /* AI-Assisted: Claude Opus 5.5: syntax */
        layer_CreateLayerInfoHandle(vkdebug::InfoSrcImg, (uint64_t) srcImage);
        layer_CreateLayerInfoHandle(vkdebug::InfoDstImg, (uint64_t) dstImage);
        /* AI-Assisted: end */
    }

    /* AI-Assisted: Claude Sonnet 5: syntax */
    void layer_CmdPipelineBarrier_after(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask,
                                        VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags,
                                        uint32_t memoryBarrierCount, VkMemoryBarrier *pMemoryBarriers,
                                        uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier *pBufferMemoryBarriers,
                                        uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier *pImageMemoryBarriers) {
        for (uint32_t i = 0; i < imageMemoryBarrierCount; i++)
            data->updateImageLayout(pImageMemoryBarriers[i].image, pImageMemoryBarriers[i].newLayout);
    }

    void layer_CmdPipelineBarrier2_after(VkCommandBuffer commandBuffer, VkDependencyInfo *pDependencyInfo) {
        for (uint32_t i = 0; i < pDependencyInfo->imageMemoryBarrierCount; i++)
            data->updateImageLayout(pDependencyInfo->pImageMemoryBarriers[i].image,
                                    pDependencyInfo->pImageMemoryBarriers[i].newLayout);
    }

    void layer_CmdPipelineBarrier2KHR_after(VkCommandBuffer commandBuffer, VkDependencyInfo *pDependencyInfo) {
        layer_CmdPipelineBarrier2_after(commandBuffer, pDependencyInfo);
    }

    void layer_CmdWaitEvents_after(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent *pEvents,
                                   VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
                                   uint32_t memoryBarrierCount, VkMemoryBarrier *pMemoryBarriers,
                                   uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier *pBufferMemoryBarriers,
                                   uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier *pImageMemoryBarriers) {
        for (uint32_t i = 0; i < imageMemoryBarrierCount; i++)
            data->updateImageLayout(pImageMemoryBarriers[i].image, pImageMemoryBarriers[i].newLayout);
    }

    void layer_CmdWaitEvents2_after(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent *pEvents,
                                    VkDependencyInfo *pDependencyInfos) {
        for (uint32_t e = 0; e < eventCount; e++)
            for (uint32_t i = 0; i < pDependencyInfos[e].imageMemoryBarrierCount; i++)
                data->updateImageLayout(pDependencyInfos[e].pImageMemoryBarriers[i].image,
                                        pDependencyInfos[e].pImageMemoryBarriers[i].newLayout);
    }

    void layer_CmdWaitEvents2KHR_after(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent *pEvents,
                                       VkDependencyInfo *pDependencyInfos) {
        layer_CmdWaitEvents2_after(commandBuffer, eventCount, pEvents, pDependencyInfos);
    }

    /* AI-Assisted: end */
    void layer_CreateDevice_after(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo *pCreateInfo,
                                  const VkAllocationCallbacks *pAllocator, VkDevice *pDevice) {
        data->newDevice(*pDevice);
    }

    void layer_AcquireNextImageKHR_before(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout,
                                          VkSemaphore semaphore, VkFence fence, uint32_t *pImageIndex) {
        if (frameDelim == DelimVkAcquireNextImageKHR)
            frameWarnBreak();
    }

    /* potential frame delimiter function, ensure the breakpoints and warnings are counted in case it is a frame delimiter function */
    void layer_QueuePresentKHR_before(VkQueue queue, VkPresentInfoKHR *pPresentInfo) {
        if (frameDelim == DelimVkQueuePresentKHR)
            frameWarnBreak();
    }

    /* AI-Assisted: Claude Opus 5.5: syntax */
    /* [xbilko03] timeline semaphores may get stuck when they are a part of the work that is submitted, keep track of them */
    void layer_CreateSemaphore_after(VkDevice device, VkSemaphoreCreateInfo *pCreateInfo,
                                     VkAllocationCallbacks *pAllocator, VkSemaphore *pSemaphore) {
        if (pSemaphore)
            layer_TimelineCreated(pCreateInfo, *pSemaphore);
    }

    void layer_DestroySemaphore_before(VkDevice device, VkSemaphore semaphore, VkAllocationCallbacks *pAllocator) {
        layer_TimelineDestroyed(semaphore);
    }

    void layer_SignalSemaphore_after(VkDevice device, VkSemaphoreSignalInfo *pSignalInfo) {
        layer_TimelineHostSignal(pSignalInfo);
    }

    void layer_SignalSemaphoreKHR_after(VkDevice device, VkSemaphoreSignalInfo *pSignalInfo) {
        layer_TimelineHostSignal(pSignalInfo);
    }

    void layer_QueueSubmit2_after(VkQueue queue, uint32_t submitCount, VkSubmitInfo2 *pSubmits, VkFence fence) {
        layer_TimelineSubmit2(queue, submitCount, pSubmits);
    }

    void layer_QueueSubmit2KHR_after(VkQueue queue, uint32_t submitCount, VkSubmitInfo2 *pSubmits, VkFence fence) {
        layer_TimelineSubmit2(queue, submitCount, pSubmits);
    }

    /* AI-Assisted: end */

    /* potential frame delimiter function, ensure the breakpoints and warnings are counted in case it is a frame delimiter function, also sends the VkMemory data of their corresponding bound VkBuffer and VkImage objects */
    void layer_QueueSubmit_after(VkQueue queue, uint32_t submitCount, VkSubmitInfo *pSubmits, VkFence fence) {
        /* AI-Assisted: Claude Opus 5.5: syntax */
        layer_TimelineSubmit(queue, submitCount, pSubmits);
        /* AI-Assisted: end */
        /* for each bound or mapped memory, read the current GPU data and send them to VkDebuggerApp */
        for (const auto &[key, value]: *(data->getMemoryMap())) {
            if (value.status == "mapped") {
                auto tarObject = value;
                if (value.data == NULL)
                    continue;
                /* AI-Assisted: Claude Opus 5.5: syntax */
                layer_CreateMemoryData((uint64_t) key, *tarObject.data, tarObject.size);
                /* AI-Assisted: end */
            } else if (value.status == "bound") {
                if (value.boundTo == "image") {
                    connected = false;
                    data->mapImageToBuffer(key);
                    connected = true;
                } else if (value.boundTo == "buffer") {
                    connected = false;
                    data->mapBufferToBuffer(key);
                    connected = true;
                }
                /* transfer only a part of the default copy-to VkBuffer contents */
                /* AI-Assisted: Claude Opus 5.5: syntax */
                layer_CreateMemoryData((uint64_t) key, *data->getBufferData(), value.size);
                /* AI-Assisted: end */
            }
        }

        if (frameDelim == DelimVkQueueSubmit)
            frameWarnBreak();
    }

    /* AI-Assisted: Claude Opus 5.5: syntax */
}

/* AI-Assisted: end */

/*
* Name		    : layerStatic.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file of the generated layer that is manually written in case a code generation would be too uncomfortable
*
* Based on the template available online by : Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann) sample_layer
* https ://github.com/baldurk/sample_layer/blob/master
* License       : BSD 2-Clause, see licenses/LICENSE_BSD
* Changes made by : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
bool connected = false;
/* AI-Assisted: Claude Sonnet 5: syntax */
thread_local bool skipLock = false;
/* AI-Assisted: end */

/* author of this function: Jozef Bilko (xbilko03) */
/* returns this application's name, useful when recognizing what application is this layer intercepting */
std::string getWindowName() {
    /* get the name of the program the layer is on [windows only] */
    char buf[MAX_PATH];
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    std::filesystem::path progPath(buf);
    auto filename = progPath.filename().string();
    return filename;
}

/* Authors of this function: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03); initializes the layer, it's instance functions, starts the VkDebuggerApp.exe & constructs IDT */
VK_LAYER_EXPORT VkResult VKAPI_CALL DebuggerLayer_CreateInstance(const VkInstanceCreateInfo *pCreateInfo,
                                                                 const VkAllocationCallbacks *pAllocator,
                                                                 VkInstance *pInstance) {
    /* AI-Assisted: Claude Opus 5.5: syntax */
    /* [xbilko03] starts control thread */
    layer_ControlThread_EnsureStarted();
    /* AI-Assisted: end */

    /* prevent opening VkDebugger on appUI startup */
    if (getWindowName() != "VkDebugger.exe") {
        /* fetch enviroment variables */
        try {
            /* AI-Assisted: Claude Sonnet 5: syntax */
            /* [xbilko03] path to debugger layer's configuration, from UI */
            char privateSettingsPath[MAX_PATH] = "";
            DWORD privateSettingsLen = GetEnvironmentVariableA("VKDBG_SETTINGS_PATH", privateSettingsPath,
                                                               sizeof(privateSettingsPath));

            std::string configPath;
            if (privateSettingsLen > 0) {
                configPath = privateSettingsPath;
            } else {
                char *appdata = getenv("APPDATA");
                configPath = appdata;

                /* [xbilko03] WINDOWS vk_layer_settings.txt path, these are the variables that influence the layer's functionality */
                configPath += "\\..\\Local\\LunarG\\vkconfig\\override\\vk_layer_settings.txt";
            }
            /* AI-Assisted: end */

            std::ifstream configFile(configPath);
            if (configFile.is_open()) {
                std::stringstream buffer;
                buffer << configFile.rdbuf();
                layer_AppStarter(buffer.str());
            } else {
                /* [xbilko03] could not find settings, VkDebugger layer will use default settings */
            }
        } catch (std::exception e) {
            /* [xbilko03] do nothing, VkDebugger layer will use default settings */
        }


        /* [xbilko03] connect layer to VkDebugger */
        if (layerWinsockInit(&connectSocket) == 0) {
            connected = true;
            /* AI-Assisted: Claude Opus 5.5: syntax */
            layer_ControlThread_TransportReady();

            /* [xbilko03] wait until agent is ready so no messages are lost */
            if (layer_StartAgent(globalUiPath)) {
                while (!layer_WriterReaderAlive())
                    Sleep(1);
            }
            /* AI-Assisted: end */
        }
        layer_SetEnvVariables();
    }

    VkLayerInstanceCreateInfo *layerCreateInfo = (VkLayerInstanceCreateInfo *) pCreateInfo->pNext;

    /* step through the chain of pNext until we get to the link info */
    while (layerCreateInfo && (layerCreateInfo->sType != VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO ||
                               layerCreateInfo->function != VK_LAYER_LINK_INFO)) {
        layerCreateInfo = (VkLayerInstanceCreateInfo *) layerCreateInfo->pNext;
    }

    if (layerCreateInfo == NULL) {
        /* No loader instance create info */
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    /* map this function to dispatch table */
    PFN_vkGetInstanceProcAddr gpa = layerCreateInfo->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    layerCreateInfo->u.pLayerInfo = layerCreateInfo->u.pLayerInfo->pNext;
    PFN_vkCreateInstance createFunc = (PFN_vkCreateInstance) gpa(VK_NULL_HANDLE, "vkCreateInstance");

    /* fetch our own dispatch table for the functions we need, into the next layer */
    CreateInstanceDispatch(gpa, pInstance);

    /* AI-Assisted: Claude Opus 5.5: syntax */
    layer_ThreadAt("vkCreateInstance");
    {
        scopedLock l(globalLock);

        /* [xbilko03] send call before loader */
        if (connected) {
            layer_NewCall();
            layer_CreateCallBegin(VKDBG_FN_vkCreateInstance);
            messageWriter writer;
            writer.begin(vkdebug::MessageCallInputs, VKDBG_FN_vkCreateInstance);
            writer.value(pCreateInfo->sType);
            writer.bits(pCreateInfo->pNext);
            writer.value(pCreateInfo->flags);
            if (writer.pointer(pCreateInfo->pApplicationInfo != NULL)) {
                writer.value(pCreateInfo->pApplicationInfo->sType);
                writer.bits(pCreateInfo->pApplicationInfo->pNext);
                writer.string(pCreateInfo->pApplicationInfo->pApplicationName);
                writer.value(pCreateInfo->pApplicationInfo->applicationVersion);
                writer.string(pCreateInfo->pApplicationInfo->pEngineName);
                writer.value(pCreateInfo->pApplicationInfo->engineVersion);
                writer.value(pCreateInfo->pApplicationInfo->apiVersion);
            }
            writer.value(pCreateInfo->enabledLayerCount);
            writer.bits(pCreateInfo->ppEnabledLayerNames);
            writer.value(pCreateInfo->enabledExtensionCount);
            writer.bits(pCreateInfo->ppEnabledExtensionNames);
            writer.end();
        }

        /* [xbilko03] additional call if defined by user [before taken to loader] */
#ifdef CREATEINSTANCE_BEFORE_EXEC_EXISTS
        if (connected) {
            layer_CreateInstance_before(pCreateInfo, pAllocator, pInstance);
        }
#endif
    }
    layer_CheckPaused();
    layer_HandlePendingBreak();

    /* [xbilko03] send to forward to the loader terminator code */
    layer_EnterDriver(false);
    VkResult ret = createFunc(pCreateInfo, pAllocator, pInstance);
    layer_LeaveDriver();

    {
        scopedLock l(globalLock);

        /* [xbilko03] additional call if defined by user [on the way back out of loader] */
#ifdef CREATEINSTANCE_AFTER_EXEC_EXISTS
        if (connected) {
            layer_CreateInstance_after(pCreateInfo, pAllocator, pInstance);
        }
#endif

        /* [xbilko03] send call after loader */
        if (connected) {
            messageWriter writer;
            writer.begin(vkdebug::MessageCallEnd, VKDBG_FN_vkCreateInstance);
            writer.end();
        }
    }
    layer_CheckPaused();
    layer_HandlePendingBreak();
    /* AI-Assisted: end */

    return ret;
}


/* Authors of this function: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03); destroys the layer & instance functions interception */
VK_LAYER_EXPORT void VKAPI_CALL DebuggerLayer_DestroyInstance(VkInstance instance,
                                                              const VkAllocationCallbacks *pAllocator) {
    /* AI-Assisted: Claude Opus 5.5: syntax */
    layer_ThreadAt("vkDestroyInstance");
    {
        scopedLock l(globalLock);

        /* [xbilko03] send call before loader */
        if (connected) {
            layer_NewCall();
            layer_CreateCallBegin(VKDBG_FN_vkDestroyInstance);
        }

        /* [xbilko03] additional call if defined by user [before taken to loader] */
#ifdef DESTROYINSTANCE_BEFORE_EXEC_EXISTS
        if (connected) {
            layer_DestroyInstance_before(instance, pAllocator);
        }
#endif
    }
    layer_CheckPaused();
    layer_HandlePendingBreak();

    {
        scopedLock l(globalLock);

        /* [xbilko03] Destroy layer */
        instance_dispatch.erase(GetKey(instance));

        /* [xbilko03] additional call if defined by user [on the way back out of loader] */
#ifdef DESTROYINSTANCE_AFTER_EXEC_EXISTS
        if (connected) {
            layer_DestroyInstance_after(instance, pAllocator);
        }
#endif

        /* [xbilko03] send call after loader */
        if (connected) {
            messageWriter writer;
            writer.begin(vkdebug::MessageCallEnd, VKDBG_FN_vkDestroyInstance);
            writer.end();
        }
    }
    layer_CheckPaused();
    layer_HandlePendingBreak();

    /* [xbilko03] Disconnect from the VkDebugger */
    if (connected) {
        layerWinsockExit(&connectSocket);
        connected = false;
    }
    /* AI-Assisted: end */
}


/* Authors of this function: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03); initializes the layer's device functions & DDT construction */
VK_LAYER_EXPORT VkResult VKAPI_CALL DebuggerLayer_CreateDevice(VkPhysicalDevice physicalDevice,
                                                               const VkDeviceCreateInfo *pCreateInfo,
                                                               const VkAllocationCallbacks *pAllocator,
                                                               VkDevice *pDevice) {
    VkLayerDeviceCreateInfo *layerCreateInfo = (VkLayerDeviceCreateInfo *) pCreateInfo->pNext;

    /* step through the chain of pNext until we get to the link info */
    while (layerCreateInfo && (layerCreateInfo->sType != VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO ||
                               layerCreateInfo->function != VK_LAYER_LINK_INFO)) {
        layerCreateInfo = (VkLayerDeviceCreateInfo *) layerCreateInfo->pNext;
    }

    if (layerCreateInfo == NULL) {
        /* No loader instance create info */
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    PFN_vkGetInstanceProcAddr gipa = layerCreateInfo->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr gdpa = layerCreateInfo->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    /* move chain on for next layer */
    layerCreateInfo->u.pLayerInfo = layerCreateInfo->u.pLayerInfo->pNext;

    PFN_vkCreateDevice createFunc = (PFN_vkCreateDevice) gipa(VK_NULL_HANDLE, "vkCreateDevice");

    /* AI-Assisted: Claude Opus 5.5: syntax */
    layer_ThreadAt("vkCreateDevice");
    {
        scopedLock l(globalLock);

        /* [xbilko03] send call before loader */
        if (connected) {
            layer_NewCall();
            layer_CreateCallBegin(VKDBG_FN_vkCreateDevice);
            messageWriter writer;
            writer.begin(vkdebug::MessageCallInputs, VKDBG_FN_vkCreateDevice);
            writer.value(pCreateInfo->sType);
            writer.bits(pCreateInfo->pNext);
            writer.value(pCreateInfo->flags);
            writer.value(pCreateInfo->queueCreateInfoCount);
            for (unsigned int i = 0; i < pCreateInfo->queueCreateInfoCount; i++) {
                writer.value(pCreateInfo->pQueueCreateInfos[i].sType);
                writer.value(pCreateInfo->pQueueCreateInfos[i].flags);
                writer.value(pCreateInfo->pQueueCreateInfos[i].queueFamilyIndex);
                writer.value(pCreateInfo->pQueueCreateInfos[i].queueCount);
                for (unsigned int a = 0; a < pCreateInfo->pQueueCreateInfos[i].queueCount; a++)
                    writer.value(pCreateInfo->pQueueCreateInfos[i].pQueuePriorities[a]);
            }
            writer.value(pCreateInfo->enabledLayerCount);
            writer.bits(pCreateInfo->ppEnabledLayerNames);
            writer.value(pCreateInfo->enabledExtensionCount);
            writer.bits(pCreateInfo->ppEnabledExtensionNames);
            writer.bits(pCreateInfo->pEnabledFeatures);
            writer.end();
        }

        /* [xbilko03] additional call if defined by user [before taken to loader] */
#ifdef CREATEDEVICE_BEFORE_EXEC_EXISTS
        if (connected) {
            layer_CreateDevice_before(physicalDevice, pCreateInfo, pAllocator, pDevice);
        }
#endif
    }
    layer_CheckPaused();
    layer_HandlePendingBreak();

    /* [xbilko03] send to forward to the loader terminator code */
    layer_EnterDriver(false);
    VkResult ret = createFunc(physicalDevice, pCreateInfo, pAllocator, pDevice);
    layer_LeaveDriver();

    {
        scopedLock l(globalLock);

        /* [xbilko03] additional call if defined by user [on the way back out of loader] */
#ifdef CREATEDEVICE_AFTER_EXEC_EXISTS
        if (connected) {
            layer_CreateDevice_after(physicalDevice, pCreateInfo, pAllocator, pDevice);
        }
#endif

        /* [xbilko03] send call after loader */
        if (connected) {
            messageWriter writer;
            writer.begin(vkdebug::MessageCallEnd, VKDBG_FN_vkCreateDevice);
            writer.end();
        }
    }

    /* [xbilko03] fetch our own dispatch table for the functions we need, into the next layer */
    CreateDeviceDispatch(gdpa, pDevice);

    layer_CheckPaused();
    layer_HandlePendingBreak();
    /* AI-Assisted: end */

    return ret;
}

/* Authors of this function: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03); destroys the layer's device funtions interception */
VK_LAYER_EXPORT void VKAPI_CALL DebuggerLayer_DestroyDevice(VkDevice device, const VkAllocationCallbacks *pAllocator) {
    /* AI-Assisted: Claude Opus 5.5: syntax */
    layer_ThreadAt("vkDestroyDevice");
    {
        scopedLock l(globalLock);

        /* [xbilko03] send call before loader */
        if (connected) {
            layer_NewCall();
            layer_CreateCallBegin(VKDBG_FN_vkDestroyDevice);
        }

        /* [xbilko03] additional call if defined by user [before taken to loader] */
#ifdef DESTROYDEVICE_BEFORE_EXEC_EXISTS
        if (connected) {
            layer_DestroyDevice_before(device, pAllocator);
        }
#endif
    }
    layer_CheckPaused();
    layer_HandlePendingBreak();

    {
        scopedLock l(globalLock);

        device_dispatch.erase(GetKey(device));

        /* [xbilko03] additional call if defined by user [on the way back out of loader] */
#ifdef DESTROYDEVICE_AFTER_EXEC_EXISTS
        if (connected) {
            layer_DestroyDevice_after(device, pAllocator);
        }
#endif

        /* [xbilko03] send call after loader */
        if (connected) {
            messageWriter writer;
            writer.begin(vkdebug::MessageCallEnd, VKDBG_FN_vkDestroyDevice);
            writer.end();
        }
    }
    layer_CheckPaused();
    layer_HandlePendingBreak();
    /* AI-Assisted: end */
}


/* Authors of this function: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03) */
VK_LAYER_EXPORT VkResult VKAPI_CALL DebuggerLayer_EnumerateInstanceLayerProperties(
    uint32_t *pPropertyCount, VkLayerProperties *pProperties) {
    /* AI-Assisted: Claude Opus 5.5: syntax */
    layer_ControlThread_EnsureStarted();
    layer_ThreadAt("vkEnumerateInstanceLayerProperties");
    {
        scopedLock l(globalLock);

        /* [xbilko03] send call before loader */
        if (connected) {
            layer_NewCall();
            layer_CreateCallBegin(VKDBG_FN_vkEnumerateInstanceLayerProperties);
        }

        /* [xbilko03] additional call if defined by user [before taken to loader] */
#ifdef ENUMERATEINSTANCELAYERPROPERTIES_BEFORE_EXEC_EXISTS
        if (connected) {
            layer_EnumerateInstanceLayerProperties_before(pPropertyCount, pProperties);
        }
#endif
    }
    layer_CheckPaused();
    layer_HandlePendingBreak();

    if (pPropertyCount) *pPropertyCount = 1;

    if (pProperties) {
        strcpy_s(pProperties->layerName, "vkDebuggerLayer");
        strcpy_s(pProperties->description, "https://github.com/xbilko03/ADT_VAPI");
        pProperties->implementationVersion = 1;
        pProperties->specVersion = VK_API_VERSION_1_0;
    }

    {
        scopedLock l(globalLock);

        /* [xbilko03] send call before loader */
        if (connected) {
            messageWriter writer;
            writer.begin(vkdebug::MessageCallEnd, VKDBG_FN_vkEnumerateInstanceLayerProperties);
            writer.end();
        }
    }
    layer_CheckPaused();
    layer_HandlePendingBreak();
    /* AI-Assisted: end */

    return VK_SUCCESS;
}

/* Authors of this function: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03) */
VK_LAYER_EXPORT VkResult VKAPI_CALL DebuggerLayer_EnumerateDeviceLayerProperties(
    VkPhysicalDevice physicalDevice, uint32_t *pPropertyCount, VkLayerProperties *pProperties) {
    /* AI-Assisted: Claude Opus 5.5: syntax */
    layer_ThreadAt("vkEnumerateDeviceLayerProperties");
    {
        scopedLock l(globalLock);

        /* [xbilko03] send call before loader */
        if (connected) {
            layer_NewCall();
            layer_CreateCallBegin(VKDBG_FN_vkEnumerateDeviceLayerProperties);
        }

        /* [xbilko03] additional call if defined by user [before taken to loader] */
#ifdef ENUMERATEDEVICELAYERPROPERTIES_BEFORE_EXEC_EXISTS
        if (connected) {
            layer_EnumerateDeviceLayerProperties_before(physicalDevice, pPropertyCount, pProperties);
        }
#endif
    }
    layer_CheckPaused();
    layer_HandlePendingBreak();

    if (pPropertyCount) *pPropertyCount = 1;

    if (pProperties) {
        strcpy_s(pProperties->layerName, "vkDebuggerLayer");
        strcpy_s(pProperties->description, "https://github.com/xbilko03/ADT_VAPI");
        pProperties->implementationVersion = 1;
        pProperties->specVersion = VK_API_VERSION_1_0;
    }

    {
        scopedLock l(globalLock);
        if (connected) {
            messageWriter writer;
            writer.begin(vkdebug::MessageCallEnd, VKDBG_FN_vkEnumerateDeviceLayerProperties);
            writer.end();
        }
    }
    layer_CheckPaused();
    layer_HandlePendingBreak();
    /* AI-Assisted: end */

    return VK_SUCCESS;
}

/* Authors of this function: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03) */
VK_LAYER_EXPORT VkResult VKAPI_CALL DebuggerLayer_EnumerateInstanceExtensionProperties(
    const char *pLayerName, uint32_t *pPropertyCount, VkExtensionProperties *pProperties) {
    /* AI-Assisted: Claude Opus 5.5: syntax */
    layer_ControlThread_EnsureStarted();
    layer_ThreadAt("vkEnumerateInstanceExtensionProperties");
    {
        scopedLock l(globalLock);

        /* [xbilko03] send call before loader */
        if (connected) {
            layer_NewCall();
            layer_CreateCallBegin(VKDBG_FN_vkEnumerateInstanceExtensionProperties);
        }

        /* [xbilko03] additional call if defined by user [before taken to loader] */
#ifdef ENUMERATEINSTANCEEXTENSIONPROPERTIES_BEFORE_EXEC_EXISTS
        if (connected) {
            layer_EnumerateInstanceExtensionProperties_before(pLayerName, pPropertyCount, pProperties);
        }
#endif
    }
    layer_CheckPaused();
    layer_HandlePendingBreak();

    VkResult ret = VK_SUCCESS;
    if (pLayerName == NULL || strcmp(pLayerName, "VK_LAYER_VUT_DebuggerLayer"))
        ret = VK_ERROR_LAYER_NOT_PRESENT;
        /* [xbilko03] don't expose any extensions */
    else if (pPropertyCount)
        *pPropertyCount = 0;

    {
        scopedLock l(globalLock);
        if (connected) {
            messageWriter writer;
            writer.begin(vkdebug::MessageCallEnd, VKDBG_FN_vkEnumerateInstanceExtensionProperties);
            writer.end();
        }
    }
    layer_CheckPaused();
    layer_HandlePendingBreak();
    /* AI-Assisted: end */

    return ret;
}

/* Authors of this function: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03) */
VK_LAYER_EXPORT VkResult VKAPI_CALL DebuggerLayer_EnumerateDeviceExtensionProperties(
    VkPhysicalDevice physicalDevice, const char *pLayerName, uint32_t *pPropertyCount,
    VkExtensionProperties *pProperties) {
    /* AI-Assisted: Claude Opus 5.5: syntax */
    layer_ThreadAt("vkEnumerateDeviceExtensionProperties");
    {
        scopedLock l(globalLock);

        /* [xbilko03] send call before loader */
        if (connected) {
            layer_NewCall();
            layer_CreateCallBegin(VKDBG_FN_vkEnumerateDeviceExtensionProperties);
        }

        /* [xbilko03] additional call if defined by user [before taken to loader] */
#ifdef ENUMERATEDEVICEEXTENSIONPROPERTIES_BEFORE_EXEC_EXISTS
        if (connected) {
            layer_EnumerateDeviceExtensionProperties_before(physicalDevice, pLayerName, pPropertyCount, pProperties);
        }
#endif
    }
    layer_CheckPaused();
    layer_HandlePendingBreak();

    VkResult ret = VK_SUCCESS;
    /* [xbilko03] pass through any queries that aren't to us */
    if (pLayerName == NULL || strcmp(pLayerName, "VK_LAYER_VUT_DebuggerLayer")) {
        if (physicalDevice != VK_NULL_HANDLE) {
            layer_EnterDriver(false);
            ret = instance_dispatch[GetKey(physicalDevice)].EnumerateDeviceExtensionProperties(
                physicalDevice, pLayerName, pPropertyCount, pProperties);
            layer_LeaveDriver();
        }
    }
    /* [xbilko03] don't expose any extensions */
    else if (pPropertyCount)
        *pPropertyCount = 0;

    {
        scopedLock l(globalLock);
        if (connected) {
            messageWriter writer;
            writer.begin(vkdebug::MessageCallEnd, VKDBG_FN_vkEnumerateDeviceExtensionProperties);
            writer.end();
        }
    }
    layer_CheckPaused();
    layer_HandlePendingBreak();
    /* AI-Assisted: end */

    return ret;
}


/* Authors of this next entire section's template: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03) */
/* [entirely generated] */

/*
* Name		    : layer_static.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file of the generated layer that is manually written in case a code generation would be too uncomfortable
*
* Based on the template available online by : Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann) sample_layer 
* https ://github.com/baldurk/sample_layer/blob/master 
* Changes made by : Jozef Bilko (xbilko03), supervised by Ing. Ján Peciva Ph.D.
*/
bool connected = false;
bool skipLock = false;

/* author of this function: Jozef Bilko (xbilko03) */
/* converts bool to text, useful when sending the bool parameter to VkDebuggerApp */
std::string bool_as_text(VkBool32 b)
{
    std::stringstream converter;
    converter << std::boolalpha << b;
    return converter.str();
}

/* author of this function: Jozef Bilko (xbilko03) */
/* returns this application's name, useful when recognizing what application is this layer intercepting */
std::string GetWindowName()
{
    /* get the name of the program the layer is on [windows only] */
    char buf[MAX_PATH];
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    std::filesystem::path progPath(buf);
    auto filename = progPath.filename().string();
    return filename;
}

/*
* Authors of this function: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03)
* 
* initializes the layer, it's instance functions, starts the VkDebuggerApp.exe & constructs IDT
*/
VK_LAYER_EXPORT VkResult VKAPI_CALL DebuggerLayer_CreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    /* prevent opening VkDebugger on appUI startup */
    if (GetWindowName() != "VkDebugger.exe")
    {
        /* fetch enviroment variables */
        try
        {
            char* appdata = getenv("APPDATA");
            std::string configPath = appdata;

            /* WINDOWS vk_layer_settings.txt path, these are the variables that influence the layer's functionality */
            configPath += "\\..\\Local\\LunarG\\vkconfig\\override\\vk_layer_settings.txt";

            std::ifstream configFile(configPath);
            if (configFile.is_open())
            {
                std::stringstream buffer;
                buffer << configFile.rdbuf();
                layer_AppStarter(buffer.str());
            }
            else
            {
                /* could not find settings, vkDetails layer will use default settings */
            }

        }
        catch (std::exception e)
        {
            /* do nothing, vkDetails layer will use default settings */
        }


        /* connect layer to vkDetails */
        if (layerWinsockInit(&ConnectSocket) == 0)
        {
            connected = true;
        }
        layer_SetEnvVariables();
    }

    VkLayerInstanceCreateInfo* layerCreateInfo = (VkLayerInstanceCreateInfo*)pCreateInfo->pNext;

    /* step through the chain of pNext until we get to the link info */
    while (layerCreateInfo && (layerCreateInfo->sType != VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO || layerCreateInfo->function != VK_LAYER_LINK_INFO))
    {
        layerCreateInfo = (VkLayerInstanceCreateInfo*)layerCreateInfo->pNext;
    }

    if (layerCreateInfo == NULL)
    {
        /* No loader instance create info */
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    /* map this function to dispatch table */
    PFN_vkGetInstanceProcAddr gpa = layerCreateInfo->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    layerCreateInfo->u.pLayerInfo = layerCreateInfo->u.pLayerInfo->pNext;
    PFN_vkCreateInstance createFunc = (PFN_vkCreateInstance)gpa(VK_NULL_HANDLE, "vkCreateInstance");

    /* fetch our own dispatch table for the functions we need, into the next layer */
    CreateInstanceDispatch(gpa, pInstance);

    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "begin_vkCreateInstance!");
    }

    /* additional call if defined by user [before taken to loader] */
    #ifdef CREATEINSTANCE_BEFORE_EXEC_EXISTS
    if (connected) {
        layer_CreateInstance_before(pCreateInfo, pAllocator, pInstance);
    }
    #endif

    /* send to forward to the loader terminator code */
    VkResult ret = createFunc(pCreateInfo, pAllocator, pInstance);

    /* additional call if defined by user [on the way back out of loader] */
    #ifdef CREATEINSTANCE_AFTER_EXEC_EXISTS
    if (connected) {
        layer_CreateInstance_after(pCreateInfo, pAllocator, pInstance);
    }
    #endif 

    /* send call after loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "pCreateInfo->sType=" + std::to_string(pCreateInfo->sType) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->pNext=" + ptrToString((void**)pCreateInfo->pNext) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->flags=" + std::to_string(pCreateInfo->flags) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->pApplicationInfo->sType=" + std::to_string(pCreateInfo->pApplicationInfo->sType) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->pApplicationInfo->pNext=" + ptrToString((void**)pCreateInfo->pApplicationInfo->pNext) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->pApplicationInfo->pApplicationName=" + charToString((char*)pCreateInfo->pApplicationInfo->pApplicationName) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->pApplicationInfo->applicationVersion=" + std::to_string(pCreateInfo->pApplicationInfo->applicationVersion) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->pApplicationInfo->pEngineName=" + charToString((char*)pCreateInfo->pApplicationInfo->pEngineName) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->pApplicationInfo->engineVersion=" + std::to_string(pCreateInfo->pApplicationInfo->engineVersion) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->pApplicationInfo->apiVersion=" + std::to_string(pCreateInfo->pApplicationInfo->apiVersion) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->enabledLayerCount=" + std::to_string(pCreateInfo->enabledLayerCount) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->ppEnabledLayerNames=" + charToString((char*)pCreateInfo->ppEnabledLayerNames) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->enabledExtensionCount=" + std::to_string(pCreateInfo->enabledExtensionCount) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->ppEnabledExtensionNames=" + charToString((char*)pCreateInfo->ppEnabledExtensionNames) + '!');
        
        winsockSendToUI(&ConnectSocket, "end_vkCreateInstance!");

        if (callEveryBreak || callAtBreak)
            layer_newCall();
    }

    return ret;
}


/*
* Authors of this function: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03)
*
* destroys the layer & instance functions interception
*/
VK_LAYER_EXPORT void VKAPI_CALL DebuggerLayer_DestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator)
{
    scoped_lock l(global_lock);

    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "begin_vkDestroyInstance!");
    }

    /* additional call if defined by user [before taken to loader] */
    #ifdef DESTROYINSTANCE_BEFORE_EXEC_EXISTS
    if (connected) {
        layer_DestroyInstance_before(instance, pAllocator);
    }
    #endif

    /* Destroy layer */
    instance_dispatch.erase(GetKey(instance));

    /* additional call if defined by user [on the way back out of loader] */
    #ifdef DESTROYINSTANCE_AFTER_EXEC_EXISTS
    if (connected) {
        layer_DestroyInstance_after(instance, pAllocator);
    }
    #endif 

    /* send call after loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "end_vkDestroyInstance!");

        if (callEveryBreak || callAtBreak)
            layer_newCall();
    }

    /* Disconnect from the VkDetails */
    layerWinsockExit(&ConnectSocket);
}


/*
* Authors of this function: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03)
*
* initializes the layer's device functions & DDT construction
*/
VK_LAYER_EXPORT VkResult VKAPI_CALL DebuggerLayer_CreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    VkLayerDeviceCreateInfo* layerCreateInfo = (VkLayerDeviceCreateInfo*)pCreateInfo->pNext;

    /* step through the chain of pNext until we get to the link info */
    while (layerCreateInfo && (layerCreateInfo->sType != VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO ||
        layerCreateInfo->function != VK_LAYER_LINK_INFO))
    {
        layerCreateInfo = (VkLayerDeviceCreateInfo*)layerCreateInfo->pNext;
    }

    if (layerCreateInfo == NULL)
    {
        /* No loader instance create info */
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    PFN_vkGetInstanceProcAddr gipa = layerCreateInfo->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr gdpa = layerCreateInfo->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    /* move chain on for next layer */
    layerCreateInfo->u.pLayerInfo = layerCreateInfo->u.pLayerInfo->pNext;

    PFN_vkCreateDevice createFunc = (PFN_vkCreateDevice)gipa(VK_NULL_HANDLE, "vkCreateDevice");

    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "begin_vkCreateDevice!");
    }

    /* additional call if defined by user [before taken to loader] */
    #ifdef CREATEDEVICE_BEFORE_EXEC_EXISTS
    if (connected) {
        layer_CreateDevice_before(physicalDevice, pCreateInfo, pAllocator, pDevice);
    }
    #endif
        
    /* send to forward to the loader terminator code */
    VkResult ret = createFunc(physicalDevice, pCreateInfo, pAllocator, pDevice);

    /* additional call if defined by user [on the way back out of loader] */
    #ifdef CREATEDEVICE_AFTER_EXEC_EXISTS
    if (connected) {
        layer_CreateDevice_after(physicalDevice, pCreateInfo, pAllocator, pDevice);
    }
    #endif

    /* send call after loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "pCreateInfo->sType=" + std::to_string(pCreateInfo->sType) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->pNext=" + ptrToString((void**)pCreateInfo->pNext) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->flags=" + std::to_string(pCreateInfo->flags) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->queueCreateInfoCount=" + std::to_string(pCreateInfo->queueCreateInfoCount) + '!');
        for (unsigned int i = 0; i < pCreateInfo->queueCreateInfoCount; i++)
        {
            winsockSendToUI(&ConnectSocket, "pCreateInfo->pQueueCreateInfos" + std::to_string(i) + "->sType=" + std::to_string(pCreateInfo->pQueueCreateInfos[i].sType) + '!');
            winsockSendToUI(&ConnectSocket, "pCreateInfo->pQueueCreateInfos" + std::to_string(i) + "->flags=" + std::to_string(pCreateInfo->pQueueCreateInfos[i].flags) + '!');
            winsockSendToUI(&ConnectSocket, "pCreateInfo->pQueueCreateInfos" + std::to_string(i) + "->queueFamilyIndex=" + std::to_string(pCreateInfo->pQueueCreateInfos[i].queueFamilyIndex) + '!');
            winsockSendToUI(&ConnectSocket, "pCreateInfo->pQueueCreateInfos" + std::to_string(i) + "->queueCount=" + std::to_string(pCreateInfo->pQueueCreateInfos[i].queueCount) + '!');
            for (unsigned int a = 0; a < pCreateInfo->pQueueCreateInfos[i].queueCount; a++)
                winsockSendToUI(&ConnectSocket, "pCreateInfo->pQueueCreateInfos" + std::to_string(i) + "->pQueuePriorities" + std::to_string(a) + "=" + std::to_string(pCreateInfo->pQueueCreateInfos[i].pQueuePriorities[a]) + '!');
        }
        winsockSendToUI(&ConnectSocket, "pCreateInfo->enabledLayerCount=" + std::to_string(pCreateInfo->enabledLayerCount) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->ppEnabledLayerNames=" + charToString((char*)pCreateInfo->ppEnabledLayerNames) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->enabledExtensionCount=" + std::to_string(pCreateInfo->enabledExtensionCount) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->ppEnabledExtensionNames=" + charToString((char*)pCreateInfo->ppEnabledExtensionNames) + '!');
        winsockSendToUI(&ConnectSocket, "pCreateInfo->pNext=" + ptrToString((void**)pCreateInfo->pEnabledFeatures) + '!');
        winsockSendToUI(&ConnectSocket, "end_vkCreateDevice!");

        if (callEveryBreak || callAtBreak)
            layer_newCall();
    }

    /* fetch our own dispatch table for the functions we need, into the next layer */
    CreateDeviceDispatch(gdpa, pDevice);

    return ret;
}

/*
* Authors of this function: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03)
*
* destroys the layer's device funtions interception
*/
VK_LAYER_EXPORT void VKAPI_CALL DebuggerLayer_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator)
{
    scoped_lock l(global_lock);
    
    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "begin_vkDestroyDevice!");
    }

    /* additional call if defined by user [before taken to loader] */
    #ifdef DESTROYDEVICE_BEFORE_EXEC_EXISTS
    if (connected) {
        layer_DestroyDevice_before(device, pAllocator);
    }
    #endif

    device_dispatch.erase(GetKey(device));

    /* additional call if defined by user [on the way back out of loader] */
    #ifdef DESTROYDEVICE_AFTER_EXEC_EXISTS
    if (connected) {
        layer_DestroyDevice_after(device, pAllocator);
    }
    #endif 

    /* send call after loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "end_vkDestroyDevice!");

        if (callEveryBreak || callAtBreak)
            layer_newCall();
    }
}


/* Authors of this function: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03) */
VK_LAYER_EXPORT VkResult VKAPI_CALL DebuggerLayer_EnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "begin_vkEnumerateInstanceLayerProperties!");
    }

    /* additional call if defined by user [before taken to loader] */
#ifdef ENUMERATEINSTANCELAYERPROPERTIES_BEFORE_EXEC_EXISTS
    if (connected) {
        layer_EnumerateInstanceLayerProperties_before(pPropertyCount, pProperties);
    }
#endif
    if (pPropertyCount) *pPropertyCount = 1;

    if (pProperties)
    {
        strcpy_s(pProperties->layerName, "vkDebuggerLayer");
        strcpy_s(pProperties->description, "https://github.com/xbilko03/ADT_VAPI");
        pProperties->implementationVersion = 1;
        pProperties->specVersion = VK_API_VERSION_1_0;
    }

    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "end_vkEnumerateInstanceLayerProperties!");
        if (callEveryBreak || callAtBreak)
            layer_newCall();
    }

    return VK_SUCCESS;
}

/* Authors of this function: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03) */
VK_LAYER_EXPORT VkResult VKAPI_CALL DebuggerLayer_EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{

    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "begin_vkEnumerateDeviceLayerProperties!");
    }

    /* additional call if defined by user [before taken to loader] */
    #ifdef ENUMERATEDEVICELAYERPROPERTIES_BEFORE_EXEC_EXISTS
    if (connected) {
        layer_EnumerateDeviceLayerProperties_before(physicalDevice, pPropertyCount, pProperties);
    }
    #endif

    if (pPropertyCount) *pPropertyCount = 1;

    if (pProperties)
    {
        strcpy_s(pProperties->layerName, "vkDebuggerLayer");
        strcpy_s(pProperties->description, "https://github.com/xbilko03/ADT_VAPI");
        pProperties->implementationVersion = 1;
        pProperties->specVersion = VK_API_VERSION_1_0;
    }

    if (connected) {
        winsockSendToUI(&ConnectSocket, "end_vkEnumerateDeviceLayerProperties!");
        if (callEveryBreak || callAtBreak)
            layer_newCall();
    }

    return VK_SUCCESS;
}

/* Authors of this function: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03) */
VK_LAYER_EXPORT VkResult VKAPI_CALL DebuggerLayer_EnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "begin_vkEnumerateInstanceExtensionProperties!");
    }

    /* additional call if defined by user [before taken to loader] */
    #ifdef ENUMERATEINSTANCEEXTENSIONPROPERTIES_BEFORE_EXEC_EXISTS
    if (connected) {
        layer_EnumerateInstanceExtensionProperties_before(pLayerName, pPropertyCount, pProperties);
    }
    #endif

    if (connected) {
        winsockSendToUI(&ConnectSocket, "end_vkEnumerateInstanceExtensionProperties!");

        if (callEveryBreak || callAtBreak)
            layer_newCall();
    }

    if (pLayerName == NULL || strcmp(pLayerName, "VK_LAYER_VUT_DebuggerLayer"))
        return VK_ERROR_LAYER_NOT_PRESENT;

    /* don't expose any extensions */
    if (pPropertyCount) *pPropertyCount = 0;
    return VK_SUCCESS;
}

/* Authors of this function: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03) */
VK_LAYER_EXPORT VkResult VKAPI_CALL DebuggerLayer_EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{

    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "begin_vkEnumerateDeviceExtensionProperties!");
    }

    /* additional call if defined by user [before taken to loader] */
    #ifdef ENUMERATEDEVICEEXTENSIONPROPERTIES_BEFORE_EXEC_EXISTS
    if (connected) {
        layer_EnumerateDeviceExtensionProperties_before(physicalDevice, pLayerName, pPropertyCount, pProperties);
    }
    #endif

    /* send call before loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "end_vkEnumerateDeviceExtensionProperties!");


        if (callEveryBreak || callAtBreak)
            layer_newCall();
    }


    /*  pass through any queries that aren't to us */
    if (pLayerName == NULL || strcmp(pLayerName, "VK_LAYER_VUT_DebuggerLayer"))
    {
        if (physicalDevice == VK_NULL_HANDLE)
            return VK_SUCCESS;

        scoped_lock l(global_lock);
        return instance_dispatch[GetKey(physicalDevice)].EnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pPropertyCount, pProperties);
    }

    /* don't expose any extensions */
    if (pPropertyCount) *pPropertyCount = 0;


    return VK_SUCCESS;
}


/* Authors of this next entire section's template: Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann), expanded upon by: Jozef Bilko (xbilko03) */
/* [entirely generated] */
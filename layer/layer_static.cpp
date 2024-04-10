
/* actual data we're recording in this layer */

/*struct CommandStats
{
    uint32_t drawCount = 0, instanceCount = 0, vertCount = 0;
};
std::map<VkCommandBuffer, CommandStats> commandbuffer_stats;
*/
bool connected = false;

std::string bool_as_text(VkBool32 b)
{
    std::stringstream converter;
    converter << std::boolalpha << b;   // flag boolalpha calls converter.setf(std::ios_base::boolalpha)
    return converter.str();
}

std::string ptrToString(void* input)
{
    if (input == NULL)
        return "NULL";
    std::stringstream s;
    s << input;
    return s.str();
}
std::string charToString(auto* input)
{
    if (input == NULL)
        return "NULL";
    std::stringstream s;
    s << *input;
    return s.str();
}
std::string GetWindowName()
{
    /* get the name of the program the layer is on [windows only] */
    char buf[MAX_PATH];
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    std::filesystem::path progPath(buf);
    auto filename = progPath.filename().string();
    return filename;
}

/* Layer init and shutdown */
VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    /* create new process */
    STARTUPINFO info = { sizeof(info) };
    PROCESS_INFORMATION processInfo;

    /* get the name of the program the layer is on [windows only] */
    char buf[MAX_PATH];
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    std::filesystem::path progPath(buf);
    std::string filename = progPath.filename().string();

    /* prevent opening vkDetails on appUI startup */
    if (GetWindowName() != "vkDetails.exe")
    {
        /* open new vkDetails window */
        CreateProcess("C:\\Users\\jozef\\Desktop\\vk details\\out\\build\\x64-debug\\vkDetails.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &info, &processInfo);
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);

        /* connect layer to vkDetails */
        if (layerWinsockInit(&ConnectSocket) == 0)
        {
            connected = true;
        }
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

    /* send to loader */
    VkResult ret = createFunc(pCreateInfo, pAllocator, pInstance);

    /* additional call if defined by user [on the way back out of loader] */
    #ifdef CREATEINSTANCE_AFTER_EXEC_EXISTS
    if (connected) {
        layer_CreateInstance_after(pCreateInfo, pAllocator, pInstance);
    }
    #endif 

    /* send call after loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "end_vkCreateInstance!");
    }

    return ret;
}

VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator)
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
    }

    /* Disconnect from the VkDetails */
    layerWinsockExit(&ConnectSocket);
}

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
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

    /* send to loader */
    VkResult ret = createFunc(physicalDevice, pCreateInfo, pAllocator, pDevice);

    /* additional call if defined by user [on the way back out of loader] */
    #ifdef CREATEDEVICE_AFTER_EXEC_EXISTS
    if (connected) {
        layer_CreateDevice_after(physicalDevice, pCreateInfo, pAllocator, pDevice);
    }
    #endif

    /* send call after loader */
    if (connected) {
        winsockSendToUI(&ConnectSocket, "end_vkCreateDevice!");
    }



    /* fetch our own dispatch table for the functions we need, into the next layer */
    CreateDeviceDispatch(gdpa, pDevice);

    return ret;
}

VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator)
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
    }
}

/* Enumeration function */
VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties)
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
        strcpy_s(pProperties->layerName, "vkDetailsLayer");
        strcpy_s(pProperties->description, "https://github.com/xbilko03/ADT_VAPI");
        pProperties->implementationVersion = 1;
        pProperties->specVersion = VK_API_VERSION_1_0;
    }

    return VK_SUCCESS;
}

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
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
        strcpy_s(pProperties->layerName, "vkDetailsLayer");
        strcpy_s(pProperties->description, "https://github.com/xbilko03/ADT_VAPI");
        pProperties->implementationVersion = 1;
        pProperties->specVersion = VK_API_VERSION_1_0;
    }
    return VK_SUCCESS;
}

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
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

    if (pLayerName == NULL || strcmp(pLayerName, "VK_LAYER_SAMPLE_DetailsLayer"))
        return VK_ERROR_LAYER_NOT_PRESENT;

    /* don't expose any extensions */
    if (pPropertyCount) *pPropertyCount = 0;
    return VK_SUCCESS;
}

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
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

    /*  pass through any queries that aren't to us */
    if (pLayerName == NULL || strcmp(pLayerName, "VK_LAYER_SAMPLE_DetailsLayer"))
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

/* Generated part */

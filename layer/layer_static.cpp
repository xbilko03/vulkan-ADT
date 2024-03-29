
/* actual data we're recording in this layer */
struct CommandStats
{
    uint32_t drawCount = 0, instanceCount = 0, vertCount = 0;
};
std::map<VkCommandBuffer, CommandStats> commandbuffer_stats;

/* Layer init and shutdown */

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
#ifdef CREATEINSTANCE_EXISTS
    layer_CreateInstance(pCreateInfo, pAllocator, pInstance);
#endif

    VkLayerInstanceCreateInfo* layerCreateInfo = (VkLayerInstanceCreateInfo*)pCreateInfo->pNext;

    /* step through the chain of pNext until we get to the link info */
    while (layerCreateInfo && (layerCreateInfo->sType != VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO ||
        layerCreateInfo->function != VK_LAYER_LINK_INFO))
    {
        layerCreateInfo = (VkLayerInstanceCreateInfo*)layerCreateInfo->pNext;
    }

    if (layerCreateInfo == NULL)
    {
        /* No loader instance create info */
        return VK_ERROR_INITIALIZATION_FAILED;
    }



    PFN_vkGetInstanceProcAddr gpa = layerCreateInfo->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    layerCreateInfo->u.pLayerInfo = layerCreateInfo->u.pLayerInfo->pNext;
    PFN_vkCreateInstance createFunc = (PFN_vkCreateInstance)gpa(VK_NULL_HANDLE, "vkCreateInstance");

    /* fetch our own dispatch table for the functions we need, into the next layer */
    CreateInstanceDispatch(gpa, pInstance);

    VkResult ret = createFunc(pCreateInfo, pAllocator, pInstance);

    return VK_SUCCESS;
}

VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator)
{
    scoped_lock l(global_lock);
    //winsockSendToUI(&ConnectSocket, "vkDestroyInstance");

    /* Stop communication */
    //layerWinsockExit(&ConnectSocket);
    instance_dispatch.erase(GetKey(instance));

}

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_CreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    VkLayerDeviceCreateInfo* layerCreateInfo = (VkLayerDeviceCreateInfo*)pCreateInfo->pNext;
    //winsockSendToUI(&ConnectSocket, "vkCreateDevice");

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

    VkResult ret = createFunc(physicalDevice, pCreateInfo, pAllocator, pDevice);

    /* fetch our own dispatch table for the functions we need, into the next layer */
    CreateDeviceDispatch(gdpa, pDevice);

    return VK_SUCCESS;
}

VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator)
{
    scoped_lock l(global_lock);
    //winsockSendToUI(&ConnectSocket, "vkDestroyDevice");
    device_dispatch.erase(GetKey(device));
}

/* Enumeration function */
VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumerateInstanceLayerProperties(uint32_t* pPropertyCount,
    VkLayerProperties* pProperties)
{
    if (pPropertyCount) *pPropertyCount = 1;
    //winsockSendToUI(&ConnectSocket, "vkEnumerateInstanceLayerProperties");

    if (pProperties)
    {
        strcpy_s(pProperties->layerName, "vkDetailsLayer");
        strcpy_s(pProperties->description, "https://github.com/xbilko03/ADT_VAPI");
        pProperties->implementationVersion = 1;
        pProperties->specVersion = VK_API_VERSION_1_0;
    }

    return VK_SUCCESS;
}

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumerateDeviceLayerProperties(
    VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    //winsockSendToUI(&ConnectSocket, "vkEnumerateDeviceLayerProperties");
    return DetailsLayer_EnumerateInstanceLayerProperties(pPropertyCount, pProperties);
}

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumerateInstanceExtensionProperties(
    const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    //winsockSendToUI(&ConnectSocket, "vkEnumerateInstanceExtensionProperties");
    if (pLayerName == NULL || strcmp(pLayerName, "VK_LAYER_SAMPLE_DetailsLayer"))
        return VK_ERROR_LAYER_NOT_PRESENT;

    /* don't expose any extensions */
    if (pPropertyCount) *pPropertyCount = 0;
    return VK_SUCCESS;
}

VK_LAYER_EXPORT VkResult VKAPI_CALL DetailsLayer_EnumerateDeviceExtensionProperties(
    VkPhysicalDevice physicalDevice, const char* pLayerName,
    uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    //winsockSendToUI(&ConnectSocket, "vkEnumerateDeviceExtensionProperties");
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





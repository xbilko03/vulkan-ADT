/*
* Name		    : vkdbgDecoderStatic.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : [xbilko03] Source file for decoding logic of binary messages written by the layer (for functions written within layerStatic.cpp)
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
/* AI-Assisted: Claude Opus 5.5: syntax, entire file */
#include "../../include/communication/vkdbgDecoder.hpp"
#include "../../../transport/include/vkdbgFunctionsGenerated.hpp"

namespace vkdebug::ui {
    bool decodeCallInputsStatic(uint16_t function, const uint8_t* payload, size_t size, decodedFields& out)
    {
        messageReader r(payload, size);
        switch (function)
        {
        case VKDBG_FN_vkCreateInstance:
            r.value<VkStructureType>(out, "pCreateInfo->sType");
            r.bits<const void*>(out, "pCreateInfo->pNext");
            r.value<VkInstanceCreateFlags>(out, "pCreateInfo->flags");
            if (r.pointer(out, "pCreateInfo->pApplicationInfo"))
            {
                r.value<VkStructureType>(out, "pCreateInfo->pApplicationInfo->sType");
                r.bits<const void*>(out, "pCreateInfo->pApplicationInfo->pNext");
                r.string(out, "pCreateInfo->pApplicationInfo->pApplicationName");
                r.value<uint32_t>(out, "pCreateInfo->pApplicationInfo->applicationVersion");
                r.string(out, "pCreateInfo->pApplicationInfo->pEngineName");
                r.value<uint32_t>(out, "pCreateInfo->pApplicationInfo->engineVersion");
                r.value<uint32_t>(out, "pCreateInfo->pApplicationInfo->apiVersion");
            }
            r.value<uint32_t>(out, "pCreateInfo->enabledLayerCount");
            r.bits<const char* const*>(out, "pCreateInfo->ppEnabledLayerNames");
            r.value<uint32_t>(out, "pCreateInfo->enabledExtensionCount");
            r.bits<const char* const*>(out, "pCreateInfo->ppEnabledExtensionNames");
            break;

        case VKDBG_FN_vkCreateDevice:
        {
            r.value<VkStructureType>(out, "pCreateInfo->sType");
            r.bits<const void*>(out, "pCreateInfo->pNext");
            r.value<VkDeviceCreateFlags>(out, "pCreateInfo->flags");
            uint32_t queueInfos = r.count<uint32_t>(out, "pCreateInfo->queueCreateInfoCount");
            for (uint32_t i = 0; i < queueInfos && r.ok(); i++)
            {
                std::string prefix = "pCreateInfo->pQueueCreateInfos" + std::to_string(i) + "->";
                r.value<VkStructureType>(out, (prefix + "sType").c_str());
                r.value<VkDeviceQueueCreateFlags>(out, (prefix + "flags").c_str());
                r.value<uint32_t>(out, (prefix + "queueFamilyIndex").c_str());
                uint32_t queues = r.count<uint32_t>(out, (prefix + "queueCount").c_str());
                for (uint32_t a = 0; a < queues && r.ok(); a++)
                    r.value<float>(out, (prefix + "pQueuePriorities" + std::to_string(a)).c_str());
            }
            r.value<uint32_t>(out, "pCreateInfo->enabledLayerCount");
            r.bits<const char* const*>(out, "pCreateInfo->ppEnabledLayerNames");
            r.value<uint32_t>(out, "pCreateInfo->enabledExtensionCount");
            r.bits<const char* const*>(out, "pCreateInfo->ppEnabledExtensionNames");
            r.bits<const VkPhysicalDeviceFeatures*>(out, "pCreateInfo->pEnabledFeatures");
            break;
        }

        case VKDBG_FN_vkDestroyInstance:
        case VKDBG_FN_vkDestroyDevice:
        case VKDBG_FN_vkEnumerateInstanceLayerProperties:
        case VKDBG_FN_vkEnumerateDeviceLayerProperties:
        case VKDBG_FN_vkEnumerateInstanceExtensionProperties:
        case VKDBG_FN_vkEnumerateDeviceExtensionProperties:
            break;

        default:
            return false;
        }
        return r.ok();
    }

    bool decodeCallEndStatic(uint16_t function, const uint8_t* payload, size_t size, decodedFields& out)
    {
        messageReader r(payload, size);
        switch (function)
        {
        case VKDBG_FN_vkCreateInstance:
        case VKDBG_FN_vkCreateDevice:
        case VKDBG_FN_vkDestroyInstance:
        case VKDBG_FN_vkDestroyDevice:
        case VKDBG_FN_vkEnumerateInstanceLayerProperties:
        case VKDBG_FN_vkEnumerateDeviceLayerProperties:
        case VKDBG_FN_vkEnumerateInstanceExtensionProperties:
        case VKDBG_FN_vkEnumerateDeviceExtensionProperties:
            break;

        default:
            return false;
        }
        return r.ok();
    }
}

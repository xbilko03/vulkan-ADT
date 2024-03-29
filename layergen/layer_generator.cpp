#include "layer_generator.hpp"

#include <iostream>
#include <fstream>
#include <list>
#include <filesystem>
#include <algorithm>

namespace laygen
{
    std::string LayerGenerator::GetGuardString(std::string commandName, std::list<XmlParser::cGuard> cmdGuard)
    {
        for (auto guard : cmdGuard)
        {
            for (auto cmd : guard.commandList)
            {
                if (strcmp(commandName.c_str(), cmd.c_str()) == 0)
                {
                    return guard.guard;
                }
            }
        }
        return "";
    }
    void LayerGenerator::generateTableInstance()
    {

        /* Instance commands */
        dispatchTableFile << INSTANCE_TABLE_HEADER;

        for (auto item : xmlp.commandListInstances)
        {
            /* if there is a specific command user wants to ignore, skip it */
            if (std::find(ignoreCmdList.begin(), ignoreCmdList.end(), item.functName) != ignoreCmdList.end())
            {
                continue;
            }

            bool printGuard = false;
            std::string guardContent;
            if (cmdPlatformProtection)
            {
                guardContent = GetGuardString(item.functName, xmlp.cmdGuard);
                if (strcmp(guardContent.c_str(), "") != 0)
                    printGuard = true;
            }
            if (printGuard)
                dispatchTableFile << "\n#if defined(" << GetGuardString(item.functName, xmlp.cmdGuard) << ")";
            dispatchTableFile << "\n\tPFN_" + item.functName + " " + item.functName.substr(2, item.functName.size()) + ";";
            if (printGuard)
                dispatchTableFile << "\n#endif";
        }

        dispatchTableFile << INSTANCE_TABLE_FOOTER;

    }
    void LayerGenerator::generateTableDevice()
    {
        /* Device commands */

        dispatchTableFile << DEVICE_TABLE_HEADER;

        for (auto item : xmlp.commandListDevices)
        {
            /* if there is a specific command user wants to ignore, skip it */
            if (std::find(ignoreCmdList.begin(), ignoreCmdList.end(), item.functName) != ignoreCmdList.end())
            {
                continue;
            }
            bool printGuard = false;
            std::string guardContent;
            if (cmdPlatformProtection)
            {
                guardContent = GetGuardString(item.functName, xmlp.cmdGuard);
                if (strcmp(guardContent.c_str(), "") != 0)
                    printGuard = true;
            }
            if (printGuard)
                dispatchTableFile << "\n#if defined(" << GetGuardString(item.functName, xmlp.cmdGuard) << ")";
            dispatchTableFile << "\n\tPFN_" + item.functName + " " + item.functName.substr(2, item.functName.size()) + ";";
            if (printGuard)
                dispatchTableFile << "\n#endif";
        }

        dispatchTableFile << DEVICE_TABLE_FOOTER;

    }
    void LayerGenerator::generateHeaders()
    {
        generatedLayerFile << "#include \"layer.hpp\"" << std::endl;
        generatedLayerFile << "#include <mutex>" << std::endl;
        generatedLayerFile << "#include <ws2tcpip.h>" << std::endl;
        generatedLayerFile << "#include \"../layergen/vk_layer_dispatch_table.h\"" << std::endl;
        generatedLayerFile << "#include \"../vklayer/vk_layer_table.h\"" << std::endl << std::endl;

        generatedLayerFile << "std::map<void*, VkLayerInstanceDispatchTable> instance_dispatch;" << std::endl;
        generatedLayerFile << "std::map<void*, VkLayerDispatchTable> device_dispatch;" << std::endl;       
        generatedLayerFile << "std::mutex global_lock;" << std::endl;
        generatedLayerFile << "typedef std::lock_guard<std::mutex> scoped_lock;" << std::endl;

        generatedLayerFile << "template<typename DispatchableType>" << std::endl;
        generatedLayerFile << "void* GetKey(DispatchableType inst)" << std::endl;
        generatedLayerFile << "{ return *(void**)inst; }" << std::endl;
    }
    void LayerGenerator::generateCmdDeclaration(std::list<XmlParser::Command> inputCmdList, std::string cmdListType)
    {
        /* list of instance inputCmdList cmd names */
        for (auto item : inputCmdList)
        {
            /* ignores */
            if (std::find(ignoreCmdList.begin(), ignoreCmdList.end(), item.functName) != ignoreCmdList.end())
            {
                continue;
            }

            bool printGuard = false;
            std::string guardContent;
            if (cmdPlatformProtection)
            {
                guardContent = GetGuardString(item.functName, xmlp.cmdGuard);
                if (strcmp(guardContent.c_str(), "") != 0)
                    printGuard = true;
            }

            if (printGuard)
                generatedLayerFile << "#if defined(" << GetGuardString(item.functName, xmlp.cmdGuard) << ")" << std::endl;

            //(PFN_vkBeginCommandBuffer)gdpa(*pDevice, "vkBeginCommandBuffer");

            generatedLayerFile << "dispatchTable." << item.functName.substr(2,item.functName.size());
            
            generatedLayerFile << " = (PFN_" << item.functName << ")";
            if (strcmp(cmdListType.c_str(), "instance") == 0)
                generatedLayerFile << "gpa";
            else
                generatedLayerFile << "gdpa";
            generatedLayerFile << "(*";

            if (strcmp(cmdListType.c_str(), "instance") == 0)
                generatedLayerFile << "pInstance";
            else
                generatedLayerFile << "pDevice";
            generatedLayerFile << ", \"" << item.functName << "\");" << std::endl;
            if (printGuard)
                generatedLayerFile << "#endif" << std::endl;
        }
    }
    void LayerGenerator::generateCmdProcCalls(std::list<XmlParser::Command> inputCmdList)
    {
        /* list of instance inputCmdList cmd names */
        for (auto item : inputCmdList)
        {
            /* ignores */
            if (std::find(ignoreCmdList.begin(), ignoreCmdList.end(), item.functName) != ignoreCmdList.end())
            {
                continue;
            }

            bool printGuard = false;
            std::string guardContent;
            if (cmdPlatformProtection)
            {
                guardContent = GetGuardString(item.functName, xmlp.cmdGuard);
                if (strcmp(guardContent.c_str(), "") != 0)
                    printGuard = true;
            }



            if (printGuard)
                generatedLayerFile << "#if defined(" << GetGuardString(item.functName, xmlp.cmdGuard) << ")" << std::endl;
            generatedLayerFile << "GETPROCADDR(" << item.functName.substr(2, item.functName.size()) << ");" << std::endl;
            if (printGuard)
                generatedLayerFile << "#endif" << std::endl;
        }
    }
    void LayerGenerator::generateGenLayerFooter(std::string cmdListType)
    {
        generatedLayerFile << "VK_LAYER_EXPORT PFN_vkVoidFunction VKAPI_CALL DetailsLayer_Get";
        if (strcmp(cmdListType.c_str(), "instance") == 0)
        {
            generatedLayerFile << "InstanceProcAddr(VkInstance instance, const char* pName) {" << std::endl;
            generateCmdProcCalls(xmlp.commandListInstances);
            generateCmdProcCalls(xmlp.commandListDevices);
        }
        else
        {
            generatedLayerFile << "DeviceProcAddr(VkDevice device, const char* pName) {" << std::endl;
            generateCmdProcCalls(xmlp.commandListDevices);
        }

        

        generatedLayerFile << "scoped_lock l(global_lock);" << std::endl;

        if (strcmp(cmdListType.c_str(), "instance") == 0)
        {
            generatedLayerFile << "return instance_dispatch[GetKey(instance)].GetInstanceProcAddr(instance, pName);}" << std::endl;
        }
        else
        {
            generatedLayerFile << "return device_dispatch[GetKey(device)].GetDeviceProcAddr(device, pName);}" << std::endl;
        }
    }
    void LayerGenerator::generateForCmdList(std::list<XmlParser::Command> inputCmdList, std::string cmdListType)
    {
        for (auto item : inputCmdList)
        {
            /* ignores */
            if (std::find(ignoreCmdList.begin(), ignoreCmdList.end(), item.functName) != ignoreCmdList.end())
            {
                continue;
            }

            /* additional ignores*/
            if (std::find(ignoreLayerCmdList.begin(), ignoreLayerCmdList.end(), item.functName) != ignoreLayerCmdList.end())
            {
                continue;
            }
            /* guards */
            bool printGuard = false;
            std::string guardContent;
            if (cmdPlatformProtection)
            {
                guardContent = GetGuardString(item.functName, xmlp.cmdGuard);
                if (strcmp(guardContent.c_str(), "") != 0)
                    printGuard = true;
            }
            if (printGuard)
            {
                generatedLayerFile << "\n#if defined(" << GetGuardString(item.functName, xmlp.cmdGuard) << ")";
            }

            /* parameters */
            generatedLayerFile << "\n VK_LAYER_EXPORT " << item.functType << " VKAPI_CALL DetailsLayer_" << item.functName.substr(2, item.functName.size()) << "(";
            unsigned i = 0;
            if (item.parameterList.size() > 0)
            {
                for (auto param : item.parameterList)
                {
                    i++;
                    generatedLayerFile << param;
                    if (i % 2 == 0 && i < item.parameterList.size())
                        generatedLayerFile << ", ";
                    else
                        generatedLayerFile << " ";

                }
            }
            generatedLayerFile << ") {" << std::endl;
            /* start of the function */

            generatedLayerFile << "scoped_lock l(global_lock);" << std::endl;
            //SENDTOUI
            std::string upperName = item.functName.substr(2, item.functName.size());
            std::transform(upperName.begin(), upperName.end(), upperName.begin(), ::toupper);
            generatedLayerFile << "#ifdef ";
            generatedLayerFile << upperName << "_EXISTS" << std::endl;
            generatedLayerFile << "layer_" << item.functName.substr(2, item.functName.size()) << "(";
            if (item.parameterList.size() > 0)
            {
                i = 0;
                for (std::string param : item.parameterList)
                {
                    i++;
                    if (i % 2 == 0)
                        generatedLayerFile << param;
                    else if (i > 1 && i < item.parameterList.size())
                        generatedLayerFile << ", ";
                }
            }
            generatedLayerFile << ");" << std::endl;
            generatedLayerFile << "#endif " << std::endl;

            if (item.parameterList.size() > 0)
            {
                generatedLayerFile << cmdListType << "_dispatch[GetKey(" << *std::next(item.parameterList.begin()) << ")].";
                generatedLayerFile << item.functName.substr(2, item.functName.size()) << "(";
                i = 0;
                for (std::string param : item.parameterList)
                {
                    i++;
                    if (i % 2 == 0)
                        generatedLayerFile << param;
                    else if (i > 1 && i < item.parameterList.size())
                        generatedLayerFile << ", ";
                }
                generatedLayerFile << ");" << std::endl;

            }
            if (strcmp(item.functType.c_str(), "VkResult") == 0 ||
                strcmp(item.functType.c_str(), "VkDeviceSize") == 0 ||
                strcmp(item.functType.c_str(), "uint32_t") == 0 ||
                strcmp(item.functType.c_str(), "uint64_t") == 0 ||
                strcmp(item.functType.c_str(), "VkDeviceAddress") == 0
                )
            {
                generatedLayerFile << "return VK_SUCCESS;" << std::endl;
            }
            else if (strcmp(item.functType.c_str(), "VkBool32") == 0)
            {
                generatedLayerFile << "return VK_TRUE;" << std::endl;
            }

            generatedLayerFile << "}" << std::endl;
            if (printGuard)
                generatedLayerFile << "\n#endif";
        }
    }
    void LayerGenerator::generateGeneratedLayer()
    {
        /* generate Instance commands */
        generateForCmdList(xmlp.commandListInstances, "instance");

        /* generate Device commands */
        generateForCmdList(xmlp.commandListDevices, "device");
    }

    void LayerGenerator::generateCmdDeclarations(std::string cmdListType)
    {
        generatedLayerFile << "void Create";
        if (strcmp(cmdListType.c_str(), "instance") == 0)
        {
            generatedLayerFile << "Instance" << "Dispatch(PFN_vkGetInstanceProcAddr gpa, VkInstance* pInstance) {" << std::endl;
            generatedLayerFile << "VkLayerInstanceDispatchTable dispatchTable;" << std::endl;
            generateCmdDeclaration(xmlp.commandListInstances, cmdListType);
            generatedLayerFile << "\n{ scoped_lock l(global_lock); " << std::endl;
            generatedLayerFile << "instance_dispatch[GetKey(*pInstance)] = dispatchTable; \n} \n} " << std::endl;
        }
        else
        {
            generatedLayerFile << "Device" << "Dispatch(PFN_vkGetDeviceProcAddr gdpa, VkDevice* pDevice) {" << std::endl;
            generatedLayerFile << "VkLayerDispatchTable dispatchTable;" << std::endl;
            generateCmdDeclaration(xmlp.commandListDevices, cmdListType);
            generatedLayerFile << "\n{ scoped_lock l(global_lock); " << std::endl;
            generatedLayerFile << "device_dispatch[GetKey(*pDevice)] = dispatchTable; \n} \n} " << std::endl;
        }
        generatedLayerFile << " " << std::endl;
        /*
        dispatchTable.GetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)gpa(*pInstance, "vkGetInstanceProcAddr");
        dispatchTable.DestroyInstance = (PFN_vkDestroyInstance)gpa(*pInstance, "vkDestroyInstance");
        dispatchTable.EnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties)gpa(*pInstance, "vkEnumerateDeviceExtensionProperties");
        */

        /*
        dispatchTable.GetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)gdpa(*pDevice, "vkGetDeviceProcAddr");
        dispatchTable.DestroyDevice = (PFN_vkDestroyDevice)gdpa(*pDevice, "vkDestroyDevice");
        dispatchTable.BeginCommandBuffer = (PFN_vkBeginCommandBuffer)gdpa(*pDevice, "vkBeginCommandBuffer");
        dispatchTable.CmdDraw = (PFN_vkCmdDraw)gdpa(*pDevice, "vkCmdDraw");
        dispatchTable.CmdDrawIndexed = (PFN_vkCmdDrawIndexed)gdpa(*pDevice, "vkCmdDrawIndexed");
        dispatchTable.EndCommandBuffer = (PFN_vkEndCommandBuffer)gdpa(*pDevice, "vkEndCommandBuffer");
        */
    }

    void LayerGenerator::run()
    {
        /* new class that handles the vk.xml document */
        if (xmlp.openXmlDoc(XML_PATH) == false)
        {
            throw std::runtime_error("cannot open the xml document");
        }
        /* if there are commands defined in the xml doc, get the parent node of the commands */
        cmdDefinitions = xmlp.containsCommands();
        /* if there are platform protections defined in the xml doc, get their parent node */
        cmdPlatformProtection = xmlp.containsPlatformProtection();

        if (cmdDefinitions == false)
        {
            throw std::runtime_error("No vulkan commands definitions were found inside vk.xml file");
        }

        if (cmdPlatformProtection == true)
        {
            xmlp.getProtectionGuards();
        }

        xmlp.assignCmdsToTables();
        xmlp.assignAliasesToTables();

        /* create a new vk_layer_dispatch_table.h file */
        dispatchTableFile.open(VK_TABLE_PATH);
        if (!dispatchTableFile)
        {
            throw std::runtime_error("cannot open the dispatch table file");
        }

        /* generate code */
        generateTableInstance();
        generateTableDevice();
        
        dispatchTableFile.close();

        /* vk_layer_generated.cpp */
        staticLayerFile.open(LAYER_STATIC_PATH);
        generatedLayerFile.open(LAYER_GENERATED_PATH);
        if (!staticLayerFile || !generatedLayerFile)
        {
            throw std::runtime_error("cannot open the layer files");
        }
        generateHeaders();
        generateCmdDeclarations("device");
        generateCmdDeclarations("instance");
        /* copy static part of the layer to generated layer file*/
        generatedLayerFile << staticLayerFile.rdbuf();
        staticLayerFile.close();

        /* generate the generated part */
        generateGeneratedLayer();

        /* generate device proc addr call */
        generatedLayerFile << "#define GETPROCADDR(func) if(!strcmp(pName, \"vk\" #func)) return (PFN_vkVoidFunction)&DetailsLayer_##func;" << std::endl;

        generateGenLayerFooter("device");
        generateGenLayerFooter("instance");

        /* generate instancce proc addr call */

        generatedLayerFile.close();
    }
}
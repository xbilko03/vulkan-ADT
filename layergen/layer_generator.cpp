/*
* Name		    : layer_xml_parser.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file of the layer generator to construct vk_layer_generated.h and vk_layer_generated.cpp
*
* Author : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#include "layer_generator.hpp"

namespace laygen
{
    /* MAIN FUNCTIONS */
    /* entry point called by main.cpp */
    void LayerGenerator::run()
    {
        /* xml parse */
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
                throw std::runtime_error("No vulkan commands definitions were found inside the vk.xml file");
            }

            if (cmdPlatformProtection == true)
            {
                xmlp.getProtectionGuards();
            }

            xmlp.assignCmdsToTables();
            xmlp.assignAliasesToTables();
            xmlp.assignStructsToTables();
            xmlp.assignEnumsToTables();
        }

        /* vk_layer_dispatch_table.h generation */
        {
            dispatchTableFile.open(VK_TABLE_PATH);
            if (!dispatchTableFile)
            {
                throw std::runtime_error("cannot open the dispatch table file");
            }

            /* this is how instance table looks like
            *
            * typedef struct VkLayerInstanceDispatchTable_ {
            * PFN_vkCreateInstance CreateInstance;
            * PFN_vkDestroyInstance DestroyInstance;
            * ...
            * } VkLayerInstanceDispatchTable;
            */
            generateTableInstance();

            /* this is how device table looks like:
            *
            * typedef struct VkLayerDispatchTable_ {
            * PFN_vkGetDeviceProcAddr GetDeviceProcAddr;
            * PFN_vkDestroyDevice DestroyDevice;
            * ...
            * } VkLayerDispatchTable;
            */
            generateTableDevice();

            dispatchTableFile.close();
        }

        /* vk_layer_generated.cpp generation */
        {
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
            generatedLayerFile << VK_DEBUGGER_GPA << std::endl;

            generateGenLayerFooter("device");
            generateGenLayerFooter("instance");

            /* generate instancce proc addr call */

            generatedLayerFile.close();
        }
    }
    /* generate IDT */
    void LayerGenerator::generateTableInstance()
    {

        /* Instance commands */
        dispatchTableFile << INSTANCE_TABLE_HEADER;

        for (auto& item : xmlp.commandListInstances)
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
    /* generate DDT */
    void LayerGenerator::generateTableDevice()
    {
        /* Device commands */

        dispatchTableFile << DEVICE_TABLE_HEADER;

        for (auto& item : xmlp.commandListDevices)
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

    /* CORE FUNCTIONS */
    /* prints a single message sending operation that sends the parameter's name and value to the vkDebuggerApp*/
    bool LayerGenerator::PrintParameterSendStruct(std::ofstream* output, std::string type, std::string name, std::string prefix, int attempts)
    {
        attempts--;
        if (attempts < 0)
            return false;
        auto* structs = &xmlp.structList;
        std::string ptrlessType = type.substr(0, type.size() - 1);
        auto selectedStruct = (*structs)[ptrlessType];
        if (selectedStruct.structType == ptrlessType)
        {
            *output << "if(" << prefix + name << " != VK_NULL_HANDLE" << " && " << prefix + name << " != NULL" << ") {" << std::endl;
            prefix += name + "->";
            for (auto& item : selectedStruct.parameterList)
            {
                std::string outputType = item.type;
                /* if there is enum, it is an array */
                if (item.enumType != "" || item.name == "color")
                {
                    outputType = "array";
                }

                if (IsStruct(item.type))
                {
                    /* struct inside struct */
                    if (item.name == "ppGeometries" || item.name == "ppUsageCounts")
                    {
                        item.name += "[array]";
                        item.type = "other";
                    }
                    PrintParameterSendStruct(output, item.type, item.name, prefix, attempts);
                }
                else
                {
                    /* not a struct anymore */
                    PrintParameterSendSingle(output, outputType, item.name, prefix);
                }

            }
            *output << "}";
            *output << "else winsockSendToUI(&ConnectSocket, \"" << name << "=" << "VK_NULL_HANDLE!\");" << std::endl;
            return true;
        }
        return false;
    }
    /* prints a single send message to the VkDebuggerApp */
    bool LayerGenerator::PrintParameterSendSingle(std::ofstream* output,std::string type, std::string name, std::string prefix)
    {
        bool ret = false;
        std::string outputStr = "winsockSendToUI(&ConnectSocket,";

        outputStr += "\"" + prefix + name + "=\" + ";
        /* imperfect xml parsing consequence exception */
        if (name == "blendConstants" || name == "pResults")
        {   
            name += "[0]";
        }

        if (name == "sType")
        {
            ret = false;
        }
        else if (type == "uint32_t*" || type == "void*" || type == "void" || name == "presentMask")
        {
            outputStr += "ptrToString((void**)std::addressof(";
            outputStr += prefix + name;
            outputStr += "))";
            ret = false;
        }
        else if (type == "uint32_t" || type == "int32_t" || type == "float" || type == "int" || type == "size_t" || type == "uint64_t" || type == "uint8_t" || type == "VkDeviceSize")
        {
            outputStr += "std::to_string(";
            outputStr += prefix + name;
            outputStr += ")";
            ret = true;
        }
        else if (type == "VkBool32")
        {
            outputStr += "bool_as_text(";
            outputStr += prefix + name;
            outputStr += ")";
            ret = true;
        }
        else if (type == "char" || type == "char*")
        {
            outputStr += "charToString((char*)";
            outputStr += prefix + name;
            outputStr += ")";
            ret = false;
        }
        else if (type == "array")
        {
            outputStr = "winsockSendToUI(&ConnectSocket,";
            outputStr += "\"";
            outputStr += prefix + name;
            outputStr += "=[array]\"";
            ret = false;
        }
        else if (IsEnum(type))
        {
            ret = false;
        }
        else
        {
            /* try a struct */
            if (IsStruct(type))
            {
                PrintParameterSendStruct(output, type, name, prefix,3);
            }
            else
            {
                /* most likely a handle */
                outputStr += "ptrToString((void**)std::addressof(";
                outputStr += prefix + name;
                outputStr += "))";
                ret = true;
            }
        }
        
        outputStr += " + '!');";

        if(ret)
            *output << outputStr << std::endl;

        return ret;
    }
    /* analyzes every parameter included in the parameter list and calls PrintParameterSendSingle or PrintParameterSendStruct for each (if possible) */
    void LayerGenerator::PrintParemetersSendAll(std::ofstream* output, std::map<std::string, XmlParser::xStruct>* structs, auto* parameterList)
    {
        std::list<XmlParser::xParameter>::iterator it;
        *output << "if(connected){" << std::endl;
        for (it = (*parameterList).begin(); it != (*parameterList).end(); ++it) 
        {
            PrintParameterSendSingle(output, it->type, it->name, "");
        }
        *output << "}" << std::endl;
    }
    /* prints all parameters from a list into a line */
    void LayerGenerator::PrintParameters(std::ofstream* output, auto* parameterList, bool typesIncluded)
    {
        uint32_t i = 0;
        for (auto& param : *parameterList)
        {
            i++;
            if (typesIncluded)
            {
                *output << param.type << " " << param.name;
            }
            else
            {
                *output << param.name;
            }
            if (i < (*parameterList).size())
                *output << ", ";

        }
    }
    /* prints a single parameter from a list */
    void LayerGenerator::PrintParameter(std::ofstream* output, auto* parameterList, uint32_t index, bool typesIncluded)
    {
        uint32_t i = 0;
        for (auto& param : *parameterList)
        {
            if (index == i)
            {
                if (typesIncluded)
                {
                    *output << param.type << " " << param.name;
                }
                else
                {
                    *output << param.name;
                }
                return;
            }
            i++;
        }
    }
    /* starts the definition of a given Vulkan function by printing it into the generated layer */
    void LayerGenerator::PrintCallDefinition(std::ofstream* output, std::string* functType, std::string* functName, auto* parameterList)
    {
        *output << VK_CALL_TYPE << *functType << VK_CALL_NAME << (*functName).substr(2, (*functName).size()) << "(";
        uint32_t i = 0;
        PrintParameters(output, parameterList, true);
        *output << ") {" << std::endl;
    }
    /* prints a global lock that ensures the thread safety within a single Vulkan function call */
    void LayerGenerator::PrintLock(std::ofstream* output)
    {
        *output << "if(skipLock == false) {" << std::endl;
        *output << "\tscoped_lock l(global_lock);" << std::endl;
    }
    /* prints a call that is referencing the programmer's defined behavior of certain set of Vulkan functions that is described in layer.cpp file  */
    void LayerGenerator::PrintCustomCall(std::ofstream* output, std::string* functName, auto* parameterList, std::string functSuffix, std::string suffix)
    {
        std::string upperName = (*functName).substr(2, (*functName).size());
        std::transform(upperName.begin(), upperName.end(), upperName.begin(), ::toupper);
        *output << "#ifdef ";
        *output << upperName << "_" << suffix << std::endl;
        generatedLayerFile << "if(connected) {" << std::endl;
        *output << "layer_" << (*functName).substr(2, (*functName).size()) << "_" << functSuffix << "(";
        PrintParameters(output, parameterList, false);
        *output << ");" << std::endl;
        generatedLayerFile << "}" << std::endl;
        *output << "#endif " << std::endl;

    }
    /* prints a call that references the next function/point in the dispatch table */
    void LayerGenerator::PrintExecuteCall(std::ofstream* output, std::string* functType, std::string* functName, auto* parameterList, std::string cmdListType)
    {
        if (parameterList->size() > 0)
        {
            /* return applies only to non void types*/
            bool voidType = true;
            if (strcmp((*functType).c_str(), "void") != 0)
            {
                *output << "auto ret = ";
                voidType = false;
            }
            *output << cmdListType << "_dispatch[GetKey(";
            PrintParameter(output, parameterList, 0, false);
            *output << ")].";
            *output << (*functName).substr(2, (*functName).size()) << "(";
            PrintParameters(output, parameterList, false);
            *output << ");" << std::endl;

            if (voidType == false)
                *output << "winsockSendToUI(&ConnectSocket, \"vkResult=\" + std::to_string(ret) + '!');" << std::endl;
        }
    }
    /* prints a simple command that sends the input message to the VkDebuggerApp */
    void LayerGenerator::PrintSendToUI(std::ofstream* output, std::string input)
    {
        *output << "winsockSendToUI(&ConnectSocket, \"" << input << "!\");" << std::endl;
    }
    /* prints the includes and other vital constructs for the generated layer, this part is always generated at the start of the generated layer */
    void LayerGenerator::generateHeaders()
    {
        generatedLayerFile << "#include \"layer.hpp\"" << std::endl;
        generatedLayerFile << "#include <mutex>" << std::endl;
        generatedLayerFile << "#include <ws2tcpip.h>" << std::endl;
        generatedLayerFile << "#include \"" << LAYER_GENERATED_NAME  << ".h\"" << std::endl;
        generatedLayerFile << "#include \"vk_layer_table.h\"" << std::endl << std::endl;

        generatedLayerFile << "std::map<void*, VkLayerInstanceDispatchTable> instance_dispatch;" << std::endl;
        generatedLayerFile << "std::map<void*, VkLayerDispatchTable> device_dispatch;" << std::endl;
        generatedLayerFile << "std::mutex global_lock;" << std::endl;
        generatedLayerFile << "typedef std::lock_guard<std::mutex> scoped_lock;" << std::endl;

        generatedLayerFile << "template<typename DispatchableType>" << std::endl;
        generatedLayerFile << "void* GetKey(DispatchableType inst)" << std::endl;
        generatedLayerFile << "{ \n\treturn *(void**)inst; \n}" << std::endl;
    }
    /* print a single function address reference */
    void LayerGenerator::generateCmdDeclaration(std::list<XmlParser::xCommand> inputCmdList, std::string cmdListType)
    {
        /* list of instance inputCmdList cmd names */
        for (auto& item : inputCmdList)
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
    /* prints the initialization functions that refer to the functions generated by this layer */
    void LayerGenerator::generateCmdProcCalls(std::list<XmlParser::xCommand> inputCmdList)
    {
        /* list of instance inputCmdList cmd names */
        for (auto& item : inputCmdList)
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
    /*
    * prints the Vulkan vkGetProcInstanceAddr and vkGetDevuceInstanceAddr for layer initialization
    * and also everything to close generated file up, this is always generated at the end of the generated layer source file
    */
    void LayerGenerator::generateGenLayerFooter(std::string cmdListType)
    {
        generatedLayerFile << VK_DEBUGGER_FOOTER;
        if (strcmp(cmdListType.c_str(), "instance") == 0)
        {
            generatedLayerFile << "InstanceProcAddr(VkInstance instance, const char* pName) {" << std::endl;
            /* header */
            generatedLayerFile << "if (GetWindowName() == \"vkDetails.exe\") { GETPROCADDR(CreateDevice); GETPROCADDR(CreateInstance); return instance_dispatch[GetKey(instance)].GetInstanceProcAddr(instance, pName); }" << std::endl;
            generateCmdProcCalls(xmlp.commandListInstances);
            generateCmdProcCalls(xmlp.commandListDevices);
        }
        else
        {
            generatedLayerFile << "DeviceProcAddr(VkDevice device, const char* pName) {" << std::endl;
            /* header */
            generatedLayerFile << "if (GetWindowName() == \"vkDetails.exe\") { return device_dispatch[GetKey(device)].GetDeviceProcAddr(device, pName); }" << std::endl;
            generateCmdProcCalls(xmlp.commandListDevices);
        }

        PrintLock(&generatedLayerFile);
        if (strcmp(cmdListType.c_str(), "instance") == 0)
        {
            generatedLayerFile << "return instance_dispatch[GetKey(instance)].GetInstanceProcAddr(instance, pName);}" << std::endl;
        }
        else
        {
            generatedLayerFile << "return device_dispatch[GetKey(device)].GetDeviceProcAddr(device, pName);}" << std::endl;
        }
        /* enclose skip lock definition*/
        generatedLayerFile << "}";
    }
    /* prints the Vulkan functions that are caught by this layer during the runtime */
    void LayerGenerator::generateForCmdList(std::list<XmlParser::xCommand> inputCmdList, std::string cmdListType)
    {
        for (auto& item : inputCmdList)
        {
            /* ignores */
            if (std::find(ignoreCmdList.begin(), ignoreCmdList.end(), item.functName) != ignoreCmdList.end())
            {
                continue;
            }

            /* manual ignores*/
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

            /* START */
            /* 
            * function intercept header of the definition
            * output example:
            * VK_LAYER_EXPORT void VKAPI_CALL DetailsLayer_GetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties) 
            */
            PrintCallDefinition(&generatedLayerFile, &(item.functType), &(item.functName), &(item.parameterList));
            /* start of the function */

            /* 
            * lock to prevent multithreaded vulkan program trouble
            * output example:
            * scoped_lock l(global_lock);
            */
            PrintLock(&generatedLayerFile);

            /*
            * send an information about api call to the app
            * output example:
            * winsock_sendToUI(string);
            */
            generatedLayerFile << "if(connected) {" << std::endl;
            PrintSendToUI(&generatedLayerFile, "begin_" + item.functName);
            generatedLayerFile << "}" << std::endl;

            /*
            * additional functionality (if) defined in layer.cpp
            * output example:
            * #ifdef GETPHYSICALDEVICEMEMORYPROPERTIES_BEFORE_EXEC_EXISTS
            * layer_GetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
            * #endif
            */
            PrintCustomCall(&generatedLayerFile, &(item.functName), &(item.parameterList), "before", "BEFORE_EXEC_EXISTS");
            
            /*
            * pass the call to the loader
            * output example:
            * device_dispatch[GetKey(commandBuffer)].CmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
            */
            PrintExecuteCall(&generatedLayerFile, &(item.functType), &(item.functName), &(item.parameterList), cmdListType);

            /*
            * print parameters that can be sent
            */

            PrintParemetersSendAll(&generatedLayerFile, &(xmlp.structList), &(item.parameterList));

            /*
            * additional functionality (if) defined in layer.cpp
            * output example:
            * #ifdef GETPHYSICALDEVICEMEMORYPROPERTIES_AFTER_EXEC_EXISTS
            * layer_GetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
            * #endif
            */
            PrintCustomCall(&generatedLayerFile, &(item.functName), &(item.parameterList), "after", "AFTER_EXEC_EXISTS");


            /*
            * send an information about api call to the app
            * output example:
            * winsock_sendToUI(string);
            */
            generatedLayerFile << "if(connected) {" << std::endl;
            PrintSendToUI(&generatedLayerFile, "end_" + item.functName);

            generatedLayerFile << "if (callEveryBreak || callAtBreak)" << std::endl;
            generatedLayerFile << "\tnewCall();" << std::endl;

            generatedLayerFile << "}" << std::endl;

            if (strcmp(item.functType.c_str(), "void") != 0)
            {
                generatedLayerFile << "return ret;" << std::endl;
            }
            /* enclose skipLock */
            generatedLayerFile << "} else {" << std::endl;
            /* call for functions that bypass global lock */
            PrintExecuteCall(&generatedLayerFile, &(item.functType), &(item.functName), &(item.parameterList), cmdListType);

            if (strcmp(item.functType.c_str(), "void") != 0)
            {
                generatedLayerFile << "return ret;" << std::endl;
            }
            generatedLayerFile << "}" << std::endl;

            generatedLayerFile << "}" << std::endl;
            /* END */
            if (printGuard)
                generatedLayerFile << "\n#endif";
        }
    }
    /* generate debugger layer */
    void LayerGenerator::generateGeneratedLayer()
    {
        /* generate Instance commands */
        generateForCmdList(xmlp.commandListInstances, "instance");

        /* generate Device commands */
        generateForCmdList(xmlp.commandListDevices, "device");
    }
    /* prints all the instance/device function addresses included in this layer */
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
    }


    /* HELPER FUNCTIONS */
    /* fetch the platform's macro definition that enables the use of the certain set of functions */
    std::string LayerGenerator::GetGuardString(std::string commandName, std::list<XmlParser::xGuard> cmdGuard)
    {
        /* find the specific function's platform, get the macro name (if it exists) */
        for (auto& guard : cmdGuard)
        {
            for (auto& cmd : guard.commandList)
            {
                if (strcmp(commandName.c_str(), cmd.c_str()) == 0)
                {
                    return guard.guard;
                }
            }
        }
        return "";
    }
    /* identifies whether the input type is Vulkan enum typed */
    bool LayerGenerator::IsEnum(std::string type)
    {
        auto mapped = xmlp.enumList;
        auto item = mapped.find(type);
        if (item == mapped.end())
        {
            return false;
        }
        return true;
    }
    /* identifies whether the input type is Vulkan struct typed */
    bool LayerGenerator::IsStruct(std::string type)
    {
        auto* mapped = &xmlp.structList;
        auto item = mapped->find(type.substr(0, type.size() - 1));
        if (item == mapped->end())
        {
            return false;
        }
        else
        {
            return true;
        }
    }

}
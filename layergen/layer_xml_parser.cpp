/*
* Name		    : layer_xml_parser.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file of an XML parser, used by the layer_generator.cpp to load data from 'vk.xml'
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#include "layer_xml_parser.hpp"
/* iostream included to warn the user in case something gets ignored or handled incorrectly */
#include <iostream>

namespace laygen {
    /* searches the input list for the existence of the Vulkan function by its name, returns true if it exists in the given list */
    bool XmlParser::existsByName(std::list<XmlParser::xCommand> inputList, std::string inputName)
    {
        for (auto& item : inputList)
        {

            if (item.functName == inputName)
                return true;
        }
        return false;
    }
    /* searches the input list for the existence of the Vulkan function by its name, returns the found xCommand if it is found */
    XmlParser::xCommand XmlParser::findByName(std::list<XmlParser::xCommand> inputList, std::string inputName)
    {
        for (auto& item : inputList)
        {
            if (item.functName == inputName )
                return item;
        }
        return xCommand{};
    }
    /* searches the input list for the existence of the Vulkan parameter by its name, returns true if it exists in the given list */
    bool XmlParser::existsByNameParam(std::list<xParameter> inputList, std::string inputName)
    {
        for (auto& item : inputList)
        {
            if (item.name == inputName)
                return true;
        }
        return false;
    }
    /* reads the vk.xml file in search of the list of Vulkan functions that are only defined on the given platform */
    std::list<std::string> XmlParser::findCommands(std::string platformName, pugi::xml_document* doc)
    {
        std::list<std::string> resultList;
        /* for each node, get platform name */
        for (auto& point : (*doc).select_nodes("//extensions/extension"))
        {
            std::string pName = point.node().attribute("platform").value();
            if (pName == platformName)
            {
                /* for each platform, get names of the required Vulkan functions */
                for (auto command : point.node().select_nodes("./require/command"))
                {
                    resultList.push_back(command.node().attribute("name").value());
                }
            }
        }
        return resultList;
    }
    /* checks vk.xml file whether it contains Vulkan function definitions */
    bool XmlParser::containsCommands()
    {
        /* lookup the node within the vk.xml to check if it contains the Vulkan function definitions */
        for (auto& point : doc.select_nodes("//commands")) {
            auto tmpNode = point.node().attribute("comment");
            if (strcmp(tmpNode.value(), "Vulkan command definitions") == 0)
            {
                /* found definitions, layer is worth generating */
                commandRootNode = point.node();
                return true;
            }
        }
        return false;
    }
    /* checks vk.xml file whether it contains Vulkan functions that are platform-specific */
    bool XmlParser::containsPlatformProtection()
    {
        for (auto& point : doc.select_nodes("//platforms")) {
            auto tmpNode = point.node().attribute("comment");
            if (strcmp(tmpNode.value(), "Vulkan platform names, reserved for use with platform- and window system-specific extensions") == 0)
            {
                /* found definitions */
                platformProtectionRootNode = point.node();
                return true;;
            }
        }
        return false;
    }
    /* extracts the information about required necessities for a given set of functions out of vk.xml file */
    void XmlParser::getProtectionGuards()
    {
        /* for each node of the platform list, get the ifdef constant and the list of Vulkan functions that depend on this platform */
        for (auto& node : platformProtectionRootNode)
        {
            auto platformName = node.attribute("name").value();
            auto guard = node.attribute("protect").value();
            auto commandList = findCommands(platformName, &doc);
            cmdGuard.push_back(xGuard(platformName, guard, commandList));
        }
    }
    /* looks for the vk.xml file in layergen folder and opens it (if it exists) */
    bool XmlParser::openXmlDoc(std::string path)
    {
        if (!doc.load_file(path.c_str()))
            return false;
        else
            return true;
    }
    /* extracts the information about Vulkan functions and saves them into a list of xCommands based on their type (instance, device) */
    void XmlParser::assignCmdsToTables()
    {
        /* for each node, get the node of a Vulkan function */
        for (auto& point : commandRootNode.select_nodes(".//command")) {

            /* save name and type */
            auto functNamePoint = point.node().select_node("./proto/name/text()").node();
            auto functRetType = point.node().select_node("./proto/type/text()").node().value();

            /* get the node of the parameters */
            auto parametersPoint = point.node().select_nodes("./param");
            std::list<xParameter> parameters = {};
            /* for each parameter in this node, add it to the list of this Vulkan function's parameters */
            for (auto param : parametersPoint)
            {
                std::string pointer = param.node().select_node("text()").node().value();
                std::string type = param.node().select_node("type/text()").node().value();
                std::string name = param.node().select_node("name/text()").node().value();

                /* if Vulkan function parameter already exists, do not add it to the list */
                if (existsByNameParam(parameters, name) == true)
                    continue;
                /* due to the unusual vk.xml formatting structure, check if the type is a pointer type, if so, add a star at the end of this parameter's type */
                if (pointer != "")
                {
                    unsigned i = 0;
                    for (char& c : pointer) {
                        if (c == '*')
                        {
                            i++;
                            type.append("*");
                        }
                    }
                    if( i == 0)
                        type.append("*");
                    /* due to the unusual vk.xml formatting structure, the following types are also pointers */
                    if (name == "ppBuildRangeInfos" || name == "ppMaxPrimitiveCounts")
                        type.append("*");
                }
                /* save the parameter */
                parameters.push_back(xParameter(type,name));
            }

            /* based on the parameter's first parameter type, decide whether this function is of instance or device type */
            std::string parameter1 = point.node().select_node("./param/type/text()").node().value();

            /* command is of instance type */
            if (parameter1 == "VkInstanceCreateInfo" || parameter1 == "VkInstance" || parameter1 == "VkPhysicalDevice")
            {
                if (existsByName(commandListInstances, functNamePoint.value()) == false)
                {
                    commandListInstances.push_back(xCommand{ functNamePoint.value(), "", functRetType, parameters});
                }
            }
            /* command is of device type */
            else if (parameter1 == "VkDevice" || parameter1 == "VkQueue" || parameter1 == "VkCommandBuffer")
            {
                if (existsByName(commandListDevices, functNamePoint.value()) == false)
                {
                    commandListDevices.push_back(xCommand{ functNamePoint.value(), "", functRetType, parameters });
                }
            }
            /* command is either an alias of a different command, special case or discarded */
            else
            {
                /* if there is no alias, it is a special case */
                if (strcmp(point.node().attribute("alias").value(), "") == 0)
                {
                    /* handle the special case or discard the command */
                    std::string specialCase = functNamePoint.value();
                    if (specialCase == "vkEnumerateInstanceVersion")
                    {
                        if (existsByName(commandListInstances, functNamePoint.value()) == true)
                        {
                            commandListInstances.push_back(xCommand{ functNamePoint.value(), "", functRetType, parameters });
                        }
                    }
                    else if (specialCase == "vkEnumerateInstanceLayerProperties")
                    {
                        if (existsByName(commandListInstances, functNamePoint.value()) == true)
                        {
                            commandListInstances.push_back(xCommand{ functNamePoint.value(), "", functRetType, parameters });
                        }
                    }
                    else if (specialCase == "vkEnumerateInstanceExtensionProperties")
                    {
                        commandListInstances.push_back(xCommand{ functNamePoint.value(), "", functRetType, parameters });
                    }
                    else
                    {
                        /* discard the call and warn the user */
                        std::cerr << "Warning: unhandled vkFunction call either of name [" << functNamePoint.value() << "] or [" << point.node().attribute("name").value() << "]" << std::endl;
                    }
                }
                else
                {
                    /* put commands with alias into a structure, assign them into the proper table later */
                    commandListAliases.push_back(xCommand{ point.node().attribute("name").value(),point.node().attribute("alias").value() });
                }
            }
        }

    }
    /* assigns the extracted aliased Vulkan commands into their proper lists of xCommands based on their type */
    void XmlParser::assignAliasesToTables()
    {
        /* for each command with aliases , assign it into its proper tables */
        for (auto command : commandListAliases)
        {
            if (existsByName(commandListInstances, command.alias) == true)
            {
                /* the alias belongs to a function inside of the InstanceDispatchTable */
                auto details = findByName(commandListInstances, command.alias);
                commandListInstances.push_back(xCommand{ command.functName, "", details.functType, details.parameterList });
            }
            else if (existsByName(commandListDevices, command.alias) == true)
            {
                /* the alias belongs to a function inside of the InstanceDispatchTable */
                auto details = findByName(commandListDevices, command.alias);
                commandListDevices.push_back(xCommand{ command.functName, "", details.functType, details.parameterList });
            }
            else
            {
                /* discard and warn the user */
                std::cerr << "Warning: unhandled vkFunction call of name [" << command.functName << "]" << std::endl;
            }
        }

    }
    /* extracts the information about Vulkan structs and assigns them to a list of xStructs */
    void XmlParser::assignStructsToTables()
    {
        auto structRootNode = doc.select_node(".//types").node();
        /* for each node, check the data for each Vulkan constructs */
        for (auto point : structRootNode.select_nodes(".//type"))
        {
            auto type = point.node().attribute("category");
            auto name = point.node().attribute("name");
            auto alias = point.node().attribute("alias");
            
            /* if the type of this Vulkan construct is of a Vulkan struct, extract the data and put them into a list */
            if (strcmp(type.value(), "struct") == 0 || strcmp(type.value(), "type") == 0)
            {
                std::list<xStructParameter> pList = {};
                /* for each attribute of a given struct, add it as a parameter */
                for (auto item : point.node().select_nodes(".//member"))
                {
                    std::string pType = item.node().select_node(".//type/text()").node().value();
                    std::string pName = item.node().select_node(".//name/text()").node().value();
                    std::string pEnum = item.node().select_node(".//enum/text()").node().value();
                    std::string pointerCheck = item.node().select_node(".//text()").node().value();
                    if (pType != pointerCheck)
                    {
                        pType += '*';
                    }
                    pList.push_back(xStructParameter(pType, pName, pEnum));
                }
                structList[name.value()] = xStruct(type.value(), alias.value(), name.value(), pList);
            }
        }
    }
    /* extracts the information about Vulkan enums and assigns them to a list of xEnums */
    void XmlParser::assignEnumsToTables()
    {
        /* for each Vulkan enum extracted from vk.xml, save it into a list along with its details */
        for (auto point : doc.select_nodes(".//enums"))
        {
            std::string name = point.node().attribute("name").value();
            std::list<XmlParser::xEnum> newList;
            for (auto item : point.node().select_nodes(".//enum"))
            {
                std::string pvalue = item.node().attribute("value").value();
                std::string pbitpos = item.node().attribute("bitpos").value();
                std::string pname = item.node().attribute("name").value();
                std::string pcomment = item.node().attribute("comment").value();
                newList.push_back(xEnum(pvalue,pbitpos,pname,pcomment));
            }
            enumList[name] = newList;
        }
    }
}
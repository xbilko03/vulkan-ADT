#include "layer_xml_parser.hpp"

#include <iostream>
#include <algorithm>

namespace laygen {
    
    bool XmlParser::existsByName(std::list<XmlParser::Command> inputList, std::string inputName)
    {
        for (auto item : inputList)
        {

            if (strcmp(item.functName.c_str(), inputName.c_str()) == 0)
                return true;
        }
        return false;
    }
    XmlParser::Command XmlParser::findByName(std::list<XmlParser::Command> inputList, std::string inputName)
    {
        for (auto item : inputList)
        {

            if (strcmp(item.functName.c_str(), inputName.c_str()) == 0)
                return item;
        }
        return Command{};
    }
    bool XmlParser::existsByNameCmd(std::list<Parameter> inputList, std::string inputName)
    {
        for (auto item : inputList)
        {

            if (strcmp(item.name.c_str(), inputName.c_str()) == 0)
                return true;
        }
        return false;
    }
    std::list<std::string> XmlParser::findCommands(std::string platformName, pugi::xml_document* doc)
    {
        std::list<std::string> resultList;
        for (auto point : (*doc).select_nodes("//extensions/extension"))
        {
            auto pName = point.node().attribute("platform").value();
            if (strcmp(pName, platformName.c_str()) == 0)
            {
                /* for each command under require, put it into the resultList */
                for (auto command : point.node().select_nodes("./require/command"))
                {
                    resultList.push_back(command.node().attribute("name").value());
                }
            }
        }

        return resultList;
    }
    bool XmlParser::containsCommands()
    {
        for (auto point : doc.select_nodes("//commands")) {
            auto tmpNode = point.node().attribute("comment");
            if (strcmp(tmpNode.value(), "Vulkan command definitions") == 0)
            {
                /* found definitions */
                commandRootNode = point.node();
                return true;
            }
        }
        return false;
    }
    bool XmlParser::containsPlatformProtection()
    {
        for (auto point : doc.select_nodes("//platforms")) {
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
    void XmlParser::getProtectionGuards()
    {
        for (auto node : platformProtectionRootNode)
        {
            auto platformName = node.attribute("name").value();
            auto guard = node.attribute("protect").value();
            auto commandList = findCommands(platformName, &doc);
            cmdGuard.push_back(cGuard(platformName, guard, commandList));
        }
    }
    bool XmlParser::openXmlDoc(std::string path)
    {
        if (!doc.load_file(path.c_str()))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    void XmlParser::assignCmdsToTables()
    {
        for (auto point : commandRootNode.select_nodes(".//command")) {


            auto functNamePoint = point.node().select_node("./proto/name/text()").node();
            auto functRetType = point.node().select_node("./proto/type/text()").node().value();

            auto parametersPoint = point.node().select_nodes("./param");
            std::list<Parameter> parameters = {};
            for (auto param : parametersPoint)
            {
                std::string pointer = param.node().select_node("text()").node().value();

                std::string type = param.node().select_node("type/text()").node().value();

                std::string name = param.node().select_node("name/text()").node().value();

                if (existsByNameCmd(parameters, name) == true)
                    continue;
                if (strcmp(pointer.c_str(),"") != 0)
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
                    if(strcmp(name.c_str(), "ppBuildRangeInfos") == 0 || strcmp(name.c_str(), "ppMaxPrimitiveCounts") == 0)
                        type.append("*");
                }
                parameters.push_back(Parameter(type,name));
            }

            auto parameter1 = point.node().select_node("./param/type/text()").node().value();

            if (
                strcmp(parameter1, "VkInstanceCreateInfo") == 0 ||
                strcmp(parameter1, "VkInstance") == 0 ||
                strcmp(parameter1, "VkPhysicalDevice") == 0
                )
            {
                /* command is of instance type */

                if (existsByName(commandListInstances, functNamePoint.value()) == false)
                {
                    commandListInstances.push_back(Command{ functNamePoint.value(), "", functRetType, parameters});
                }
            }
            else if
                (
                    strcmp(parameter1, "VkDevice") == 0 ||
                    strcmp(parameter1, "VkQueue") == 0 ||
                    strcmp(parameter1, "VkCommandBuffer") == 0
                    )
            {
                /* command is of device type */
                if (existsByName(commandListDevices, functNamePoint.value()) == false)
                {
                    commandListDevices.push_back(Command{ functNamePoint.value(), "", functRetType, parameters });
                }
            }
            else
            {
                /* command is either an alias of a different command, special case or discarded */

                /* if there is no alias, it is a special case */
                if (strcmp(point.node().attribute("alias").value(), "") == 0)
                {
                    /* handle the special case or discard the command */
                    if (strcmp(functNamePoint.value(), "vkEnumerateInstanceVersion") == 0)
                    {
                        if (existsByName(commandListInstances, functNamePoint.value()) == true)
                        {
                            commandListInstances.push_back(Command{ functNamePoint.value(), "", functRetType, parameters });
                        }
                    }
                    else if (strcmp(functNamePoint.value(), "vkEnumerateInstanceLayerProperties") == 0)
                    {
                        if (existsByName(commandListInstances, functNamePoint.value()) == true)
                        {
                            commandListInstances.push_back(Command{ functNamePoint.value(), "", functRetType, parameters });
                        }
                    }
                    else if (strcmp(functNamePoint.value(), "vkEnumerateInstanceExtensionProperties") == 0)
                    {
                        commandListInstances.push_back(Command{ functNamePoint.value(), "", functRetType, parameters });
                    }
                    else
                    {
                        /* discard and warn the user */
                        std::cerr << "Warning: unhandled vkFunction call either of name [" << functNamePoint.value() << "] or [" << point.node().attribute("name").value() << "]" << std::endl;
                    }
                }
                else
                {
                    /* put commands with alias into a structure, assign them into the proper table later */
                    commandListAliases.push_back(Command{ point.node().attribute("name").value(),point.node().attribute("alias").value() });
                }
            }
        }

    }
    void XmlParser::assignAliasesToTables()
    {
        /* assign commands with aliases into their proper tables */
        for (auto command : commandListAliases)
        {
            if (existsByName(commandListInstances, command.alias) == true)
            {
                /* the alias belongs to a function inside of the InstanceDispatchTable */
                auto details = findByName(commandListInstances, command.alias);
                commandListInstances.push_back(Command{ command.functName, "", details.functType, details.parameterList });
            }
            else if (existsByName(commandListDevices, command.alias) == true)
            {
                /* the alias belongs to a function inside of the InstanceDispatchTable */
                auto details = findByName(commandListDevices, command.alias);
                commandListDevices.push_back(Command{ command.functName, "", details.functType, details.parameterList });
            }
            else
            {
                /* discard and warn the user */
                std::cerr << "Warning: unhandled vkFunction call of name [" << command.functName << "]" << std::endl;
            }
        }

    }
    
}
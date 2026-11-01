/*
* Name		    : layerXmlParser.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file of an XML parser, used by the layerGenerator.cpp to load data from 'vk.xml'
*
* Author		: Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#pragma once

#include "pugixml.hpp"
#include <list>
#include <map>
/* AI-Assisted: Claude Opus 5.5: syntax */
#include <set>
/* AI-Assisted: end */

namespace vkdebug::generator {
    class xmlParser {
    public:
        /* this structure holds the list of what commands are defined per platform and the 'guard' which should be used to ifdef these commands */
        struct xGuard {
            std::string substr;
            std::string guard;
            std::list<std::string> commandList;
        };

        /* this structure associates the function's type to it's name */
        struct xParameter {
            std::string type;
            std::string name;
            /* AI-Assisted: Claude Opus 5.5: syntax */
            /* [xbilko03] layer differentiates between const and non-const parameters, sends const parameters before call so command might be inspected in case of crash */
            bool isConst = false;
            /* AI-Assisted: end */
        };

        /* this structure is the xParameter variant, the extra 'enumType' attribute defined here references one of the types that xParameters already defined */
        struct xStructParameter {
            std::string type;
            std::string name;
            std::string enumType;
        };

        /* this structure holds all the information about a given Vulkan function regarding its definition */
        struct xCommand {
            std::string functName;
            std::string alias;
            std::string functType;
            std::list<xParameter> parameterList;
        };

        /* this structure holds all the information about a given Vulkan struct regarding its definition */
        struct xStruct {
            std::string structName;
            std::string alias;
            std::string structType;
            std::list<xStructParameter> parameterList;
        };

        /* this structure hold all the information about a given Vulkan enum regarding its definition */
        struct xEnum {
            std::string value;
            std::string bitpos;
            std::string name;
            std::string comment;
        };

        /* checks vk.xml file whether it contains Vulkan function definitions */
        bool containsCommands();

        /* checks vk.xml file whether it contains Vulkan functions that are platform-specific */
        bool containsPlatformProtection();

        /* extracts the information about required necessities for a given set of functions out of vk.xml file */
        void getProtectionGuards();

        /* looks for the vk.xml file in libs/vulkan folder and opens it (if it exists) */
        bool openXmlDoc(std::string path);

        /* extracts the information about Vulkan functions and saves them into a list of xCommands based on their type (instance, device) */
        void assignCmdsToTables();

        /* assigns the extracted aliased Vulkan commands into their proper lists of xCommands based on their type */
        void assignAliasesToTables();

        /* extracts the information about Vulkan structs and assigns them to a list of xStructs */
        void assignStructsToTables();

        /* extracts the information about Vulkan enums and assigns them to a list of xEnums */
        void assignEnumsToTables();

        /* stores data about Vulkan instance functions */
        std::list<xmlParser::xCommand> commandListInstances;
        /* stores data about Vulkan device functions */
        std::list<xmlParser::xCommand> commandListDevices;
        /* stores data about Vulkan structs */
        std::map<std::string, xmlParser::xStruct> structList;
        /* stores data about Vulkan enums */
        std::map<std::string, std::list<xmlParser::xEnum> > enumList;
        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] handle types such as { VkBuffer, VkImage, VkDeviceMemory, VkQueue, ... } */
        std::set<std::string> handleList;
        /* AI-Assisted: end */
        /* stores data about necessities for certain set of Vulkan functions */
        std::list<xmlParser::xGuard> cmdGuard;

    private:
        /* stores data about Vulkan aliased functions */
        std::list<xCommand> commandListAliases;

        /* loaded XML document and relevant nodes */
        pugi::xml_document doc;
        pugi::xml_node commandRootNode;
        pugi::xml_node platformProtectionRootNode;

        /* searches the input list for the existence of the Vulkan function by its name, returns true if it exists in the given list */
        bool existsByName(std::list<xmlParser::xCommand> inputList, std::string inputName);

        /* searches the input list for the existence of the Vulkan parameter by its name, returns true if it exists in the given list */
        bool existsByNameParam(std::list<xParameter> inputList, std::string inputName);

        /* searches the input list for the existence of the Vulkan function by its name, returns the found xCommand if it is found */
        xmlParser::xCommand findByName(std::list<xmlParser::xCommand> inputList, std::string inputName);

        /* reads the vk.xml file in search of the list of Vulkan functions that are only defined on the given platform */
        std::list<std::string> findCommands(std::string platformName, pugi::xml_document *doc);
    };
}

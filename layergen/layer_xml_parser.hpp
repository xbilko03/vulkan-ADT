/*
* Name		    : layer_xml_parser.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file of an XML parser, used by the layer_generator.cpp to load data from 'vk.xml'
*
* Author		: Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#pragma once

#include "pugixml.hpp"
#include <list>
#include <map>
#include <cstring>
#include <stdint.h>
#include <string.h>

namespace laygen {
	class XmlParser {
	public:
		/* this structure holds the list of what commands are defined per platform and the 'guard' which should be used to ifdef these commands */
		struct xGuard {
			std::string substr;
			std::string guard;
			std::list<std::string> commandList;
		};
		/* this structure associates the function's type to it's name */
		struct xParameter
		{
			std::string type;
			std::string name;
		};
		/* this structure is the xParameter variant, the extra 'enumType' attribute defined here references one of the types that xParameters already defined */
		struct xStructParameter
		{
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
		/* looks for the vk.xml file in layergen folder and opens it (if it exists) */
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
		std::list<XmlParser::xCommand> commandListInstances;
		/* stores data about Vulkan device functions */
		std::list<XmlParser::xCommand> commandListDevices;
		/* stores data about Vulkan structs */
		std::map<std::string, XmlParser::xStruct> structList;
		/* stores data about Vulkan enums */
		std::map<std::string, std::list<XmlParser::xEnum>> enumList;
		/* stores data about necessities for certain set of Vulkan functions */
		std::list<XmlParser::xGuard> cmdGuard;

	private:
		/* stores data about Vulkan aliased functions */
		std::list<xCommand> commandListAliases;

		/* loaded XML document and relevant nodes */
		pugi::xml_document doc;
		pugi::xml_node commandRootNode;
		pugi::xml_node platformProtectionRootNode;

		/* searches the input list for the existence of the Vulkan function by its name, returns true if it exists in the given list */
		bool existsByName(std::list<XmlParser::xCommand> inputList, std::string inputName);
		/* searches the input list for the existence of the Vulkan parameter by its name, returns true if it exists in the given list */
		bool existsByNameParam(std::list<xParameter> inputList, std::string inputName);
		/* searches the input list for the existence of the Vulkan function by its name, returns the found xCommand if it is found */
		XmlParser::xCommand findByName(std::list<XmlParser::xCommand> inputList, std::string inputName);
		/* reads the vk.xml file in search of the list of Vulkan functions that are only defined on the given platform */
		std::list<std::string> findCommands(std::string platformName, pugi::xml_document* doc);
	};
}

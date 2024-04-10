#pragma once

#include "pugixml.hpp"
#include <list>
#include <map>

namespace laygen {
	class XmlParser {
	public:
		struct cGuard {
			std::string substr;
			std::string guard;
			std::list<std::string> commandList;
		};
		struct Parameter
		{
			std::string type;
			std::string name;
		};
		struct StructParameter
		{
			std::string type;
			std::string name;
			std::string enumType;
		};
		struct Command {
			std::string functName;
			std::string alias;
			std::string functType;
			std::list<Parameter> parameterList;
		};
		struct Struct {
			std::string structName;
			std::string alias;
			std::string structType;
			std::list<StructParameter> parameterList;
		};
		struct Enum {
			std::string value;
			std::string bitpos;
			std::string name;
			std::string comment;
		};

		bool containsCommands();
		bool containsPlatformProtection();
		void getProtectionGuards();
		bool openXmlDoc(std::string path);
		void assignCmdsToTables();
		void assignAliasesToTables();
		void assignStructsToTables();
		void assignEnumsToTables();

		/* output */
		std::list<XmlParser::Command> commandListInstances;
		std::list<XmlParser::Command> commandListDevices;
		std::map<std::string, XmlParser::Struct> structList;
		std::map<std::string, std::list<XmlParser::Enum>> enumList;
		std::list<XmlParser::cGuard> cmdGuard;

	private:
		std::list<Command> commandListAliases;

		bool existsByName(std::list<XmlParser::Command> inputList, std::string inputName);
		XmlParser::Command findByName(std::list<XmlParser::Command> inputList, std::string inputName);
		std::list<std::string> findCommands(std::string platformName, pugi::xml_document* doc);
		std::string GetGuardString(std::string commandName, std::list<cGuard> cmdGuard);
		bool existsByNameCmd(std::list<Parameter> inputList, std::string inputName);

		pugi::xml_document doc;
		pugi::xml_node commandRootNode;
		pugi::xml_node platformProtectionRootNode;
	};
}
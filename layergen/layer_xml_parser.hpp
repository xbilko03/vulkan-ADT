#pragma once

#include "pugixml.hpp"
#include <list>

namespace laygen {
	class XmlParser {
	public:
		struct cGuard {
			std::string substr;
			std::string guard;
			std::list<std::string> commandList;
		};
		struct Command {
			std::string functName;
			std::string alias;
			std::string functType;
			std::list<std::string> parameterList;
		};

		bool containsCommands();
		bool containsPlatformProtection();
		void getProtectionGuards();
		bool openXmlDoc(std::string path);
		void assignCmdsToTables();
		void assignAliasesToTables();

		/* output */
		std::list<XmlParser::Command> commandListInstances;
		std::list<XmlParser::Command> commandListDevices;
		std::list<XmlParser::cGuard> cmdGuard;

	private:
		std::list<Command> commandListAliases;

		bool existsByName(std::list<XmlParser::Command> inputList, std::string inputName);
		XmlParser::Command findByName(std::list<XmlParser::Command> inputList, std::string inputName);
		std::list<std::string> findCommands(std::string platformName, pugi::xml_document* doc);
		std::string GetGuardString(std::string commandName, std::list<cGuard> cmdGuard);
		bool existsByNameCmd(std::list<std::string> inputList, std::string inputName);

		pugi::xml_document doc;
		pugi::xml_node commandRootNode;
		pugi::xml_node platformProtectionRootNode;
	};
}
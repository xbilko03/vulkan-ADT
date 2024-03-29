#pragma once

#include <string>
#include <list>
#include <fstream>

#include "layer_xml_parser.hpp"

namespace laygen {
	class LayerGenerator {
	public: 
		void run();

	private:
		#define LAYER_GENERATED_PATH \
		"..\\..\\..\\layer\\layer_generated.cpp"
		#define LAYER_STATIC_PATH \
		"..\\..\\..\\layer\\layer_static.cpp"

		#define VK_TABLE_PATH \
		"..\\..\\..\\layergen\\vk_layer_dispatch_table.h"

		#define XML_PATH \
		"C:\\Users\\jozef\\Desktop\\vk details\\layergen\\vk.xml"

		#define INSTANCE_TABLE_HEADER \
		"#pragma once\n\
        \n#if !defined(PFN_GetPhysicalDeviceProcAddr) \
        \n\ttypedef PFN_vkVoidFunction(VKAPI_PTR* PFN_GetPhysicalDeviceProcAddr)(VkInstance instance, const char* pName); \
        \n#endif \
        \n\ntypedef struct VkLayerInstanceDispatchTable_ { \
        \n #if !defined(PFN_GetPhysicalDeviceProcAddr) \
        \nPFN_GetPhysicalDeviceProcAddr GetPhysicalDeviceProcAddr; \
        \n#endif\
        "
		#define INSTANCE_TABLE_FOOTER \
		"\n } VkLayerInstanceDispatchTable;"

		#define DEVICE_TABLE_HEADER \
		"\n#define DEVICE_DISP_TABLE_MAGIC_NUMBER 0x10ADED040410ADEDUL\
        \ntypedef struct VkLayerDispatchTable_ {\
        \nuint64_t magic;\
        "

		#define DEVICE_TABLE_FOOTER \
		"\n} VkLayerDispatchTable;"

		#define IGNORED_COMMANDS \
		"vkGetPhysicalDeviceRefreshableObjectTypesKHR", \
		"vkCmdSetLineStippleKHR", \
		"vkGetFaultData", \
		"vkCmdRefreshObjectsKHR", \
		"vkGetCommandPoolMemoryConsumption", \
		"vkCmdSetRenderingAttachmentLocationsKHR", \
		"vkCmdSetRenderingInputAttachmentIndicesKHR"

		#define LAYER_LEVEL_IGNORE_COMMANDS \
		"vkCreateInstance", \
		"vkDestroyInstance", \
		"vkCreateDevice", \
		"vkDestroyDevice", \
		"vkEnumerateInstanceLayerProperties", \
		"vkEnumerateDeviceLayerProperties", \
		"vkEnumerateInstanceExtensionProperties", \
		"vkEnumerateDeviceExtensionProperties", \
		"vkGetDeviceProcAddr", \
		"vkGetInstanceProcAddr"

		std::list<std::string> ignoreCmdList = { IGNORED_COMMANDS };
		std::list<std::string> ignoreLayerCmdList = { LAYER_LEVEL_IGNORE_COMMANDS };

		std::string GetGuardString(std::string commandName, std::list<XmlParser::cGuard> cmdGuard);

		void generateTableInstance();
		void generateTableDevice();
		void generateGeneratedLayer();
		void generateForCmdList(std::list<XmlParser::Command> inputCmdList, std::string cmdListType);
		void generateGenLayerFooter(std::string cmdListType);
		void generateCmdProcCalls(std::list<XmlParser::Command> inputCmdList);
		void generateCmdDeclarations(std::string cmdListType);
		void generateCmdDeclaration(std::list<XmlParser::Command> inputCmdList, std::string cmdListType);
		void generateHeaders();

		XmlParser xmlp;
		/* controls */
		bool cmdPlatformProtection;
		bool cmdDefinitions;
		std::ofstream dispatchTableFile;
		std::ifstream staticLayerFile;
		std::ofstream generatedLayerFile;
	};

}
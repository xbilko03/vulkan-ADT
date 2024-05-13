/*
* Name		    : layer_generator.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file of the layer generator to construct layer_generated.h and layer_generated.cpp
*
* Author		: Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#pragma once

#include "layer_xml_parser.hpp"
#include <string>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <stdint.h>
#include <string.h>

namespace laygen {
	class LayerGenerator {
	public:
		/* entry point called by main.cpp */
		void run();

	private:

		/* code generation macros */
		#define VK_DEBUGGER_NAME \
		"DebuggerLayer"

		#define VK_DEBUGGER_GPA \
		"#define GETPROCADDR(func) if(!strcmp(pName, \"vk\" #func)) return (PFN_vkVoidFunction)&" << VK_DEBUGGER_NAME << "_##func;"

		#define VK_DEBUGGER_FOOTER \
		"VK_LAYER_EXPORT PFN_vkVoidFunction VKAPI_CALL " << VK_DEBUGGER_NAME << "_Get"

		#define VK_CALL_TYPE \
		"\n VK_LAYER_EXPORT "

		#define VK_CALL_NAME \
		" VKAPI_CALL " << VK_DEBUGGER_NAME << "_"

		#define LAYER_GENERATED_NAME \
		"layer_generated"

		/* path, where DT will be generated */
		#define VK_TABLE_PATH \
		"..\\..\\..\\layer\\" LAYER_GENERATED_NAME ".h"

		/* path, where generated layer will be generated */
		#define LAYER_GENERATED_PATH \
		"..\\..\\..\\layer\\" LAYER_GENERATED_NAME ".cpp"

		/* path to a file, that includes manual definitions of certain Vulkan functions */
		#define LAYER_STATIC_PATH \
		"..\\..\\..\\layer\\layer_static.cpp"

		/* path to a file, where Vulkan API definitions are, this is the source of the code generation for this layer */
		#define XML_PATH \
		"..\\..\\..\\layergen\\vk.xml"

		#define HEADER_NOTE_DT \
		"/*\n" \
		"* Name		    : layer_generated.h\n" \
		"* Project	    : A Debugging Tool for Vulkan API (VkDebugger)\n" \
		"* Description  : Header file of the generated layer defining IDT and DDT structs\n" \
		"* [Careful! This file is generated] \n" \
		"* due to this, there is little handholding when reading this code \n" \
		"* to attempt and make up for this, here is the basic structure to help you understand \n" \
		"*\n"\
		"* in this file, there are IDT and DDT structures defined \n"\
		"* VkLayerInstanceDispatchTable_ and VkLayerDeviceDispatchTable_\n"\
		"* they are used in the layer_generated.cpp file to make dispatch chain initialization successful\n" \
		"* \n"\
		"* Based on the template available online by : Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann) sample_layer \n" \
		"* https ://github.com/baldurk/sample_layer/blob/master \n" \
		"* No relevant changes, it is just expanded by : Jozef Bilko (xbilko03), supervised by Ing. Ján Peciva Ph.D.\n" \
		"*/\n"

		#define HEADER_NOTE_GL \
		"/*\n" \
		"* Name		    : layer_generated.cpp\n" \
		"* Project	    : A Debugging Tool for Vulkan API (VkDebugger)\n" \
		"* Description  : Source file of the generated layer, this includes layer initialization, list of intercepted functions along their behavior\n" \
		"* [Careful! This file is generated] \n" \
		"* due to this, there is little handholding when reading this code \n" \
		"* to attempt and make up for this, here is the basic structure to help you understand \n" \
		"*\n"\
		"* CreateDeviceDispatch & CreateInstanceDispatch -- these fill the DT structs defined in layer_generated.h\n"\
		"*\n"\
		"* DebuggerLayer_CreateInstance & DebuggerLayer_DeviceInstance: \n" \
		"* these functions start the initialization process by filling the IDT and DDT structs with addresses\n"\
		"* from the next point in the dispatch chain (so the layer knows how to pass the program control and doesn't get stuck here)\n" \
		"*\n"\
		"* Next section is just all the Vulkan function calls we're intercepting\n"\
		"*\n" \
		"* DebuggerLayer_GetInstanceProcAddr & DebuggerLayer_GetDeviceProcAddr -- this is where previous points of the dispatch chain refer to\n" \
		"* when they're trying to figure out whether this layer intercepts the given function or not\n" \
		"*\n" \
		"* Based on the template available online by : Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann) sample_layer \n" \
		"* https ://github.com/baldurk/sample_layer/blob/master \n" \
		"* Changes made by : Jozef Bilko (xbilko03), supervised by Ing. Ján Peciva Ph.D.\n" \
		"*/\n"

		#define INSTANCE_TABLE_HEADER \
        HEADER_NOTE_DT \
		"#pragma once\n\
        \n#if !defined(PFN_GetPhysicalDeviceProcAddr) \
        \n\ttypedef PFN_vkVoidFunction(VKAPI_PTR* PFN_GetPhysicalDeviceProcAddr)(VkInstance instance, const char* pName); \
        \n#endif \
        \n\ntypedef struct VkLayerInstanceDispatchTable_ { \
        \n #if !defined(PFN_GetPhysicalDeviceProcAddr) \
        \nPFN_GetPhysicalDeviceProcAddr GetPhysicalDeviceProcAddr; \
        \n#endif"

		#define INSTANCE_TABLE_FOOTER \
		"\n } VkLayerInstanceDispatchTable;"

		#define DEVICE_TABLE_HEADER \
		"\n#define DEVICE_DISP_TABLE_MAGIC_NUMBER 0x10ADED040410ADEDUL\
        \ntypedef struct VkLayerDispatchTable_ {\
        \nuint64_t magic;"

		#define DEVICE_TABLE_FOOTER \
		"\n} VkLayerDispatchTable;"

		/* commands that will not be generated and put into the layer as per programmer's choice */
		#define IGNORED_COMMANDS \
		"vkGetPhysicalDeviceRefreshableObjectTypesKHR", \
		"vkCmdSetLineStippleKHR", \
		"vkGetFaultData", \
		"vkCmdRefreshObjectsKHR", \
		"vkGetCommandPoolMemoryConsumption", \
		"vkCmdSetRenderingAttachmentLocationsKHR", \
		"vkCmdSetRenderingInputAttachmentIndicesKHR"
		std::list<std::string> ignoreCmdList = { IGNORED_COMMANDS };

		/*
		* commands that will not be generated and put into the layer, however, both DTs will include these calls,
		* it is because they are supposed to be defined in layer_static 
		*/
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
		std::list<std::string> ignoreLayerCmdList = { LAYER_LEVEL_IGNORE_COMMANDS };


		/* MAIN FUNCTIONS */
		/* generate IDT */
		void generateTableInstance();
		/* generate DDT */
		void generateTableDevice();
		/* generate debugger layer */
		void generateGeneratedLayer();

		/* CORE FUNCTIONS */
		/* prints the includes and other vital constructs for the generated layer, this part is always generated at the start of the generated layer */
		void generateHeaders();
		/* analyzes every parameter included in the parameter list and calls PrintParameterSendSingle or PrintParameterSendStruct for each (if possible) */
		void PrintParemetersSendAll(std::ofstream* output, std::map<std::string, XmlParser::xStruct>* structs, auto* parameterList);
		/* prints a single send message to the VkDebuggerApp */
		void PrintParameterSendSingle(std::ofstream* output, std::string type, std::string name, std::string prefix);
		/* prints a single message sending operation that sends the parameter's name and value to the vkDebuggerApp*/
		void PrintParameterSendStruct(std::ofstream* output, std::string type, std::string name, std::string prefix, int attempts);
		/* prints all parameters from a list into a line */
		void PrintParameters(std::ofstream* output, auto* parameterList, bool typesIncluded);
		/* prints a single parameter from a list */
		void PrintParameter(std::ofstream* output, auto* parameterList, uint32_t index, bool typesIncluded);
		/* starts the definition of a given Vulkan function by printing it into the generated layer */
		void PrintCallDefinition(std::ofstream* output, std::string* functType, std::string* functName, auto* parameterList);
		/* prints a global lock that ensures the thread safety within a single Vulkan function call */
		void PrintLock(std::ofstream* output);
		/* prints a call that is referencing the programmer's defined behavior of certain set of Vulkan functions that is described in layer.cpp file  */
		void PrintCustomCall(std::ofstream* output, std::string* functName, auto* parameterList, std::string functSuffix, std::string suffix);
		/* prints a call that references the next function/point in the dispatch table */
		void PrintExecuteCall(std::ofstream* output, std::string* functType, std::string* functName, auto* parameterList, std::string cmdListType);
		/* prints a simple command that sends the input message to the VkDebuggerApp */
		void PrintSendToUI(std::ofstream* output, std::string input);
		/* prints the Vulkan functions that are caught by this layer during the runtime */
		void generateForCmdList(std::list<XmlParser::xCommand> inputCmdList, std::string cmdListType);
		/* prints the initialization functions that refer to the functions generated by this layer */
		void generateCmdProcCalls(std::list<XmlParser::xCommand> inputCmdList);
		/* prints all the instance/device function addresses included in this layer */
		void generateCmdDeclarations(std::string cmdListType);
		/* print a single function address reference */
		void generateCmdDeclaration(std::list<XmlParser::xCommand> inputCmdList, std::string cmdListType);
		/* 
		* prints the Vulkan vkGetProcInstanceAddr and vkGetDevuceInstanceAddr for layer initialization
		* and also everything to close generated file up, this is always generated at the end of the generated layer source file
		*/
		void generateGenLayerFooter(std::string cmdListType);

		/* HELPER FUNCTIONS */
		/* fetch the platform's macro definition that enables the use of the certain set of functions */
		std::string GetGuardString(std::string commandName, std::list<XmlParser::xGuard> cmdGuard);
		/* identifies whether the input type is Vulkan enum typed */
		bool IsEnum(std::string type);
		/* identifies whether the input type is Vulkan struct typed */
		bool IsStruct(std::string type);
		
		/* XmlParser object that collects the data, for this generator, out of vk.xml */
		XmlParser xmlp;
		/* true, if xml has found the platform specific functions */
		bool cmdPlatformProtection;
		/* true, if xml has found Vulkan function definitions */
		bool cmdDefinitions;
		/*
		* target files:
		* dispatchTableFile -- dispatch table for layer's proper functionality and initialization
		* generatedLayerFile -- the generated debugger layer
		* 
		* source file:
		* staticLayerFile -- behavior of caught Vulkan functions that are too specific or different from the others to generate
		*/
		std::ofstream dispatchTableFile;
		std::ofstream generatedLayerFile;
		std::ifstream staticLayerFile;
	};

}

/*
* Name		    : layerGenerator.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file of the layer generator to construct layerGenerated.hpp and layerGenerated.cpp
*
* Author		: Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#pragma once

#include "layerXmlParser.hpp"
#include <string>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
/* AI-Assisted: Claude Opus 5.5: syntax */
#include <vector>
#include <sstream>
/* AI-Assisted: end */

namespace vkdebug::generator {
    class layerGenerator {
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
		"layerGenerated"

        /* AI-Assisted: Claude Opus 5.5: boilerplate */
#ifndef VKDBG_SOURCE_DIR
#error "VKDBG_SOURCE_DIR must be set by CMake (project root folder)"
#endif
        /* AI-Assisted: end */

        /* path, where DT will be generated */
        /* AI-Assisted: Claude Opus 5.5: boilerplate */
#define VK_TABLE_PATH \
		VKDBG_SOURCE_DIR "/layer/include/" LAYER_GENERATED_NAME ".hpp"
        /* AI-Assisted: end */

        /* path, where generated layer will be generated */
        /* AI-Assisted: Claude Opus 5.5: boilerplate */
#define LAYER_GENERATED_PATH \
		VKDBG_SOURCE_DIR "/layer/src/" LAYER_GENERATED_NAME ".cpp"
        /* AI-Assisted: end */

        /* path to a file, that includes manual definitions of certain Vulkan functions */
        /* AI-Assisted: Claude Opus 5.5: boilerplate */
#define LAYER_STATIC_PATH \
		VKDBG_SOURCE_DIR "/layer/src/layerStatic.cpp"
        /* AI-Assisted: end */

        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] path to a file, that is generated when ran: when created, it contains enumerated vulkan functions */
#define FUNCTION_IDS_PATH \
		VKDBG_SOURCE_DIR "/transport/include/vkdbgFunctionsGenerated.hpp"

        /* [xbilko03] path to a file, that is generated when ran: when created, it contains decoder logic for each vulkan function */
#define DECODER_PATH \
		VKDBG_SOURCE_DIR "/appUI/src/communication/vkdbgDecoderGenerated.cpp"
        /* AI-Assisted: end */

        /* path to a file, where Vulkan API definitions are, this is the source of the code generation for this layer */
        /* AI-Assisted: Claude Opus 5.5: syntax */
#define XML_PATH \
		VKDBG_SOURCE_DIR "/libs/vulkan/vk.xml"
        /* AI-Assisted: end */

#define HEADER_NOTE_DT \
		"/*\n" \
		"* Name		    : layerGenerated.hpp\n" \
		"* Project	    : A Debugging Tool for Vulkan API (VkDebugger)\n" \
		"* Description  : Header file of the generated layer defining IDT and DDT structs\n" \
		"* [Careful! This file is generated] \n" \
		"* due to this, there is little handholding when reading this code \n" \
		"* to attempt and make up for this, here is the basic structure to help you understand \n" \
		"*\n"\
		"* in this file, there are IDT and DDT structures defined \n"\
		"* VkLayerInstanceDispatchTable_ and VkLayerDeviceDispatchTable_\n"\
		"* they are used in the layerGenerated.cpp file to make dispatch chain initialization successful\n" \
		"* \n"\
		"* Based on the template available online by : Baldur Karlsson(baldurk) and Johannes Kuhlmann's (jkuhlmann) sample_layer \n" \
		"* https ://github.com/baldurk/sample_layer/blob/master \n" \
		"* License       : BSD 2-Clause, see licenses/LICENSE_BSD\n" \
		"* No relevant changes, it is just expanded by : Jozef Bilko (xbilko03), supervised by Ing. Jan Pe\xC4\x8Diva Ph.D.\n" \
		"*/\n"

#define HEADER_NOTE_GL \
		"/*\n" \
		"* Name		    : layerGenerated.cpp\n" \
		"* Project	    : A Debugging Tool for Vulkan API (VkDebugger)\n" \
		"* Description  : Source file of the generated layer, this includes layer initialization, list of intercepted functions along their behavior\n" \
		"* [Careful! This file is generated] \n" \
		"* due to this, there is little handholding when reading this code \n" \
		"* to attempt and make up for this, here is the basic structure to help you understand \n" \
		"*\n"\
		"* CreateDeviceDispatch & CreateInstanceDispatch -- these fill the DT structs defined in layerGenerated.hpp\n"\
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
		"* License       : BSD 2-Clause, see licenses/LICENSE_BSD\n" \
		"* Changes made by : Jozef Bilko (xbilko03), supervised by Ing. Jan Pe\xC4\x8Diva Ph.D.\n" \
		"*/\n"

        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] code generated at the start of layerGenerated.hpp */
#define INSTANCE_TABLE_HEADER \
        HEADER_NOTE_DT \
		"#pragma once\n\nnamespace vkdebug::layer {\n\
        \n#if !defined(PFN_GetPhysicalDeviceProcAddr) \
        \n\ttypedef PFN_vkVoidFunction(VKAPI_PTR* PFN_GetPhysicalDeviceProcAddr)(VkInstance instance, const char* pName); \
        \n#endif \
        \n\ntypedef struct VkLayerInstanceDispatchTable_ { \
        \n #if !defined(PFN_GetPhysicalDeviceProcAddr) \
        \nPFN_GetPhysicalDeviceProcAddr GetPhysicalDeviceProcAddr; \
        \n#endif"
        /* AI-Assisted: end */

#define INSTANCE_TABLE_FOOTER \
		"\n } VkLayerInstanceDispatchTable;"

#define DEVICE_TABLE_HEADER \
		"\n#define DEVICE_DISP_TABLE_MAGIC_NUMBER 0x10ADED040410ADEDUL\
        \ntypedef struct VkLayerDispatchTable_ {\
        \nuint64_t magic;"

        /* AI-Assisted: Claude Opus 5.5: syntax */
#define DEVICE_TABLE_FOOTER \
		"\n} VkLayerDispatchTable;\n\n}\n"
        /* AI-Assisted: end */

        /* commands that will not be generated and put into the layer as per programmer's choice */
#define IGNORED_COMMANDS \
		"vkGetPhysicalDeviceRefreshableObjectTypesKHR", \
		"vkCmdSetLineStippleKHR", \
		"vkGetFaultData", \
		"vkCmdRefreshObjectsKHR", \
		"vkGetCommandPoolMemoryConsumption", \
		"vkCmdSetRenderingAttachmentLocationsKHR", \
		"vkCmdSetRenderingInputAttachmentIndicesKHR"
        std::list<std::string> ignoreCmdList = {IGNORED_COMMANDS};

        /* commands that will not be generated and put into the layer, however, both DTs will include these calls, it is because they are supposed to be defined in layerStatic */
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
        std::list<std::string> ignoreLayerCmdList = {LAYER_LEVEL_IGNORE_COMMANDS};

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

        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] one step of writing (at layer) / reading (at vkDebuggerUI) a parameter
         * parameterStepType = what step does: StepValue, StepBits, StepPointer, StepEndPointer
         * expression = c++ code of the parameter, for example pCreateInfo->size
         * label = label of given parameter in vkDebugger UI
         * type = c++ code of the parameter's type, for example VkDeviceSize
         */
        struct parameterStep {
            enum parameterStepKind { StepValue, StepBits, StepPointer, StepEndPointer };

            parameterStepKind parameterStepType;
            std::string expression;
            std::string label;
            std::string type;
        };

        /* [xbilko03] adds parameter step to the list */
        void addParameterStep(std::vector<parameterStep> &parameterStepsList,
                              parameterStep::parameterStepKind parameterStepType, std::string expression,
                              std::string label, std::string type);

        /* [xbilko03] transform all parameters from a list into vector of parameterSteps */
        std::vector<parameterStep> createParameterSteps(std::list<xmlParser::xParameter> *parameterList);

        /* [xbilko03] create steps from given parameter */
        void createParameterStepsSingle(std::vector<parameterStep> &parameterStepsList, std::string type,
                                        std::string name, std::string prefix);

        /* [xbilko03] create steps for struct type of parameter */
        void createParameterStepsStruct(std::vector<parameterStep> &parameterStepsList, std::string type,
                                        std::string name, std::string prefix, int attempts);

        /* [xbilko03] generate code that writes parameters */
        void generateEncoderForLayer(std::ofstream *output, const std::vector<parameterStep> &parameterStepsList);

        /* [xbilko03] generate code that reads written parameters */
        void generateDecoderForUi(std::ostream *output, const std::vector<parameterStep> &parameterStepsList);

        /* [xbilko03] generate code that lists vulkan functions as enumerated numbers, these are used to classify calls in layer -> UI communication */
        void generateFunctionIds();

        /* [xbilko03] take function name and append 'VKDBG_FN_', for example vkMapMemory -> VKDBG_FN_vkMapMemory */
        std::string functionId(const std::string &functName);

        /* AI-Assisted: end */
        /* prints all parameters from a list into a line */
        void printParameters(std::ofstream *output, auto *parameterList, bool typesIncluded);

        /* prints a single parameter from a list */
        void printParameter(std::ofstream *output, auto *parameterList, uint32_t index, bool typesIncluded);

        /* starts the definition of a given Vulkan function by printing it into the generated layer */
        void printCallDefinition(std::ofstream *output, std::string *functType, std::string *functName,
                                 auto *parameterList);

        /* prints a call that is referencing the programmer's defined behavior of certain set of Vulkan functions that is described in layer.cpp file  */
        void printCustomCall(std::ofstream *output, std::string *functName, auto *parameterList,
                             std::string functSuffix, std::string suffix);

        /* prints a call that references the next function/point in the dispatch table */
        void printExecuteCall(std::ofstream *output, std::string *functType, std::string *functName,
                              auto *parameterList, std::string cmdListType);

        /* prints the Vulkan functions that are caught by this layer during the runtime */
        void generateForCmdList(std::list<xmlParser::xCommand> inputCmdList, std::string cmdListType);

        /* prints the initialization functions that refer to the functions generated by this layer */
        void generateCmdProcCalls(std::list<xmlParser::xCommand> inputCmdList);

        /* prints all the instance/device function addresses included in this layer */
        void generateCmdDeclarations(std::string cmdListType);

        /* print a single function address reference */
        void generateCmdDeclaration(std::list<xmlParser::xCommand> inputCmdList, std::string cmdListType);

        /* prints the Vulkan vkGetProcInstanceAddr and vkGetDevuceInstanceAddr for layer initialization and also everything to close generated file up, this is always generated at the end of the generated layer source file */
        void generateGenLayerFooter(std::string cmdListType);

        /* HELPER FUNCTIONS */
        /* fetch the platform's macro definition that enables the use of the certain set of functions */
        std::string getGuardString(std::string commandName, std::list<xmlParser::xGuard> cmdGuard);

        /* identifies whether the input type is Vulkan enum typed */
        bool isEnum(std::string type);

        /* identifies whether the input type is Vulkan struct typed */
        bool isStruct(std::string type);

        /* xmlParser object that collects the data, for this generator, out of vk.xml */
        xmlParser xmlp;
        /* true, if xml has found the platform specific functions */
        bool cmdPlatformProtection = false;
        /* true, if xml has found Vulkan function definitions */
        bool cmdDefinitions = false;
        /* target files: dispatchTableFile -- dispatch table for layer's proper functionality and initialization generatedLayerFile -- the generated debugger layer source file: staticLayerFile -- behavior of caught Vulkan functions that are too specific or different from the others to generate */
        std::ofstream dispatchTableFile;
        std::ofstream generatedLayerFile;
        std::ifstream staticLayerFile;
        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] generated file containing vulkan function IDs */
        std::ofstream functionIdsFile;
        /* [xbilko03] generated file to decode call's parameters in vkDebugger UI */
        std::ofstream decoderFile;
        /* [xbilko03] string buffer, it stores c++ code for decoding each vulkan function's input parameters (so the generator does not need to read all functions twice), this code is written to decoder */
        std::ostringstream decoderInputCases;
        std::map<std::string, uint16_t> functionIds;
        /* AI-Assisted: end */
    };
}

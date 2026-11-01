# AI-Assisted: Claude Opus 5.5: boilerplate
cmake_minimum_required (VERSION 3.14.0)
# AI-Assisted: end

set(project_name VkDebugger)
set(generator_name vkGen)
project(${project_name} C CXX)
if(NOT "${CMAKE_CXX_STANDARD}")
	set(CMAKE_CXX_STANDARD 20)
endif()

# imgui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/libs/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)

# app src
add_executable(${project_name}
${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp 
${IMGUI_DIR}/imgui.cpp 
${IMGUI_DIR}/imgui_draw.cpp 
${IMGUI_DIR}/imgui_demo.cpp 
${IMGUI_DIR}/imgui_tables.cpp 
${IMGUI_DIR}/imgui_widgets.cpp
 "appUI/src/main.cpp"
 "appUI/include/debuggerUI.hpp"
 "appUI/src/debuggerUI.cpp"

 "appUI/include/renderer.hpp"
 "appUI/src/renderer.cpp"


 "appUI/include/communication/winsock.hpp"
 "appUI/src/communication/winsock.cpp"

 "appUI/src/communication/events.cpp"
 "appUI/include/communication/events.hpp"

 "appUI/include/data/apiCallsManager.hpp"

 "appUI/include/data/memoryManager.hpp"
 "appUI/src/data/memoryManager.cpp"

 "appUI/include/data/bufferManager.hpp"
 "appUI/src/data/bufferManager.cpp"

 "appUI/include/data/imageManager.hpp"
 "appUI/src/data/imageManager.cpp"

 "appUI/include/data/systemManager.hpp"
 "appUI/src/data/systemManager.cpp"

 # AI-Assisted: Claude Opus 5.5: boilerplate
 "transport/include/vkdbgFormat.hpp"
 "transport/include/vkdbgFunctionsGenerated.hpp"
 "appUI/include/communication/vkdbgDecoder.hpp"
 "appUI/src/communication/vkdbgDecoderGenerated.cpp"
 "appUI/src/communication/vkdbgDecoderStatic.cpp"
 # AI-Assisted: end
)

# vulkan
# AI-Assisted: Claude Opus 5.5: boilerplate
# found through the VULKAN_SDK environment variable set by the Vulkan SDK installer
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})
# AI-Assisted: end
target_link_libraries(${project_name} ${Vulkan_LIBRARIES})

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# AI-Assisted: Claude Opus 5.5: boilerplate
# downloaded and built together with the project at the first configure
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
if(POLICY CMP0135)
	cmake_policy(SET CMP0135 NEW)
endif()
include(FetchContent)
FetchContent_Declare(glfw
	URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
	URL_HASH SHA256=b5ec004b2712fd08e8861dc271428f048775200a2df719ccf575143ba749a3e9)
FetchContent_MakeAvailable(glfw)
target_link_libraries(${project_name} glfw wsock32 ws2_32)
# AI-Assisted: end

# layer
# AI-Assisted: Claude Opus 5.5: boilerplate
include_directories("layerGen/include/" "layer/include/")
# AI-Assisted: end

# generator
add_executable(${generator_name}
"layerGen/src/layerGenerator.cpp"
"layerGen/include/layerGenerator.hpp"
"layerGen/src/layerXmlParser.cpp"
"layerGen/include/layerXmlParser.hpp"
"libs/pugixml/pugixml.cpp"
"layerGen/src/main.cpp")
include_directories("libs/pugixml/")
# AI-Assisted: Claude Opus 5.5: boilerplate
target_compile_definitions(${generator_name} PRIVATE VKDBG_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
# AI-Assisted: end

# generates layer files + communication helpers
# generates only if not found or 'generator/layerStatic.cpp/vk.xml' was edited
# AI-Assisted: Claude Opus 5.5: boilerplate
set(GENERATED_FILES
"${CMAKE_SOURCE_DIR}/layer/src/layerGenerated.cpp"
"${CMAKE_SOURCE_DIR}/layer/include/layerGenerated.hpp"
"${CMAKE_SOURCE_DIR}/transport/include/vkdbgFunctionsGenerated.hpp"
"${CMAKE_SOURCE_DIR}/appUI/src/communication/vkdbgDecoderGenerated.cpp")
add_custom_command(
	OUTPUT ${GENERATED_FILES}
	COMMAND ${generator_name}
	DEPENDS ${generator_name} "${CMAKE_SOURCE_DIR}/libs/vulkan/vk.xml" "${CMAKE_SOURCE_DIR}/layer/src/layerStatic.cpp"
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "Generating the layer, function numbers and UI decoder (vkGen)"
)
add_custom_target(layer-codegen DEPENDS ${GENERATED_FILES})
# AI-Assisted: end

# layer
option(BUILD_SHARED_LIBS ON)
add_library(VkDebuggerLayer SHARED
${Vulkan_LIBRARIES}
"layer/include/layer.hpp"
"layer/src/layer.cpp"
"layer/include/layerStrings.hpp"
"layer/src/layerStrings.cpp"
"layer/src/layerGenerated.cpp"
"layer/include/layerGenerated.hpp"

"layer/include/layerData.hpp"
"layer/src/layerData.cpp"
# AI-Assisted: Claude Opus 5.5: boilerplate
"layer/include/layerControl.hpp"
"layer/src/layerControl.cpp"
"layer/include/layerWriter.hpp"
"layer/src/layerWriter.cpp"
"transport/include/vkdbgFormat.hpp"
"transport/include/vkdbgFunctionsGenerated.hpp"
# AI-Assisted: end
 "appUI/include/communication/winsock.hpp"
 "appUI/src/communication/winsock.cpp")
target_link_libraries(VkDebuggerLayer wsock32 ws2_32 ${Vulkan_LIBRARIES})
# AI-Assisted: Claude Opus 5.5: boilerplate
add_dependencies(VkDebuggerLayer layer-codegen)
add_dependencies(${project_name} layer-codegen)
# AI-Assisted: end

# data transport
# AI-Assisted: Claude Opus 5.5: boilerplate
add_executable(VkDebuggerAgent
"agent/src/agent.cpp"
"transport/include/vkdbgFormat.hpp")
target_link_libraries(VkDebuggerAgent ws2_32)
# AI-Assisted: end

# layer manifest next to the built layer, install.bat expects it there
# AI-Assisted: Claude Opus 5.5: boilerplate
configure_file("${CMAKE_SOURCE_DIR}/vkDebugger_windows.json" "${CMAKE_BINARY_DIR}/vkDebugger_windows.json" COPYONLY)
# AI-Assisted: end


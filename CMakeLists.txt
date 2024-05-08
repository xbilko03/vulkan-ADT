# CMakeList.txt : CMake project for vk details, include source and define
cmake_minimum_required (VERSION 3.11.0)

set(project_name VkDebugger)
set(generator_name vkGen)
project(${project_name} C CXX)
if(NOT "${CMAKE_CXX_STANDARD}")
	set(CMAKE_CXX_STANDARD 20)
endif()

# imgui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/libs/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)
include_directories("${CMAKE_SOURCE_DIR}/libs/stbimage")

# app src
add_executable(${project_name}
${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp 
${IMGUI_DIR}/imgui.cpp 
${IMGUI_DIR}/imgui_draw.cpp 
${IMGUI_DIR}/imgui_demo.cpp 
${IMGUI_DIR}/imgui_tables.cpp 
${IMGUI_DIR}/imgui_widgets.cpp
 "appUI/main.cpp"
 "appUI/appUI.hpp"
 "appUI/appUI.cpp"
 
 "appUI/appWindow.hpp"
 "appUI/appWindow.cpp"

 "appUI/callbacks.cpp"
 "appUI/callbacks.h"
 
 "appUI/winsock.h"
 "appUI/winsock.cpp"
 
 "appUI/events.cpp"
 "appUI/events.hpp"
 
 "appUI/class_ApiCall.hpp"
 "appUI/class_apiCall.cpp"
 
 "appUI/class_memory.hpp"
 "appUI/class_memory.cpp"
 
 "appUI/class_buffer.hpp"
 "appUI/class_buffer.cpp"
 
 "appUI/class_image.hpp"
 "appUI/class_image.cpp"

 "appUI/class_system.hpp"
 "appUI/class_system.cpp"
)

# vulkan
set(ENV{VULKAN_SDK} "C:/VulkanSDK/1.3.275.0")
find_package(Vulkan REQUIRED FATAL_ERROR)
include_directories("C:/VulkanSDK/1.3.275.0/include")
target_link_libraries(${project_name} ${Vulkan_LIBRARIES})

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
include_directories("C:/GLFW/glfw-3.4.bin.WIN64/include")
set(Glfw_LIBRARY "C:/GLFW/glfw-3.4.bin.WIN64/lib/glfw3.lib")
target_link_libraries(${project_name} ${Glfw_LIBRARY} wsock32 ws2_32)

# layer
include_directories("libs/vklayer/" "layergen/" "layer/")

# generator
add_executable(${generator_name} 
"layergen/layer_generator.cpp"
"layergen/layer_generator.hpp"
"layergen/layer_xml_parser.cpp"
"layergen/layer_xml_parser.hpp"
"libs/pugixml/pugixml.cpp"
"layergen/main.cpp")
include_directories("libs/pugixml/")

# layer
option(BUILD_SHARED_LIBS ON)
add_library(dbgrvlayer SHARED 
${Glfw_LIBRARY}
${Vulkan_LIBRARIES}
"layer/layer.hpp"
"layer/layer.cpp"
"layer/layer_messages.h"
"layer/layer_messages.cpp"
"layer/layer_generated.cpp"
"layer/layer_generated.h"

"layer/layer_data.hpp"
"layer/layer_data.cpp"
 "appUI/winsock.h"
 "appUI/winsock.cpp")
target_link_libraries(dbgrvlayer wsock32 ws2_32 ${Vulkan_LIBRARIES})


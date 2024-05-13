# CMakeList.txt : CMake project for vk details, include source and define
cmake_minimum_required (VERSION 3.11.0)

set(project_name VkDebugger)
set(generator_name vkGen)
project(${project_name} C CXX)
set(CMAKE_CXX_STANDARD 20)

# imgui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/libs/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)

# vulkan
# fix this path in case of failure
set(ENV{VULKAN_SDK} "libs/vulkanSDK/vulkan")
# fix this path in case of failure
include_directories("libs/vulkanSDK/vulkan/include")
set(Vulkan_LIBRARIES "${CMAKE_SOURCE_DIR}/libs/vulkanSDK/vulkan/lib/libvulkan.so" "${CMAKE_SOURCE_DIR}/libs/vulkanSDK/vulkan/lib/libvulkan.so.1")

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
 
 "appUI/class_apiCall.hpp"
 "appUI/class_apiCall.cpp"
 
 "appUI/class_memory.hpp"
 "appUI/class_memory.cpp"
 
 "appUI/class_buffer.hpp"
 "appUI/class_buffer.cpp"
 
 "appUI/class_image.hpp"
 "appUI/class_image.cpp"

 "appUI/class_system.hpp"
 "appUI/class_system.cpp"
 ${Vulkan_LIBRARIES}
)
target_link_libraries(${project_name} ${Vulkan_LIBRARIES})

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# fix this path in case of failure

find_package(glfw3 3.3 REQUIRED)
	
include_directories("libs/glfw/include")
# fix this path in case of failure
IF(WIN32)
	target_link_libraries(${project_name} glfw wsock32 ws2_32)
ELSE()
	target_link_libraries(${project_name} glfw)
ENDIF()

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
add_library(VkDebuggerLayer SHARED 
${Glfw_LIBRARY}
"layer/layer.h"
"layer/layer.cpp"
"layer/layer_messages.h"
"layer/layer_messages.cpp"
"layer/layer_generated.cpp"
"layer/layer_generated.h"

"layer/layer_data.hpp"
"layer/layer_data.cpp"
 "appUI/winsock.h"
 "appUI/winsock.cpp")
IF(WIN32)
	target_link_libraries(VkDebuggerLayer wsock32 ws2_32 ${Vulkan_LIBRARIES})
ELSE()
	target_link_libraries(VkDebuggerLayer ${Vulkan_LIBRARIES})
ENDIF()

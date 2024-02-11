# CMakeList.txt : CMake project for dbgrv2, include source and define
# project specific logic	re.
#
cmake_minimum_required (VERSION 2.8)

set(project_name vkDetails)
project(${project_name} C CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)

set(includes
PUBLIC ${Vulkan_INCLUDE_DIR}
${CMAKE_SOURCE_DIR}/libs/imgui
${CMAKE_SOURCE_DIR}/libs/imgui/backends
${CMAKE_SOURCE_DIR}/libs/imgui/imgui.h
${CMAKE_SOURCE_DIR}/libs/imgui/imgui.cpp
${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_vulkan.cpp
${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_vulkan.h
)
set(libraries ${Vulkan_LIBRARIES})
set(headers "appUI/winsock.h" "appUI/events.h")

add_executable(${project_name} "appUI/appUI.cpp" "appUI/winsock.cpp" "appUI/events.cpp" ${headers})
include_directories(PUBLIC ${includes})
link_libraries(${libraries})

# Enable Hot Reload for MSVC compilers if supported.
#if (POLICY CMP0141)
#  cmake_policy(SET CMP0141 NEW)
#  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
#endif()

#include_directories(${CMAKE_SOURCE_DIR}/libs/vklayer)

# VULKAN

#add_library(glfw SHARED IMPORTED)
#set_target_properties(glfw PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/glfw-3.3.9.bin.WIN64/lib-vc2022/glfw3.lib")

#set(imgui
#${CMAKE_SOURCE_DIR}/libs/imgui/imgui_tables.cpp
#${CMAKE_SOURCE_DIR}/libs/imgui/imgui_demo.cpp
#${CMAKE_SOURCE_DIR}/libs/imgui/imgui_draw.cpp
#${CMAKE_SOURCE_DIR}/libs/imgui/imgui_internal.h
#${CMAKE_SOURCE_DIR}/libs/imgui/imgui_widgets.cpp
#${CMAKE_SOURCE_DIR}/libs/imgui/imstb_rectpack.h
#${CMAKE_SOURCE_DIR}/libs/imgui/imstb_textedit.h
#${CMAKE_SOURCE_DIR}/libs/imgui/imconfig.h
#${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_glfw.cpp
#${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_glfw.h
#)

#target_include_directories(vkdebugger ${CMAKE_SOURCE_DIR}/libs/glfw-3.3.9.bin.WIN64/include)
#target_link_libraries(vkdebugger ${Vulkan_LIBRARIES}) # glfw
#target_include_directories(vkdebugger PUBLIC ${VULKAN_INCLUDE_DIRS})

#add_executable(vkdebugger "appUI/appUI.cpp" ${VK_INCLUDE_DIRS} ${imgui}) #

#option(BUILD_SHARED_LIBS ON)
#add_library(dbgrvlayer SHARED ${CMAKE_SOURCE_DIR}/layer/layer.cpp  "appUI/winsock.h" "appUI/winsock.cpp" "appUI/events.cpp" "appUI/events.h")


#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET vkdebugger PROPERTY CXX_STANDARD 20)
#endif()

# TODO: Add tests and install targets if needed.

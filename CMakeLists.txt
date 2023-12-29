# CMakeList.txt : CMake project for dbgrv2, include source and define
# project specific logic	re.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

include_directories(${CMAKE_SOURCE_DIR}/libs/vulkan1.3.261.1/Include)
include_directories(${CMAKE_SOURCE_DIR}/libs/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/vklayer)

add_library(vulkan SHARED IMPORTED)
add_library(glfw SHARED IMPORTED)
set_target_properties(vulkan PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/vulkan1.3.261.1/Lib/vulkan-1.lib")
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/glfw/lib-vc2022/glfw3.lib")

include_directories(${CMAKE_SOURCE_DIR}/libs/imgui)
include_directories(${CMAKE_SOURCE_DIR}/libs/imgui/backends)
set(imgui 
${CMAKE_SOURCE_DIR}/libs/imgui/imgui.cpp
${CMAKE_SOURCE_DIR}/libs/imgui/imgui.h
${CMAKE_SOURCE_DIR}/libs/imgui/imgui_tables.cpp
${CMAKE_SOURCE_DIR}/libs/imgui/imgui_demo.cpp
${CMAKE_SOURCE_DIR}/libs/imgui/imgui_draw.cpp
${CMAKE_SOURCE_DIR}/libs/imgui/imgui_internal.h
${CMAKE_SOURCE_DIR}/libs/imgui/imgui_widgets.cpp
${CMAKE_SOURCE_DIR}/libs/imgui/imstb_rectpack.h
${CMAKE_SOURCE_DIR}/libs/imgui/imstb_textedit.h
${CMAKE_SOURCE_DIR}/libs/imgui/imconfig.h
${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_opengl2.cpp
${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_opengl2.h
${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_glfw.cpp
${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_glfw.h
)

find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS})

project ("dbgrv2")

# Add source to this project's executable.
add_executable (dbgrv2 "appUI/appUI.cpp" "appUI/appUI.h" ${imgui} "layer/winsock.h" "layer/winsock.cpp")
target_link_libraries(dbgrv2 vulkan glfw ${OPENGL_LIBRARIES})

option(BUILD_SHARED_LIBS ON)
add_library(dbgrv2layer SHARED ${CMAKE_SOURCE_DIR}/layer/layer.cpp ${CMAKE_SOURCE_DIR}/layer/layer.h "layer/winsock.h" "layer/winsock.cpp")


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET dbgrv2 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

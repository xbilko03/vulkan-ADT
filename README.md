# VkDebugger
A Debugging Tool for Vulkan API

## DESCRIPTION
VkDebugger is an application dedicated to help with debugging of 2D/3D Vulkan graphics applications. It is implemented in a form of a Vulkan layer
and an application (VkDebuggerApp) with UI that makes the debugging easier. This tool collects data from a program and crafts a current state 
representation based on what is collected, this representation is then accessible to user via the already mentioned UI.

A part of this tool is a code generator that is used to list the full vulkan API function calls list based on provided XML file.

## Requirements
The program uses following technologies to make it work:
* C++ 20 
* Vulkan API 1.3.275.0
* CMAKE 3.11.0
* GLFW 3.4
* PugiXml 1.14
* Winsock 2.2

## PLATFORMS
As per version 1.3 of VkDebugger the program is working and has been tested on:
* Windows 10/11

## FEATURES
As per version 1.3 of VkDebugger the following features are available:
* tracing Vulkan API calls
* tracing the state of VkMemory, VkImages and VkBuffers
* inspection of VkMemory, VkImages and VkBuffers and its contents
* texture inspection for VkImages
* simple warnings when a condition is met
* simple breakponts when a condition is met

## SETUP
Can be found within the following .pdf file:

[setup.pdf {SLOVAK}](https://github.com/xbilko03/ADT_VAPI/blob/master/setup.pdf)
    
## USE-EXAMPLE
![alt text](https://github.com/xbilko03/ADT_VAPI/blob/master/view.jpg?raw=true)

## AUTHOR
Jozef Bilko (xbilko03)

## LICENSE
Can be found within this project's licenses folder or at:

    https://opensource.org/license/MIT

    https://opensource.org/license/bsd-2-clause

## LAST-UPDATE
8th of May 2024

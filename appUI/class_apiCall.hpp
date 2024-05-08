/*
* Name		    : class_apiCall.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file for data class that represents a single Vulkan function call
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#pragma once
#include <list>
#include <string>

namespace details {
    class apiCall {
    public:
        /* construct new call */
        apiCall(unsigned long long inputID) { this->ID = inputID; }

        /* skip prefix when getting the name e.g. begin_apiCallName() */
        std::string getName() { return this->name.substr(6, this->name.size()); }
        unsigned long long getID() { return this->ID; }
        /* VkResult of the operation */
        int getRetVal() { return this->returnVal; }

        /* parse a stream of function parameters and have them saved */
        void assignParameter(std::string input);
        void assignName(std::string input) { this->name = input; }
        void assignRetVal(std::string input) { this->returnVal = atoi(input.c_str()); }

        /* api call's parameters */
        struct ParamStr {
            std::string name;
            std::string value;
        };
        std::list<ParamStr> getParameters() { return this->parameters; }
    private:
        std::list<ParamStr> parameters;
        std::string name;
        unsigned long long ID;
        /* VkResult of the operation */
        int returnVal;
    };
}
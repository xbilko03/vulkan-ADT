/*
* Name		    : apiCallsManager.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Header file for data class that represents a single Vulkan function call
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
#pragma once
#include <list>
#include <string>

namespace vkdebug::ui {
    class apiCall {
    public:
        /* construct new call */
        apiCall(unsigned long long inputID) { this->ID = inputID; }

        /* skip prefix when getting the name e.g. begin_apiCallName() */
        std::string getName() { return this->name.substr(6, this->name.size()); }
        unsigned long long getID() { return this->ID; }
        /* VkResult of the operation */
        int getRetVal() { return this->returnVal; }

        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] split input string to label and value, then append this as a parameter to the given call */
        void assignParameter(const std::string &input) {
            size_t pos = input.find('=');
            parameters.push_back({input.substr(0, pos), pos == std::string::npos ? "" : input.substr(pos + 1)});
        }
        /* AI-Assisted: end */

        void assignName(std::string input) { this->name = input; }
        void assignRetVal(std::string input) { this->returnVal = atoi(input.c_str()); }

        /* api call's parameters */
        struct paramStr {
            std::string name;
            std::string value;
        };

        std::list<paramStr> getParameters() { return this->parameters; }
        /* AI-Assisted: Claude Opus 5.5: syntax */
        /* [xbilko03] true from MessageCallBegin message until MessageCallEnd */
        bool getBeforeExecution() { return this->beforeExecution; }
        void assignBeforeExecution(bool input) { this->beforeExecution = input; }
        /* AI-Assisted: end */
    private:
        std::list<paramStr> parameters;
        std::string name;
        unsigned long long ID;
        /* VkResult of the operation */
        int returnVal;
        /* AI-Assisted: Claude Opus 5.5: syntax */
        bool beforeExecution = false;
        /* AI-Assisted: end */
    };
}

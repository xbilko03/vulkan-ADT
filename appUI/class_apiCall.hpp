#pragma once
#include <list>
#include <string>

namespace details {
    class apiCall {
    public:
        apiCall(unsigned long long inputID) { this->ID = inputID; }

        struct ParamStr {
            std::string name;
            std::string value;
        };

        std::list<ParamStr> getParameters() { return this->parameters; }
        std::string getName() { return this->name; }
        unsigned long long getID() { return this->ID; }
        int getRetVal() { return this->returnVal; }

        void assignParameter(std::string input);
        void assignName(std::string input) { this->name = input; }
        void assignRetVal(std::string input) { this->returnVal = atoi(input.c_str()); }
    private:
        std::list<ParamStr> parameters;
        std::string name;
        unsigned long long ID;
        int returnVal;
    };
}
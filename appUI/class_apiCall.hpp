#pragma once
#include <list>
#include <string>

namespace details {
    class apiCall {
    public:
        struct ParamStr {
            std::string name;
            std::string value;
        };

        std::list<ParamStr> getParameters() { return this->parameters; }
        std::string getName() { return this->name; }
        unsigned long long getID() { return this->ID; }
        apiCall(unsigned inputID) { this->ID = inputID; }
        void assignParameter(std::string input);
        void assignName(std::string input) { this->name = input; }
    private:
        std::list<ParamStr> parameters;
        std::string name;
        unsigned long long ID;
    };
}
#pragma once
#include <list>
#include <string>

namespace details {
    class Buffer : public apiCall {
    private:
        std::string pointer;
        std::string data;
    };
}
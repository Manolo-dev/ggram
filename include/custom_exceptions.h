#pragma once

#include <exception>
#include <string>

namespace CustomExceptions {

class OptionError : public std::exception { // base of all logic-error exceptions
public:
    explicit OptionError(const std::string& message) : _message(message) {}
    explicit OptionError(const char* message) : _message(message) {}

    const char* what() {
        return _message.c_str();
    }

private:
    const std::string _message;
};

}
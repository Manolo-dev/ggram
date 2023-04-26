#pragma once

#include <string>
#include <iostream>

enum class ErrorType {
    INVALID_FILE_EXTENSION,
    FILE_NOT_FOUND,
    NO_FILENAME_SPECIFIED,
    REGEX_ERROR,
    INVALID_SYNTAX,
    INVALID_RULE,
};

class Error {
public:
    Error(ErrorType type, std::string other);
    Error(ErrorType type, int num);
    Error(ErrorType type, std::string *other);
    void throw_error();
private:
    std::string _error;
    ErrorType _type;
};
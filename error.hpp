#pragma once

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
    Error(ErrorType type, string other);
    Error(ErrorType type, s32 num);
    Error(ErrorType type, string *other);
    void throw_error();
private:
    string error;
    ErrorType type;
};

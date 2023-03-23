#pragma once

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long int u64;
typedef signed long int s64;

typedef float f32;
typedef double f64;

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
    Error(ErrorType type, s32 num);
    Error(ErrorType type, std::string *other);
    void throw_error();
private:
    std::string _error;
    ErrorType _type;
};

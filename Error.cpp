#include "Error.hpp"

Error::Error(ErrorType type, std::string other) {
    std::string temp[1] = {other};
    *this = Error(type, temp);
}

Error::Error(ErrorType type, s32 num) {
    *this = Error(type, std::to_string(num));
}
Error::Error(ErrorType type, std::string *other) {
    std::string error = "\e[1;31mError:\e[0m ";

    switch(type){
        case ErrorType::INVALID_FILE_EXTENSION:
            error += "Invalid file extension. Only " + other[0] + " files are allowed.";
            break;
        case ErrorType::FILE_NOT_FOUND:
            error += "The file " + other[0] + " was not found.";
            break;
        case ErrorType::NO_FILENAME_SPECIFIED:
            error += "No " + other[0] + " specified.";
            break;
        case ErrorType::REGEX_ERROR:
            error += "Invalid regex on line \e[1m" + other[0] + "\e[0m: " + other[1] + "\n regex_error code: " + other[2];
            break;
        case ErrorType::INVALID_SYNTAX:
            error += "Invalid syntax on line \e[1m" + other[0] + "\e[0m: " + other[1];
            break;
        case ErrorType::INVALID_RULE:
            error += "Invalid rule on line \e[1m" + other[0] + "\e[0m";
            break;
        default:
            error += "Unknown error.";
            break;

    }
    this->error = error;
    this->type = type;
    
}

void Error::throw_error() {
    std::cerr << error << std::endl;
    exit(1);
}
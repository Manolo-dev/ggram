#include "error.hpp"
// ----------------------------- GgramError ------------------------------ //

GgramError::GgramError():
    std::runtime_error::runtime_error("GgramError") {}

GgramError::~GgramError() throw() {}

const char* GgramError::what() const throw() {
    return _message.c_str();
}

// ---------------------- InvalidFileExtensionError ---------------------- //

InvalidFileExtensionError::InvalidFileExtensionError(std::string filename, std::string extension):
    _message("Invalid file extension. Only " + extension + " files are allowed.") {}

InvalidFileExtensionError::~InvalidFileExtensionError() throw() {}

const char* InvalidFileExtensionError::what() const throw() {
    return _message.c_str();
}

// ---------------------------- FileNotFound ----------------------------- //

FileNotFound::FileNotFound(std::string filename):
    _message("File " + filename + " not found.") {}

FileNotFound::~FileNotFound() throw() {}

const char* FileNotFound::what() const throw() {
    return _message.c_str();
}

// ------------------------- NoFilenameSpecified ------------------------- //

NoFilenameSpecified::NoFilenameSpecified(std::string type_file):
    _message("No " + type_file + " filename specified.") {}

NoFilenameSpecified::~NoFilenameSpecified() throw() {}

const char* NoFilenameSpecified::what() const throw() {
    return _message.c_str();
}

// ----------------------------- RegexError ------------------------------ //

RegexError::RegexError(std::string line, std::string regex, std::string error):
    _message("Regex error: " + error + " in line " + line + " with regex " + regex) {}

RegexError::RegexError(int line, std::string regex, std::string error):
    _message("Regex error: " + error + " in line " + std::to_string(line) + " with regex " + regex) {}

RegexError::~RegexError() throw() {}

const char* RegexError::what() const throw() {
    return _message.c_str();
}

// ---------------------------- InvalidSyntax ---------------------------- //

InvalidSyntax::InvalidSyntax(std::string line, std::string error):
    _message("Invalid syntax in line " + line + " : " + error) {}

InvalidSyntax::InvalidSyntax(int line, std::string error):
    _message("Invalid syntax in line " + std::to_string(line) + " : " + error) {}

InvalidSyntax::~InvalidSyntax() throw() {}

const char* InvalidSyntax::what() const throw() {
    return _message.c_str();
}

// ----------------------------- InvalidRule ----------------------------- //

InvalidRule::InvalidRule(std::string line, std::string rule):
    _message("Invalid rule " + rule + " in line " + line) {}

InvalidRule::InvalidRule(int line, std::string rule):
    _message("Invalid rule " + rule + " in line " + std::to_string(line)) {}

InvalidRule::~InvalidRule() throw() {}

const char* InvalidRule::what() const throw() {
    return _message.c_str();
}

// ----------------------------- InputError ------------------------------ //

InputError::InputError(std::string message):
    _message(message) {}

InputError::~InputError() throw() {}

const char* InputError::what() const throw() {
    return _message.c_str();
}

// ------------------------------ BnfError ------------------------------- //

BnfError::BnfError(std::string message):
    _message(message) {}

BnfError::~BnfError() throw() {}

const char* BnfError::what() const throw() {
    return _message.c_str();
}

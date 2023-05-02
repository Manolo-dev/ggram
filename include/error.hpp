#pragma once

#include <string>
#include <stdexcept>

// ----------------------------- GgramError ------------------------------ //
class GgramError : public std::runtime_error {
private:
    const std::string _message;
public:
    GgramError();
    virtual ~GgramError() throw();
    virtual const char* what() const throw();
};

// ---------------------- InvalidFileExtensionError ---------------------- //
class InvalidFileExtensionError : public GgramError {
private:
    const std::string _message;
public:
    InvalidFileExtensionError(std::string filename, std::string extension);
    virtual ~InvalidFileExtensionError() throw();
    virtual const char* what() const throw();
};

// ---------------------------- FileNotFound ----------------------------- //
class FileNotFound : public GgramError {
private:
    const std::string _message;
public:
    FileNotFound(std::string filename);
    virtual ~FileNotFound() throw();
    virtual const char* what() const throw();
};

// ------------------------- NoFilenameSpecified ------------------------- //
class NoFilenameSpecified : public GgramError {
private:
    const std::string _message;
public:
    NoFilenameSpecified(std::string type_file);
    virtual ~NoFilenameSpecified() throw();
    virtual const char* what() const throw();
};

// ----------------------------- RegexError ------------------------------ //
class RegexError : public GgramError {
private:
    const std::string _message;
public:
    RegexError(int line, std::string regex, std::string error);
    RegexError(std::string line, std::string regex, std::string error);
    virtual ~RegexError() throw();
    virtual const char* what() const throw();
};

// ---------------------------- InvalidSyntax ---------------------------- //
class InvalidSyntax : public GgramError {
private:
    const std::string _message;
public:
    InvalidSyntax(int line, std::string error);
    InvalidSyntax(std::string line, std::string error);
    virtual ~InvalidSyntax() throw();
    virtual const char* what() const throw();
};

// ----------------------------- InvalidRule ----------------------------- //
class InvalidRule : public GgramError {
private:
    const std::string _message;
public:
    InvalidRule(int line, std::string rule);
    InvalidRule(std::string line, std::string rule);
    virtual ~InvalidRule() throw();
    virtual const char* what() const throw();
};

// ----------------------------- InputError ------------------------------ //
class InputError : public GgramError {
private:
    const std::string _message;
public:
    InputError(std::string message);
    virtual ~InputError() throw();
    virtual const char* what() const throw();
};

// ------------------------------ BnfError ------------------------------- //
class BnfError : public GgramError {
private:
    const std::string _message;
public:
    BnfError(std::string message);
    virtual ~BnfError() throw();
    virtual const char* what() const throw();
};

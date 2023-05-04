#pragma once

#include <string>
#include <stdexcept>

// ----------------------------- GgramError ------------------------------ //
class GgramError : public std::runtime_error {
private:
    const std::string _message;
public:
    GgramError();
    virtual ~GgramError() = 0;
    virtual const char* what() const throw();
};

// ---------------------- InvalidFileExtensionError ---------------------- //
class InvalidFileExtensionError : public GgramError {
private:
    const std::string _message;
public:
    InvalidFileExtensionError(const std::string&, const std::string &extension);
    ~InvalidFileExtensionError() throw() override;
    const char* what() const throw() override;
};

// ---------------------------- FileNotFound ----------------------------- //
class FileNotFound : public GgramError {
private:
    const std::string _message;
public:
    explicit FileNotFound(const std::string &filename);
    ~FileNotFound() throw() override;
    const char* what() const throw() override;
};

// ------------------------- NoFilenameSpecified ------------------------- //
class NoFilenameSpecified : public GgramError {
private:
    const std::string _message;
public:
    explicit NoFilenameSpecified(const std::string &type_file);
    ~NoFilenameSpecified() throw() override;
    const char* what() const throw() override;
};

// ----------------------------- RegexError ------------------------------ //
class RegexError : public GgramError {
private:
    const std::string _message;
public:
    RegexError(unsigned int line, const std::string &regex, const std::string &error);
    RegexError(const std::string &line, const std::string &regex, const std::string &error);
    ~RegexError() throw() override;
    const char* what() const throw() override;
};

// ---------------------------- InvalidSyntax ---------------------------- //
class InvalidSyntax : public GgramError {
private:
    const std::string _message;
public:
    InvalidSyntax(unsigned int line, const std::string &error);
    InvalidSyntax(const std::string &line, const std::string &error);
    ~InvalidSyntax() throw() override;
    const char* what() const throw() override;
};

// ----------------------------- InvalidRule ----------------------------- //
class InvalidRule : public GgramError {
private:
    const std::string _message;
public:
    InvalidRule(unsigned int line, const std::string &rule);
    InvalidRule(const std::string &line, const std::string &rule);
    ~InvalidRule() throw() override;
    const char* what() const throw() override;
};

// ----------------------------- InputError ------------------------------ //
class InputError : public GgramError {
private:
    const std::string _message;
public:
    explicit InputError(const std::string &message);
    ~InputError() throw() override;
    const char* what() const throw() override;
};

// ------------------------------ BnfError ------------------------------- //
class BnfError : public GgramError {
private:
    const std::string _message;
public:
    explicit BnfError(const std::string &message);
    ~BnfError() throw() override;
    const char* what() const throw() override;
};

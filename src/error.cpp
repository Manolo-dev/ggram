#include "error.hpp"
// ----------------------------- GgramError ------------------------------ //

GgramError::GgramError() : std::runtime_error::runtime_error("GgramError") {}

const char *GgramError::what() const noexcept {
	return _message.c_str();
}

// ---------------------- InvalidFileExtensionError ---------------------- //

InvalidFileExtensionError::InvalidFileExtensionError(
	const std::string & /*unused*/, const std::string &extension)
	: _message("Invalid file extension. Only " + extension +
			   " files are allowed.") {}

const char *InvalidFileExtensionError::what() const noexcept {
	return _message.c_str();
}

// ---------------------------- FileNotFound ----------------------------- //

FileNotFound::FileNotFound(const std::string &filename)
	: _message("File " + filename + " not found.") {}

const char *FileNotFound::what() const noexcept {
	return _message.c_str();
}

// ------------------------- NoFilenameSpecified ------------------------- //

NoFilenameSpecified::NoFilenameSpecified(const std::string &type_file)
	: _message("No " + type_file + " filename specified.") {}

const char *NoFilenameSpecified::what() const noexcept {
	return _message.c_str();
}

// ----------------------------- RegexError ------------------------------ //

RegexError::RegexError(const std::string &line, const std::string &regex,
					   const std::string &error)
	: _message("Regex error: " + error + " in line " + line + " with regex " +
			   regex) {}

RegexError::RegexError(unsigned int line, const std::string &regex,
					   const std::string &error)
	: _message("Regex error: " + error + " in line " + std::to_string(line) +
			   " with regex " + regex) {}

const char *RegexError::what() const noexcept {
	return _message.c_str();
}

// ---------------------------- InvalidSyntax ---------------------------- //

InvalidSyntax::InvalidSyntax(const std::string &line, const std::string &error)
	: _message("Invalid syntax in line " + line + " : " + error) {}

InvalidSyntax::InvalidSyntax(unsigned int line, const std::string &error)
	: _message("Invalid syntax in line " + std::to_string(line) + " : " +
			   error) {}

const char *InvalidSyntax::what() const noexcept {
	return _message.c_str();
}

// ----------------------------- InvalidRule ----------------------------- //

InvalidRule::InvalidRule(const std::string &line, const std::string &rule)
	: _message("Invalid rule " + rule + " in line " + line) {}

InvalidRule::InvalidRule(unsigned int line, const std::string &rule)
	: _message("Invalid rule " + rule + " in line " + std::to_string(line)) {}

const char *InvalidRule::what() const noexcept {
	return _message.c_str();
}

// ----------------------------- InputError ------------------------------ //

InputError::InputError(const std::string &message) : _message(message) {}

const char *InputError::what() const noexcept {
	return _message.c_str();
}

// ------------------------------ BnfError ------------------------------- //

BnfError::BnfError(const std::string &message) : _message(message) {}

const char *BnfError::what() const noexcept {
	return _message.c_str();
}

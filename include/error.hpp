#pragma once

#include <stdexcept>
#include <string>

// ----------------------------- GgramError ------------------------------ //
class GgramError : public std::runtime_error {
  private:
	const std::string _message;

  public:
	GgramError();
	~GgramError() override = default;
	[[nodiscard]] const char *what() const noexcept override;
};

// ---------------------- InvalidFileExtensionError ---------------------- //
class InvalidFileExtensionError : public GgramError {
  private:
	const std::string _message;

  public:
	InvalidFileExtensionError(const std::string & /*unused*/,
							  const std::string &extension);
	~InvalidFileExtensionError() noexcept override = default;
	[[nodiscard]] const char *what() const noexcept override;
};

// ---------------------------- FileNotFound ----------------------------- //
class FileNotFound : public GgramError {
  private:
	const std::string _message;

  public:
	explicit FileNotFound(const std::string &filename);
	~FileNotFound() noexcept override = default;
	[[nodiscard]] const char *what() const noexcept override;
};

// ------------------------- NoFilenameSpecified ------------------------- //
class NoFilenameSpecified : public GgramError {
  private:
	const std::string _message;

  public:
	explicit NoFilenameSpecified(const std::string &type_file);
	~NoFilenameSpecified() noexcept override = default;
	[[nodiscard]] const char *what() const noexcept override;
};

// ----------------------------- RegexError ------------------------------ //
class RegexError : public GgramError {
  private:
	const std::string _message;

  public:
	RegexError(unsigned int line, const std::string &regex,
			   const std::string &error);
	RegexError(const std::string &line, const std::string &regex,
			   const std::string &error);
	~RegexError() noexcept override = default;
	[[nodiscard]] const char *what() const noexcept override;
};

// ---------------------------- InvalidSyntax ---------------------------- //
class InvalidSyntax : public GgramError {
  private:
	const std::string _message;

  public:
	InvalidSyntax(unsigned int line, const std::string &error);
	InvalidSyntax(const std::string &line, const std::string &error);
	~InvalidSyntax() noexcept override = default;
	[[nodiscard]] const char *what() const noexcept override;
};

// ----------------------------- InvalidRule ----------------------------- //
class InvalidRule : public GgramError {
  private:
	const std::string _message;

  public:
	InvalidRule(unsigned int line, const std::string &rule);
	InvalidRule(const std::string &line, const std::string &rule);
	~InvalidRule() noexcept override = default;
	[[nodiscard]] const char *what() const noexcept override;
};

// ----------------------------- InputError ------------------------------ //
class InputError : public GgramError {
  private:
	const std::string _message;

  public:
	explicit InputError(const std::string &message);
	~InputError() noexcept override = default;
	[[nodiscard]] const char *what() const noexcept override;
};

// ------------------------------ BnfError ------------------------------- //
class BnfError : public GgramError {
  private:
	const std::string _message;

  public:
	explicit BnfError(const std::string &message);
	~BnfError() noexcept override = default;
	[[nodiscard]] const char *what() const noexcept override;
};

#pragma once

#include "error/ggram_error.hpp"

enum class ErrorType { InvalidRule, BnfError, RegexError, SyntaxError };

class SyntaxError : public GgramError {
  public:
    SyntaxError(const std::string &message, unsigned long line, unsigned long column,
                ErrorType type = ErrorType::SyntaxError);
    SyntaxError(const std::string &message, unsigned long line = 0,
                ErrorType type = ErrorType::SyntaxError);

    ~SyntaxError() noexcept override = default;

    [[nodiscard]] unsigned long line() const noexcept;

    [[nodiscard]] unsigned long column() const noexcept;

    [[nodiscard]] ErrorType type() const noexcept;

  private:
    unsigned long _line;
    unsigned long _column;
    ErrorType _type;
};
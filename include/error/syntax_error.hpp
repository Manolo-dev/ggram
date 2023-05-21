#pragma once

#include "error/ggram_error.hpp"
#include "file_handler.hpp"
#include <iostream>

enum class ErrorType { InvalidRule, BnfError, RegexError, SyntaxError };

class SyntaxError : public GgramError {
  public:
    SyntaxError(const std::string &message, unsigned long line, unsigned long column_start,
                unsigned long column_end, ErrorType type = ErrorType::SyntaxError);
    SyntaxError(const std::string &message, unsigned long line = 0,
                ErrorType type = ErrorType::SyntaxError);

    ~SyntaxError() noexcept override = default;

    [[nodiscard]] unsigned long line() const noexcept;

    [[nodiscard]] unsigned long columnStart() const noexcept;

    [[nodiscard]] unsigned long columnEnd() const noexcept;

    [[nodiscard]] ErrorType type() const noexcept;

    /**
     * @brief Print the error in stderr and use the input file to display the line with arrow
     * pointing to the error
     *
     * @param handler The file handler that manage the input file
     */
    void print(FileHandler &handler) const;

    constexpr static const char *error_type_to_string(ErrorType type);

  private:
    unsigned long _line;
    unsigned long _column_start;
    unsigned long _column_end;
    ErrorType _type;
};

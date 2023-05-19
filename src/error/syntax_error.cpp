#include "error/syntax_error.hpp"

SyntaxError::SyntaxError(const std::string &message, unsigned long line, unsigned long column,
                         ErrorType type)
    : GgramError(message), _line(line), _column(column), _type(type) {}

SyntaxError::SyntaxError(const std::string &message, unsigned long line, ErrorType type)
    : GgramError(message), _line(line), _column(0), _type(type) {}

unsigned long SyntaxError::line() const noexcept {
    return _line;
}

unsigned long SyntaxError::column() const noexcept {
    return _column;
}

ErrorType SyntaxError::type() const noexcept {
    return _type;
}

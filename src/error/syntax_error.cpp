#include "error/syntax_error.hpp"

SyntaxError::SyntaxError(const std::string &message, unsigned long line, unsigned long column_start, unsigned long column_end,
                         ErrorType type)
    : GgramError(message), _line(line), _column_start(column_start), _column_end(column_end), _type(type) {}

SyntaxError::SyntaxError(const std::string &message, unsigned long line, ErrorType type)
    : GgramError(message), _line(line), _column_start(0), _column_end(0), _type(type) {}

unsigned long SyntaxError::line() const noexcept {
    return _line;
}

unsigned long SyntaxError::columnStart() const noexcept {
    return _column_start;
}

unsigned long SyntaxError::columnEnd() const noexcept {
    return _column_end;
}

ErrorType SyntaxError::type() const noexcept {
    return _type;
}

constexpr const char *SyntaxError::error_type_to_string(ErrorType type) {
    switch (type) {
        case ErrorType::InvalidRule:
            return "InvalidRule";
        case ErrorType::BnfError:
            return "BnfError";
        case ErrorType::RegexError:
            return "RegexError";
        case ErrorType::SyntaxError:
            return "SyntaxError";
    }
    return "UnknownError";
}

void SyntaxError::print(FileHandler &handler) const {
    std::cerr << SyntaxError::error_type_to_string(_type) << ": " << what() << std::endl;
    if (_line == 0) {
        return;
    }
    std::cerr << "At line " << _line;
    std::string line;
    if (!handler.getline(line, _line - 1)) {
        std::cerr << std::endl;
        return;
    }
    std::cerr << " and column " << _column_start << ": \n" << std::endl;
    std::cerr << line << std::endl;

    if (_column_end == 0) {
        std::cerr << std::endl;
        return;
    }

    for (unsigned long i = 0; i < _column_start - 1; i++) {
        std::cerr << " ";
    }
	for (unsigned long i = _column_start; i < _column_end; i++) {
		std::cerr << "~";
	}
    std::cerr << "^\n" << std::endl;
}

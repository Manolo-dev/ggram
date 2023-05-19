#include "error/ggram_error.hpp"

GgramError::GgramError() : std::runtime_error::runtime_error("GgramError") {}

GgramError::GgramError(const std::string &message)
    : std::runtime_error::runtime_error(message), _message(message) {}

const char *GgramError::what() const noexcept {
    return _message.c_str();
}

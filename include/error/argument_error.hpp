#pragma once

#include "error/ggram_error.hpp"

class ArgumentError : public GgramError {
  public:
    explicit ArgumentError(const std::string &error);
    ~ArgumentError() noexcept override = default;
};

#pragma once

#include <stdexcept>
#include <string>

class GgramError : public std::runtime_error {

  public:
    GgramError();
    GgramError(const std::string &message);
    ~GgramError() override = default;
    [[nodiscard]] const char *what() const noexcept override;

  private:
    const std::string _message;
};

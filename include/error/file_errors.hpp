#pragma once

#include "error/ggram_error.hpp"

class FileNotFound : public GgramError {
  public:
    explicit FileNotFound(const std::string &filename);
    ~FileNotFound() noexcept override = default;
};

class InvalidFileExtensionError : public GgramError {
  public:
    InvalidFileExtensionError(const std::string & /*unused*/, const std::string &extension);
    ~InvalidFileExtensionError() noexcept override = default;
};

class NoFilenameSpecified : public GgramError {
  public:
    explicit NoFilenameSpecified(const std::string &type_file);
    ~NoFilenameSpecified() noexcept override = default;
};

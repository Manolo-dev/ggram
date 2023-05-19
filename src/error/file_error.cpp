#include "error/file_errors.hpp"

// ---------------------- InvalidFileExtensionError ---------------------- //

InvalidFileExtensionError::InvalidFileExtensionError(const std::string & /*unused*/,
                                                     const std::string &extension)
    : GgramError("Invalid file extension. Only " + extension + " files are allowed.") {}

// ---------------------------- FileNotFound ----------------------------- //

FileNotFound::FileNotFound(const std::string &filename)
    : GgramError("File " + filename + " not found.") {}

// ------------------------- NoFilenameSpecified ------------------------- //

NoFilenameSpecified::NoFilenameSpecified(const std::string &type_file)
    : GgramError("No " + type_file + " filename specified.") {}

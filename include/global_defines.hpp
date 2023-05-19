#pragma once

#include <string>
#include <string_view>
#include <vector>

// #define DEBUG_RULES

using combinations = std::vector<std::vector<std::string>>;
using Rule = std::vector<std::string>;
static constexpr std::string_view POP_FUNCTION_PREFIX = "pop_";
static constexpr std::string_view FILE_SEPARATOR = "---";

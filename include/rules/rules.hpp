#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <stack>

#include "error/syntax_error.hpp"
#include "regex/regex.hpp"
#include "global_defines.hpp"
#include "utils.hpp"

std::vector<std::pair<std::string, Rule>> readRules(FileHandler &files);

void writeRulesPopFunctions(const std::vector<std::pair<std::string, Rule>> &rules,
                                FileHandler &files);
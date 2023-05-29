#pragma once

#include "regex/types.hpp"
#include <functional>
#include <string_view>
#include <unordered_set>
#include <vector>
#include <string>
#include <stack>

using Parser = std::function<bool(
    std::stack<std::string_view> &, Rule &, std::unordered_set<std::string> &, std::string &,
    bool &, const std::string &, std::string &, std::vector<std::pair<std::string, Rule>> &)>;
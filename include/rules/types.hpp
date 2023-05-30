#pragma once

#include "regex/types.hpp"
#include <functional>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

using Parser = std::function<bool(
    std::stack<std::string_view> &, Rule &, std::unordered_set<std::string> &, std::string &,
    bool &, const std::string &, std::string &, std::vector<std::pair<std::string, Rule>> &)>;
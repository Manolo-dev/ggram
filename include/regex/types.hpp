#pragma once

#include "global_defines.hpp"
#include <optional>
#include <functional>
#include <string>
#include <vector>
#include <stack>
#include <unordered_set>
#include <string_view>

using MatchResult = std::optional<std::pair<std::string_view, size_t>>;
using Matcher = std::function<MatchResult(std::string_view)>;

using Parser = std::function<bool(
    std::stack<std::string_view> &, Rule &, std::unordered_set<std::string> &, std::string &,
    bool &, const std::string &, std::string &, std::vector<std::pair<std::string, Rule>> &)>;
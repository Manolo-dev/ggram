#pragma once

#include "regex/types.hpp"
#include <functional>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

struct ParserContext {
    std::vector<std::pair<std::string, Rule>> rules;
    std::unordered_set<std::string> allRuleNames;
    Rule currentRule;
    std::string currentRuleName;
    std::stack<std::string_view> brackets;
    bool assigned = false;
};

using Parser = std::function<bool(ParserContext &, const std::string &, std::string &)>;

#include "ctre.hpp"
#include <optional>
#include <stack>
#include <string>
#include <vector>

using Rule = std::vector<std::string>;
using MatchResult = std::optional<std::pair<std::string_view, size_t>>;
using Matcher = MatchResult (*)(std::string_view);
using Parser = bool (*)(std::stack<std::string_view> &, Rule &, std::vector<std::string> &, bool &,
                        const std::string &, std::string &);
using LexerRule = std::tuple<std::string_view, Matcher, Parser>;

extern "C" {
constexpr auto funPattern = ctll::fixed_string{R"-(:([a-zA-Z][a-zA-Z0-9_]*))-"};
constexpr MatchResult funMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<funPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
[[maybe_unused]] static bool optionParser(std::stack<std::string_view> &brackets, Rule &currentRule,
                                          std::vector<std::string> &allRuleNames, bool &assigned,
                                          const std::string &value, std::string &error) {
    if (!assigned) {
        error = "Expected assignment";
        return true;
    }
    currentRule.push_back(value);
    return false;
}

std::vector<LexerRule> getLexerRules() {
    return {{"FUN", funMatcher, optionParser}};
}
}
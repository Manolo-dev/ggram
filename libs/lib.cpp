#include <string>
#include <vector>
#include <stack>
#include <optional>
#include "ctre.hpp"

using Rule = std::vector<std::string>;
using MatchResult = std::optional<std::pair<std::string_view, size_t>>;
using Matcher = MatchResult(*)(std::string_view);
using Parser = bool(*)(
    std::stack<std::string_view> &,
    Rule &,
    std::vector<std::string> &,
    std::string_view &,
    bool &,
    const std::string &,
    std::string &);

extern "C" {
    constexpr auto funPattern = ctll::fixed_string{R"-(:([a-zA-Z][a-zA-Z0-9_]*))-"};
    constexpr MatchResult funMatcher(std::string_view str) {
        if (auto match = ctre::starts_with<funPattern>(str); match) {
            return std::make_pair(match.get<0>().to_view(), match.size());
        }
        return std::nullopt;
    }
    [[maybe_unused]] static bool optionParser(
        std::stack<std::string_view> &brackets,
        Rule &currentRule,
        std::vector<std::string> &allRuleNames,
        std::string_view &currentRuleName,
        bool &assigned,
        const std::string &value,
        std::string &error) {
        if (!assigned) {
            error = "Expected assignment";
            return true;
        }
        currentRule.push_back(value);
        return false;
    }

    std::vector<std::pair<std::string_view, Matcher>> get_matchers() {
        return {
            {"FUN", funMatcher}
        };
    }

    std::vector<std::pair<std::string_view, Parser>> get_parsers() {
        return {
            {"FUN", optionParser}
        };
    }
}
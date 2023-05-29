#pragma once

#include "ctre/ctre.hpp"
#include "error/syntax_error.hpp"
#include "global_defines.hpp"
#include <array>
#include <functional>
#include <optional>
#include <string>
#include <tuple>
#include <stack>
#include <vector>
#include <algorithm>

using MatchResult = std::optional<std::pair<std::string_view, size_t>>;
using Matcher = MatchResult(*)(std::string_view);
using Parser = bool(*)(
    std::stack<std::string_view> &,
    Rule &,
    std::vector<std::string> &,
    std::string_view &,
    bool &,
    const std::string &,
    std::string &,
    std::vector<std::pair<std::string, Rule>> &);

static constexpr auto ignorePattern = ctll::fixed_string{R"-([ \n\r\s\t]+)-"};
constexpr MatchResult ignoreMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<ignorePattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
[[maybe_unused]] bool ignoreParser(
    std::stack<std::string_view> &/*unused*/,
    Rule &,
    std::vector<std::string> &/*unused*/,
    std::string_view &/*unused*/,
    bool &/*unused*/,
    const std::string &/*unused*/,
    std::string &/*unused*/,
    std::vector<std::pair<std::string, Rule>> &/*unused*/);

static constexpr auto commentPattern = ctll::fixed_string{R"-(#.+$)-"};
constexpr MatchResult commentMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<commentPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
[[maybe_unused]] bool commentParser(
    std::stack<std::string_view> &/*unused*/,
    Rule &,
    std::vector<std::string> &/*unused*/,
    std::string_view &/*unused*/,
    bool &/*unused*/,
    const std::string &/*unused*/,
    std::string &/*unused*/,
    std::vector<std::pair<std::string, Rule>> &/*unused*/);

static constexpr auto ruleNamePattern = ctll::fixed_string{R"-(<[a-zA-Z][a-zA-Z0-9_]*>)-"};
constexpr MatchResult ruleNameMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<ruleNamePattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
[[maybe_unused]] bool ruleNameParser(
    std::stack<std::string_view> &/*unused*/,
    Rule &currentRule,
    std::vector<std::string> &allRuleNames,
    std::string_view &currentRuleName,
    bool &assigned,
    const std::string &value,
    std::string &error,
    std::vector<std::pair<std::string, Rule>> &/*unused*/);

static constexpr auto assignPattern = ctll::fixed_string{R"-(::=)-"};
constexpr MatchResult assignMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<assignPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
[[maybe_unused]] bool assignParser(
    std::stack<std::string_view> &/*unused*/,
    Rule &currentRule,
    std::vector<std::string> &/*unused*/,
    std::string_view &currentRuleName,
    bool &assigned,
    const std::string &value,
    std::string &error,
    std::vector<std::pair<std::string, Rule>> &/*unused*/);

static constexpr auto orPattern = ctll::fixed_string{R"-(\|)-"};
constexpr MatchResult orMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<orPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
[[maybe_unused]] bool orParser(
    std::stack<std::string_view> &/*unused*/,
    Rule &currentRule,
    std::vector<std::string> &/*unused*/,
    std::string_view &/*unused*/,
    bool &assigned,
    const std::string &value,
    std::string &error,
    std::vector<std::pair<std::string, Rule>> &/*unused*/);

static constexpr auto parenthPattern = ctll::fixed_string{R"-(\()-"};
constexpr MatchResult parenthMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<parenthPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
[[maybe_unused]] bool parenthParser(
    std::stack<std::string_view> &brackets,
    Rule &currentRule,
    std::vector<std::string> &/*unused*/,
    std::string_view &/*unused*/,
    bool &assigned,
    const std::string &value,
    std::string &error,
    std::vector<std::pair<std::string, Rule>> &/*unused*/);

static constexpr auto endParenthPattern = ctll::fixed_string{R"-(\))-"};
constexpr MatchResult endParenthMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<endParenthPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
[[maybe_unused]] bool endParenthParser(
    std::stack<std::string_view> &brackets,
    Rule &currentRule,
    std::vector<std::string> &/*unused*/,
    std::string_view &/*unused*/,
    bool &assigned,
    const std::string &value,
    std::string &error,
    std::vector<std::pair<std::string, Rule>> &/*unused*/);

static constexpr auto loopPattern = ctll::fixed_string{R"-(\{)-"};
constexpr MatchResult loopMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<loopPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
[[maybe_unused]] bool loopParser(
    std::stack<std::string_view> &brackets,
    Rule &currentRule,
    std::vector<std::string> &/*unused*/,
    std::string_view &/*unused*/,
    bool &assigned,
    const std::string &value,
    std::string &error,
    std::vector<std::pair<std::string, Rule>> &/*unused*/);

static constexpr auto endLoopPattern = ctll::fixed_string{R"-(\})-"};
constexpr MatchResult endLoopMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<endLoopPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
[[maybe_unused]] bool endLoopParser(
    std::stack<std::string_view> &brackets,
    Rule &currentRule,
    std::vector<std::string> &/*unused*/,
    std::string_view &/*unused*/,
    bool &assigned,
    const std::string &value,
    std::string &error,
    std::vector<std::pair<std::string, Rule>> &/*unused*/);

static constexpr auto optionPattern = ctll::fixed_string{R"-(\[)-"};
constexpr MatchResult optionMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<optionPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
[[maybe_unused]] bool optionParser(
    std::stack<std::string_view> &brackets,
    Rule &currentRule,
    std::vector<std::string> &/*unused*/,
    std::string_view &/*unused*/,
    bool &assigned,
    const std::string &value,
    std::string &error,
    std::vector<std::pair<std::string, Rule>> &/*unused*/);

static constexpr auto endOptionPattern = ctll::fixed_string{R"-(\])-"};
constexpr MatchResult endOptionMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<endOptionPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
[[maybe_unused]] bool endOptionParser(
    std::stack<std::string_view> &brackets,
    Rule &currentRule,
    std::vector<std::string> &/*unused*/,
    std::string_view &/*unused*/,
    bool &assigned,
    const std::string &value,
    std::string &error,
    std::vector<std::pair<std::string, Rule>> &/*unused*/);

static constexpr auto stringPattern = ctll::fixed_string{R"-(\"([^"]|\\")*\")-"};
constexpr MatchResult stringMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<stringPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
[[maybe_unused]] bool stringParser(
    std::stack<std::string_view> &/*unused*/,
    Rule &currentRule,
    std::vector<std::string> &/*unused*/,
    std::string_view &/*unused*/,
    bool &assigned,
    const std::string &value,
    std::string &error,
    std::vector<std::pair<std::string, Rule>> &/*unused*/);

static constexpr auto endPattern = ctll::fixed_string{R"-(;)-"};
constexpr MatchResult endMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<endPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
[[maybe_unused]] bool endParser(
    std::stack<std::string_view> &brackets,
    Rule &currentRule,
    std::vector<std::string> &/*unused*/,
    std::string_view &currentRuleName,
    bool &assigned,
    const std::string &value,
    std::string &error,
    std::vector<std::pair<std::string, Rule>> &rules);

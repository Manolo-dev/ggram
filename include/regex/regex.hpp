#pragma once

#include "ctre/ctre.hpp"
#include "error/syntax_error.hpp"
#include "global_defines.hpp"
#include "regex/types.hpp"
#include <optional>
#include <string>
#include <tuple>
#include <sstream>

static constexpr auto ignorePattern = ctll::fixed_string{R"-([ \n\r\s\t]+)-"};
constexpr MatchResult ignoreMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<ignorePattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}

static constexpr auto commentPattern = ctll::fixed_string{R"-(#.+$)-"};
constexpr MatchResult commentMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<commentPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}

static constexpr auto ruleNamePattern = ctll::fixed_string{R"-(<[a-zA-Z][a-zA-Z0-9_]*>)-"};
constexpr MatchResult ruleNameMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<ruleNamePattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}

static constexpr auto assignPattern = ctll::fixed_string{R"-(::=)-"};
constexpr MatchResult assignMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<assignPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}

static constexpr auto orPattern = ctll::fixed_string{R"-(\|)-"};
constexpr MatchResult orMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<orPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}

static constexpr auto parenthPattern = ctll::fixed_string{R"-(\()-"};
constexpr MatchResult parenthMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<parenthPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}

static constexpr auto endParenthPattern = ctll::fixed_string{R"-(\))-"};
constexpr MatchResult endParenthMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<endParenthPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}

static constexpr auto loopPattern = ctll::fixed_string{R"-(\{)-"};
constexpr MatchResult loopMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<loopPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}

static constexpr auto endLoopPattern = ctll::fixed_string{R"-(\})-"};
constexpr MatchResult endLoopMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<endLoopPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}

static constexpr auto optionPattern = ctll::fixed_string{R"-(\[)-"};
constexpr MatchResult optionMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<optionPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}

static constexpr auto endOptionPattern = ctll::fixed_string{R"-(\])-"};
static constexpr auto endOptionMatch = ctre::starts_with<endOptionPattern>;
constexpr MatchResult endOptionMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<endOptionPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}

static constexpr auto stringPattern = ctll::fixed_string{R"-(\"([^"]|\\")*\")-"};
constexpr MatchResult stringMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<stringPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}

static constexpr auto endPattern = ctll::fixed_string{R"-(;)-"};
constexpr MatchResult endMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<endPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}

/**
 * @brief Replace the input string with the replacement string if the pattern match
 *
 * @param pattern the pattern to match
 * @param replacement the replacement string
 * @param str the input string
 * @return const std::optional<std::string> the replaced string or nullopt if the pattern doesn't
 * match
 */
template<CTRE_REGEX_INPUT_TYPE input, typename... Modifiers>
std::optional<std::string> replace(const std::string_view &str, const std::string_view &replacement);

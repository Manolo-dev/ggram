#pragma once

#include "ctre/ctre.hpp"
#include <array>
#include <functional>
#include <optional>
#include <string>
#include <tuple>

using MatchResult = std::optional<std::pair<std::string_view, size_t>>;
using Matcher = std::function<MatchResult(std::string_view)>;

enum class LexemeName {
    IGNORE,
    COMMENT,
    RULENAME,
    ASSIGN,
    OR,
    PARENTH,
    ENDPARENTH,
    LOOP,
    ENDLOOP,
    OPTION,
    ENDOPTION,
    STRING,
    END
};

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

// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
static const std::array<std::pair<LexemeName, Matcher>, 13> LEX_GGRAM_FILE{
    {// iteration on that, that why it isn't map
     {LexemeName::IGNORE, ignoreMatcher},
     {LexemeName::COMMENT, commentMatcher},
     {LexemeName::RULENAME, ruleNameMatcher},
     {LexemeName::ASSIGN, assignMatcher},
     {LexemeName::OR, orMatcher},
     {LexemeName::PARENTH, parenthMatcher},
     {LexemeName::ENDPARENTH, endParenthMatcher},
     {LexemeName::LOOP, loopMatcher},
     {LexemeName::ENDLOOP, endLoopMatcher},
     {LexemeName::OPTION, optionMatcher},
     {LexemeName::ENDOPTION, endOptionMatcher},
     {LexemeName::STRING, stringMatcher},
     {LexemeName::END, endMatcher}}};

static constexpr auto LEXME_INFOS_REGEX =
    ctll::fixed_string{R"-(^(\.?[a-zA-Z][a-zA-Z_0-9]*)\s*"(([^"]|\\")*)"$)-"};

#pragma once

#include "ctre/ctre.hpp"
#include "error/syntax_error.hpp"
#include <array>
#include <functional>
#include <optional>
#include <string>
#include <tuple>
#include <stack>

using MatchResult = std::optional<std::pair<std::string_view, size_t>>;
typedef MatchResult (*Matcher)(std::string_view);
typedef void (*Parser)(std::stack<std::string_view> &, bool &, bool &, FileHandler &);

static constexpr auto ignorePattern = ctll::fixed_string{R"-([ \n\r\s\t]+)-"};
constexpr MatchResult ignoreMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<ignorePattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
static void ignoreParser(std::stack<std::string_view> &/*unused*/, bool &/*unused*/, bool &/*unused*/, FileHandler &/*unused*/) {
    return;
}

static constexpr auto commentPattern = ctll::fixed_string{R"-(#.+$)-"};
constexpr MatchResult commentMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<commentPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
static void commentParser(std::stack<std::string_view> &/*unused*/, bool &/*unused*/, bool &/*unused*/, FileHandler &/*unused*/) {
    return;
}

static constexpr auto ruleNamePattern = ctll::fixed_string{R"-(<[a-zA-Z][a-zA-Z0-9_]*>)-"};
constexpr MatchResult ruleNameMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<ruleNamePattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
static void ruleNameParser(std::stack<std::string_view> &brackets, bool &named, bool &assigned, FileHandler &files) {
    if (!named && !assigned) {
        if (brackets.empty()) {
            named = true;
            return;
        } else {
            throw SyntaxError("Expected rule name", files.getCurrentLineNumber());
        }
    }
}

static constexpr auto assignPattern = ctll::fixed_string{R"-(::=)-"};
constexpr MatchResult assignMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<assignPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
static void assignParser(std::stack<std::string_view> &brackets, bool &named, bool &assigned, FileHandler &files) {
    if (named && !assigned) {
        if (brackets.empty()) {
            assigned = true;
            return;
        } else {
            throw SyntaxError("Expected assignment ('::=')", files.getCurrentLineNumber());
        }
    }
}

static constexpr auto orPattern = ctll::fixed_string{R"-(\|)-"};
constexpr MatchResult orMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<orPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
static void orParser(std::stack<std::string_view> &/*unused*/, bool &named, bool &assigned, FileHandler &files) {
    if (!named && !assigned) {
        throw SyntaxError("Expected rule name and assignment", files.getCurrentLineNumber());
    };
}

static constexpr auto parenthPattern = ctll::fixed_string{R"-(\()-"};
constexpr MatchResult parenthMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<parenthPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
static void parenthParser(std::stack<std::string_view> &brackets, bool &named, bool &assigned, FileHandler &files) {
    if (!named && !assigned) {
        throw SyntaxError("Expected rule name and assignment", files.getCurrentLineNumber());
    }
    brackets.push("(");
}

static constexpr auto endParenthPattern = ctll::fixed_string{R"-(\))-"};
constexpr MatchResult endParenthMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<endParenthPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
static void endParenthParser(std::stack<std::string_view> &brackets, bool &named, bool &assigned, FileHandler &files) {
    if (!named && !assigned) {
        throw SyntaxError("Expected rule name and assignment", files.getCurrentLineNumber());
    }
    if (brackets.empty()) {
        throw SyntaxError("Unexpected ')'", files.getCurrentLineNumber());
    }
    if (brackets.top() != "(") {
        throw SyntaxError("Expected ')'", files.getCurrentLineNumber());
    }
}

static constexpr auto loopPattern = ctll::fixed_string{R"-(\{)-"};
constexpr MatchResult loopMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<loopPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
static void loopParser(std::stack<std::string_view> &brackets, bool &named, bool &assigned, FileHandler &files) {
    if (!named && !assigned) {
        throw SyntaxError("Expected rule name and assignment", files.getCurrentLineNumber());
    }
    brackets.push("{");
}

static constexpr auto endLoopPattern = ctll::fixed_string{R"-(\})-"};
constexpr MatchResult endLoopMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<endLoopPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
static void endLoopParser(std::stack<std::string_view> &brackets, bool &named, bool &assigned, FileHandler &files) {
    if (!named && !assigned) {
        throw SyntaxError("Expected rule name and assignment", files.getCurrentLineNumber());
    }
    if (brackets.empty()) {
        throw SyntaxError("Unexpected '}'", files.getCurrentLineNumber());
    }
    if (brackets.top() != "{") {
        throw SyntaxError("Expected '}'", files.getCurrentLineNumber());
    }
}

static constexpr auto optionPattern = ctll::fixed_string{R"-(\[)-"};
constexpr MatchResult optionMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<optionPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
static void optionParser(std::stack<std::string_view> &brackets, bool &named, bool &assigned, FileHandler &files) {
    if (!named && !assigned) {
        throw SyntaxError("Expected rule name and assignment", files.getCurrentLineNumber());
    }
    brackets.push("[");
}

static constexpr auto endOptionPattern = ctll::fixed_string{R"-(\])-"};
constexpr MatchResult endOptionMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<endOptionPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
static void endOptionParser(std::stack<std::string_view> &brackets, bool &named, bool &assigned, FileHandler &files) {
    if (!named && !assigned) {
        throw SyntaxError("Expected rule name and assignment", files.getCurrentLineNumber());
    }
    if (brackets.empty()) {
        throw SyntaxError("Unexpected ']'", files.getCurrentLineNumber());
    }
    if (brackets.top() != "[") {
        throw SyntaxError("Expected ']'", files.getCurrentLineNumber());
    }
}

static constexpr auto stringPattern = ctll::fixed_string{R"-(\"([^"]|\\")*\")-"};
constexpr MatchResult stringMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<stringPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
static void stringParser(std::stack<std::string_view> &/*unused*/, bool &named, bool &assigned, FileHandler &files) {
    if (!named && !assigned) {
        throw SyntaxError("Expected rule name and assignment", files.getCurrentLineNumber());
    }
}

static constexpr auto endPattern = ctll::fixed_string{R"-(;)-"};
constexpr MatchResult endMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<endPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
static void endParser(std::stack<std::string_view> &brackets, bool &named, bool &assigned, FileHandler &files) {
    if (!named && !assigned) {
        throw SyntaxError("Expected rule name and assignment", files.getCurrentLineNumber());
    }
    if (!brackets.empty()) {
        throw SyntaxError("Expected " + std::string(brackets.top()) + " to be closed", files.getCurrentLineNumber());
    }
}

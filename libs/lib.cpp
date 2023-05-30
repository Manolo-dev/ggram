#include "ctre/ctre.hpp"
#include "global_defines.hpp"
#include "regex/types.hpp"
#include "rules/types.hpp"
#include "input_handler/types.hpp"

#include <optional>
#include <stack>
#include <string>
#include <vector>
#include <unordered_set>

extern "C" {
constexpr auto funPattern = ctll::fixed_string{R"-(:([a-zA-Z][a-zA-Z0-9_]*))-"};
constexpr MatchResult funMatcher(std::string_view str) {
    if (auto match = ctre::starts_with<funPattern>(str); match) {
        return std::make_pair(match.get<0>().to_view(), match.size());
    }
    return std::nullopt;
}
[[maybe_unused]] bool optionParser(std::stack<std::string_view> & /* unused */, Rule &currentRule,
                                   std::unordered_set<std::string> & /* unused */,
                                   std::string & /* unused */, bool &assigned,
                                   const std::string &value, std::string &error,
                                   std::vector<std::pair<std::string, Rule>> & /* unused */) {
    if (!assigned) {
        error = "Expected assignment";
        return true;
    }
    currentRule.push_back(value);
    return false;
}

std::vector<InputHandler::LexerRule> getLexerRules() {
    return {{"FUN", funMatcher, optionParser}};
}

constexpr auto funGenPattern = ctll::fixed_string{R"-(:([a-zA-Z0-9_]+))-"};
bool funGen(std::string_view str, std::string &rule_element) {
    if (auto match = ctre::starts_with<funGenPattern>(str); match) {
        rule_element = "cgg::" + match.get<1>().to_string() + "(it_cur, it_end)";
        return true;
    }
    return false;
}

std::vector<InputHandler::GenRule> getGenRules() {
    return {funGen};
}
} // extern "C"
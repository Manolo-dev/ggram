#include <string>
#include <vector>
#include <optional>
#include "ctre.hpp"

using MatchResult = std::optional<std::pair<std::string_view, size_t>>;
typedef MatchResult (*Matcher)(std::string_view);

extern "C" {
    constexpr auto funPattern = ctll::fixed_string{R"-(:([a-zA-Z][a-zA-Z0-9_]*))-"};
    constexpr MatchResult funMatcher(std::string_view str) {
        if (auto match = ctre::starts_with<funPattern>(str); match) {
            return std::make_pair(match.get<0>().to_view(), match.size());
        }
        return std::nullopt;
    }

    std::vector<std::pair<std::string_view, Matcher>> get_matchers() {
        return {
            {"FUN", funMatcher}
        };
    }
}
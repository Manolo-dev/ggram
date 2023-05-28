#include <string>
#include <vector>
#include <optional>
#include "ctre.hpp"

using MatchResult = std::optional<std::pair<std::string_view, size_t>>;

extern "C" {
    std::vector<std::string> get_matchers() {
        return {"funMatcher"};
    }

    static constexpr auto funPattern = ctll::fixed_string{R"-(:([a-zA-Z][a-zA-Z0-9_]*))-"};
    MatchResult funMatcher(std::string_view str) {
        if (auto match = ctre::starts_with<funPattern>(str); match) {
            return std::make_pair(match.get<0>().to_view(), match.size());
        }
        return std::nullopt;
    }
}
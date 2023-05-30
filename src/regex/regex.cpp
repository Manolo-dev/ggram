#include "regex/regex.hpp"

template<CTRE_REGEX_INPUT_TYPE input, typename... Modifiers>
std::optional<std::string> replace(const std::string_view &str, const std::string_view &replacement) {
    auto match = ctre::match<input, Modifiers...>(str);
    if (!match) {
		return std::nullopt;
	}
	std::stringstream result;
	const size_t size = match.size();
	for (int i = 0; i < str.size() - 1; i++) {
		if (str.at(i) == '$' && str.at(i + 1) >= '0' && str.at(i + 1) <= '9') {
			const int index = str.at(i + 1) - '0';
			if (index < size) {
                result << match.get<index>().to_view();
			}
			i++;
		} else {
			result << str.at(i);
		}
	}
	return result.str();
}

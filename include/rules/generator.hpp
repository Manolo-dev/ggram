#pragma once

#include "regex/regex.hpp"


static constexpr auto loopGenPattern = ctll::fixed_string{R"-(\{([a-zA-Z0-9_]+)\})-"};
bool loopGen(std::string_view str, std::string& rule_element);

static constexpr auto ruleGenPattern = ctll::fixed_string{R"-(<([a-zA-Z_][a-zA-Z0-9_]*)>)-"};
bool ruleGen(std::string_view str, std::string& rule_element);

static constexpr auto stringGenPattern = ctll::fixed_string{R"-(\"([^\"]*)\")-"};
bool stringGen(std::string_view str, std::string& rule_element);
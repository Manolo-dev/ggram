#include "rules/generator.hpp"

bool loopGen(std::string_view str, std::string& rule_element) {
    if (auto match = ctre::match<loopGenPattern>(str); match) {
        rule_element = "_pop_while(pop_" + match.get<1>().to_string() + ", current)";
        return true;
    }
    return false;
}

bool ruleGen(std::string_view str, std::string& rule_element) {
    if (auto match = ctre::match<ruleGenPattern>(str); match) {
        rule_element = "pop_" + match.get<1>().to_string() + "(createNext(current))";
        return true;
    }
    return false;
}

bool stringGen(std::string_view str, std::string& rule_element) {
    if (auto match = ctre::match<stringGenPattern>(str); match) {
        rule_element = "_pop_value(\"" + match.get<1>().to_string() + "\", createNext(current))";
        return true;
    }
    return false;
}
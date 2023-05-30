#include "rules/parser.hpp"

bool ignoreParser(ParserContext & /* unused */, const std::string & /* unused */,
                  std::string & /* unused */) {
    return false;
}

bool commentParser(ParserContext & /* unused */, const std::string & /* unused */,
                   std::string & /* unused */) {
    return false;
}

bool ruleNameParser(ParserContext &context, const std::string &value, std::string &error) {
    if (!context.assigned) {
        if (context.allRuleNames.find(value) != context.allRuleNames.end()) {
            error = "Rule name already exists";
            return true;
        }
        context.currentRuleName = std::string_view(value).substr(1, value.size() - 2);
        context.allRuleNames.emplace(context.currentRuleName);
    } else {
        context.currentRule.push_back(value);
    }
    return false;
}

bool assignParser(ParserContext &context, const std::string & /* unused */, std::string &error) {
    if (context.assigned) {
        error = "Unexpected assignment";
        return true;
    }
    if (context.currentRuleName.empty()) {
        error = "Expected rule name";
        return true;
    }
    context.assigned = true;
    return false;
}

bool orParser(ParserContext &context, const std::string &value, std::string &error) {
    if (!context.assigned) {
        error = "Expected assignment";
        return true;
    }
    context.currentRule.push_back(value);
    return false;
}

bool parenthParser(ParserContext &context, const std::string &value, std::string &error) {
    if (!context.assigned) {
        error = "Expected assignment";
        return true;
    }
    context.brackets.push("(");
    context.currentRule.push_back(value);
    return false;
}

bool endParenthParser(ParserContext &context, const std::string &value, std::string &error) {
    if (!context.assigned) {
        error = "Expected assignment";
        return true;
    }
    if (context.brackets.empty()) {
        error = "Unexpected end of parentheses";
        return true;
    }
    if (context.brackets.top() != "(") {
        error = "Unexpected end of parentheses";
        return true;
    }
    context.brackets.pop();
    context.currentRule.push_back(value);
    return false;
}

bool loopParser(ParserContext &context, const std::string &value, std::string &error) {
    if (!context.assigned) {
        error = "Expected assignment";
        return true;
    }
    context.brackets.emplace("{");
    context.currentRule.push_back(value);
    return false;
}

bool endLoopParser(ParserContext &context, const std::string &value, std::string &error) {
    if (!context.assigned) {
        error = "Expected assignment";
        return true;
    }
    if (context.brackets.empty()) {
        error = "Unexpected end of loop";
        return true;
    }
    if (context.brackets.top() != "{") {
        error = "Unexpected end of loop";
        return true;
    }
    context.brackets.pop();
    context.currentRule.push_back(value);
    return false;
}

bool optionParser(ParserContext &context, const std::string &value, std::string &error) {
    if (!context.assigned) {
        error = "Expected assignment";
        return true;
    }
    context.brackets.emplace("[");
    context.currentRule.push_back(value);
    return false;
}

bool endOptionParser(ParserContext &context, const std::string &value, std::string &error) {
    if (!context.assigned) {
        error = "Expected assignment";
        return true;
    }
    if (context.brackets.empty()) {
        error = "Unexpected end of option";
        return true;
    }
    if (context.brackets.top() != "[") {
        error = "Unexpected end of option";
        return true;
    }
    context.brackets.pop();
    context.currentRule.push_back(value);
    return false;
}

bool stringParser(ParserContext &context, const std::string &value, std::string &error) {
    if (!context.assigned) {
        error = "Expected assignment";
        return true;
    }
    if (value.size() < 2) {
        error = "Unexpected content of string";
        return true;
    }
    context.currentRule.push_back(value);
    return false;
}

bool endParser(ParserContext &context, const std::string & /* unused */, std::string &error) {
    if (!context.assigned) {
        error = "Expected assignment";
        return true;
    }
    if (!context.brackets.empty()) {
        error = "'" + std::string(context.brackets.top()) + "' is never closed";
        return true;
    }
    context.rules.emplace_back(context.currentRuleName, context.currentRule);
    context.currentRule.clear();
    context.assigned = false;
    context.currentRuleName = "";
    return false;
}

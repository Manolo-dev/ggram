#include "rules/parser.hpp"

bool ignoreParser(std::stack<std::string_view> & /*unused*/, Rule & /*unused*/,
                  std::unordered_set<std::string> /*unused*/, std::string & /*unused*/,
                  bool & /*unused*/, const std::string & /*unused*/, std::string & /*unused*/,
                  std::vector<std::pair<std::string, Rule>> & /*unused*/) {
    return false;
}

bool commentParser(std::stack<std::string_view> & /*unused*/, Rule & /*unused*/,
                   std::unordered_set<std::string> /*unused*/, std::string & /*unused*/,
                   bool & /*unused*/, const std::string & /*unused*/, std::string & /*unused*/,
                   std::vector<std::pair<std::string, Rule>> & /*unused*/) {
    return false;
}

bool ruleNameParser(std::stack<std::string_view> & /*unused*/, Rule &currentRule,
                    std::unordered_set<std::string> allRuleNames, std::string &currentRuleName,
                    bool &assigned, const std::string &value, std::string &error,
                    std::vector<std::pair<std::string, Rule>> & /*unused*/) {
    if (!assigned) {
        if (allRuleNames.find(value) != allRuleNames.end()) {
            error = "Rule name already exists";
            return true;
        }
        currentRuleName = std::string_view(value).substr(1, value.size() - 2);
        allRuleNames.emplace(std::string(currentRuleName));
    } else {
        currentRule.push_back(value);
    }
    return false;
}

bool assignParser(std::stack<std::string_view> & /*unused*/, Rule & /*unused*/,
                  std::unordered_set<std::string> /*unused*/, std::string &currentRuleName,
                  bool &assigned, const std::string & /*unused*/, std::string &error,
                  std::vector<std::pair<std::string, Rule>> & /*unused*/) {
    if (assigned) {
        error = "Unexpected assignment";
        return true;
    }
    if (currentRuleName.empty()) {
        error = "Expected rule name";
        return true;
    }
    assigned = true;
    return false;
}

bool orParser(std::stack<std::string_view> & /*unused*/, Rule &currentRule,
              std::unordered_set<std::string> /*unused*/, std::string & /*unused*/, bool &assigned,
              const std::string &value, std::string &error,
              std::vector<std::pair<std::string, Rule>> & /*unused*/) {
    if (!assigned) {
        error = "Expected assignment";
        return true;
    }
    currentRule.push_back(value);
    return false;
}

bool parenthParser(std::stack<std::string_view> &brackets, Rule &currentRule,
                   std::unordered_set<std::string> /*unused*/, std::string & /*unused*/,
                   bool &assigned, const std::string &value, std::string &error,
                   std::vector<std::pair<std::string, Rule>> & /*unused*/) {
    if (!assigned) {
        error = "Expected assignment";
        return true;
    }
    brackets.push("(");
    currentRule.push_back(value);
    return false;
}

bool endParenthParser(std::stack<std::string_view> &brackets, Rule &currentRule,
                      std::unordered_set<std::string> /*unused*/, std::string & /*unused*/,
                      bool &assigned, const std::string &value, std::string &error,
                      std::vector<std::pair<std::string, Rule>> & /*unused*/) {
    if (!assigned) {
        error = "Expected assignment";
        return true;
    }
    if (brackets.empty()) {
        error = "Unexpected end of parentheses";
        return true;
    }
    if (brackets.top() != "(") {
        error = "Unexpected end of parentheses";
        return true;
    }
    brackets.pop();
    currentRule.push_back(value);
    return false;
}

bool loopParser(std::stack<std::string_view> &brackets, Rule &currentRule,
                std::unordered_set<std::string> /*unused*/, std::string & /*unused*/,
                bool &assigned, const std::string &value, std::string &error,
                std::vector<std::pair<std::string, Rule>> & /*unused*/) {
    if (!assigned) {
        error = "Expected assignment";
        return true;
    }
    brackets.emplace("{");
    currentRule.push_back(value);
    return false;
}

bool endLoopParser(std::stack<std::string_view> &brackets, Rule &currentRule,
                   std::unordered_set<std::string> /*unused*/, std::string & /*unused*/,
                   bool &assigned, const std::string &value, std::string &error,
                   std::vector<std::pair<std::string, Rule>> & /*unused*/) {
    if (!assigned) {
        error = "Expected assignment";
        return true;
    }
    if (brackets.empty()) {
        error = "Unexpected end of loop";
        return true;
    }
    if (brackets.top() != "{") {
        error = "Unexpected end of loop";
        return true;
    }
    brackets.pop();
    currentRule.push_back(value);
    return false;
}

bool optionParser(std::stack<std::string_view> &brackets, Rule &currentRule,
                  std::unordered_set<std::string> /*unused*/, std::string & /*unused*/,
                  bool &assigned, const std::string &value, std::string &error,
                  std::vector<std::pair<std::string, Rule>> & /*unused*/) {
    if (!assigned) {
        error = "Expected assignment";
        return true;
    }
    brackets.push("[");
    currentRule.push_back(value);
    return false;
}

bool endOptionParser(std::stack<std::string_view> &brackets, Rule &currentRule,
                     std::unordered_set<std::string> /*unused*/, std::string & /*unused*/,
                     bool &assigned, const std::string &value, std::string &error,
                     std::vector<std::pair<std::string, Rule>> & /*unused*/) {
    if (!assigned) {
        error = "Expected assignment";
        return true;
    }
    if (brackets.empty()) {
        error = "Unexpected end of option";
        return true;
    }
    if (brackets.top() != "[") {
        error = "Unexpected end of option";
        return true;
    }
    brackets.pop();
    currentRule.push_back(value);
    return false;
}

bool stringParser(std::stack<std::string_view> & /*unused*/, Rule &currentRule,
                  std::unordered_set<std::string> /*unused*/, std::string & /*unused*/,
                  bool &assigned, const std::string &value, std::string &error,
                  std::vector<std::pair<std::string, Rule>> & /*unused*/) {
    if (!assigned) {
        error = "Expected assignment";
        return true;
    }
    if (value.size() < 2) {
        error = "Unexpected content of string";
        return true;
    }
    currentRule.push_back(value);
    return false;
}

bool endParser(std::stack<std::string_view> &brackets, Rule &currentRule,
               std::unordered_set<std::string> /*unused*/, std::string &currentRuleName,
               bool &assigned, const std::string & /*unused*/, std::string &error,
               std::vector<std::pair<std::string, Rule>> &rules) {
    if (!assigned) {
        error = "Expected assignment";
        return true;
    }
    if (!brackets.empty()) {
        error = "Unexpected " + std::string(brackets.top());
        return true;
    }
    rules.emplace_back(currentRuleName, currentRule);
    currentRule.clear();
    assigned = false;
    currentRuleName = "";
    return false;
}

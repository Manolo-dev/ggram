#pragma once
#include "global_defines.hpp"
#include <stack>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

[[maybe_unused]] bool ignoreParser(std::stack<std::string_view> & /*unused*/, Rule &,
                                   std::unordered_set<std::string> /*unused*/,
                                   std::string & /*unused*/, bool & /*unused*/,
                                   const std::string & /*unused*/, std::string & /*unused*/,
                                   std::vector<std::pair<std::string, Rule>> & /*unused*/);

[[maybe_unused]] bool commentParser(std::stack<std::string_view> & /*unused*/, Rule &,
                                    std::unordered_set<std::string> /*unused*/,
                                    std::string & /*unused*/, bool & /*unused*/,
                                    const std::string & /*unused*/, std::string & /*unused*/,
                                    std::vector<std::pair<std::string, Rule>> & /*unused*/);

[[maybe_unused]] bool ruleNameParser(std::stack<std::string_view> & /*unused*/, Rule &currentRule,
                                     std::unordered_set<std::string> allRuleNames,
                                     std::string &currentRuleName, bool &assigned,
                                     const std::string &value, std::string &error,
                                     std::vector<std::pair<std::string, Rule>> & /*unused*/);

[[maybe_unused]] bool assignParser(std::stack<std::string_view> & /*unused*/, Rule &currentRule,
                                   std::unordered_set<std::string> /*unused*/,
                                   std::string &currentRuleName, bool &assigned,
                                   const std::string &value, std::string &error,
                                   std::vector<std::pair<std::string, Rule>> & /*unused*/);

[[maybe_unused]] bool orParser(std::stack<std::string_view> & /*unused*/, Rule &currentRule,
                               std::unordered_set<std::string> /*unused*/, std::string & /*unused*/,
                               bool &assigned, const std::string &value, std::string &error,
                               std::vector<std::pair<std::string, Rule>> & /*unused*/);

[[maybe_unused]] bool parenthParser(std::stack<std::string_view> &brackets, Rule &currentRule,
                                    std::unordered_set<std::string> /*unused*/,
                                    std::string & /*unused*/, bool &assigned,
                                    const std::string &value, std::string &error,
                                    std::vector<std::pair<std::string, Rule>> & /*unused*/);

[[maybe_unused]] bool endParenthParser(std::stack<std::string_view> &brackets, Rule &currentRule,
                                       std::unordered_set<std::string> /*unused*/,
                                       std::string & /*unused*/, bool &assigned,
                                       const std::string &value, std::string &error,
                                       std::vector<std::pair<std::string, Rule>> & /*unused*/);

[[maybe_unused]] bool loopParser(std::stack<std::string_view> &brackets, Rule &currentRule,
                                 std::unordered_set<std::string> /*unused*/,
                                 std::string & /*unused*/, bool &assigned, const std::string &value,
                                 std::string &error,
                                 std::vector<std::pair<std::string, Rule>> & /*unused*/);

[[maybe_unused]] bool endLoopParser(std::stack<std::string_view> &brackets, Rule &currentRule,
                                    std::unordered_set<std::string> /*unused*/,
                                    std::string & /*unused*/, bool &assigned,
                                    const std::string &value, std::string &error,
                                    std::vector<std::pair<std::string, Rule>> & /*unused*/);

[[maybe_unused]] bool optionParser(std::stack<std::string_view> &brackets, Rule &currentRule,
                                   std::unordered_set<std::string> /*unused*/,
                                   std::string & /*unused*/, bool &assigned,
                                   const std::string &value, std::string &error,
                                   std::vector<std::pair<std::string, Rule>> & /*unused*/);

[[maybe_unused]] bool endOptionParser(std::stack<std::string_view> &brackets, Rule &currentRule,
                                      std::unordered_set<std::string> /*unused*/,
                                      std::string & /*unused*/, bool &assigned,
                                      const std::string &value, std::string &error,
                                      std::vector<std::pair<std::string, Rule>> & /*unused*/);

[[maybe_unused]] bool stringParser(std::stack<std::string_view> & /*unused*/, Rule &currentRule,
                                   std::unordered_set<std::string> /*unused*/,
                                   std::string & /*unused*/, bool &assigned,
                                   const std::string &value, std::string &error,
                                   std::vector<std::pair<std::string, Rule>> & /*unused*/);

[[maybe_unused]] bool endParser(std::stack<std::string_view> &brackets, Rule &currentRule,
                                std::unordered_set<std::string> /*unused*/,
                                std::string &currentRuleName, bool &assigned,
                                const std::string &value, std::string &error,
                                std::vector<std::pair<std::string, Rule>> &rules);
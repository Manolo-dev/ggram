#pragma once
#include "global_defines.hpp"
#include "rules/types.hpp"
#include <stack>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

[[maybe_unused]] bool ignoreParser(ParserContext & /* unused */, const std::string & /* unused */,
                                   std::string & /* unused */);

[[maybe_unused]] bool commentParser(ParserContext & /* unused */, const std::string & /* unused */,
                                    std::string & /* unused */);

[[maybe_unused]] bool ruleNameParser(ParserContext &context, const std::string &value, std::string &error);

[[maybe_unused]] bool assignParser(ParserContext &context, const std::string &value, std::string &error);

[[maybe_unused]] bool orParser(ParserContext &context, const std::string &value, std::string &error);

[[maybe_unused]] bool parenthParser(ParserContext &context, const std::string &value,
                                    std::string &error);

[[maybe_unused]] bool endParenthParser(ParserContext &context, const std::string &value,
                                       std::string &error);

[[maybe_unused]] bool loopParser(ParserContext &context, const std::string &value,
                                 std::string &error);

[[maybe_unused]] bool endLoopParser(ParserContext &context, const std::string &value,
                                    std::string &error);

[[maybe_unused]] bool optionParser(ParserContext &context, const std::string &value,
                                   std::string &error);

[[maybe_unused]] bool endOptionParser(ParserContext &context, const std::string &value,
                                      std::string &error);

[[maybe_unused]] bool stringParser(ParserContext &context, const std::string &value,
                                   std::string &error);

[[maybe_unused]] bool endParser(ParserContext &context, const std::string & /* unused */,
                                std::string &error);
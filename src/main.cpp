#include <array>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

#include "error/syntax_error.hpp"
#include "file_handler.hpp"
#include "global_defines.hpp"
#include "input_handler.hpp"
#include "lexme/lexme.hpp"
#include "utils.hpp"

struct PairRuleFunction {
    PairRuleFunction(const std::string &name, const std::string &functionName)
        : name(name), function(functionName) {}
    std::string name;
    std::string function;
};

inline std::vector<std::string> getInsideBrackets(const std::vector<std::string> &tree, size_t &i,
                                                  const std::string_view &open_bracket,
                                                  const std::string_view &closed_bracket) {

    unsigned int level = 1;
    std::vector<std::string> inside_brackets;

    while (level > 0) {
        i++;
        if (i >= tree.size()) {
            throw SyntaxError("Missing closing bracket", i);
        }
        if (tree[i] == open_bracket) {
            level++;
        }
        if (tree[i] == closed_bracket) {
            level--;
        }
        if (level != 0) {
            inside_brackets.push_back(tree[i]);
        }
    }
    return inside_brackets;
}

combinations generateCombinations(const std::vector<std::string> &tree) {
    combinations result;
    combinations branch;
    branch.emplace_back();

    for (size_t i = 0; i < tree.size(); i++) {
        if (tree[i] == "(" || tree[i] == "[") {
            // Keep what's inside the brackets
            const std::string open_bracket = tree[i];
            const std::string closed_bracket = tree[i] == "(" ? ")" : "]";
            std::vector<std::string> const inside_brackets =
                getInsideBrackets(tree, i, open_bracket, closed_bracket);

            // Possible combinations inside the brackets
            combinations const sub_combinations = generateCombinations(inside_brackets);

            // Add these combinations to the previous results
            combinations combined_result;
            for (std::vector<std::string> const &previous_result : branch) {
                for (std::vector<std::string> const &sub_combination : sub_combinations) {
                    // prefix each inside brackets
                    // combination with what was before
                    combined_result.push_back(previous_result);
                    extend(combined_result.back(), sub_combination);
                    // ifinside square brackets,
                    // subcombinations are optional so we
                    // have to add the pure previous result
                    if (open_bracket == "[") {
                        combined_result.push_back(previous_result);
                    }
                }
            }
            branch = combined_result;
        } else if (tree[i] == "|") {
            extend(result, branch);
            branch.clear();
            branch.emplace_back();
        } else {
            pushToEachElement(branch, tree[i]);
        }
    }
    extend(result, branch);
    return result;
}

std::string generateSimpleRulePopFunction(const std::vector<std::string> &rule,
                                          const std::string &name) {
    std::string result;

    result += "    const IT it_start = it_cur;\n";
    result +=
        "    master = Token(\"" + name + "\", \"\", it_start->line(), it_start->column()); \n";
    result += "    std::vector<Token> current;\n";
    result += "    SEARCH(\"" + name + "\")\n";

    std::string prefix;
    std::string suffix;
    for (std::vector<std::string> &rule_combination : generateCombinations(rule)) {
        result += "    current.clear();\n";
        result += "    if(";

        for (size_t i = 0; i < rule_combination.size(); i++) {
            /*
            {truc}->pop_while(truc, current) OU pop_while(truc,
            it_cur, it_end) <truc>->pop_truc(createNext(current))
            OU pop_truc(it_cur, it_end) "truc"->pop_value("truc",
            createNext(current)) OU pop_value("truc", it_cur,
            it_end) otherwise : INVALID_SYNTAX
            */

            switch (rule_combination[i][0]) {
                case '{':
                    if (rule_combination[i].back() != '}') {
                        throw SyntaxError("Missing '}'");
                    }
                    prefix = "_pop_while(";
                    prefix += POP_FUNCTION_PREFIX;
                    suffix = ", current)";
                    break;

                case '<':
                    if (rule_combination[i].back() != '>') {
                        throw SyntaxError("Missing '>'");
                    }
                    prefix = POP_FUNCTION_PREFIX;
                    suffix = "(createNext(current))";
                    break;

                case '"':
                    if (rule_combination[i].back() != '"' || rule_combination[i].size() < 2) {
                        throw SyntaxError("Missing '\"'");
                    }
                    prefix = "_pop_value(\"";
                    suffix = "\", createNext(current))";
                    break;

                default:
                    throw SyntaxError("Invalid syntax");
            }
            result +=
                prefix + rule_combination[i].substr(1, rule_combination[i].size() - 2) + suffix;

            if (i >= rule_combination.size() - 1) {
                continue;
            }
            result += " || ";
        }
        result += ") it_cur = it_start;\n";
        result += "    else {master.children() += current; FOUND(\"" + name + "\") return 0;};\n";
    }
    result += "    FAILED(\"" + name + "\")\n";
    result += "    return 1;\n";

    return result;
}

void addRulePopFunctions(const Rule &rule, const std::string &name,
                         std::vector<PairRuleFunction> &result) {
    size_t rule_id = 0;
    std::vector<std::string> newRule;
    for (size_t i = 0; i < rule.size(); i++) {
        if (rule[i] == "{") {
            std::vector<std::string> const loopRule = getInsideBrackets(rule, i, "{", "}");
            const std::string aux_name = std::to_string(rule_id) + "_" + name;
            rule_id++;
            addRulePopFunctions(loopRule, aux_name, result);
            newRule.push_back("{" + aux_name + "}");
        } else {
            newRule.push_back(rule[i]);
        }
    }
    result.emplace_back(name, generateSimpleRulePopFunction(newRule, name));
}

std::vector<std::pair<std::string, Rule>> readRules(FileHandler &files) {
    std::vector<std::pair<std::string, Rule>> rules;
    std::vector<std::string> allRuleNames;
    std::string line;
    Rule currentRule;
    std::stack<LexemeName> brackets;

    while (files.getline(line)) {
        if (line[0] == '#' || line.empty()) {
            continue;
        }
        if (line == FILE_SEPARATOR) {
            throw SyntaxError("Unexpected '" + std::string(FILE_SEPARATOR) + "'",
                              files.getCurrentLineNumber());
        }
        std::string_view line_view = line;
        bool match_found = false;
        while (!line_view.empty()) {
            match_found = false;
            for (size_t i = 0; i < LEX_GGRAM_FILE.size() && !match_found; i++) {
                const auto &[name, matcher] = LEX_GGRAM_FILE.at(i);
                const MatchResult result = matcher(line_view);
                if (result == std::nullopt) {
                    continue;
                }
                match_found = true;
                const auto &match = result.value();
                if (name == LexemeName::IGNORE || name == LexemeName::COMMENT) {
                    line_view = line_view.substr(match.second);
                } else if (name == LexemeName::END) {
                    if (currentRule.size() < 2) {
                        throw SyntaxError("Expected rule name", files.getCurrentLineNumber());
                    }
                    if (!brackets.empty()) {
                        throw SyntaxError("Expected ')', '}' or ']'", files.getCurrentLineNumber());
                    }
                    rules.emplace_back(currentRule[0].substr(1, currentRule[0].size() - 2),
                                       Rule(currentRule.begin() + 2, currentRule.end()));
                    allRuleNames.push_back(currentRule[0]);
                    line_view = line_view.substr(match.second);
                    currentRule.clear();
                } else {
                    const std::string &match_str = std::string(match.first);
                    if (currentRule.empty() && name != LexemeName::RULENAME) {
                        throw SyntaxError("Expected rule name", files.getCurrentLineNumber());
                    }
                    if (currentRule.size() == 1 && name != LexemeName::ASSIGN) {
                        throw SyntaxError("Expected '::='", files.getCurrentLineNumber());
                    }
                    if (currentRule.empty() && find(allRuleNames.begin(), allRuleNames.end(),
                                                    match_str) != allRuleNames.end()) {
                        throw SyntaxError("Rule '" + match_str + "' already defined",
                                          files.getCurrentLineNumber());
                    }
                    if (name == LexemeName::PARENTH || name == LexemeName::LOOP ||
                        name == LexemeName::OPTION) {
                        brackets.push(name);
                    } else if (name == LexemeName::ENDPARENTH || name == LexemeName::ENDLOOP ||
                               name == LexemeName::ENDOPTION) {
                        if (name == LexemeName::ENDPARENTH &&
                            brackets.top() != LexemeName::PARENTH) {
                            throw SyntaxError("Unexpected '" + match_str + "'",
                                              files.getCurrentLineNumber());
                        }
                        if (brackets.empty()) {
                            throw SyntaxError("Unexpected '" + match_str + "'",
                                              files.getCurrentLineNumber());
                        }
                        if (name == LexemeName::ENDLOOP && brackets.top() != LexemeName::LOOP) {
                            throw SyntaxError("Unexpected '" + match_str + "'",
                                              files.getCurrentLineNumber());
                        }
                        if (name == LexemeName::ENDOPTION && brackets.top() != LexemeName::OPTION) {
                            throw SyntaxError("Unexpected '" + match_str + "'",
                                              files.getCurrentLineNumber());
                        }
                        brackets.pop();
                    }

                    currentRule.push_back(match_str);
                    line_view = line_view.substr(match.second);
                }
            }
            if (!match_found) {
                throw SyntaxError("Unexpected token", files.getCurrentLineNumber());
            }
        }
    }
    if (!currentRule.empty()) {
        throw SyntaxError("Expected ';'", files.getCurrentLineNumber());
    }
    return rules;
}
void writeRulesPopFunctions(const std::vector<std::pair<std::string, Rule>> &rules,
                            FileHandler &files) {
    // Declare rules' pop functions
    for (const auto &[rule_name, _] : rules) {
        files << FileHandler::WriteMode::HPP << "bool " << POP_FUNCTION_PREFIX << rule_name
              << "(Token &master);" << std::endl;
    }
    // Define rules' pop functions
    for (const auto &[rule_name, rule_expr] : rules) {
        files << FileHandler::WriteMode::CPP << "\n"
              << "/********************************************************************************"
                 "*******************/\n"
              << "/*                       Functions to pop tokens of type : " << rule_name
              << (40UL - rule_name.size()) * std::string(" ") << "*/\n"
              << "/********************************************************************************"
                 "*******************/\n";
        std::vector<PairRuleFunction> result;
        addRulePopFunctions(rule_expr, rule_name, result);
        for (const auto &[aux_rule_name, aux_rule_func] : result) {
            files << FileHandler::WriteMode::CPP << std::endl
                  << "bool " << POP_FUNCTION_PREFIX << aux_rule_name << "(Token &master) {"
                  << std::endl
                  << aux_rule_func << "}" << std::endl;
        }
    }
}

void initOutputFiles(const InputHandler::Configuration &cfg, FileHandler &files) {
    files.open(cfg.input_filename, cfg.output_filepath_cpp, cfg.output_filepath_hpp);

    files.copy("template/head.hpp", FileHandler::WriteMode::HPP);

    files << FileHandler::WriteMode::CPP << "#include " << cfg.output_filepath_hpp.filename()
          << std::endl;

    files.copy("template/token.cpp", FileHandler::WriteMode::CPP);
    files.copy("template/lex.cpp", FileHandler::WriteMode::CPP);
    files.copy("template/value_expression.cpp", FileHandler::WriteMode::CPP);
}

int main(int argc, char const *argv[]) {
    InputHandler::Configuration cfg;
    InputHandler::handleParameters(std::vector<std::string>{argv, argv + argc}, cfg);
    FileHandler files;
    initOutputFiles(cfg, files);
    const auto lexeme_names = createLexemes(files);
    writeLexemesPopFunctions(lexeme_names, files);
    const std::vector<std::pair<std::string, Rule>> rules = readRules(files);

#ifdef DEBUG_RULES
    for (auto [rule_name, _] : rules) {
        cout << rule_name << std::endl;
        cout << _ << std::endl;
    }
#endif
    writeRulesPopFunctions(rules, files);

    files << FileHandler::WriteMode::CPP << std::endl;

    files.copy("template/main.cpp", FileHandler::WriteMode::CPP);

    return 0;
}

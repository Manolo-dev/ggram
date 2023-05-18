#include <array>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

#include "error.hpp"
#include "file_handler.hpp"
#include "input_handler.hpp"
#include "regex/regex.hpp"

// #define DEBUG_RULES

using combinations = std::vector<std::vector<std::string>>;
using Rule = std::vector<std::string>;
static constexpr std::string_view POP_FUNCTION_PREFIX = "pop_";

struct PairRuleFunction {
    PairRuleFunction(const std::string &name, const std::string &functionName)
        : name(name), function(functionName) {}
    std::string name;
    std::string function;
};

std::ostream &operator<<(std::ostream &os, std::vector<std::string> const &vector) {
    os << "[";

    for (size_t i = 0; i < vector.size(); i++) {
        os << vector[i];
        if (i != vector.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

std::string operator*(const std::string &base_string, size_t n) {
    std::stringstream out;

    while ((n--) != 0U) {
        out << base_string;
    }
    return out.str();
}

std::string operator*(size_t n, const std::string &base_string) {
    return base_string * n;
}

template<typename T> void extend(std::vector<T> &dest, const std::vector<T> &src) {
    for (const T &elem : src) {
        dest.push_back(elem);
    }
}

template<typename T>
void add_to_each_element(std::vector<std::vector<T>> &liste, const T &new_elem) {
    for (std::vector<T> &comb : liste) {
        comb.push_back(new_elem);
    }
}

inline std::vector<std::string> get_inside_brackets(const std::vector<std::string> &tree, size_t &i,
                                                    const std::string_view &open_bracket,
                                                    const std::string_view &closed_bracket) {

    unsigned int level = 1;
    std::vector<std::string> inside_brackets;

    while (level > 0) {
        i++;
        if (i >= tree.size()) {
            throw InvalidSyntax(i, "Missing closing bracket");
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
                get_inside_brackets(tree, i, open_bracket, closed_bracket);

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
                    // if inside square brackets,
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
            add_to_each_element(branch, tree[i]);
        }
    }
    extend(result, branch);
    return result;
}

std::string generateSimpleRulePopFunction(const std::vector<std::string> &rule,
                                          const std::string &name,
                                          InputHandler::ResultType res_type) {
    std::string result;
    switch (res_type) {
        case InputHandler::ResultType::ORS:
            result += "    const IT it_start = it_cur;\n";
            result += "    master = Token(\"" + name +
                      "\", \"\", it_start -> line(), it_start -> "
                      "column()); \n";
            result += "    std::vector<Token> current;\n";
            result += "    SEARCH(\"" + name + "\")\n";
            break;
        case InputHandler::ResultType::ERROR_TOKEN:
        case InputHandler::ResultType::TRY_CATCHS:
            result += "    SEARCH(\"" + name + "\")\n";
            result += "    const IT it_start = it_cur;\n";
            result += "    Token master(\"" + name +
                      "\", \"\", it_start -> line(), it_start -> "
                      "column()); \n\n";
            break;
        default:
            throw std::invalid_argument("Not yet implemented here !");
    }

    for (std::vector<std::string> &rule_combination : generateCombinations(rule)) {
        switch (res_type) {
            case InputHandler::ResultType::ORS:
                result += "    current.clear();\n";
                result += "    if(";
                break;
            case InputHandler::ResultType::TRY_CATCHS:
                result += "    try{\n";
                result += "        return master << ";
                break;
            case InputHandler::ResultType::ERROR_TOKEN:
                result += "    master << ";
                break;
            default:
                throw std::invalid_argument("Not yet implemented here !");
        }

        for (size_t i = 0; i < rule_combination.size(); i++) {
            std::string prefix;
            std::string suffix;
            /*
            {truc} -> pop_while(truc, current) OU pop_while(truc,
            it_cur, it_end) <truc> -> pop_truc(createNext(current))
            OU pop_truc(it_cur, it_end) "truc" -> pop_value("truc",
            createNext(current)) OU pop_value("truc", it_cur,
            it_end) otherwise : INVALID_SYNTAX
            */

            switch (rule_combination[i][0]) {
                case '{':
                    if (rule_combination[i].back() != '}') {
                        throw InvalidSyntax("none", "Missing '}'");
                    }
                    prefix = "_pop_while(";
					prefix += POP_FUNCTION_PREFIX;
                    switch (res_type) {
                        case InputHandler::ResultType::ORS:
                            suffix = ", current)";
                            break;
                        case InputHandler::ResultType::TRY_CATCHS:
                        case InputHandler::ResultType::ERROR_TOKEN:
                            suffix = ", it_cur, it_end)";
                            break;
                        default:
                            throw std::invalid_argument("Not yet "
                                                        "implemented "
                                                        "here !");
                    }
                    break;

                case '<':
                    if (rule_combination[i].back() != '>') {
                        throw InvalidSyntax("none", "Missing '>'");
                    }
                    prefix = POP_FUNCTION_PREFIX;
                    switch (res_type) {
                        case InputHandler::ResultType::ORS:
                            suffix = "(createNext("
                                     "current))";
                            break;
                        case InputHandler::ResultType::TRY_CATCHS:
                        case InputHandler::ResultType::ERROR_TOKEN:
                            suffix = "(it_cur, it_end)";
                            break;
                        default:
                            throw std::invalid_argument("Not yet "
                                                        "implemented "
                                                        "here !");
                    }
                    break;

                case '"':
                    if (rule_combination[i].back() != '"' || rule_combination[i].size() < 2) {
                        throw InvalidSyntax("none", "Missing '\"'");
                    }
                    prefix = "_pop_value(\"";
                    switch (res_type) {
                        case InputHandler::ResultType::ORS:
                            suffix = "\", "
                                     "createNext("
                                     "current))";
                            break;
                        case InputHandler::ResultType::TRY_CATCHS:
                        case InputHandler::ResultType::ERROR_TOKEN:
                            suffix = "\", it_cur, "
                                     "it_end)";
                            break;
                        default:
                            throw std::invalid_argument("Not yet "
                                                        "implemented "
                                                        "here !");
                    }
                    break;
                default:
                    throw InvalidSyntax("none", "Invalid syntax");
            }
            result +=
                prefix + rule_combination[i].substr(1, rule_combination[i].size() - 2) + suffix;

            if (i >= rule_combination.size() - 1) {
                continue;
            }

            switch (res_type) {
                case InputHandler::ResultType::ORS:
                    result += " || ";
                    break;
                case InputHandler::ResultType::TRY_CATCHS:
                    result += " << ";
                    break;
                case InputHandler::ResultType::ERROR_TOKEN:
                    result += ";\n    if(!master.is_error())\n   "
                              "     master <<";
                    break;
                default:
                    throw std::invalid_argument("Not yet implemented here !");
            }
        }
        switch (res_type) {
            case InputHandler::ResultType::ORS:
                result += ") it_cur = it_start;\n";
                result += "    else {master.children() += "
                          "current; FOUND(\"" +
                          name + "\") return 0;};\n";
                break;
            case InputHandler::ResultType::TRY_CATCHS:
                result += ";\n    } catch(syntax_error &e) { "
                          "master.children().clear(); it_cur = "
                          "it_start; }\n";
                break;
            case InputHandler::ResultType::ERROR_TOKEN:
                result += ";\n    if(!master.is_error()){\n";
                result += "        FOUND(\"" + name + "\")\n";
                result += "        return master ;\n";
                result += "    } master.clear(); it_cur = "
                          "it_start;\n\n";
                break;
            default:
                throw std::invalid_argument("Not yet implemented here !");
        }
    }
    switch (res_type) {
        case InputHandler::ResultType::ORS:
            result += "    FAILED(\"" + name + "\")\n";
            result += "    return 1;\n";
            break;
        case InputHandler::ResultType::TRY_CATCHS:
            result += "    throw syntax_error(it_start);\n";
            break;
        case InputHandler::ResultType::ERROR_TOKEN:
            result += "    master.make_error();\n";
            result += "    FAILED(\"" + name + "\")\n";
            result += "    return master;\n";
            break;
        default:
            throw std::invalid_argument("Not yet implemented here !");
    }
    return result;
}

std::vector<std::string> createLexemes(FileHandler &files, uint &lineNum) {
    std::vector<std::string> lexeme_names;

    std::vector<std::string> special_lexeme_names;
    std::string line;

    while (files.getline(line)) {
        lineNum++;

        if (line.empty()) {
            continue; // skip empty line
        }
        if (line[0] == '#') {
            continue; // ingnore comment
        }
        if (line == "---") {
            break; // threat only the first part of the file
        }

        if (auto match = ctre::starts_with<LEXME_INFOS_REGEX>(line)) {
            std::string lexeme_name = match.get<1>().to_string();
            const std::string &lexme_regex = match.get<2>().to_string();

            if (std::find(lexeme_names.begin(), lexeme_names.end(), lexeme_name) !=
                lexeme_names.end()) {
                throw BnfError("Plural lexeme with the same name: " + lexeme_name);
            }

            if (lexeme_name[0] != '.') {
                lexeme_names.push_back(lexeme_name);
            } else {
                special_lexeme_names.push_back(lexeme_name);
                lexeme_name[0] = '_';
            }

            files << FileHandler::WriteMode::HPP
                  << "const std::regex " + lexeme_name + "_ = std::regex(\"" + lexme_regex + "\");"
                  << std::endl;
        } else {
            throw InvalidSyntax(lineNum, "Invalid lexeme declaration");
        }
    }

    files << FileHandler::WriteMode::CPP << std::endl
          << "constexpr Lexeme LEXEME_LIST[" << lexeme_names.size() + special_lexeme_names.size()
          << "] = {" << std::endl;

    for (const std::string &lexeme_name : lexeme_names) {
        files << FileHandler::WriteMode::CPP
              << "    {\"" + lexeme_name + "\", " + lexeme_name + "_}," << std::endl;
    }
    for (const std::string &lexeme_name : special_lexeme_names) {
        files << FileHandler::WriteMode::CPP
              << "    {\"" + lexeme_name + "\", _" + &(lexeme_name[1]) + "_},"
              << std::endl;
    }
    files << FileHandler::WriteMode::CPP << "};" << std::endl << std::endl;

    if (line != "---") {
        throw InvalidSyntax(lineNum, "Expected '---'");
    }

    return lexeme_names;
}

void writeLexemesPopFunctions(const std::vector<std::string> &token_types, FileHandler &files,
                              InputHandler::ResultType res_type) {

    for (const std::string &token_name : token_types) {
        switch (res_type) {
            case InputHandler::ResultType::ORS:
                // Writes bool __token_name(Token & master) {
                // return _type("token_name", master); }
                files << FileHandler::WriteMode::CPP << "bool " << POP_FUNCTION_PREFIX << token_name
                      << "(Token &master) { return _pop_type(\"" + token_name + "\", master); }"
                      << std::endl;
                break;
            case InputHandler::ResultType::TRY_CATCHS:
            case InputHandler::ResultType::ERROR_TOKEN:
                // Writes : Token pop_token(IT& it_cur, const
                // IT it_end) { return pop_type("token", it_cur,
                // it_end); }
                files << FileHandler::WriteMode::CPP << "Token pop_" << token_name
                      << "(IT& it_curr, const IT it_end) { "
                         "return _pop_type(\"" +
                             token_name + "\", it_curr, it_end); }"
                      << std::endl;
                break;
            default:
                throw std::invalid_argument("Not yet implemented here !");
        }
    }
    files << FileHandler::WriteMode::CPP << std::endl;
}

void addRulePopFunctions(const Rule &rule, const std::string &name,
                         std::vector<PairRuleFunction> &result, InputHandler::ResultType res_type) {
    size_t rule_id = 0;
    std::vector<std::string> newRule;
    for (size_t i = 0; i < rule.size(); i++) {
        if (rule[i] == "{") {
            std::vector<std::string> const loopRule = get_inside_brackets(rule, i, "{", "}");
            const std::string aux_name = std::to_string(rule_id) + "_" + name;
            rule_id++;
            addRulePopFunctions(loopRule, aux_name, result, res_type);
            newRule.push_back("{" + aux_name + "}");
        } else {
            newRule.push_back(rule[i]);
        }
    }
    result.emplace_back(name, generateSimpleRulePopFunction(newRule, name, res_type));
}

std::vector<std::pair<std::string, Rule>> readRules(FileHandler &files, uint &line_num) {
    std::vector<std::pair<std::string, Rule>> rules;
    std::vector<std::string> allRuleNames;
    std::string line;
    Rule currentRule;
    std::stack<LexemeName> brackets;

    while (files.getline(line)) {
        line_num++;
        if (line[0] == '#' || line.empty()) {
            continue;
        }
        if (line == "---") {
            throw InvalidSyntax(line_num, "Unexpected '---'");
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
                        throw InvalidSyntax(line_num, "Expected rule "
                                                      "name");
                    }
                    if (!brackets.empty()) {
                        throw InvalidSyntax(line_num, "Expected ')', '}' "
                                                      "or ']'");
                    }
                    rules.emplace_back(currentRule[0].substr(1, currentRule[0].size() - 2),
                                       Rule(currentRule.begin() + 2, currentRule.end()));
                    allRuleNames.push_back(currentRule[0]);
                    line_view = line_view.substr(match.second);
                    currentRule.clear();
                } else {
                    const std::string &match_str = std::string(match.first);
                    if (currentRule.empty() && name != LexemeName::RULENAME) {
                        throw InvalidSyntax(line_num, "Expected rule "
                                                      "name");
                    }
                    if (currentRule.size() == 1 && name != LexemeName::ASSIGN) {
                        throw InvalidSyntax(line_num, "Expected '::='");
                    }
                    if (currentRule.empty() && find(allRuleNames.begin(), allRuleNames.end(),
                                                    match_str) != allRuleNames.end()) {
                        throw InvalidSyntax(line_num, "Rule '" + match_str +
                                                          "' already "
                                                          "defined");
                    }
                    if (name == LexemeName::PARENTH || name == LexemeName::LOOP ||
                        name == LexemeName::OPTION) {
                        brackets.push(name);
                    } else if (name == LexemeName::ENDPARENTH || name == LexemeName::ENDLOOP ||
                               name == LexemeName::ENDOPTION) {
                        if (name == LexemeName::ENDPARENTH &&
                            brackets.top() != LexemeName::PARENTH) {
                            throw InvalidSyntax(line_num, "Unexpected"
                                                          " '" +
                                                              match_str + "'");
                        }
                        if (brackets.empty()) {
                            throw InvalidSyntax(line_num, "Unexpected"
                                                          " '" +
                                                              match_str + "'");
                        }
                        if (name == LexemeName::ENDLOOP && brackets.top() != LexemeName::LOOP) {
                            throw InvalidSyntax(line_num, "Unexpected"
                                                          " '" +
                                                              match_str + "'");
                        }
                        if (name == LexemeName::ENDOPTION && brackets.top() != LexemeName::OPTION) {
                            throw InvalidSyntax(line_num, "Unexpected"
                                                          " '" +
                                                              match_str + "'");
                        }
                        brackets.pop();
                    }

                    currentRule.push_back(match_str);
                    line_view = line_view.substr(match.second);
                }
            }
            if (!match_found) {
                throw InvalidSyntax(line_num, "Unexpected token");
            }
        }
    }
    if (!currentRule.empty()) {
        throw InvalidSyntax(line_num, "Expected ';'");
    }
    return rules;
}
void writeRulesPopFunctions(const std::vector<std::pair<std::string, Rule>> &rules,
                            FileHandler &files, InputHandler::ResultType res_type) {
    // Declare rules' pop functions
    for (const auto &[rule_name, _] : rules) {
        switch (res_type) {
            case InputHandler::ResultType::ORS:
                files << FileHandler::WriteMode::HPP << "bool " << POP_FUNCTION_PREFIX << rule_name
                      << "(Token &master);" << std::endl;
                break;
            case InputHandler::ResultType::TRY_CATCHS:
            case InputHandler::ResultType::ERROR_TOKEN:
                files << FileHandler::WriteMode::HPP << "Token " << POP_FUNCTION_PREFIX << rule_name
                      << "(IT&, const IT);" << std::endl;
                break;
            default:
                throw std::invalid_argument("Not yet implemented here !");
        }
    }
    // Define rules' pop functions
    for (const auto &[rule_name, rule_expr] : rules) {
        files << FileHandler::WriteMode::CPP << "\n";
        files << FileHandler::WriteMode::CPP
              << "/****************************************************"
                 "********"
                 "***************************************/\n";
        files << FileHandler::WriteMode::CPP
              << "/*                       Functions to pop tokens of "
                 "type : "
              << rule_name << (40UL - rule_name.size()) * std::string(" ") << "*/\n";
        files << FileHandler::WriteMode::CPP
              << "/****************************************************"
                 "********"
                 "***************************************/\n";
        std::vector<PairRuleFunction> result;
        addRulePopFunctions(rule_expr, rule_name, result, res_type);
        for (const auto &[aux_rule_name, aux_rule_func] : result) {
            files << FileHandler::WriteMode::CPP << std::endl;
            switch (res_type) {
                case InputHandler::ResultType::ORS:
                    files << FileHandler::WriteMode::CPP << "bool " << POP_FUNCTION_PREFIX
                          << aux_rule_name << "(Token &master) {" << std::endl;
                    break;
                case InputHandler::ResultType::TRY_CATCHS:
                case InputHandler::ResultType::ERROR_TOKEN:
                    files << FileHandler::WriteMode::CPP << "Token " << POP_FUNCTION_PREFIX
                          << aux_rule_name << "(IT& it_cur, const IT it_end) {" << std::endl;
                    break;
                default:
                    throw std::invalid_argument("Not yet implemented here !");
            }
            files << FileHandler::WriteMode::CPP << aux_rule_func;
            files << FileHandler::WriteMode::CPP << "}" << std::endl;
        }
    }
}

int main(int argc, char const *argv[]) {

	// Step 1: set up the configuration

    InputHandler::Configuration cfg;
    InputHandler::handleParameters(std::vector<std::string>{argv, argv + argc}, cfg);
    FileHandler files;
    files.open(cfg.input_filename, cfg.output_filepath_cpp, cfg.output_filepath_hpp);

    // Step 2: Compute the module part
	//   syntax still to decide, but kiss

	// Step 3: Compute the lexer part
	//    
	//    return a list of couples (name, regex)
	//  Lex the 

	// Step 4: Compute the parser part
	// 

	// Step 5: Generate the code
	//

    files.copy("template/head.hpp", FileHandler::WriteMode::HPP);

    files << FileHandler::WriteMode::CPP << "#include " << cfg.output_filepath_hpp.filename()
          << std::endl;

    uint lineNum = 0;
    const std::vector<std::string> lexeme_names = createLexemes(files, lineNum);

    files.copy("template/token.cpp", FileHandler::WriteMode::CPP);
    files.copy("template/lex.cpp", FileHandler::WriteMode::CPP);

    switch (cfg.result_type) {
        case InputHandler::ResultType::ORS:
            files.copy("template/value_expression.cpp", FileHandler::WriteMode::CPP);
            break;
        case InputHandler::ResultType::TRY_CATCHS:
            files.copy("template/value_expression2.cpp", FileHandler::WriteMode::CPP);
            break;
        case InputHandler::ResultType::ERROR_TOKEN:
            files.copy("template/value_expression3.cpp", FileHandler::WriteMode::CPP);
            break;
        default:
            throw std::invalid_argument("Not yet implemented here !");
    }

    writeLexemesPopFunctions(lexeme_names, files, cfg.result_type);

    const std::vector<std::pair<std::string, Rule>> rules = readRules(files, lineNum);
    #ifdef DEBUG_RULES
		for(auto [rule_name, _] : rules) {
			cout << rule_name << std::endl;
			cout << _ << std::endl;
		}
	#endif
    writeRulesPopFunctions(rules, files, cfg.result_type);

    files << FileHandler::WriteMode::CPP << std::endl;

    files.copy("template/main.cpp", FileHandler::WriteMode::CPP);

    return 0;
}

#include "rules/rules.hpp"
#include "input_handler/input_handler.hpp"
#include <regex>

struct PairRuleFunction {
    PairRuleFunction(const std::string &name, const std::string &functionName)
        : name(name), function(functionName) {}
    std::string name;
    std::string function;
};

struct PatternParser {
    std::string pattern;
    std::string replacement;
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
                                          const std::string &name,
                                          InputHandler::Configuration &cfg) {
    std::string result{"    const IT it_start = it_cur;\n"
                       "    master = Token(\"" +
                       name +
                       "\", \"\", it_start->line(), it_start->column()); \n"
                       "    std::vector<Token> current;\n"
                       "    SEARCH(\"" +
                       name + "\")\n"};

    for (std::vector<std::string> &rule_combination : generateCombinations(rule)) {
        result += "    current.clear();\n"
                  "    if(";

        for (size_t i = 0; i < rule_combination.size(); i++) {
            /*
            {truc}->pop_while(truc, current) OU pop_while(truc,
            it_cur, it_end) <truc>->pop_truc(createNext(current))
            OU pop_truc(it_cur, it_end) "truc"->pop_value("truc",
            createNext(current)) OU pop_value("truc", it_cur,
            it_end) otherwise : INVALID_SYNTAX
            */

            // std::vector<PatternParser> const patterns = {
            //     {R"-(^\{([a-zA-Z0-9_]+)\}$)-"       , R"-(_pop_while(pop_$1, current))-"},
            //     {R"-(^<([a-zA-Z_][a-zA-Z0-9_]*)>$)-", R"-(pop_$1(createNext(current)))-"},
            //     {R"-(^\"([^\"]*)\"$)-"              , R"-(_pop_value("$1", createNext(current)))-"},
            //     {R"-(^:([a-zA-Z_][a-zA-Z0-9_]*)$)-" , R"-(cgg::$1(it_cur, it_end))-"},
            // };

            // std::string rule_element = rule_combination[i];
            // bool found = false;
            // for (PatternParser const &p : patterns) {
            //     std::smatch m;
            //     if (std::regex_match(rule_element, m, std::regex(p.pattern))) {
            //         rule_element =
            //             std::regex_replace(rule_element, std::regex(p.pattern), p.replacement);
            //         found = true;
            //         break;
            //     }
            // }

            std::string rule_element = rule_combination[i];
            std::string generated_element;
            bool found = false;
            for (const auto &gen : cfg.gen_ggram_file) {
                if (gen(rule_element, generated_element)) {
                    rule_element = generated_element;
                    found = true;
                    break;
                }
            }

            if (!found) {
                throw SyntaxError("Invalid syntax", 0);
            }

            result += rule_element;

            if (i >= rule_combination.size() - 1) {
                continue;
            }
            result += " || ";
        }
        result += ") it_cur = it_start;\n"
                  "    else {master.children() += current; FOUND(\"" +
                  name + "\") return false;};\n";
    }
    result += "    FAILED(\"" + name +
              "\")\n"
              "    return true;\n";

    return result;
}

void addRulePopFunctions(const Rule &rule, const std::string &name,
                         std::vector<PairRuleFunction> &result,
                         InputHandler::Configuration &cfg) {
    size_t rule_id = 0;
    std::vector<std::string> newRule;
    for (size_t i = 0; i < rule.size(); i++) {
        if (rule[i] == "{") {
            std::vector<std::string> const loopRule = getInsideBrackets(rule, i, "{", "}");
            const std::string aux_name = std::to_string(rule_id) + "_" + name;
            rule_id++;
            addRulePopFunctions(loopRule, aux_name, result, cfg);
            newRule.push_back("{" + aux_name + "}");
        } else {
            newRule.push_back(rule[i]);
        }
    }
    result.emplace_back(name, generateSimpleRulePopFunction(newRule, name, cfg));
}

std::vector<std::pair<std::string, Rule>> readRules(FileHandler &files,
                                                    InputHandler::Configuration &cfg) {
    std::vector<std::pair<std::string, Rule>> rules;
    std::unordered_set<std::string> allRuleNames;
    std::string line;
    Rule currentRule;
    std::string currentRuleName;
    std::stack<std::string_view> brackets;
    bool assigned = false;

    while (files.getline(line)) {
        if (line[0] == '#' || line.empty()) {
            continue;
        }
        if (line == FILE_SEPARATOR) {
            throw SyntaxError("Unexpected '" + std::string(FILE_SEPARATOR) + "'",
                              files.getCurrentLineNumber());
        }
        std::string_view line_view = line;
        std::string error;
        bool match_found = false;
        while (!line_view.empty()) {
            match_found = false;
            for (auto it = cfg.lex_ggram_file.begin();
                 it != cfg.lex_ggram_file.end() && !match_found; it++) {
                const auto &[name, matcher, parser] = *it;
                if (matcher == nullptr) {
                    throw std::runtime_error("Matcher is null");
                }
                const MatchResult result = matcher(line_view);
                if (result == std::nullopt) {
                    continue;
                }

                const auto &match = result.value();
                const std::string &match_str = std::string(match.first);

                bool const temp = parser(brackets, currentRule, allRuleNames, currentRuleName,
                                         assigned, match_str, error, rules);
                if (temp) {
                    throw SyntaxError(error, files.getCurrentLineNumber());
                }

                match_found = true;
                line_view = line_view.substr(match.second);
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
                            FileHandler &files, InputHandler::Configuration &cfg) {
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
        addRulePopFunctions(rule_expr, rule_name, result, cfg);
        for (const auto &[aux_rule_name, aux_rule_func] : result) {
            files << FileHandler::WriteMode::CPP << std::endl
                  << "bool " << POP_FUNCTION_PREFIX << aux_rule_name << "(Token &master) {"
                  << std::endl
                  << aux_rule_func << "}" << std::endl;
        }
    }
}

void writeParseFunction(FileHandler &files, InputHandler::Configuration &cfg) {
    files << FileHandler::WriteMode::CPP << std::endl
          << "Token parse(const std::string &code) {" << std::endl
          << "    std::vector<Token> tokens = lex(code);" << std::endl;

    for (const auto preFun : cfg.pre_functions) {
        preFun(files);
    }
    
    files << FileHandler::WriteMode::CPP << std::endl
          << "    return parse(tokens);" << std::endl
          << "}" << std::endl;
}
#include <iostream>
#include <string>
#include <vector>

#include "file_handler.hpp"
#include "input_handler/input_handler.hpp"
#include "lexme/lexme.hpp"
#include "regex/regex.hpp"
#include "rules/parser.hpp"
#include "rules/rules.hpp"

void initOutputFiles(const InputHandler::Configuration &cfg, FileHandler &files) {
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

    files.copy("template/token.cpp", FileHandler::WriteMode::CPP);
    files.copy("template/lex.cpp", FileHandler::WriteMode::CPP);
    files.copy("template/value_expression.cpp", FileHandler::WriteMode::CPP);
}

int main(int argc, char const *argv[]) {
    InputHandler::Configuration cfg;

    cfg.lex_ggram_file = {// iteration and append (for lib), that is why we use std::vector
                          {"IGNORE", ignoreMatcher, ignoreParser},
                          {"COMMENT", commentMatcher, commentParser},
                          {"RULENAME", ruleNameMatcher, ruleNameParser},
                          {"ASSIGN", assignMatcher, assignParser},
                          {"OR", orMatcher, orParser},
                          {"PARENTH", parenthMatcher, parenthParser},
                          {"ENDPARENTH", endParenthMatcher, endParenthParser},
                          {"LOOP", loopMatcher, loopParser},
                          {"ENDLOOP", endLoopMatcher, endLoopParser},
                          {"OPTION", optionMatcher, optionParser},
                          {"ENDOPTION", endOptionMatcher, endOptionParser},
                          {"STRING", stringMatcher, stringParser},
                          {"END", endMatcher, endParser}};

    if (!InputHandler::handleParameters(std::vector<std::string>{argv, argv + argc}, cfg)) {
        return 1;
    }
    FileHandler files;
    initOutputFiles(cfg, files);
    try {
        const auto lexeme_names = createLexemes(files);
        writeLexemesPopFunctions(lexeme_names, files);
        const std::vector<std::pair<std::string, Rule>> rules = readRules(files, cfg);
#ifdef DEBUG_RULES
        for (auto [rule_name, _] : rules) {
            cout << rule_name << std::endl;
            cout << _ << std::endl;
        }
#endif
        writeRulesPopFunctions(rules, files);
    } catch (const SyntaxError &e) {
        e.print(files);
        return 2;
    }

    files << FileHandler::WriteMode::CPP << std::endl;

    files.copy("template/main.cpp", FileHandler::WriteMode::CPP);

    return 0;
}

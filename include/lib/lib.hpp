#pragma once

#include "input_handler/types.hpp"
#include "utils.hpp"

struct Library {
    std::vector<InputHandler::LexerRule> lex_ggram_rules;
    std::vector<InputHandler::GenRule> gen_ggram_rules;
    std::vector<InputHandler::PreFunction> pre_functions;

    void loadLibrary(InputHandler::Configuration &config) const {
        extend(config.gen_ggram_rules, this->gen_ggram_rules);
        extend(config.lex_ggram_rules, this->lex_ggram_rules);
        extend(config.pre_functions, this->pre_functions);
    }
};

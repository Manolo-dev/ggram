#pragma once

#include "ctre/ctre.hpp"
#include "error/syntax_error.hpp"
#include "file_handler.hpp"
#include "global_defines.hpp"
#include "regex/lexme_regex.hpp"
#include "regex/regex.hpp"
#include <iostream>
#include <string>
#include <unordered_set>

using LexmeList = std::unordered_set<std::string>;

/**
 * @brief Build the list of tokens regexes from the gg input file
 *
 * @param files the file handler
 * @return LexmeList the list of tokens names
 */
LexmeList createLexemes(FileHandler &files);

/**
 * @brief write the functions used to pop the tokens from the lexer stack
 *
 * @param token_types the list of token types
 * @param files the file handler
 */
void writeLexemesPopFunctions(const LexmeList &token_types, FileHandler &files) noexcept;

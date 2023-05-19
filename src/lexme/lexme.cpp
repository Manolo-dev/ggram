#include "lexme/lexme.hpp"

/**
 * @brief Read all the tokens regexes from the input file
 *
 * @param files the file handler
 * @return std::pair<LexmeList, LexmeList> the list of tokens names and the list of special tokens
 * names
 */
std::pair<LexmeList, LexmeList> readLexmes(FileHandler &files) {
    LexmeList lexeme_names;
    LexmeList special_lexeme_names;
    std::string line;

    while (files.getline(line) && line != FILE_SEPARATOR) {
        if (line.empty()) {
            continue; // skip empty line
        }

        if (line[0] == '#') {
            continue; // ingnore comment
        }

        if (auto match = ctre::starts_with<LEXME_INFOS_REGEX>(line)) {
            std::string lexeme_name = match.get<1>().to_string();
            const std::string &lexme_regex = match.get<2>().to_string();

            if (lexeme_names.find(lexeme_name) != lexeme_names.end()) {
                throw SyntaxError("Plural lexeme with the same name: " + lexeme_name,
                                  files.getCurrentLineNumber(), ErrorType::BnfError);
            }

            if (lexeme_name[0] != '.') {
                lexeme_names.emplace(lexeme_name);
            } else {
                special_lexeme_names.emplace(lexeme_name);
                lexeme_name[0] = '_';
            }

            files << FileHandler::WriteMode::HPP
                  << "const std::regex " + lexeme_name + "_ = std::regex(\"" + lexme_regex + "\");"
                  << std::endl;
        } else {
            throw SyntaxError("Invalid lexeme declaration", files.getCurrentLineNumber());
        }
    }

    if (line != FILE_SEPARATOR) {
        throw SyntaxError("Expected " + std::string(FILE_SEPARATOR), files.getCurrentLineNumber());
    }

    return std::make_pair(lexeme_names, special_lexeme_names);
}

LexmeList createLexemes(FileHandler &files) {
    auto [lexeme_names, special_lexeme_names] = readLexmes(files);
    files << FileHandler::WriteMode::HPP << std::endl
          << "constexpr Lexeme LEXEME_LIST[" << lexeme_names.size() + special_lexeme_names.size()
          << "] = {" << std::endl;

    for (const std::string &lexeme_name : lexeme_names) {
        files << FileHandler::WriteMode::HPP
              << "    {\"" + lexeme_name + "\", " + lexeme_name + "_}," << std::endl;
    }
    for (const std::string &lexeme_name : special_lexeme_names) {
        files << FileHandler::WriteMode::HPP
              << "    {\"" + lexeme_name + "\", _" + &(lexeme_name[1]) + "_}," << std::endl;
    }
    files << FileHandler::WriteMode::HPP << "};" << std::endl << std::endl;

    return lexeme_names;
}

void writeLexemesPopFunctions(const LexmeList &token_types, FileHandler &files) {
    for (const std::string &token_name : token_types) {
        files << FileHandler::WriteMode::CPP << "bool " << POP_FUNCTION_PREFIX << token_name
              << "(Token &master) { return _pop_type(\"" + token_name + "\", master); }"
              << std::endl;
    }
    files << FileHandler::WriteMode::CPP << std::endl;
}

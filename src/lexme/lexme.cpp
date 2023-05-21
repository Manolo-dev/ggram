#include "lexme/lexme.hpp"

bool isRegexEscapeChar(char c) noexcept {
	return c == '.' || c == '*' || c == '+' || c == '?' || c == '|' || c == '(' || c == ')' ||
		   c == '[' || c == ']' || c == '{' || c == '}' || c == '\\' || c == '^' || c == '$' ||
		   c == '-' || c == 'b' || c == 'B' || c == 's' || c == 'S' || c == 'w' || c == 'W' ||
		   c == 'd' || c == 'D';
}

/**
 * @brief This functions is used to double the backslash in the given regex if it escape a regex
 * special character
 *
 * @param regex the regex to escape between double quotes
 * @return std::string the escaped regex
 */
std::string escapeRegexSpetialChar(const std::string_view &regex) {
    std::string result;
    result.reserve(regex.size());
    for (size_t i = 0; i < regex.size(); i++) {
        if (regex[i] == '\\' && isRegexEscapeChar(regex[i + 1])) {
            result += "\\";
        }
        result += regex[i];
    }
	return result;
}

[[noreturn]] void manageErrors(const std::string &line, unsigned long line_number) {
    size_t pos = line.find_first_of(' ');
    if (pos == std::string::npos) {
        pos = line.size();
    }

    if (line[0] == '<') {
        throw SyntaxError("Invalid lexeme name, you may have forgoten file separator '" +
                              std::string(FILE_SEPARATOR) + "' before rule declaration",
                          line_number, 1, pos, ErrorType::BnfError);
    }

    if (std::string_view(line).substr(0, FILE_SEPARATOR.size()) == FILE_SEPARATOR) {
        throw SyntaxError("File separator '" + std::string(FILE_SEPARATOR) +
                              "' must be alone on the line",
                          line_number, 4, line.size(), ErrorType::BnfError);
    }

    throw SyntaxError("Invalid lexeme declaration", line_number, 1, pos, ErrorType::BnfError);
}

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
                                  files.getCurrentLineNumber(), 1, lexeme_name.size(),
                                  ErrorType::BnfError);
            }

            if (lexeme_name[0] == '.') {
                special_lexeme_names.emplace(lexeme_name);
                lexeme_name[0] = '_';
            } else {
                lexeme_names.emplace(lexeme_name);
            }

            files << FileHandler::WriteMode::HPP
                  << "const std::regex " + lexeme_name + "_ = std::regex(" + escapeRegexSpetialChar(lexme_regex) + ");"
                  << std::endl;
        } else {
            if (auto nameMatch = ctre::starts_with<LEXME_NAME_REGEX>(line)) {
                throw SyntaxError("Invalid lexeme regex", files.getCurrentLineNumber(),
                                  nameMatch.size() + 1, line.size(), ErrorType::RegexError);
            }
            manageErrors(line, files.getCurrentLineNumber());
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

void writeLexemesPopFunctions(const LexmeList &token_types, FileHandler &files) noexcept {
    for (const std::string &token_name : token_types) {
        files << FileHandler::WriteMode::CPP << "bool " << POP_FUNCTION_PREFIX << token_name
              << "(Token &master) { return _pop_type(\"" + token_name + "\", master); }"
              << std::endl;
    }
    files << FileHandler::WriteMode::CPP << std::endl;
}

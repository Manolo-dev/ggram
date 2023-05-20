

std::vector<Token> lex(std::string code) {
    /**
     * @brief Lex the code
     * @param code
     * @return std::vector<Token>
     */

    std::vector<Token> tokens{}; // Vector of tokens
    int column = 0;
    int line = 0;

    // Try to split the code in lexeme by removing the first one each time
    while (code.size() > 0) {
        // Try to match every lexeme with the beginning of the remaining code
        // Ifno lexeme match, we raise an error
        bool found = false;
        for (const Lexeme &lexeme : LEXEME_LIST) {
            const std::regex current_regex(lexeme.regex); // Regex forthe current lexeme
            std::smatch match;
            // Try to match current regex at the beginning of the remaining code
            if (regex_search(code, match, current_regex,
                             std::regex_constants::match_continuous |
                                 std::regex_constants::match_not_null)) {

                // Ifthe lexeme is not to be ignored
                if (strcmp(lexeme.name, ".ignore")) {
                    tokens.emplace_back(lexeme.name, match.str(), line, column);
                }

                // Update line and column
                for (const char &c : match.str()) {
                    if (c == '\r' || c == '\n') {
                        line++;
                        column = 0;
                    } else {
                        column++;
                    }
                }
                // Remove the matched part from the code
                code = match.suffix();
                found = true;
                break;
            }
        }
        if (!found) {
            throw std::runtime_error("No matching pattern :\n" + code);
        }
    }

    return tokens;
}

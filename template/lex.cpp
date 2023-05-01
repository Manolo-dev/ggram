

vector<Token> lex(string code) {
    /**
     * @brief Lex the code
     * @param code
     * @return vector<Token>
     */

    vector<Token> tokens{}; // Vector of tokens
    int column = 0;
    int line = 0;

    // Try to split the code in lexeme by removing the first one each time
    while(code.size() > 0) {
        // Try to match every lexeme with the beginning of the remaining code
        // If no lexeme match, we raise an error
        bool found = false;
        for(const Lexeme &lexeme : LEXEME_LIST) {
            const regex current_regex(lexeme.regex); // Regex for the current lexeme
            smatch match;
            // Try to match current regex at the beginning of the remaining code
            if( regex_search(code, match, current_regex,
                regex_constants::match_continuous | regex_constants::match_not_null) ) {

                // If the lexeme is not to be ignored
                if(strcmp(lexeme.name, ".ignore")) {
                    tokens.emplace_back(lexeme.name, match.str(), line, column);
                }

                // Update line and column
                for(const char& c : match.str()){
                    if(c == '\r' || c == '\n'){
                        line++;
                        column = 0;
                    } else{
                        column++;
                    }
                }
                // Remove the matched part from the code
                code = match.suffix();
                found = true;
                break;
            }
        }
        if(!found) {
            throw runtime_error("No matching pattern :\n" + code) ;
        }
    }

    return tokens;
}

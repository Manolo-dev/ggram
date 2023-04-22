void create_lexemes(vector<Lexeme> &lexemes);

vector<Token> lex(string code) {
    /**
     * @brief Lex the code
     * @param code
     * @return vector<Token>
     */
    vector<Lexeme> lexemes;
    create_lexemes(lexemes);

    vector<Token> tokens = vector<Token>(); // Vector of tokens
    int column = 0;
    int line = 0;

    while(code.size() > 0) {
        bool found = false;
        for(long unsigned int i = 0; i != lexemes.size(); ++i) {
            regex re(lexemes[i].value()); // Regex for the current lexeme
            smatch match; // Match for the current lexeme
            if(regex_search(code, match, re)) {
                if(match.position() == 0 && match.length() > 0) { // If the match is at the good position
                    if(lexemes[i].type() != ".ignore") { // If the lexeme is not to be ignored
                        tokens.push_back(Token(lexemes[i].type(), match.str(), line, column));
                    }
                    if(match.str().find("\n") != string::npos || match.str().find("\r") != string::npos) {
                        // Increment line and reset column
                        size_t count = count_if(
                            match.str().begin(),
                            match.str().end(),
                            [](char c){ return c == '\n' || c == '\r'; }
                        ) / 2;
                        line += count;
                        column = -count;
                    }
                    column += match.length(); // Increment column
                    code = match.suffix(); // Remove the matched part from the code
                    found = true;
                    break;
                }
            }
        }
        if(!found) {
            cerr << "ERROR" << endl;
            exit(1);
        }
    }

    return tokens;
}
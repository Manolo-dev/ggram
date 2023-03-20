using IT = vector<Token>::iterator;

IT it;
IT it_e;
IT it_label;
string it_label_value;

void create_lexemes(vector<Lexeme> &lexemes);

vector<Token> lex(string code) {
    vector<Lexeme> lexemes;
    create_lexemes(lexemes);

    vector<Token> tokens = vector<Token>();

    while(code.size() > 0) {
        bool found = false;
        for(long unsigned int i = 0; i != lexemes.size(); ++i) {
            regex re(lexemes[i].value());
            smatch match;
            if(regex_search(code, match, re)) {
                if(match.position() == 0 && match.length() > 0) {
                    tokens.push_back(Token(lexemes[i].type(), match.str(), 0, 0));
                    code = match.suffix();
                    found = true;
                    break;
                }
            }
        }
        if(!found) {
            code = code.substr(1);
        }
    }

    return tokens;
}
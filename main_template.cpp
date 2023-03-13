int main() {
    string code;
    getline(cin, code);
    vector<Token> tokens = lex(code);
    IT it = tokens.begin();
    
    try {
        __expr(it, tokens.end());
    } catch(const char* msg) {
        cout << "BAD" << endl;
        return 0;
    }

    return 0;
}

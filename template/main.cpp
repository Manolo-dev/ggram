Token parse(const string& code){
    vector<Token> tokens = lex(code);
    // cout << tokens << endl;
    return parse(tokens);
}

int main() {

    try{
        Token res = parse("a = 1 \n b = a + 1 c = b + 1");
        cout << "OK" << endl;
        cout << res;
    } catch(syntax_error&) {
        cout << "ERROR" << endl;
    }

    return 0;
}
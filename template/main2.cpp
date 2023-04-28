Token parse(vector<Token>& v){
    IT it = v.begin();
    return pop_program(it, v.end());
}

int main() {
    string code;
    code = "a = 1 \n b = a + 1 c = b + 1";
    vector<Token> tokens = lex(code);
    cout << tokens << endl;
    
    try{
        Token res = parse(tokens);
        cout << "OK" << endl;
        cout << res;
    } catch(syntax_error&) {
        cout << "ERROR" << endl;
    }

    return 0;
}
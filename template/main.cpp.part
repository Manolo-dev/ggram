int main() {
    string code;
    code = "a = 1 b = a + 1 c = b + 1";
    vector<Token> tokens = lex(code);
    cout << tokens << endl;
    it = tokens.begin();
    it_e = tokens.end(); // end
    p_err = nullptr; // error
    Token tree;
    
    if(pop_program(tree)) {
        cout << "ERROR" << endl;
        cout << *p_err << endl;
    } else {
        cout << "OK" << endl;
        cout << tree << endl;
    }

    return 0;
}
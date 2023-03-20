int main() {
    string code;
    getline(cin, code);
    vector<Token> tokens = lex(code);
    it = tokens.begin();
    it_e = tokens.end();
    
    if(__expr()) {
        cout << "ERROR" << endl;
    } else {
        cout << "OK" << endl;
        IT t = tokens.begin();
        while(t != it) {
            cout << t->value() << " ";
            t++;
        }
        cout << endl;
    }

    return 0;
}
Token parse(const string& code){
    vector<Token> tokens = lex(code);
    // cout << tokens << endl;
    return parse(tokens);
}

#include <chrono>

using namespace std::chrono;

int main() {
    string code = "" ;
    for(char letter = 'a' ; letter <= 'z' ; letter++){
        code += letter;
        code += " = ";
        for(char letter2 = 'a' ; letter2 < letter ; letter2++){
            code += letter2;
            code += " + ";
        }
        code += "1\n";
    }
    // cout << code;
    
    microseconds duration(0); 
    for(int i = 0; i < 100; i++){
        auto start = high_resolution_clock::now();
        try{
            Token res = parse(code);
            // cout << "OK" << endl;
            // cout << res;
        } catch(syntax_error& e) {
            cout << "ERROR AT :" << e.get_error_token() << endl;
        }
        auto stop = high_resolution_clock::now();
        duration += duration_cast<microseconds>(stop - start);
    }
    cout << "Time taken by function: " << duration_cast<milliseconds>(duration).count() << " ms" << endl;

    return 0;
}
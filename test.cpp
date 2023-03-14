#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

class Token {
    public:
        Token(string type, string value, int line, int column) {
            this->_type   = type;
            this->_value  = value;
            this->_line   = line;
            this->_column = column;
        }
        string type() {
            return this->_type;
        }
        string value() {
            return this->_value;
        }
        int line() {
            return this->_line;
        }
        int column() {
            return this->_column;
        }
    private:
        string _type;
        string _value;
        int _line;
        int _column;
};

class Lexeme {
    public:
        Lexeme(string type, string value) {
            this->_type   = type;
            this->_value  = value;
        }
        string type() {
            return this->_type;
        }
        regex value() {
            return this->_value;
        }
    private:
        string _type;
        regex _value;
};

using IT = vector<Token>::iterator;

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

void create_lexemes(vector<Lexeme> &lexemes) {
    lexemes.push_back(Lexeme("int", "([1-9][0-9]*)"));
    lexemes.push_back(Lexeme("add", "(\\+)"));
    lexemes.push_back(Lexeme("sub", "(-)"));
    lexemes.push_back(Lexeme("mult", "(\\*)"));
    lexemes.push_back(Lexeme("fun", "([a-z]+)"));
}

void __int(IT&it, IT e) { if(it == e) throw "BAD"; if(it->type() != "int") throw "BAD"; it++; }
void __add(IT&it, IT e) { if(it == e) throw "BAD"; if(it->type() != "add") throw "BAD"; it++; }
void __sub(IT&it, IT e) { if(it == e) throw "BAD"; if(it->type() != "sub") throw "BAD"; it++; }
void __mult(IT&it, IT e) { if(it == e) throw "BAD"; if(it->type() != "mult") throw "BAD"; it++; }
void __fun(IT&it, IT e) { if(it == e) throw "BAD"; if(it->type() != "fun") throw "BAD"; it++; }
void __expr(IT&it, IT e);

void __expr(IT&it, IT e) {
    IT t;t = it;try {__int(it, e);} catch(...) {it = t;t = it;try {__add(it, e);__expr(it, e);} catch(...) {it = t;t = it;try {__sub(it, e);__expr(it, e);} catch(...) {it = t;t = it;try {__mult(it, e);__expr(it, e);} catch(...) {it = t;__fun(it, e);__int(it, e);while(true) {t = it; try {__int(it, e);} catch(...) {it = t;break;}}}}}}
}

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

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

IT it;
IT it_e;

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
    lexemes.push_back(Lexeme("a", "(a)"));
    lexemes.push_back(Lexeme("b", "(b)"));
    lexemes.push_back(Lexeme("c", "(c)"));
    lexemes.push_back(Lexeme("d", "(d)"));
    lexemes.push_back(Lexeme("e", "(e)"));
}

bool __a() { if(it == it_e) return 1; if(it->type() != "a") return 1; it++; return 0; }
bool __b() { if(it == it_e) return 1; if(it->type() != "b") return 1; it++; return 0; }
bool __c() { if(it == it_e) return 1; if(it->type() != "c") return 1; it++; return 0; }
bool __d() { if(it == it_e) return 1; if(it->type() != "d") return 1; it++; return 0; }
bool __e() { if(it == it_e) return 1; if(it->type() != "e") return 1; it++; return 0; }
bool __expr();

bool __expr() {
    IT t = it;
    if(__a() || __b() || __d()) it = t;
    else return 0;
    if(__a() || __c() || __expr() || __d()) it = t;
    else return 0;
    return 1;
}

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
    }

    return 0;
}

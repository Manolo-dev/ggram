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
                    if(lexemes[i].type() != ".ignore") {
                        tokens.push_back(Token(lexemes[i].type(), match.str(), 0, 0));
                    }
                    code = match.suffix();
                    found = true;
                    break;
                }
            }
        }
        if(!found) {
            cerr << "ERROR" << endl;
            exit(1);
            //code = code.substr(1);
        }
    }

    return tokens;
}

void create_lexemes(vector<Lexeme> &lexemes) {
    lexemes.push_back(Lexeme("int", "([1-9][0-9]*|0)"));
    lexemes.push_back(Lexeme("float", "([0-9]+\\.[0-9]+)"));
    lexemes.push_back(Lexeme("string", "(\"[^\"]*\")"));
    lexemes.push_back(Lexeme("char", "('[^']*')"));
    lexemes.push_back(Lexeme("id", "([a-zA-Z_][a-zA-Z0-9_]*)"));
    lexemes.push_back(Lexeme("operator", "([\\+\\-\\*/%&|^<>!$=]+)"));
    lexemes.push_back(Lexeme("bracket", "(\\(|\\))"));
    lexemes.push_back(Lexeme("comma", "(,)"));
    lexemes.push_back(Lexeme(".ignore", "([ \t\r\n]+)"));
}

bool _loop(bool (*f)()) { if(f()) return 1; while(1) if(f()) break; return 0; }
bool __(string val) { if(it == it_e) return 1; if(it->value() != val) return 1; it++; return 0; }

bool __int() { if(it == it_e) return 1; if(it->type() != "int") return 1; it++; return 0; }
bool __float() { if(it == it_e) return 1; if(it->type() != "float") return 1; it++; return 0; }
bool __string() { if(it == it_e) return 1; if(it->type() != "string") return 1; it++; return 0; }
bool __char() { if(it == it_e) return 1; if(it->type() != "char") return 1; it++; return 0; }
bool __id() { if(it == it_e) return 1; if(it->type() != "id") return 1; it++; return 0; }
bool __operator() { if(it == it_e) return 1; if(it->type() != "operator") return 1; it++; return 0; }
bool __bracket() { if(it == it_e) return 1; if(it->type() != "bracket") return 1; it++; return 0; }
bool __comma() { if(it == it_e) return 1; if(it->type() != "comma") return 1; it++; return 0; }
bool _0_program();
bool _1_program();
bool __program();
bool __expr();
bool _0_instructions();
bool _1_instructions();
bool __instructions();
bool __casted();
bool _0_fun();
bool __fun();

bool _0_program() {
    IT t = it;
    if(__fun()) it = t;
    else return 0;
    return 1;
}

bool _1_program() {
    IT t = it;
    if(__expr()) it = t;
    else return 0;
    return 1;
}

bool __program() {
    IT t = it;
    if(_loop(_0_program)) it = t;
    else return 0;
    if(_loop(_1_program)) it = t;
    else return 0;
    return 1;
}

bool __expr() {
    IT t = it;
    if(__casted() || __operator() || __expr()) it = t;
    else return 0;
    if(__casted()) it = t;
    else return 0;
    return 1;
}

bool _0_instructions() {
    IT t = it;
    if(__expr()) it = t;
    else return 0;
    return 1;
}

bool _1_instructions() {
    IT t = it;
    if(__fun()) it = t;
    else return 0;
    return 1;
}

bool __instructions() {
    IT t = it;
    if(__("(") || _loop(_0_instructions) || __(")")) it = t;
    else return 0;
    if(__("(") || _loop(_1_instructions) || __(")")) it = t;
    else return 0;
    return 1;
}

bool __casted() {
    IT t = it;
    if(__("(") || __expr() || __(")")) it = t;
    else return 0;
    if(__int()) it = t;
    else return 0;
    if(__float()) it = t;
    else return 0;
    if(__string()) it = t;
    else return 0;
    if(__char()) it = t;
    else return 0;
    if(__instructions()) it = t;
    else return 0;
    if(__id()) it = t;
    else return 0;
    return 1;
}

bool _0_fun() {
    IT t = it;
    if(__expr() || __(",")) it = t;
    else return 0;
    return 1;
}

bool __fun() {
    IT t = it;
    if(__id() || _loop(_0_fun) || __expr()) it = t;
    else return 0;
    if(__id() || __expr()) it = t;
    else return 0;
    if(__id()) it = t;
    else return 0;
    return 1;
}

int main() {
    string code;
    getline(cin, code);
    vector<Token> tokens = lex(code);
    it = tokens.begin();
    it_e = tokens.end();
    
    if(__program()) {
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

//                               #%%(#((((((##%%#                                 
//                               ,#////(/%#%#####                                 
//                              %%#**//(((&&&&#/#                                 
//                            &%%%*/**/((%#%&@%*(                                 
//                           %##%%****/##%%####%%&%%%%%%&&&                       
//                           ##%%%(**/****,***/%%%(/&#######%&&@                  
//                          .####%#/**(/...,,..... ../ %%#(#####%&@@              
//                           %###%%#/,.../&%%#(((#((( .    %#(///((#%&&           
//                          ,%%%###%#%/,,*,**&&%#(//(//((#%,,, %((((((((##.       
//                           %&&%&%%%%&%%/(&&&%%%##(((((#&&&%#%* ..,#(*/**/(      
//                           ,&&&%%##(((###%(    &&%###(((#((#%#(,#%*.*//////     
//                            &%%###(/#%##/(/          *@%%%&@&%%###%#* .,*/*     
//                           ((#(#(*/%##%(((/                   .#*/(%,**((,      
//                          ,(((#/**%%#//&(/                  (,*/###%(###.       
//                          #((((,%%,//%%(/.                #//(//*#%##(/         
//                         #((((*%.,*###//,                %(((/,*(%#//,          
//                         %#(((**,#(###/*                 &%#((,/%((,            
//                         &(((/*&(*/(%(                  ###%(,*%(#,             
//                        .((#/*%/./(##                   (#&(.*%((               
//                        %#((*&/*#(.                    (#%(.,#/*                
//                        %##(//*%(,.                   ,%&(.#(/(                 
//                     .%##%/**,.*,                     %##,%#(/                  
//                   (/(#./..,***                      ((*,##*.                   
//                 //(##(( *,,,                      ,(//,,(,                     
//                (((#///#/*.                       /*/%*///#&                    
//               %#(/,(((#**                       (****,*(%#&&%@                 
//               &&%#%%&/.*                         *(((((((((#%%@%%#%(           
//              (##*##/**                                     (##(//*#(((         
//             ,##%(%/,/                                                          
//             %%.****                                                            
//            #%(***.                                                             
//          .(%/..,.                                                              
//       ##((*/,**/                                                               
//      *###/(*,*,                                                                
//       ((/(.,*/%                                                                
//          %(../*                                                                
//           .*,/(                                                                
//            /*//&                                                               
//             /*./&&                                                             
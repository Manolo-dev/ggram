#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

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

class Token {
public:
    Token() {
        this->_type   = "";
        this->_value  = "";
        this->_line   = 0;
        this->_column = 0;
    }
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

class TreeToken {
public:
    TreeToken() {
        this->_type   = "";
        this->_value  = "";
        this->_line   = 0;
        this->_column = 0;
        this->_children = vector<TreeToken>();
    }
    TreeToken(string type) {
        this->_type   = type;
        this->_value  = "";
        this->_line   = 0;
        this->_column = 0;
        this->_children = vector<TreeToken>();
    }
    TreeToken(string type, string value) {
        this->_type   = type;
        this->_value  = value;
        this->_line   = 0;
        this->_column = 0;
        this->_children = vector<TreeToken>();
    }
    TreeToken(string type, string value, int line, int column) {
        this->_type   = type;
        this->_value  = value;
        this->_line   = line;
        this->_column = column;
        this->_children = vector<TreeToken>();
    }
    TreeToken(Token token) {
        this->_type   = token.type();
        this->_value  = token.value();
        this->_line   = token.line();
        this->_column = token.column();
        this->_children = vector<TreeToken>();
    }
    void push(Token token) {
        if(this->_children.size() == 0) {
            this->_line = token.line();
            this->_column = token.column();
        }
        this->_children.push_back(TreeToken(token));
    }
    void push(TreeToken tree) {
        if(this->_children.size() == 0) {
            this->_line = tree.line();
            this->_column = tree.column();
        }
        this->_children.push_back(tree);
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
    vector<TreeToken> children() {
        return this->_children;
    }
    ostream& print(ostream& os, int depth = 0) {
        for(int i = 0; i != depth; ++i) {
            os << "  ";
        }
        string value = this->_value;
        size_t pos = 0;
        while((pos = value.find("\n", pos)) != string::npos || (pos = value.find("\r", pos)) != string::npos) {
            value.replace(pos, 1, "↵");
            pos += 1;
        }
        if(this->_value == "")
            os  << "\e[4m"
                << this->_type << "\e[0m(\e[1;32m"
                << this->_line << ":"
                << this->_column << "\e[0m)"
                << endl;
        else
            os  << "\e[4m"
                << this->_type << "\e[0m(\e[1;32m"
                << this->_line << ":"
                << this->_column << "\e[0m \e[1;33m"
                << value << "\e[0m)"
                << endl;

        for(long unsigned int i = 0; i != this->_children.size(); ++i) {
            this->_children[i].print(os, depth + 1);
        }
        return os;
    }
    friend ostream& operator<<(ostream& os, TreeToken& tree) {
        return tree.print(os);
    }
private:
    string _type;
    string _value;
    int _line;
    int _column;
    vector<TreeToken> _children;
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
    int column = 0;
    int line = 0;

    while(code.size() > 0) {
        bool found = false;
        for(long unsigned int i = 0; i != lexemes.size(); ++i) {
            regex re(lexemes[i].value());
            smatch match;
            if(regex_search(code, match, re)) {
                if(match.position() == 0 && match.length() > 0) {
                    if(lexemes[i].type() != ".ignore") {
                        tokens.push_back(Token(lexemes[i].type(), match.str(), line, column));
                    }
                    if(match.str().find("\n") != string::npos || match.str().find("\r") != string::npos) {
                        size_t count = count_if(match.str().begin(), match.str().end(), [](char c){ return c == '\n' || c == '\r'; }) / 2;
                        line += count;
                        column = -count;
                    }
                    column += match.length();
                    code = match.suffix();
                    found = true;
                    break;
                }
            }
        }
        if(!found) {
            cerr << "ERROR" << endl;
            exit(1);
        }
    }

    return tokens;
}

void create_lexemes(vector<Lexeme> &lexemes) {
    lexemes.push_back(Lexeme("int", "([1-9][0-9]*|0)"));
    lexemes.push_back(Lexeme(".ignore", "([ \t\r\n]+)"));
}

bool _loop(bool (*f)()) { if(f()) return 1; while(it != it_e) if(f()) break; return 0; }
bool __(string val) { if(it == it_e) return 1; if(it->value() != val) return 1; it++; return 0; }

bool __int(TreeToken &tree) { if(it == it_e) return 1; if(it->type() != "int") return 1; tree.push(*it); it++; return 0; }
bool __program();

bool __program(TreeToken &master) {
    IT t = it;
    TreeToken tree;
    tree = TreeToken("program");
    if(__int(tree) || __program(tree)) it = t;
    else { master.push(tree); return 0; }
    tree = TreeToken("program");
    if(__int(tree)) it = t;
    else { master.push(tree); return 0; }
    return 1;
}

int main() {
    string code;
    code = "1 2\n\n3 4";
    vector<Token> tokens = lex(code);
    it = tokens.begin();
    it_e = tokens.end();
    TreeToken tree = TreeToken();
    
    if(__program(tree)) {
        cout << "ERROR" << endl;
    } else {
        cout << "OK" << endl;
        cout << tree.children()[0] << endl;
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


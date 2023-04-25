#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

class Lexeme {
    /**
     * @brief Lexeme class
     * @details This class is used to store the lexemes checker for the lexer
     */
public:
    Lexeme(string type, string value) {
        /**
         * @brief Construct a new Lexeme object
         * @param type
         * @param value
         * @return Lexeme
         */
        this->_type   = type;
        this->_value  = value;
    }
    string type() {
        /**
         * @brief Get the Type of the Lexeme
         * @return string
         */
        return this->_type;
    }
    regex value() {
        /**
         * @brief Get the Value of the Lexeme
         * @return regex
         */
        return this->_value;
    }
private:
    string _type; // Type of the lexeme
    regex _value; // Value of the lexeme, in the form of a regex
};

class Token {
    /**
     * @brief Token class
     * @details This class is used to store the tokens of the lexer and the tree of the parser
     */
public:
    Token() {
        /**
         * @brief Construct a new Token object
         */
        this->_type   = "";
        this->_value  = "";
        this->_line   = 0;
        this->_column = 0;
        this->_children = vector<Token>();
    }
    Token(string type) {
        /**
         * @brief Construct a new Token object
         * @param type
         * @return Token
         */
        this->_type   = type;
        this->_value  = "";
        this->_line   = 0;
        this->_column = 0;
        this->_children = vector<Token>();
    }
    Token(string type, string value) {
        /**
         * @brief Construct a new Token object
         * @param type
         * @param value
         * @return Token
         */
        this->_type   = type;
        this->_value  = value;
        this->_line   = 0;
        this->_column = 0;
        this->_children = vector<Token>();
    }
    Token(string type, string value, int line, int column) {
        /**
         * @brief Construct a new Token object
         * @param type
         * @param value
         * @param line
         * @param column
         * @return Token
         */
        this->_type   = type;
        this->_value  = value;
        this->_line   = line;
        this->_column = column;
        this->_children = vector<Token>();
    }
    void push(Token tree) {
        /**
         * @brief Push a new child to the tree
         * @param tree
         */
        if(this->_children.size() == 0) {
            this->_line = tree.line();
            this->_column = tree.column();
        }
        this->_children.push_back(tree);
    }
    void push(vector<Token>& trees) {
        /**
         * @brief Push children to the tree
         * @param trees
         */
        for(long unsigned int i = 0; i != trees.size(); ++i) {
            this->push(trees[i]);
        }
    }
    string type() {
        /**
         * @brief Get the Type of the Token
         * @return string
         */
        return this->_type;
    }
    string value() {
        /**
         * @brief Get the Value of the Token
         * @return string
         */
        return this->_value;
    }
    int line() {
        /**
         * @brief Get the Line of the Token
         * @return int
         */
        return this->_line;
    }
    int column() {
        /**
         * @brief Get the Column of the Token
         * @return int
         */
        return this->_column;
    }
    vector<Token>& children() {
        /**
         * @brief Get the Children of the Token in the form of a vector
         * @return vector<Token>
         */
        return this->_children;
    }
    ostream& print(ostream& os, int depth = 0) {
        /**
         * @brief Print the Token tree
         * @param os
         * @param depth
         * @return ostream&
         */
        for(int i = 0; i != depth; ++i) {
            os << "  ";
        }
        string value = this->_value;
        size_t pos = 0;
        while((pos = value.find("\n", pos)) != string::npos || (pos = value.find("\r", pos)) != string::npos) {
            value.replace(pos, 1, "↵");
            pos += 1;
        } // Replace newlines values with ↵
        if(this->_value == "")
            os  << "\e[4m" // 4 = underline
                << this->_type << "\e[0m(\e[1;32m" // 1 = bold, 32 = green
                << this->_line << ":"
                << this->_column << "\e[0m)" // 0 = reset
                << endl;
        else
            os  << "\e[4m" // 4 = underline
                << this->_type << "\e[0m(\e[1;32m" // 1 = bold, 32 = green
                << this->_line << ":"
                << this->_column << "\e[0m \e[1;33m" // 0 = reset, 1 = bold, 33 = yellow
                << value << "\e[0m)" // 0 = reset
                << endl;

        for(long unsigned int i = 0; i != this->_children.size(); ++i) {
            this->_children[i].print(os, depth + 1);
        } // Print children
        return os;
    }
    friend ostream& operator<<(ostream& os, Token& tree) {
        /**
         * @brief Print the Token tree
         * @param os
         * @param tree
         * @return ostream&
         */
        return tree.print(os);
    }
private:
    string _type; // Type of the token
    string _value; // Value of the token
    int _line; // Line of the token
    int _column; // Column of the token
    vector<Token> _children; // Children of the token
};

void create_lexemes(vector<Lexeme> &lexemes);

vector<Token> lex(string code) {
    /**
     * @brief Lex the code
     * @param code
     * @return vector<Token>
     */
    vector<Lexeme> lexemes;
    create_lexemes(lexemes);

    vector<Token> tokens = vector<Token>(); // Vector of tokens
    int column = 0;
    int line = 0;

    while(code.size() > 0) {
        bool found = false;
        for(long unsigned int i = 0; i != lexemes.size(); ++i) {
            regex re(lexemes[i].value()); // Regex for the current lexeme
            smatch match; // Match for the current lexeme
            if(regex_search(code, match, re)) {
                if(match.position() == 0 && match.length() > 0) { // If the match is at the good position
                    if(lexemes[i].type() != ".ignore") { // If the lexeme is not to be ignored
                        tokens.push_back(Token(lexemes[i].type(), match.str(), line, column));
                    }
                    if(match.str().find("\n") != string::npos || match.str().find("\r") != string::npos) {
                        // Increment line and reset column
                        size_t count = count_if(
                            match.str().begin(),
                            match.str().end(),
                            [](char c){ return c == '\n' || c == '\r'; }
                        ) / 2;
                        line += count;
                        column = -count;
                    }
                    column += match.length(); // Increment column
                    code = match.suffix(); // Remove the matched part from the code
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

using IT = vector<Token>::iterator; // Iterator type

IT it; // Iterator for the current tokens checked by the parser
IT it_e; // Iterator for the end of the tokens
Token * p_err; // Pointer to the badd token for error handling

template<typename T> T& next(vector<T> &v) {
    /**
     * @brief Get the next element of the vector
     * @param v
     * @return T&
     */
    v.push_back(T());
    return v.back();
}

template<typename T> ostream& operator<<(ostream &os, vector<T> &v) {
    /**
     * @brief Print a vector
     * @param os
     * @param v
     * @return ostream&
     */
    for(auto &e : v) {
        os << e;
    }
    return os;
}

template<typename T> void operator+=(vector<T> &v, vector<T> &v2) {
    /**
     * @brief Concatenate two vectors
     * @details The first vector is modified
     * @param v
     * @param v2
     */
    v.insert(v.end(), v2.begin(), v2.end());
}

bool _loop(bool (*f)(Token &), vector<Token> &master) {
    /**
     * @brief Loop the expression contained in the curly brackets
     * @param f
     * @param master
     * @return bool
     */
    Token c;
    if(f(c)) {
        p_err = &(*it);
        return 1;
    }
    master += c.children();
    while(it != it_e) {
        if(f(c)) break;
        master += c.children();
    }
    return 0;
}

bool _value(string val, Token &master) {
    /**
     * @brief Check if the current token has the right value
     * @param val
     * @param master
     * @return bool
     */
    if(it == it_e) {
        p_err = &(*it);
        return 1;
    }
    if(it->value() != val) {
        p_err = &(*it);
        return 1;
    }
    master = *it;
    it++;
    return 0;
}

bool _type(string val, Token &master) {
    /**
     * @brief Check if the current token has the right type
     * @param val
     * @param master
     * @return bool
     */
    if(it == it_e) {
        p_err = &(*it);
        return 1;
    }
    if(it->type() != val) {
        p_err = &(*it);
        return 1;
    }
    master = *it;
    it++;
    return 0;
}

void create_lexemes(vector<Lexeme> &lexemes) {
    lexemes.emplace_back("int", "([1-9][0-9]*|0)");
    lexemes.emplace_back("id", "([a-zA-Z_][a-zA-Z0-9_]*)");
    lexemes.emplace_back("op", "([+\\-\\*/])");
    lexemes.emplace_back("eq", "(=)");
    lexemes.emplace_back(".ignore", "([ \t\r\n]+)");
}

bool __int(Token &master) { return _type("int", master); }
bool __id(Token &master) { return _type("id", master); }
bool __op(Token &master) { return _type("op", master); }
bool __eq(Token &master) { return _type("eq", master); }

bool _0_program(Token &master);
bool __program(Token &master);
bool _0_expr(Token &master);
bool __expr(Token &master);
bool __term(Token &master);

bool _0_program(Token &master) {
    IT t = it;
    master = Token("._0_program");
    vector<Token> current;
    current.clear();
    if(__expr(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}

bool __program(Token &master) {
    IT t = it;
    master = Token("program");
    vector<Token> current;
    current.clear();
    if(_loop(_0_program, current)) it = t;
    else {master.children() += current; return 0;};
    return 1;
}

bool _0_expr(Token &master) {
    IT t = it;
    master = Token("._0_expr");
    vector<Token> current;
    current.clear();
    if(__op(next(current)) || __term(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}

bool __expr(Token &master) {
    IT t = it;
    master = Token("expr");
    vector<Token> current;
    current.clear();
    if(__id(next(current)) || _value("=", next(current)) || __expr(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(__term(next(current)) || _loop(_0_expr, current)) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(__term(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}

bool __term(Token &master) {
    IT t = it;
    master = Token("term");
    vector<Token> current;
    current.clear();
    if(__id(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(__int(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}

int main() {
    string code;
    code = "a = 1 b = a + 1 c = b + 1";
    vector<Token> tokens = lex(code);
    cout << tokens << endl;
    it = tokens.begin();
    it_e = tokens.end(); // end
    p_err = nullptr; // error
    Token tree;
    
    if(__program(tree)) {
        cout << "ERROR" << endl;
        cout << *p_err << endl;
    } else {
        cout << "OK" << endl;
        cout << tree << endl;
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


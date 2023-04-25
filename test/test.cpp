#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <exception>

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

template<typename T>
void operator+=(vector<T> &v, vector<T> &v2) {
    /**
     * @brief Concatenate two vectors
     * @details The first vector is modified
     * @param v
     * @param v2
     */
    v.insert(v.end(), v2.begin(), v2.end());
}

class Token {
    /**
     * @brief Token class
     * @details This class is used to store the tokens of the lexer and the tree of the parser
     */
public:
    Token(string type = "", string value = "", int line = 0, int column = 0) {
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

    void add_children(vector<Token> & v) {
        this->_children += v;
    }

    vector<Token> & children(){
        /**
         * @brief Get the Children of the Token in the form of a vector
         * @return vector<Token>
         */
        return this->_children;
    }

    void clear() {
        /**
         * @brief Clear the Token tree
         */
        this->_children.clear();
        this->_type = "";
        this->_value = "";
        this->_line = 0;
        this->_column = 0;
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

    friend ostream& operator<<(ostream& os, Token && tree) {
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
        for(Lexeme lexeme : lexemes) {
            regex re(lexeme.value()); // Regex for the current lexeme
            smatch match; // Match for the current lexeme
            if(regex_search(code, match, re)) {
                if(match.position() == 0 && match.length() > 0) { // If the match is at the good position
                    if(lexeme.type() != ".ignore") { // If the lexeme is not to be ignored
                        tokens.push_back(Token(lexeme.type(), match.str(), line, column));
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
            cerr << "No matching pattern :\n" << code << endl;
            exit(1);
        }
    }

    return tokens;
}

template<typename T> 
ostream& operator<<(ostream &os, vector<T> &v) {
    /**
     * @brief Print a vector
     * @param os
     * @param v
     * @return ostream&
     */
    for(T &e : v)
        os << e;
    return os;
}

void create_lexemes(vector<Lexeme> &lexemes) {
    lexemes.emplace_back("int", "([1-9][0-9]*|0)");
    lexemes.emplace_back("id", "([a-zA-Z_][a-zA-Z0-9_]*)");
    lexemes.emplace_back("op", "([+\\-\\*/])");
    lexemes.emplace_back("eq", "(=)");
    lexemes.emplace_back(".ignore", "([ \t\r\n]+)");
}

class syntax_error : runtime_error{
public:
    syntax_error(vector<Token>::iterator t) : runtime_error("syntax_error"), error_token(&(*t)){
    }
    Token * get_error_token(){
        return error_token;
    }
private:
    Token * error_token;
};

Token & operator<<(Token& tkn, const Token& child){
    tkn.children().push_back(child);
    return tkn;
}

Token & operator<<(Token& tkn, vector<Token> children){
    for(Token &child : children){
        tkn.children().push_back(child);
    }
    return tkn;
}

class Parser {
public:
    Parser(vector<Token> tokens):p_err(nullptr){
        it = tokens.begin();
        it_e = tokens.end();
    }

    bool parse(){
        try {
            pop__program();
        } catch (syntax_error &e){
            e.get_error_token();
            return 1;
        }
        return 0;
    }
    Token getResult(){
        return result;
    }

    Token * getErrorToken(){
        return p_err;
    }
private:
    using IT = vector<Token>::iterator; // Iterator type

    IT it; // Iterator for the current tokens checked by the parser
    IT it_e; // Iterator for the end of the tokens
    Token * p_err; // Pointer to the bad token for error handling
    Token result;

    typedef Token (Parser::*pop_t)(void); // Type for the pop functions
    vector<Token> pop_while(pop_t pop) {
        /**
         * @brief Loop the expression contained in the curly brackets
         * @param pop
         * @return bool
         */
        
        vector<Token> master{(this->*pop)()};
        while(it != it_e) {
            try{
                master.push_back((this->*pop)());
            } catch (syntax_error & e){
                break;
            }
        }
        return master;
    }

    Token pop_value(string val) {
        /**
         * @brief Check if the current token has the right value
         * @param val
         * @return bool
         */
        if(it == it_e || it->value() != val) {
            throw syntax_error(it);
        }
        return *(it++);
    }

    Token pop_type(string val) {
        /**
         * @brief Check if the current token has the right type
         * @param val
         * @return bool
         */
        if(it == it_e || it->type() != val) {
            throw syntax_error(it);
        }
        return *(it++);
    }

    Token pop__int() { return pop_type("int"); }
    Token pop__id() { return pop_type("id"); }
    Token pop__op() { return pop_type("op"); }
    Token pop__eq() { return pop_type("eq"); }

    Token pop_0_program();
    Token pop__program();
    Token pop__expr();
    Token pop__term();
};

Token Parser::pop_0_program() {
    IT t = it;
    Token master(".0_program");
    try { return master << pop__expr(); }
    catch(syntax_error &e) { master.clear(); }
    throw syntax_error(t);
}

Token Parser::pop__program() {
    IT t = it;
    Token master("program");
    try { return master << pop_while(&Parser::pop_0_program); }
    catch(syntax_error &e) { master.clear(); }
    throw syntax_error(t);
}

Token Parser::pop__expr() {
    IT t = it;
    Token master("expr");
    try { return master << pop__id() << pop_value("=") << pop__expr(); }
    catch(syntax_error &e){ master.clear(); }
    try { return master << pop__term() << pop__op() << pop__term(); }
    catch(syntax_error &e) { master.clear(); }
    throw syntax_error(t);
}

Token Parser::pop__term() {
    IT t = it;
    Token master("expr");
    try { return master << pop__id(); }
    catch(syntax_error &e){ master.clear(); }
    try { return master << pop__int(); }
    catch(syntax_error &e) { master.clear(); }
    throw syntax_error(t);
}

int main() {
    string code;
    code = "a = 1 b = a + 1 c = b + 1";
    vector<Token> tokens = lex(code);
    cout << tokens << endl;
    Parser p(tokens);
    
    if(p.parse()) {
        cout << "ERROR" << endl;
        cout << *p.getErrorToken() << endl;
    } else {
        cout << "OK" << endl;
        cout << p.getResult() << endl;
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


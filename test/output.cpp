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

    vector<Token> tokens{}; // Vector of tokens
    int column = 0;
    int line = 0;
    // Try to split the code in lexeme by removing the first one each time
    while(code.size() > 0) {
        // Try to match every lexeme with the beginning of the remaining code
        // If no lexeme match, we raise an error 
        bool found = false;
        for(Lexeme lexeme : lexemes) {
            const regex current_regex(lexeme.value()); // Regex for the current lexeme
            smatch match; 
            // Try to match current regex at the beginning of the remaining code
            if( regex_search(code, match, current_regex, 
                regex_constants::match_continuous | regex_constants::match_not_null) ) {

                // If the lexeme is not to be ignored
                if(lexeme.type() != ".ignore") { 
                    tokens.push_back(Token(lexeme.type(), match.str(), line, column));
                }

                // Update line and column
                for (const char& c : match.str()){
                    if(c == '\r' || c == '\n'){
                        line++;
                        column = 0;
                    } else{
                        column++;
                    }
                }
                // Remove the matched part from the code
                code = match.suffix(); 
                found = true;
                break;
            }
        }
        if(!found) {
            throw runtime_error("No matching pattern :\n" + code) ;
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
    v.emplace_back();
    return v.back();
}

template<typename T> 
ostream& operator<<(ostream &os, vector<T> &v) {
    /**
     * @brief Print a vector
     * @param os
     * @param v
     * @return ostream&
     */
    for(T &e : v) {
        os << e;
    }
    return os;
}

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

bool pop_while(bool (*f)(Token &), vector<Token> &master) {
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

bool pop_value(string val, Token &master) {
    /**
     * @brief Check if the current token has the right value
     * @param val
     * @param master
     * @return bool
     */
    if(it == it_e || it->value() != val) {
        p_err = &(*it);
        return 1;
    }
    master = *it;
    it++;
    return 0;
}

bool pop_type(string val, Token &master) {
    /**
     * @brief Check if the current token has the right type
     * @param val
     * @param master
     * @return bool
     */
    if(it == it_e || it->type() != val) {
        p_err = &(*it);
        return 1;
    }
    master = *it;
    it++;
    return 0;
}

void create_lexemes(vector<Lexeme> &lexemes) {
    lexemes.emplace_back("int", "([1-9][0-9]*|0)");
    lexemes.emplace_back("float", "([0-9]+\\.[0-9]+)");
    lexemes.emplace_back("string", "(\"[^\"]*\")");
    lexemes.emplace_back("char", "('[^']')");
    lexemes.emplace_back("id", "([a-zA-Z_][a-zA-Z0-9_]*)");
    lexemes.emplace_back("operator", "([\\+\\-\\*/%&|^<>!$=\\.]+)");
    lexemes.emplace_back("bracket", "(\\(|\\)|\\[|\\])");
    lexemes.emplace_back("comma", "(,|\\|)");
    lexemes.emplace_back("type_checker_id", "([A-Z][a-zA-Z0-9_]*)");
    lexemes.emplace_back(".ignore", "([ \t\r\n]+)");
}

bool pop_int(Token &master) { return pop_type("int", master); }
bool pop_float(Token &master) { return pop_type("float", master); }
bool pop_string(Token &master) { return pop_type("string", master); }
bool pop_char(Token &master) { return pop_type("char", master); }
bool pop_id(Token &master) { return pop_type("id", master); }
bool pop_operator(Token &master) { return pop_type("operator", master); }
bool pop_bracket(Token &master) { return pop_type("bracket", master); }
bool pop_comma(Token &master) { return pop_type("comma", master); }
bool pop_type_checker_id(Token &master) { return pop_type("type_checker_id", master); }

bool pop_program(Token &master);
bool pop_instructions(Token &master);
bool pop_expr(Token &master);
bool pop_casted(Token &master);
bool pop_name(Token &master);
bool pop_fun(Token &master);
bool pop_comprehensive_list(Token &master);
bool pop_structure_if(Token &master);
bool pop_structure_while(Token &master);
bool pop_structure_loop(Token &master);
bool pop_structure_for(Token &master);
bool pop_list(Token &master);
bool pop_structure_lambda(Token &master);
bool pop_structure_function(Token &master);
bool pop_type_checker(Token &master);
bool pop_structure_typed_lambda(Token &master);
bool pop_structure_typed_function(Token &master);
bool pop_function_call(Token &master);
bool pop_structure(Token &master);
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : program
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_0_program(Token &master) {
    IT t = it;
    master = Token( "0_program" );
    vector<Token> current;
    current.clear();
    if(pop_expr(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_fun(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}

bool pop_program(Token &master) {
    IT t = it;
    master = Token( "program" );
    vector<Token> current;
    current.clear();
    if(pop_while( pop_0_program, current)) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : instructions
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_0_instructions(Token &master) {
    IT t = it;
    master = Token( "0_instructions" );
    vector<Token> current;
    current.clear();
    if(pop_expr(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_fun(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}

bool pop_instructions(Token &master) {
    IT t = it;
    master = Token( "instructions" );
    vector<Token> current;
    current.clear();
    if(pop_value("(", next(current)) || pop_while( pop_0_instructions, current) || pop_value(")", next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_fun(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : expr
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_0_expr(Token &master) {
    IT t = it;
    master = Token( "0_expr" );
    vector<Token> current;
    current.clear();
    if(pop_operator(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}

bool pop_expr(Token &master) {
    IT t = it;
    master = Token( "expr" );
    vector<Token> current;
    current.clear();
    if(pop_casted(next(current)) || pop_while( pop_0_expr, current) || pop_expr(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_casted(next(current)) || pop_while( pop_0_expr, current) || pop_operator(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_casted(next(current)) || pop_while( pop_0_expr, current) || pop_fun(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_casted(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : casted
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_casted(Token &master) {
    IT t = it;
    master = Token( "casted" );
    vector<Token> current;
    current.clear();
    if(pop_int(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_float(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_string(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_char(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_id(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_instructions(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_structure(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_list(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_comprehensive_list(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : name
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_name(Token &master) {
    IT t = it;
    master = Token( "name" );
    vector<Token> current;
    current.clear();
    if(pop_id(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_value("operator", next(current)) || pop_operator(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : fun
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_0_fun(Token &master) {
    IT t = it;
    master = Token( "0_fun" );
    vector<Token> current;
    current.clear();
    if(pop_expr(next(current)) || pop_value(",", next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}

bool pop_fun(Token &master) {
    IT t = it;
    master = Token( "fun" );
    vector<Token> current;
    current.clear();
    if(pop_name(next(current)) || pop_while( pop_0_fun, current) || pop_expr(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_name(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_name(next(current)) || pop_expr(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_name(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : comprehensive_list
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_comprehensive_list(Token &master) {
    IT t = it;
    master = Token( "comprehensive_list" );
    vector<Token> current;
    current.clear();
    if(pop_value("[", next(current)) || pop_expr(next(current)) || pop_value("|", next(current)) || pop_id(next(current)) || pop_value("in", next(current)) || pop_expr(next(current)) || pop_value("|", next(current)) || pop_expr(next(current)) || pop_value("|", next(current)) || pop_expr(next(current)) || pop_value("]", next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_value("[", next(current)) || pop_expr(next(current)) || pop_value("|", next(current)) || pop_id(next(current)) || pop_value("in", next(current)) || pop_expr(next(current)) || pop_value("]", next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_value("[", next(current)) || pop_expr(next(current)) || pop_value("|", next(current)) || pop_id(next(current)) || pop_value("in", next(current)) || pop_expr(next(current)) || pop_value("|", next(current)) || pop_expr(next(current)) || pop_value("]", next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_value("[", next(current)) || pop_expr(next(current)) || pop_value("|", next(current)) || pop_id(next(current)) || pop_value("in", next(current)) || pop_expr(next(current)) || pop_value("]", next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : structure_if
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_structure_if(Token &master) {
    IT t = it;
    master = Token( "structure_if" );
    vector<Token> current;
    current.clear();
    if(pop_value("if", next(current)) || pop_expr(next(current)) || pop_instructions(next(current)) || pop_value("else", next(current)) || pop_instructions(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_value("if", next(current)) || pop_expr(next(current)) || pop_instructions(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : structure_while
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_structure_while(Token &master) {
    IT t = it;
    master = Token( "structure_while" );
    vector<Token> current;
    current.clear();
    if(pop_value("while", next(current)) || pop_expr(next(current)) || pop_instructions(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : structure_loop
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_structure_loop(Token &master) {
    IT t = it;
    master = Token( "structure_loop" );
    vector<Token> current;
    current.clear();
    if(pop_value("loop", next(current)) || pop_instructions(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : structure_for
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_structure_for(Token &master) {
    IT t = it;
    master = Token( "structure_for" );
    vector<Token> current;
    current.clear();
    if(pop_value("for", next(current)) || pop_id(next(current)) || pop_value("in", next(current)) || pop_expr(next(current)) || pop_instructions(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : list
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_0_list(Token &master) {
    IT t = it;
    master = Token( "0_list" );
    vector<Token> current;
    current.clear();
    if(pop_expr(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}

bool pop_list(Token &master) {
    IT t = it;
    master = Token( "list" );
    vector<Token> current;
    current.clear();
    if(pop_value("[", next(current)) || pop_while( pop_0_list, current) || pop_value("]", next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_value("[", next(current)) || pop_value("]", next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : structure_lambda
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_0_structure_lambda(Token &master) {
    IT t = it;
    master = Token( "0_structure_lambda" );
    vector<Token> current;
    current.clear();
    if(pop_value("&", next(current)) || pop_name(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_name(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}

bool pop_structure_lambda(Token &master) {
    IT t = it;
    master = Token( "structure_lambda" );
    vector<Token> current;
    current.clear();
    if(pop_while( pop_0_structure_lambda, current) || pop_value("=>", next(current)) || pop_instructions(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_value("=>", next(current)) || pop_instructions(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : structure_function
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_structure_function(Token &master) {
    IT t = it;
    master = Token( "structure_function" );
    vector<Token> current;
    current.clear();
    if(pop_name(next(current)) || pop_value("=", next(current)) || pop_structure_lambda(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : type_checker
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_0_type_checker(Token &master) {
    IT t = it;
    master = Token( "0_type_checker" );
    vector<Token> current;
    current.clear();
    if(pop_type_checker(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_value("(", next(current)) || pop_expr(next(current)) || pop_value(")", next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}

bool pop_type_checker(Token &master) {
    IT t = it;
    master = Token( "type_checker" );
    vector<Token> current;
    current.clear();
    if(pop_type_checker_id(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_type_checker_id(next(current)) || pop_value("[", next(current)) || pop_while( pop_0_type_checker, current) || pop_value("]", next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : structure_typed_lambda
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_0_structure_typed_lambda(Token &master) {
    IT t = it;
    master = Token( "0_structure_typed_lambda" );
    vector<Token> current;
    current.clear();
    if(pop_value("&", next(current)) || pop_name(next(current)) || pop_value(":", next(current)) || pop_type_checker(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_name(next(current)) || pop_value(":", next(current)) || pop_type_checker(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}

bool pop_structure_typed_lambda(Token &master) {
    IT t = it;
    master = Token( "structure_typed_lambda" );
    vector<Token> current;
    current.clear();
    if(pop_while( pop_0_structure_typed_lambda, current) || pop_value("=>", next(current)) || pop_instructions(next(current))) it = t;
    else {master.children() += current; return 0;};
    current.clear();
    if(pop_value("=>", next(current)) || pop_instructions(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : structure_typed_function
//////////////////////////////////////////////////////////////////////////////////////////////////

bool pop_structure_typed_function(Token &master) {
    IT t = it;
    master = Token( "structure_typed_function" );
    vector<Token> current;
    current.clear();
    if(pop_name(next(current)) || pop_value("<-", next(current)) || pop_structure_typed_lambda(next(current))) it = t;
    else {master.children() += current; return 0;};
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//                       Functions to pop tokens of type : function_call
//////////////////////////////////////////////////////////////////////////////////////////////////

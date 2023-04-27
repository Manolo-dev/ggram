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

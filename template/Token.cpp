class Token {
    /**
     * @brief Token class
     * @details This class is used to store the tokens of the lexer and the tree of the parser
     */
public:
    Token(string type = "", string value = "", int line = 0, int column = 0):
        _type(type),
        _value(value),
        _line(line),
        _column(column),
        _children()
    {
        /**
         * @brief Construct a new Token object
         * @param type
         * @param value
         * @param line
         * @param column
         * @return Token
         */
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

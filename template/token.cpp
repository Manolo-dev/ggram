/**
 * @brief Construct a new Token object
 * @param type
 * @param value
 * @param line
 * @param column
 * @return Token
 */
Token::Token(std::string type, std::string value, int line, int column, bool error)
    : _type(type), _value(value), _line(line), _column(column), _children(), _is_error(error) {}
Token::Token(std::string type, bool error) : Token(type, "", 0, 0, error) {}

void Token::make_error() {
    _is_error = true;
}

bool Token::is_error() const {
    return _is_error;
}

void Token::clear() {
    _children.clear();
    _is_error = false;
}
Token Token::make_error_copy() {
    Token res(_type, _value, _line, _column);
    res.make_error();
    return res;
}
/**
 * @brief Push a new child to the tree
 * @param tree
 */
void Token::push(Token tree) {
    if (this->_children.size() == 0) {
        this->_line = tree.line();
        this->_column = tree.column();
    }
    this->_children.push_back(tree);
}

/**
 * @brief Push children to the tree
 * @param trees
 */
void Token::push(std::vector<Token> &trees) {
    for (long unsigned int i = 0; i != trees.size(); ++i) {
        this->push(trees[i]);
    }
}

/**
 * @brief Get the Type of the Token
 * @return std::string
 */
std::string Token::type() const {
    return this->_type;
}
/**
 * @brief Get the Value of the Token
 * @return std::string
 */
std::string Token::value() const {
    return this->_value;
}
/**
 * @brief Get the Line of the Token
 * @return int
 */
int Token::line() const {
    return this->_line;
}
/**
 * @brief Get the Column of the Token
 * @return int
 */
int Token::column() const {
    return this->_column;
}
/**
 * @brief Get the Children of the Token in the form of a std::vector
 * @return std::vector<Token>
 */
std::vector<Token> &Token::children() {
    return this->_children;
}
/**
 * @brief Get the Children of the Token in the form of a std::vector
 * @return std::vector<Token>
 */
const std::vector<Token> &Token::children() const {
    return this->_children;
}
/**
 * @brief Print the Token tree
 * @param os
 * @param depth
 * @return std::ostream&
 */
std::ostream &Token::print(std::ostream &os, int depth) const {
    for (int i = 0; i != depth; ++i) {
        os << "  ";
    }
    std::string value = this->_value;
    size_t pos = 0;
    while ((pos = value.find("\n", pos)) != std::string::npos ||
           (pos = value.find("\r", pos)) != std::string::npos) {
        value.replace(pos, 1, "↵");
        pos += 1;
    } // Replace newlines values with ↵
    if (this->_value == "")
        os << "\e[4m"                                         // 4 = underline
           << this->_type << "\e[0m(\e[1;32m"                 // 1 = bold, 32 = green
           << this->_line << ":" << this->_column << "\e[0m)" // 0 = reset
           << std::endl;
    else
        os << "\e[4m"                         // 4 = underline
           << this->_type << "\e[0m(\e[1;32m" // 1 = bold, 32 = green
           << this->_line << ":" << this->_column
           << "\e[0m \e[1;33m"  // 0 = reset, 1 = bold, 33 = yellow
           << value << "\e[0m)" // 0 = reset
           << std::endl;

    for (long unsigned int i = 0; i != this->_children.size(); ++i) {
        this->_children[i].print(os, depth + 1);
    } // Print children
    return os;
}

/**
 * @brief Print the Token tree
 * @param os
 * @param tree
 * @return std::ostream&
 */
std::ostream &operator<<(std::ostream &os, const Token &tree) {
    return tree.print(os);
}

/**
 * @brief Print a std::vector
 * @param os
 * @param v
 * @return std::ostream&
 */
template<typename T> std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
    for (const T &e : v) {
        os << e;
    }
    return os;
}

/**
 * @brief Concatenate two std::vectors
 * @details The first std::vector is modified
 * @param v
 * @param v2
 */
template<typename T> void operator+=(std::vector<T> &v, const std::vector<T> &v2) {
    v.insert(v.end(), v2.begin(), v2.end());
}
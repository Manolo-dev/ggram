
IT it; // Iterator for the current tokens checked by the parser
IT it_end; // Iterator for the end of the tokens
IT it_err; // Pointer to the bad token for error handling

template<typename T> T& createNext(vector<T> &v) {
    /**
     * @brief Create a new element at the end of the vector and return a reference to it
     * @param v
     * @return T&
     */
    v.emplace_back();
    return v.back();
}

bool _pop_while(bool (*f)(Token &), vector<Token> &master) {
    /**
     * @brief Loop the expression contained in the curly brackets
     * @param f
     * @param master
     * @return bool
     */
    Token c;
    if(f(c)) {
        it_err = it;
        return 1;
    }
    master += c.children();
    while(it != it_end) {
        if(f(c)) break;
        master += c.children();
    }
    return 0;
}

bool _pop_value(const string& val, Token &master) {
    /**
     * @brief Check if the current token has the right value
     * @param val
     * @param master
     * @return bool
     */
    if(it == it_end || it->value() != val) {
        it_err = it;
        return 1;
    }
    master = *it;
    it++;
    return 0;
}

bool _pop_type(const string& val, Token &master) {
    /**
     * @brief Check if the current token has the right type
     * @param val
     * @param master
     * @return bool
     */
    if(it == it_end || it->type() != val) {
        it_err = it;
        return 1;
    }
    master = *it;
    it++;
    return 0;
}

Token parse(vector<Token>& tokens){
    it = tokens.begin();
    it_end = tokens.end();
    Token tree;
    if(pop_program(tree)){
        throw syntax_error(it_err);
    }
    if(it != it_end){
        throw syntax_error(it);
    }
    return tree;
}

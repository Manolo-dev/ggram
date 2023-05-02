
IT it_cur; // Iterator for the current tokens checked by the parser
IT it_end; // Iterator for the end of the tokens
IT it_err; // Iterator for the bad token for error handling

template<typename T> T& createNext(vector<T> &v) {
    /**
     * @brief Create a new element at the end of the vector and return a reference to it_cur
     * @param v
     * @return T&
     */
    v.emplace_back();
    return v.back();
}
typedef bool (*popfunction)(Token &);
bool _pop_while(popfunction pop, vector<Token> &master) {
    /**
     * @brief Loop the expression contained in the curly brackets
     * @param f
     * @param master
     * @return bool
     */
    SEARCH("while")
    Token c;
    if(pop(c)) {
        FAILED("while")
        it_err = it_cur;
        return 1;
    }
    master += c.children();
    while(it_cur != it_end) {
        LOOP
        if(pop(c)) break;
        master += c.children();
    }
    FOUND("while")
    return 0;
}

bool _pop_value(const string& val, Token &master) {
    /**
     * @brief Check if the current token has the right value
     * @param val
     * @param master
     * @return bool
     */
    SEARCH("value :" << val)
    if(it_cur == it_end || it_cur->value() != val) {
        FAILED("value: " << (it_cur == it_end ? "EOF" : it_cur->value()) ) 
        it_err = it_cur;
        return 1;
    }
    master = *it_cur;
    it_cur++;
    FOUND("value")
    return 0;
}

bool _pop_type(const string& val, Token &master) {
    /**
     * @brief Check if the current token has the right type
     * @param val
     * @param master
     * @return bool
     */
    SEARCH( "type : " << val)
    if(it_cur == it_end || it_cur->type() != val) {
        FAILED("type: " << (it_cur == it_end ? "EOF" : it_cur->type()) ) 
        it_err = it_cur;
        return 1;
    }
    master = *it_cur;
    it_cur++;
    FOUND("type")
    return 0;
}

Token parse(vector<Token>& tokens){
    it_cur = tokens.begin();
    it_end = tokens.end();
    Token tree;
    if(pop_program(tree)){
        throw syntax_error(it_err);
    }
    if(it_cur != it_end){
        throw syntax_error(it_cur);
    }
    return tree;
}

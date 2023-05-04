Token& operator<<(Token& tkn, const Token& new_child){
    tkn.children().push_back(new_child);
    return tkn;
}
Token& operator<<(Token& tkn, const vector<Token>& new_children){
    tkn.children() += new_children;
    return tkn;
}

typedef Token (*popfunction)(IT&, const IT);
vector<Token> _pop_while(const popfunction pop, IT& it_cur, const IT it_end) {
    /**
     * @brief Loop the expression contained in the curly brackets
     * @param f
     * @param master
     * @return bool
     */
    vector<Token> master = pop(it_cur,it_end).children();
    while(it_cur != it_end) {
        try{
            master += pop(it_cur, it_end).children();
        } catch(syntax_error&){
            break;
        }
    }
    return master;
}

Token& _pop_value(const string& val, IT& it_cur, const IT it_end) {
    /**
     * @brief Check if the current token has the right value
     * @param val
     * @param master
     * @return bool
     */
    SEARCH("value :" << val)
    if(it_cur == it_end || it_cur->value() != val) {
        FAILED("value: " << (it_cur == it_end ? "EOF" : it_cur->value())(
        throw syntax_error(it_cur);
    }
    FOUND("value")
    return *(it_cur++);
}

Token& _pop_type(const string& val, IT& it_cur, const IT it_end) {
    /**
     * @brief Check if the current token has the right type
     * @param val
     * @param master
     * @return bool
     */
    SEARCH("type : " << val)
    if(it_cur == it_end || it_cur->type() != val) {
        FAILED("type: " << (it_cur == it_end ? "EOF" : it_cur->type())(
        throw syntax_error(it_cur);
    }
    FOUND("type")
    return *(it_cur++);
}

Token parse(vector<Token>& v){
    IT it = v.begin();
    Token result = pop_program(it, v.end());
    if(it != v.end()){
        throw syntax_error(it);
    }
    return result;
}

/***************************************************************************************************************************/
/***************************************************************************************************************************/
/**************************************                                             ****************************************/
/**************************************       Automatically generated content       ****************************************/
/**************************************                                             ****************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
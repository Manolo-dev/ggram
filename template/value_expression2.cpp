Token& operator<<(Token& tkn, const Token& new_child){
    tkn.children().push_back(new_child);
    return tkn;
}
Token& operator<<(Token& tkn, const std::vector<Token>& new_children){
    tkn.children() += new_children;
    return tkn;
}

/**
 * @brief Loop the expression contained in the curly brackets
 * @param f
 * @param master
 * @return bool
 */
typedef Token (*popfunction)(IT&, const IT);
std::vector<Token> _pop_while(const popfunction pop, IT& it_cur, const IT it_end) {
    std::vector<Token> master = pop(it_cur,it_end).children();
    while(it_cur != it_end) {
        try{
            master += pop(it_cur, it_end).children();
        } catch(syntax_error&){
            break;
        }
    }
    return master;
}

/**
 * @brief Check if the current token has the right value
 * @param val
 * @param master
 * @return bool
 */
Token& _pop_value(const std::string& val, IT& it_cur, const IT it_end) {
    SEARCH("value :" << val)
    if(it_cur == it_end || it_cur->value() != val) {
        FAILED("value: " << (it_cur == it_end ? "EOF" : it_cur->value()))
        throw syntax_error(it_cur);
    }
    FOUND("value")
    return *(it_cur++);
}

/**
 * @brief Check if the current token has the right type
 * @param val
 * @param master
 * @return bool
 */
Token& _pop_type(const std::string& val, IT& it_cur, const IT it_end) {
    SEARCH("type : " << val)
    if(it_cur == it_end || it_cur->type() != val) {
        FAILED("type: " << (it_cur == it_end ? "EOF" : it_cur->type()))
        throw syntax_error(it_cur);
    }
    FOUND("type")
    return *(it_cur++);
}

Token parse(std::vector<Token>& v){
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
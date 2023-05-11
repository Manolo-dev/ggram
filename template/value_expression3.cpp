Token& operator<<(Token& tkn, const Token& new_child){
    if(tkn.is_error()){
        return tkn;
    }
    if(new_child.is_error()){
        tkn = new_child;
        return tkn;
    }
    tkn.children().push_back(new_child);
    return tkn;
}
Token& operator<<(Token& tkn, const std::vector<Token>& new_children){
    if(tkn.is_error()){
        return tkn;
    }
    if(new_children[0].is_error()){
        tkn = new_children[0];
        return tkn;
    }
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
    SEARCH("while")
    Token master = pop(it_cur,it_end);
    if(master.is_error()){
        FAILED("while")
        return {master};
    }
    while(it_cur != it_end) {
        LOOP
        const Token res = pop(it_cur, it_end);
        if(res.is_error()){
            break;
        }
        master << res.children();
    }
    FOUND("while")
    return master.children();
}

/**
 * @brief Check if the current token has the right value
 * @param val
 * @param master
 * @return bool
 */
Token _pop_value(const std::string& val, IT& it_cur, const IT it_end) {
    SEARCH("value :" << val)
    if(it_cur == it_end){
        FAILED("value: EOF") 
        return Token("EOF", true);
    }
    if(it_cur->value() != val) {
        FAILED("value: " << it_cur->value()) 
        return it_cur -> make_error_copy();
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
Token _pop_type(const std::string& val, IT& it_cur, const IT it_end) {
    SEARCH("type : " << val)
    if(it_cur == it_end){
        FAILED("type: EOF") 
        return Token("EOF", true);
    }
    if(it_cur->type() != val) {
        FAILED("type: " << it_cur->type()) 
        return it_cur -> make_error_copy();
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
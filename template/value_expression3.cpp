Token& operator<<(Token& tkn, const Token& new_child ){
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
Token& operator<<(Token& tkn, const vector<Token>& new_children ){
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

typedef Token (*popfunction)(IT&, const IT);
vector<Token> _pop_while(const popfunction pop, IT& curr_it, const IT it_end) {
    /**
     * @brief Loop the expression contained in the curly brackets
     * @param f
     * @param master
     * @return bool
     */
    SEARCH("while")
    Token master = pop(curr_it,it_end);
    if(master.is_error()){
        FAILED("while")
        return {master};
    }
    while(curr_it != it_end) {
        LOOP
        const Token res = pop(curr_it, it_end);
        if(res.is_error()){
            break;
        }
        master << res.children();
    }
    FOUND("while")
    return master.children();
}

Token _pop_value(const string& val, IT& curr_it, const IT it_end) {
    /**
     * @brief Check if the current token has the right value
     * @param val
     * @param master
     * @return bool
     */
    SEARCH("value :" << val)
    if(curr_it == it_end || curr_it->value() != val) {
        FAILED("value: " << curr_it->value()) 
        return curr_it -> make_error_copy();
    }
    FOUND("value")
    return *(curr_it++);
}

Token _pop_type(const string& val, IT& curr_it, const IT it_end) {
    /**
     * @brief Check if the current token has the right type
     * @param val
     * @param master
     * @return bool
     */
    SEARCH( "type : " << val)
    if(curr_it == it_end || curr_it->type() != val) {
        FAILED("type: " << curr_it->type()) 
        return curr_it -> make_error_copy();
    }
    FOUND("type")
    return *(curr_it++);
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
Token& operator<<(Token& tkn, const Token& new_child ){
    tkn.children().push_back(new_child);
    return tkn;
}
Token& operator<<(Token& tkn, const vector<Token> new_children ){
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
    vector<Token> master = pop(curr_it,it_end).children();
    while(curr_it != it_end) {
        try{
            master += pop(curr_it, it_end).children();
        } catch(syntax_error&){
            break;
        }
    }
    return master;
}

Token& _pop_value(const string& val, IT& curr_it, const IT it_end) {
    /**
     * @brief Check if the current token has the right value
     * @param val
     * @param master
     * @return bool
     */

    if(curr_it == it_end || curr_it->value() != val) {
        throw syntax_error(curr_it);
    }
    return *(curr_it++);
}

Token& _pop_type(const string& val, IT& curr_it, const IT it_end) {
    /**
     * @brief Check if the current token has the right type
     * @param val
     * @param master
     * @return bool
     */
    if(curr_it == it_end || curr_it->type() != val) {
        throw syntax_error(curr_it);
    }
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
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
void operator+=(vector<T> &v, const vector<T> &v2) {
    /**
     * @brief Concatenate two vectors
     * @details The first vector is modified
     * @param v
     * @param v2
     */
    v.insert(v.end(), v2.begin(), v2.end());
}

class syntax_error : runtime_error{
public:
    syntax_error(vector<Token>::iterator start_token) : runtime_error("syntax_error"), error_token(&(*start_token)){
        // cout << "syntax_error created :" << *error_token ;
    }
    Token * get_error_token(){
        return error_token;
    }
private:
    Token * error_token;
};



Token operator<<(Token tkn, const Token new_child ){
    tkn.children().push_back(new_child);
    return tkn;
}
Token operator<<(Token tkn, const vector<Token> new_children ){
    tkn.children() += new_children;
    return tkn;
}

typedef vector<Token>::iterator IT; // Iterator type

typedef Token (*popfunction)(IT&, const IT);
vector<Token> pop_while(popfunction pop, IT& curr_it, const IT it_end) {
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
        } catch(syntax_error & e){
            break;
        }
    }
    return master;
}

Token pop_value(string val, IT& curr_it, const IT it_end) {
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

Token pop_type(string val, IT& curr_it, const IT it_end) {
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

/***************************************************************************************************************************/
/***************************************************************************************************************************/
/**************************************                                             ****************************************/
/**************************************       Automatically generated content       ****************************************/
/**************************************                                             ****************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
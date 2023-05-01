
IT it; // Iterator for the current tokens checked by the parser
IT it_e; // Iterator for the end of the tokens
Token * p_err; // Pointer to the bad token for error handling

template<typename T> T& createNext(vector<T> &v) {
    /**
     * @brief Create a new element at the end of the vector and return a reference to it
     * @param v
     * @return T&
     */
    v.emplace_back();
    return v.back();
}

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
void operator+=(vector<T> &v, vector<T> &v2) {
    /**
     * @brief Concatenate two vectors
     * @details The first vector is modified
     * @param v
     * @param v2
     */
    v.insert(v.end(), v2.begin(), v2.end());
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
        p_err = &(*it);
        return 1;
    }
    master += c.children();
    while(it != it_e) {
        if(f(c)) break;
        master += c.children();
    }
    return 0;
}

bool _pop_value(string val, Token &master) {
    /**
     * @brief Check if the current token has the right value
     * @param val
     * @param master
     * @return bool
     */
    if(it == it_e || it->value() != val) {
        p_err = &(*it);
        return 1;
    }
    master = *it;
    it++;
    return 0;
}

bool _pop_type(string val, Token &master) {
    /**
     * @brief Check if the current token has the right type
     * @param val
     * @param master
     * @return bool
     */
    if(it == it_e || it->type() != val) {
        p_err = &(*it);
        return 1;
    }
    master = *it;
    it++;
    return 0;
}

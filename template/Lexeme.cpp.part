class Lexeme {
    /**
     * @brief Lexeme class
     * @details This class is used to store the lexemes checker for the lexer
     */
public:
    Lexeme(string type, string value) {
        /**
         * @brief Construct a new Lexeme object
         * @param type
         * @param value
         * @return Lexeme
         */
        this->_type   = type;
        this->_value  = value;
    }
    string type() {
        /**
         * @brief Get the Type of the Lexeme
         * @return string
         */
        return this->_type;
    }
    regex value() {
        /**
         * @brief Get the Value of the Lexeme
         * @return regex
         */
        return this->_value;
    }
private:
    string _type; // Type of the lexeme
    regex _value; // Value of the lexeme, in the form of a regex
};
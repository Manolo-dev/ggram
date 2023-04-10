class Token {
public:
    Token(string type, string value, int line, int column) {
        this->_type   = type;
        this->_value  = value;
        this->_line   = line;
        this->_column = column;
    }
    string type() {
        return this->_type;
    }
    string value() {
        return this->_value;
    }
    int line() {
        return this->_line;
    }
    int column() {
        return this->_column;
    }
private:
    string _type;
    string _value;
    int _line;
    int _column;
};
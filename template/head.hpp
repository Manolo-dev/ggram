/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***********************************************                            ************************************************/
/***********************************************       Copied Content       ************************************************/
/***********************************************                            ************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cstring>

using namespace std;



class Token {
    /**
     * @brief Token class
     * @details This class is used to store the tokens of the lexer and the tree of the parser
     */
public:
    Token(string type = "", string value = "", int line = 0, int column = 0);

    void push(Token tree);
    void push(vector<Token>& trees);
    string type();
    string value();
    int line();
    int column();
    vector<Token>& children();
    ostream& print(ostream& os, int depth = 0) ;
    friend ostream& operator<<(ostream& os, Token& tree);
private:
    string _type; // Type of the token
    string _value; // Value of the token
    int _line; // Line of the token
    int _column; // Column of the token
    vector<Token> _children; // Children of the token
};

vector<Token> lex(string code);

struct Lexeme {
    /**
     * @brief Lexeme struct
     * @details This struct represents a lexeme: its name and its checking regex
     */
    const char *name;
    const std::regex &regex;
};

bool _pop_while(bool (*f)(Token &), vector<Token> &master);
bool _pop_value(string val, Token &master);
bool _pop_type(string val, Token &master);

typedef vector<Token>::iterator IT;
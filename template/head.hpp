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
    string type() const;
    string value() const;
    int line() const;
    int column() const;
    bool is_error() const;
    void make_error();
    vector<Token>& children();
    ostream& print(ostream& os, int depth = 0) const ;
    friend ostream& operator<<(ostream& os, const Token& tree);
private:
    bool _is_error;
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

typedef vector<Token>::iterator IT;

class syntax_error : runtime_error{
public:
    syntax_error(const Token& err_tok): runtime_error("syntax_error"), error_token(err_tok){}
    syntax_error(const IT start_token) : syntax_error(*start_token){}
    Token get_error_token() const {
        return error_token;
    }
private:
    const Token& error_token;
};

Token parse(vector<Token>& v);
Token parse(const string& code);

// #define DEBUG_MODE  // Décommenter pour voir l'arbre de recherche

#ifdef DEBUG_MODE
    unsigned int indent_lvl = 0;
    string indentation(){
        string res = "";
        for (int i = 0; i < indent_lvl; ++i)
        {
            res += "   ";
        }
        return res;
    }
    #define SEARCH(name) cout << indentation() << "search " << name << endl; indent_lvl++;
    #define FOUND(name) indent_lvl--; cout << indentation() << "FOUND "<< name << endl;
    #define FAILED(name) indent_lvl--; cout << indentation() << "Failed "<< name << endl;
    #define LOOP cout << indentation() << "LOOOP" << endl;
#else
    #define SEARCH(name)  
    #define FOUND(name)  
    #define FAILED(name)  
    #define LOOP
#endif


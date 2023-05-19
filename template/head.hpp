/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***********************************************                            ************************************************/
/***********************************************       Copied Content       ************************************************/
/***********************************************                            ************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/

#include <cstring>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

    /**
     * @brief Token class
     * @details This class is used to store the tokens of the lexer and the tree of the parser
     */
    class Token {
  public:
    Token(std::string type = "", std::string value = "", int line = 0, int column = 0,
          bool error = false);
    Token(std::string type, bool error);
    void push(Token tree);
    void push(std::vector<Token> &trees);
    std::string type() const;
    std::string value() const;
    int line() const;
    int column() const;
    bool is_error() const;
    void clear();
    void make_error();
    Token make_error_copy();
    std::vector<Token> &children();
    const std::vector<Token> &children() const;
    std::ostream &print(std::ostream &os, int depth = 0) const;
    friend std::ostream &operator<<(std::ostream &os, const Token &tree);

  private:
    std::string _type;            // Type of the token
    std::string _value;           // Value of the token
    int _line;                    // Line of the token
    int _column;                  // Column of the token
    std::vector<Token> _children; // Children of the token
    bool _is_error;
};

std::vector<Token> lex(std::string code);

struct Lexeme {
    /**
     * @brief Lexeme struct
     * @details This struct represents a lexeme: its name and its checking regex
     */
    const char *name;
    const std::regex &regex;
};

using IT = std::vector<Token>::iterator;

class syntax_error : std::runtime_error {
  public:
    syntax_error(const Token &err_tok) : runtime_error("syntax_error"), error_token(err_tok) {}
    syntax_error(const IT start_token) : syntax_error(*start_token) {}
    Token get_error_token() const {
        return error_token;
    }

  private:
    const Token &error_token;
};

Token parse(std::vector<Token> &v);
Token parse(const std::string &code);

// #define DEBUG_MODE  // Décommenter pour voir l'arbre de recherche

#ifdef DEBUG_MODE
unsigned int indent_lvl = 0;
std::string indentation() {
    std::string res = "";
    for(int i = 0; i < indent_lvl && i < 20; ++i) {
        res += "   ";
    }
    return res;
}
#define SEARCH(name)                                                                               \
    cout << indentation() << "search " << name << std::endl;                                       \
    indent_lvl++;
#define FOUND(name)                                                                                \
    indent_lvl--;                                                                                  \
    cout << indentation() << "FOUND " << name << std::endl;
#define FAILED(name)                                                                               \
    indent_lvl--;                                                                                  \
    cout << indentation() << "Failed " << name << std::endl;
#define LOOP cout << indentation() << "LOOOP" << std::endl;
#else
#define SEARCH(name)
#define FOUND(name)
#define FAILED(name)
#define LOOP
#endif

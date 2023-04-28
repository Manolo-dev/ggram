#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <stack>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <cctype>
#include <algorithm>

#include "error.hpp"
#include "input_handler.hpp"

using namespace std;
using combinations = vector<vector<string>>;

void copy(string filename, ofstream &output) {
    ifstream input(filename);

    if(input.is_open()) {
        string line;
        while(getline(input, line)) {
            output << line << endl;
        }
        output << endl;
    } else {
        throw FileNotFound(filename);
    }
}

ostream& operator<<(ostream& os, vector<string> const &v) {
    os << "[";

    for(size_t i = 0; i < v.size(); i++) {
        os << v[i];
        if(i != v.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

string operator*(const string& s, uint32_t n) {
    stringstream out;

    while(n--)
        out << s;
    return out.str();
}

string operator*(uint32_t n, const string& s) { return s * n; }

string join(vector<string> const &strings, string delim = "") {
    string result = "";
    for(size_t i = 0; i < strings.size() - 1; i++) {
        result += strings[i] + delim;
    }
    result += strings.back();
    return result;
}

template<typename T>
void extend(vector<T>& dest, const vector<T>& src ){
    for(const T& elem : src){
        dest.push_back(elem);
    }
}

template<typename T>
void add_to_each_element(vector<vector<T>>& liste, const T& new_elem){
    for(vector<T> & comb : liste ) {
        comb.push_back(new_elem);
    }
}

template<typename T>
vector<T> flatten(const vector<vector<T>> & list_of_lists){
    vector<T> res;
    for(vector<T> list : list_of_lists){
        extend(res, list);
    }
    return res;
}

inline vector<string> get_inside_brackets( const vector<string>& tree, size_t & i,  const string open_bracket, const string closed_bracket ){
    unsigned int level = 1;
    vector<string> inside_brackets;
    
    while(level > 0) {
        i++;
        if(i >= tree.size()) throw InvalidSyntax(i, "Missing closing bracket");
        if(tree[i] == open_bracket) level++;
        if(tree[i] == closed_bracket) level--;
        if(level != 0) inside_brackets.push_back(tree[i]);
    }
    return inside_brackets;
}

combinations generateCombinations(const vector<string>&tree) {
    combinations result, branch ;
    branch.emplace_back();

    for(size_t i = 0; i < tree.size(); i++) {
        if(tree[i] == "(" || tree[i] == "[") {
            // Keep what's inside the brackets
            const string open_bracket = tree[i];
            const string closed_bracket = tree[i] == "(" ? ")" : "]";
            vector<string> inside_brackets = get_inside_brackets(tree, i, open_bracket, closed_bracket );

            // Possible combinations inside the brackets
            combinations sub_combinations = generateCombinations(inside_brackets);
            
            // Add these combinations to the previous results
            combinations combined_result;
            for(vector<string> & previous_result : branch) {
                for(vector<string> & sub_combination : sub_combinations ) {
                    // prefix each inside brackets combination with what was before
                    combined_result.push_back(previous_result);
                    extend(combined_result.back(), sub_combination);
                    if(open_bracket == "["){
                        combined_result.push_back(previous_result);
                    }
                }
            }
            branch = combined_result;
        } else if(tree[i] == "|") {
            extend(result, branch);
            branch.clear();
            branch.emplace_back();
        } else {    
            add_to_each_element(branch, tree[i]);
        }
    }
    extend(result, branch);
    return result;
}

static const string POP_FUNCTION_PREFIX = "pop_";

 
string generateSimpleRulePopFunction(const vector<string> & rule, const string name, InputHandler::ResultType res_type) {
    string result;
    if(res_type == InputHandler::ResultType::ORS){
        result += "    IT t = it;\n";
        result += "    master = Token(\"" + name + "\"); \n";
        result += "    vector<Token> current;\n";
    } else {
        result += "    const IT start_token = curr_it;\n" ;
        result += "    Token master(\"" + name + "\", \"\", start_token -> line(), start_token -> column() ); \n";
    }

    for(vector<string>&x : generateCombinations(rule)) {
        if(res_type == InputHandler::ResultType::ORS){
            result += "    current.clear();\n";
            result += "    if(";
        }else {
            result += "    try{\n";
            result += "        return master << " ;
        }

        for(size_t i = 0; i < x.size(); i++) {
            string prefix, suffix;
            /*
            {truc} -> pop_while(truc, current) OU pop_while(truc, curr_it, it_end)
            <truc> -> pop_truc(createNext(current))  OU pop_truc( curr_it, it_end )
            "truc" -> pop_value("truc", createNext(current)) OU pop_value("truc", curr_it, it_end )
            otherwise : INVALID_SYNTAX
            */
            switch( x[i][0] ){      
                case '{' :
                    if(x[i].back() != '}') throw InvalidSyntax("none", "Missing '}'");
                    if(res_type == InputHandler::ResultType::ORS){ 
                        prefix = "pop_while( " + POP_FUNCTION_PREFIX ; suffix = ", current)";
                    }else{
                        prefix = "pop_while(" + POP_FUNCTION_PREFIX ; suffix = ", curr_it, it_end)";
                    }
                    break;
                
                case '<' :
                    if(x[i].back() != '>') throw InvalidSyntax("none", "Missing '>'");
                    if(res_type == InputHandler::ResultType::ORS){ 
                        prefix = POP_FUNCTION_PREFIX; suffix = "(createNext(current))";
                    } else {
                        prefix = POP_FUNCTION_PREFIX; suffix = "(curr_it, it_end)";
                    }
                    break;

                case '"' :
                    if(x[i].back() != '"'  || x[i].size() < 2 ) throw InvalidSyntax("none", "Missing '\"'");
                    if(res_type == InputHandler::ResultType::ORS){
                        prefix = "pop_value(\""; suffix = "\", createNext(current))";
                    } else {
                        prefix = "pop_value(\""; suffix = "\", curr_it, it_end)";
                    }
                    break;
                
                default :
                    throw InvalidSyntax("none","Invalid syntax");
            }
            result += prefix + x[i].substr(1, x[i].size() - 2) + suffix; 

            if(i < x.size() - 1){
                if(res_type == InputHandler::ResultType::ORS){ 
                    result += " || ";
                } else {
                    result += " << ";
                }
            }
        }
        if(res_type == InputHandler::ResultType::ORS){  
            result += ") it = t;\n";
            result += "    else {master.children() += current; return 0;};\n";
        } else {
            result += ";\n    } catch(syntax_error &e) { master.children().clear(); curr_it = start_token; }\n";
        }
    }
    if(res_type == InputHandler::ResultType::ORS){ 
        result += "    return 1;\n";
    } else {
        result += "    throw syntax_error(start_token);\n" ;
    }
    return result;
}

void tryToOpenFiles(string &input_filename, string &output_filename, ifstream &file, ofstream &out) {
    if(output_filename == "") {
        throw NoFilenameSpecified("output file");
    }

    if(input_filename == "") {
        throw NoFilenameSpecified("input file");
    }

    file = ifstream(input_filename);
    if(!file.is_open()) {
        throw FileNotFound(input_filename);
    }

    remove(output_filename.c_str()); // TODO: ask for confirmation
    out = ofstream(output_filename, ios::app);
    if(!out.is_open()) {
        throw FileNotFound(output_filename);
    }
}

vector<string> createLexemes(ofstream &outputFile, ifstream &inputFile, uint32_t &lineNum) {
    vector<string> lexeme_names;
    vector<string> special_lexeme_names;
    string line;
    smatch match; 
    
    // match with : lexeme_name   "lexeme_regex"
    const regex lexeme_infos_regex(R"-(^(\.?[a-zA-Z][a-zA-Z_0-9]*)\s*"(([^"]|\\")*)"$)-");
    
    while(getline(inputFile, line)) {
        lineNum++;

        if(line == "") continue; // skip empty line
        if(line[0] == '#') continue; // ingnore comment
        if(line == "---") break; // threat only the first part of the file


        if(regex_search(line, match, lexeme_infos_regex)) {
            std::string lexeme_name = match.str(1);
            const std::string lexeme_regex = match.str(2);
    
            if (std::find(lexeme_names.begin(), lexeme_names.end(), lexeme_name) != lexeme_names.end()) {
                throw BnfError("Plural lexeme with the same name: " + lexeme_name);
            }

            if(lexeme_name[0] != '.') {
                lexeme_names.push_back(lexeme_name);
            } else {
                special_lexeme_names.push_back(lexeme_name);
                lexeme_name[0] = '_'; //change the first character to avoid having a variable name starting with a dot
            }

            // Write :
            //   const std::regex <lexeme_name>_regex("<lexeme_regex>");
            //   constexpr Lexeme <lexeme_name>("<lexeme_name>", <lexeme_name>_regex);

            outputFile << "const std::regex " + lexeme_name + "_regex(\"" + lexeme_regex + "\");" << std::endl;
        } else {
            throw InvalidSyntax(lineNum, "Invalid lexeme declaration");
        }
    }

    outputFile << std::endl << "constexpr Lexeme lexeme_list[" << lexeme_names.size() + special_lexeme_names.size() << "] = {" << std::endl;
    for(const string &lexeme_name : lexeme_names) {
        outputFile << "    {\"" + lexeme_name + "\", " + lexeme_name + "_regex}," << std::endl;
    }
    for(const string &lexeme_name : special_lexeme_names) {
        outputFile << "    {\"" + lexeme_name + "\", _" + &(lexeme_name[1]) + "_regex}," << std::endl;
    }
    outputFile << "};" << std::endl << std::endl;

    if(line != "---"){
        throw InvalidSyntax(lineNum, "Expected '---'");
    }

    return lexeme_names;
}

void writeLexemesPopFunctions(const vector<string> &token_types, ofstream &out, InputHandler::ResultType res_type) {
    for(const string & token_name : token_types) {
        if(res_type == InputHandler::ResultType::ORS){
            //Writes bool __token_name(Token & master) { return _type("token_name", master); } 
            out << "bool " << POP_FUNCTION_PREFIX << token_name << "(Token &master) { return pop_type(\"" + token_name + "\", master); }" << endl;
        } else {   
            // //Writes : Token pop_token(IT& curr_it, const IT it_end) { return pop_type("token", curr_it, it_end); }
            out << "Token pop_" << token_name << "(IT& it_curr, const IT it_end) { return pop_type(\"" + token_name + "\", it_curr, it_end); }" << endl;
        }
    }
    out << endl;
}

struct PairRuleFunction {
    PairRuleFunction(string n, string f):name(n), function(f){}
    string name;
    string function;
};

typedef vector<string> Rule;

void addRulePopFunctions(const Rule& rule, const string name, vector<PairRuleFunction>& result , InputHandler::ResultType res_type ) {
    size_t j = 0;
    vector<string> newRule;
    for(size_t i = 0; i < rule.size(); i++) {
        if(rule[i] == "{") {
            vector<string> loopRule = get_inside_brackets(rule, i, "{", "}");
            
            const string aux_name = to_string(j) + "_" + name; j++;
            
            addRulePopFunctions(loopRule, aux_name, result, res_type);

            newRule.push_back("{" + aux_name + "}");
            
        } else {
            newRule.push_back(rule[i]);
        }
    }
    result.emplace_back(name, generateSimpleRulePopFunction(newRule, name, res_type));
}

int find_outside_quotes(const string& s, const char elem, bool& in_quotes){
    for(size_t i = 0; i < s.size() ; i++){
        const char c = s[i];
        if(c == '"'){
            in_quotes = !in_quotes;
        } else if(c == ';' && !in_quotes){
            return i;
        }
    }
    return -1;
}

vector<pair<string, Rule>> readRules(ifstream &file, uint32_t& lineNum){
    vector<pair<string, Rule>> rules;
    vector<string> allRuleNames;
    string line;
    smatch match;
    Rule currentRule;
    stack<string> brackets;
    
    vector<pair<string, regex>> regexes = {
        {".ignore", regex(R"-([ \n\r\s\t]+)-")},
        {".comment", regex(R"-(#.+$)-")},
        {"rulename", regex(R"-(<[a-zA-Z][a-zA-Z0-9_]*>)-")},
        {"assign", regex(R"-(::=)-")},
        {"or", regex(R"-(\|)-")},
        {"parenth", regex(R"-(\()-")},
        {"endparenth", regex(R"-(\))-")},
        {"loop", regex(R"-(\{)-")},
        {"endloop", regex(R"-(\})-")},
        {"option", regex(R"-(\[)-")},
        {"endoption", regex(R"-(\])-")},
        {"string", regex(R"-(\"([^"]|\\")*\")-")},
        {"end", regex(R"-(;)-")}
    };

    while(getline(file, line)) {
        lineNum++;
        if(line[0] == '#') continue; // ingnore comment
        if(line == "---") break; // threat only the second part of the file
        if(line == "") continue; // skip empty line
        while(line.size() > 0) {
            for(auto [name, regex] : regexes) {
                if(regex_search(line, match, regex, regex_constants::match_continuous | regex_constants::match_not_null )) {
                    if(name == ".ignore" || name == ".comment") {
                        line = match.suffix();
                    } else if(name == "end") {
                        if(currentRule.size() < 2)
                            throw InvalidSyntax(lineNum, "Expected rule name");
                        if(brackets.size() != 0)
                            throw InvalidSyntax(lineNum, "Expected ')', '}' or ']'");
                        rules.emplace_back(currentRule[0].substr(1, currentRule[0].size() - 2), Rule(currentRule.begin() + 2, currentRule.end()));
                        allRuleNames.push_back(currentRule[0]);
                        line = match.suffix();
                        currentRule.clear();
                    } else {
                        if(currentRule.size() == 0 && name != "rulename")
                            throw InvalidSyntax(lineNum, "Expected rule name");
                        if(currentRule.size() == 1 && name != "assign")
                            throw InvalidSyntax(lineNum, "Expected '::='");
                        if(currentRule.size() == 0 && find(allRuleNames.begin(), allRuleNames.end(), match.str(0)) != allRuleNames.end())
                            throw InvalidSyntax(lineNum, "Rule '" + match.str(0) + "' already defined");
                        if(name == "parenth" || name == "loop" || name == "option") { brackets.push(name);
                        } else if(name == "endparenth" || name == "endloop" || name == "endoption") {
                            if(name == "endparenth" && brackets.top() != "parenth")
                                throw InvalidSyntax(lineNum, "Unexpected '" + name + "'");
                            if(brackets.empty())
                                throw InvalidSyntax(lineNum, "Unexpected '" + name + "'");
                            if(name == "endloop" && brackets.top() != "loop")
                                throw InvalidSyntax(lineNum, "Unexpected '" + name + "'");
                            if(name == "endoption" && brackets.top() != "option")
                                throw InvalidSyntax(lineNum, "Unexpected '" + name + "'");
                            brackets.pop();
                        }

                        currentRule.push_back(match.str(0));
                        line = match.suffix();
                    }
                }
            }
        }
    }
    if(brackets.size() != 0)
        throw InvalidSyntax(lineNum, "Expected ')', '}' or ']'");
    if(currentRule.size() != 0)
        throw InvalidSyntax(lineNum, "Expected ';'");
    return rules;
}
void writeRulesPopFunctions( ofstream &out, vector<pair<string, Rule>> rules, InputHandler::ResultType res_type){
    // Declare rules' pop functions
    for(auto [rule_name, _] : rules){
        if(res_type == InputHandler::ResultType::ORS){ 
            out << "bool " << POP_FUNCTION_PREFIX << rule_name << "(Token &master);" << endl;
        } else {
            out << "Token " << POP_FUNCTION_PREFIX << rule_name << "(IT&, const IT);" << endl;
        }
    }
    // Define rules' pop functions
    for(auto [rule_name ,rule_expr] : rules) {
        out << "\n";
        out << "/***************************************************************************************************/\n";
        out << "/*                       Functions to pop tokens of type : " << rule_name << (40-rule_name.size())*string(" ") << "*/\n";
        out << "/***************************************************************************************************/\n" ;
        vector<PairRuleFunction> result;
        addRulePopFunctions(rule_expr, rule_name, result, res_type );
        for(auto [aux_rule_name, aux_rule_func ] : result){
            out << endl;
            if(res_type == InputHandler::ResultType::ORS){ 
                out << "bool " << POP_FUNCTION_PREFIX << aux_rule_name << "(Token &master) {" << endl;
            } else {
                out << "Token " << POP_FUNCTION_PREFIX << aux_rule_name << "(IT& curr_it, const IT it_end) {" << endl;
            }
            out << aux_rule_func;
            out << "}" << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    InputHandler::Configuration cfg;

    try {
        InputHandler::handleParameters(argc, argv, cfg);
    } catch (InputError& except) {
        std::cerr << "Input error :\n  " << except.what() << std::endl;
        exit(1);
    }

    ifstream file;
    ofstream out;
    
    tryToOpenFiles(cfg.input_filename, cfg.output_filename, file, out);

    uint32_t lineNum = 0;

    copy("template/head.cpp", out);

    copy("template/lexeme.cpp", out);
    vector<string> lexeme_names = createLexemes(out, file, lineNum);

    copy("template/token.cpp", out);
    copy("template/lex.cpp", out);
    
    if(cfg.result_type == InputHandler::ResultType::ORS){ 
        copy("template/value_expression.cpp", out);
    } else {
        copy("template/value_expression2.cpp", out);
    }

    writeLexemesPopFunctions(lexeme_names, out, cfg.result_type);

    vector<pair<string, Rule>> rules = readRules(file, lineNum);
    for(auto [rule_name, _] : rules){
        cout << rule_name << endl;
        cout << _ << endl;
    }
    writeRulesPopFunctions(out, rules, cfg.result_type);

    out << std::endl;
    if(cfg.result_type == InputHandler::ResultType::ORS){
        copy("template/main.cpp", out);
    }  else {
        copy("template/main2.cpp", out);
    }
    file.close();
    out.close();

    return 0;
}

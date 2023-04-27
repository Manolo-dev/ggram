#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <stack>
#include <cstring>
#include <cstdint>
#include <cstdio>

#include "error.hpp"
#include "options.hpp"

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

 
string generateSimpleRulePopFunction(const vector<string> & rule, const string name) {
    string result = R"(    IT t = it;
    master = Token( ")" + name + R"(" );
    vector<Token> current;)";
    result += "\n";
    for(vector<string>&x : generateCombinations(rule)) {
        cout << x << endl;
        result += "    current.clear();\n";
        result += "    if(";
        for(size_t i = 0; i < x.size(); i++) {
            string prefix, suffix;
            /*
            {truc} -> _loop(truc, current)
            <truc> -> pop_truc(next(current))
            "truc" -> _value("truc", next(current))
            otherwise : INVALID_SYNTAX
            */
            switch( x[i][0] ){      
                case '{' :
                    if(x[i].back() != '}') throw InvalidSyntax("none", "Missing '}'");
                    prefix = "pop_while( " + POP_FUNCTION_PREFIX ; suffix = ", current)";
                    break;
                case '<' :
                    if(x[i].back() != '>') throw InvalidSyntax("none", "Missing '>'");
                    prefix = POP_FUNCTION_PREFIX; suffix = "(next(current))";
                    break;
                case '"' :
                    if(x[i].back() != '"'  || x[i].size() < 2 ) throw InvalidSyntax("none", "Missing '\"'");
                    prefix = "pop_value(\""; suffix = "\", next(current))";
                    break;
                default :
                    throw InvalidSyntax("none","Invalid syntax");
            }
            result += prefix + x[i].substr(1, x[i].size() - 2) + suffix; 

            if(i < x.size() - 1)
                result += " || ";
        }
        result += ") it = t;\n";
        result += "    else {master.children() += current; return 0;};\n";
    }
    result += "    return 1;\n";
    return result;
}

void checkFile(string &filename, string &output, ifstream &file, ofstream &out) {
    if(output == "") {
        throw NoFilenameSpecified("output file");
    }

    if(filename == "") {
        throw NoFilenameSpecified("input file");
    }

    file = ifstream(filename);
    if(!file.is_open()) {
        throw FileNotFound(filename);
    }

    remove(output.c_str());
    out = ofstream(output, ios::app);

    if(!out.is_open()) {
        throw FileNotFound(output);
    }

}

vector<string> createLexemes(ofstream &outputFile, ifstream &inputFile, uint32_t &lineNum) {
    outputFile << "void create_lexemes(vector<Lexeme> &lexemes) {" << endl;
    
    vector<string> lexeme_names;
    string line;
    smatch match; 
    
    // match with : lexeme_name   "lexeme_regex"
    const regex lexem_infos_regex(R"-(^(\.?[a-zA-Z][a-zA-Z_0-9]*)\s*"(([^"]|\\")*)"$)-");
    
    while(getline(inputFile, line)) {
        lineNum++;

        if(line == "") continue; // skip empty line
        if(line[0] == '#') continue; // ingnore comment
        if(line == "---") break; // threat only the first part of the file


        if(regex_search(line, match, lexem_infos_regex)) {
            // Write :  lexemes.emplace_back("lexeme_name", "lexeme_regexe");
            outputFile << ("    lexemes.emplace_back(\"" + match.str(1) + "\", \"(" + match.str(2) + ")\");") << endl;
            
            if(match.str(1)[0] != '.')
                lexeme_names.push_back(match.str(1));
        } else {
            throw InvalidSyntax(lineNum, "Invalid lexeme declaration");
        }
    }
    outputFile << "}" << endl << endl;

    if(line != "---"){
        throw InvalidSyntax(lineNum, "Expected '---'");
    }
    return lexeme_names;
}

void writeLexemesPopFunctions(const vector<string> &token_types, ofstream &out) {
    for(const string & token_name : token_types) {
        //Writes bool __token_name(Token & master) { return _type("token_name", master); } 
        out << "bool " << POP_FUNCTION_PREFIX << token_name << "(Token &master) { return pop_type(\"" + token_name + "\", master); }" << endl;
        // //Writes : Token pop_token(IT& curr_it, const IT it_end) { return pop_type("token", curr_it, it_end); }
        // out << "Token pop_" << token_name << "(IT& it_curr, const IT it_end) { return pop_type(\"" + token_name + "\", it_curr, it_end); }" << endl;
    }
    out << endl;
}

struct PairRuleFunction {
    PairRuleFunction(string n, string f):name(n), function(f){}
    string name;
    string function;
};

typedef vector<string> Rule;

void addRulePopFunctions(const Rule& rule, const string name, vector<PairRuleFunction>& result ) {
    size_t j = 0;
    vector<string> newRule;
    for(size_t i = 0; i < rule.size(); i++) {
        if(rule[i] == "{") {
            vector<string> loopRule = get_inside_brackets(rule, i, "{", "}");
            
            const string aux_name = to_string(j) + "_" + name; j++;
            
            addRulePopFunctions(loopRule, aux_name, result);

            newRule.push_back("{" + aux_name + "}");
            
        } else {
            newRule.push_back(rule[i]);
        }
    }
    result.emplace_back(name, generateSimpleRulePopFunction(newRule, name));
}


vector<pair<string, Rule>> readRules(ifstream &file, uint32_t& lineNum){
    vector<pair<string, Rule>> rules;
    string line;
    smatch match;
    
    // regex match with : <rule_name>  ::= rule_expression
    const regex rule_infos_regex(R"(^\s*<([a-zA-Z][a-zA-Z_0-9]*)>\s*::=\s*(.+)\s*$)");
    
    while(getline(file, line)) {
        lineNum++;
        while(line.size() > 0){

            if(line[0] == '#') continue; // ingnore comment
            if(line == "---") break; // threat only the second part of the file
            

            if(!regex_search(line, match, rule_infos_regex)) { 
                break;
                // throw InvalidSyntax(line, "Invalid rule definition");
            }
            const string rule_name = match.str(1);
            string rule_expr = match.str(2);

            // rule expression ends at ';''
            while(line.find(';') > line.length()) {
                lineNum++;
                if(!getline(file, line)) {
                    throw InvalidSyntax(line, "Expected ';'");
                }
                rule_expr += line;
            }
            // get the rule_expression 
            rule_expr = rule_expr.substr(0, rule_expr.find(';')); 
            // what's after is kept to be treated
            line = rule_expr.substr(rule_expr.find(';') + 1, rule_expr.length());
            
            // bool new_elem = true;
            // for(const char : rule_expr ){
                
            // }
            // split the expression by words
            regex space_regex("\\s+");
            regex_token_iterator<string::iterator> j(rule_expr.begin(), rule_expr.end(), space_regex, -1); 
            regex_token_iterator<string::iterator> end;
            
            Rule currentRule;
            while(j != end) 
                currentRule.push_back(*j++);

            rules.emplace_back(rule_name, currentRule);
        }
    }
    return rules;
}
void writeRulesPopFunctions( ofstream &out, vector<pair<string, Rule>> rules){
    // Declare rules' pop functions
    for(auto [rule_name, _] : rules){
        out << "bool " << POP_FUNCTION_PREFIX << rule_name << "(Token &master);" << endl;
    }
    // Define rules' pop functions
    for(auto [rule_name ,rule_expr] : rules) {
        out << endl;
        out << "/***************************************************************************************************/" << endl;
        out << "/*                       Functions to pop tokens of type : " << rule_name << (40-rule_name.size())*string(" ") << "*/"  <<endl;
        out << "/***************************************************************************************************/" << endl;
        vector<PairRuleFunction> result;
        addRulePopFunctions(rule_expr, rule_name, result );
        for(auto [aux_rule_name, aux_rule_func ] : result){
            out << endl << "bool " << POP_FUNCTION_PREFIX << aux_rule_name << "(Token &master) {" << endl;
            out << aux_rule_func;
            out << "}" << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    Options::Configuration cfg;

    try {
        Options::handleOptions(argc, argv, cfg);
    } catch (OptionError& except) {
        std::cerr << "Option error :\n  " << except.what() << std::endl;
        exit(1);
    }

    ifstream file;
    ofstream out;
    
    checkFile(cfg.input_file, cfg.output_file, file, out);

    copy("template/head.cpp.part", out);
    copy("template/Lexeme.cpp.part", out);
    copy("template/Token.cpp.part", out);
    copy("template/lex.cpp.part", out);
    copy("template/valueExpression.cpp.part", out);

    uint32_t lineNum = 0;

    vector<string> lexeme_names = createLexemes(out, file, lineNum);
    writeLexemesPopFunctions(lexeme_names, out);

    writeRulesPopFunctions(out, readRules(file, lineNum));
    
    out << endl;
    copy("template/main.cpp.part", out);

    file.close();
    out.close();

    return 0;
}

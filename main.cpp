#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <filesystem>
#include <vector>
#include <stack>
#include "Error.cpp"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long int u64;
typedef signed long int s64;

typedef float f32;
typedef double f64;

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
        Error err(ErrorType::FILE_NOT_FOUND, filename);
        err.throw_error();
    }
}

ostream& operator<<(ostream& os, vector<string> const &v) {
    os << "[";

    for(u64 i = 0; i < v.size(); i++) {
        os << v[i];
        if(i != v.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

string operator*(const string& s, u32 n) {
    stringstream out;

    while(n--)
        out << s;
    return out.str();
}

string operator*(u32 n, const string& s) { return s * n; }

string join(vector<string> const &strings, string delim = "", string exception = "") {
    string result = "";
    for(u64 i = 0; i < strings.size() - 1; i++) {
        if(strings[i] == exception) {
            result += strings[i];
        } else if(strings[i+1] == exception) {
            result += strings[i];
        } else {
            result += strings[i] + delim;
        }
    }
    result += strings[strings.size() - 1];
    return result;
}

combinations generateCombinations(vector<string>&tree) {
    vector<combinations> or_results = {{{}}};
    combinations*result = &or_results[0];
    for(u64 i = 0; i < tree.size(); i++) {
        if(tree[i] == "(") {
            s32 level = 1;
            vector<string> temp;
            while(level > 0) {
                i++;
                if(tree[i] == "(") level++;
                if(tree[i] == ")") level--;
                if(level != 0) temp.push_back(tree[i]);
            }

            combinations ors = generateCombinations(temp);
            combinations temp_result;
            for(u64 j = 0; j < result->size(); j++) {
                for(u64 k = 0; k < ors.size(); k++) {
                    temp_result.push_back((*result)[j]);
                    for(u64 l = 0; l < ors[k].size(); l++) {
                        temp_result[temp_result.size() - 1].push_back(ors[k][l]);
                    }
                }
            }
            *result = temp_result;
        } else if(tree[i] == "[") {
            s32 level = 1;
            vector<string> temp;
            while(level > 0) {
                i++;
                if(tree[i] == "[") level++;
                if(tree[i] == "]") level--;
                if(level != 0) temp.push_back(tree[i]);
            }

            combinations ors = generateCombinations(temp);
            ors.push_back({});
            combinations temp_result;
            for(u64 j = 0; j < result->size(); j++) {
                for(u64 k = 0; k < ors.size(); k++) {
                    temp_result.push_back((*result)[j]);
                    for(u64 l = 0; l < ors[k].size(); l++) {
                        temp_result[temp_result.size() - 1].push_back(ors[k][l]);
                    }
                }
            }
            *result = temp_result;
        } else if(tree[i] == "|") {
            or_results.push_back({{}});
            result = &or_results[or_results.size() - 1];
        } else {
            for(u64 j = 0; j < result->size(); j++) {
                (*result)[j].push_back(tree[i]);
            }
        }
    }

    combinations final_result;
    for(u64 i = 0; i < or_results.size(); i++) {
        for(u64 j = 0; j < or_results[i].size(); j++) {
            final_result.push_back(or_results[i][j]);
        }
    }
    return final_result;
}

string parse(vector<string>&tree) {
    string result = "";
    for(vector<string>&x : generateCombinations(tree)) {
        result += "    if(";
        for(u64 i = 0; i < x.size(); i++) {
            if(x[i][0] == '{' && x[i][x[i].size() - 1] == '}')
                result += "_loop(" + x[i].substr(1, x[i].size() - 2) + ")";
            else if(x[i][0] == '<' && x[i][x[i].size() - 1] == '>')
                result += "__" + x[i].substr(1, x[i].size() - 2) + "()";
            else if(x[i][0] == '"' && x[i][x[i].size() - 1] == '"')
                result += "__(\"" + x[i].substr(1, x[i].size() - 2) + "\")";
            if(i < x.size() - 1)
                result += " || ";
        }
        result += ") it = t;\n    else return 0;\n";
    }
    return result;
}

void getArg(int argc, char *argv[], string &filename, string &output) {
    for(u32 i = 1; i != argc; ++i) {
        if(strcmp(argv[i],"-h") == 0) {
            cout << "Usage: " << endl;
            cout << "  " << argv[0] << " -h" << endl;
            cout << "  " << argv[0] << " -v" << endl;
            cout << "  " << argv[0] << " -f \e[4mfile\e[0m -o \e[4mfile\e[0m" << endl;
            cout << "Options:" << endl;
            cout << "  -h  Show this help message" << endl;
            cout << "  -v  Show version" << endl;
            cout << "  -f  File name" << endl;
            cout << "  -o  Output file name" << endl;
            exit(0);
        } else if(strcmp(argv[i],"-v") == 0) {
            cout << "Version: 0.0.1" << endl;
            exit(0);
        } else if(strcmp(argv[i],"-f") == 0) {
            if(i+1 < argc) {
                filename = argv[i+1];
                string extension = filename.substr(filename.find_last_of(".") + 1);
                if(extension != "gg") {
                    Error err(ErrorType::INVALID_FILE_EXTENSION, ".gg");
                    err.throw_error();
                }
            } else {
                Error err(ErrorType::NO_FILENAME_SPECIFIED, "input file");
                err.throw_error();
            }
        } else if(strcmp(argv[i],"-o") == 0) {
            if(i+1 < argc) {
                output = argv[i+1];
                string extension = output.substr(output.find_last_of(".") + 1);
                if(extension != "cpp") {
                    Error err(ErrorType::INVALID_FILE_EXTENSION, ".cpp");
                    err.throw_error();
                }
            } else {
                Error err(ErrorType::NO_FILENAME_SPECIFIED, "output file");
                err.throw_error();
            }
        }
    }
}

void checkFile(string &filename, string &output, ifstream &file, ofstream &out) {
    if(output == "") {
        Error err(ErrorType::NO_FILENAME_SPECIFIED, "output file");
        err.throw_error();
    }

    if(filename == "") {
        Error err(ErrorType::NO_FILENAME_SPECIFIED, "input file");
        err.throw_error();
    }

    file = ifstream(filename);
    if(!file.is_open()) {
        Error err(ErrorType::FILE_NOT_FOUND, filename);
        err.throw_error();
    }

    filesystem::remove(output);
    out = ofstream(output, ios::app);

    if(!out.is_open()) {
        Error err(ErrorType::FILE_NOT_FOUND, output);
        err.throw_error();
    }

    copy("template/head.cpp", out);
    copy("template/Token.cpp", out);
    copy("template/Lexeme.cpp", out);
    copy("template/lex.cpp", out);
}

string createLexemsPart(ofstream &outputFile, ifstream &inputFile, string &outputName, vector<string> &tokens, u32 &lineNum) {
    outputFile << "void create_lexemes(vector<Lexeme> &lexemes) {" << endl;

    string line;
    while(getline(inputFile, line)) {
        lineNum++;

        if(line == "") continue; // skip empty line
        if(line[0] == '#') continue; // ingnore comment
        if(line == "---") break; // threat only the first part of the file

        smatch match;
        regex re("^(\\.?[a-zA-Z_][a-zA-Z_0-9]*)\\s*\"(([^\"]|\\\\\")*)\"$");

        if(regex_search(line, match, re)) {
            try {
                regex check(match.str(2));
            } catch (regex_error& e) {
                string arg[] = {match.str(2), to_string(lineNum), e.what()};
                Error err(ErrorType::REGEX_ERROR, arg);
                err.throw_error();
            }

            outputFile << ("    lexemes.push_back(Lexeme(\"" + match.str(1) + "\", \"(" + match.str(2) + ")\"));") << endl;

            if(match.str(1)[0] != '.')
                tokens.push_back(match.str(1));
        } else {
            Error err(ErrorType::INVALID_SYNTAX, lineNum);
            err.throw_error();
        }
    }

    outputFile << "}" << endl << endl;
    return line;
}

void typeExpressionGenerator(vector<string> &tokens, ofstream &out) {
    for(auto token : tokens) {
        out << "bool __" << token << "() { if(it == it_e) return 1; if(it->type() != \"" + token + "\") return 1; it++; return 0; }" << endl;
    }
}

vector<string> loopExpressionGenerator(vector<string>&currentRule, vector<pair<string, string>>&rules, string name) {
    u32 j = 0;
    vector<string> newRule;
    for(u32 i = 0; i < currentRule.size(); i++) {
        if(currentRule[i] == "{") {
            s32 level = 1;
            vector<string> loopRule;
            while(level != 0) {
                i++;
                if(currentRule[i] == "{") level++;
                if(currentRule[i] == "}") level--;
                if(level != 0) loopRule.push_back(currentRule[i]);
            }
            loopRule = loopExpressionGenerator(loopRule, rules, "__" + to_string(j) + name);
            newRule.push_back("{_" + to_string(j) + name + "}");
            rules.push_back(make_pair("._" + to_string(j) + name, parse(loopRule)));
            j++;
        } else {
            newRule.push_back(currentRule[i]);
        }
    }
    return newRule;
}

void createRulesPart(ifstream &file, ofstream &out, u32 lineNum, vector<pair<string, string>> &rules){
    bool semicolon = false;
    string line;
    while(1) {
        if(!semicolon) {
            if(!getline(file, line))
                break;
            lineNum++;
        }

        if(line == "") continue; // skip empty line
        if(line[0] == '#') continue; // ingnore comment
        if(line == "---") break; // threat only the second part of the file
        smatch match;
        regex re("^\\s*<([a-zA-Z_][a-zA-Z_0-9]*)>\\s*::=\\s*(.+)\\s*$");

        if(regex_search(line, match, re)) { // TODO: make error handling
            string name = match.str(1);
            string expr = match.str(2);
            u32 i = 0;

            while(line.find(';') > line.length()) { // run until the end of the expression
                i++;
                if(!getline(file, line)) {
                    Error err(ErrorType::INVALID_RULE, lineNum);
                    err.throw_error();
                }
                expr += line;
            }

            line = expr.substr(expr.find(';') + 1, expr.length()); // get the rest of the line
            expr = expr.substr(0, expr.find(';')); // get the expression

            lineNum += i;
            semicolon = line.size() > 0;

            regex e("\\s+");
            regex_token_iterator<string::iterator> j(expr.begin(), expr.end(), e, -1); // split the expression by words
            regex_token_iterator<string::iterator> end;

            vector<string> currentRule;

            while(j != end) // convert the expression to vector
                currentRule.push_back(*j++);

            currentRule = loopExpressionGenerator(currentRule, rules, "_" + name);
            rules.push_back(make_pair(name, parse(currentRule)));
            // pair : first - name, second - expression
        }
    }
}

void declareRule(vector<pair<string, string>> &rules, ofstream &out) {
    for(auto rule : rules) {
        if(rule.first[0] == '.')
            out << "bool " << rule.first.substr(1) << "();" << endl;
        else
            out << "bool __" << rule.first << "();" << endl;
    }
}

void writeRule(vector<pair<string, string>> &rules, ofstream &out) {
    for(auto rule : rules) {
        if(rule.first[0] == '.')
            out << endl << "bool " << rule.first.substr(1) << "() {" << endl;
        else
            out << endl << "bool __" << rule.first << "() {" << endl;
        out << "    IT t = it;" << endl << rule.second;
        out << "    return 1;" << endl;
        out << "}" << endl;
    }
}

int main(int argc, char *argv[]) {
    string filename = "";
    string outputName = "output.cpp";

    getArg(argc, argv, filename, outputName);

    ifstream file;
    ofstream out;
    
    checkFile(filename, outputName, file, out);

    vector<string> tokens;
    u32 lineNum = 0;

    string last_line = createLexemsPart(out, file, outputName, tokens, lineNum);

    copy("template/valueExpression.cpp", out);
    typeExpressionGenerator(tokens, out);

    if(last_line != "---")
        return 0; // no rules

    vector<pair<string, string>> rules;

    createRulesPart(file, out, lineNum, rules);

    declareRule(rules, out);

    writeRule(rules, out);
    
    out << endl;
    copy("template/main.cpp", out);

    file.close();
    out.close();

    return 0;
}
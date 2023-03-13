#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <filesystem>
#include <vector>

using namespace std;

ostream& operator<<(ostream& os, vector<string> const &v) {
    os << "[";
    for(long unsigned int i = 0; i < v.size(); i++) {
        os << v[i];
        if(i != v.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

string join(vector<string> const &strings, string delim = "", string exception = "") {
    string result = "";
    for(long unsigned int i = 0; i < strings.size() - 1; i++) {
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

string ____or(vector<string> const &strings) {
    string result = "";
    for(long unsigned int i = 0; i < strings.size() - 1; i++) {
        result += "t = it;try {";
        result += strings[i];
        result += "} catch(...) {it = t;";
    }
    result += strings[strings.size() - 1];
    for(long unsigned int i = 0; i < strings.size() - 1; i++) {
        result += "}";
    }
    return result;
}

string ____option(vector<string> const &strings) {
    string result = "";
    result += "t = it;try {";
    for(long unsigned int i = 0; i < strings.size(); i++) {
        result += strings[i];
    }
    result += "} catch(...) {it = t;}";
    return result;
}

string ____loop(vector<string> const &strings) {
    string result = "";
    for(long unsigned int i = 0; i < strings.size(); i++) {
        result += strings[i];
    }
    result += "while(true) {t = it; try {";
    for(long unsigned int i = 0; i < strings.size(); i++) {
        result += strings[i];
    }
    result += "} catch(...) {it = t;break;}}";
    return result;
}

vector<string> parse(vector<string>&words) {
    vector<string> result = vector<string>();
    for(long unsigned int i = 0; i < words.size(); i++) {
        if(words[i][0] == '<' && words[i][words[i].size() - 1] == '>') {
            string word = words[i].substr(1, words[i].size() - 2);
            result.push_back("__" + word + "(it, e);");
        }
        if(words[i] == "(") {
            int level = 1;
            vector<string> subwords = vector<string>();
            for(long unsigned int j = i + 1; j < words.size(); j++) {
                if(words[j] == "(") {
                    level++;
                } else if(words[j] == ")") {
                    level--;
                }
                if(level == 0) {
                    i = j;
                    break;
                }
                subwords.push_back(words[j]);
            }
            cout << subwords << endl;
            result.push_back(join(parse(subwords)));
        }
        if(words[i] == "[") {
            int level = 1;
            vector<string> subwords = vector<string>();
            for(long unsigned int j = i + 1; j < words.size(); j++) {
                if(words[j] == "[") {
                    level++;
                } else if(words[j] == "]") {
                    level--;
                }
                if(level == 0) {
                    i = j;
                    break;
                }
                subwords.push_back(words[j]);
            }
            result.push_back(____option(parse(subwords)));
        }
        if(words[i] == "{") {
            int level = 1;
            vector<string> subwords = vector<string>();
            for(long unsigned int j = i + 1; j < words.size(); j++) {
                if(words[j] == "{") {
                    level++;
                } else if(words[j] == "}") {
                    level--;
                }
                if(level == 0) {
                    i = j;
                    break;
                }
                subwords.push_back(words[j]);
            }
            result.push_back(____loop(parse(subwords)));
        }
        if(words[i] == "<") {
            int level = 1;
            vector<string> subwords = vector<string>();
            for(long unsigned int j = i + 1; j < words.size(); j++) {
                if(words[j] == "<") {
                    level++;
                } else if(words[j] == ">") {
                    level--;
                }
                if(level == 0) {
                    i = j;
                    break;
                }
                subwords.push_back(words[j]);
            }
            result.push_back(____or(parse(subwords)));
        }
    }
    return result;
}


int main(int argc, char *argv[]) {
    string filename = "";
    string output = "output.cpp";
    for(int i = 1; i != argc; ++i) {
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
            return 0;
        } else if(strcmp(argv[i],"-v") == 0) {
            cout << "Version: 0.0.1" << endl;
            return 0;
        } else if(strcmp(argv[i],"-f") == 0) {
            if(i+1 < argc) {
                filename = argv[i+1];
                string extension = filename.substr(filename.find_last_of(".") + 1);
                if(extension != "gg") {
                    cerr << "\e[1;31mError:\e[0m Invalid file extension" << endl;
                    return 1;
                }
            } else {
                cerr << "\e[1;31mError:\e[0m No file name specified" << endl;
                return 1;
            }
        } else if(strcmp(argv[i],"-o") == 0) {
            if(i+1 < argc) {
                output = argv[i+1];
                string extension = output.substr(output.find_last_of(".") + 1);
                if(extension != "cpp") {
                    cerr << "\e[1;31mError:\e[0m Invalid file extension" << endl;
                    return 1;
                }
            } else {
                cerr << "\e[1;31mError:\e[0m No file name specified" << endl;
                return 1;
            }
        }
    }

    if(output == "") {
        cerr << "\e[1;31mError:\e[0m No output file specified" << endl;
        return 1;
    }

    if(filename == "") {
        cerr << "\e[1;31mError:\e[0m No file specified" << endl;
        return 1;
    }

    ifstream file(filename);
    if(!file.is_open()) {
        cerr << "\e[1;31mError:\e[0m Could not open file" << endl;
        return 1;
    }

    filesystem::remove(output);
    filesystem::copy("template.cpp", output);

    ofstream out(output, ios::app);
    if(!out.is_open()) {
        cerr << "\e[1;31mError:\e[0m Could not open output file" << endl;

        filesystem::remove(output);
        return 1;
    }

    out << endl << endl << "void create_lexemes(vector<Lexeme> &lexemes) {" << endl;

    string line;
    int lineNum = 0;
    vector<string> tokens;
    while(getline(file, line)) {
        lineNum++;
        if(line == "") continue;
        if(line[0] == '#') continue;
        if(line == "---") break;
        smatch match;
        regex re("^([a-zA-Z_][a-zA-Z_0-9]*)\\s*\"(([^\"]|\\\\\")*)\"$");
        if(regex_search(line, match, re)) {
            try {
                regex check(match.str(2));
            } catch (regex_error& e) {
                cerr << "\e[1;31mError:\e[0m Invalid regex on line \e[1m" << lineNum << "\e[0m" << endl;

                filesystem::remove(output);
                return 1;
            }
            out << ("    lexemes.push_back(Lexeme(\"" + match.str(1) + "\", \"(" + match.str(2) + ")\"));") << endl;
            tokens.push_back(match.str(1));
        } else {
            cerr << "\e[1;31mError:\e[0m Invalid syntax on line \e[1m" << lineNum << "\e[0m" << endl;

            filesystem::remove(output);
            return 1;
        }
    }

    out << "}" << endl << endl;

    for(auto token : tokens) {
        out << "void __" << token << "(IT&it, IT e) { if(it == e) throw \"BAD\"; if(it->type() != \"" + token + "\") throw \"BAD\"; it++; }" << endl;
    }

    if(line != "---")
        return 0;

    bool semicolon = false;
    vector<pair<string, string>> rules;
    while(1) {
        if(!semicolon) {
            if(!getline(file, line))
                break;
            lineNum++;
        }
        if(line == "") continue;
        if(line[0] == '#') continue;
        if(line == "---") break;
        smatch match;
        regex re("^\\s*<([a-zA-Z_][a-zA-Z_0-9]*)>\\s*::=\\s*(.+)\\s*$");
        if(regex_search(line, match, re)) {
            string name = match.str(1);
            string expr = match.str(2);
            int i = 0;
            while(line.find(';') > line.length()) {
                i++;
                if(!getline(file, line)) {
                    cerr << "\e[1;31mError:\e[0m Invalid rule on line \e[1m" << lineNum << "\e[0m" << endl;

                    filesystem::remove(output);
                    return 1;
                }
                expr += line;
            }
            line = expr.substr(expr.find(';') + 1, expr.length());
            expr = expr.substr(0, expr.find(';'));
            lineNum += i;
            semicolon = line.size() > 0;

            regex e("\\s+");
            regex_token_iterator<string::iterator> j(expr.begin(), expr.end(), e, -1);
            regex_token_iterator<string::iterator> end;

            vector<string> currentRule;
            while(j != end) {
                string word = *j++;
                currentRule.push_back(word);
            }

            rules.push_back(make_pair(name, "IT t;" + join(parse(currentRule))));
        }
    }

    for(auto rule : rules) {
        out << "void __" << rule.first << "(IT&it, IT e);" << endl;
    }

    for(auto rule : rules) {
        out << endl << "void __" << rule.first << "(IT&it, IT e) {" << endl;
        out << "    " << rule.second << endl;
        out << "}" << endl;
    }

    out << endl;
    ifstream main_template("main_template.cpp");
    while(getline(main_template, line)) {
        out << line << endl;
    }
    main_template.close();

    file.close();
    out.close();

    return 0;
}
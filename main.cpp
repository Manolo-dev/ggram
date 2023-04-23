#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <filesystem>
#include <vector>
#include <stack>
#include <cstring>
#include "Error.hpp"
#include <cstdint>

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


combinations generateCombinations(vector<string>&tree) {
	combinations result, branch ;
	branch.emplace_back();

	for(size_t i = 0; i < tree.size(); i++) {
		if(tree[i] == "(" || tree[i] == "[") {
			// Keep what's inside the brackets
			vector<string> inside_brackets;
			const string open_bracket = tree[i];
			const string closed_bracket = (tree[i] == "(" ? ")" : "]"); 
			unsigned int level = 1;
			
			while(level > 0) {
				i++;
				if(i >= tree.size()) Error(ErrorType::INVALID_SYNTAX, "Missing bracket !").throw_error();
				if(tree[i] == open_bracket) level++;
				if(tree[i] == closed_bracket) level--;
				if(level != 0) inside_brackets.push_back(tree[i]);
			}

			// Possible combinations inside the brackets
			combinations sub_combinations = generateCombinations(inside_brackets);
			
			// Add these combinations to the previous results
			combinations combined_result;
			for(vector<string> & previous_result : branch) {
				for(vector<string> & sub_combination : sub_combinations ) {
					// prefix each inside brackets combination with what was before
					combined_result.push_back(previous_result);
					extend(combined_result.back(), sub_combination);
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

string parse(const vector<string> & tree) {
	string result = "";
	for(vector<string>&x : generateCombinations(tree)) {
		cout << x << endl;
		result += "    current = vector<Token>();\n";
		result += "    if(";
		for(size_t i = 0; i < x.size(); i++) {
			string prefix, suffix;
			/*
			{truc} -> _loop(truc, current)
			<truc> -> __truc(next(current))
			"truc" -> _value("truc", next(current))
			otherwise : INVALID_SYNTAX
			*/
			switch( x[i][0] ){		
				case '{' :
					if(x[i].back() != '}') Error(ErrorType::INVALID_SYNTAX, "Missing '}'").throw_error();
					prefix = "_loop("; suffix = ", current)";
					break;
				case '<' :
					if(x[i].back() != '>') Error(ErrorType::INVALID_SYNTAX, "Missing '>'").throw_error();
					prefix = "__"; suffix = "(next(current))";
					break;
				case '"' :
					if(x[i].back() != '"' || x[i].size() < 2 ) Error(ErrorType::INVALID_SYNTAX, "Missing '\"'").throw_error();
					prefix = "_value(\""; suffix = "\", next(current))";
					break;
				default :
					Error(ErrorType::INVALID_SYNTAX, "Mauvais !").throw_error();
			}
			result += prefix + x[i].substr(1, x[i].size() - 2) + suffix; 

			if(i < x.size() - 1)
				result += " || ";
		}
		result += ") it = t;\n    else {master.children() + current; return 0;};\n";
	}
	return result;
}

void getArg(int argc, char *argv[], string &filename, string &output) {
	for(uint32_t i = 1; i != argc; ++i) {
		if(strcmp(argv[i], "-h") == 0) {
			cout << "Usage: " << endl;
			cout << "  " << argv[0] << " -h" << endl;
			cout << "  " << argv[0] << " -v" << endl;
			cout << "  " << argv[0] << " -f \e[4mfile\e[0m -o \e[4mfile\e[0m [-d]" << endl;
			cout << "Options:" << endl;
			cout << "  -h  Show this help message" << endl;
			cout << "  -v  Show version" << endl;
			cout << "  -f  File name" << endl;
			cout << "  -o  Output file name" << endl;
			exit(0);
		} else if(strcmp(argv[i], "-v") == 0) {
			cout << "Version: 0.0.1" << endl;
			exit(0);
		} else if(strcmp(argv[i], "-f") == 0) {
			i++;
			if(i < argc) {
				filename = argv[i];
				string extension = filename.substr(filename.find_last_of(".") + 1);
				if(extension != "gg") {
					Error err(ErrorType::INVALID_FILE_EXTENSION, ".gg");
					err.throw_error();
				}
			} else {
				Error err(ErrorType::NO_FILENAME_SPECIFIED, "input file");
				err.throw_error();
			}
		} else if(strcmp(argv[i], "-o") == 0) {
			i++;
			if(i < argc) {
				output = argv[i];
				string extension = output.substr(output.find_last_of(".") + 1);
				if(extension != "cpp") {
					Error err(ErrorType::INVALID_FILE_EXTENSION, ".cpp");
					err.throw_error();
				}
			} else {
				Error err(ErrorType::NO_FILENAME_SPECIFIED, "output file");
				err.throw_error();
			}
		} else {
			filename = argv[i];
			string extension = filename.substr(filename.find_last_of(".") + 1);
			if(extension != "gg") {
				Error err(ErrorType::INVALID_FILE_EXTENSION, ".gg");
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

	copy("template/head.cpp.part", out);
	copy("template/Lexeme.cpp.part", out);
	copy("template/Token.cpp.part", out);
	copy("template/lex.cpp.part", out);
}

string createLexemsPart(ofstream &outputFile, ifstream &inputFile, string &outputName, vector<string> &tokens, uint32_t &lineNum) {
	outputFile << "void create_lexemes(vector<Lexeme> &lexemes) {" << endl;

	string line;
	while(getline(inputFile, line)) {
		lineNum++;

		if(line == "") continue; // skip empty line
		if(line[0] == '#') continue; // ingnore comment
		if(line == "---") break; // threat only the first part of the file

		smatch match;
		regex re("^(\\.?[a-zA-Z][a-zA-Z_0-9]*)\\s*\"(([^\"]|\\\\\")*)\"$");

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
		out << "bool __" << token << "(Token &master) { return _type(\"" + token + "\", master); }" << endl;
	}
}

vector<string> loopExpressionGenerator(vector<string>&currentRule, vector<pair<string, string>>&rules, string name) {
	uint32_t j = 0;
	vector<string> newRule;
	for(uint32_t i = 0; i < currentRule.size(); i++) {
		if(currentRule[i] == "{") {
			int level = 1;
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

void createRulesPart(ifstream &file, ofstream &out, uint32_t lineNum, vector<pair<string, string>> &rules){
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
		regex re("^\\s*<([a-zA-Z][a-zA-Z_0-9]*)>\\s*::=\\s*(.+)\\s*$");

		if(regex_search(line, match, re)) { // TODO: make error handling
			string name = match.str(1);
			string expr = match.str(2);
			int i = 0;

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
			out << "bool " << rule.first.substr(1) << "(Token &master);" << endl;
		else
			out << "bool __" << rule.first << "(Token &master);" << endl;
	}
}

void writeRule(vector<pair<string, string>> &rules, ofstream &out) {
	for(auto rule : rules) {
		if(rule.first[0] == '.')
			out << endl << "bool " << rule.first.substr(1) << "(Token &master) {" << endl;
		else
			out << endl << "bool __" << rule.first << "(Token &master) {" << endl;
		out << "    IT t = it;" << endl;
		out << "    master = Token(\"" + rule.first + "\");" << endl;
		out << "    vector<Token> current;" << endl;
		out << rule.second;
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
	uint32_t lineNum = 0;

	copy("template/valueExpression.cpp.part", out);
	string last_line = createLexemsPart(out, file, outputName, tokens, lineNum);
	typeExpressionGenerator(tokens, out);

	if(last_line != "---")
		return 0; // no rules

	vector<pair<string, string>> rules;

	createRulesPart(file, out, lineNum, rules);

	declareRule(rules, out);

	writeRule(rules, out);
	
	out << endl;
	copy("template/main.cpp.part", out);

	file.close();
	out.close();

	return 0;
}
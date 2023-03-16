#include <iostream>
#include <vector>
#include <string>

using namespace std;

template<typename T>

ostream& operator<<(ostream& os, vector<T> const &v) {
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

vector<vector<string>> generateTree(vector<string>&tree) {
    vector<vector<vector<string>>> or_results = {{{}}};
    vector<vector<string>>*result = &or_results[0];
    for(long unsigned int i = 0; i < tree.size(); i++) {
        if(tree[i] == "(") {
            int level = 1;
            vector<string> temp;
            while(level > 0) {
                i++;
                if(tree[i] == "(")
                    level++;
                else if(tree[i] == ")")
                    level--;
                if(level != 0)
                    temp.push_back(tree[i]);
            }

            vector<vector<string>> ors = generateTree(temp);
            vector<vector<string>> temp_result;
            for(long unsigned int j = 0; j < result->size(); j++) {
                for(long unsigned int k = 0; k < ors.size(); k++) {
                    temp_result.push_back((*result)[j]);
                    for(long unsigned int l = 0; l < ors[k].size(); l++) {
                        temp_result[temp_result.size() - 1].push_back(ors[k][l]);
                    }
                }
            }
            *result = temp_result;
        } else if(tree[i] == "|") {
            or_results.push_back({{}});
            result = &or_results[or_results.size() - 1];
        } else {
            for(long unsigned int j = 0; j < result->size(); j++) {
                (*result)[j].push_back(tree[i]);
            }
        }
    }

    vector<vector<string>> final_result;
    for(long unsigned int i = 0; i < or_results.size(); i++) {
        for(long unsigned int j = 0; j < or_results[i].size(); j++) {
            final_result.push_back(or_results[i][j]);
        }
    }
    return final_result;
}

int main() {
    vector<string> tree = {"a", "(", "b", "|", "(", "c", "|", "(", "d", "|", "f", ")", "g", ")", "h", ")", "i"};
    cout << generateTree(tree) << endl;
    return 0;
}

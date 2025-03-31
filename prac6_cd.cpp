#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
using namespace std;

struct TAC {
    string result, op, arg1, arg2;
   
    TAC(string r, string o, string a1, string a2)
        : result(r), op(o), arg1(a1), arg2(a2) {}
   
    string toString() const {
    if (op.empty())
        return result + " = " + arg1;
    return result + " = " + arg1 + " " + op + " " + arg2;
}

};

bool isNumber(const string &s) {
    return !s.empty() && (isdigit(s[0]) || s[0] == '-');
}

string evaluateConstant(const string &a, const string &op, const string &b) {
    int x = stoi(a), y = stoi(b);
    if (op == "+") return to_string(x + y);
    if (op == "-") return to_string(x - y);
    if (op == "*") return to_string(x * y);
    if (op == "/" && y != 0) return to_string(x / y);
    return "";
}

vector<TAC> optimizeTAC(vector<TAC> &tacList) {
    unordered_map<string, string> valueMap;  
    unordered_map<string, string> exprMap;  
    unordered_map<string, bool> usedVars;    

    vector<TAC> optimized;
    bool changed;

    do {
        changed = false;
        optimized.clear();
        valueMap.clear();
        exprMap.clear();
        usedVars.clear();

        for (auto inst : tacList) {
            string r = inst.result, op = inst.op, a1 = inst.arg1, a2 = inst.arg2;

            if (valueMap.find(a1) != valueMap.end()) a1 = valueMap[a1];
            if (valueMap.find(a2) != valueMap.end()) a2 = valueMap[a2];

            if (isNumber(a1) && isNumber(a2) && !op.empty()) {
                string foldedValue = evaluateConstant(a1, op, a2);
                if (!foldedValue.empty()) {
                    valueMap[r] = foldedValue;
                    optimized.push_back(TAC(r, "", foldedValue, ""));
                    changed = true;
                    continue;
                }
            }
            if (op.empty() && isNumber(a1)) {
                valueMap[r] = a1;
                optimized.push_back(TAC(r, "", a1, ""));
                changed = true;
                continue;
            }

            string exprKey = a1 + " " + op + " " + a2;
            if (!op.empty() && exprMap.find(exprKey) != exprMap.end()) {
                valueMap[r] = exprMap[exprKey];
                optimized.push_back(TAC(r, "", exprMap[exprKey], ""));
                changed = true;
                continue;
            } else if (!op.empty()) {
                exprMap[exprKey] = r;
            }

            optimized.push_back(TAC(r, op, a1, a2));
            usedVars[r] = true;
        }

   
        vector<TAC> finalOptimized;
        for (auto &inst : optimized) {
            if (usedVars[inst.result] || inst.result.empty()) {
                finalOptimized.push_back(inst);
            } else {
                changed = true;
            }
        }

        tacList = finalOptimized;
    } while (changed);

    return tacList;
}

vector<TAC> getTACInput() {
    vector<TAC> tacList;
    int n;
    cout << "Enter number of three-address code statements: ";
    cin >> n;
    cin.ignore();

    cout << "Enter the statements in the form: result = arg1 op arg2 OR result = arg1\n";
    for (int i = 0; i < n; i++) {
        string line, result, op, arg1, arg2;
        getline(cin, line);
        stringstream ss(line);
        ss >> result >> op;
        if (op == "=") {
            ss >> arg1;
            if (ss >> op) ss >> arg2;  
            else op = "";
        }
        tacList.push_back(TAC(result, op, arg1, arg2));
    }
    return tacList;
}

void printTAC(const vector<TAC> &tacList) {
    for (const auto &inst : tacList) {
        cout << inst.toString() << endl;
    }
}

int main() {
    vector<TAC> tacList = getTACInput();
   
    cout << "\nOptimizing TAC...\n";
    vector<TAC> optimizedTAC = optimizeTAC(tacList);

    cout << "\nOptimized Three-Address Code:\n";
    printTAC(optimizedTAC);
 
    return 0;
}


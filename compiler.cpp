
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <stack>
#include <cctype>

using namespace std;

bool isSeparator(char ch) {
    string value = " ,;(){}[]";
    for (int i = 0; i < value.length(); i++) {
        if (ch == value[i]) {
            return true;
        }
    }
    return false;
}

bool isOperator(char ch) {
    string value = "+-*/<>=!%";
    for (int i = 0; i < value.length(); i++) {
        if (ch == value[i]) {
            return true;
        }
    }
    return false;
}

bool isNumber(string s) {
    bool valid = true;
    for (int i = 0; i < s.length(); i++) {
        if (((s[i] >= '0') && (s[i] <= '9')) || (s[i] == '.')) {
            continue;
        }
        else {
            valid = false;
            break;
        }
    }
    return valid;
}

bool isKeyword(const string& s) {
    string keyword[] = { "auto", "break", "case", "char", "const", "continue", "default",
                        "do", "double", "else", "enum", "extern", "float", "for", "goto",
                        "if", "int", "long", "register", "return", "short", "signed", "sizeof",
                        "static", "struct", "switch", "typedef", "union", "unsigned",
                        "void", "volatile", "while", "endl", "for", "main", "cout", "cin", "bool" };
    for (int i = 0; i < (sizeof(keyword) / sizeof(string)); i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

bool isValidIdentifier(const string& s) {
    if (s.empty() || isdigit(s[0])) {
        return false;  
    }

    for (char c : s) {
        if (!isalnum(c) && c != '_') {
            return false;  
        }
    }

    return true;
}

bool isValidVariableName(const std::string& variableName) {

    std::regex variableNameRegex("^[a-zA-Z_][a-zA-Z0-9_]*$");

    return std::regex_match(variableName, variableNameRegex);
}

bool isSyntaxCorrect(const string& code) {
    stack<char> brackets;
    stringstream ss(code);
    string token;

    while (ss >> token) {
        for (char c : token) {
            if (c == '(' || c == '[' || c == '{') {
                brackets.push(c);
            }
            else if (c == ')' || c == ']' || c == '}') {
                if (brackets.empty() || !((c == ')' && brackets.top() == '(') ||
                    (c == ']' && brackets.top() == '[') ||
                    (c == '}' && brackets.top() == '{'))) {
                    return false;  
                }
                brackets.pop();
            }
        }
    }

    return brackets.empty(); 
}

void parse(const string& s, bool& syntaxCorrect) {
    vector<string> tokens;
    string token = "";
    bool syntaxError = false;  

    bool isNewLine = false;

    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '\n') {
            
            isNewLine = true;
        }
        else if (isSeparator(s[i])) {
            if (token != "") {
                tokens.push_back(token);
                
                if (isValidIdentifier(token) && !isValidVariableName(token)) {
                    syntaxError = true;
                    cout << '\"' << token << "\" is an invalid variable name" << endl;
                }
            }
            tokens.push_back(string(1, s[i]));  
            token = "";
            isNewLine = false;  
        }
        else if (isOperator(s[i])) {
            if (token != "") {
                tokens.push_back(token);
                
                if (isValidIdentifier(token) && !isValidVariableName(token)) {
                    syntaxError = true;
                    cout << '\"' << token << "\" is an invalid variable name" << endl;
                }
                token = "";
            }
            token.push_back(s[i]);
            tokens.push_back(token);
            token = "";
            isNewLine = false;  
        }
        else {
            token.push_back(s[i]);
            isNewLine = false;  
        }

        if (isNewLine) {
            
            continue;
        }
    }

    if (token != "") {
        tokens.push_back(token);
        
        if (isValidIdentifier(token) && !isValidVariableName(token)) {
            syntaxError = true;
            cout << '\"' << token << "\" is an invalid variable name" << endl;
        }
    }

    for (int i = 0; i < tokens.size(); i++) {
        if (isKeyword(tokens[i])) {
            cout << '\"' << tokens[i] << "\" is a keyword" << endl;
        }
        else if (isNumber(tokens[i])) {
            cout << '\"' << tokens[i] << "\" is a number" << endl;
        }
        else if (isOperator(tokens[i][0])) {
            cout << '\"' << tokens[i] << "\" is an operator" << endl;
        }
        else if (isSeparator(tokens[i][0])) {
            cout << '\"' << tokens[i] << "\" is a separator" << endl;
        }
        else if (isValidIdentifier(tokens[i])) {
            cout << '\"' << tokens[i] << "\" is a valid identifier" << endl;
            if (isValidVariableName(tokens[i])) {
                cout << '\"' << tokens[i] << "\" is a valid variable name" << endl;
            }
            else {
                syntaxError = true;
            }
        }
        else {
            cout << '\"' << tokens[i] << "\" is an Invalid variable name" << endl;
            syntaxError = true;
        }
    }

    syntaxCorrect = !syntaxError;
}

int main() {
    cout << "Enter the code (type 'exit' on a new line to end):\n";

    stringstream buffer;
    string line;

    while (true) {
        getline(cin, line);

        if (line == "exit") {
            break;
        }

        buffer << line << '\n';
    }

    string code = buffer.str();
    bool syntaxCorrect = false;

    parse(code, syntaxCorrect);

    if (syntaxCorrect) {
        if (isSyntaxCorrect(code)) {
            cout << "Syntax is correct." << endl;
        }
    }
    else if (syntaxCorrect == false) {
        cout << "Syntax error: Invalid variable name found." << endl;
    }
    else {
        cout << "Syntax error." << endl;
    }

    return 0;
}

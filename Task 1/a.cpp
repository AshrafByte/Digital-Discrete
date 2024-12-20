
#include <iomanip>
#include <iostream>
#include <stack>
#include <bits/stdc++.h>

#define endl '\n'
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

using namespace std;


vector<char> var;
set<pair<string,char>> varSet;
unordered_map<char, bool> inputs;
bool satisfiable=false;
bool valid=true;
vector<int> padding;

// Check if the symbol is a logical operator
bool isLogicalOperator(char symbol)
{
    return symbol == '&' || symbol == '|' || symbol == '~';
}

// Check if the symbol is a valid input variable
bool isInput(const unordered_map<char, bool> &inputs, char symbol)
{
    return inputs.find(symbol) != inputs.end();
}

//check if the premises vector is true
bool isPremisesTrue(vector<bool> &results){
    for(int i=0; i<results.size()-1;i++){
        if(!results[i])
            return false;
    }
    return true;
}

//checks if the expression is satisfiable or not
bool isSatisfiable(vector<bool> &results){
    for(int i=0; i<results.size()-1;i++){
        if(!results[i])
            return false;
    }
    return true;
}

//checks if the expression is valid or not
bool isvalid(vector<bool> &results){
    for(int i=0; i< results.size()-1;i++){
        if(results[i]==0) return true;
    }
    return results[results.size()-1];
}

// Pop the top element from a stack and return it
template<typename T>T popTop(stack<T> &st)
{
    T top = st.top();
    st.pop();
    return top;
}

// Evaluate a single operation and update the operands stack
void evaluate(stack<char> &operations, stack<bool> &operands)
{
    char operation = popTop(operations);
    switch (operation)
    {
        case '~':
        {
            bool operand = popTop(operands);
            operands.push(!operand);
            break;
        }
        case '&':
        {
            bool right = popTop(operands);
            bool left = popTop(operands);
            operands.push(left && right);
            break;
        }
        case '|':
        {
            bool right = popTop(operands);
            bool left = popTop(operands);
            operands.push(left || right);
            break;
        }
        default:
            cerr << "Error: Unknown operation '" << operation << "' encountered." << endl;
            break;
    }
}

// Main function to evaluate a logical expression
bool evaluateExpression(const unordered_map<char, bool> &inputs, const string &expression)
{
    stack<bool> operands; // Stack to store operands (values)
    stack<char> operations; // Stack to store operators and parentheses

    for (char symbol: expression)
    {
        if (isspace(symbol)) continue; // Skip whitespace

        if (isLogicalOperator(symbol) || symbol == '(')
        {
            // Push operators and opening parentheses
            operations.push(symbol);
        }
        else if (isInput(inputs, symbol))
        {
            // Push input values onto the operands stack
            operands.push(inputs.at(symbol));
        }
        else if (symbol == ')')
        {
            // Evaluate until matching '(' is found
            while (!operations.empty() && operations.top() != '(')
                evaluate(operations, operands);

            if (!operations.empty())
                operations.pop(); // Remove '(' from the stack
        }
    }

    // Evaluate remaining operations
    while (!operations.empty())
        evaluate(operations, operands);

    // Final result should be the only value in the operands stack
    return operands.empty() ? false : popTop(operands);
}

void printResult(vector<bool> &results, string color){
    for(int i=0; i<results.size();i++)
        cout<<color<< setw(padding[i]/2)<< results[i]<< setw(padding[i]-1)<< RESET<< ' ';

}

//prints and makes a vector of the bits of the number
void printNum(int num, int len){
    if(len==0) return;
    printNum(num>>1, len-1);
    inputs[var[len-1]] = num & 1;
    cout<< (num&1)<<' ';
}

void printRow(int num, int n, vector<string> &expressions){
    printNum(num, n);
    vector <bool> results(expressions.size());
    for(int i=0; i<expressions.size();i++){
        results[i] = evaluateExpression(inputs, expressions[i]);
    }
    if(isPremisesTrue(results)){
        if(results[results.size()-1]) printResult(results, GREEN);
        else printResult(results, YELLOW);
    }
    else{
        printResult(results, RED);
    }

    if(!satisfiable) satisfiable = isSatisfiable(results);
    if(valid) valid = isvalid(results);
    cout<<endl;
}

//prints the name of the variables
void printVar(vector<string> &expressions){
    for(char i : var){
        cout<<i<< ' ';
    }
    for(const string& expression:expressions){
        padding.push_back((int)expression.size());
        cout<<expression<<' ';
    }
    cout<<endl;
}

//constructs the whole truth table
void truthTable(vector<string> &expressions){
    printVar(expressions);

    //prints the truth table
    for(int i=0;i< (1<<var.size());i++){
        printRow(i, (int) var.size(), expressions);
    }
    cout<<endl;
}

//reverse the stack
stack<char> rStack(stack<char> &st){
    stack<char> temp;
    while(!st.empty()){
        temp.push(st.top());
        st.pop();
    }
    return temp;
}

//convert stack into string
string stack2string(stack<char> &st){
    string s;
    while(!st.empty()){
        s+=st.top();
        st.pop();
    }
    return s;
}

//print the boolean expression
void printExpression(const string& s){
    for(char c:s)
        cout<<c;
    cout<<endl;
}

//checks if the variable is already present
bool isVar(string s){
    for(char c: s)
        if(varSet.find(make_pair(s, c)) != varSet.end()) return true;
    return false;
}
bool checkVar(char c){
    for(auto it: var){
        if(it == c)
            return true;
    }
    return false;
}
//adds the variable to the set if it's not present
char addName(string s){
    if(isVar(s))
        return s[0];

    int setSize = (int)varSet.size();
    int i=-1;
    while(setSize == varSet.size()){
        i++;
        if(i==s.size())break;
        if(checkVar(s[i])==0) {
            varSet.insert(make_pair(s, s[i]));
        }
    }
    var.push_back(s[i]);
    return s[i];
}

//extracts the expression from the given string
string getExpression(const string& s){
    stack<char> st;

    stringstream stream(s);
    string token;

    //flag to the presence of conditional statement
    bool ifFlag= false;

    //extract boolean algebra
    while(getline(stream, token, ' ')){
        if(token=="if")st.push('('), st.push('~'), st.push('('), ifFlag=true;
        else if(token == "then")st.push(')'), st.push(')'), st.push('|'),st.push('(');
        else if( token == "(")st.push('(');
        else if(token == ")")st.push(')');
        else if(token == "and")st.push('&');
        else if(token == "or")st.push('|');
        else if(token == "not")st.push('~');
        else st.push(addName(token));
    }
    if(ifFlag) st.push(')');

    //reverse the stack
    stack<char> temp = rStack(st);

    //convert to string
    string expression = stack2string(temp);

    //print boolean expression
    printExpression(expression);

    //return the boolean expression
    return expression;
}


int main(){
    string s;
    vector <string> expressions;
    while(getline(cin,s)){
        if(s.empty())break;
        expressions.push_back(getExpression(s));
    }
    truthTable(expressions);
    cout<< "Satisfiable: "<< (satisfiable ? "Yes" : "No")<<endl;
    cout<<"Valid: " << (valid ? "Yes" : "No")<<endl;
    return 0;
}
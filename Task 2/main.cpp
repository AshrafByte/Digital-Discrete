#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <iomanip>
#include <vector>
#include "TruthTable.h"
using namespace std;
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define RESET   "\033[0m"


map<char, bool> readInputs(const string & message);
string readExpression(string const &message);
void printExpression(const string &s);
void printInputs(const map<string, bool> &inputs);
pair<string, string> changeExpression(string expression);
void evaluateExpressionEquivalence(TruthTable &table);
void evaluateExpressionSatisfiability(TruthTable &table, const string &expression, const string &label, bool isOriginal);
void adjustExpressionForSatisfiability(TruthTable &table, map<char, bool> &inputs, string &expression, const string &otherExpression, bool isOriginal);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    // Read inputs and expressions
    map<char, bool> inputs = readInputs("Enter the number of input variables: ");
    string orginalExp = readExpression("Enter the original expression: ");
    string simplifiedExp = readExpression("Enter the simplified expression: ");
    vector<string> expressions = {orginalExp, simplifiedExp};
    cout << endl;

    // Create TruthTable instance and generate table
    TruthTable table(inputs, expressions);
    table.print();

    evaluateExpressionEquivalence(table);

    evaluateExpressionSatisfiability(table, orginalExp, "Original expression", true);
    evaluateExpressionSatisfiability(table, simplifiedExp, "Simplified expression", false);

    adjustExpressionForSatisfiability(table, inputs, orginalExp, simplifiedExp, true);
    adjustExpressionForSatisfiability(table, inputs, simplifiedExp, orginalExp, false);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Method definitions
map<char, bool> readInputs(const string &message)
{
    cout << message;
    int input_size;
    cin >> input_size;

    map<char, bool> inputs;
    for (int i = 0; i < input_size; i++)
    {
        cout << "Enter symbol for input variable " << i + 1 << ": ";
        char symbol;
        cin >> symbol;
        inputs.insert({symbol, false});
    }

    cin.ignore(); // To clear the newline left by previous input
    return inputs;
}

string readExpression(string const &message)
{
    cout << message;
    string expression;
    getline(cin, expression);
    return expression;
}

pair<string, string> changeExpression(string expression)
{
    bool logicExist = false;
    for (auto &symbol: expression)
    {
        if (symbol == '|' || symbol == '&')
        {
            logicExist = true;
            break;
        }
    }

    if (!logicExist)
        return {"Error", expression};

    int index = rand() % (expression.size());
    while (expression[index] != '&' && expression[index] != '|')
        index = rand() % (expression.size());

    char symbol = expression[index];
    string log;
    string newExpression = expression;

    if (expression[index] == '|')
    {
        newExpression[index] = '&';
        log = "changing one OR gate to AND gate, old expression is [ " + expression + " ] new expression is [ " +
              newExpression + " ]\n";;
    } else if (expression[index] == '&')
    {
        newExpression[index] = '|';
        log = "changing one AND gate to OR gate, old expression is [ " + expression + " ] new expression is [ " +
              newExpression + " ]\n";
    }
    return {log, newExpression};
}

void evaluateExpressionEquivalence(TruthTable &table)
{
    if (table.hasEquivalentExpressions())
        cout << "The two logical expressions are " << GREEN << "equivalent.\n" << RESET ;
    else
        cout << "The two logical expressions are " << RED << "not equivalent.\n" << RESET;
    cout << endl;
}

void evaluateExpressionSatisfiability(TruthTable &table, const string &expression, const string &label, bool isOriginal)
{
    if (isOriginal ? table.isOrignalExpSatisfiable() : table.isSimplifiedExpSatisfiable())
    {
        // Output satisfiable message in green
        cout << label << " [" << expression << "] is " << GREEN << "satisfiable" << RESET << " with the following inputs: { ";
        printInputs(isOriginal ? table.getSatisfiableInputsForOrginal() : table.getSatisfiableInputsForSimplified());
        cout << "}\n";
    }
    else
    {
        // Output not satisfiable message in red
        cout << label << " [" << expression << " ] is " << RED << "not satisfiable" << RESET << ".\n";
    }
}

void adjustExpressionForSatisfiability(TruthTable &table, map<char, bool> &inputs, string &expression, const string &otherExpression, bool isOriginal)
{
    while (isOriginal ? !table.isOrignalExpSatisfiable() : !table.isSimplifiedExpSatisfiable())
    {
        auto [log, newExpression] = changeExpression(expression);
        vector<string> expressions = isOriginal ? vector<string>{newExpression, otherExpression}
        : vector<string>{otherExpression, newExpression};
        table.regenrate(inputs, expressions);

        if (isOriginal ? table.isOrignalExpSatisfiable() : table.isSimplifiedExpSatisfiable())
        {
            cout << log << (isOriginal ? "Original" : "Simplified") << " expression [" << newExpression << "] is now "<< GREEN << "satisfiable" << RESET << " with inputs: { ";
            printInputs(isOriginal ? table.getSatisfiableInputsForOrginal() : table.getSatisfiableInputsForSimplified());
            cout << "}\n";
        }
        expression = newExpression; // Update the expression with the modified one
    }
    cout << endl;
}

void printInputs(const map<string, bool> &inputs)
{
    for (const auto &[key,value]: inputs)
        cout << key << ":" << value << "  ";
}
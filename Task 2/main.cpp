#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <iomanip>
#include <vector>

#include "DiscreteLogic.h"
#include "TruthTable.h"
using namespace std;
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define RESET   "\033[0m"


map<char, bool> extractAndValidateVariables(const string & originalExp , const string & simplifiedExp);
string readExpression(string const &message);
void displayWelcomeAndInstructions();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    displayWelcomeAndInstructions();
    // Read inputs and expressions
    string originalExp = readExpression("Enter the original expression: ");
    string simplifiedExp = readExpression("Enter the simplified expression: ");
    cout << endl ;
    // Extract input variables from the original expression and validate that the simplified expression uses the same variables.
    map <char,bool> inputs = extractAndValidateVariables(originalExp,simplifiedExp);
    map <string,string> expressions = {{"Original",originalExp}, {"Simplified",simplifiedExp}};
    cout << endl ;

    DiscreteLogic circuit(originalExp,simplifiedExp);
    circuit.displayTruthTable();
    circuit.evaluateExpressionEquivalence();
    circuit.evaluateExpressionSatisfiability(ExpressionType::Original);
    circuit.evaluateExpressionSatisfiability(ExpressionType::Simplified);
    circuit.adjustExpressionForSatisfiability(ExpressionType::Original);
    circuit.adjustExpressionForSatisfiability(ExpressionType::Simplified);
    circuit.repeat();
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Method definitions
map<char, bool> extractAndValidateVariables(const string & originalExp , const string & simplifiedExp)
{
    map<char, bool> inputVariables;

    for (const auto & symbol : originalExp)
    {
        if (isalpha(symbol))
            inputVariables.emplace(symbol,false);
    }

    for (const auto &symbol : simplifiedExp)
    {
        if (isalpha(symbol) && inputVariables.find(symbol) == inputVariables.end())
        {
            auto getInputSymbolsList = [&inputVariables]() -> string
            {
                string symbolsList;
                for (const auto &entry : inputVariables)
                    symbolsList += entry.first + string(", ");
                if (!symbolsList.empty())
                    symbolsList.pop_back(), symbolsList.pop_back(); // Remove the trailing ", "
                return symbolsList;
            };

            throw runtime_error(
                "\nError: Unexpected input symbol '" + string(1, symbol) + "' found in the simplified expression.\n" +
                "The simplified expression must use the same input symbols (variables) as the original expression.\n" +
                "Original expression input symbols: [" +
                getInputSymbolsList() + "].");
        }
    }
    return inputVariables;
}

string readExpression(string const &message)
{
    cout << message;
    string expression;
    getline(cin, expression);
    return expression;
}

void displayWelcomeAndInstructions() {
    cout << "====================================" << endl;
    cout << "||        Welcome to the           ||" << endl;
    cout << "||       Logic Evaluator           ||" << endl;
    cout << "====================================" << endl;
    cout << endl;

    cout << "This program allows you to evaluate logical expressions using a truth table.\n"
         << "Please follow the instructions below for proper usage:\n" << endl;

    cout << "1. Use the following operators for logical operations:\n";
    cout << "   - '&'  : AND operator\n"
         << "   - '|'  : OR operator\n"
         << "   - '~'  : NOT operator\n" << endl;

    cout << "2. Use valid **single-character** variable names (e.g., A, B, C, ...).\n";
    cout << "   Example: A & B | ~C means (A AND B) OR (NOT C).\n" << endl;

    cout << "3. You can use parentheses '(' and ')' to group expressions and control\n"
         << "   the order of operations.\n";
    cout << "   Example: (A & B) | (~C & D).\n" << endl;

    cout << "====================================" << endl;
}



#include <iostream>
#include <string>
#include <iomanip>
#include "DiscreteLogic.h"
using namespace std;

string readExpression(string const &message);
void displayWelcomeAndInstructions();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    displayWelcomeAndInstructions();
    // Read logical expressions that represent the digital circuit.
    string originalExp = readExpression("Enter the original expression: ");
    string simplifiedExp = readExpression("Enter the simplified expression: ");
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



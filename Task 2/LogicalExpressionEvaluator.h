//
// Created by Hamad on 30/11/2024.
//

#ifndef LOGICALEXPRESSIONEVALUATOR_H
#define LOGICALEXPRESSIONEVALUATOR_H
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <unordered_map>
using namespace std;

class LogicalExpressionEvaluator
{
private:
    std::map<char, bool> inputs; // Member data field to store inputs
    // Check if the symbol is a logical operator
    static bool isLogicalOperator(char symbol);

    // Check if the symbol is a valid input variable
    bool isInput(char symbol);

    // Pop the top element from a stack and return it
    template<typename T>
    T popTop(stack<T> &st);

    // Evaluate a single operation and update the operands stack
    void evaluate(stack<char> &operations, stack<bool> &operands);

    // Process a single symbol in the expression
    void processSymbol(char symbol, stack<char> &operations, stack<bool> &operands);

    static bool isTopPrecedenceHigher(stack<char> &operators, char op);

public:
    // Constructor to initialize inputs map
    explicit LogicalExpressionEvaluator(const map<char, bool> &inputs);

    // Public method to evaluate the logical expression
    bool evaluateExpression(const string &expression);
};

// Constructor
inline LogicalExpressionEvaluator::LogicalExpressionEvaluator(const map<char, bool> &inputs): inputs(inputs) {}

inline bool LogicalExpressionEvaluator::evaluateExpression(const string &expression)
{
    stack<bool> operands; // Stack to store operands (values)
    stack<char> operations; // Stack to store operators and parentheses

    for (char symbol: expression)
        processSymbol(symbol, operations, operands);

    // Evaluate remaining operations
    while (!operations.empty())
        evaluate(operations, operands);

    // The final result should be the only value in the operands stack
    return operands.empty() ? false : popTop(operands);
}

inline void LogicalExpressionEvaluator::processSymbol(char symbol, stack<char> &operations, stack<bool> &operands)
{
    if (isspace(symbol))
        return; // Skip whitespace

    // Push operator or '(' to operations stack
    if (symbol == '(')
        operations.push(symbol);

    else if (isLogicalOperator(symbol))
    {
        if (isTopPrecedenceHigher(operations, symbol))
            evaluate(operations, operands);
        operations.push(symbol);
    }
    // Push the value of the input symbol to operands stack
    else if (isInput(symbol))
        operands.push(inputs.at(symbol));

    else if (symbol == ')')
    {
        // Evaluate until matching '(' is found
        while (!operations.empty() && operations.top() != '(')
            evaluate(operations, operands);

        if (!operations.empty())
            operations.pop(); // Remove '(' from the stack
    }
}

inline void LogicalExpressionEvaluator::evaluate(stack<char> &operations, stack<bool> &operands)
{
    if (operations.empty()) return;

    char operation = popTop(operations);
    switch (operation)
    {
        case '~':
        {
            // NOT operation
            bool operand = popTop(operands);
            operands.push(!operand);
            break;
        }
        case '&':
        {
            // AND operation
            bool right = popTop(operands);
            bool left = popTop(operands);
            operands.push(left && right);
            break;
        }
        case '|':
        {
            // OR operation
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

inline bool LogicalExpressionEvaluator::isLogicalOperator(char symbol)
{
    return symbol == '&' || symbol == '|' || symbol == '~';
}

inline bool LogicalExpressionEvaluator::isInput(char symbol)
{
    return inputs.find(symbol) != inputs.end();
}

template<typename T>
T LogicalExpressionEvaluator::popTop(stack<T> &st)
{
    T top = st.top();
    st.pop();
    return top;
}

inline bool LogicalExpressionEvaluator::isTopPrecedenceHigher(stack<char> &operators, char op)
{
    if (operators.empty())
        return false;

    unordered_map<char, int> precedence = {{'~', 3}, {'&', 2}, {'|', 1}};
    return precedence[operators.top()] > precedence[op];
}


#endif //LOGICALEXPRESSIONEVALUATOR_H

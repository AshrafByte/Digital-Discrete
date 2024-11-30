//
// Created by Hamad on 30/11/2024.
//

#ifndef TRUTHTABLE_H
#define TRUTHTABLE_H
#include <iomanip> // For setw
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "LogicalExpressionEvaluator.h"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define RESET   "\033[0m"
using namespace std;

class TruthTable
{
private:
    map<char, bool> inputs; // Map of input variables
    vector<string> expressions; // List of logical expressions
    vector<string> headers; // Stores input variables and expressions as headers
    vector<vector<bool> > table; // 2D vector to store the truth table values

    int width; // Width for table formatting
    const double padding = 1.5;

    bool areEquivalent;

    int orignalExpIndex;
    int simplifiedExpIndex;

    bool orignalSatisfiable;
    bool simplifiedSatisfiable;

    map<string, bool> satisfiableInputsForOrginal;
    map<string, bool> satisfiableInputsForSimplified;


    // Helper method to calculate the maximum width for formatting
    int getMaxWidth();

    // Generate the truth table data
    void generate();

    // helper method to get inputs that make an expression satisfiable.
    map<string, bool> getSatisfiableInputs(int expressionIndex);

public:
    TruthTable(const map<char, bool> &inputs, const vector<string> &expressions);

    // Print the truth table
    void print();

    bool hasEquivalentExpressions() const { return areEquivalent; }

    map<string, bool> getSatisfiableInputsForOrginal() { return satisfiableInputsForOrginal; }
    map<std::string, bool> getSatisfiableInputsForSimplified() { return satisfiableInputsForSimplified; }

    string getOrginalExpression() { return headers[orignalExpIndex]; }
    string getSimplifiedExpression() { return headers[simplifiedExpIndex]; }

    bool isOrignalExpSatisfiable() const { return orignalSatisfiable; }
    bool isSimplifiedExpSatisfiable() const { return simplifiedSatisfiable; }

    void regenrate(const map<char, bool> &inputs, const vector<string> &expressions);
};

// Constructor implementation
inline TruthTable::TruthTable(const map<char, bool> &inputs, const vector<string> &expressions)
    : inputs(inputs), expressions(expressions), width(getMaxWidth()), areEquivalent(true)
{
    generate();
    orignalExpIndex = headers.size() - 2;
    simplifiedExpIndex = headers.size() - 1;

    satisfiableInputsForOrginal = getSatisfiableInputs(orignalExpIndex);
    satisfiableInputsForSimplified = getSatisfiableInputs(simplifiedExpIndex);

    orignalSatisfiable = !satisfiableInputsForOrginal.empty();
    simplifiedSatisfiable = !satisfiableInputsForSimplified.empty();
}


inline int TruthTable::getMaxWidth()
{
    int max_width = 0;

    // Find the maximum width among variable names and expressions
    for (const auto &entry: inputs)
        max_width = max(max_width, (int) to_string(entry.first).length());

    for (const string &expression: expressions)
        max_width = max(max_width, (int) expression.length());

    return max_width; // Add padding for better readability
}

inline void TruthTable::generate()
{
    int numInputs = inputs.size();
    table.clear(); // Clear any existing data
    headers.clear(); // Reset headers

    // Populate headers with input variables and expressions
    for (const auto &entry: inputs)
        headers.emplace_back(1, entry.first);

    headers.insert(headers.end(), expressions.begin(), expressions.end());

    // Iterate over all possible input combinations
    for (int i = 0; i < (1 << numInputs); i++)
    {
        vector<bool> row;
        auto it = inputs.begin();

        // Assign binary values to inputs
        for (int j = 0; j < numInputs; j++, ++it)
        {
            bool current_bit = (i >> (numInputs - j - 1)) & 1;
            it->second = current_bit;
            row.push_back(current_bit);
        }

        // Evaluate each expression for the current input combination
        LogicalExpressionEvaluator evaluator(inputs);
        for (const string &expression: expressions)
            row.push_back(evaluator.evaluateExpression(expression));

        areEquivalent &= !(row[row.size() - 1] ^ row[row.size() - 2]);
        table.push_back(row); // Add the row to the table
    }
}

inline void TruthTable::print()
{
    // Print headers
    for (const string &header: headers)
        cout << left << setw(width * padding) << header;
    cout << endl;

    // Print rows
    for (const vector<bool> &row: table)
    {
        for (bool value: row)
        {
            if (value)
                cout << "\033[34m" << left << setw(width * padding) << value << "\033[0m"; // Blue for 1
            else
                cout << "\033[31m" << left << setw(width * padding) << value << "\033[0m"; // Red for 0
        }
        cout << endl;
    }
    cout << endl;
}

inline map<string, bool> TruthTable::getSatisfiableInputs(int expressionIndex)
{
    map<string, bool> satisfiableInputs;
    for (auto &row: table)
    {
        bool expValue = row[expressionIndex];
        if (expValue)
        {
            for (int i = 0; i < inputs.size(); i++)
                satisfiableInputs.insert({headers[i], row[i]});
            break;
        }
    }
    return satisfiableInputs;
}

inline void TruthTable::regenrate(const map<char, bool> &inputs, const vector<string> &expressions)
{
    this->inputs = inputs;
    this->expressions = expressions;
    this->width = getMaxWidth();
    this->areEquivalent = true;

    generate();
    orignalExpIndex = headers.size() - 2;
    simplifiedExpIndex = headers.size() - 1;

    satisfiableInputsForOrginal = getSatisfiableInputs(orignalExpIndex);
    satisfiableInputsForSimplified = getSatisfiableInputs(simplifiedExpIndex);

    orignalSatisfiable = !getSatisfiableInputsForOrginal().empty();
    simplifiedSatisfiable = !getSatisfiableInputsForSimplified().empty();
}


#endif //TRUTHTABLE_H

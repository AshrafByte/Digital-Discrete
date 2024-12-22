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

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"

using namespace std;

class TruthTable
{
public:
    // Constructor that initializes the truth table with input variables and logical expressions
    TruthTable(const map<char, bool> &inputs, const map<string, string> &expressions);

    // Prints the generated truth table
    void print();

    // Regenerates the truth table with new inputs and expressions
    void regenerate(const map<char, bool> &inputs, const map<string, string> &expressions);

    [[nodiscard]] const vector<vector<bool>>& getTableValues() const { return table; }

private:
    map<char, bool> inputs; // Map of input variables (e.g., 'A', 'B', 'C')
    map<string, string> expressions; // Map of logical expressions with names as keys
    vector<string> headers; // Stores input variables and expressions as headers
    vector<vector<bool>> table; // 2D vector to store the truth table values
    const double padding = 1.5; // Padding for output formatting

    // Returns the maximum width for formatting the columns in the table
    size_t getMaxWidth() const;

    // Generates the truth table by evaluating all possible input combinations for each expression
    void generate();

    // Initializes the truth table with the provided inputs and expressions
    void initialize(const map<char, bool>& inputs, const map<string, string> &expressions);
};

// Constructor implementation
inline TruthTable::TruthTable(const map<char, bool> &inputs, const map<string, string> &expressions)
{
    initialize(inputs, expressions);
}

// Calculate the maximum width for formatting, based on the longest input and expression names
inline size_t TruthTable::getMaxWidth() const
{
    size_t max_width = 0;

    // Find the maximum width among input variables and expression names
    for (const auto& entry : inputs)
        max_width = max(max_width, size_t(1));  // Input variables are always 1 character

    for (const auto& entry : expressions)
        max_width = max(max_width, entry.first.length());  // Expression names

    return max_width; // Return the maximum width found
}

// Generate the truth table by evaluating all possible combinations of input values
inline void TruthTable::generate()
{
    auto numInputs = inputs.size();
    table.clear(); // Clear any existing data
    headers.clear(); // Reset headers

    // Populate headers with input variables and expressions
    for (const auto& entry : inputs)
        headers.emplace_back(1, entry.first);

    for (const auto& entry : expressions)
        headers.push_back(entry.first);  // Add the expression name (label) to headers

    // Iterate over all possible input combinations (2^numInputs)
    for (int i = 0; i < (1 << numInputs); i++)
    {
        vector<bool> row;
        auto it = inputs.begin();

        // Assign binary values to inputs for the current combination
        for (int j = 0; j < numInputs; j++, ++it)
        {
            bool current_bit = (i >> (numInputs - j - 1)) & 1;
            it->second = current_bit;
            row.push_back(current_bit);
        }

        // Evaluate each expression for the current input combination
        LogicalExpressionEvaluator evaluator(inputs);
        for (const auto& expression : expressions)
            row.push_back(evaluator.evaluateExpression(expression.second)); // Evaluate the expression

        table.push_back(row); // Add the row to the table
    }
}

// Print the truth table
inline void TruthTable::print()
{
    size_t width = getMaxWidth(); // Get the maximum width for formatting

    // Print headers
    for (const string& header : headers)
        cout << left << setw(width * padding) << header;  // Print each header with padding
    cout << endl;

    // Print rows (truth values for each input and expression)
    for (const vector<bool>& row : table)
    {
        for (bool value : row)
        {
            // Print values with color coding (blue for true, red for false)
            if (value)
                cout << BLUE << left << setw(width * padding) << value << RESET;  // Blue for 1
            else
                cout << RED << left << setw(width * padding) << value << RESET;  // Red for 0
        }
        cout << endl;  // New line after each row
    }
    cout << endl;  // New line after the table
}

// Regenerate the truth table with new inputs and expressions
inline void TruthTable::regenerate(const map<char, bool> &inputs, const map<string, string> &expressions)
{
    initialize(inputs, expressions);
}

// Initialize the truth table by setting inputs, expressions, and generating the table
inline void TruthTable::initialize(const map<char, bool>& inputs, const map<string, string> &expressions)
{
    this->inputs = inputs;
    this->expressions = expressions;
    generate(); // Generate the truth table
}

#endif //TRUTHTABLE_H

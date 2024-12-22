#ifndef DISCRETELOGIC_H
#define DISCRETELOGIC_H

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include "TruthTable.h"

using namespace std;

enum class ExpressionType
{
    Original,
    Simplified
};

class DiscreteLogic
{
public:
    DiscreteLogic(string originalExp, string simplifiedExp)
        : originalExp(move(originalExp)), simplifiedExp(move(simplifiedExp))
    {
        this->inputs = extractAndValidateVariables();
        this->table = buildTruthTable();
    }

    void evaluateExpressionEquivalence() const;
    void evaluateExpressionSatisfiability(ExpressionType exprType) const;
    void adjustExpressionForSatisfiability(ExpressionType exprType);
    void displayTruthTable() const {table->print();}
    void repeat() const;

private:
    TruthTable *table; // Truth table object for both expressions
    string originalExp, simplifiedExp;
    map<char, bool> inputs;

    bool isExpressionSatisfiable(ExpressionType exprType, int cnt) const;

    bool areExpressionsEquivalent() const;

    inline void printInputs(ExpressionType exprType) const;

    pair<string, string> changeExpression(ExpressionType exprType) const;

    map<char, bool> extractAndValidateVariables();

    TruthTable *buildTruthTable();

    void modifyExpression(ExpressionType exprType , const string &expression);
    bool  adjustSucceed = false;
};

inline bool DiscreteLogic::areExpressionsEquivalent() const
{
    vector<vector<bool> > tableValues = table->getTableValues();
    size_t simplifiedColumn = tableValues[0].size() - 1;
    size_t originalColumn = tableValues[0].size() - 2;
    for (const auto & row : tableValues)
    {
        if (row[originalColumn] != row[simplifiedColumn])
            return false;
    }
    return true;
}

inline void DiscreteLogic::evaluateExpressionEquivalence() const
{
    if (areExpressionsEquivalent())
        cout << "The two logical expressions are " << GREEN << "Equivalent.\n" << RESET;
    else
        cout << "The two logical expressions are " << RED <<"Not Equivalent.\n" << RESET;
    cout << "\n";
}

inline void DiscreteLogic::evaluateExpressionSatisfiability(ExpressionType exprType) const
{
    string expressionType = (exprType == ExpressionType :: Original) ? "Original" : "Simplified";
    if (isExpressionSatisfiable(exprType , 0))
    {
        cout << expressionType << " expression is " << GREEN <<"Satisfiable" <<RESET<<" with the following inputs: \n";
        printInputs(exprType);
        cout << "\n";
    }
    else
        cout << expressionType << " expression is "<<RED<< "Not Satisfiable." << endl << RESET;
}

inline bool DiscreteLogic::isExpressionSatisfiable(ExpressionType exprType , int cnt) const
{
   string expression = exprType == ExpressionType :: Original ? originalExp : simplifiedExp;
    vector<vector<bool> > tableValues = table->getTableValues();
    size_t offset =  exprType == ExpressionType :: Original ? 2 : 1;
    size_t expressionColumn = tableValues[0].size() -  offset;

    bool tutology = (cnt == 0);
    bool satisfiable = false;
    for (const auto & row : tableValues)
    {
        tutology &= row[expressionColumn];
        satisfiable |= row[expressionColumn];
    }
   return !(tutology || !satisfiable) ;
}

inline void DiscreteLogic::adjustExpressionForSatisfiability(ExpressionType exprType)
{
    if (isExpressionSatisfiable(exprType,0))
        return;
    string expressionType = (exprType == ExpressionType :: Original) ? "Original" : "Simplified";
    string otherExpressionType = (exprType != ExpressionType :: Original) ? "Original" : "Simplified";
    string expression = exprType == ExpressionType::Original ? originalExp : simplifiedExp;
    string otherExpression = exprType != ExpressionType::Original ? originalExp : simplifiedExp;
    map<string, string> expressions;
    int cnt = 0 ;
    while (!isExpressionSatisfiable(exprType , cnt++))
    {
        auto [log, newExpression] = changeExpression(exprType);
        expressions.emplace(expressionType,newExpression);
        expressions.emplace(otherExpressionType,otherExpression);
        table->regenerate(inputs,expressions);
        if (isExpressionSatisfiable(exprType,cnt))
        {
            cout << log << expressionType << " expression [" << newExpression << "] is now "
                    << GREEN << "satisfiable" << RESET << " with inputs: \n";
            printInputs(exprType);
            cout << "\n";
            modifyExpression(exprType,newExpression); // Update the expression with the modified one
            adjustSucceed = true;
            return;
        }
    }
    cout << endl;
}

inline void DiscreteLogic::printInputs(ExpressionType exprType) const
{
    vector<vector<bool> > tableValues = table->getTableValues();
    size_t offset =  exprType == ExpressionType :: Original ? 2 : 1;
    size_t expressionColumn = tableValues[0].size() -  offset;

    for (int i = 0 ; i < tableValues.size() ; i++)
    {
         if(tableValues[i][expressionColumn])
         {
             int cnt = 0;
             cout << "[ ";
            for (auto [variable,value] : inputs)
                cout << variable << ":" << tableValues[i][cnt++] <<" ";
            cout << "]\n";
         }
    }
}

inline pair<string, string> DiscreteLogic::changeExpression(ExpressionType exprType) const
{
    string expression = exprType == ExpressionType::Original ? originalExp : simplifiedExp;
    bool logicExist = false;
    for (const auto &symbol: expression)
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

    string log;
    string newExpression = expression;

    string expressionType = (exprType == ExpressionType :: Original) ? "original" : "simplified";
    if (expression[index] == '|')
    {
        newExpression[index] = '&';
        log = "changing one OR gate to AND gate, old "+ expressionType +" expression is [ " + expression +
            " ] new " + expressionType + " expression is [ " + newExpression + " ]\n";
    } else if (expression[index] == '&')
    {
        newExpression[index] = '|';
        log = "changing one AND gate to OR gate, old "+ expressionType +" expression is [ " + expression +
            " ] new " + expressionType + " expression is [ " + newExpression + " ]\n";
    }
    return {log, newExpression};
}

inline map<char, bool> DiscreteLogic::extractAndValidateVariables()
{
    map<char, bool> inputVariables;

    for (const auto &symbol: originalExp)
    {
        if (isalpha(symbol))
            inputVariables.emplace(symbol, false);
    }

    for (const auto &symbol: simplifiedExp)
    {
        if (isalpha(symbol) && inputVariables.find(symbol) == inputVariables.end())
        {
            auto getInputSymbolsList = [&inputVariables]() -> string {
                string symbolsList;
                for (const auto &entry: inputVariables)
                    symbolsList += entry.first + string(", ");
                if (!symbolsList.empty())
                    symbolsList.pop_back(), symbolsList.pop_back(); // Remove the trailing ", "
                return symbolsList;
            };

            throw runtime_error(
                "Error: Unexpected input symbol '" + string(1, symbol) + "' found in the simplified expression.\n" +
                "The simplified expression must use the same input symbols (variables) as the original expression.\n" +
                "Original expression input symbols: [" +
                getInputSymbolsList() + "].");
        }
    }
    return inputVariables;
}

inline TruthTable* DiscreteLogic::buildTruthTable()
{
    map<string, string> expressions = {{"Original", originalExp}, {"Simplified", simplifiedExp}};
    return new TruthTable(inputs, expressions);
}

inline void DiscreteLogic:: modifyExpression(ExpressionType exprType , const string &expression)
{
    if (exprType == ExpressionType::Original)
        originalExp = expression;
    else
        simplifiedExp = expression;
}

inline void  DiscreteLogic:: repeat() const
{
    if (!adjustSucceed)
        return;
    displayTruthTable();
    evaluateExpressionEquivalence();
}
#endif // DISCRETELOGIC_H

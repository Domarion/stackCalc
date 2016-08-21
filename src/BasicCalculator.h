#pragma once
#include <string>
using std::string;

class BasicCalculator
{
public:
    BasicCalculator();
    bool readInput(const string& inputString);
    void calculate();
    bool hasResult();
    double getResult();
    string getErrorMessage();
private:
    bool validate();
    bool BraketsPairnessCheck(const string &inputString);
    bool checkForWrongSymbols(const string& inputString, const string& regularExpr);

    string toReversePolishNotation(const string& inputString);
    string prettyString;
    string errorString;
    bool hasNormalResult;
    double result;

    bool isDecimalDelimiter(char symbol);
    bool isDigitOrDelimiter(char symbol);
    bool isLeftBracket(char symbol);
    bool isRightBracket(char symbol);
    bool isOperator(char symbol);
    bool isPlusOrMinus(char symbol);

    bool isUnary(char operationChar);
    bool isBinary(char operationChar);

    int getOperationPriority(char operation);


    double computeBinary(double operand1, double operand2, char operation);
    double computeUnary(double operand1, char operation);

    const string binaryOperators="*/+-";
};

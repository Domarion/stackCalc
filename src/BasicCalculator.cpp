#include "BasicCalculator.h"
#include <sstream>
using std::stringstream;

#include <list>
using std::list;

#include <vector>
using std::vector;

#include <regex>
#include <cmath>

#include <algorithm>

BasicCalculator::BasicCalculator()
    :hasNormalResult(false), result(0.0)
{

}

bool BasicCalculator::readInput(const string &inputString)
{

    prettyString = inputString;
    prettyString.erase(std::remove(prettyString.begin(), prettyString.end(), ' '), prettyString.end());//удаление пробелов из строки
    std::replace(prettyString.begin(), prettyString.end(), ',', '.');//замена разделителя десятичной части на точку
    return validate();
}

void BasicCalculator::calculate()
{
    hasNormalResult = false;
    prettyString = toReversePolishNotation(prettyString);

    list<double> operands;
    stringstream stream (prettyString);
    string lexem;
    while (stream >> lexem)
    {
        if (lexem.size() == 1 && isOperator(lexem[0]))
        {
            if (!operands.empty())
            {
               double result{};

               double operand1 = operands.back();
               operands.pop_back();
               if (!isUnary(lexem[0]))
               {
                    double operand2 = operands.back();
                    operands.pop_back();
                    try
                    {
                        result = computeBinary(operand2, operand1, lexem[0]);
                    }
                    catch(std::invalid_argument & exception)
                    {
                        errorString = exception.what();
                        return;
                    }
               }
               else
                   result = computeUnary(operand1, lexem[0]);

               operands.push_back(result);
            }
        }
        else
        {
            try
            {
                double operand = std::stod(lexem);
                operands.push_back(operand);
            }
            catch(std::invalid_argument & exception)
            {
                errorString = exception.what();
                return;
            }
        }

    }

    hasNormalResult = true;
    result = round(operands.back() * 100) / 100;
}

bool BasicCalculator::hasResult()
{
    return hasNormalResult;
}

double BasicCalculator::getResult()
{
    return result;
}

string BasicCalculator::getErrorMessage()
{
    return errorString;
}

bool BasicCalculator::validate()
{
    errorString.clear();

    static const list<string> wrongInputChecks =
    {
        "[^0-9 ()*/+,.-]", //Недопустимые символы
        "[*/+,.-]{2,}", //Несколько подряд идущих знаков операций и/или разделителей десятичной части
        "(\\)[0-9.,]{1,}})|(\\([.,*/]{1,})", //Числа после закрывающейся скобки, знаки операций после открывающейся
        "(\\(\\))|(\\)\\()", //Пустые скобки () или )(
        "[^0-9]{1}[.][^0-9]{1}", //Не числа рядом с разделяющей точкой
        "[0-9]{1,}[.][0-9]{1,}[.]", //Число с несколькими разделяющими точками
        "[.][0-9]{3,}", //Количество знаков после разделительной точки больше двух
        "([^0-9)][*/+-][^0-9(])|([*/+-]($|\\)))|(^[*/])" //Операции без наличия необходимых операндов

    };


    bool validationResult = !prettyString.empty() && BraketsPairnessCheck(prettyString);

    if (validationResult)
        for(auto wrongInputCheck : wrongInputChecks)
            if (!checkForWrongSymbols(prettyString, wrongInputCheck))
                return false;

    return validationResult;
}

string BasicCalculator::toReversePolishNotation(const string &inputString)
{
    size_t stringSize = inputString.size();

    string outputString;
    outputString.reserve(stringSize);

    list<char> operatorStack;

    for(size_t characterIndex = 0; characterIndex < stringSize; ++characterIndex)
    {
        char character = inputString[characterIndex];

        if (isDigitOrDelimiter(character))
        {
            if (characterIndex > 0 && !isDigitOrDelimiter(inputString[characterIndex - 1]))
                outputString.push_back(' ');//вновь добавляются пробелы для удобства разбития на лексемы в дальнейшем

            outputString.push_back(character);
        }
        else
        {
            if (isLeftBracket(character))
            {
                operatorStack.push_back(character);
            }
            else
                if (isRightBracket(character)) //Если символ - правая скобка, то в выходную строку выталкиваются символы операторов до первой левой скобки.
                {

                    while (!operatorStack.empty() && !isLeftBracket(operatorStack.back()))
                    {
                        outputString.push_back(' ');
                        outputString.push_back(operatorStack.back());
                        operatorStack.pop_back();
                    }

                    if (!operatorStack.empty() && isLeftBracket(operatorStack.back()))
                        operatorStack.pop_back();

                }
                else
                    if (isOperator(character))
                    {

                        if (isPlusOrMinus(character) && ( characterIndex == 0 || isLeftBracket(inputString[characterIndex - 1])))
                            character = (character == '+')? 'p': 'm'; //вводим новые обозначения для унарных плюсов и минусов, чтоб отличать их от бинарных


                        while (!operatorStack.empty() && getOperationPriority(character) <= getOperationPriority(operatorStack.back()))
                        {

                            if (characterIndex > 0)
                                outputString.push_back(' ');

                            outputString.push_back(operatorStack.back());
                            operatorStack.pop_back();
                        }

                        operatorStack.push_back(character);
                    }
        }
    }

    while (!operatorStack.empty())//выталкиваем из стека в выходную строку оставшиеся операторы
    {
        outputString.push_back(' ');
        outputString.push_back(operatorStack.back());
        operatorStack.pop_back();
    }

    return outputString;
}

bool BasicCalculator::BraketsPairnessCheck(const string& inputString)
{
    int normalBrackets = 0;
    for (size_t i = 0; i < inputString.size(); ++i)
    {
        if (isLeftBracket(inputString[i]))
            ++normalBrackets;
        else
            if (isRightBracket(inputString[i]))
                --normalBrackets;
    }
    bool result = (normalBrackets == 0);

    if (!result)
        errorString += "некорректный ввод, в строке присутствуют непарные скобки\n";

    return result;
}

bool BasicCalculator::checkForWrongSymbols(const string& inputString, const string& regularExpr)
{
    const std::regex regularPOSIX(regularExpr);

    auto words_begin = std::sregex_iterator(inputString.begin(), inputString.end(), regularPOSIX);
    auto words_end = std::sregex_iterator();
    if (words_begin != words_end)
    {
        errorString += "некорректный ввод, найдены недопустимые выражения";
        for(std::sregex_iterator word_iterator = words_begin; word_iterator != words_end; ++word_iterator)
        {
            std::smatch match = *word_iterator;
            errorString += " " + match.str();
        }
        errorString += "\n";
        return false;
    }
    return true;
}



bool BasicCalculator::isDecimalDelimiter(char symbol)
{
    return (symbol == ',' || symbol == '.');
}

bool BasicCalculator::isDigitOrDelimiter(char symbol)
{
    return isdigit(symbol) || isDecimalDelimiter(symbol);

}

bool BasicCalculator::isLeftBracket(char symbol)
{
    return symbol == '(';
}
bool BasicCalculator::isRightBracket(char symbol)
{
    return symbol == ')';
}

bool BasicCalculator::isOperator(char symbol)
{
    return (isBinary(symbol) || isUnary(symbol));
}

bool BasicCalculator::isPlusOrMinus(char symbol)
{
    return (symbol == '+' || symbol == '-');
}

int BasicCalculator::getOperationPriority(char operation)
{
    const int defaultPriority{-1};

    static const string operations="pm*/+-()";

    static vector<int> priorities;
    if (priorities.empty())
    {
        priorities.resize(operations.size());
        size_t currentPriority = operations.size();
        for(size_t index = 0 ; index < priorities.size() - 1; ++index)
        {
            priorities[index] = priorities[index + 1] = currentPriority;
            --currentPriority;
        }
    }

    for(size_t index = 0; index < operations.size(); ++index)
    {
        if(operations[index] == operation)
            return priorities[index];
    }

    return defaultPriority;
}

bool BasicCalculator::isUnary(char operationChar)
{
    return operationChar == 'm' || operationChar == 'p';
}

bool BasicCalculator::isBinary(char operationChar)
{
    return  binaryOperators.find(operationChar) != string::npos;
}


double BasicCalculator::computeBinary(double operand1, double operand2, char operation)
{
    double result{};
    const double EPS2 = 5e-3;
    switch(operation)
    {
    case '*':
    {
        result = operand1 * operand2;
        break;
    }
    case '/':
    {
        if (fabs(operand2) < EPS2)
            throw std::invalid_argument("деление на ноль запрещено!");

        result = operand1 / operand2;
        break;
    }
    case '+':
    {
        result = operand1 + operand2;
        break;
    }
    case '-':
    {
        result = operand1 - operand2;
        break;
    }
    }
    return result;
}
double BasicCalculator::computeUnary(double operand1, char operation)
{
    return (operation == 'm')? -operand1 : operand1;
}

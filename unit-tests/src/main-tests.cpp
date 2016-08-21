#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;

#include "../../src/BasicCalculator.h"

void ValidationTest(BasicCalculator& aCalc);
void CalculationTest(BasicCalculator& aCalc);
const string Passed = "Test Passed";
const string Failed = "Test Failed";


void ValidationTest(BasicCalculator& aCalc)
{
    const vector<string> cases =
    {
        "",
        "(25*2))-(31-22)"
        "4343 / 2 + abc",
        "4+22 ()",
        ")(",
        "(*5-)",
        "*7 - 4+",
        "4.99999",
        "2.3.44.1",
        "..1",

    };

    size_t failedTests{0};
    size_t passedTests{0};

    cout << "ValidationTest" << endl;
    
    for(auto inputString : cases)
    {
        bool validationPassed = aCalc.readInput(inputString);
        cout << "Case " << inputString << ": " << (validationPassed? (++failedTests, Failed) : (++passedTests, Passed)) << endl;

    }
    size_t testsCount = failedTests + passedTests;

    cout << endl << "Tests count: " << testsCount << " Failed: " << failedTests << " Passed: " << passedTests << endl;
}

void CalculationTest(BasicCalculator& aCalc)
{
    const vector<string> cases =
    {
        "-1 + 5 - 3",
        "-10 + (8 * 2.5) - (3 / 1.5)",
        "1 + (2 * (2.5 + 2.5 + (3 - 2))) - (3 / 1.5)"
    };

    const vector<double> expectedResults =
    {
        1,
        8,
        11
    };

    size_t failedTests{0};
    size_t passedTests{0};
    cout << endl << "CalculationTest" << endl;
    size_t caseCount = cases.size();
    for(size_t caseIndex = 0; caseIndex < caseCount; ++caseIndex)
    {
        bool validationPassed = aCalc.readInput(cases[caseIndex]);
        cout << "Case " << cases[caseIndex] << ": ";

        if (!validationPassed)
        {
            cout << "Validation fault" << endl;
            ++failedTests;
        }
        else
        {
            aCalc.calculate();
            double calcResult = aCalc.getResult();

            cout << " Expected result: " << expectedResults[caseIndex] << " Result: " << calcResult;
            cout << " " << ((expectedResults[caseIndex] == calcResult)? (++passedTests, Passed):(++failedTests, Failed)) << endl;
        }

    }

    cout << endl << "Tests count: " << caseCount << " Failed: " << failedTests << " Passed: " << passedTests << endl;
}



int main()
{
    BasicCalculator calculator;
    ValidationTest(calculator);
    CalculationTest(calculator);
    return 0;
}

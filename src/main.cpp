#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>
#include <cmath>
#include "BasicCalculator.h"

int main()
{
   string inputString;

   do
   {
       cout << "Введите выражение(quit для выхода): ";

       getline(std::cin, inputString);

       cout << "Входное выражение: " << inputString << endl;

       if (inputString == "quit")
           break;

       BasicCalculator calculator;

       cout << "Результат равен: ";

       if (calculator.readInput(inputString))
       {
           calculator.calculate();

           if (calculator.hasResult())
           {
               double result = calculator.getResult();

               double fractionalPart{};
               if (std::modf(result, &fractionalPart) == 0.0)//хак для правильного отображения целочисленных переменных
                    cout << static_cast<int>(result) << endl;
               else
               {
                    cout.precision(2);
                    cout << std::fixed << result << endl;
               }
           }
       }

       cout << calculator.getErrorMessage() << endl;

   }while(true);

    return 0;

}

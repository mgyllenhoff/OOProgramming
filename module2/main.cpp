#include <iostream>

#include "Statistic.h"

// TO RUN: g++ main.cpp Statistic.cpp -o program && ./program

// function that runs functions of Statistic.cpp
int main()
{
    // APPROACH 1: Adding sequence of numbers using the add function
    // Numbers from my Discussion 1 post
    std::vector<double> discussionNumbers = {3, 80, 83, 24, 81, 57, 16, 66, 38,
                                             64, 36, 33, 48, 70, 65, 86, 81, 89,
                                             14, 45};
    Statistic stat1;
    for (int number : discussionNumbers)
    {
        stat1.add(number);
    }
    std::cout << "Average: " << stat1.average()
              << " (expected: 53.95)" << std::endl;
    std::cout << "Standard Deviation: " << stat1.STD()
              << " (expected: 26.69)" << std::endl;
    std::cout << "\n";

    // APPROACH 2: Adding sequence of numbers using the constructor
    std::vector<double> numbers1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Statistic stat2(numbers1);
    std::cout << "Average: " << stat2.average()
              << " (expected: 5.5)" << std::endl;
    std::cout << "Standard Deviation: " << stat2.STD()
              << " (expected: 3.03)" << std::endl;
    std::cout << "\n";

    // Edge case: empty array
    Statistic stat3;
    std::cout << "Average: " << stat3.average()
              << " (expected: warning message)" << std::endl;
    std::cout << "Standard Deviation: " << stat3.STD()
              << " (expected: warning message)" << std::endl;
    std::cout << "\n";

    // Edge case: minimum numbers
    std::vector<double> numbers2 = {5, 5};
    Statistic stat4(numbers2);
    std::cout << "Average: " << stat4.average()
              << " (expected: 5.0)" << std::endl;
    std::cout << "Standard Deviation: " << stat4.STD()
              << " (expected: 0.0)" << std::endl;
    std::cout << "\n";

    return 0;
} // end function main
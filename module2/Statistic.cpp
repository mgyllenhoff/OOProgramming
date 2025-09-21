#include <iostream>

#include "Statistic.h"

// Default constructor
Statistic::Statistic() : numberSequence{} {}

// Constructor that takes a vector of numbers a the input
Statistic::Statistic(std::vector<double> numbers) : numberSequence(numbers) {}

// function that adds an item to the statistics
void Statistic::add(double x)
{
    this->numberSequence.push_back(x);
} // end function add

// function that gets the average
double Statistic::average() const
{
    // Checks that there is enough data to calculate the average
    if (this->numberSequence.empty())
    {
        std::cerr << ("No data exists!");
    }

    // Calculate the average
    double sum = 0.0;
    for (double number : this->numberSequence)
    {
        sum += number;
    }

    return sum / this->numberSequence.size();
} // end function average

// function that gets the standard deviation (STD)
double Statistic::STD() const
{
    // Checks that there is enough data to calculate the STD
    if (this->numberSequence.size() < 2)
    {
        std::cerr << ("Not enough data exists! ");
    }

    // Calculate the STD
    double sum = 0.0;
    double sumSquared = 0.0;
    for (double number : this->numberSequence)
    {
        sum += number;
        sumSquared += number * number;
    }
    double N = static_cast<double>(this->numberSequence.size());

    return std::sqrt((sumSquared - (sum * sum) / N) / (N - 1));
} // end function STD
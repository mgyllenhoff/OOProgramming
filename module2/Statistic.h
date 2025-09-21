#include <vector>

// Statistic class definition
class Statistic
{
public:
    // Constructors: initialize private data
    Statistic();                                     // default constructor
    explicit Statistic(std::vector<double> numbers); // initializes numbers

    // add an item to the statistics
    void add(double x);

    // get average
    double average() const;

    // get Standard deviation: using the hint
    double STD() const;

private:
    std::vector<double> numberSequence;
};
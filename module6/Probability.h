#ifndef PROBABILITY_H
#define PROBABILITY_H

#include <stdexcept>

class Probability {
  public:
    // Constructor from double
    explicit Probability(double v = 0.0);

    // Assignment from double
    Probability& operator=(double v);

    // Getter
    // Not needed since I made it so probability can be used as double!
    // double getProbability() const;

    // Probability can be used anywhere as a double
    operator double() const;

    // Canonical assignment operators
    Probability& operator&=(const Probability& rhs);
    Probability& operator|=(const Probability& rhs);
    Probability& operator^=(const Probability& rhs);
    Probability& operator-=(const Probability& rhs);

    // Unary operator
    Probability operator~() const;

  private:
    double value;

    // Helper to validate values
    static double checkProbability(double v);
};

// Binary operators as non-member functions
Probability operator&(const Probability& lhs, const Probability& rhs);
Probability operator|(const Probability& lhs, const Probability& rhs);
Probability operator^(const Probability& lhs, const Probability& rhs);
Probability operator-(const Probability& lhs, const Probability& rhs);

#endif 

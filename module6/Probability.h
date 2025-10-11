#ifndef PROBABILITY_H
#define PROBABILITY_H

#include <stdexcept>

// Binary operators as non-member functions
Probability operator&(const Probability& lhs, const Probability& rhs);
Probability operator|(const Probability& lhs, const Probability& rhs);
Probability operator^(const Probability& lhs, const Probability& rhs);
Probability operator-(const Probability& lhs, const Probability& rhs);

class Probability {
  public:
    // Constructor from double
    explicit Probability(double v = 0.0);

    // Assignment from double
    Probability& operator=(double v);

    // Getter
    double getProbability() const;

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

#endif 

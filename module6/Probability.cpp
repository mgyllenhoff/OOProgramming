#include "Probability.h"

// Helper funciton to make sure probability is valid
double Probability::checkProbability(double v) {
  if (v < 0.0 || v > 1.0) {
    throw std::out_of_range("Probability must be between 0.0 and 1.0");
  }
  return v;
}

// Constructor
// Set object to 0.0 if probability is not defined
Probability::Probability(double v) : value(checkProbability(v)) {}

// Assignment from double
Probability& Probability::operator=(double v) {
  value = checkProbability(v);
  return *this;
}

// Getter
double Probability::getProbability() const { 
  return value; 
}

Probability::operator double() const { 
  return value; 
}

// Canonical function to help &
Probability& Probability::operator&=(const Probability& rhs) {
  value = checkProbability(value * rhs.value);
  return *this;
}

// Canonical function to help |
Probability& Probability::operator|=(const Probability& rhs) {
  value = checkProbability(value + rhs.value - (value * rhs.value));
  return *this;
}

// Canonical function to help ^
Probability& Probability::operator^=(const Probability& rhs) {
  // Could have used | and & in equation but did not want more overhead
  value = checkProbability(value + rhs.value - 2 * (value * rhs.value));
  return *this;
}

// Canonical function to help -
Probability& Probability::operator-=(const Probability& rhs) {
  value = checkProbability(value * (1.0 - rhs.value));
  return *this;
}

// Probability of both A and B occurring (A & B)
Probability operator&(const Probability& lhs, const Probability& rhs) {
  Probability temp(lhs);
  temp &= rhs;
  return temp;
}

// Probability of either A or B occurring (A | B)
Probability operator|(const Probability& lhs, const Probability& rhs) {
  Probability temp(lhs);
  temp |= rhs;
  return temp;
}

// Probability of either A or B but not both occurring (A ^ B)
Probability operator^(const Probability& lhs, const Probability& rhs) {
  Probability temp(lhs);
  temp ^= rhs;
  return temp;
}

// Probability of A but not B occurring (A - B)
Probability operator-(const Probability& lhs, const Probability& rhs) {
  Probability temp(lhs);
  temp -= rhs;
  return temp;
}

// Probability of A not occurring (~A)
Probability Probability::operator~() const {
  return Probability(1.0 - value);
}
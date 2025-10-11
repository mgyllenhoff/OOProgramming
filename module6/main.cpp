#include <iostream>
#include "Probability.h"

// Main function used for testing
// Run using: g++ main.cpp Probability.cpp -o program && ./program
int main() {
  try {
    std::cout << "EXAMPLE 1:" << std::endl;
    Probability A(0.7);
    Probability B(0.3);

    std::cout << "A = " << A << "\n";
    std::cout << "B = " << B << "\n";

    std::cout << "A & B = " << (A & B) << "\n";
    std::cout << "A | B = " << (A | B) << "\n";
    std::cout << "A ^ B = " << (A ^ B) << "\n";
    std::cout << "A - B = " << (A - B) << "\n";
    std::cout << "~A = "   << (~A) << "\n";

    std::cout << "\nEXAMPLE 2:" << std::endl;
    Probability C;
    C = 1.0;
    Probability D;
    D = 0.0;

    std::cout << "C = " << C << "\n";
    std::cout << "D = " << D << "\n";

    std::cout << "C & D = " << (C & D) << "\n";
    std::cout << "C | D = " << (C | D) << "\n";
    std::cout << "C ^ D = " << (C ^ D) << "\n";
    std::cout << "C - D = " << (C - D) << "\n";
    std::cout << "~C = "   << (~C) << "\n";

    // Invalid value
    std::cout << "\nEXAMPLE 3:" << std::endl;
    Probability E(-0.6);
  }
  catch (const std::out_of_range& e) {
    std::cerr << "Error: " << e.what() << "\n";
  }

  return 0;
}
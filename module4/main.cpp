#include <iostream>
#include "PokerHand.h"

// Main function used for testing
int main() {
  // Straight Flush
  std::cout << "Expected: Straight Flush \nRecevied: ";
  PokerHand hand1("9C 8C JC TC 7C");
  std::cout << "\n";

  // Four of a Kind
  std::cout << "Expected: Four of a Kind \nRecevied: ";
  PokerHand hand2("5S 5D 2D 5H 5C");
  std::cout << "\n";

  // Full House
  std::cout << "Expected: Full House \nRecevied: ";
  PokerHand hand3("6S KS 6H 6D KH");
  std::cout << "\n";

  // Flush
  std::cout << "Expected: Flush \nRecevied: ";
  PokerHand hand4("JD 8D 4D 9D 3D");
  std::cout << "\n";

  // Straight
  std::cout << "Expected: Straight \nRecevied: ";
  PokerHand hand5("TD 9S 8H 7D 6S");
  std::cout << "\n";

  // Three of a Kind
  std::cout << "Expected: Three of a Kind \nRecevied: ";
  PokerHand hand6("QC 2S QS QH 9H");
  std::cout << "\n";

  // Two Pair
  std::cout << "Expected: Two Pair \nRecevied: ";
  PokerHand hand7("JH JS 3C 3S 2H");
  std::cout << "\n";

  // One Pair
  std::cout << "Expected: One Pair \nRecevied: ";
  PokerHand hand8("TS TH 8S 7H 4C");
  std::cout << "\n";

  // High Card
  std::cout << "Expected: High Card \nRecevied: ";
  PokerHand hand9("KD QD 7S 4S 3H");
  std::cout << "\n";

  // Comparing hands
  hand1.determineRank();
  std::cout << "vs" << std::endl;
  hand2.determineRank();
  PokerHand::compare(hand1, hand2);
  std::cout << "\n";

  hand6.determineRank();
  std::cout << "vs" << std::endl;
  hand6.determineRank();
  PokerHand::compare(hand6, hand6);
  std::cout << "\n";

  hand9.determineRank();
  std::cout << "vs" << std::endl;
  hand3.determineRank();
  PokerHand::compare(hand9, hand3);
  std::cout << "\n";

  return 0;
}
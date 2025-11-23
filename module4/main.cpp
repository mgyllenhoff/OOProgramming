#include "PokerHand.h"
#include "PokerGame.h"
#include <iostream>

// Helper function for testing
void printHandResult(int result) {
  if (result == 0) {
    std::cout << "It is a tie" << std::endl;
  } else {
    std::cout << "Hand #" << result << " wins" << std::endl;
  }
}

// Main function used for testing
int main() {
  std::cout << "TEST COMPARISONS FROM THE ASSIGNMENT" << std::endl;
  PokerGame game1;

  // Royal Flush
  PokerHand royalFlush1("AS KS QS JS TS");
  PokerHand straightFlush0("8H 7H 6H 5H 4H");
  printHandResult(game1.compareHandsReturn(royalFlush1, straightFlush0));
  std::cout << std::endl;

  // Straight Flush
  PokerHand straightFlush1("TC 9C 8C 7C 6C");
  PokerHand straightFlush2("8H 7H 6H 5H 4H");
  PokerHand straightFlush3("6S 5S 4S 3S 2S");
  PokerHand straightFlush4("7D 6D 5D 4D 3D");
  PokerHand straightFlush5("7S 6S 5S 4S 3S");
  printHandResult(game1.compareHandsReturn(straightFlush1, straightFlush2));
  printHandResult(game1.compareHandsReturn(straightFlush2, straightFlush3));
  printHandResult(game1.compareHandsReturn(straightFlush4, straightFlush5));
  std::cout << std::endl;

  // Four of a Kind
  PokerHand fourKind1("KS KH KC KD 3H");
  PokerHand fourKind2("7H 7D 7S 7C QH");
  PokerHand fourKind3("7H 7D 7S 7C TS");
  PokerHand fourKind4("4C 4S 4D 4H 9C");
  PokerHand fourKind5("4C 4S 4D 4H 9D");
  printHandResult(game1.compareHandsReturn(fourKind1, fourKind2));
  printHandResult(game1.compareHandsReturn(fourKind2, fourKind3));
  printHandResult(game1.compareHandsReturn(fourKind4, fourKind5));
  std::cout << std::endl;

  // Full House
  PokerHand fullHouse1("8S 8D 8H 7D 7C");
  PokerHand fullHouse2("4D 4S 4C 9D 9C");
  PokerHand fullHouse3("4D 4S 4C 5C 5D");
  PokerHand fullHouse4("KC KS KD JC JS");
  PokerHand fullHouse5("KC KH KD JC JH");
  printHandResult(game1.compareHandsReturn(fullHouse1, fullHouse2));
  printHandResult(game1.compareHandsReturn(fullHouse2, fullHouse3));
  printHandResult(game1.compareHandsReturn(fullHouse4, fullHouse5));
  std::cout << std::endl;

  // Flush
  PokerHand flush1("KD JD 9D 6D 4D");
  PokerHand flush2("QC JC 7C 6C 5C");
  PokerHand flush3("JH TH 9H 4H 2H");
  PokerHand flush4("JS TS 8S 6S 3S");
  PokerHand flush5("JH TH 8H 4H 3H");
  PokerHand flush6("JC TC 8C 4C 2C");
  PokerHand flush7("TD 8D 7D 6D 5D");
  PokerHand flush8("TS 8S 7S 6S 5S");
  printHandResult(game1.compareHandsReturn(flush1, flush2));
  printHandResult(game1.compareHandsReturn(flush2, flush3));
  printHandResult(game1.compareHandsReturn(flush3, flush4));
  printHandResult(game1.compareHandsReturn(flush4, flush5));
  printHandResult(game1.compareHandsReturn(flush5, flush6));
  printHandResult(game1.compareHandsReturn(flush7, flush8));
  std::cout << std::endl;

  // Straight
  PokerHand straight1("JH TH 9C 8S 7H");
  PokerHand straight2("TS 9S 8C 7H 6S");
  PokerHand straight3("6C 5S 4H 3S 2D");
  PokerHand straight4("9C 8C 7C 6D 5D");
  PokerHand straight5("9S 8S 7S 6H 5H");
  printHandResult(game1.compareHandsReturn(straight1, straight2));
  printHandResult(game1.compareHandsReturn(straight2, straight3));
  printHandResult(game1.compareHandsReturn(straight4, straight5));
  std::cout << std::endl;

  // Three of a Kind
  PokerHand threeKind1("6H 6D 6S QC 4S");
  PokerHand threeKind2("3D 3S 3C KS 2S");
  PokerHand threeKind3("3D 3S 3C JC 7H");
  PokerHand threeKind4("3D 3S 3C JS 5D");
  PokerHand threeKind5("9S 9H 9D TD 8H");
  PokerHand threeKind6("9C 9S 9H TD 8D");
  printHandResult(game1.compareHandsReturn(threeKind1, threeKind2));
  printHandResult(game1.compareHandsReturn(threeKind2, threeKind3));
  printHandResult(game1.compareHandsReturn(threeKind3, threeKind4));
  printHandResult(game1.compareHandsReturn(threeKind5, threeKind6));
  std::cout << std::endl;

  // Two Pair
  PokerHand twoPair1("TD TS 2S 2C KC");
  PokerHand twoPair2("5C 5S 4D 4H TH");
  PokerHand twoPair3("5C 5S 3C 3D QS");
  PokerHand twoPair4("5C 5S 3C 3D JS");
  PokerHand twoPair5("KD KS 7D 7H 8H");
  PokerHand twoPair6("KC KS 7C 7H 8C");
  printHandResult(game1.compareHandsReturn(twoPair1, twoPair2));
  printHandResult(game1.compareHandsReturn(twoPair2, twoPair3)); // WRONG
  printHandResult(game1.compareHandsReturn(twoPair3, twoPair4));
  printHandResult(game1.compareHandsReturn(twoPair5, twoPair6));
  std::cout << std::endl;

  // One Pair
  PokerHand onePair1("9C 9D QS JH 5H");
  PokerHand onePair2("6D 6H KS 7H 4C");
  PokerHand onePair3("6D 6H QH JS 2C");
  PokerHand onePair4("6D 6H QS 8C 7D");
  PokerHand onePair5("6D 6H QD 8H 3S");
  PokerHand onePair6("8S 8D TH 6C 5S");
  PokerHand onePair7("8H 8C TC 6S 5C");
  printHandResult(game1.compareHandsReturn(onePair1, onePair2));
  printHandResult(game1.compareHandsReturn(onePair2, onePair3));
  printHandResult(game1.compareHandsReturn(onePair3, onePair4));
  printHandResult(game1.compareHandsReturn(onePair4, onePair5));
  printHandResult(game1.compareHandsReturn(onePair6, onePair7));
  std::cout << std::endl;

  // High Card
  PokerHand highCard1("KS 6C 5H 3D 2C");
  PokerHand highCard2("QS JD 6C 5H 3C");
  PokerHand highCard3("QS TD 8C 7D 4S");
  PokerHand highCard4("QH TH 7C 6H 4S");
  PokerHand highCard5("QC TC 7D 5C 4D");
  PokerHand highCard6("QH TD 7S 5S 2H");
  PokerHand highCard7("TC 8S 7S 6H 4D");
  PokerHand highCard8("TD 8D 7S 6C 4C");
  printHandResult(game1.compareHandsReturn(highCard1, highCard2));
  printHandResult(game1.compareHandsReturn(highCard2, highCard3));
  printHandResult(game1.compareHandsReturn(highCard3, highCard4));
  printHandResult(game1.compareHandsReturn(highCard4, highCard5));
  printHandResult(game1.compareHandsReturn(highCard5, highCard6));
  printHandResult(game1.compareHandsReturn(highCard7, highCard8));
  std::cout << std::endl;

  std::cout << "PLAY A GAME" << std::endl;
  PokerGame game2;
  game2.setup();

  char again = 'y';
  while (again == 'y' || again == 'Y') {
      game2.playRound();
      std::cout << "\nPlay another round? (y/n): ";
      std::cin >> again;
  }

  return 0;
}
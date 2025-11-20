#include <iostream>
#include "PokerHand.h"
#include "PokerGame.h"

// Main function used for testing
int main() {
  // // Royal Flush
  // PokerHand royalFlush1("AS KS QS JS TS");
  // PokerHand straightFlush0("8H 7H 6H 5H 4H");
  // PokerHand::compare(royalFlush1, straightFlush0);
  // std::cout << std::endl;

  // // Straight Flush
  // PokerHand straightFlush1("TC 9C 8C 7C 6C");
  // PokerHand straightFlush2("8H 7H 6H 5H 4H");
  // PokerHand straightFlush3("6S 5S 4S 3S 2S");
  // PokerHand straightFlush4("7D 6D 5D 4D 3D");
  // PokerHand straightFlush5("7S 6S 5S 4S 3S");
  // PokerHand::compare(straightFlush1, straightFlush2);
  // PokerHand::compare(straightFlush2, straightFlush3);
  // PokerHand::compare(straightFlush4, straightFlush5);
  // std::cout << std::endl;

  // // Four of a Kind
  // PokerHand fourKind1("KS KH KC KD 3H");
  // PokerHand fourKind2("7H 7D 7S 7C QH");
  // PokerHand fourKind3("7H 7D 7S 7C TS");
  // PokerHand fourKind4("4C 4S 4D 4H 9C");
  // PokerHand fourKind5("4C 4S 4D 4H 9D");
  // PokerHand::compare(fourKind1, fourKind2);
  // PokerHand::compare(fourKind2, fourKind3);
  // PokerHand::compare(fourKind4, fourKind5);
  // std::cout << std::endl;

  // // Full House
  // PokerHand fullHouse1("8S 8D 8H 7D 7C");
  // PokerHand fullHouse2("4D 4S 4C 9D 9C");
  // PokerHand fullHouse3("4D 4S 4C 5C 5D");
  // PokerHand fullHouse4("KC KS KD JC JS");
  // PokerHand fullHouse5("KC KH KD JC JH");
  // PokerHand::compare(fullHouse1, fullHouse2);
  // PokerHand::compare(fullHouse2, fullHouse3);
  // PokerHand::compare(fullHouse4, fullHouse5);
  // std::cout << std::endl;

  // // Flush
  // PokerHand flush1("KD JD 9D 6D 4D");
  // PokerHand flush2("QC JC 7C 6C 5C");
  // PokerHand flush3("JH TH 9H 4H 2H");
  // PokerHand flush4("JS TS 8S 6S 3S");
  // PokerHand flush5("JH TH 8H 4H 3H");
  // PokerHand flush6("JC TC 8C 4C 2C");
  // PokerHand flush7("TD 8D 7D 6D 5D");
  // PokerHand flush8("TS 8S 7S 6S 5S");
  // PokerHand::compare(flush1, flush2);
  // PokerHand::compare(flush2, flush3);
  // PokerHand::compare(flush3, flush4);
  // PokerHand::compare(flush4, flush5);
  // PokerHand::compare(flush5, flush6);
  // PokerHand::compare(flush7, flush8);
  // std::cout << std::endl;

  // // Straight
  // PokerHand straight1("JH TH 9C 8S 7H");
  // PokerHand straight2("TS 9S 8C 7H 6S");
  // PokerHand straight3("6C 5S 4H 3S 2D");
  // PokerHand straight4("9C 8C 7C 6D 5D");
  // PokerHand straight5("9S 8S 7S 6H 5H");
  // PokerHand::compare(straight1, straight2);
  // PokerHand::compare(straight2, straight3);
  // PokerHand::compare(straight4, straight5);
  // std::cout << std::endl;

  // // Three of a Kind
  // PokerHand threeKind1("6H 6D 6S QC 4S");
  // PokerHand threeKind2("3D 3S 3C KS 2S");
  // PokerHand threeKind3("3D 3S 3C JC 7H");
  // PokerHand threeKind4("3D 3S 3C JS 5D");
  // PokerHand threeKind5("9S 9H 9D TD 8H");
  // PokerHand threeKind6("9C 9S 9H TD 8D");
  // PokerHand::compare(threeKind1, threeKind2);
  // PokerHand::compare(threeKind2, threeKind3);
  // PokerHand::compare(threeKind3, threeKind4);
  // PokerHand::compare(threeKind5, threeKind6);
  // std::cout << std::endl;

  // // Two Pair
  // PokerHand twoPair1("TD TS 2S 2C KC");
  // PokerHand twoPair2("5C 5S 4D 4H TH");
  // PokerHand twoPair3("5C 5S 3C 3D QS");
  // PokerHand twoPair4("5C 5S 3C 3D JS");
  // PokerHand twoPair5("KD KS 7D 7H 8H");
  // PokerHand twoPair6("KC KS 7C 7H 8C");
  // PokerHand::compare(twoPair1, twoPair2);
  // PokerHand::compare(twoPair2, twoPair3);
  // PokerHand::compare(twoPair3, twoPair4);
  // PokerHand::compare(twoPair5, twoPair6);
  // std::cout << std::endl;

  // // One Pair
  // PokerHand onePair1("9C 9D QS JH 5H");
  // PokerHand onePair2("6D 6H KS 7H 4C");
  // PokerHand onePair3("6D 6H QH JS 2C");
  // PokerHand onePair4("6D 6H QS 8C 7D");
  // PokerHand onePair5("6D 6H QD 8H 3S");
  // PokerHand onePair6("8S 8D TH 6C 5S");
  // PokerHand onePair7("8H 8C TC 6S 5C");
  // PokerHand::compare(onePair1, onePair2);
  // PokerHand::compare(onePair2, onePair3);
  // PokerHand::compare(onePair3, onePair4);
  // PokerHand::compare(onePair4, onePair5);
  // PokerHand::compare(onePair6, onePair7);
  // std::cout << std::endl;

  // // High Card
  // PokerHand highCard1("KS 6C 5H 3D 2C");
  // PokerHand highCard2("QS JD 6C 5H 3C");
  // PokerHand highCard3("QS TD 8C 7D 4S");
  // PokerHand highCard4("QH TH 7C 6H 4S");
  // PokerHand highCard5("QC TC 7D 5C 4D");
  // PokerHand highCard6("QH TD 7S 5S 2H");
  // PokerHand highCard7("TC 8S 7S 6H 4D");
  // PokerHand highCard8("TD 8D 7S 6C 4C");
  // PokerHand::compare(highCard1, highCard2);
  // PokerHand::compare(highCard2, highCard3);
  // PokerHand::compare(highCard3, highCard4);
  // PokerHand::compare(highCard4, highCard5);
  // PokerHand::compare(highCard5, highCard6);
  // PokerHand::compare(highCard7, highCard8);
  // std::cout << std::endl;

  PokerGame game;
  game.setup();

  char again = 'y';
  while (again == 'y' || again == 'Y') {
      game.playRound();
      std::cout << "\nPlay another round? (y/n): ";
      std::cin >> again;
  }

  return 0;
}
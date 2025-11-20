#pragma once
#include <array>
#include <string>
#include <vector>
#include <unordered_map>

enum class Suit { C, D, H, S };

enum class Rank {
  Two   = 2,
  Three = 3,
  Four  = 4,
  Five  = 5,
  Six   = 6,
  Seven = 7,
  Eight = 8,
  Nine  = 9,
  Ten   = 10,
  Jack  = 11,
  Queen = 12,
  King  = 13,
  Ace   = 14
};


// Hand ranking categories
enum class HandType {
  HighCard = 0,
  OnePair,
  TwoPair,
  ThreeOfAKind,
  Straight,
  Flush,
  FullHouse,
  FourOfAKind,
  StraightFlush,
  RoyalFlush
};

class Card {
 public:
  Rank rank;
  Suit suit;

  Card() = default;
  Card(Rank r, Suit s) : rank(r), suit(s) {}

  // Needed for sorting cards by rank
  bool operator<(const Card &other) const {
    return static_cast<int>(rank) < static_cast<int>(other.rank);
  }
};

class PokerHand {
 public:
  explicit PokerHand(const std::string &handStr);
  void determineRank();
  static void compare(const PokerHand &firstHand, const PokerHand &secondHand);

  HandType getRankValue() const { return rankValue; }
  Rank getHighestRankNumber() const { return highestRankNumber; }
  const std::array<Card,5>& getCards() const { return cards; }

 private:
  std::array<Card, 5> cards;
  HandType rankValue;
  Rank highestRankNumber;

  // Turns the face cards to integers
  static Rank rankCharToRank(char c);

  void parseHand(const std::string &handStr);

  // All of the checks used to determine rank
  bool checkRoyalFlush();
  bool checkStraightFlush();
  bool checkFourOfAKind();
  bool checkFullHouse();
  bool checkFlush();
  bool checkStraight();
  bool checkThreeOfAKind();
  bool checkTwoPair();
  bool checkOnePair();
};

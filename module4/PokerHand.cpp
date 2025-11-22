#include "PokerHand.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

// Constructor that sets the ranking of each hand
PokerHand::PokerHand(const string &handStr) {
  parseHand(handStr);
  determineRank();
}

// Turns the face cards to integers
Rank PokerHand::rankCharToRank(char c) {
  if (c >= '2' && c <= '9') {
    return static_cast<Rank>(c - '0');
  }

  switch (c) {
    case 'T':
      return Rank::Ten;
    case 'J':
      return Rank::Jack;
    case 'Q':
      return Rank::Queen;
    case 'K':
      return Rank::King;
    case 'A':
      return Rank::Ace;
    default: 
      throw runtime_error("Rank is not valid.");
  }
}

// Parses the hand into an array to make it easier to read by the computer
void PokerHand::parseHand(const std::string &handStr) {
  std::stringstream ss(handStr);
  std::string cardStr;
  size_t idx = 0;

  // Turn each rank into a number to store in the cards array
  while (ss >> cardStr && idx < cards.size()) {
    Rank r = rankCharToRank(cardStr[0]);

    char suitChar = cardStr[1];
    Suit s;
    switch (suitChar) {
      case 'C': 
        s = Suit::C; 
        break;
      case 'D': 
        s = Suit::D; 
        break;
      case 'H': 
        s = Suit::H; 
        break;
      case 'S': 
        s = Suit::S; 
        break;
      default: 
        throw runtime_error("Suit is not valid.");
    }

    // Add value and suit to a 2D array representation of the hand
    cards[idx++] = Card(r, s);
  }

  if (idx != cards.size())
    throw std::runtime_error("Expected exactly 5 cards.");

  // Sort cards to make determining the rank of the hand easier later on
  std::sort(cards.begin(), cards.end());
}

// Helper to count rank frequencies
static unordered_map<Rank, int> countRanks(const std::array<Card,5> &cards) {
  unordered_map<Rank, int> countsMap;
  for (const Card &c : cards) {
    countsMap[c.rank]++;
  }

  return countsMap;
}

// Checks if hand is a Flush
bool PokerHand::checkFlush() {
  Suit s = cards[0].suit;

  for (const Card &c : cards) {
    if (c.suit != s) {
      return false;
    }
  }

  highestRankNumber = cards.back().rank;
  return true;
}

// Checks if hand is a Straight
bool PokerHand::checkStraight() {
  int r0 = static_cast<int>(cards[0].rank);
  int r4 = static_cast<int>(cards[4].rank);

  // handle wheel (straight with 5, 4, 3, 2, A)
  bool wheel =
    (cards[4].rank == Rank::Ace &&
      cards[0].rank == Rank::Two &&
      cards[1].rank == Rank::Three &&
      cards[2].rank == Rank::Four &&
      cards[3].rank == Rank::Five);

  if (wheel) {
    highestRankNumber = Rank::Five;
    return true;
  }

  for (int i = 0; i < 4; i++) {
    int r1 = static_cast<int>(cards[i].rank);
    int r2 = static_cast<int>(cards[i + 1].rank);

    if (r2 - r1 != 1) {
      return false;
    }
  }

  highestRankNumber = cards.back().rank;
  return true;
}

// Checks if hand is a Royal Flush
bool PokerHand::checkRoyalFlush() {
    // Must be a straight flush and highest card must be Ace
    if (checkStraightFlush() && cards.back().rank == Rank::Ace) {
      highestRankNumber = Rank::Ace;
      return true;
    }
    return false;
}

// Checks if hand is a Straight Flush
bool PokerHand::checkStraightFlush() {
  return checkFlush() && checkStraight();
}

// Checks if hand is a Four of a Kind
bool PokerHand::checkFourOfAKind() {
  unordered_map<Rank, int> counts = countRanks(cards);

  for (auto &p : counts) {
    if (p.second == 4) {
      highestRankNumber = p.first;
      return true;
    }
  }

  return false;
}

// Checks if hand is a Full House
bool PokerHand::checkFullHouse() {
  unordered_map<Rank, int> counts = countRanks(cards);
  bool twos = false, threes = false;
  Rank threeRank = Rank::Two;

  for (auto &p : counts) {
    if (p.second == 3) {
      threes = true;
      threeRank = p.first;
    } else if (p.second == 2) {
      twos = true;
    }
  }

  if (threes && twos) {
    highestRankNumber = threeRank;
    return true;
  }

  return false;
}

// Checks if hand is a Three of a Kind
bool PokerHand::checkThreeOfAKind() {
  unordered_map<Rank, int> counts = countRanks(cards);

  for (auto &p : counts) {
    if (p.second == 3) {
      highestRankNumber = p.first;
      return true;
    }
  }

  return false;
}

// Checks if hand is a Two Pair
bool PokerHand::checkTwoPair() {
  unordered_map<Rank, int> counts = countRanks(cards);
  vector<Rank> pairs;

  for (auto &p : counts) {
      if (p.second == 2)
          pairs.push_back(p.first);
  }

  if (pairs.size() == 2) {
    highestRankNumber = (static_cast<int>(pairs[0]) > static_cast<int>(pairs[1])) ? pairs[0] : pairs[1];
    return true;
  }

  return false;
}

// Checks if hand is a One Pair
bool PokerHand::checkOnePair() {unordered_map<Rank, int> counts = countRanks(cards);
  for (auto &p : counts) {
    if (p.second == 2) {
      highestRankNumber = p.first;
      return true;
    }
  }
  return false;
}

// Uses all of the hand checks to rank the cards
// 8 is the strongest and 0 is weakest hand
void PokerHand::determineRank() {
  if (checkRoyalFlush()) {
    rankValue = HandType::RoyalFlush;
    cout << "Royal Flush" << endl;
  } else if (checkStraightFlush()) {
    rankValue = HandType::StraightFlush;
    cout << "Straight Flush" << endl;
  } else if (checkFourOfAKind()) {
    rankValue = HandType::FourOfAKind;
    cout << "Four of a Kind" << endl;
  } else if (checkFullHouse()) {
    rankValue = HandType::FullHouse;
    cout << "Full House" << endl;
  } else if (checkFlush()) {
    rankValue = HandType::Flush;
    cout << "Flush" << endl;
  } else if (checkStraight()) {
    rankValue = HandType::Straight;
    cout << "Straight" << endl;
  } else if (checkThreeOfAKind()) {
    rankValue = HandType::ThreeOfAKind;
    cout << "Three of a Kind" << endl;
  } else if (checkTwoPair()) {
    rankValue = HandType::TwoPair;
    cout << "Two Pair" << endl;
  } else if (checkOnePair()) {
    rankValue = HandType::OnePair;
    cout << "One Pair" << endl;
  } else {
    rankValue = HandType::HighCard;
    highestRankNumber = cards.back().rank;
    cout << "High Card" << endl;
  }
}
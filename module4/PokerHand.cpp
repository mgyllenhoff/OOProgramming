#include "PokerHand.h"
#include <iostream>
#include <sstream>

using namespace std;

// Constructor that sets the ranking of each hand
PokerHand::PokerHand(const string &handStr) {
  parseHand(handStr);
  determineRank();
}

// Turns the face cards to integers
int PokerHand::rankCharToInt(char c) {
  if (c >= '2' && c <= '9') {
    return c - '0';
  }

  switch (c) {
    case 'T':
      return 10;
      break;
    case 'J':
      return 11;
      break;
    case 'Q':
      return 12;
      break;
    case 'K':
      return 13;
      break;
    case 'A':
      return 14;
      break;
    default:
      return -1;
  }
}

// Parses the hand into an array to make it easier to read by the computer
void PokerHand::parseHand(const std::string &handStr) {
  std::stringstream ss(handStr);
  std::string card;
  size_t idx = 0;

  // Turn each rank into a number to store in the cards array
  while (ss >> card && idx < cards.size()) {
    int val = rankCharToInt(card[0]);
    if (val == -1) {
      throw std::runtime_error("Rank is not valid.");
    }

    char suit = card[1];
    if (suit != 'C' && suit != 'D' && suit != 'H' && suit != 'S') {
      throw std::runtime_error("Suit is not valid.");
    }

    // Add value and suit to a 2D array representation of the hand
    cards[idx++] = {val, suit};
  }

  if (idx != cards.size())
    throw std::runtime_error("Expected exactly 5 cards.");

  // Sort cards to make determining the rank of the hand easier later on
  std::sort(cards.begin(), cards.end());
}

// Counts how many of each rank are in a hand
static unordered_map<int, int> countRanks(
    const std::array<std::pair<int, char>, 5> &cards) {
  unordered_map<int, int> countsMap;

  for (const std::pair<int, char> &c : cards) {
    countsMap[c.first]++;
  }

  return countsMap;
}

// Checks if hand is a Flush
bool PokerHand::checkFlush() {
  char suit = cards[0].second;

  for (std::pair<int, char> &c : cards) {
    if (c.second != suit) {
      return false;
    }
  }

  highestRankNumber = cards.back().first;
  return true;
}

// Checks if hand is a Straight
bool PokerHand::checkStraight() {
  // handle wheel (straight with 5, 4, 3, 2, A)
  bool wheel =
      (cards[4].first == 14 && cards[0].first == 2 && cards[1].first == 3 &&
       cards[2].first == 4 && cards[3].first == 5);

  if (wheel) {
    highestRankNumber = 5;
    return true;
  }

  for (int i = 0; i < 4; i++) {
    if (!wheel && cards[i + 1].first - cards[i].first != 1) {
      return false;
    }
  }

  highestRankNumber = cards.back().first;
  return true;
}

// Checks if hand is a Straight Flush
bool PokerHand::checkStraightFlush() {
  if (checkFlush() && checkStraight()) {
    return true;
  } else {
    return false;
  }
}

// Checks if hand is a Four of a Kind
bool PokerHand::checkFourOfAKind() {
  unordered_map<int, int> counts = countRanks(cards);

  for (const std::pair<int, int> &count : counts) {
    if (count.second == 4) {
      highestRankNumber = count.first;
      return true;
    }
  }

  return false;
}

// Checks if hand is a Full House
bool PokerHand::checkFullHouse() {
  unordered_map<int, int> counts = countRanks(cards);

  bool twosSuits = false;
  bool threeSuits = false;
  int threeRank = 0;

  for (const std::pair<int, int> &count : counts) {
    if (count.second == 3) {
      threeSuits = true;
      threeRank = count.first;
    } else if (count.second == 2) {
      twosSuits = true;
    }
  }

  if (threeSuits && twosSuits) {
    highestRankNumber = threeRank;
    return true;
  }

  return false;
}

// Checks if hand is a Three of a Kind
bool PokerHand::checkThreeOfAKind() {
  unordered_map<int, int> counts = countRanks(cards);

  for (const std::pair<int, int> &count : counts) {
    if (count.second == 3) {
      highestRankNumber = count.first;
      return true;
    }
  }

  return false;
}

// Checks if hand is a Two Pair
bool PokerHand::checkTwoPair() {
  unordered_map<int, int> counts = countRanks(cards);

  vector<int> pairs;

  for (const std::pair<int, int> &count : counts) {
    if (count.second == 2) {
      pairs.push_back(count.first);
    }
  }

  if (pairs.size() == 2) {
    highestRankNumber = max(pairs[0], pairs[1]);
    return true;
  }

  return false;
}

// Checks if hand is a One Pair
bool PokerHand::checkOnePair() {
  unordered_map<int, int> counts = countRanks(cards);

  for (const std::pair<int, int> &count : counts) {
    if (count.second == 2) {
      highestRankNumber = count.first;
      return true;
    }
  }

  return false;
}

// Uses all of the hand checks to rank the cards
// 8 is the strongest and 0 is weakest hand
void PokerHand::determineRank() {
  if (checkStraightFlush()) {
    rankValue = 8;
    cout << "Straight Flush" << endl;
  } else if (checkFourOfAKind()) {
    rankValue = 7;
    cout << "Four of a Kind" << endl;
  } else if (checkFullHouse()) {
    rankValue = 6;
    cout << "Full House" << endl;
  } else if (checkFlush()) {
    rankValue = 5;
    cout << "Flush" << endl;
  } else if (checkStraight()) {
    rankValue = 4;
    cout << "Straight" << endl;
  } else if (checkThreeOfAKind()) {
    rankValue = 3;
    cout << "Three of a Kind" << endl;
  } else if (checkTwoPair()) {
    rankValue = 2;
    cout << "Two Pair" << endl;
  } else if (checkOnePair()) {
    rankValue = 1;
    cout << "One Pair" << endl;
  } else {
    // High card
    rankValue = 0;
    highestRankNumber = cards.back().first;
    cout << "High Card" << endl;
  }
}

// Determines a winner by first comparing the rank of two hands
// If ranks are the same, compares the highest ranking number
void PokerHand::compare(const PokerHand &firstHand,
                        const PokerHand &secondHand) {
  if (firstHand.rankValue > secondHand.rankValue) {
    cout << "First Hand Wins!" << endl;
  } else if (firstHand.rankValue < secondHand.rankValue) {
    cout << "Second Hand Wins!" << endl;
  } else {
    if (firstHand.highestRankNumber > secondHand.highestRankNumber) {
      cout << "First Hand Wins!" << endl;
    } else if (firstHand.highestRankNumber < secondHand.highestRankNumber) {
      cout << "Second Hand Wins!" << endl;
    } else {
      cout << "It's a Tie!" << endl;
    }
  }
}

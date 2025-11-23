#pragma once
#include "PokerHand.h"
#include <vector>

struct StandardCard {
  Rank rank;
  Suit suit;
};

class Player {
  public:
    int chips;
    std::string name;
    bool isHuman;
    std::vector<StandardCard> hand;

    Player(std::string name, bool isHuman);
    std::string handToString() const;
    void addCard(StandardCard c);
    void clearHand();
};

class Deck {
  public:
    Deck();
    void reset();
    void shuffle();
    StandardCard deal();

  private:
    std::vector<StandardCard> cards;
};

class PokerGame {
  public:
    void setup();
    void playRound();
    int compareHandsReturn(const PokerHand &h1, const PokerHand &h2);

  private:
    std::vector<Player> players;
    Deck deck;
    int pot = 0;

    void dealHands();
    void bettingRound();
    void declareWinner();
};

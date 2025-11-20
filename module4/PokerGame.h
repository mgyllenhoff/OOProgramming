#pragma once
#include "PokerHand.h"
#include <vector>
#include <string>

// Card structure for a standard deck
struct StandardCard {
    Rank rank;
    Suit suit;
};

class Player {
    public:
        std::string name;
        bool isHuman;
        int chips;
        std::vector<StandardCard> hand;

        Player(std::string name, bool isHuman);

        void clearHand();
        void addCard(StandardCard c);
        std::string handToString() const;
};

class Deck {
    public:
        std::vector<StandardCard> cards;

        Deck();
        void reset();
        void shuffle();
        StandardCard deal();
};

class PokerGame {
    public:
        std::vector<Player> players;
        Deck deck;
        int pot = 0;

        void setup();
        void dealHands();
        void bettingRound();
        void declareWinner();
        void playRound();

    private:
        int compareHandsReturn(const PokerHand &h1, const PokerHand &h2);
};

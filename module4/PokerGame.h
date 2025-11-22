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

        Player(std::string name, bool isHuman);

        void clearHand();
        void addCard(StandardCard c);
        std::string handToString() const;

    private:
        std::vector<StandardCard> hand;

        friend class PokerGame; // allows PokerGame to access the hand when building PokerHand
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

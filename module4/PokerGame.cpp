#include "PokerGame.h"
#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

// Constructor for player (whether human or computer)
Player::Player(string name, bool isHuman)
    : name(name), isHuman(isHuman), chips(100) {}

// Clears the players hand
void Player::clearHand() {
    hand.clear();
}

// Adds a single card to the player's hand
void Player::addCard(StandardCard c) {
    hand.push_back(c);
}

// Converts the player's hand into a human-readable string
// Each card is converted into rank + suit characters.
string Player::handToString() const {
    string s;
    for (auto &c : hand) {
        char rankChar;
        int rv = (int)c.rank; // Convert enum Rank to its integer value

        // Convert numeric rank values to their corresponding characters
        if (rv >= 2 && rv <= 9) {
            rankChar = '0' + rv;
        } else if (rv == 10) { 
            rankChar = 'T';
        } else if (rv == 11) { 
            rankChar = 'J';
        } else if (rv == 12) { 
            rankChar = 'Q';
        } else if (rv == 13) { 
            rankChar = 'K';
        } else rankChar = 'A';

        // Convert suit enum to suit letter
        char suitChar;
        switch (c.suit) {
            case Suit::C: suitChar='C'; break;
            case Suit::D: suitChar='D'; break;
            case Suit::H: suitChar='H'; break;
            case Suit::S: suitChar='S'; break;
        }

        // Append rank and suit to final string
        s += rankChar;
        s += suitChar;
        s += " ";
    }

    // Return completed hand string
    return s;
}

// Deck constructor (creates a fresh 52 card deck)
Deck::Deck() {
    reset();
}

// Creates a full 52 card deck
void Deck::reset() {
    // Remove any existing cards from deck
    cards.clear();

    // Loop over the 4 suits
    for (int s = 0; s < 4; s++) {
        // Loop over ranks 2 through Ace (14)
        for (int r = 2; r <= 14; r++) {
            cards.push_back(StandardCard{
                static_cast<Rank>(r),
                static_cast<Suit>(s)
            });
        }
    }
}

// Shuffles the deck using Fisher-Yates shuffle algorithm
void Deck::shuffle() {
    random_device rd;
    mt19937 rng(rd());

    // Iterate backward through deck and swap each element with a random element
    for (int i = cards.size() - 1; i > 0; i--) {
        uniform_int_distribution<int> dist(0, i); // Distribution for index
        int j = dist(rng); // Random index
        swap(cards[i], cards[j]);
    }
}

// Deals the card from the top of deck (back of vector)
StandardCard Deck::deal() {
    // Get last card and removes it from deck
    StandardCard c = cards.back();
    cards.pop_back();
    return c;
}

// Initializes the PokerGame by prompt for players
void PokerGame::setup() {
    int numPlayers;

    // Repeatedly prompt until valid number is entered
    do {
        cout << "Enter number of players (2–7): ";
        cin >> numPlayers;
    } while (numPlayers < 2 || numPlayers > 7);

    // Create each player either as human or computer
    for (int i = 0; i < numPlayers; i++) {
        char type;
        cout << "Is player " << (i + 1) << " human? (y/n): ";
        cin >> type;

        if (type == 'y' || type == 'Y')
            players.emplace_back("Human" + to_string(i + 1), true);
        else
            players.emplace_back("Computer" + to_string(i + 1), false);
    }
}

// Deals a 5-card hand to each player
void PokerGame::dealHands() {
    // Prepares for deal by reseting and shuffling it
    deck.reset();
    deck.shuffle();

    // Clears each player's old hands before new round
    for (Player &p : players)
        p.clearHand();

    // Deal 5 cards each (one at a time)
    for (int i = 0; i < 5; i++) {
        for (Player &p : players) {
            p.addCard(deck.deal());
        }
    }
}

// Handles the betting (everyone antes 10, then optionally bets)
void PokerGame::bettingRound() {
    // Resets the pot for this round
    pot = 0;

    cout << "\n--- Ante Round (everyone antes 10 chips) ---\n";
    for (Player &p : players) {
        p.chips -= 10;
        pot += 10;
    }
    cout << "Pot is now: " << pot << " chips.\n";

    // Additional betting round
    for (Player &p : players) {
        if (p.isHuman) {
            int bet;

            // Show the human player's hand
            cout << p.name << ", your hand: " << p.handToString() << "\n";
            cout << "Enter additional bet (0 to check): ";
            cin >> bet;

            p.chips -= bet;
            pot += bet;
        } else {
            // Computer bets 5 if its high card is better than Jack
            int highRank = (int)p.hand.back().rank;
            int bet = (highRank > 11 ? 5 : 0);

            p.chips -= bet;
            pot += bet;

            cout << p.name << " bets " << bet << "\n";
        }
    }
}

// Determines the winner and awards the pot
void PokerGame::declareWinner() {
    cout << "\n--- Showdown ---\n";

    // Stores PokerHand objects for each player
    vector<PokerHand> hands;

    // Convert each player's hand into PokerHand class
    for (Player &p : players)
        hands.emplace_back(p.handToString());

    // Compare each player's hand against current best
    int bestIndex = 0;
    for (int i = 1; i < players.size(); i++) {
        int result = compareHandsReturn(hands[bestIndex], hands[i]);
        if (result == 2)
            bestIndex = i;
    }

    cout << "\nWinner: " << players[bestIndex].name << "!\n";
    cout << "They win the pot of " << pot << " chips!\n";

    players[bestIndex].chips += pot;
}

// Compares two PokerHand objects
// Returns 1 if h1 winds, 2 if h2 wins, or 0 if tie
int PokerGame::compareHandsReturn(const PokerHand &h1, const PokerHand &h2) {
    if ((int)h1.getRankValue() > (int)h2.getRankValue()) {
        return 1;
    }
    if ((int)h1.getRankValue() < (int)h2.getRankValue()) {
        return 2;
    }

    if ((int)h1.getHighestRankNumber() > (int)h2.getHighestRankNumber()) {
        return 1;
    }
    if ((int)h1.getHighestRankNumber() < (int)h2.getHighestRankNumber()) {
        return 2;
    }

    vector<int> r1, r2;
    for (auto &c : h1.getCards()) {
        r1.push_back((int)c.rank);
    }
    for (auto &c : h2.getCards()) {
        r2.push_back((int)c.rank);
    }

    sort(r1.begin(), r1.end(), greater<int>());
    sort(r2.begin(), r2.end(), greater<int>());

    for (int i = 0; i < 5; i++) {
        if (r1[i] > r2[i]) {
            return 1;
        }
        if (r1[i] < r2[i]) {
            return 2;
        }
    }

    return 0; // Tie
}

// Begins the round!
void PokerGame::playRound() {
    dealHands();
    bettingRound();
    declareWinner();
}

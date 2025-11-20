#include "PokerGame.h"
#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

// Constructor for player
Player::Player(string name, bool isHuman)
    : name(name), isHuman(isHuman), chips(100) {}

void Player::clearHand() {
    hand.clear();
}

void Player::addCard(StandardCard c) {
    hand.push_back(c);
}

string Player::handToString() const {
    string s;
    for (auto &c : hand) {
        char rankChar;
        int rv = (int)c.rank;

        if (rv >= 2 && rv <= 9) rankChar = '0' + rv;
        else if (rv == 10) rankChar = 'T';
        else if (rv == 11) rankChar = 'J';
        else if (rv == 12) rankChar = 'Q';
        else if (rv == 13) rankChar = 'K';
        else rankChar = 'A';

        char suitChar;
        switch (c.suit) {
            case Suit::C: suitChar='C'; break;
            case Suit::D: suitChar='D'; break;
            case Suit::H: suitChar='H'; break;
            case Suit::S: suitChar='S'; break;
        }

        s += rankChar;
        s += suitChar;
        s += " ";
    }
    return s;
}

// =============================================================
// Deck Implementation
// =============================================================

Deck::Deck() { reset(); }

void Deck::reset() {
    cards.clear();
    for (int s = 0; s < 4; s++) {
        for (int r = 2; r <= 14; r++) {
            cards.push_back(StandardCard{
                static_cast<Rank>(r),
                static_cast<Suit>(s)
            });
        }
    }
}

void Deck::shuffle() {
    random_device rd;
    mt19937 rng(rd());

    for (int i = cards.size() - 1; i > 0; i--) {
        uniform_int_distribution<int> dist(0, i);
        int j = dist(rng);
        std::swap(cards[i], cards[j]);
    }
}

StandardCard Deck::deal() {
    StandardCard c = cards.back();
    cards.pop_back();
    return c;
}

// =============================================================
// PokerGame Implementation
// =============================================================

void PokerGame::setup() {
    int numPlayers;
    do {
        cout << "Enter number of players (2–7): ";
        cin >> numPlayers;
    } while (numPlayers < 2 || numPlayers > 7);

    for (int i = 0; i < numPlayers; i++) {
        char type;
        cout << "Is player " << (i+1) << " human? (y/n): ";
        cin >> type;

        if (type == 'y' || type == 'Y')
            players.emplace_back("Human" + to_string(i+1), true);
        else
            players.emplace_back("Computer" + to_string(i+1), false);
    }
}

void PokerGame::dealHands() {
    deck.reset();
    deck.shuffle();

    for (auto &p : players)
        p.clearHand();

    // deal 5 cards each (one at a time)
    for (int i = 0; i < 5; i++) {
        for (auto &p : players) {
            p.addCard(deck.deal());
        }
    }
}

void PokerGame::bettingRound() {
    pot = 0;

    cout << "\n--- Betting Round (everyone antes 10 chips) ---\n";
    for (auto &p : players) {
        p.chips -= 10;
        pot += 10;
    }
    cout << "Pot is now: " << pot << " chips.\n";

    // optional simple betting
    for (auto &p : players) {
        if (p.isHuman) {
            int bet;
            cout << p.name << ", your hand: " << p.handToString() << "\n";
            cout << "Enter additional bet (0 to check): ";
            cin >> bet;

            p.chips -= bet;
            pot += bet;
        } else {
            // simple CPU heuristic
            int highRank = (int)p.hand.back().rank;
            int bet = (highRank > 11 ? 5 : 0);

            p.chips -= bet;
            pot += bet;

            cout << p.name << " bets " << bet << "\n";
        }
    }
}

void PokerGame::declareWinner() {
    cout << "\n--- Showdown ---\n";

    vector<PokerHand> hands;
    for (auto &p : players)
        hands.emplace_back(p.handToString());

    int bestIndex = 0;

    for (int i = 1; i < players.size(); i++) {
        cout << "Comparing " << players[bestIndex].name
             << " vs " << players[i].name << "...\n";

        int result = compareHandsReturn(hands[bestIndex], hands[i]);
        if (result == 2)
            bestIndex = i;
    }

    cout << "\nWinner: " << players[bestIndex].name << "!\n";
    cout << "They win the pot of " << pot << " chips!\n";

    players[bestIndex].chips += pot;
}

int PokerGame::compareHandsReturn(const PokerHand &h1, const PokerHand &h2) {
    // high-level category
    if ((int)h1.getRankValue() > (int)h2.getRankValue()) return 1;
    if ((int)h1.getRankValue() < (int)h2.getRankValue()) return 2;

    // primary rank comparison
    if ((int)h1.getHighestRankNumber() > (int)h2.getHighestRankNumber()) return 1;
    if ((int)h1.getHighestRankNumber() < (int)h2.getHighestRankNumber()) return 2;

    // compare kickers
    vector<int> r1, r2;
    for (auto &c : h1.getCards()) r1.push_back((int)c.rank);
    for (auto &c : h2.getCards()) r2.push_back((int)c.rank);

    sort(r1.begin(), r1.end(), greater<int>());
    sort(r2.begin(), r2.end(), greater<int>());

    for (int i = 0; i < 5; i++) {
        if (r1[i] > r2[i]) return 1;
        if (r1[i] < r2[i]) return 2;
    }

    return 0; // tie
}

void PokerGame::playRound() {
    dealHands();
    bettingRound();
    declareWinner();
}

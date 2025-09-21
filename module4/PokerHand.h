#include <vector>

class PokerHand {
 public:
  explicit PokerHand(const std::string &handStr);
  void determineRank();
  static void compare(const PokerHand &firstHand, const PokerHand &secondHand);

 private:
  std::array<std::pair<int, char>, 5> cards;
  int rankValue;
  int highestRankNumber;

  static int rankCharToInt(char c);
  void parseHand(const std::string &handStr);

  // All of the checks used to determine rank
  bool checkStraightFlush();
  bool checkFourOfAKind();
  bool checkFullHouse();
  bool checkFlush();
  bool checkStraight();
  bool checkThreeOfAKind();
  bool checkTwoPair();
  bool checkOnePair();
};
#include <uC++.h>

class Printer {
 public:
  Printer(const unsigned int NoOfPlayers, const unsigned int NoOfCards);
  void prt(const unsigned int id,
           const unsigned int took,
           unsigned int PlayersRemaining);
 private:
  void main() {

  }
};

_Coroutine Player {
 public:
  enum { DEATH_DECK_DIVISOR = 5 };
  static void players(unsigned int num);
  Player(Printer& printer, unsigned int id);
  void start(Player& lp, Player& rp);
  void play(unsigned int deck);
 private:
  void main() {

  }
};

void uMain::main() {

}

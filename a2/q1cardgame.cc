#include <uC++.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unistd.h>

#include "PRNG.h"

using namespace std;

static PRNG gRand;

class Printer {
 public:
  Printer(const uint32_t NoOfPlayers, const uint32_t NoOfCards) { }
  void prt(
      const uint32_t id, const uint32_t took, uint32_t PlayersRemaining) { }
 private:
  void main() {

  }
};

_Coroutine Player {
 public:
  enum { DEATH_DECK_DIVISOR = 5 };
  static void players(uint32_t num) { }
  Player(Printer& printer, uint32_t id) { }
  void start(Player& lp, Player& rp) { }
  void play(uint32_t deck) { }
 private:
  void main() {

  }
};

void uMain::main() {
  uint32_t games = 5;
  // something suitably random. i saw this used once, and it seemed like a
  // not overly terrible idea :)
  uint32_t seed = time(NULL) ^ getpid();
  // this command-line argument reading idiom is awesome, at least for
  // quick-and-dirty scripts that can get away with positional arguments.
  switch (argc) {
    case 3:
      seed = atoi(argv[2]);
    case 2:
      games = atoi(argv[1]);
  }

  for (uint32_t g = 0; g < games; g++) {
    uint32_t numPlayers = gRand(2, 10);
    uint32_t numCards = gRand(10, 200);
    uint32_t startingPlayer = gRand(0, numPlayers-1);
    Player::players(numPlayers);
    Printer printer(numPlayers, numCards);
    Player* players[numPlayers];
    for (uint32_t p = 0; p < numPlayers; p++) {
      players[p] = new Player(printer, p);
    }
    for (uint32_t p = 0; p < numPlayers; p++) {
      players[p]->start(
          *players[(p + numPlayers - 1) % numPlayers],
          *players[(p + 1) % numPlayers]);
    }
    players[startingPlayer]->play(numCards);
    for (uint32_t p  = 0; p < numPlayers; p++) {
      delete players[p];
    }
  }
}

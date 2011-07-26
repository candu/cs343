#include <uC++.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unistd.h>

#include "PRNG.h"

using namespace std;

static PRNG gRand;

/**
 * Used by the Printer to store player state information for printing.
 */
struct PlayerInfo {
  enum Action {
    PASS_LEFT,
    PASS_RIGHT,
    END_NO_MORE_CARDS,
    END_NO_MORE_PLAYERS,
    NONE
  };

  PlayerInfo()
    : cardsTaken_(0),
      cardsRemaining_(0),
      action_(NONE),
      overwroteBuffer_(false),
      died_(false) { }

  friend ostream& operator<<(ostream& os, const PlayerInfo& info);
  
  uint32_t cardsTaken_;
  uint32_t cardsRemaining_;
  Action action_;
  bool overwroteBuffer_;
  bool died_;
};

ostream& operator<<(ostream& os, const PlayerInfo& info) {
  if (info.action_ == PlayerInfo::END_NO_MORE_PLAYERS) {
    os << "#";
  } else {
    os << info.cardsTaken_;
    os << ":";
  }
  os << info.cardsRemaining_;
  switch (info.action_) {
    case PlayerInfo::PASS_LEFT:
      os << "<";
      break;
    case PlayerInfo::PASS_RIGHT:
      os << ">";
      break;
    case PlayerInfo::END_NO_MORE_CARDS:
    case PlayerInfo::END_NO_MORE_PLAYERS:
      os << "#";
      break;
    default:
      // TODO: raise error
      break;
  }
  if (info.overwroteBuffer_) {
    os << "*";
  }
  if (info.died_) {
    os << "X";
  }
  return os;
}

_Coroutine Printer {
 public:
  Printer(const uint32_t NoOfPlayers, const uint32_t NoOfCards)
    : playersInitial_(NoOfPlayers),
      cardsInitial_(NoOfCards) {
    // assume ids are assigned on [0, playersInitial_)
    buffer_ = new PlayerInfo*[playersInitial_];
    // ensure initial state is sensible
    for (uint32_t i = 0; i < playersInitial_; i++) {
      buffer_[i] = NULL;
    }
    prtHeader();
  }
  /**
   * We're departing from the assignment here because it seems stupid to
   * duplicate this logic in the Player and Printer, and not much better to
   * put all playing logic in the Printer - instead, we'll have the Player
   * decide what PlayerInfo it wants to send to the printer. (The Player is
   * trusted anyways; we trust it to accurately report cards remaining, etc.)
   */
  void prt(
      const uint32_t id, PlayerInfo* info) {
    if (buffer_[id] != NULL) {
      buffer_[id]->overwroteBuffer_ = true;
      flush();
    }
    buffer_[id] = info;
    if (info->action_ == PlayerInfo::END_NO_MORE_PLAYERS ||
        info->action_ == PlayerInfo::END_NO_MORE_CARDS) {
      // we assume Players will trigger suspend() back into uMain::main(),
      // so that the printer need not really exit
      flush();
    }
    // actual printing takes place in main()
    resume();
  }
 private:
  uint32_t playersInitial_;
  uint32_t cardsInitial_;
  bool done_;
  PlayerInfo** buffer_;
  void prtHeader() {
    cout << "Players: " << playersInitial_ << '\t' <<
            "Cards: " << cardsInitial_ << endl;
    for (uint32_t i = 0; i < playersInitial_; i++) {
      cout << "P" << i << '\t';
    }
    cout << endl;
  }
  void flush() {
    for (uint32_t i = 0; i < playersInitial_; i++) {
      if (buffer_[i] != NULL) {
        cout << *buffer_[i];
        delete buffer_[i];
        buffer_[i] = NULL;
      }
      cout << '\t';
    }
    cout << endl;
  }
  void main() {
    while (true) {
      suspend();
    }
  }
};

_Coroutine Player {
 public:
  enum { DEATH_DECK_DIVISOR = 5 };
  static void players(uint32_t num) {
    playersRemaining_ = num;
  }
  Player(Printer& printer, uint32_t id)
    : printer_(printer),
      id_(id),
      lp_(NULL),
      rp_(NULL),
      deckReceived_(0) { }
  void start(Player& lp, Player& rp) {
    lp_ = &lp;
    rp_ = &rp;
    resume();
  }
  void play(uint32_t deck) {
    deckReceived_ = deck;
    // actual playing takes place in main()
    resume();
  }
 private:
  static uint32_t playersRemaining_;
  Printer& printer_;
  uint32_t id_;
  Player* lp_;
  Player* rp_;
  uint32_t deckReceived_;
  void removeSelfFromGame() {
    lp_->rp_ = rp_;
    rp_->lp_ = lp_;
    playersRemaining_--;
  }
  void main() {
    // start() calls main() to set first starter, so we force a suspend() here
    // until play() is explicitly called...
    suspend();
    while (true) {
      PlayerInfo* info = new PlayerInfo();
      if (playersRemaining_ == 1) {
        info->cardsRemaining_ = deckReceived_;
        info->action_ = PlayerInfo::END_NO_MORE_PLAYERS;
      } else {
        info->cardsTaken_ = min(deckReceived_, gRand(1, 8));
        info->cardsRemaining_ = deckReceived_ - info->cardsTaken_;
        if (info->cardsRemaining_ == 0) {
          info->action_ = PlayerInfo::END_NO_MORE_CARDS;
        } if (info->cardsRemaining_ % 2 == 0) {
          info->action_ = PlayerInfo::PASS_RIGHT;
        } else {
          info->action_ = PlayerInfo::PASS_LEFT;
        }
      }
      if (deckReceived_ % DEATH_DECK_DIVISOR == 0) {
        info->died_ = true;
        removeSelfFromGame();
      }
      printer_.prt(id_, info);
      switch (info->action_) {
        case PlayerInfo::PASS_LEFT:
          lp_->play(info->cardsRemaining_);
          break;
        case PlayerInfo::PASS_RIGHT:
          rp_->play(info->cardsRemaining_);
          break;
        case PlayerInfo::END_NO_MORE_PLAYERS:
        case PlayerInfo::END_NO_MORE_CARDS:
        default:
          return;
      }
    }
  }
};

uint32_t Player::playersRemaining_ = 0;

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
  gRand.seed(seed);

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

#ifndef SEQUENCER_CC
#define SEQUENCER_CC

#include <uC++.h>
#include <uSemaphore.h>

class Sequencer {
 public:
  Sequencer()
    : lastTicket_(0),
      synch_(1) { }
  uint32_t ticket() {
    synch_.P();
    uint32_t ticket = ++lastTicket_;
    synch_.V();
    return ticket;
  }
  uint32_t check() {
    return lastTicket_;
  }
 private:
  uint32_t lastTicket_;
  uSemaphore synch_;
};

#endif // SEQUENCER_CC

#ifndef SEQUENCER_CC
#define SEQUENCER_CC

#include <uC++.h>
#include <uSemaphore.h>

class Sequencer {
 public:
  Sequencer()
    : lastTicket_(0),
      semaphore_() { }
  uint32_t ticket() {
    semaphore_.P();
    uint32_t ticket = ++lastTicket_;
    semaphore_.V();
    return ticket;
  }
  uint32_t check() {
    semaphore_.P();
    uint32_t ticket = lastTicket_;
    semaphore_.V();
    return ticket;
  }
 private:
  uint32_t lastTicket_;
  uSemaphore semaphore_;
};

#endif // SEQUENCER_CC

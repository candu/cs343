#ifndef EVENTCOUNTER_CC
#define EVENTCOUNTER_CC

#include <uC++.h>

class EventCounter {
 public:
  EventCounter(uint32_t event = 0)
    : nextEvent_(event),
      semaphore_() { }
  void await(uint32_t ticket) {

  }
  void advance() {

  }
  uint32_t check() {
  }
 private:
  uint32_t nextEvent_;
};

#endif // EVENTCOUNTER_CC

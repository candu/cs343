#ifndef EVENTCOUNTER_CC
#define EVENTCOUNTER_CC

#include <uC++.h>
#include <uSemaphore.h>

using namespace std;

class EventCounter {
 public:
  EventCounter(uint32_t event = 0)
    : lastEvent_(event),
      service_(event),
      wait_(0),
      numWaiting_(0),
      synch_() { }
  void await(uint32_t ticket) {
    synch_.P();
    if (lastEvent_ < ticket) {
      numWaiting_.V();
      for (uint32_t i = ticket; i > lastEvent_; i--) {
        wait_.P(synch_);
        synch_.P();
      }
      numWaiting_.P();
    }
    synch_.V();
    service_.P();
  }
  void advance() {
    synch_.P();
    ++lastEvent_;
    int32_t numWaiting = numWaiting_.counter();
    if (numWaiting > 0) {
      wait_.V(numWaiting);
    }
    service_.V();
    synch_.V();
  }
  uint32_t check() {
    return lastEvent_;
  }
 private:
  uint32_t lastEvent_;
  uSemaphore service_;
  uSemaphore wait_;
  uSemaphore numWaiting_;
  uSemaphore synch_;
};

#endif // EVENTCOUNTER_CC

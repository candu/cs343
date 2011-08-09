#ifndef EVENTCOUNTER_CC
#define EVENTCOUNTER_CC

#include <uC++.h>
#include <uSemaphore.h>

#include <iostream>

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
    cout << uThisTask().getName() << ": await(" << ticket <<
        "), lastEvent_ == " << lastEvent_ << endl;
    if (lastEvent_ < ticket) {
      numWaiting_.V();
      synch_.V();
      cout << uThisTask().getName() << ": must wait " <<
            (ticket - lastEvent_) << endl;
      for (uint32_t i = ticket; i > lastEvent_; i--) {
        wait_.P();
        cout << uThisTask().getName() << ": wait_.P(), must wait " <<
            (i - lastEvent_ - 1) << endl;
      }
      synch_.P();
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
      cout << uThisTask().getName() << ": V(" <<
          numWaiting << ")" << endl;
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

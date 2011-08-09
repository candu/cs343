#ifndef EVENTCOUNTERBOUNDEDBUFFER_CC
#define EVENTCOUNTERBOUNDEDBUFFER_CC

#include <uC++.h>

#include "BoundedBuffer.cc"
#include "Sequencer.cc"
#include "EventCounter.cc"

using namespace std;

/**
 * EventCounterLock is a Sequencer/EventCounter-based lock providing
 * acquire()/release() semantics for ownership.
 *
 * Let A be the number of calls to acquire(), R be the number of calls to
 * release(), and E the value of event passed to the constructor. Then the
 * following invariant is maintained at all times:
 *
 * A <= E + R
 *
 * Tasks need not acquire() before release(); any task may release() to
 * signal additional availability within the lock.
 */
class EventCounterLock {
 public:
  EventCounterLock(uint32_t event = 0)
    : S_(),
      E_(event) { }

  void acquire() {
    E_.await(S_.ticket());
  }
  void release() {
    E_.advance();
  }
 private:
  Sequencer S_;
  EventCounter E_;
};

template <typename T>
class EventCounterBoundedBuffer {
 public:
  EventCounterBoundedBuffer(const uint32_t capacity = 10);
  ~EventCounterBoundedBuffer();
  void insert(T elem);
  T remove();
 private:
  BoundedBuffer<T> buffer_;
  EventCounterLock lock_;
  EventCounterLock itemAvailable_;
  EventCounterLock slotFree_;
};

template <typename T>
EventCounterBoundedBuffer<T>::EventCounterBoundedBuffer(
    const uint32_t capacity)
  : buffer_(capacity),
    lock_(1),
    itemAvailable_(0),
    slotFree_(capacity) { }

template <typename T>
EventCounterBoundedBuffer<T>::~EventCounterBoundedBuffer() { }

template <typename T>
void EventCounterBoundedBuffer<T>::insert(T elem) { 
  slotFree_.acquire();

  lock_.acquire();
  buffer_.insert(elem);
  lock_.release();

  itemAvailable_.release();
}

template <typename T>
T EventCounterBoundedBuffer<T>::remove() {
  itemAvailable_.acquire();

  lock_.acquire();
  T ret = buffer_.remove();
  lock_.release();

  slotFree_.release();
  return ret;
}

#endif // EVENTCOUNTERBOUNDEDBUFFER_CC

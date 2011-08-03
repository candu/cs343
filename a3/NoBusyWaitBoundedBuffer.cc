#ifndef Q1_NOBUSYWAITBOUNDEDBUFFER_CC
#define Q1_NOBUSYWAITBOUNDEDBUFFER_CC

#include <uC++.h>
#include "BoundedBuffer.cc"

using namespace std;

template <typename T>
class NoBusyWaitBoundedBuffer {
 public:
  NoBusyWaitBoundedBuffer(const uint32_t capacity = 10);
  ~NoBusyWaitBoundedBuffer();
  void insert(T elem);
  T remove();
 private:
  BoundedBuffer<T> buffer_;
  uOwnerLock lock_;
  bool signal_;
  uCondLock barging_;
  uCondLock itemAvailable_;
  uCondLock slotFree_;
};

template <typename T>
NoBusyWaitBoundedBuffer<T>::NoBusyWaitBoundedBuffer(const uint32_t capacity)
  : buffer_(capacity),
    lock_(),
    signal_(false),
    barging_(),
    itemAvailable_(),
    slotFree_() { }

template <typename T>
NoBusyWaitBoundedBuffer<T>::~NoBusyWaitBoundedBuffer() { }

template <typename T>
void NoBusyWaitBoundedBuffer<T>::insert(T elem) {
  lock_.acquire();
  if (signal_) {
    cout << uThisTask().getName() << ": insert() waits on barging" << endl;
    barging_.wait(lock_);
    cout << uThisTask().getName() << ": insert() signalled from barging" << endl;
  }
  if (buffer_.full()) {
    cout << uThisTask().getName() << ": insert() waits on slotFree" << endl;
    slotFree_.wait(lock_);
    cout << uThisTask().getName() << ": insert() signalled from slotFree" << endl;
    signal_ = false;
  }

  buffer_.insert(elem);

  if (!itemAvailable_.empty()) {
    signal_ = true;
    itemAvailable_.signal();
  } else if (!barging_.empty()) {
    barging_.signal();
  }
  lock_.release();
}

template <typename T>
T NoBusyWaitBoundedBuffer<T>::remove() {
  lock_.acquire();
  if (signal_) {
    cout << uThisTask().getName() << ": remove() waits on barging" << endl;
    barging_.wait(lock_);
    cout << uThisTask().getName() << ": remove() signalled from barging" << endl;
  }
  if (buffer_.empty()) {
    cout << uThisTask().getName() << ": remove() waits on itemAvailable" << endl;
    itemAvailable_.wait(lock_);
    cout << uThisTask().getName() << ": remove() signalled from itemAvailable" << endl;
    signal_ = false;
  }

  T ret = buffer_.remove();

  if (!slotFree_.empty()) {
    signal_ = true;
    slotFree_.signal();
  } else if (!barging_.empty()) {
    barging_.signal();
  }
  lock_.release();

  return ret;
}

#endif // Q1_NOBUSYWAITBOUNDEDBUFFER_CC

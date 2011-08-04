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
  void enter();
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
    signal_(),
    barging_(),
    itemAvailable_(),
    slotFree_() { }

template <typename T>
NoBusyWaitBoundedBuffer<T>::~NoBusyWaitBoundedBuffer() { }

template <typename T>
void NoBusyWaitBoundedBuffer<T>::enter() {
  lock_.acquire();
  if (signal_) {
    barging_.wait(lock_);
    signal_ = false;
  }
}

template <typename T>
void NoBusyWaitBoundedBuffer<T>::insert(T elem) {
  enter();
  if (buffer_.full()) {
    if (!barging_.empty()) {
      signal_ = true;
      barging_.signal();
    }
    slotFree_.wait(lock_);
    signal_ = false;
  }

  buffer_.insert(elem);
  
  if (!itemAvailable_.empty()) {
    signal_ = true;
    itemAvailable_.signal();
  } else if (!barging_.empty()) {
    signal_ = true;
    barging_.signal();
  }
  lock_.release();
}

template <typename T>
T NoBusyWaitBoundedBuffer<T>::remove() {
  enter();
  if (buffer_.empty()) {
    if (!barging_.empty()) {
      signal_ = true;
      barging_.signal();
    }
    itemAvailable_.wait(lock_);
    signal_ = false;
  }

  T ret = buffer_.remove();

  if (!slotFree_.empty()) {
    signal_ = true;
    slotFree_.signal();
  } else if (!barging_.empty()) {
    signal_ = true;
    barging_.signal();
  }
  lock_.release();

  return ret;
}

#endif // Q1_NOBUSYWAITBOUNDEDBUFFER_CC

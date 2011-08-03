#ifndef Q1_BUSYWAITBOUNDEDBUFFER_CC
#define Q1_BUSYWAITBOUNDEDBUFFER_CC

#include <uC++.h>
#include "BoundedBuffer.cc"

using namespace std;

template <typename T>
class BusyWaitBoundedBuffer {
 public:
  BusyWaitBoundedBuffer(const uint32_t capacity = 10);
  ~BusyWaitBoundedBuffer();
  void insert(T elem);
  T remove();
 private:
  BoundedBuffer<T> buffer_;
  uOwnerLock lock_;
  uCondLock itemAvailable_;
  uCondLock slotFree_;
};

template <typename T>
BusyWaitBoundedBuffer<T>::BusyWaitBoundedBuffer(const uint32_t capacity)
  : buffer_(capacity),
    lock_(),
    itemAvailable_(),
    slotFree_() { }

template <typename T>
BusyWaitBoundedBuffer<T>::~BusyWaitBoundedBuffer() { }

template <typename T>
void BusyWaitBoundedBuffer<T>::insert(T elem) {
  lock_.acquire();
  while (true) {
    if (!buffer_.full()) {
      break;
    }
    slotFree_.wait(lock_);
  }

  buffer_.insert(elem);

  itemAvailable_.signal();
  lock_.release();
}

template <typename T>
T BusyWaitBoundedBuffer<T>::remove() {
  lock_.acquire();
  while (true) {
    if (!buffer_.empty()) {
      break;
    }
    itemAvailable_.wait(lock_);
  }

  T ret = buffer_.remove();

  slotFree_.signal();
  lock_.release();

  return ret;
}

#endif // Q1_BUSYWAITBOUNDEDBUFFER_CC

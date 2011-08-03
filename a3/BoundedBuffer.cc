#ifndef Q1_BOUNDEDBUFFER_CC
#define Q1_BOUNDEDBUFFER_CC

#include <uC++.h>
#include <cassert>
#include <iostream>

using namespace std;

// busy waiting implementation
template <typename T> class BoundedBuffer {
 public:
  BoundedBuffer(const uint32_t capacity = 10);
  ~BoundedBuffer();
  void insert(T elem);
  T remove();
  bool full() const;
  bool empty() const;
  template <typename U>
  friend ostream& operator<<(ostream& os, const BoundedBuffer<U>& buffer);
 private:
  T* buffer_;
  uint32_t head_;
  uint32_t tail_;
  uint32_t size_;
  uint32_t capacity_;
  uOwnerLock lock_;
  uCondLock itemAvailable_;
  uCondLock slotFree_;
};

template <typename U>
ostream& operator<<(ostream& os, const BoundedBuffer<U>& buffer) {
  os << "[";
  for (uint32_t i = 0; i < buffer.size_; i++) {
    if (i > 0) {
      os << ", ";
    }
    os << buffer.buffer_[(buffer.head_ + i) % buffer.capacity_];
  }
  os << "]";
  return os;
}

template <typename T>
BoundedBuffer<T>::BoundedBuffer(const uint32_t capacity)
  : buffer_(new T[capacity]),
    head_(0),
    size_(0),
    capacity_(capacity),
    lock_(),
    itemAvailable_(),
    slotFree_() { }

template <typename T>
BoundedBuffer<T>::~BoundedBuffer() {
  delete[] buffer_;
}

template <typename T>
void BoundedBuffer<T>::insert(T elem) {
  lock_.acquire();
  while (true) {
    if (!full()) {
      break;
    }
    slotFree_.wait(lock_);
  }
  assert(0 <= size_ && size_ < capacity_);

  buffer_[(head_ + size_) % capacity_] = elem;
  size_++;

  itemAvailable_.signal();
  lock_.release();
}

template <typename T>
T BoundedBuffer<T>::remove() {
  lock_.acquire();
  while (true) {
    if (!empty()) {
      break;
    }
    itemAvailable_.wait(lock_);
  }
  assert(0 < size_ && size_ <= capacity_);

  T ret = buffer_[head_];
  head_++;
  if (head_ == capacity_) {
    head_ = 0;
  }
  size_--;

  slotFree_.signal();
  lock_.release();

  return ret;
}

template <typename T>
bool BoundedBuffer<T>::full() const {
  return size_ == capacity_;
}

template <typename T>
bool BoundedBuffer<T>::empty() const {
  return size_ == 0;
}

#endif // Q1_BOUNDEDBUFFER_CC

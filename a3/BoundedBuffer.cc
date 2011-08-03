#ifndef Q1_BOUNDEDBUFFER_CC
#define Q1_BOUNDEDBUFFER_CC

#include <uC++.h>
#include <cassert>
#include <iostream>

using namespace std;

template <typename T>
class BoundedBuffer {
 public:
  BoundedBuffer(const uint32_t capacity = 10);
  ~BoundedBuffer();
  template <typename U>
  friend ostream& operator<<(ostream& os, const BoundedBuffer<U>& buffer);
  bool full() const {
    return size_ == capacity_;
  }
  bool empty() const {
    return size_ == 0;
  }
  void insert(T elem) {
    assert(0 <= size_ && size_ < capacity_);
    buffer_[(head_ + size_) % capacity_] = elem;
    size_++;
  }
  T remove() {
    assert(0 < size_ && size_ <= capacity_);
    T ret = buffer_[head_];
    head_++;
    if (head_ == capacity_) {
      head_ = 0;
    }
    size_--;
    return ret;
  }
 private:
  T* buffer_;
  uint32_t head_;
  uint32_t size_;
  uint32_t capacity_;
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
    capacity_(capacity) { }

template <typename T>
BoundedBuffer<T>::~BoundedBuffer() {
  delete[] buffer_;
}

#endif // Q1_BOUNDEDBUFFER_CC

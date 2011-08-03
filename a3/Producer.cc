#ifndef Q1_PRODUCER_CC
#define Q1_PRODUCER_CC

#include <uC++.h>
#include <cstdlib>

#include "BoundedBuffer.cc"

_Task Producer {
 public:
  Producer(BoundedBuffer<int32_t>& buffer,
           const int32_t produce,
           const int32_t delay)
    : buffer_(buffer),
      produce_(produce),
      delay_(delay) { }
 private:
  BoundedBuffer<int32_t>& buffer_;
  const int32_t produce_;
  const int32_t delay_;
  void main() {
    for (int32_t i = 1; i <= produce_; i++) {
      yield(rand() % delay_);
      buffer_.insert(i);
    }
  }
};

#endif // Q1_PRODUCER_CC


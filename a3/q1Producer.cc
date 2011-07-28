#include <uC++.h>
#include "q1BoundedBuffer.cc"

_Task Producer {
 public:
  Producer(BoundedBuffer<int>& buffer,
           const int32_t produce,
           const int32_t delay)
    : buffer_(buffer),
      produce_(produce),
      delay_(delay) {

  }
 private:
  BoundedBuffer<int>& buffer_;
  const int32_t produce_;
  const int32_t delay_;
  void main() {

  }
};

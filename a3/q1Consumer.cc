#include <uC++.h>
#include "q1BoundedBuffer.cc"

_Task Consumer {
 public:
  Consumer(BoundedBuffer<int>& buffer,
           const int32_t delay,
           const int32_t sentinel,
           int32_t& sum)
    : buffer_(buffer),
      delay_(delay),
      sentinel_(sentinel),
      sum_(sum) {
    // TODO: implement this
  }
 private:
  BoundedBuffer<int>& buffer_;
  const int32_t delay_;
  const int32_t sentinel_;
  int32_t& sum_;
  void main() {

  }
};

#ifndef Q1_CONSUMER_CC
#define Q1_CONSUMER_CC

#include <uC++.h>
#include <cstdlib>
#include <sstream>
#include <string>

#include "BoundedBuffer.cc"

using namespace std;

template <typename BufType>
_Task Consumer {
 public:
  Consumer(uint32_t id,
           BufType& buffer,
           const int32_t delay,
           const int64_t sentinel,
           int64_t& sum)
    : buffer_(buffer),
      delay_(delay),
      sentinel_(sentinel),
      sum_(sum),
      name_() {
    ostringstream ostr;
    ostr << "consumer-" << id;
    name_ = ostr.str();
    setName(name_.c_str());
  }
 private:
  BufType& buffer_;
  const int32_t delay_;
  const int64_t sentinel_;
  int64_t& sum_;
  string name_;
  void main() {
    sum_ = 0;
    while (true) {
      yield(rand() % delay_);
      int64_t value = buffer_.remove();
      if (value == sentinel_) {
        break;
      }
      sum_ += value;
    }
  }
};

#endif // Q1_CONSUMER_CC

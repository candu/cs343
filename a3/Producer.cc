#ifndef Q1_PRODUCER_CC
#define Q1_PRODUCER_CC

#include <uC++.h>
#include <cstdlib>
#include <sstream>
#include <string>

#include "BoundedBuffer.cc"

using namespace std;

template <typename BufType>
_Task Producer {
 public:
  Producer(uint32_t id,
           BufType& buffer,
           const int32_t produce,
           const int32_t delay)
    : buffer_(buffer),
      produce_(produce),
      delay_(delay),
      name_() {
    ostringstream ostr;
    ostr << "producer-" << id;
    name_ = ostr.str();
    setName(name_.c_str());
  }
 private:
  BufType& buffer_;
  const int32_t produce_;
  const int32_t delay_;
  string name_;
  void main() {
    for (int32_t i = 1; i <= produce_; i++) {
      yield(rand() % delay_);
      buffer_.insert(i);
    }
  }
};

#endif // Q1_PRODUCER_CC


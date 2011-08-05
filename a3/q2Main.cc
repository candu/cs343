#include <uC++.h>

#include <iostream>
#include <sstream>
#include <string>

#include "Sequencer.cc"

using namespace std;

_Task Customer {
 public:
  Customer(Sequencer& sequencer, uint32_t id)
    : sequencer_(sequencer) {
    ostringstream ostr;
    ostr << "customer-" << id;
    name_ = ostr.str();
    setName(name_.c_str());
  }
 private:
  Sequencer& sequencer_;
  string name_;

  void main() {
    for (uint32_t i = 0; i < 10; i++) {
      yield(rand() % 10);
      cout << uThisTask().getName() << " takes ticket " <<
          sequencer_.ticket() << endl;
    }
  }
};

void uMain::main() {
  uint32_t numCustomers = 5;
  switch (argc) {
   case 2:
    numCustomers = atoi(argv[1]);
   default:
    break;
  }

  Sequencer s;

  Customer* c[numCustomers];
  for (uint32_t i = 0; i < numCustomers; i++) {
    c[i] = new Customer(s, i);
  }
  for (uint32_t i = 0; i < numCustomers; i++) {
    delete c[i];
  }

}

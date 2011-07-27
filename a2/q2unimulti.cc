#include <uC++.h>
#include <iostream>

using namespace std;

int iterations = 8000000, shared = iterations;

_Task decrement {
  void main() {
    // ignore volatile, prevent dead code removal
    for (volatile int i = 1; i <= iterations; i += 1) {
      shared -= 1;
    }
  }
};

void uMain::main() {
  switch (argc) {
    case 2:
      iterations = atoi(argv[1]);
  }
#ifdef __U_MULTI__
  uProcessor p;   // create second kernel thread
#endif // __U_MULTI__
  {
    decrement t[2];
  } // wait for tasks to finish
  cout << "shared: " << shared << endl;
}

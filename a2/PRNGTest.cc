#include <iostream>

#include "PRNG.h"

using namespace std;

int main() {
  // start two PRNGs, since these share the same seed and are both
  // deterministic.
  PRNG r1;
  PRNG r2;

  // now we use the tortoise and hare algorithm to detect cycle length,
  // as a test of PRNG fitness.
  uint32_t i = 0;
  uint32_t tortoise, hare;
  do {
    tortoise = r1();
    r2();
    hare = r2();
    i++;
    if (!(i % 10000)) {
      cout << "i = " << i << endl;
    }
  } while (tortoise != hare);
  cout << "cycle of length " << i << " detected" << endl;
}

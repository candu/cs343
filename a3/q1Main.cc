#include <uC++.h>
#include <iostream>

#include "Producer.cc"
#include "Consumer.cc"

#ifdef BUSY
  #include "BusyWaitBoundedBuffer.cc"
  typedef BusyWaitBoundedBuffer<int64_t> Buffer;
#else
  #ifdef NOBUSY
    #include "NoBusyWaitBoundedBuffer.cc"
    typedef NoBusyWaitBoundedBuffer<int64_t> Buffer;
  #else
    #include "BoundedBuffer.cc"
    typedef BoundedBuffer<int64_t> Buffer;
  #endif
#endif

using namespace std;

void uMain::main() {
  uint32_t numCons = 5;
  uint32_t numProd = 3;
  uint32_t numItemsProduced = 10;
  uint32_t bufferSize = 10;
  uint32_t delays = 0;
  bool delaysSet = false;

  switch (argc) {
    case 6:
      delays = atoi(argv[5]);
      delaysSet = true;
    case 5:
      bufferSize = atoi(argv[4]);
    case 4:
      numItemsProduced = atoi(argv[3]);
    case 3:
      numProd = atoi(argv[2]);
    case 2:
      numCons = atoi(argv[1]);
  }
  if (!delaysSet) {
    delays = numCons + numProd;
  }
  
  Buffer buffer(bufferSize);
  int64_t sentinel = -1;
  int64_t partialSums[numCons];
  Consumer<Buffer>* consumers[numCons];
  for (uint32_t i = 0; i < numCons; i++) {
    consumers[i] = new Consumer<Buffer>(
        i, buffer, delays, sentinel, partialSums[i]);
  }
  Producer<Buffer>* producers[numProd];
  for (uint32_t i = 0; i < numProd; i++) {
    producers[i] = new Producer<Buffer>(
        i, buffer, numItemsProduced, delays);
  }
  for (uint32_t i = 0; i < numProd; i++) {
    delete producers[i];
  }
  for (uint32_t i = 0; i < numCons; i++) {
    buffer.insert(sentinel);
  }
  for (uint32_t i = 0; i < numCons; i++) {
    delete consumers[i];
  }

  int64_t sum = 0;
  for (uint32_t i = 0; i < numCons; i++) {
    sum += partialSums[i];
  }
  cout << "sum: " << sum << endl;
}

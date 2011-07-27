#include <uC++.h>
#include <boost/scoped_ptr.hpp>
#include <iostream>

using namespace boost;
using namespace std;

template<typename T>
_Task QuickSort {
 public:
  // this task will sort T[low, high)
  QuickSort(T array[], int low, int high)
    : array_(array),
      low_(low),
      high_(high),
      subTask_() { }
 private:
  T* array_;
  int low_;
  int high_;
  scoped_ptr<QuickSort> subTask_;
  void swap(int a, int b) {
    T temp = array_[a];
    array_[a] = array_[b];
    array_[b] = temp;
  }
  void main() {
    while (true) {
      // check for base case
      int left = low_;
      int right = high_ - 1;
      if (left >= right) {
        break;
      }
      // partition around pivot
      int mid = low_ + (high_ - low_) / 2;
      T pivot = array_[mid];
      swap(mid, right);
      for (int i = left; i < right; i++) {
        if (array_[i] < pivot) {
          swap(i, left);
          left++;
        }
      }
      swap(left, right);
      mid = left;
      left = low_;

      // invoke new task to sort left partition
      subTask_.reset(new QuickSort(array_, left, mid));
    
      // sort right partition
      low_ = mid;
    }
  }
};

template<typename T>
void printArray(T array[], int n) {
  for (int i = 0; i < n; i++) {
    cout << array[i] << " ";
  }
  cout << endl;
}

void uMain::main() {
  int n;
  while (true) {
    cin >> n;
    if (cin.eof()) {
      break;
    }
    if (n == 0) {
      continue;
    }
    int* array = new int[n];
    for (int i = 0; i < n; i++) {
      cin >> array[i];
    }
    printArray(array, n);
    {
      QuickSort<int> q(array, 0, n);
    }
    printArray(array, n);
    delete[] array;
  }
}

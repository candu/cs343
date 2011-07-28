#include <uC++.h>

template<typename T> class BoundedBuffer {
 public:
  BoundedBuffer(const uint32_t size = 10);
  void insert(T elem);
  T remove();
 private:
  // TODO: add things here
};

#ifdef BUSY   // busy waiting implementation

BoundedBuffer::BoundedBuffer(const uint32_t size) {

}

template <typename T>
void BoundedBuffer::insert(T elem) {

}

template <typename T>
T BoundedBuffer::remove() {

}

#endif

#ifdef NOBUSY // no busy waiting implementation

BoundedBuffer::BoundedBuffer(const uint32_t size) {

}

template <typename T>
void BoundedBuffer::insert(T elem) {

}

template <typename T>
T BoundedBuffer::remove() {

}

#endif

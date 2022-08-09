#include <cstddef>
#include "AverageBuffer.h"

template class AverageBuffer<float>;

template <class T>
AverageBuffer<T>::AverageBuffer(size_t size, T defaultValue) {
  this->bufferSize = size;
  this->values = new T[size];
  for(size_t i = 0; i < size; i++) {
    values[i] = defaultValue;
  }
}

template <class T>
void AverageBuffer<T>::addValue(T value) {
  values[this->circularIdx] = value;
  this->circularIdx++;
  this->circularIdx = this->circularIdx % this->bufferSize;
}

template <class T>
T AverageBuffer<T>::getAverageValue(void) {
  T sum = 0;
  for(size_t i = 0; i < this->bufferSize; i++) {
    sum += values[i];
  }

  return sum / bufferSize;
}

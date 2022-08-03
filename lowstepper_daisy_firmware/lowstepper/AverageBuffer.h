#ifndef AVERAGE_BUFFER_H
#define AVERAGE_BUFFER_H

template <class T>
class AverageBuffer {
  public:
    AverageBuffer(size_t size, T defaultValue);
    void addValue(T value);
    T getAverageValue();
  private:
    T *values;
    size_t bufferSize;
    uint64_t circularIdx = 0;
};

#endif
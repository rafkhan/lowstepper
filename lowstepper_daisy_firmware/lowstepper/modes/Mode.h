#ifndef MODE_H
#define MODE_H

#include <stdint.h>
#include "BaseMode.h"

template <class T>
class Mode {
  public:
    T handlerA;
    T handlerB;
    uint8_t ledRed;
    uint8_t ledGreen;
    uint8_t ledBlue;

  Mode(T a, T b); 
};

template<>
Mode<BaseMode*>::Mode(BaseMode* a, BaseMode* b) {
  this->handlerA = a;
  this->handlerB = b;
}

#endif
#ifndef MODES_H
#define MODES_H

#ifdef __EMSCRIPTEN__
#include "ui/ui_emscripten.h"
#else
#include "ui/ui.h"
#endif

class Mode {
  public:
    float tick(UI ui);
  private:
    virtual uint32_t getTime(void);
    virtual void writeToDAC(int value);
};

#endif
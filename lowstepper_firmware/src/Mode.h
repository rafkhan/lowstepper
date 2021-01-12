#ifndef MODES_H
#define MODES_H

#ifdef __EMSCRIPTEN__
#include "ui_emscripten.h"
#else
#include "ui/ui.h"
#endif

class Mode {
  public:
    void tick(UI* ui);
};

#endif
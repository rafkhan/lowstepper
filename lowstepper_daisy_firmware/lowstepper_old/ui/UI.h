#ifndef UI_H
#define UI_H

#include "GateIn.h"
#include "GateOut.h"
#include "PotIn.h"
#include "CVIn.h"
#include "CVOut.h"

class UI
{
public:
  UI(void);

  void init(void);
  void scan(void);
  int combineAnalogInputs(int, int);

  //Outputs
  CVOut* cvOutA;
  CVOut* cvOutB;

  // EOC
  GateOut* eocA;
  GateOut* eocB;

  // TRIGGER INPUTS
  GateIn* trigInA;
  GateIn* clockInA;
  GateIn* resetInA;

  GateIn* trigInB;
  GateIn* clockInB;
  GateIn* resetInB;

  // POT INPUTS
  PotIn* potInRateA;
  PotIn* potInMorphA;
  PotIn* potInChunksA;

  PotIn* potInRateB;
  PotIn* potInMorphB;
  PotIn* potInChunksB;

  // CV INPUTS
  CVIn* cvInRateA;
  CVIn* cvInMorphA;
  CVIn* cvInChunksA;

  CVIn* cvInRateB;
  CVIn* cvInMorphB;
  CVIn* cvInChunksB;
};

#endif
#ifndef UI_EMSCRIPTEN
#define UI_EMSCRIPTEN

#define PIN_DAC1 0
#define PIN_DAC0 0
#define PIN_TRIG_IN 0
#define PIN_CLOCK_IN 0 // TODO build
#define BLOCK_THIS_PIN_TEMPORARILY 0
#define PIN_RATE_POT  0           // TODO build
#define PIN_SEGMENT_DIVIDE_POT 0 // TODO build
#define PIN_MORPH_POT 0          // TODO build

#define PI 3.14159265359
#define TWO_PI 6.283185307180

double map(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

class PotIn
{
private:
  int pin;
  double value;
public:
  PotIn(int pinNum);
  ~PotIn();

  void init(void);
  void scan(void);
  double getValue();
};

class CVIn
{
private:
  int pin;
  double value;
public:
  CVIn(int pinNum);
  ~CVIn();

  void init(void);
  void scan(void);
  double getValue();
};

class GateIn 
{
private:
  int pin;
  bool isPinLow;
  bool isTrigHigh;
public:
  GateIn(int pinNum);
  ~GateIn();

  void init(void);
  void scan(void);
  bool checkGateHigh(void);
  bool checkTrigHigh(void);
};

class UI
{
public:
  UI(void);

  void init(void);
  void scan(void);

  GateIn* trigIn;
  GateIn* clockIn;
  PotIn* potInRate;
  PotIn* potInSegmentDivide;
  PotIn* potInMorph;
  CVIn* cvInRate;
  CVIn* cvInSegmentDivide;
  CVIn* cvInMorph;
};

#endif
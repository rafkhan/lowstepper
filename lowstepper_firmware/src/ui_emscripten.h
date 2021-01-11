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


#define PI 0
#define TWO_PI 0
double micros(void) {
  return 0.0;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
double analogWrite(int pin, int value) {
  return 0.0;
}

class PotInput 
{
private:
  int pin;
  double value;
public:
  PotInput(int pinNum);
  ~PotInput();

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
  PotInput* potInRate;
  PotInput* potInSegmentDivide;
  PotInput* potInMorph;
};

UI::UI()
{
  this->trigIn = new GateIn(PIN_TRIG_IN);
  this->clockIn = new GateIn(PIN_CLOCK_IN);
  this->potInRate = new PotInput(PIN_RATE_POT);
  this->potInSegmentDivide = new PotInput(PIN_SEGMENT_DIVIDE_POT);
  this->potInMorph = new PotInput(PIN_MORPH_POT);
}

void UI::init(void) {
  trigIn->init();
  clockIn->init();
  potInRate->init();
  potInSegmentDivide->init();
  potInMorph->init();
}

void UI::scan(void)
{
  trigIn->scan();
  clockIn->scan();
  potInRate->scan();
  potInSegmentDivide->scan();
  potInMorph->scan();
}

#endif
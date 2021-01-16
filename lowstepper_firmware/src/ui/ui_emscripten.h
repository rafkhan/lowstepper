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
#define PIN_RATE_CV 0
#define PIN_SEGMENT_DIVIDE_CV 0
#define PIN_MORPH_CV 0


/**
 * ARDUINO STUFF
 */
#define PI 3.14159265359
#define TWO_PI 6.283185307180

double map(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * ANALOG INPUT CLASSES
 */
class AnalogIn
{
private:
  int pin;
  double value;
public:
  AnalogIn(int pinNum);
  ~AnalogIn();

  void init(void);
  void scan(void);
  double getValue();
  void setValue(double value);
};

AnalogIn::AnalogIn(int pinNum) {
  this->pin = pinNum;
  this->value = 0;
}

AnalogIn::~AnalogIn() {}

void AnalogIn::init() {
  return;
}

void AnalogIn::scan() {
  return;
}

double AnalogIn::getValue() {
  return this->value;
}

void AnalogIn::setValue(double value) {
  this->value = value;
}

typedef AnalogIn PotIn;
typedef AnalogIn CVIn;

/**
 * GATE/TRIGGER 
 */
class GateIn 
{
private:
  int pin;
  bool isPinLow;
  bool isTrigHigh;
  bool value;
public:
  GateIn(int pinNum);
  ~GateIn();

  void init(void);
  void scan(void);
  bool checkGateHigh(void);
  bool checkTrigHigh(void);

  void setValue(bool);

private:
  int getValue(void);
};


GateIn::GateIn(int pinNum) {
  this->isPinLow = false;
  this->isTrigHigh = false;
  this->value = true;
}
GateIn::~GateIn() {}
void GateIn::init() {}

int GateIn::getValue()
{
  return this->value;
}

void GateIn::setValue(bool value)
{
  this->value = value;
}

void GateIn::scan(void) {
  // int value = digitalReadFast(this->pin);
  int value = this->getValue();

  // If the pin goes low, but isn't already low, toggle gate on.
  if(!value && !this->isPinLow) {
    this->isPinLow = true;
    this->isTrigHigh = true;
    return;
  }

  // pin has gone back high (gate is low) Reset isPinLow so trig can happen again
  if(value) {
    this->isPinLow = false;
    return;
  }
}

// Trigs can only be checked once, to avoid infinite retrigger when kept input kept low
bool GateIn::checkTrigHigh(void) {
  if(this->isTrigHigh) {
    this->isTrigHigh = false;
    return true;
  }

  return false;
}

// Gates are high if the pin is low
bool GateIn::checkGateHigh(void) {
  return isPinLow;
}


/**
 * UI general.
 */
class UI
{
public:
  UI(void);

  void init(void);
  void scan(void);
  void setGate(int);

  GateIn* trigIn;
  GateIn* clockIn;
  PotIn* potInRate;
  PotIn* potInSegmentDivide;
  PotIn* potInMorph;
  CVIn* cvInRate;
  CVIn* cvInSegmentDivide;
  CVIn* cvInMorph;
};

UI::UI() {
  this->trigIn = new GateIn(PIN_TRIG_IN);
  this->clockIn = new GateIn(PIN_CLOCK_IN);
  this->potInRate = new PotIn(PIN_RATE_POT);
  this->potInSegmentDivide = new PotIn(PIN_SEGMENT_DIVIDE_POT);
  this->potInMorph = new PotIn(PIN_MORPH_POT);
  this->cvInRate = new CVIn(PIN_RATE_POT);
  this->cvInSegmentDivide = new CVIn(PIN_SEGMENT_DIVIDE_CV);
  this->cvInMorph = new CVIn(PIN_MORPH_CV);
}

void UI::init(void) {
  trigIn->init();
  clockIn->init();
  potInRate->init();
  potInSegmentDivide->init();
  potInMorph->init();
  cvInRate->init();
  cvInSegmentDivide->init();
  cvInMorph->init();
}

// get the everything from external hardware
void UI::scan(void) {
  trigIn->scan();
  clockIn->scan();
  potInRate->scan();
  potInSegmentDivide->scan();
  potInMorph->scan();
  cvInRate->scan();
  cvInSegmentDivide->scan();
  cvInMorph->scan();
}

void UI::setGate(int v) {
  this->trigIn->setValue(v);
}

#endif
#include <cstddef>
#include "daisy_seed.h"
#include "daisysp.h"

#include "lowstepper/util.h"
#include "lowstepper/LowStepper.h"
#include "lowstepper/LowStepperChannel.h"

#define DEBUG 1

uint16_t mapFFII(float x, float in_min, float in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

using namespace daisy;
using namespace daisysp;

int eocAValue = 1;
int eocBValue = 0;

// hardware objects
static DaisySeed hw;

dsy_gpio gpioResetA;
uint8_t gpioResetAValue = 0;

dsy_gpio gpioResetDetectA;
uint8_t gpioResetDetectAValue = 0;

// dsy_gpio gate_output;
// Metro tick;
// bool metroValue = false;

// Config
float sampleRate;
float sampleIncrementMicros;

// Globals lol
float cvCh1 = 0;
float cvCh2 = 0;
float cvIn = 0;

float mux1In0 = 0;
float mux1In1 = 0;
float mux1In2 = 0;
float mux1In3 = 0;
float mux1In4 = 0;
float mux1In5 = 0;
float mux1In6 = 0;
float mux1In7 = 0;

float mux2In0 = 0;
float mux2In1 = 0;
float mux2In2 = 0;
float mux2In3 = 0;
float mux2In4 = 0;
float mux2In5 = 0;
float mux2In6 = 0;
float mux2In7 = 0;

// PIN MAPPING
const int MUX1ADC = 28;
const int MUX1S0 = 27;
const int MUX1S1 = 26;
const int MUX1S2 = 25;

const int MUX2ADC = 24;
const int MUX2S0 = 3;
const int MUX2S1 = 2;
const int MUX2S2 = 1;


#define PIN_RESET_A 13	
#define PIN_RESET_DETECT_A 8

#define PIN_EOC_A 20
#define PIN_EOC_B 19

const size_t channelCount = 2;
LowStepperOutput outputs[channelCount] = { NULL, NULL };
LowStepperOutput o { 0, 0, false };
LowStepperOutput o2 { 0, 0, false };
LowStepper *lowStepper;

int counter = 0;

void readAdc() {
	counter = (counter + 1) % 8;
	switch(counter) {
		case 0:
			mux1In0 = hw.adc.GetMuxFloat(0, 0);
			break;
		case 1:
			mux1In1 = hw.adc.GetMuxFloat(0, 1);
			break;
		case 2:
			mux1In2 = hw.adc.GetMuxFloat(0, 2);
			break;
		case 3:
			mux1In3 = hw.adc.GetMuxFloat(0, 3);
			break;
		case 4:
			mux1In4 = hw.adc.GetMuxFloat(0, 4);
			break;
		case 5:
			mux1In5 = hw.adc.GetMuxFloat(0, 5);
			break;
		case 6:
			mux1In6 = hw.adc.GetMuxFloat(0, 6);
			break;
		case 7:
			mux1In7 = hw.adc.GetMuxFloat(0, 7);
			break;
	}

	switch(counter) {
		case 0:
			mux2In0 = hw.adc.GetMuxFloat(1, 0);
			break;
		case 1:
			mux2In1 = hw.adc.GetMuxFloat(1, 1);
			break;
		case 2:
			mux2In2 = hw.adc.GetMuxFloat(1, 2);
			break;
		case 3:
			mux2In3 = hw.adc.GetMuxFloat(1, 3);
			break;
		case 4:
			mux2In4 = hw.adc.GetMuxFloat(1, 4);
			break;
		case 5:
			mux2In5 = hw.adc.GetMuxFloat(1, 5);
			break;
		case 6:
			mux2In6 = hw.adc.GetMuxFloat(1, 6);
			break;
		case 7:
			mux2In7 = hw.adc.GetMuxFloat(1, 7);
			break;
	}
}

void AudioCallback(AudioHandle::InterleavingInputBuffer in,
									 AudioHandle::InterleavingOutputBuffer out,
									 size_t size)
{

	// if(tick.Process()) {
	// 	dsy_gpio_write(&gate_output, metroValue); 
	// 	metroValue = !metroValue;
	// }

	gpioResetAValue = dsy_gpio_read(&gpioResetA);
	gpioResetDetectAValue = dsy_gpio_read(&gpioResetDetectA);
	readAdc();

	// Read hardware state into memory for entire block of samples

	for (size_t n = 0; n < size; n += 2)
	{
		LowStepperInput input;
		input.frequency = 100;
		input.phase = outputs[0].phase;
		input.start = 0.8f;
		input.length = 0.5;

		LowStepperInput input2;
		input2.frequency = 20;
		input2.phase = outputs[1].phase;
		input2.start = 0.1f;
		input2.length = 1;

		LowStepperInput inputs[2] = { input, input2 };

		// Process inputs array, dump into outputs array
		lowStepper->tick(inputs, outputs);

		float y = outputs[0].cvOutput;
		cvCh1 = outputs[0].cvOutput;
		cvCh2 = outputs[1].cvOutput;

		// Don't do anything on audio output
		out[n] = 0;
		out[n + 1] = 0;
	}
}

void initAdc() {
	AdcChannelConfig adc[2];
	adc[0].InitMux(hw.GetPin(MUX1ADC), 8, hw.GetPin(MUX1S0), hw.GetPin(MUX1S1), hw.GetPin(MUX1S2));
	adc[1].InitMux(hw.GetPin(MUX2ADC), 8, hw.GetPin(MUX2S0), hw.GetPin(MUX2S1), hw.GetPin(MUX2S2));
	hw.adc.Init(adc, 2);
	hw.adc.Start();
}

void initGpioIn() {
	gpioResetA.pin  = hw.GetPin(PIN_RESET_A);
	gpioResetA.mode = DSY_GPIO_MODE_INPUT;
	gpioResetA.pull = DSY_GPIO_PULLUP;
	dsy_gpio_init(&gpioResetA);

	gpioResetDetectA.pin  = hw.GetPin(PIN_RESET_DETECT_A);
	gpioResetDetectA.mode = DSY_GPIO_MODE_INPUT;
	gpioResetDetectA.pull = DSY_GPIO_PULLUP;
	dsy_gpio_init(&gpioResetDetectA);
}

void initGpioOut() {
	// gpioOutEocA.pin  = hw.GetPin(PIN_EOC_A);
	// gpioOutEocA.mode = DSY_GPIO_MODE_OUTPUT_PP;
	// gpioOutEocA.pull = DSY_GPIO_NOPULL;
	// dsy_gpio_init(&gpioOutEocA);

	// gpioOutEocB.pin  = hw.GetPin(PIN_EOC_B);
	// gpioOutEocB.mode = DSY_GPIO_MODE_OUTPUT_PP;
	// gpioOutEocB.pull = DSY_GPIO_NOPULL;
	// dsy_gpio_init(&gpioOutEocB);

	// gpioOutEocA.Init() 
}

void initDac() {
	// Init DAC
	DacHandle::Config cfg;
	cfg.bitdepth = DacHandle::BitDepth::BITS_12;
	cfg.buff_state = DacHandle::BufferState::ENABLED;
	cfg.mode = DacHandle::Mode::POLLING;
	cfg.chn = DacHandle::Channel::BOTH;
	hw.dac.Init(cfg);
	hw.dac.WriteValue(DacHandle::Channel::BOTH, 0);
	hw.dac.WriteValue(DacHandle::Channel::ONE, 0); // CV0
	hw.dac.WriteValue(DacHandle::Channel::TWO, 0); // CV1
}

void writeDac(void) {
	uint16_t y1 = mapFFII(cvCh1, -1.0, 1.0, 0, 4095);
	uint16_t y2 =	mapFFII(cvCh2, -1.0, 1.0, 0, 4095);
	hw.dac.WriteValue(DacHandle::Channel::ONE, y1);
	hw.dac.WriteValue(DacHandle::Channel::TWO, y2);
}

int main(void)
{
	outputs[0] = o;
	outputs[1] = o2;

	// Init Daisy
	hw.Configure();
	hw.Init();
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	sampleRate = hw.AudioSampleRate();

	// tick.Init(1.f, sampleRate);
	// gate_output.pin  = hw.GetPin(PIN_EOC_A);
	// gate_output.mode = DSY_GPIO_MODE_OUTPUT_PP;
	// gate_output.pull = DSY_GPIO_PULLDOWN;
	// dsy_gpio_init(&gate_output);

#if DEBUG
  hw.StartLog();
	System::Delay(500);
#endif

	initDac();
	initAdc();
	initGpioIn();
	initGpioOut();

	// NOTE TO SELF: Do not move from main, application will segfault lol
	LowStepperChannel *channelA = new LowStepperChannel(sampleRate); // don't use this
	LowStepperChannel *channelB = new LowStepperChannel(sampleRate);; // don't use this
	LowStepperChannel *lsChannels[channelCount] = { channelA, channelB }; // use this
	lowStepper = new LowStepper(lsChannels, channelCount);

	hw.SetAudioBlockSize(1);
	hw.StartAudio(AudioCallback);
 
	for (;;)
	{
		writeDac();


#if DEBUG
		// hw.PrintLine("1: %f\t2: %f\t3: %f\t4: %f\t5:%f\t6:%f\t7:%f\t8:%f\t", mux1In0, mux1In1, mux1In2, mux1In3, mux1In4, mux1In5, mux1In6, mux1In7);
		// hw.PrintLine("1: %f\t2: %f\t3: %f\t4: %f\t5:%f\t6:%f\t7:%f\t8:%f\t", mux2In0, mux2In1, mux2In2, mux2In3, mux2In4, mux2In5, mux2In6, mux2In7);
		// hw.PrintLine("RESET A: %d\tRESET DETECT A:%d", gpioResetAValue, gpioResetDetectAValue);
		// hw.PrintLine("%d / %d", eocAValue, eocBValue);
		// System::Delay(10);
#endif
	}

}

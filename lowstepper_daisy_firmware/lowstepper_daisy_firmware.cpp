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

#define PIN_GATE_IN 2 // this is the daisy pin number

using namespace daisy;
using namespace daisysp;

// hardware objects
static DaisySeed hw;
dsy_gpio gate_in;

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

const int MUX1ADC = 28;
const int MUX1S0 = 27;
const int MUX1S1 = 26;
const int MUX1S2 = 25;

const int MUX2ADC = 24;
const int MUX2S0 = 3;
const int MUX2S1 = 2;
const int MUX2S2 = 1;

const int SYNC_A = 14;
const int SYNC_B = 12;
const int RESET_A = 15;
const int RESET_B = 16;

const int RESET_B_DETECT = 6;
const int SYNC_B_DETECT = 7;
const int RESET_A_DETECT = 8;
const int SYNC_A_DETECT = 9;

uint8_t x = 0;

const size_t channelCount = 2;
LowStepperOutput outputs[channelCount] = { NULL, NULL };
LowStepperOutput o { 0, 0, false };
LowStepperOutput o2 { 0, 0, false };
LowStepper *lowStepper;

int counter = 0;

void AudioCallback(AudioHandle::InterleavingInputBuffer in,
									 AudioHandle::InterleavingOutputBuffer out,
									 size_t size)
{

	x = dsy_gpio_read(&gate_in);
	counter = (counter + 1) % 7;

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
			mux2In1 = hw.adc.GetMuxFloat(1, 0);
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

		// Don't do anything.
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
	gate_in.pin  = hw.GetPin(1);
	gate_in.mode = DSY_GPIO_MODE_INPUT;
	gate_in.pull = DSY_GPIO_PULLUP;
	dsy_gpio_init(&gate_in);
}

void initGpioOut() {

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

#if DEBUG
  hw.StartLog();
	// Logger<LOGGER_SEMIHOST>::StartLog();
	System::Delay(500);
#endif

	initDac();
	initAdc();
	initGpioIn();
	initGpioOut();

	// Do not remove from main, application will segfault lol
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
		// Logger<LOGGER_SEMIHOST>::PrintLine("test");
		// hw.PrintLine("RATE: %f\tMORPH: %f\tSTART: %f\tEND: %f\t", ratePot, morphPot, startPot, endPot);
		// hw.PrintLine("RATE2: %f\tMORPH2: %f\tSTART2: %f\tEND2: %f\t", ratePot2, morphPot2, startPot2, endPot2);
		// hw.PrintLine("%f", mux2In5);
		// System::Delay(10);
#endif
	}

}

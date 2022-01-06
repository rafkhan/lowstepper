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

float ratePot = 0;
float morphPot = 0;
float startPot = 0;
float endPot = 0;

uint8_t x = 0;

const size_t channelCount = 2;
LowStepperOutput outputs[channelCount] = { NULL, NULL };
LowStepper *lowStepper;

// Init output array
LowStepperOutput o { 0, 0, false };
LowStepperOutput o2 { 0, 0, false };
// o.phase = 0;
// o.cvOutput = 0;
// o.eocGateHigh = false;


void AudioCallback(AudioHandle::InterleavingInputBuffer in,
									 AudioHandle::InterleavingOutputBuffer out,
									 size_t size)
{

	x = dsy_gpio_read(&gate_in);

	// cvIn = hw.adc.GetFloat(0);

	// ratePot = hw.adc.GetMuxFloat(0, 0);
	// morphPot = hw.adc.GetMuxFloat(0, 1);
	// startPot = hw.adc.GetMuxFloat(0, 2);
	// endPot = hw.adc.GetMuxFloat(0, 3);

	// Read hardware state into memory for entire block of samples

	for (size_t n = 0; n < size; n += 2)
	{
		LowStepperInput input;
		input.frequency = 1;
		input.phase = outputs[0].phase;
		input.start = 0.8f;
		input.length = 0.5;

		LowStepperInput input2;
		input2.frequency = 2;
		input2.phase = outputs[1].phase;
		input2.start = 0.1f;
		input2.length = 1;
		LowStepperInput inputs[2] = { input, input2 };

		// Process inputs array, dump into outputs array
		lowStepper->tick(inputs, outputs);

		float y = outputs[0].cvOutput;
		cvCh1 = outputs[0].cvOutput;
		cvCh2 = outputs[1].cvOutput;
		out[n] = y;
		out[n + 1] = y * -1.0f;
	}
}

void initAdc() {
	// Init ADC
	AdcChannelConfig adc[1];
	// adc[0].InitSingle(hw.GetPin(15));
	adc[0].InitMux(hw.GetPin(15), 4, hw.GetPin(14), hw.GetPin(13), hw.GetPin(12));
	hw.adc.Init(adc, 1);
	hw.adc.Start();
}

void initGpioIn() {
	// INIT GPIO
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

void initLowstepperApplication(void) {

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
	hw.SetAudioBlockSize(32);
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	sampleRate = hw.AudioSampleRate();

#if DEBUG
  hw.StartLog();
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

	hw.StartAudio(AudioCallback);
 
	for (;;)
	{
		writeDac();

		// hw.PrintLine("RATE: %f\tMORPH: %f\tSTART: %f\tEND: %f\t", ratePot, morphPot, startPot, endPot);
		// if(x) {
		// 	hw.PrintLine("HIGH");
		// } else {
		// 	hw.PrintLine("LOW");
		// }
	}
}
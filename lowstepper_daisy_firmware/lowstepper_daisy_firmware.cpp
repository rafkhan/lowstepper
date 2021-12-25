#include <cstddef>
#include "daisy_seed.h"
#include "daisysp.h"

#include "lowstepper/util.h"
#include "lowstepper/LowStepper.h"
#include "lowstepper/LowStepperChannel.h"

uint16_t mapFFII(float x, float in_min, float in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


#define PIN_GATE_IN 2 // this is the daisy pin number

using namespace daisy;
using namespace daisysp;

// hardware objects
static DaisySeed seed;
dsy_gpio gate_in;

// Config
float sampleRate;
float sampleIncrementMicros;

// Globals lol
float cvCh1 = 0;
float cvCh2 = 0;

LowStepperOutput outputs[2] = { NULL, NULL };
LowStepperChannel channelA; // don't use this
LowStepperChannel channelB; // don't use this
LowStepperChannel lsChannels[2] = { channelA, channelB }; // use this
LowStepper lowStepper{lsChannels, 2};

void AudioCallback(AudioHandle::InterleavingInputBuffer in,
									 AudioHandle::InterleavingOutputBuffer out,
									 size_t size)
{

	// x = dsy_gpio_read(&gate_in);

	for (size_t n = 0; n < size; n += 2)
	{
		LowStepperInput input;
		input.frequency = 1;
		input.phase = outputs[0].phase;
		LowStepperInput inputs[2] = { input, input };

		lowStepper.tick(inputs, outputs);

		float y = outputs[0].cvOutput;
		out[n] = y;
		out[n + 1] = y * -1.0f;
	}
}

int main(void)
{
	LowStepperOutput o;
	o.phase = 0;
	o.cvOutput = 0;
	o.eocGateHigh = false;

	LowStepperOutput o2;
	o2.phase = 0;
	o2.cvOutput = 0;
	o2.eocGateHigh = false;

	outputs[0] = o;
	outputs[1] = o2;
	seed.Configure();
	seed.Init();
	seed.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	sampleRate = seed.AudioSampleRate();

	// INIT DAC
	DacHandle::Config cfg;
	cfg.bitdepth = DacHandle::BitDepth::BITS_12;
	cfg.buff_state = DacHandle::BufferState::ENABLED;
	cfg.mode = DacHandle::Mode::POLLING;
	cfg.chn = DacHandle::Channel::BOTH;
	seed.dac.Init(cfg);
	seed.dac.WriteValue(DacHandle::Channel::BOTH, 0);
	seed.dac.WriteValue(DacHandle::Channel::ONE, 0); // CV0
	seed.dac.WriteValue(DacHandle::Channel::TWO, 0); // CV1

	// INIT GPIO
	// gate_in.pin  = seed.GetPin(PIN_GATE_IN);
	// gate_in.mode = DSY_GPIO_MODE_INPUT;
	// gate_in.pull = DSY_GPIO_PULLUP;
	// dsy_gpio_init(&gate_in);

	// osc.Init(sampleRate);
	// osc.SetWaveform(osc.WAVE_SIN);
	// osc.SetFreq(0.1);
	// osc.SetAmp(1);

	// initializeLowStepperApplication();

	seed.StartAudio(AudioCallback);

	for (;;)
	{
		//Send the latest envelope values to the CV outs
		uint16_t y1 = mapFFII(cvCh1, -1.0, 1.0, 0, 4095);
		uint16_t y2 =	mapFFII(cvCh2, -1.0, 1.0, 0, 4095);
		seed.dac.WriteValue(DacHandle::Channel::ONE, y1);
		seed.dac.WriteValue(DacHandle::Channel::TWO, y2);
	}
}
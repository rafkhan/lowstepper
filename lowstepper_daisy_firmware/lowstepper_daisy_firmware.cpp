#include "daisy_seed.h"
#include "daisysp.h"

#include "lowstepper/util.h"

using namespace daisy;
using namespace daisysp;

static DaisySeed seed;
static Oscillator osc;
float sampleRate;

#define PIN_GATE_IN 2 // this is the daisy pin number
dsy_gpio gate_in;

float cvCh1 = 0;
float cvCh2 = 0;
uint8_t x = 0;

void AudioCallback(AudioHandle::InterleavingInputBuffer in,
									 AudioHandle::InterleavingOutputBuffer out,
									 size_t size)
{

	x = dsy_gpio_read(&gate_in);

	for (size_t n = 0; n < size; n += 2)
	{
		float y = osc.Process();
		cvCh1 = y;
		cvCh2 = y * -1.0f;
		out[n] = y;
		out[n + 1] = y * -1.0f;
	}
}

int main(void)
{
	seed.Configure();
	seed.Init();
	sampleRate = seed.AudioSampleRate();

	// init DAC outputs
	DacHandle::Config cfg;
	cfg.bitdepth = DacHandle::BitDepth::BITS_12;
	cfg.buff_state = DacHandle::BufferState::ENABLED;
	cfg.mode = DacHandle::Mode::POLLING;
	cfg.chn = DacHandle::Channel::BOTH;
	seed.dac.Init(cfg);
	seed.dac.WriteValue(DacHandle::Channel::BOTH, 0);
	seed.dac.WriteValue(DacHandle::Channel::ONE, 0); // CV0
	seed.dac.WriteValue(DacHandle::Channel::TWO, 0); // CV1

	gate_in.pin  = seed.GetPin(PIN_GATE_IN);
	gate_in.mode = DSY_GPIO_MODE_INPUT;
	gate_in.pull = DSY_GPIO_PULLUP;
	dsy_gpio_init(&gate_in);

	osc.Init(sampleRate);
	osc.SetWaveform(osc.WAVE_SIN);
	osc.SetFreq(0.1); // audible freq
	osc.SetAmp(1);

	seed.StartAudio(AudioCallback);

	for (;;)
	{
		//Send the latest envelope values to the CV outs
		uint16_t y1 = map(cvCh1, -1.0, 1.0, 0, 4095);
		uint16_t y2 =	map(cvCh2, -1.0, 1.0, 0, 4095);
		seed.dac.WriteValue(DacHandle::Channel::ONE, y1);
		seed.dac.WriteValue(DacHandle::Channel::TWO, y2);
		// seed.DelayMs(1);

		seed.SetLed(x);	
	}
}
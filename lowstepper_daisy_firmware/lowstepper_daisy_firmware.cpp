#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

#ifndef TWO_PI
#define TWO_PI 6.28318530718
#endif

float sample_rate;

volatile uint16_t cv1 = 0;
volatile uint16_t cv2 = 0;

DaisySeed seed;
static Oscillator osc_sine;

class LFO {
	public:
		uint32_t sample = 0;
		double freq = 1;
		float tick(void);
};

float LFO::tick(void) {
	float y = sin(TWO_PI * sample / (sample_rate / freq));
	sample += 1;
	return y;
}

LFO lfo;
float y = 0.0f;
float z = -1.0f;

int map(float x, float in_min, float in_max, int out_min, int out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		// float y = lfo.tick();
		// // float y = osc_sine.Process();
		// // float y = -1;

		// if(y > 0) {
		// 	z = 1.0f;
		// } else {
		// 	z = -1.0f;
		// }

		// seed.dac.WriteValue(
		// 	DacHandle::Channel::ONE,
		// 	// map(y, -1.0, 1.0, 0, 4095)
		// 	2000
		// );

		// seed.dac.WriteValue(
		// 	DacHandle::Channel::TWO,
		// 	// map(z, -1.0, 1.0, 0, 4095)
		// 	2000
		// );

		out[0][i] = 1.0f;
		out[1][i] = -1.0f;
	}
}


void initCV(void) {
	// DacHandle::Config cfg;
	// cfg.bitdepth   = DacHandle::BitDepth::BITS_12;
	// cfg.buff_state = DacHandle::BufferState::ENABLED;
	// cfg.mode       = DacHandle::Mode::POLLING;
	// cfg.chn        = DacHandle::Channel::BOTH;
	// seed.dac.Init(cfg);
	// seed.dac.WriteValue(DacHandle::Channel::BOTH, 0);
}

void outputCV(void) {
		float y = lfo.tick()	;

		if(y > 0) {
			z = 1.0f;
		} else {
			z = -1.0f;
		}

	seed.dac.WriteValue(
		DacHandle::Channel::ONE,
		map(z, -1.0, 1.0, 0, 4095)
	);

	seed.dac.WriteValue(
		DacHandle::Channel::TWO,
		map(z * -1.0f, -1.0, 1.0, 0, 4095)
		// 0
	);
}

int main(void)
{
	seed.Configure();
	initCV();
	seed.Init();
	sample_rate = seed.AudioSampleRate();
	seed.StartAudio(AudioCallback);

	while(1) {
		// outputCV();
		// seed.DelayMs(1);
	}
}

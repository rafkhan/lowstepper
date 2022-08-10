#include <stdint.h>
#include <math.h>
#include "LowStepperChannel.h"
#include "util.h"


/**
 * All of the following functions take a phase input from 0 - TWO_PI
 * and return a value from -1 to 1.
 * 
 * std::sin is also leveraged but not included because sine waves are
 * incredibly based and just do that by default
 * 
 * lmao math
 */
float LowStepperChannel::triangle(float phase) {
  //REALLY DUMB CODE, NEED TO BE OPTIMIZED!

  if (phase <= PI * 0.5) //FIRST QUARTER
  {
    return 2 + ( -1 * (((PI - phase * 2) + PI) / PI) ); //GO DOWN!
  }

  if(phase >= PI * 1.5) //FOURTH QUARTER
  {
    return -1 + ( -1 * (((TWO_PI - phase * 2) + PI) / PI) ) ; //GO DOWN TOO!
  } 
  
  //SECOND AND THIRD QUARTER 
  return 1 + ( -1 * ((phase*2 - PI) / PI)); //GO UP!

}

float LowStepperChannel::ramp(float phase) {
  return -1 * ((phase - PI) / PI);
}

float LowStepperChannel::saw(float phase) {
  if (phase <= PI) {
    return phase / PI;
  }

  return ((phase - PI) / PI) - 1;
}

float LowStepperChannel::square(float phase) {
  if (phase >= PI)
  {
    return -1;
  }
  return 1;
}

// TODO: stop computing the same thing more than once!!!
// TODO: CLEAN UP THIS LOGIC.
float LowStepperChannel::getMorphedOutput(float morphPosition, float phase) {
  double newMorph;
  if(morphPosition <= 0.33) {
    newMorph = mapFFFF(morphPosition, 0, 0.33, 0, 1);
    return sin(phase) + (newMorph * (triangle(phase) - sin(phase)));
  } else if(morphPosition > 0.33 && morphPosition <= 0.66) {
    newMorph = mapFFFF(morphPosition, 0.34, 0.66, 0, 1);
    return triangle(phase) + (newMorph * (saw(phase) - triangle(phase)));
  } else {
    newMorph = mapFFFF(morphPosition, 0.67, 1, 0, 1);
    return saw(phase) + (newMorph * (square(phase) - saw(phase)));
  }
}


LowStepperChannel::LowStepperChannel(float sampleRate) {
  this->sampleRate = sampleRate;
  this->sampleLength = (1.0f/sampleRate)*1000000.0f;
}

LowStepperOutput LowStepperChannel::tick(LowStepperInput input) {
    float phaseIncrement = (this->sampleLength / (1000000.0 / input.frequency)) * TWO_PI;
    float phase = input.phase + phaseIncrement;

    float startPhase = TWO_PI * input.start;
    float endPhase = TWO_PI - (TWO_PI * input.end);

    if (phase > endPhase) {
      phase = startPhase + (phase - endPhase);
    }

    if(input.shouldReset) {
      phase = startPhase;
    }

    LowStepperOutput output;
    output.cvOutput = getMorphedOutput(input.morph, phase);
    output.eocGateHigh = false;
    output.phase = phase;
    return output;
}


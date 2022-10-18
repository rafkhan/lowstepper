#include <stdint.h>
#include <math.h>
#include "LowStepperLfo.h"
#include "util.h"


/**
 * All of the following functions take a phase input from 0 - TWO_PI
 * and return a value from -1 to 1.
 * 
 * lmao math
 */
float LowStepperLfo::triangle(float phase) {
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

float LowStepperLfo::ramp(float phase) {
  return -1 * ((phase - PI) / PI);
}

float LowStepperLfo::saw(float phase) {
  if (phase <= PI) {
    return phase / PI;
  }

  return ((phase - PI) / PI) - 1;
}

float LowStepperLfo::square(float phase) {
  if (phase >= PI)
  {
    return -1;
  }
  return 1;
}

// TODO: stop computing the same thing more than once!!!
// TODO: CLEAN UP THIS LOGIC.
float LowStepperLfo::getMorphedOutput(float morphPosition, float phase) {
  double newMorph;
  if(morphPosition <= 0.33) {
    newMorph = mapFFFF(morphPosition, 0, 0.33, 0, 1);
    return sin(phase) + (newMorph * (triangle(phase) - sin(phase)));
  } else if(morphPosition > 0.33 && morphPosition <= 0.66) {
    newMorph = mapFFFF(morphPosition, 0.34, 0.66, 0, 1);
    return triangle(phase) + (newMorph * (saw(phase) - triangle(phase)));
  } else {
    newMorph = mapFFFF(morphPosition, 0.67, 1, 0, 0.8); // NOT 0-1 to avoid square wave
    return saw(phase) + (newMorph * (square(phase) - saw(phase)));
  }
}

float LowStepperLfo::mapRateInputToFrequency(float input, bool enableSync, LowStepperLfoMode mode, float bpm) {
  if(enableSync) {
    float mult = 0.5f;
    if(mode == LowStepperLfoMode::MEDIUM) {
      mult = 16;
    }

    if(mode == LowStepperLfoMode::FAST) {
      mult = 32;
    }

    float maxFreq = (bpm / 60.0f) * mult;
    float exponent = floor(mapFFFF(input, 0, 1, 8, 0)); // will never actually go to 17?
    float position = pow(2, exponent);
    return maxFreq / (float) position;
  } else {
    // non-linear curve here.
    float min = 0.005;
    float max = 3.0f;

    if(mode == LowStepperLfoMode::MEDIUM) {
      min = 1.5;
      max = 40;
    }

    if(mode == LowStepperLfoMode::FAST) {
      min = 35;
      max = 100;
    }

    return mapFFFF(pow(input, 2.5), 0, 1, min, max);
  }
}

float LowStepperLfo::mapMorphInput(float input) {
  return input;
}

float getDividerFromPosition(float input) {
    int position = floor(mapFFFF(input, 0, 1, 0, 9));
    float div = 1;
    if(position == 0) {
      div = 0;
    }

    if(position == 1) {
      div = 0.125;
    }

    if(position == 2) {
      div = 0.25;
    }

    if(position == 3) {
      div = 0.375;
    }

    if(position == 4) {
      div = 0.5;
    }

    if(position == 5) {
      div = 0.625;
    }

    if(position == 6) {
      div = 0.75;
    }

    if(position == 7) {
      div = 0.875;
    }

    if(position == 8) {
      div = 1;
    }

    return div;
}


float LowStepperLfo::mapLengthInput(float input, bool enableSync) {
  if(enableSync) {
    return getDividerFromPosition(input);
  }

  return input;
}

float LowStepperLfo::mapStartInput(float input, bool enableSync)
{
  if(enableSync) {
    return getDividerFromPosition(input);
  }

  return input;
}

LowStepperLfo::LowStepperLfo(float sampleRate) {
  this->sampleRate = sampleRate;
  this->sampleLength = (1.0f/sampleRate)*1000000.0f;
}

LowStepperOutput LowStepperLfo::tick(LowStepperInput input) {
    float phaseIncrement = (this->sampleLength / (1000000.0 / input.frequency)) * TWO_PI;

    float start = input.start;
    float end = input.end;
    float startPhase;
    float endPhase;
    float phase;

    LowStepperOutput output;
    output.eocGateHigh = false;

    startPhase = TWO_PI * start;
    endPhase = TWO_PI - (TWO_PI * (1 - end));

    if(start < end) {
      phase = input.phase + phaseIncrement;

      if (phase > endPhase) {
        phase = startPhase + (phase - endPhase);
        output.eocGateHigh = true;
      }
    } else {
      phase = input.phase - phaseIncrement;

      if (phase < endPhase) {
        phase = startPhase + (phase - endPhase);
        output.eocGateHigh = true;
      }
    }

    if(input.shouldReset) {
      phase = startPhase;
    }

    output.cvOutput = getMorphedOutput(input.morph, phase);
    output.phase = phase;
    return output;
}


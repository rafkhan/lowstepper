import * as React from "react";
import "./styles.css";
import lowstepperWasm from "./wasm";
import Oscillator from "./Oscillator";
// @ts-ignore
import P5Wrapper from "react-p5-wrapper";

import five from "p5";

var AudioContext = window.AudioContext // Default
//@ts-ignore
    || window.webkitAudioContext // Safari and old versions of Chrome 
    || false; 

const audioCtx = new AudioContext();
const bf2 = audioCtx.createBiquadFilter();
bf2.type = "highpass";
bf2.frequency.setValueAtTime(200, audioCtx.currentTime);
bf2.Q.setValueAtTime(0.1, audioCtx.currentTime);
bf2.connect(audioCtx.destination);

const biquadFilter = audioCtx.createBiquadFilter();
biquadFilter.type = "lowpass";
biquadFilter.frequency.setValueAtTime(20000, audioCtx.currentTime);
biquadFilter.Q.setValueAtTime(0.5, audioCtx.currentTime);
biquadFilter.connect(bf2);


function round(value: number, precision: number) {
  var multiplier = Math.pow(10, precision || 0);
  return Math.round(value * multiplier) / multiplier;
}

async function sketch(p: five) {
  let module: any;
  let xspacing = 5; // Distance between each horizontal location
  let w; // Width of entire wave
  let theta = 0.0; // Start angle at 0
  let amplitude = 50.0; // Height of wave
  let period = 100.0; // How many pixels before the wave repeats
  let dx: number; // Value for incrementing x
  let yvalues: Array<number> = []; // Using an array to store height values for the wave

  let state: any = {};

setInterval(() => {
  module && module._setGate(false)
  setTimeout(() => {
    module && module._setGate(true)
  }, 100)
}, 200)


  //@ts-ignore
  p.myCustomRedrawAccordingToNewPropsHandler = function (props) {
    // console.table(props)
    if (module) {
      props.rate && module._setRate(parseInt(props.rate))
      props.morph && module._setMorph(parseFloat(props.morph))
      props.chunks && module._setChunks(parseInt(props.chunks))
      props.gate !== undefined && module._setGate(props.gate)
    }
    state = props;
  };

  function generateFunction(x: number, state: any): () => number {
    return () => {
      if (!module) return 0;
      const y =  module._tickLFO(Date.now())
      return y;
    }
  }

  // @ts-ignore
  function mapnum(x, in_min, in_max, out_min, out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

  function calcWave() {
    yvalues.shift();
    const y = generateFunction(0, state)();
    const f = mapnum(y, -1, 1, 0, 5000);
    biquadFilter.frequency.setValueAtTime(f, audioCtx.currentTime);
    yvalues.push(y * amplitude)
  }

  function renderWave() {
    p.noStroke();
    p.fill(255);
    // A simple way to draw the wave with an ellipse at each location
    for (let x = 0; x < yvalues.length; x++) {
      p.ellipse(x * xspacing, p.height / 2 + yvalues[x], 5,5);
    }
  }

  p.setup = async function () {
    
    p.createCanvas(710, 260);
    
 
    w = p.width + 16;
    dx = (p.TWO_PI / period) * xspacing;
    yvalues = new Array(p.floor(w / xspacing));
    module = await lowstepperWasm();
    module._tickLFO(0);

    const freqE = 164.81; 
    const freqG = 196;
    const freqB = 246.94;
    const unisonDetune = 0.5;
    
    const unisonNote = (freq: number) => {
      const o1 = new Oscillator(audioCtx, biquadFilter);
      o1.playFrequency(freq);

      // const o2 = new Oscillator(audioCtx, biquadFilter);
      // o2.playFrequency(freq + unisonDetune);

      // const o3 = new Oscillator(audioCtx, biquadFilter);
      // o3.playFrequency(freq - unisonDetune);
    }

    unisonNote(freqE);
    unisonNote(freqG);
    unisonNote(freqB);
  };

  p.draw = function () {
    p.background(0);
    calcWave();
    renderWave();
  };

}

export default function App() {
  const [state, setState] = React.useState<{[parameter: string]: any}>({
    chunks: 1,
    rate: 750,
    morph: 0,
    gate: true
  });

  const [currentKnobIndex, setCurrentKnobIndex] = React.useState(0);

  const knobs = [
    {
      name: "Chunks",
      description: "Break an LFO into 1-10 chunks. Each chunk is played in sequence when gate or trigger is activated.",
      parameter: "chunks",
      inputProps: {
        step: 1,
        min: 1,
        max: 10,

      }
    },
    {
      name: "Rate",
      description: "Adjust the frequency of the LFO.",
      parameter: "rate",
      inputProps: {
        step: 1,
        min: 1,
        max: 1000,

      }
    },
    {
      name: "Morph",
      description: "Morph between Sin, Saw, and Square LFOs.",
      parameter: "morph",
      inputProps: {
        step: 0.001,
        min: 0,
        max: 1,

      }
    }
  ];

  return (
    <div className="App">
      <h1>LowStepper</h1>
    <p>A new type of LFO from Okay Synthesizer.</p>

      <P5Wrapper sketch={sketch} {...state} />

      <div className="container">
      <div>
      <img src={require("./Lowstepper.svg").default} />
      </div>
      <div >

        {/* <div>
          {knobs.map((knob, index) => {
            return (
              <h2 onClick={() => setCurrentKnobIndex(index)}>{knob.name}</h2>
            );
          })}
        </div> */}
        <div>
        <h2 className="parameterName">Trig</h2>
             <p className="parameterDescription">Click here to trigger the LFO.</p>
        <button onMouseDown={(e) =>  {
          setState({ ...state, "gate": false })
          audioCtx.resume();
        }}
        onMouseUp={(e) => {
          setState({ ...state, "gate": true })
        }}
        onMouseOut={() => {
          setState({ ...state, "gate": true })
        }}
      >Trig</button>
        </div>
          {knobs.map((currentKnob) => (
             <div>
             <h2 className="parameterName"> {currentKnob.name}</h2>
             <p className="parameterDescription"> {currentKnob.description}</p>
             {/* <label>state : ({JSON.stringify(state)})</label> */}
             <input
               type="range"
               onChange={(e) =>
                 setState({ ...state, [currentKnob.parameter]: e.target.value })
               }
     
               defaultValue={state[currentKnob.parameter]} 
             {...currentKnob.inputProps}
             />
           </div>
          ))}


      </div>
      </div>
    </div>
  );
}

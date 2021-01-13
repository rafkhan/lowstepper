import * as React from "react";
import "./styles.css";
import lowstepperWasm from "./wasm";
import Oscillator from "./Oscillator";
// @ts-ignore
import P5Wrapper from "react-p5-wrapper";

import five from "p5";


const audioCtx = new AudioContext();
const biquadFilter = audioCtx.createBiquadFilter();
biquadFilter.type = "lowpass";
biquadFilter.frequency.setValueAtTime(20000, audioCtx.currentTime);
biquadFilter.Q.setValueAtTime(10, audioCtx.currentTime);
biquadFilter.connect(audioCtx.destination);

function round(value: number, precision: number) {
  var multiplier = Math.pow(10, precision || 0);
  return Math.round(value * multiplier) / multiplier;
}

async function sketch(p: five) {
  let module: any;
  let xspacing = 5; // Distance between each horizontal location
  let w; // Width of entire wave
  let theta = 0.0; // Start angle at 0
  let amplitude = 100.0; // Height of wave
  let period = 100.0; // How many pixels before the wave repeats
  let dx: number; // Value for incrementing x
  let yvalues: Array<number> = []; // Using an array to store height values for the wave

  let state: any = {};

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
    const f = mapnum(y, -1, 1, 0, 4000);
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
    
    p.createCanvas(710, 400);
    
 
    w = p.width + 16;
    dx = (p.TWO_PI / period) * xspacing;
    yvalues = new Array(p.floor(w / xspacing));
    module = await lowstepperWasm();
    module._tickLFO(0);

    const freq = 100;
    const unisonDetune = 1;
    const o1 = new Oscillator(audioCtx, biquadFilter);
    o1.playFrequency(freq);

    const o2 = new Oscillator(audioCtx, biquadFilter);
    o2.playFrequency(freq + unisonDetune);

    const o3 = new Oscillator(audioCtx, biquadFilter);
    o3.playFrequency(freq - unisonDetune);
  };

  p.draw = function () {
    p.background(0);
    calcWave();
    renderWave();
  };

}

export default function App() {
  const [state, setState] = React.useState({

  });

  const [currentKnobIndex, setCurrentKnobIndex] = React.useState(0);

  const knobs = [
    {
      name: "Chunks",
      description: "Number of chunks",
      parameter: "chunks",
      inputProps: {
        step: 1,
        min: 1,
        max: 10
      }
    },
    {
      name: "Rate",
      description: "Shift the wave's period.",
      parameter: "rate",
      inputProps: {
        step: 1,
        min: 1,
        max: 1000
      }
    },
    {
      name: "Morph",
      description: "Lorem",
      parameter: "morph",
      inputProps: {
        step: 0.01,
        min: 0,
        max: 1
      }
    }
  ];

  return (
    <div className="App">
      <button onMouseDown={(e) =>  {
          setState({ ...state, "gate": false });
          audioCtx.resume();
        }}
        onMouseUp={(e) => {
          setState({ ...state, "gate": true })
        }}
        onMouseOut={() => {
          setState({ ...state, "gate": true })
        }}
      >FUCK</button>
      <P5Wrapper sketch={sketch} {...state} />
      <div className="container">
        {/* <div>
          {knobs.map((knob, index) => {
            return (
              <h2 onClick={() => setCurrentKnobIndex(index)}>{knob.name}</h2>
            );
          })}
        </div> */}
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
               
             {...currentKnob.inputProps}
             />
           </div>
          ))}


      </div>
    </div>
  );
}

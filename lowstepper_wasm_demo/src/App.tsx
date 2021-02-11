import * as React from "react";
import "./styles.css";
import lowstepperWasm from "./wasm";
import Oscillator from "./Oscillator";
// @ts-ignore
import P5Wrapper from "react-p5-wrapper";

import five from "p5";

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
  let yvalues_TRIG: Array<boolean> = []; // Using an array to store height values for the wave


  let state: any = {};

  let TRIGGED = false;

  const setGateA = (high: boolean) => {
    module._setGateA(high);
    if(high) {
      TRIGGED = true;
    }
  };

  setInterval(() => {
    setGateA(true);
    setTimeout(() => setGateA(false), 100);
  }, 500);

  //@ts-ignore
  p.myCustomRedrawAccordingToNewPropsHandler = function (props) {
    // console.table(props)
    if (module) {
      props.rate && module._setRateA(parseInt(props.rate))
      props.morph && module._setMorphA(parseFloat(props.morph))
      props.chunks && module._setChunksA(parseInt(props.chunks));

      if(props.gate) {
        setGateA(true);
      }
    }
    state = props;
  };

  function generateFunction(x: number, state: any): () => { y: number, y2: boolean } {
    return () => {
      if (!module) return { y: 0, y2: false };
    
      const d = Date.now();
      const y = module._tickLFO(d);
      module._setGateA(false);

      let wasTriggered = false;
      if(TRIGGED) {
        wasTriggered = true;
        TRIGGED = false;
      }

      return { y, y2: wasTriggered };
    }
  }

  // @ts-ignore
  function mapnum(x, in_min, in_max, out_min, out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

  function calcWave() {
    const { y, y2 } = generateFunction(0, state)();

    yvalues.shift();
    yvalues_TRIG.shift();

    yvalues.push(y * amplitude)
    yvalues_TRIG.push(y2);
  }

  function renderWave() {
    // p.noStroke();
    p.fill(255);
    p.stroke(255);
    // A simple way to draw the wave with an ellipse at each location
    for (let x = 0; x < yvalues.length; x++) {
      if(yvalues_TRIG[x]) {
        p.line(x * xspacing, 0, x * xspacing, 1000);
      }
      p.ellipse(x * xspacing, p.height / 2 + yvalues[x], 5,5);
    }
  }

  p.setup = async function () {
    p.createCanvas(440, 260);
    w = p.width + 16;
    dx = (p.TWO_PI / period) * xspacing;
    yvalues = new Array(p.floor(w / xspacing));
    yvalues_TRIG = new Array(p.floor(w / xspacing));
    module = await lowstepperWasm();
    module._tickLFO(0);
  };

  p.draw = function () {
    p.background(0);
    calcWave();
    renderWave();
  };
}

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
      min: 25,
      max: 2000,

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

export default function App() {
  const [state, setState] = React.useState<{[parameter: string]: any}>({
    chunks: 1,
    rate: 500,
    morph: 0,
    gate: false 
  });

  return (
    <div className="App">
      <P5Wrapper sketch={sketch} {...state} />

      <div className="container">
        <div>
          <h2 className="parameterName">Trig</h2>
          <p className="parameterDescription">Click here to trigger the LFO.</p>
          <button onMouseDown={(e) =>  {
            setState({ ...state, "gate": true })
          }}
          onMouseUp={(e) => {
            setState({ ...state, "gate": false })
          }}
          onMouseOut={() => {
            setState({ ...state, "gate": false })
          }}>
            Trig
          </button>
        </div>
        {knobs.map((currentKnob) => (
            <div>
            <h2 className="parameterName"> {currentKnob.name}</h2>
            <p className="parameterDescription"> {currentKnob.description}</p>
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
  );
}

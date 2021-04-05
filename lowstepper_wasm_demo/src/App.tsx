import * as React from "react";
import "./styles.css";
import lowstepperWasm from "./wasm";
// @ts-ignore
import P5Wrapper from "react-p5-wrapper";

import five from "p5";

class TrigHandler {
  wasmModule: any;
  triggedA: boolean;
  autoTrigHandler: NodeJS.Timeout | null;

  constructor() {
    this.triggedA = false;
    this.autoTrigHandler = null;
  }

  setWasmModule(m: any): void {
    this.wasmModule = m;
  }

  gateA(b: boolean): void {
    if(!this.wasmModule) {
      return;
    }

    this.wasmModule._setGateA(b);
    if(b) {
      this.triggedA = true;
    }
  };

  startAutoTriggerA(interval: number) {
    this.autoTrigHandler = setInterval(() => {
      this.gateA(true);
      setTimeout(() => this.gateA(false), 100);
    }, interval);
  }

  stopAutoTriggerA() {
    if(this.autoTrigHandler) {
      clearInterval(this.autoTrigHandler);
    }
  }
}

const trigHandler = new TrigHandler();


async function sketch(p: five) {
  let module: any;
  let w; // Width of entire wave
  let xspacing = 5; // Distance between each horizontal location
  let amplitude = 100.0; // Height of wave
  let period = 100.0; // How many pixels before the wave repeats
  let dx: number; // Value for incrementing x
  let yvalues: Array<number> = []; // Using an array to store height values for the wave
  let yvalues_TRIG: Array<boolean> = []; // Using an array to store instances of triggers


  let state: any = {};

  //@ts-ignore
  p.myCustomRedrawAccordingToNewPropsHandler = function (props) {
    if (module) {
      props.rate && module._setRateA(parseInt(props.rate))
      props.morph && module._setMorphA(parseFloat(props.morph))
      props.chunks && module._setChunksA(parseInt(props.chunks));

      if(props.gate) {
        trigHandler.gateA(true);
      }
    }
    state = props;
  };

  function generateFunction(x: number, state: any): () => { y: number, y2: boolean } {
    return () => {
      if (!module) return { y: 0, y2: false };
    
      const d = Date.now();
      const y = -1 * module._tickLFOA(d);
      trigHandler.gateA(false);

      let wasTriggered = false;
      if(trigHandler.triggedA) {
        wasTriggered = true;
        trigHandler.triggedA = false;
      }

      return { y, y2: wasTriggered };
    }
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
    module._tickLFOA(Date.now());

    trigHandler.setWasmModule(module);
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
    description: "Morph between sine -> triangle -> saw -> square.",
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
    rate: 1500,
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

          <button onClick={() => {
            if(trigHandler.autoTrigHandler) {
              trigHandler.stopAutoTriggerA();
              return;
            }

            trigHandler.startAutoTriggerA(1000);
          }}>TOGGLE AUTO TRIG (fixed 1000ms)</button>

        </div>
        {knobs.map((currentKnob) => (
          <div key={currentKnob.name}>
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

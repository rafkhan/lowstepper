import * as React from "react";
import "./styles.css";
import lowstepperWasm from "./wasm";
// @ts-ignore
import P5Wrapper from "react-p5-wrapper";

import five from "p5";

import { Knob } from "react-rotary-knob";

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

class LimitedKnob extends React.Component<{style:any, min:any, max:any, unlockDistance:any, preciseMode:any, width:any, height:any }, { value: any, min: any, max:any }>{
    constructor(props: any) {
      super(props);
      this.state = {
        value: props.value, 
        min: props.min, 
        max: props.max 
      };
      this.handleOnChange = this.handleOnChange.bind(this);
    }

    handleOnChange(val: any) {
      //ignore change if distance is greater than defined
      //here we use a distance of 200 because our max value is 1000
      //change if needed
      const maxDistance = 200;
      let distance = Math.abs(val - this.state.value);
      if (distance > maxDistance) {
        return;
      } else {
        this.setState({ value: val });
      }
    }
    render() {
     //let { this.state.value, ...rest } = this.props;

     /* return (
        <Knob value={this.state.value} onChange={this.handleOnChange} {...rest} />
      );*/
      return <Knob onChange={this.handleOnChange} min={this.state.min} max={this.state.max} value={this.state.value}/>
    
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
    name: "RATE",
    description: "Adjust the frequency of the LFO.",
    parameter: "rate",
    inputProps: {
      step: 1,
      min: 25,
      max: 2000,

    }
  },
  {
<<<<<<< HEAD
    name: "MORPH",
    description: "Morph between Sin, Saw, and Square LFOs.",
=======
    name: "Morph",
    description: "Morph between sine -> triangle -> saw -> square.",
>>>>>>> 8a2b563642c58888d0b4d69359e5ea74cbca6be3
    parameter: "morph",
    inputProps: {
      step: 0.001,
      min: 0,
      max: 1,
    }
  },
  {
    name: "CHUNKS",
    description: "Break an LFO into 1-10 chunks. Each chunk is played in sequence when gate or trigger is activated.",
    parameter: "chunks",
    inputProps: {
      step: 1,
      min: 1,
      max: 10,

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
    <img className="guide" src="static/images/interface_guide.png" />
      <img className="white_hole top left" src="static/images/ico_white_hole.png" />
      <img className="white_hole top right" src="static/images/ico_white_hole.png" />
      <h1>LowStepper</h1>

      <div className="container sideWrapper">
        <div className="buttonWrapper white first_row">
            <button className="button" onMouseDown={(e) =>  {
              setState({ ...state, "gate": true })
            }}
            onMouseUp={(e) => {
              setState({ ...state, "gate": false })
            }}
            onMouseOut={() => {
              setState({ ...state, "gate": false })
            }}>
            </button>
            <h3 className="parameterName">EOC</h3>
        </div>       

        <div className="buttonWrapper white">
            <button className="button" onMouseDown={(e) =>  {
              setState({ ...state, "gate": true })
            }}
            onMouseUp={(e) => {
              setState({ ...state, "gate": false })
            }}
            onMouseOut={() => {
              setState({ ...state, "gate": false })
            }}>
            </button>
        </div>

        <div className="buttonWrapper white third_row">
            <button className="button" onMouseDown={(e) =>  {
              setState({ ...state, "gate": true })
            }}
            onMouseUp={(e) => {
              setState({ ...state, "gate": false })
            }}
            onMouseOut={() => {
              setState({ ...state, "gate": false })
            }}>
            </button>
            <h3 className="parameterName outsTitle">OUTS</h3>
        </div>

        <div className="buttonWrapper white">
            <button className="button" onMouseDown={(e) =>  {
              setState({ ...state, "gate": true })
            }}
            onMouseUp={(e) => {
              setState({ ...state, "gate": false })
            }}
            onMouseOut={() => {
              setState({ ...state, "gate": false })
            }}>
            </button>
        </div>

      </div>
      <div className="container trigBigWrapper">
        <div className="trigWrapper">
          
          <div className="buttonWrapper red">
            <button className="button" onMouseDown={(e) =>  {
              setState({ ...state, "gate": true })
            }}
            onMouseUp={(e) => {
              setState({ ...state, "gate": false })
            }}
            onMouseOut={() => {
              setState({ ...state, "gate": false })
            }}>
            </button>
          </div>

          <h3 className="parameterName">TRIG <img src="static/images/ico_trig.png" /></h3>

          <button className="autoTrigBtn" onClick={() => {
            if(trigHandler.autoTrigHandler) {
              trigHandler.stopAutoTriggerA();
              return;
            }

            trigHandler.startAutoTriggerA(1000);
          }}>AUTO TRIG (1000ms)</button>

        </div>
        <div className="trigWrapper">
          <div>
              <div className="buttonWrapper red">
                <button className="button" onMouseDown={(e) =>  {
                  setState({ ...state, "gate": true })
                }}
                onMouseUp={(e) => {
                  setState({ ...state, "gate": false })
                }}
                onMouseOut={() => {
                  setState({ ...state, "gate": false })
                }}>
                </button>
              </div>
              <h3 className="parameterName">CLK <img src="static/images/ico_clock.png" /></h3>
            </div>
        </div>
        <div className="trigWrapper">
          <div>
              <div className="buttonWrapper red">
                <button className="button" onMouseDown={(e) =>  {
                  setState({ ...state, "gate": true })
                }}
                onMouseUp={(e) => {
                  setState({ ...state, "gate": false })
                }}
                onMouseOut={() => {
                  setState({ ...state, "gate": false })
                }}>
                </button>
              </div>
               <h3 className="parameterName">RST <img src="static/images/ico_reset.png" /></h3>
            </div>
        </div>
        
        <div className="trigWrapper">
          <div>
              <div className="buttonWrapper blue">
                <button className="button" onMouseDown={(e) =>  {
                  setState({ ...state, "gate": true })
                }}
                onMouseUp={(e) => {
                  setState({ ...state, "gate": false })
                }}
                onMouseOut={() => {
                  setState({ ...state, "gate": false })
                }}>
                </button>
              </div>
            </div>
        </div>

        <div className="trigWrapper">
          <div>
              <div className="buttonWrapper blue">
                <button className="button" onMouseDown={(e) =>  {
                  setState({ ...state, "gate": true })
                }}
                onMouseUp={(e) => {
                  setState({ ...state, "gate": false })
                }}
                onMouseOut={() => {
                  setState({ ...state, "gate": false })
                }}>
                </button>
              </div>
            </div>
        </div>

        <div className="trigWrapper">
          <div>
              <div className="buttonWrapper blue">
                <button className="button" onMouseDown={(e) =>  {
                  setState({ ...state, "gate": true })
                }}
                onMouseUp={(e) => {
                  setState({ ...state, "gate": false })
                }}
                onMouseOut={() => {
                  setState({ ...state, "gate": false })
                }}>
                </button>
              </div>
            </div>
        </div>
        
        <div className="whiteLine"></div>

        <div className="trigWrapper">
          <div>
              <div className="buttonWrapper red">
                <button className="button" onMouseDown={(e) =>  {
                  setState({ ...state, "gate": true })
                }}
                onMouseUp={(e) => {
                  setState({ ...state, "gate": false })
                }}
                onMouseOut={() => {
                  setState({ ...state, "gate": false })
                }}>
                </button>
              </div>
            </div>
            <h3 className="parameterName">RATE</h3>
        </div>

        <div className="trigWrapper">
          <div>
              <div className="buttonWrapper red">
                <button className="button" onMouseDown={(e) =>  {
                  setState({ ...state, "gate": true })
                }}
                onMouseUp={(e) => {
                  setState({ ...state, "gate": false })
                }}
                onMouseOut={() => {
                  setState({ ...state, "gate": false })
                }}>
                </button>
              </div>
            </div>
            <h3 className="parameterName">MORPH</h3>
        </div>

        <div className="trigWrapper">
          <div>
              <div className="buttonWrapper red">
                <button className="button" onMouseDown={(e) =>  {
                  setState({ ...state, "gate": true })
                }}
                onMouseUp={(e) => {
                  setState({ ...state, "gate": false })
                }}
                onMouseOut={() => {
                  setState({ ...state, "gate": false })
                }}>
                </button>
              </div>
            </div>
            <h3 className="parameterName">CHUNKS</h3>
        </div>

        <div className="trigWrapper">
          <div>
              <div className="buttonWrapper blue">
                <button className="button" onMouseDown={(e) =>  {
                  setState({ ...state, "gate": true })
                }}
                onMouseUp={(e) => {
                  setState({ ...state, "gate": false })
                }}
                onMouseOut={() => {
                  setState({ ...state, "gate": false })
                }}>
                </button>
              </div>
            </div>
        </div>

        <div className="trigWrapper">
          <div>
              <div className="buttonWrapper blue">
                <button className="button" onMouseDown={(e) =>  {
                  setState({ ...state, "gate": true })
                }}
                onMouseUp={(e) => {
                  setState({ ...state, "gate": false })
                }}
                onMouseOut={() => {
                  setState({ ...state, "gate": false })
                }}>
                </button>
              </div>
            </div>
        </div>

        <div className="trigWrapper">
          <div>
              <div className="buttonWrapper blue">
                <button className="button" onMouseDown={(e) =>  {
                  setState({ ...state, "gate": true })
                }}
                onMouseUp={(e) => {
                  setState({ ...state, "gate": false })
                }}
                onMouseOut={() => {
                  setState({ ...state, "gate": false })
                }}>
                </button>
              </div>
            </div>
        </div>

      </div>
      
      <P5Wrapper sketch={sketch} {...state} />
      <div className="container potentiometerWrapper">
        {knobs.map((currentKnob) => (
          <div className="wrapper_potentiometer" key={currentKnob.name}>
            <div className="bg_potentiometer"></div>
            <h2 className="parameterName"> {currentKnob.name}</h2>
            <div className="bgKnob knobB">
              <LimitedKnob
                style={{ display: "inline-block" }}
                min={currentKnob.inputProps.min}
                max={currentKnob.inputProps.max}
                unlockDistance={0}
                preciseMode={false}
                width={300}
                height={300} 
              />
            </div>
            <div className="bgKnob knobA">
              <LimitedKnob
                style={{ display: "inline-block" }}
                min={currentKnob.inputProps.min}
                max={currentKnob.inputProps.max}
                unlockDistance={0}
                preciseMode={false}
                width={300}
                height={300} 
              />
            </div>
            
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
      <img className="white_hole bottom left" src="static/images/ico_white_hole.png" />
      <img className="white_hole bottom right" src="static/images/ico_white_hole.png" />
      <img className="okay_logo" src="static/images/okay_logo.png" />
    </div>
  );
}

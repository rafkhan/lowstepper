import * as React from "react";
import "./styles.css";

// @ts-ignore
import P5Wrapper from "react-p5-wrapper";

import five from "p5";

function sketch(p: five) {
  let xspacing = 1; // Distance between each horizontal location
  let w; // Width of entire wave
  let theta = 0.0; // Start angle at 0
  let amplitude = 75.0; // Height of wave
  let period = 500.0; // How many pixels before the wave repeats
  let dx: number; // Value for incrementing x
  let yvalues: Array<number>; // Using an array to store height values for the wave

  let state: any = {};

  //@ts-ignore
  p.myCustomRedrawAccordingToNewPropsHandler = function (props) {
    state = props;
  };

  function generateFunction(x: number, state: any): () => number {
    return () => p.sin(x) * state.amplitude + state.offset;
  }

  function calcWave() {
    // Increment theta (try different values for
    // 'angular velocity' here)
    theta += 0.02;

    // For every x value, calculate a y value with sine function
    let x = theta;
    for (let i = 0; i < yvalues.length; i++) {
      yvalues[i] = generateFunction(x, state)() * amplitude;
      x += dx;
    }
  }

  function renderWave() {
    p.noStroke();
    p.fill(255);
    // A simple way to draw the wave with an ellipse at each location
    for (let x = 0; x < yvalues.length; x++) {
      p.ellipse(x * xspacing, p.height / 2 + yvalues[x], 3, 3);
    }
  }

  p.setup = function () {
    p.createCanvas(710, 400);
    w = p.width + 16;
    dx = (p.TWO_PI / period) * xspacing;
    yvalues = new Array(p.floor(w / xspacing));
  };

  p.draw = function () {
    p.background(0);
    calcWave();
    renderWave();
  };
}

export default function App() {
  const [state, setState] = React.useState({
    amplitude: 1,
    offset: 0
  });

  const [currentKnobIndex, setCurrentKnobIndex] = React.useState(0);

  const knobs = [
    {
      name: "Chunks",
      description: "Number of chunks",
      parameter: "amplitude"
    },
    {
      name: "Rate",
      description: "Shift the wave's period.",
      parameter: "offset"
    },
    {
      name: "Morph",
      description: "Lorem",
      parameter: "amplitude"
    },
    {
      name: "Trigger",
      description: "Lorem",
      parameter: "amplitude"
    }
  ];
  const currentKnob = knobs[currentKnobIndex];

  return (
    <div className="App">
      <P5Wrapper sketch={sketch} {...state} />
      <div className="container">
        <div>
          {knobs.map((knob, index) => {
            return (
              <h2 onClick={() => setCurrentKnobIndex(index)}>{knob.name}</h2>
            );
          })}
        </div>
        <div>
          <h2 className="parameterName"> {currentKnob.name}</h2>
          <p className="parameterDescription"> {currentKnob.description}</p>
          {/* <label>state : ({JSON.stringify(state)})</label> */}
          <input
            type="range"
            onChange={(e) =>
              setState({ ...state, [currentKnob.parameter]: e.target.value })
            }
            step="0.01"
            min={0}
            max={1}
          />
        </div>
      </div>
    </div>
  );
}

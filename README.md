# Lowstepper

Lowstepper Eurorack Module. 


## Components

- `lowstepper_firmware/` - C++ PlatformIO project for Teensy3.5 board
- `lowstepper_hardware/` - KiCad project containing schematics and and PCB designs
- `lowstepper_wasm_demo/` - ReactJS project containing web demo of `/lowstepper_firmware`
- `panel/` - The front panel of the module, Adobe Illustrator designs and KiCad Project

### Firmware
All code is located in `lowstepper_firmware/`. Currently prototyping on a [teensy 3.5](https://www.pjrc.com/store/teensy35.html) using [PlatformIO](https://platformio.org/) in vscode.
Use PIO build tools inside of `lowstepper_firmware/` to compile, flash to teensy, and monitor application over USB.

### Hardware
Download KiCad and open `.pro` file inside `lowstepper_hardware/`

### Web Demo

The web demo requires that `lowstepper_wasm_demo/public/static/js/wasm.wasm` exists. It is generated by `lowstepper_firmware/build_wasm.sh`. We leave it in the repo to avoid installing emscripten in CI (for now) and to simplify the vercel deploy.

#### Install dependencies
```
npm install 
```

#### Start dev server
```
npm start
```

#### Build for prod
```
npm run build
```

#### Update wasm binary from firmware
- Install emscripten
- `cd` to `lowstepper_firmware/`
- `bash build_wasm.sh`
- This will copy to `lowstepper_wasm_demo` and you will be able to refresh and see changes you made from C++


# LICENSE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.



------

Look, if you're considering selling this... go for it, but we would rather you call us up and collaborate. If you are going to sell it, please make your files available for the community to build on. That's all we ask :)

------




Maintained, with love, by [Okay Synthesizer](https://okaysynthesizer.com/) <3

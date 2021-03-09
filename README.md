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


Maintained by Rafael Khan.

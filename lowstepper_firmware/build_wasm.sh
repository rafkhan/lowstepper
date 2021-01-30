#!/bin/bash

emcc wasm.cc -o wasm.js -s MODULARIZE -s EXPORTED_RUNTIME_METHODS=['ccall'] -s WASM=1
mv wasm.js ../lowstepper_wasm_demo/src
mv wasm.wasm ../lowstepper_wasm_demo/public/static/js
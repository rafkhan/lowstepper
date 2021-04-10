#!/bin/bash

emcc wasm.cc -o wasm.js -s MODULARIZE -s EXPORTED_RUNTIME_METHODS=['ccall'] -s WASM=1


emcc wasm.cc -g -o wasm.js -s WASM=1 -std=c++1z \
    -s MODULARIZE -s EXPORTED_RUNTIME_METHODS=['ccall'] \
    -s LLD_REPORT_UNDEFINED \
    -g4 -s ASSERTIONS=2 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=1 \
    --source-map-base /static/js/


mv wasm.js ../lowstepper_wasm_demo/src
mv wasm.wasm ../lowstepper_wasm_demo/public/static/js
mv wasm.wasm.map ../lowstepper_wasm_demo/public/static/js

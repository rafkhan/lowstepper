#!/bin/bash

emcc wasm.cc -o wasm.js -s MODULARIZE -s EXPORTED_RUNTIME_METHODS=['ccall'] -s WASM=1


emcc wasm.cc -o wasm.js -s WASM=1 -std=c++1z \
    -s MODULARIZE -s EXPORTED_RUNTIME_METHODS=['ccall'] \
    -g4 -s ASSERTIONS=2 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=1 \
    --source-map-base http://your_server_ip:port/


mv wasm.js ../lowstepper_wasm_demo/src
mv wasm.wasm ../lowstepper_wasm_demo/public/static/js
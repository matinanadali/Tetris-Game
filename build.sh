#!/bin/bash
set -e  # Exit immediately if a command exits with a non-zero status

# Install Emscripten
echo "Cloning Emscripten SDK..."
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

echo "Installing and activating Emscripten SDK..."
./emsdk install latest
./emsdk activate latest
source emsdk_env.sh

# Go back to the project root directory
cd ..

# Run your build command
echo "Building project..."
emcc ./programs/game_wasm.o ./modules/state_wasm.o ./programs/interface_wasm.o ./modules/vec2_wasm.o lib/k08_wasm.a lib/libraylib_wasm.a -o game.html -Wall -Werror -g -I./include -I../include -lm -ldl -lpthread -lGL -s WASM=1 -s USE_GLFW=3 -s USE_WEBGL2=1 -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY

echo "Build completed successfully."

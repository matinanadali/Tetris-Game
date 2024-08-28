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

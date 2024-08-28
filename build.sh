#!/bin/bash
set -e  # Exit immediately if a command exits with a non-zero status

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check if Emscripten is already installed
if command_exists emcc; then
    echo "Emscripten is already installed."
else
    echo "Emscripten not found. Installing..."

    # Install Emscripten
    echo "Cloning Emscripten SDK..."
    git clone https://github.com/emscripten-core/emsdk.git
    cd emsdk

    echo "Installing and activating Emscripten SDK..."
    ./emsdk install latest
    ./emsdk activate latest
    source emsdk_env.sh

    # Go back to the root project directory
    cd ..
fi

# Run the build command
echo "Running the build command..."
make run-wasm

# Check if the build output exists
if [ ! -f game.html ]; then
    echo "Build failed: game.html not found."
    exit 1
fi

echo "Build completed successfully."

#!/bin/bash

echo "⚙️ Configuring CMake..."
cd build
cmake ..

echo "🔨 Building project..."
cmake --build .

cd ..

echo "🚀 Running Boids..."
./build/EC327-Boids
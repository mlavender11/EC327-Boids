#!/bin/bash

echo "🧹 Clearing old CMake cache..."
rm -rf build
mkdir build
cd build

echo "⚙️ Configuring CMake..."
cmake ..

echo "🔨 Building project..."
cmake --build .

cd ..

echo "🚀 Running Boids..."
./build/EC327-Boids.app/Contents/MacOS/EC327-Boids
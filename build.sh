#!/bin/bash

cmake -B build -G Ninja
cp build/compile_commands.json ./
cd build
ninja
echo "Running main..."
./bin/main $1

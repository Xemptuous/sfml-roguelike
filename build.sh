#!/bin/bash

cmake -B build -G Ninja
cp build/compile_commands.json ./
cd build
ninja
echo "Running main..."
./main $1

#!/bin/bash

cmake -B build -G Ninja
cd build
cp compile_commands.json ../
ninja
echo "Running main..."
./bin/main $1

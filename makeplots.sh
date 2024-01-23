#!/bin/bash

# Compile and run GPU version
g++ src/*.cpp -lGL -lGLEW -lGLU -ldl -lglut -O3 -o release/gpu.out
cd release
./gpu.out gpu >> ../out.txt
cd ..

# CPU with -O3
g++ src/*.cpp -lGL -lGLEW -lGLU -ldl -lglut -DCPU_BASED -O3 -o release/O3.out
cd release
./O3.out cpu03 >> ../out.txt
cd ..

# CPU with -O2
g++ src/*.cpp -lGL -lGLEW -lGLU -ldl -lglut -DCPU_BASED -O2 -o release/O2.out
cd release
./O2.out cpu02 >> ../out.txt
cd ..

# CPU version
g++ src/*.cpp -lGL -lGLEW -lGLU -ldl -lglut -DCPU_BASED -o debug/debug.out
cd debug
./debug.out cpu >> ../out.txt
cd ..

# Make the plots
#conda activate fog
#python plots/plots.py -n $(awk '{print $1}' out.txt) -c $(awk '{print $2}' out.txt)
#!/bin/bash

# Compile and run GPU version
g++ *.cpp -lGL -lGLEW -lGLU -ldl -lglut
./a.out gpu >> out.txt

# CPU version
g++ *.cpp -lGL -lGLEW -lGLU -ldl -lglut -DCPU_BASED
./a.out g++ >> out.txt

# CPU with -O3
g++ *.cpp -lGL -lGLEW -lGLU -ldl -lglut -DCPU_BASED -O3
./a.out g++O3 >> out.txt

# CPU with -O2
g++ *.cpp -lGL -lGLEW -lGLU -ldl -lglut -DCPU_BASED -O2
./a.out g++O2 >> out.txt

# Make the plots
#conda activate fog
#python plots/plots.py -n $(awk '{print $1}' out.txt) -c $(awk '{print $2}' out.txt)
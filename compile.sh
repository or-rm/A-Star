#!/bin/bash

#Compiling Libraries
g++ -c -o ./obj/monitoring.o ./lib/monitoring.cpp  -std=c++11 -I./include
g++ -c -o ./obj/heuristic.o ./lib/heuristic.cpp  -std=c++11 -lm -I./include
g++ -c -o ./obj/nodebinsearch.o ./lib/nodebinsearch.cpp  -std=c++11 -I./include
g++ -c -o ./obj/nodequicksort.o ./lib/nodequicksort.cpp  -std=c++11 -I./include
g++ -c -o ./obj/astarqueues.o ./lib/astarqueues.cpp  -std=c++11 -fopenmp -I./include
g++ -c -o ./obj/closestpoint.o ./lib/closestpoint.cpp  -std=c++11 -fopenmp -I./include
#Compiling Sources
g++ -c -o ./obj/readnodes.o ./src/readnodes.cpp  -std=c++11 -lm -I./include
g++ -c -o ./obj/readways.o ./src/readways.cpp  -std=c++11 -lm -I./include
g++ -c -o ./obj/astar.o ./src/astar.cpp  -std=c++11 -lm -I./include
#Compiling Code
g++ -o main main.cpp -std=c++11 -lm -fopenmp -I./include ./obj/heuristic.o ./obj/nodequicksort.o ./obj/nodebinsearch.o ./obj/monitoring.o ./obj/astarqueues.o ./obj/readnodes.o ./obj/readways.o ./obj/astar.o ./obj/closestpoint.o 


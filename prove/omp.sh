#! /bin/bash

g++-15 -O3 -march=native -ffast-math -fopenmp omp.cpp -o omp
./omp > out.csv
rm omp

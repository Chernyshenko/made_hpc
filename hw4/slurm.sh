#!/bin/bash

./matrix_debug 1024 5
./matrix 1024 5
export OMP_NUM_THREADS=4
./matrix_omp 1024 5

#!/bin/bash
echo -n > timing_log.txt

for np in 1 2 4 8 16; do
  for h in 0.01 0.005 0.001 0.0005 0.0001; do
    echo "Запуск: np=$np, h=$h"
    export H_STEP=$h
    mpirun --oversubscribe -np $np ./parallel_integral.o
  done
done



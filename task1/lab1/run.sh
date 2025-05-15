#!/bin/bash
EXE=./transport_equation_paralel.o
echo -n > timing_log.txt
for np in 1 2 3 4
do
  for tau in 0.1 0.01 0.001 0.0001
  do
    echo "=== Запуск: np=$np, tau=$tau ==="
    export TAU=$tau
    mpirun -np $np $EXE
  done
done
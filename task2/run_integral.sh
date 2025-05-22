#!/bin/bash

gcc -o parallel_integral parallel_integral.c -lm -lpthread
> timing_log.txt

for np in 1 2 4 8 16; do
  for eps in 1e-2 1e-3 1e-4 1e-5 1e-6; do
    ./parallel_integral $np $eps >> timing_log.txt
  done
done


#!/bin/sh
#SBATCH -n 8
#SBATCH -o out.out   # STDOUT
#SBATCH -e err.err   # STDERR 

mpirun -np 8 ./hello_world_mpi.o

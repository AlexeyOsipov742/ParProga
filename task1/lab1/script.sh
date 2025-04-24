#!/bin/sh
#SBATCH -n 8
#SBATCH -o hello_world_mpi.out   # STDOUT
#SBATCH -e hello_world_mpi.err   # STDERR 

mpirun -np 8 ./hello_world_mpi.o

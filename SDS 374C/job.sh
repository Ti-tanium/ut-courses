#!/bin/bash
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -o lxx-sds
#SBATCH -t 00:30:00
#SBATCH -p small

export OMP_NUM_THREADS=1; numactl --cpunodebind=0 --preferred=0 ./hw1/part1/a.out >> pa1.log
export OMP_NUM_THREADS=1; numactl --cpunodebind=0 --preferred=0 ./hw1/part1/a.out >> pa1.log
export OMP_NUM_THREADS=1; numactl --cpunodebind=0 --preferred=0 ./hw1/part1/a.out >> pa1.log

 
export OMP_NUM_THREADS=8; numactl --cpunodebind=0 --preferred=0 ./hw1/part1/a.out >> pa8.log
export OMP_NUM_THREADS=8; numactl --cpunodebind=0 --preferred=0 ./hw1/part1/a.out >> pa8.log
export OMP_NUM_THREADS=8; numactl --cpunodebind=0 --preferred=0 ./hw1/part1/a.out >> pa8.log




#!/bin/bash
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -o lxx-sds
#SBATCH -t 01:00:00
#SBATCH -p small

export OMP_NUM_THREADS=2; numactl --cpunodebind=0 --preferred=0 ./hw1/part1/a1.out >> assign2a.log
export OMP_NUM_THREADS=4; numactl --cpunodebind=0 --preferred=0 ./hw1/part1/a1.out >> assign2a.log
export OMP_NUM_THREADS=16; numactl --cpunodebind=0 --preferred=0 ./hw1/part1/a1.out >> assign2a.log
export OMP_NUM_THREADS=32; numactl --cpunodebind=0 --preferred=0 ./hw1/part1/a1.out >> assign2a.log
export OMP_NUM_THREADS=64; numactl --cpunodebind=0 --preferred=0 ./hw1/part1/a1.out >> assign2a.log
export OMP_NUM_THREADS=128; numactl --cpunodebind=0 --preferred=0 ./hw1/part1/a1.out >> assign2a.log
export OMP_NUM_THREADS=256; numactl --cpunodebind=0 --preferred=0 ./hw1/part1/a1.out >> assign2a.log

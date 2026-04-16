#!/bin/bash
#SBATCH --partition cosma8
#SBATCH -A dr004
#SBATCH --job-name=perf_test
#SBATCH --time=00:05:00

echo "PATH=$PATH"
which perf
perf --version

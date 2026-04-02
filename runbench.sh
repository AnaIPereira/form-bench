#!/bin/bash

#SBATCH --partition cosma8
#SBATCH -A dr004
#SBATCH --job-name=formbenchmark
#SBATCH --output=log/logfile-%A-%a.log
#SBATCH --export=ALL
#SBATCH --mem=20G
#SBATCH --cpus-per-task=1
#SBATCH --time 0-01:00:00

export FORMTMP=${TMPDIR:-/tmp}

./run-compare.sh



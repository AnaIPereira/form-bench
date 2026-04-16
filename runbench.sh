#!/bin/bash

#SBATCH --partition cosma8
#SBATCH -A dr004
#SBATCH --job-name=formbenchmark

#for arrays use
#SBATCH --output=log/logfile-%A-%a.log 

#SBATCH --export=ALL
#SBATCH --mem=40G
#SBATCH --cpus-per-task=16
#SBATCH --time 0-10:00:00
#SBATCH --mail-type=END # notifications 
#SBATCH --mail-user=ana.costa-pereira@liverpool.ac.uk

#SBATCH --array=1-8
case $SLURM_ARRAY_TASK_ID in
       1) TESTS="trace" ;;
       2) TESTS="mincer" ;;
esac       

#running for array
#./run-compare.sh --tests="$TESTS" --label="testsAna_32_difficulty=3_40G_40h" --timestamp="1" --testdir="$TMPDIR"

OUT=perf-${SLURM_ARRAY_TASK_ID}

perf record -g -o ${OUT}.data \
./run-compare.sh --tests="$TESTS" \
--label="testsAna_16_difficulty=1_40G_10h_N=1_perf" \
--timestamp="1" \
--testdir="$TMPDIR"

perf report --stdio -i ${OUT}.data > ${OUT}.txt



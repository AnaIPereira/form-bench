#!/bin/bash
#SBATCH --partition cosma8
#SBATCH -A dr004
#SBATCH --job-name=formbenchmark
#SBATCH --output=log/logfile-%A-%a.log
#SBATCH --mem=40G
#SBATCH --cpus-per-task=16
#SBATCH --time=00:10:00
#SBATCH --array=1-2
#SBATCH --partition cosma8


case $SLURM_ARRAY_TASK_ID in
  1) TESTS="trace" ;;
  2) TESTS="mincer" ;;
esac


module load perf

srun perf record -o perf.${SLURM_ARRAY_TASK_ID}.data ./run-compare.sh --tests="$TESTS" --testdir="${TMPDIR:-.}"



#!/bin/bash
#SBATCH --job-name=formbenchmark
#SBATCH --output=log/logfile-%A-%a.log
#SBATCH --mem=40G
#SBATCH --cpus-per-task=16
#SBATCH --time=00:10:00
#SBATCH --mail-type=END # notifications 
#SBATCH --mail-user=ana.costa-pereira@liverpool.ac.uk
#SBATCH --array=1-2

set -e

mkdir -p log

cd $SLURM_SUBMIT_DIR

module load perf || { echo "perf not available"; exit 1; }

which perf

case $SLURM_ARRAY_TASK_ID in
  1) TESTS="trace" ;;
  2) TESTS="mincer" ;;
esac

OUT=perf-${SLURM_ARRAY_TASK_ID}

perf record -g --call-graph fp -o ${OUT}.data -- \
./run-compare.sh --tests="$TESTS" \
--testdir="${TMPDIR:-.}"

perf report --stdio -i ${OUT}.data > ${OUT}.txt || echo "report failed"

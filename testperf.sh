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

set -e

cd $SLURM_SUBMIT_DIR
mkdir -p log

echo "Node: $(hostname)"
echo "PATH=$PATH"

# find perf dynamically
PERF=$(command -v perf) || { echo "perf not found on compute node"; exit 1; }

case $SLURM_ARRAY_TASK_ID in
  1) TESTS="trace" ;;
  2) TESTS="mincer" ;;
esac

OUT=perf-${SLURM_ARRAY_TASK_ID}

echo "Using perf: $PERF"

$PERF record -g --call-graph fp -o ${OUT}.data -- \
./run-compare.sh \
  --tests="$TESTS" \
  --testdir="${TMPDIR:-.}"

$PERF report --stdio -i ${OUT}.data > ${OUT}.txt || echo "report failed"

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

# always run from submission directory
cd $SLURM_SUBMIT_DIR

mkdir -p log

# choose test
case $SLURM_ARRAY_TASK_ID in
  1) TESTS="trace" ;;
  2) TESTS="mincer" ;;
esac

OUT=perf-${SLURM_ARRAY_TASK_ID}

echo "Running test: $TESTS"
echo "Node: $(hostname)"
echo "TMPDIR: ${TMPDIR:-not set}"

# run profiler
/usr/bin/perf record -g --call-graph fp -o ${OUT}.data -- \
./run-compare.sh \
  --tests="$TESTS" \
  --testdir="${TMPDIR:-.}"

# generate report
/usr/bin/perf report --stdio -i ${OUT}.data > ${OUT}.txt || {
  echo "perf report failed"
  exit 0
}

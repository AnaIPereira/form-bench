#!/bin/bash

#SBATCH --partition cosma8
#SBATCH -A dr004
#SBATCH --job-name=formbenchmark

#for arrays use
#SBATCH --output=log/logfile-%A-%a.log 

#SBATCH --export=ALL
#SBATCH --mem=20G
#SBATCH --cpus-per-task=64
#SBATCH --time 0-06:00:00
#SBATCH --mail-type=END # notifications 
#SBATCH --mail-user=ana.costa-pereira@liverpool.ac.uk

#SBATCH --array=1-13
case $SLURM_ARRAY_TASK_ID in
       1) TESTS="trace" ;;
       2) TESTS="mincer" ;;
       3) TESTS="minceex" ;;
       4) TESTS="mass-fact" ;;
       5) TESTS="forcer" ;;
       6) TESTS="forcer-exp" ;;
       7) TESTS="fmft" ;;
       8) TESTS="mbox1l" ;;
       9) TESTS="color" ;;
       10) TESTS="chromatic" ;;
       11) TESTS="sort-small" ;;
       12) TESTS="sort-large" ;;
       13) TESTS="sort-disk" ;;
esac       

#running for array
./run-compare.sh --tests="$TESTS" --label="testsAna_48" --timestamp="1" --testdir="$TMPDIR"

#if running for one test
#./run-compare.sh \
#--tests="trace" \
#--label="testsAnatrace" \
#--timestamp="1" \
#--testdir="$TMP"



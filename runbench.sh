#!/bin/bash

#SBATCH --partition cosma8
#SBATCH -A dr004
#SBATCH --job-name=formbenchmark

#for arrays use
#SBATCH --output=log/logfile-%A-%a.log 

#SBATCH --export=ALL
#SBATCH --mem=40G
#SBATCH --cpus-per-task=64
#SBATCH --time 0-20:00:00
#SBATCH --mail-type=END # notifications 
#SBATCH --mail-user=ana.costa-pereira@liverpool.ac.uk

#SBATCH --array=1-8
case $SLURM_ARRAY_TASK_ID in
       1) TESTS="mincer" ;;
       2) TESTS="minceex" ;;
       3) TESTS="forcer" ;;
       4) TESTS="forcer-exp" ;;
       5) TESTS="fmft" ;;
       6) TESTS="mbox1l" ;;
       7) TESTS="color" ;;
       8) TESTS="chromatic" ;;
esac       

#running for array
./run-compare.sh --tests="$TESTS" --label="testsAna_64_difficulty=2_40G" --timestamp="1" --testdir="$TMPDIR"

#if running for one test
#./run-compare.sh \
#--tests="trace" \
#--label="testsAnatrace" \
#--timestamp="1" \
#--testdir="$TMP"



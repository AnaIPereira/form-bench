#!/bin/bash

#SBATCH --partition cosma8
#SBATCH -A dr004
#SBATCH --job-name=formbenchmark

#for arrays use
#SBATCH --output=log/logfile-%A-%a.log 

#SBATCH --export=ALL
#SBATCH --mem=20G
#SBATCH --cpus-per-task=64
#SBATCH --time 0-10:00:00
#SBATCH --mail-type=END # notifications 
#SBATCH --mail-user=ana.costa-pereira@liverpool.ac.uk

#SBATCH --array=1-9
case $SLURM_ARRAY_TASK_ID in
       1) TESTS="mincer" ;;
       2) TESTS="minceex" ;;
       3) TESTS="mass-fact" ;;
       4) TESTS="forcer" ;;
       5) TESTS="forcer-exp" ;;
       6) TESTS="fmft" ;;
       7) TESTS="mbox1l" ;;
       8) TESTS="color" ;;
       9) TESTS="chromatic" ;;
esac       

#running for array
./run-compare.sh --tests="$TESTS" --label="testsAna_64_difficulty=2" --timestamp="1" --testdir="$TMPDIR"

#if running for one test
#./run-compare.sh \
#--tests="trace" \
#--label="testsAnatrace" \
#--timestamp="1" \
#--testdir="$TMP"



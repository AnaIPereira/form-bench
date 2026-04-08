#!/bin/bash

#SBATCH --partition cosma8
#SBATCH -A dr004
#SBATCH --job-name=formbenchmark

#for arrays use
#SBATCH --output=log/logfile-%A-%a.log 

#not arrays use
#SBATCH --output=log/logfile-%j.log

#SBATCH --export=ALL
#SBATCH --mem=20G
#SBATCH --cpus-per-task=16
#SBATCH --time 0-05:00:00
#SBATCH --mail-type=END # notifications 
#SBATCH --mail-user=ana.costa-pereira@liverpool.ac.uk

#threads for tform
#THREADS=4

#SBATCH --array=1-10
#case $SLURM_ARRAY_TASK_ID in
 #      1) TESTS="trace" ;;
#esac

#running for array
#./run-compare.sh --tests="$TESTS" --label="testsAna" --timestamp="1" --testdir="$TMP"

#if running for one test
./run-compare.sh \
--tests="trace" \
--label="testsAnatrace" \
--timestamp="1" \
--testdir="$TMP"



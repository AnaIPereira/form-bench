#!/bin/bash

#SBATCH --partition cosma8
#SBATCH -A dr004
#SBATCH --job-name=formbenchmark

#for arrays use
#SBATCH --output=log/logfile-%A-%a.log 

#SBATCH --export=ALL
#SBATCH --mem=20G
#SBATCH --cpus-per-task=32
#SBATCH --time 0-4:00:00
#SBATCH --mail-type=END # notifications 
#SBATCH --mail-user=ana.costa-pereira@liverpool.ac.uk

#SBATCH --array=1-3

case $SLURM_ARRAY_TASK_ID in
       1) TESTS="sort-2-disk" ;;
       2) TESTS="sort-2-large" ;;
       3) TESTS="sort-2-small" ;;
esac      
 

#running for array
./run-compare.sh --tests="$TESTS" --label="sotbotsfunc" --timestamp="1" --testdir="$TMPDIR" --form_cmds="tformsb -w8, tformsb -w16, tformsb -w32"




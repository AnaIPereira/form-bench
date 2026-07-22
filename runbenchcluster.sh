#!/bin/bash

#SBATCH --job-name=formbenchmarksortbots

#for arrays use
#SBATCH --output=log/logfile-%A-%a.log 

#SBATCH --export=ALL
#SBATCH --mem=20G
#SBATCH --cpus-per-task=8
#SBATCH --time 0-2:00:00
#SBATCH --mail-type=END # notifications 
#SBATCH --mail-user=ana.costa-pereira@liverpool.ac.uk

#SBATCH --array=1-3

case $SLURM_ARRAY_TASK_ID in
       1) TESTS="sort-2-disk" ;;
       2) TESTS="sort-2-large" ;;
       3) TESTS="sort-2-small" ;;
esac      
 
export LD_LIBRARY_PATH="$HOME/software/lib:$LD_LIBRARY_PATH"

#running for array
./run-compare.sh \
  --tests="$TESTS" \
  --label="sortbotsfunc8" \
  --timestamp="1" \
  --testdir="$TMPDIR" \
  --form_cmds="$HOME/software/bin/tformsb -w8, $HOME/software/bin/tform5.0 -w8"




#!/bin/bash

#SBATCH --job-name=formbenchmarksortbots

#for arrays use
#SBATCH --output=log/logfile-%A-%a.log 

#SBATCH --export=ALL
#SBATCH --mem=20G
#SBATCH --cpus-per-task=32
#SBATCH --time 0-10:00:00
#SBATCH --mail-type=END # notifications 
#SBATCH --mail-user=ana.costa-pereira@liverpool.ac.uk

#SBATCH --array=1-4

case $SLURM_ARRAY_TASK_ID in
       1) TESTS="sort-2-disk" ;;
       2) TESTS="sort-2-large" ;;
       3) TESTS="sort-2-small" ;;
       4) TESTS="chromatic" ;;
esac      
 
export LD_LIBRARY_PATH="$HOME/software/lib:$LD_LIBRARY_PATH"


./run-compare.sh \
  --tests "$TESTS" \
  --label "sortbotsfunc32" \
  --timestamp "1" \
  --testdir "$TMPDIR" \
  --runs 30 \
  --form_cmds "$HOME/software/bin/tformsortbot -w32,$HOME/software/bin/tformformmaster -w32"




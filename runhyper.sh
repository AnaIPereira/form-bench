#!/bin/bash

#SBATCH --job-name=hypertest

#for arrays use
#SBATCH --output=log/logfile-%A-%a.log 

#SBATCH --export=ALL
#SBATCH --mem=40G
#SBATCH --cpus-per-task=16
#SBATCH --time 0-00:10:00
#SBATCH --mail-type=END # notifications 
#SBATCH --mail-user=ana.costa-pereira@liverpool.ac.uk

#SBATCH --array=1

case $SLURM_ARRAY_TASK_ID in
       1) TESTS="testhyperform" ;;
esac      
 
export LD_LIBRARY_PATH="$HOME/software/lib:$LD_LIBRARY_PATH"


./run-compare.sh \
  --tests "$TESTS" \
  --label "hyper" \
  --timestamp "1" \
  --testdir "$TMPDIR" \
  --runs 1 \
  --form_cmds "$HOME/software/bin/tform222fb05 -w16,$HOME/software/bin/tform5f8f71d -w16"




#!/bin/bash

#SBATCH --job-name=alltests16t5.0.2
#SBATCH --nodelist=node10
#for arrays use
#SBATCH --output=log/logfile-%A-%a.log 

#SBATCH --export=ALL
#SBATCH --mem=50G
#SBATCH --cpus-per-task=32
#SBATCH --time 1-00:00:00
#SBATCH --mail-type=END # notifications 
#SBATCH --mail-user=ana.costa-pereira@liverpool.ac.uk

#SBATCH --array=1-22%10

case $SLURM_ARRAY_TASK_ID in
       1) TESTS="color" ;;
       2) TESTS="fmft" ;;
       3) TESTS="forcer" ;;
       4) TESTS="chromatic" ;;
       5) TESTS="forcer-exp" ;;
       6) TESTS="minceex" ;;
       7) TESTS="mincer" ;;
       8) TESTS="sort-disk" ;;
       9) TESTS="ibp-mbox1l" ;;
       10) TESTS="ibp-box2l" ;;
       11) TESTS="ibp-npbox2l" ;;
       12) TESTS="ibp-nptri3l" ;;
       13) TESTS="ibp-pent1l" ;;
       14) TESTS="ibp-tri3l" ;;
       15) TESTS="mzv-dm " ;;
       16) TESTS="sort-large" ;;
       17) TESTS="sort-small" ;;
       18) TESTS="sort-2-disk" ;;
       19) TESTS="sort-2-large" ;;
       20) TESTS="sort-2-small" ;;
       21) TESTS="trace" ;;
       22) TESTS="test" ;;
esac      
 
 

 
export LD_LIBRARY_PATH="$HOME/software/lib:$LD_LIBRARY_PATH"


./run-compare.sh \
  --tests "$TESTS" \
  --label "15-10" \
  --timestamp "1" \
  --testdir "$TMPDIR" \
  --runs 3 \
  --form_cmds "$HOME/software/bin/tform -w16,$HOME/software/bin/tformMIN16 -w16,$HOME/software/bin/tformMIN32 -w16,$HOME/software/bin/tformMIN64 -w16"




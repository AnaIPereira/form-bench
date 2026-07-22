#!/bin/bash

#SBATCH --partition cosma8
#SBATCH -A dr004
#SBATCH --job-name=formbenchmark

#for arrays use
#SBATCH --output=log/logfile-%A-%a.log 

#SBATCH --export=ALL
#SBATCH --mem=20G
#SBATCH --cpus-per-task=64
#SBATCH --time 0-5:00:00
#SBATCH --mail-type=END # notifications 
#SBATCH --mail-user=ana.costa-pereira@liverpool.ac.uk

#SBATCH --array=1-11

case $SLURM_ARRAY_TASK_ID in
       1) TESTS="mincer" ;;
       2) TESTS="minceex" ;;
       3) TESTS="forcer" ;;
       4) TESTS="forcer-exp" ;;
       5) TESTS="fmft" ;;
       6) TESTS="mbox1l" ;;
       7) TESTS="chromatic" ;;
       8) TESTS="trace" ;;
       9) TESTS="sort-small" ;;
       10) TESTS="sort-large" ;;
       11) TESTS="sort-disk" ;;
esac      




#case $SLURM_ARRAY_TASK_ID in
 #      1) TESTS="nowork-10" ;;
  #     2) TESTS="nowork-100" ;;
   #    3) TESTS="ordered-10" ;;
    #   4) TESTS="ordered-100" ;;
     #  5) TESTS="disordered-10" ;;
      # 6) TESTS="disordered-100" ;;
#esac  

     

#running for array
./run-compare.sh --tests="$TESTS" --label="FORMplotnt" --timestamp="1" --testdir="$TMPDIR" --form_cmds="tform8_1_1 -w4, tform8_1_1 -w8, tform8_1_1 -w16, tform8_1_1 -w32, tform8_1_1 -w64"




#!/bin/bash

#SBATCH --partition cosma8
#SBATCH -A dr004
#SBATCH --job-name=formbenchmark

#for arrays use
#SBATCH --output=log/logfile-%A-%a.log 

#SBATCH --export=ALL
#SBATCH --mem=20G
#SBATCH --cpus-per-task=32
#SBATCH --time 0-05:00:00
#SBATCH --mail-type=END # notifications 
#SBATCH --mail-user=ana.costa-pereira@liverpool.ac.uk

#SBATCH --array=1-6

#case $SLURM_ARRAY_TASK_ID in
 #      1) TESTS="mincer" ;;
  #     2) TESTS="minceex" ;;
   #    3) TESTS="forcer" ;;
    #   4) TESTS="forcer-exp" ;;
     #  5) TESTS="fmft" ;;
      # 6) TESTS="mbox1l" ;;
       #7) TESTS="colour" ;;
       #8) TESTS="chromatic" ;;
#esac      

case $SLURM_ARRAY_TASK_ID in
       1) TESTS="nowork-10" ;;
       2) TESTS="nowork-100" ;;
       3) TESTS="ordered-10" ;;
       4) TESTS="ordered-100" ;;
       5) TESTS="disordered-10" ;;
       6) TESTS="disordered-100" ;;
esac  

     

#running for array
./run-compare-local.sh --tests="$TESTS" --label="tests_t=32_difficulty=1_20G_N=60" --timestamp="1" --testdir="$TMPDIR"




#!/bin/bash

#SBATCH --partition cosma8
#SBATCH -A dr004
#SBATCH --job-name=formbenchmark
#SBATCH --output=log/logfile-%A-%a.log
#SBATCH --export=ALL
#SBATCH --mem=20G
#SBATCH --cpus-per-task=1
#SBATCH --time 0-01:00:00

#SBATCH --array=1-10

do
    case $SLURM_ARRAY_TASK_ID in:
        1) TESTS="trace" ;;
    esac
done

./run-compare.sh --tests="$TESTS" --label="myname" --timestamp="1" --testdir="$TMP"



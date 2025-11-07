#!/bin/bash
set -eE
trap 'echo Cleaning up ; rm -rf $TESTDIR $ORIGDIR/output/$TIMESTAMP' ERR

# Compare two FORM binaries over the range of benchmarks. This is useful for
# testing performance optimizations of the code, or checking for regressions.

ORIGDIR=$(pwd)

TIMESTAMP=$(date +%Y-%m-%d-%H-%M-%S)

RESULTSDIR=$ORIGDIR/output/$TIMESTAMP/results/
LOGDIR=$ORIGDIR/output/$TIMESTAMP/logs/
mkdir -p "$RESULTSDIR"
mkdir -p "$LOGDIR"

TESTDIR="/dev/shm/form-bench-$TIMESTAMP/"
mkdir "$TESTDIR"

TMPDIR=$TESTDIR/formtmp
mkdir "$TMPDIR"
export FORMTMP=$TMPDIR

cp -r "$ORIGDIR"/tests/* "$TESTDIR"
cd "$TESTDIR"


# For reference, a 7900X with tform -w24 takes about N*30m to run through all
# tests with two binaries, and form takes about N*6hr. This is quite a long
# time, but we want tests that are representative of real-world use.
N=1
declare -A runs
runs["trace"]=$((     N * 30 ))
runs["mincer"]=$((    N * 2  ))
runs["minceex"]=$((   N * 3  ))
runs["mass-fact"]=$(( N * 2  ))
runs["forcer"]=$((    N * 2  ))
runs["forcer-exp"]=$((N * 2  ))
runs["mbox1l"]=$((    N * 8  ))
runs["color"]=$((     N * 8  ))
runs["chromatic"]=$(( N * 2  ))

# A warmup run helps to get stable times from very short-running tests.
declare -A warmup
warmup["trace"]=1
warmup["mincer"]=0
warmup["minceex"]=0
warmup["mass-fact"]=0
warmup["forcer"]=0
warmup["forcer-exp"]=0
warmup["mbox1l"]=0
warmup["color"]=0
warmup["chromatic"]=0


FORM1="tform-test"
FORM2="tform-test-fix"
CPU=24

#TESTS="trace mincer minceex mass-fact forcer forcer-exp mbox1l color chromatic"
TESTS="trace mincer minceex forcer forcer-exp mbox1l color chromatic"


for test in $TESTS; do
	(
	cd "$test"
	echo ""
	echo ""
	echo Running "$test"
	hyperfine --warmup "${warmup[$test]}" --runs "${runs[$test]}" \
		--export-json "$RESULTSDIR/results-$test.json" \
		--export-markdown "$RESULTSDIR/table-$test.md" \
		--command-name "$FORM1 -w$CPU" \
		--command-name "$FORM2 -w$CPU" \
		"nice -n -10 $FORM1 -w$CPU $test.frm > $LOGDIR/$test.log1" \
		"nice -n -10 $FORM2 -w$CPU $test.frm > $LOGDIR/$test.log2"
	)
done


cd "$ORIGDIR"
rm -rf "$TESTDIR"


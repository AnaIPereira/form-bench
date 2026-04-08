#!/bin/bash
set -eE
trap 'echo Cleaning up ; rm -rf $TESTDIR $ORIGDIR/output/$TIMESTAMP' ERR


###############################################################################

# Compare multiple FORM binaries over the range of benchmarks. This is useful
# for testing performance optimizations of the code or checking for regressions.
# Parallel scaling can be checked by using the same binary with multiple worker
# counts.


# Configurable parameters: set their default values and then re-set with args

LABEL="scaling-test-Ana"
TESTDIRBASE="/dev/shm/"
TIMESTAMP=$(date +%Y-%m-%d-%H-%M-%S)
# Negative nice can lead to more consistent timings if the user has permission.
NICE=0

FORM_CMDS="\
form,\
# tform -w1,\
# tform -w2,\
tform -w4,\
# tform -w6,\
# tform -w8,\
# tform -w10,\
# tform -w12,\
# tform -w16,\
# tform -w20,\
# tform -w24\
"

#TESTS="trace mincer minceex mass-fact forcer forcer-exp fmft mbox1l color chromatic sort-small sort-large sort-disk"
# TESTS="trace mincer minceex forcer forcer-exp fmft mbox1l color chromatic sort-small sort-large sort-disk"

TESTS="trace"

# Number of times to run test batches:
N=1

# Run a harder version of the tests? Not all tests are affected by this.
DIFFICULTY=1


options=$(getopt -o "" -l "label:,testdir:,timestamp:,nice:,form_cmds:,tests:,runs:,difficulty:" -- "$@")
if [ $? -ne 0 ]; then
	echo "Error, invalid argument in $@"
fi
eval set -- $options
while :
do
	case "$1" in
		--label)      LABEL="$2"; shift 2 ;;
		--testdir)    TESTDIRBASE="$2"; shift 2 ;;
		--timestamp)  TIMESTAMP="$2"; shift 2 ;;
		--nice)       NICE="$2"; shift 2 ;;
		--form_cmds)  FORM_CMDS="$2"; shift 2 ;;
		--tests)      TESTS=$(echo "$2" | sed 's/,/ /g'); shift 2 ;;
		--runs)       N="$2"; shift 2 ;;
		--difficulty) DIFFICULTY="$2"; shift 2 ;;
		--) shift; break ;;
		*) echo "Error, invalid option: $1" ;;
	esac
done
# this variable needs to be available within form scripts:
export DIFFICULTY

echo "form-bench: $(git rev-parse HEAD)"
echo "	LABEL      = $LABEL"
echo "	TESTDIR    = $TESTDIRBASE"
echo "	TIMESTAMP  = $TIMESTAMP"
echo "	NICE       = $NICE"
echo "	FORM_CMDS  = $FORM_CMDS"
echo "	TESTS      = $TESTS"
echo "	N          = $N"
echo "	DIFFICULTY = $DIFFICULTY"


###############################################################################


# For reference, a 7900X with tform -w24 takes about N*30m to run through all
# DIFFICULTY=1 tests with two binaries, and form takes about N*6hr.
# This is quite a long time, but we want tests that are representative of
# real-world use.
declare -A runs
runs["trace"]=$((     N * 30 ))
runs["mincer"]=$((    N * 2  ))
runs["minceex"]=$((   N * 3  ))
runs["mass-fact"]=$(( N * 2  ))
runs["forcer"]=$((    N * 2  ))
runs["forcer-exp"]=$((N * 2  ))
runs["fmft"]=$((      N * 3  ))
runs["mbox1l"]=$((    N * 8  ))
runs["color"]=$((     N * 8  ))
runs["chromatic"]=$(( N * 2  ))
runs["sort-large"]=$((N * 2  ))
runs["sort-small"]=$((N * 15 ))
runs["sort-disk"]=$(( N * 2  ))

# A warmup run helps to get stable times from very short-running tests.
declare -A warmup
warmup["trace"]=1
warmup["mincer"]=0
warmup["minceex"]=0
warmup["mass-fact"]=0
warmup["forcer"]=0
warmup["forcer-exp"]=0
warmup["fmft"]=0
warmup["mbox1l"]=0
warmup["color"]=0
warmup["chromatic"]=0
warmup["sort-large"]=0
warmup["sort-small"]=1
warmup["sort-disk"]=0

###############################################################################


# Check for python and hyperfine:
for bin in hyperfine python3; do
	if ! command -v "$bin" &> /dev/null; then
		echo "Error, script requires $bin"
		exit 1
	fi
done

# Check for the specified FORM binaries:
for form in $(echo "$FORM_CMDS" | sed -e 's/ -w[0-9]\+//g' -e's/,/ /g'); do
	if ! command -v "$form" &> /dev/null; then
		echo "Error, script requires $form"
		exit 1
	fi
done

ORIGDIR=$(pwd)
RESULTSDIR=$ORIGDIR/output/$LABEL-$TIMESTAMP/results/
echo "Results: $RESULTSDIR"
LOGDIR=$ORIGDIR/output/$LABEL-$TIMESTAMP/logs/
mkdir -p "$RESULTSDIR"
mkdir -p "$LOGDIR"

TESTDIR="$TESTDIRBASE/form-bench-$TIMESTAMP/"
mkdir "$TESTDIR"

TMPDIR=$TESTDIR/formtmp
mkdir "$TMPDIR"
export FORMTMP=$TMPDIR

cp -r "$ORIGDIR"/tests/* "$TESTDIR"
cd "$TESTDIR"


for test in $TESTS; do
	(
	cd "$test"
	echo ""
	echo ""
	echo Running "$test"
	hyperfine --warmup "${warmup[$test]}" --runs "${runs[$test]}" \
		--export-json "$RESULTSDIR/results-$test.json" \
		--export-markdown "$RESULTSDIR/table-$test.md" \
		--parameter-list form "$FORM_CMDS" \
		--command-name "{form}" \
		"nice -n $NICE {form} $test.frm > $LOGDIR/$test.log"
	)
done
python3 "$ORIGDIR/scripts/plot-compare.py" "$RESULTSDIR"

# Clean up
cd "$ORIGDIR"
rm -rf "$TESTDIR"

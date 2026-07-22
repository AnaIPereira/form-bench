#!/bin/bash
set -eE
trap 'echo Cleaning up ; rm -rf "$TESTDIR"' ERR


###############################################################################

# Compare multiple FORM binaries over the range of benchmarks. This is useful
# for testing performance optimizations of the code or checking for regressions.
# Parallel scaling can be checked by using the same binary with multiple worker
# counts.


# Configurable parameters: set their default values and then re-set with user args

# The output directory will be called $LABEL-$TIMESTAMP
LABEL="run-label"
TIMESTAMP=$(date +%Y-%m-%d-%H-%M-%S)

# The tests will be run from here. Using a tmpfs (eg, /dev/shm or perhaps /tmp)
# removes disk-access performance effects from the timings.
TESTDIRBASE="${TMPDIR:-/tmp}"

# Negative nice can lead to more consistent timings, if the user has permission.
NICE=0

# Comma-separated list of FORM commands to test:
FORM_CMDS="\
tform-5.0.0 -w8\
,tform-master -w8\
"

# All current tests
TESTS="chromatic color fmft forcer forcer-exp hyperform \
ibp-box2l ibp-mbox1l ibp-npbox2l ibp-nptri3l ibp-pent1l ibp-tri3l \
minceex mincer mzv-dm sort-disk sort-large sort-small \
sort-2-disk sort-2-large sort-2-small trace"

# Number of times to run test batches. Running more times leads to more reliable
# timing statistics, but increases total test run time.
N=1

# Run a harder version of the tests? Not all tests are affected by this.
DIFFICULTY=1


# Now check for non-default options from the user:
while [[ $# -gt 0 ]]; do
	case "$1" in
		--label)      LABEL="$2"; shift 2 ;;
		--testdir)    TESTDIRBASE="$2"; shift 2 ;;
		--timestamp)  TIMESTAMP="$2"; shift 2 ;;
		--nice)       NICE="$2"; shift 2 ;;
		--form_cmds)  FORM_CMDS="$2"; shift 2 ;;
		--tests)      TESTS=${2//,/ }; shift 2 ;;
		--runs)       N="$2"; shift 2 ;;
		--difficulty) DIFFICULTY="$2"; shift 2 ;;
		--) shift; break ;;
		*) echo "Error, invalid option: $1"; exit 1 ;;
	esac
done
# This variable needs to be available within form scripts:
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

# Load test run and warmup counts:
tests_list=()
runs_list=()
warmup_list=()
for test in $TESTS; do
	if [ ! -d "tests/$test" ]; then
		echo "Error, invalid test: $test"
		exit 1
	fi

	source "tests/$test/conf.sh"
	tests_list+=("$test")
	if [ $N -eq 0 ]; then
		runs_list+=("1")
		warmup_list+=("0")
	else
		runs_list+=("$((N * runs))")
		warmup_list+=("$warmup")
	fi
done

# Check for python3 and hyperfine:
for bin in hyperfine python3; do
	if ! command -v "$bin" &> /dev/null; then
		echo "Error, script requires $bin"
		exit 1
	fi
done

# Check for the specified FORM binaries:
for form in $(echo "$FORM_CMDS" | sed -E -e 's/ -w[0-9]+//g' -e 's/,/ /g'); do
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
mkdir -p "$TESTDIR"

TMPDIR=$TESTDIR/formtmp
mkdir "$TMPDIR"
export FORMTMP=$TMPDIR

cp -r "$ORIGDIR"/tests/* "$TESTDIR"
cd "$TESTDIR"


for i in "${!tests_list[@]}"; do
	test="${tests_list[$i]}"
	run_count="${runs_list[$i]}"
	warmup_count="${warmup_list[$i]}"
	(
	cd "$test"
	echo ""
	echo ""
	echo Running "$test"
	hyperfine \
		--warmup "$warmup_count" \
		--runs "$run_count" \
		--export-json "$RESULTSDIR/results-$test.json" \
		--export-markdown "$RESULTSDIR/table-$test.md" \
		--parameter-list form "$FORM_CMDS" \
		--command-name "{form}" \
		"nice -n $NICE {form} $test.frm > $LOGDIR/$test.log"
	)
done
#python3 "$ORIGDIR/scripts/plot-compare.py" "$RESULTSDIR"

# Clean up
cd "$ORIGDIR"
rm -rf "$TESTDIR"


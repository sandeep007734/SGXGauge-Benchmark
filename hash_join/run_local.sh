# CHANGE THESE
CMD=$@
echo $CMD
# exit 1

export PATH="/home/sandeep/Desktop/work/phd/intel/linux/tools/perf:$PATH"
export PATH="/home/sandeep/Desktop/work/phd/SecureFS/graphene/Runtime:$PATH"
export PATH="/home/sandeep/Desktop/work/phd/intel/linux/tools/perf:$PATH"

./prepare_graphene.sh
TREND_DIR="./scripts"
PERF_EVENTS=$(cat ${TREND_DIR}/perf-all-fmt-less)
CONT_PERF_EVENTS=$(cat ${TREND_DIR}/perf-trend-fmt-less)
# CHANGE THESE



PERF_TIMER=10000
PERF="/usr/bin/perf"
OUTFILE="log.dat"
LOGFILE="log"

RUNDIR="."

$CMD 2>&1 | tee -a $LOGFILE &
BENCHMARK_PID=$!


SECONDS=0
DURATION=$SECONDS
#echo "Benchmark ready in Time (seconds): $DURATION" 2>&1 | tee -a $OUTFILE
SECONDS=0

$PERF stat -x, -o $OUTFILE --append -e $PERF_EVENTS -p $BENCHMARK_PID &
PERF_STATUS=$?
PERF_PID=$!

echo "Benchmark PID is "$BENCHMARK_PID
echo "PERF PID is "$PERF_PID

wait $BENCHMARK_PID 2>/dev/null
wait $PERF_PID 2>/dev/null

kill -INT $PERF_PID &>/dev/null

DURATION=$SECONDS
echo "Execution Time (seconds): $DURATION" >>$OUTFILE
echo "****success****" >>$OUTFILE


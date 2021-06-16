#!/bin/bash
if [[ $EUID -ne 0 ]];then
    echo "Please run as root"
    exit
fi


if [ $# -eq 0 ];then
    echo "Need arg"
    exit 1
fi


EXEC_TYPE=$1
WORKLOAD_TYPE=$2
user=$(who|awk '{print $1}')

if [ "$WORKLOAD_TYPE" = "LOW_" ]; then
    BENCH_ARGS=" -g 150 -l 100"
elif [ "$WORKLOAD_TYPE" = "MEDIUM_" ]; then
    BENCH_ARGS=" -g 250 -l 100"
elif [ "$WORKLOAD_TYPE" = "HIGH_" ]; then
    BENCH_ARGS=" -s small -l 100"
else
    echo "ERROR"
    exit 1
fi

BENCH="xsbench"
EXP_NAME="sgxgauge_$WORKLOAD_TYPE"

if [ $EXEC_TYPE -eq 1 ];then
    PREFIX="SGX-GRAPHENE-${BENCH}"
    MANIFEST_FILE="xsbench"
    make xsbench.manifest.sgx
    CMD="graphene-sgx ${MANIFEST_FILE} ${BENCH_ARGS} "
elif [ $EXEC_TYPE -eq 2 ];then
    PREFIX="SGX-PGRAPHENE-${BENCH}"
    MANIFEST_FILE="pxsbench"
    make pxsbench.manifest.sgx
    CMD="graphene-sgx ${MANIFEST_FILE} ${BENCH_ARGS}  "
elif [ $EXEC_TYPE -eq 3 ];then
    PREFIX="NOSGX-VANILLA-${BENCH}"
    CMD="./openmp-threading/XSBench ${BENCH_ARGS}"

else
    echo "ERROR"
    exit 1
fi



if [ -e ./prepare_graphene.sh ];then
    echo "Running prepare scripts"
    ./prepare_graphene.sh $PREFIX
fi


# ======================================================================================
# ============================ SETTING UP===============================================
# ======================================================================================

TMP_FILE="/tmp/alloctest-bench.ready"
QUIT_FILE="/tmp/alloctest-bench.quit"
TREND_DIR="../scripts"
PERF="/usr/bin/perf"

MAIN_DIR="evaluation/${EXP_NAME}/${BENCH}/graphene-"$PREFIX"-"$(date +"%Y%m%d-%H%M%S")
mkdir -p $MAIN_DIR
PRE_OUTFILE=${MAIN_DIR}"/perflog"
OUTFILE=${MAIN_DIR}"/perflog-"$PREFIX"-log.dat"
LOGFILE=${MAIN_DIR}"/perflog-"$PREFIX"-securefsartifactlog"
SGXFILE=${MAIN_DIR}"/perflog-"$PREFIX"-sgxlog"

# RUNDIR="."

echo $PREFIX
echo $CMD
echo $OUTFILE
echo $LOGFILE
char="-"
hy_cont=$(awk -F"${char}" '{print NF-1}' <<< "${PREFIX}")
if [ $hy_cont -ne 2 ];then
    echo $PREFIX $hy_cont "is not formatted properly. SGX-GRAHPHENE-iozone"
    exit 1
fi

# ======================================================================================
# ============================ START UP===============================================
# ======================================================================================

rm ${TMP_FILE}
rm ${QUIT_FILE}

if [ "$user" = "sandeep" ]; then
    ${TREND_DIR}/test_ioctl.o 1
    ${TREND_DIR}/test_ioctl.o  &> ${SGXFILE}
    PERF_EVENTS=$(cat ${TREND_DIR}/perf-all-fmt)
    CONT_PERF_EVENTS=$(cat ${TREND_DIR}/perf-trend-fmt)
else
    PERF_EVENTS=$(cat ${TREND_DIR}/perf-all-fmt-less)
    CONT_PERF_EVENTS=$(cat ${TREND_DIR}/perf-trend-fmt-less)
fi

$PERF stat -x, -o $OUTFILE -e $PERF_EVENTS  $CMD 2>&1 | tee  $LOGFILE &


while [ -z "$BENCHMARK_PID" ]; do
        sleep .5
        echo "-------------------------------------------------------------"
        if [ $EXEC_TYPE -ne 3 ];then
            ps aux|grep "graphene/sgx/libpal.so"|grep sgx|grep -v color|grep -v perf|grep -v "grep"
            ps aux|grep "graphene/sgx/libpal.so"|grep sgx|grep -v color|grep -v perf|grep -v "grep"|awk '{print $2}'
            BENCHMARK_PID=$(ps aux|grep "graphene/sgx/libpal.so"|grep sgx|grep -v color|grep -v perf|grep -v "grep"|awk '{print $2}')
        else
            
            
            ps aux|grep ./openmp-threading/XSBench|grep -v color|grep -v perf|grep -v "grep"
            ps aux|grep ./openmp-threading/XSBench|grep -v color|grep -v perf|grep -v "grep"|awk '{print $2}'
            BENCHMARK_PID=$(ps aux|grep ./openmp-threading/XSBench|grep -v color|grep -v perf|grep -v "grep"|awk '{print $2}')
        fi
        echo "Benchmark PID is "$BENCHMARK_PID
        echo "-------------------------------------------------------------"
done

SECONDS=0
DURATION=$SECONDS
SECONDS=0

# ======================================================================================
# ============================ CONT SETUP===============================================
# ======================================================================================

CONT_PERF_EVENTS=$(cat ${TREND_DIR}/perf-trend-fmt)
echo "Starting the monitor"
PERF_TIMER=1000
SLEEP_DURATION=2

# sleep 2

sync; echo 3 > /proc/sys/vm/drop_caches
$PERF stat -I $PERF_TIMER -e $CONT_PERF_EVENTS -p $BENCHMARK_PID &>${PRE_OUTFILE}.perf &

${TREND_DIR}/mem_stats.sh $BENCHMARK_PID ${PRE_OUTFILE}.meminfo $SLEEP_DURATION  &
${TREND_DIR}/graph_stats.sh $BENCHMARK_PID ${PRE_OUTFILE}.status $SLEEP_DURATION &
${TREND_DIR}/capture.sh $BENCHMARK_PID $MAIN_DIR $SLEEP_DURATION &

# ======================================================================================
# ============================ WAITING =================================================
# ======================================================================================
while  ps -p $BENCHMARK_PID > /dev/null 2>&1; do
    sleep 0.1
done

wait $BENCHMARK_PID 2>/dev/null
# kill -INT $PERF_PID &>/dev/null



DURATION=$SECONDS
echo "Execution Time (seconds): $DURATION" >>$OUTFILE

if [ "$user" = "sandeep" ]; then
    ${TREND_DIR}/test_ioctl.o  &>> ${SGXFILE}
    sudo chown -R sandeep:sandeep -- *
else
    sudo chown -R abhishek:abhishek -- *
fi

echo "Cleaning"

touch ${QUIT_FILE}

ps -aux | grep mem_stats.sh |  grep -v 'color'

for pid in $(ps -aux | grep mem_stats.sh |  grep -v 'color' | awk '{print $2}'); do kill -9 $pid; done
for pid in $(ps -aux | grep graph_stats.sh |  grep -v 'color' | awk '{print $2}'); do kill -9 $pid; done
for pid in $(ps -aux | grep capture.sh |  grep -v 'color' | awk '{print $2}'); do kill -9 $pid; done

echo "****success****" >>$OUTFILE
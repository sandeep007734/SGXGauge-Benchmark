#!/bin/bash
if [[ $EUID -ne 0 ]];then
    echo "Please run as root"
    exit
fi


if [ $# -lt 2 ];then
    echo "Need two args mode and bench"
    exit 1
fi


EXEC_TYPE=$1
BENCH=$2

EXP_NAME="nbench_van_test_final"
BENCH_ARGS=" -cCONF.DAT"

if [ $EXEC_TYPE -eq 1 ];then
    PREFIX="SGX-GRAPHENE-${BENCH}"
    MANIFEST_FILE="nbench"
    make ${MANIFEST_FILE}.manifest.sgx NONPF=1
    CMD="graphene-sgx ${MANIFEST_FILE} ${BENCH_ARGS} "
elif [ $EXEC_TYPE -eq 2 ];then
    PREFIX="SGX-PGRAPHENE-${BENCH}"
    MANIFEST_FILE="pnbench"
    make ${MANIFEST_FILE}.manifest.sgx NONPF=0
    CMD="graphene-sgx ${MANIFEST_FILE} ${BENCH_ARGS}  "
    echo "Need SETUP"
    exit 1
elif [ $EXEC_TYPE -eq 3 ];then
    PREFIX="NOSGX-VANILLA-${BENCH}"
    CMD="./nbench ${BENCH_ARGS}"
elif [ $EXEC_TYPE -eq 4 ];then
    PREFIX="SGX-NATIVE-${BENCH}"
    CMD="./app -u nobody ${BENCH_ARGS}"

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
TREND_DIR="/home/sandeep/Desktop/work/phd/SecureFS/securefs_bench/scripts"
PERF="/usr/bin/perf"

MAIN_DIR="$(pwd)/evaluation/${EXP_NAME}/${BENCH}/perflog-"$PREFIX"-"$(date +"%Y%m%d-%H%M%S")
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

# Restting the SGX counters
${TREND_DIR}/test_ioctl.o 1
${TREND_DIR}/test_ioctl.o  &> ${SGXFILE}

PERF_EVENTS=$(cat ${TREND_DIR}/perf-all-fmt)

cat CONF.DAT > $LOGFILE
echo "" >> $LOGFILE
$PERF stat -x, -o $OUTFILE -e $PERF_EVENTS  $CMD 2>&1 | tee  -a $LOGFILE &


while [ -z "$BENCHMARK_PID" ]; do
        sleep .5
        echo "-------------------------------------------------------------"
        if [ $EXEC_TYPE -lt 3 ];then
            ps aux|grep "graphene/sgx/libpal.so"|grep sgx|grep -v color|grep -v perf|grep -v "grep"
            ps aux|grep "graphene/sgx/libpal.so"|grep sgx|grep -v color|grep -v perf|grep -v "grep"|awk '{print $2}'
            BENCHMARK_PID=$(ps aux|grep "graphene/sgx/libpal.so"|grep sgx|grep -v color|grep -v perf|grep -v "grep"|awk '{print $2}')
        elif [ $EXEC_TYPE -eq 3 ];then
        
            ps aux|grep ./nbench|grep CONF.DAT|grep -v color|grep -v perf|grep -v "grep"
            ps aux|grep ./nbench|grep CONF.DAT|grep -v color|grep -v perf|grep -v "grep"|awk '{print $2}'
            BENCHMARK_PID=$(ps aux|grep ./nbench|grep CONF.DAT|grep -v color|grep -v perf|grep -v "grep"|awk '{print $2}')

        elif [ $EXEC_TYPE -eq 4 ];then
            echo "========"
            exit 1
            ps aux|grep app|grep nobody|grep -v color|grep -v perf|grep -v "grep"
            echo "========"
            # ps aux|grep app|grep nobody|grep -v color|grep -v perf|grep -v "grep"|awk '{print $2}'
            BENCHMARK_PID=$(ps aux|grep app|grep nobody|grep -v color|grep -v perf|grep -v "grep"|awk '{print $2}')
        fi
        echo "========"
        echo "Benchmark PID is "$BENCHMARK_PID
        echo "========"
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

# echo $PERF stat -I $PERF_TIMER -e $CONT_PERF_EVENTS -p $BENCHMARK_PID
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
${TREND_DIR}/test_ioctl.o  &>> ${SGXFILE}

DURATION=$SECONDS
echo "Execution Time (seconds): $DURATION" >>$OUTFILE

sudo chown -R sandeep:sandeep -- *

echo "Cleaning"
touch ${QUIT_FILE}
ps -aux | grep mem_stats.sh |  grep -v 'color'

for pid in $(ps -aux | grep mem_stats.sh |  grep -v 'color' | awk '{print $2}'); do kill -9 $pid; done
for pid in $(ps -aux | grep graph_stats.sh |  grep -v 'color' | awk '{print $2}'); do kill -9 $pid; done
for pid in $(ps -aux | grep capture.sh |  grep -v 'color' | awk '{print $2}'); do kill -9 $pid; done

echo "****success****" >>$OUTFILE

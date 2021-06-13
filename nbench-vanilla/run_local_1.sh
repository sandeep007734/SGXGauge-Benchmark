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

BENCH="nbench"
EXP_NAME="graphene_2"

if [ "$WORKLOAD_TYPE" = "1" ]; then
    BENCH_ARGS="-cNUMSORT.DAT"
elif [ "$WORKLOAD_TYPE" = "2" ]; then
    BENCH_ARGS="-cSTRINGSORT.DAT"
elif [ "$WORKLOAD_TYPE" = "3" ]; then
    BENCH_ARGS="-cBITFIELD.DAT"
elif [ "$WORKLOAD_TYPE" = "4" ]; then
    BENCH_ARGS="-cEMF.DAT"
elif [ "$WORKLOAD_TYPE" = "5" ]; then
    BENCH_ARGS="-cFOUR.DAT"
else
    echo "ERROR"
    exit 1
fi

user=$(whoami)

if [ $EXEC_TYPE -eq 1 ];then
    PREFIX="SGX-GRAPHENE-${BENCH}"
    MANIFEST_FILE="hashjoin"
    make ${MANIFEST_FILE}.manifest.sgx NONPF=1
    CMD="graphene-sgx ${MANIFEST_FILE} ${BENCH_ARGS} "
elif [ $EXEC_TYPE -eq 2 ];then
    PREFIX="SGX-PGRAPHENE-${BENCH}"
    MANIFEST_FILE="phashjoin"
    make ${MANIFEST_FILE}.manifest.sgx NONPF=0
    CMD="graphene-sgx ${MANIFEST_FILE} ${BENCH_ARGS}  "
elif [ $EXEC_TYPE -eq 3 ];then
    PREFIX="NOSGX-VANILLA-${BENCH}"
    CMD="./nbench ${BENCH_ARGS}"

else
    echo "ERROR"
    exit 1
fi


if [ -e ./prepare_graphene.sh ];then
    echo "Running prepare scripts"
    ./prepare_graphene.sh $PREFIX
fi

TREND_DIR="../scripts"

# ======================================================================================
# ============================ SETTING UP===============================================
# ======================================================================================

TMP_FILE="/tmp/alloctest-bench.ready"
QUIT_FILE="/tmp/alloctest-bench.quit"
TREND_DIR="../scripts"
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

if [ "$user" = "sandeep" ]; then
    ${TREND_DIR}/test_ioctl.o 1
    ${TREND_DIR}/test_ioctl.o  &> ${SGXFILE}
    PERF_EVENTS=$(cat ${TREND_DIR}/perf-all-fmt)
else
    PERF_EVENTS=$(cat ${TREND_DIR}/perf-all-fmt-less)
fi

if [ $EXEC_TYPE -ne 4 ]; then
    $PERF stat -x, -o $OUTFILE -e $PERF_EVENTS  $CMD 2>&1 | tee  $LOGFILE &
else
    echo ""
    echo "#!/bin/bash" > ${BENCHHOME}/runme.sh
    echo "if [[ \$EUID -ne 0 ]];then echo "Please run as root. sudo -H -E"; exit 1;  fi" >> ${BENCHHOME}/runme.sh
    echo "make " ${BENCHHOME}/runme.sh
    echo "touch ${TMP_FILE}" >> ${BENCHHOME}/runme.sh
    echo $PERF stat -x, -o $OUTFILE -e $PERF_EVENTS  $CMD 2\>\&1 \| tee  $LOGFILE  >> ${BENCHHOME}/runme.sh
    chmod +x ${BENCHHOME}/runme.sh

    echo "**************************"
    echo "*********WAITING**********"
    echo "**************************"

    while [ ! -f  ${TMP_FILE} ]; do
        sleep 0.1
    done  
    # exit
fi

while [ -z "$BENCHMARK_PID" ]; do
        sleep .5
        echo "-------------------------------------------------------------"
        if [ $EXEC_TYPE -lt 3 ];then
            ps aux|grep "graphene/sgx/libpal.so"|grep sgx|grep -v color|grep -v perf|grep -v "grep"
            ps aux|grep "graphene/sgx/libpal.so"|grep sgx|grep -v color|grep -v perf|grep -v "grep"|awk '{print $2}'
            BENCHMARK_PID=$(ps aux|grep "graphene/sgx/libpal.so"|grep sgx|grep -v color|grep -v perf|grep -v "grep"|awk '{print $2}')
        elif [ $EXEC_TYPE -eq 3 ];then
        
            ps aux|grep ./nbench ${BENCH_ARGS}|grep -v color|grep -v perf|grep -v "grep"
            ps aux|grep ./nbench ${BENCH_ARGS}|grep -v perf|grep -v "grep"|awk '{print $2}'
            BENCHMARK_PID=$(ps aux|grep ./nbench ${BENCH_ARGS}|grep -v color|grep -v perf|grep -v "grep"|awk '{print $2}')

        elif [ $EXEC_TYPE -eq 4 ];then
            echo "========"
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

if [ "$user" = "sandeep" ]; then
    CONT_PERF_EVENTS=$(cat ${TREND_DIR}/perf-trend-fmt)
else
    CONT_PERF_EVENTS=$(cat ${TREND_DIR}/perf-trend-fmt-less)
fi

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

if [ "$user" = "sandeep" ]; then
    ${TREND_DIR}/test_ioctl.o  &>> ${SGXFILE}
fi

DURATION=$SECONDS
echo "Execution Time (seconds): $DURATION" >>$OUTFILE

sudo chown -R abhishek:abhishek -- *

echo "Cleaning"

touch ${QUIT_FILE}

ps -aux | grep mem_stats.sh |  grep -v 'color'

for pid in $(ps -aux | grep mem_stats.sh |  grep -v 'color' | awk '{print $2}'); do kill -9 $pid; done
for pid in $(ps -aux | grep graph_stats.sh |  grep -v 'color' | awk '{print $2}'); do kill -9 $pid; done
for pid in $(ps -aux | grep capture.sh |  grep -v 'color' | awk '{print $2}'); do kill -9 $pid; done

echo "****success****" >>$OUTFILE

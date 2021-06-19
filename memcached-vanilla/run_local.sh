#!/bin/bash
if [[ $EUID -ne 0 ]];then
    echo "Please run as root"
    exit
fi


if [ $# -eq 0 ];then
    echo "Need arg"
    exit 1
fi




# ======================================================================================
# ============================ PARSING ARGS===============================================
# ======================================================================================


EXEC_TYPE=$1
WORKLOAD_TYPE=$2

user=$(who|awk '{print $1}')

# Not required
# make clean; 
make WORKLOAD_TYPE=${WORKLOAD_TYPE} -j12

if [ "$WORKLOAD_TYPE" = "LOW_" ]; then
    STRESS_ARGS=" scripts/workload_low"
elif [ "$WORKLOAD_TYPE" = "MEDIUM_" ]; then
    STRESS_ARGS=" scripts/workload_medium"
elif [ "$WORKLOAD_TYPE" = "HIGH_" ]; then
    STRESS_ARGS=" scripts/workload_high"
else
    echo "ERROR"
    exit 1
fi


BENCH="memcached"
BENCHHOME=$(pwd)
EXP_NAME="sgxgauge_$WORKLOAD_TYPE"

BENCH_ARGS=" -u nobody  -m 200m "
if [ $EXEC_TYPE -eq 1 ];then
    PREFIX="SGX-GRAPHENE-${BENCH}"
    MANIFEST_FILE="memcached"
    make ${MANIFEST_FILE}.manifest.sgx -j12
    CMD="graphene-sgx ${MANIFEST_FILE} ${BENCH_ARGS}  "
elif [ $EXEC_TYPE -eq 2 ];then
    PREFIX="SGX-PGRAPHENE-${BENCH}"
    MANIFEST_FILE="pmemcached"
    make ${MANIFEST_FILE}.manifest.sgx
    CMD="graphene-sgx ${MANIFEST_FILE} ${BENCH_ARGS}  "
elif [ $EXEC_TYPE -eq 3 ];then
    PREFIX="NOSGX-VANILLA-${BENCH}"
    CMD="./memcached ${BENCH_ARGS} "
else
    echo "ERROR. Unknown mode. Supported are 1 2 and 3"
    exit 1
fi


if [ -e ./prepare_graphene.sh ];then
    echo "Running prepare scripts"
    ./prepare_graphene.sh $PREFIX
fi


# ======================================================================================
# ============================ SETTING UP===============================================
# ======================================================================================

echo "Dropping caches"
sync; echo 3 > /proc/sys/vm/drop_caches

echo never | sudo tee >/sys/kernel/mm/transparent_hugepage/enabled
echo never | sudo tee >/sys/kernel/mm/transparent_hugepage/defrag

cat /sys/kernel/mm/transparent_hugepage/enabled 2>&1 | tee -a $OUTFILE
cat /sys/kernel/mm/transparent_hugepage/defrag 2>&1 | tee -a $OUTFILE

echo "Enable performance mode"
sudo cpupower frequency-set --governor performance >/dev/null

echo "Disabling address space randomization"
sudo sysctl kernel.randomize_va_space=0	



TMP_FILE="/tmp/alloctest-bench.ready"
QUIT_FILE="/tmp/alloctest-bench.quit"
TREND_DIR="../scripts"
PERF="/usr/bin/perf"

MAIN_DIR="$(pwd)/evaluation/${EXP_NAME}/${BENCH}/perflog-"$PREFIX"-"$(date +"%Y%m%d-%H%M%S")
mkdir -p $MAIN_DIR
PRE_OUTFILE=${MAIN_DIR}"/perflog"

OUTFILE=${MAIN_DIR}"/perflog-"$PREFIX"-log.dat"
LOGFILE=${MAIN_DIR}"/perflog-"$PREFIX"-securefsartifactlog"
LOADFILE=${MAIN_DIR}"/perflog-"$PREFIX"-ycsbloadlog"
RUNFILE=${MAIN_DIR}"/perflog-"$PREFIX"-ycsbrunlog"
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


echo ""
echo "#!/bin/bash" > ${BENCHHOME}/runme.sh
echo "if [[ \$EUID -ne 0 ]];then echo "Please run as root. sudo -H -E"; exit 1;  fi" >> ${BENCHHOME}/runme.sh
echo "touch ${TMP_FILE}" >> ${BENCHHOME}/runme.sh
echo $PERF stat -x, -o $OUTFILE -e $PERF_EVENTS  $CMD 2\>\&1 \| tee  $LOGFILE  >> ${BENCHHOME}/runme.sh
chmod +x ${BENCHHOME}/runme.sh

echo "**************************"
echo "*********WAITING**********"
echo "**************************"

while [ ! -f  ${TMP_FILE} ]; do
    sleep 0.1
done  

# exit 1


while [ -z "$BENCHMARK_PID" ]; do
        sleep .5
        echo "-------------------------------------------------------------"
        if [ $EXEC_TYPE -ne 3 ];then
            ps aux|grep "graphene/sgx/libpal.so"|grep sgx|grep -v color|grep -v perf|grep -v "grep"
            ps aux|grep "graphene/sgx/libpal.so"|grep sgx|grep -v color|grep -v perf|grep -v "grep"|awk '{print $2}'
            BENCHMARK_PID=$(ps aux|grep "graphene/sgx/libpal.so"|grep sgx|grep -v color|grep -v perf|grep -v "grep"|awk '{print $2}')
        else
            
            # NON SGX HERE
            BENCHMARK_PID=$(ps aux|grep memcached|grep -v color|grep -v perf|grep -v "grep"|awk '{print $2}')
        fi
        echo "Benchmark PID is "$BENCHMARK_PID
        echo "-------------------------------------------------------------"
done

PERF_PID=$(ps aux|grep 'usr/bin/perf stat -x'|grep -v color|grep -v 'grep'|awk '{print $2}')
echo "Main PERF PID is ${PERF_PID}"


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
# ============================ YCSB =================================================
# ======================================================================================
YSCSB_HOME="/home/sandeep/Desktop/work/phd/SecureFS/YCSB"
# Wait for the server to come up
echo "Wating for the server to be up."
sleep 14
# Load the data
${YSCSB_HOME}/bin/ycsb.sh load memcached -s -P ${STRESS_ARGS}  -p "memcached.hosts=127.0.0.1" 2>&1 | tee ${LOADFILE}

SECONDS=0
DURATION=$SECONDS
SECONDS=0


# Run
${YSCSB_HOME}/bin/ycsb.sh run memcached -s -P ${STRESS_ARGS}   -p "memcached.hosts=127.0.0.1" 2>&1 | tee ${RUNFILE}

# ======================================================================================
# ============================ WAITING =================================================
# ======================================================================================

DURATION=$SECONDS
echo "Execution Time (seconds): ${DURATION}" >> $OUTFILE

kill -INT $PERF_PID &>/dev/null
wait $PERF_PID
# kill -INT $BENCHMARK_PID &>/dev/null

echo "Waiting for the benchmark to end"
killall loader
wait $WBENCHMARK_PID 2>/dev/null
echo "Waiting for the benchmark to end.. DONE"



cat ${STRESS_ARGS} >> $LOGFILE

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

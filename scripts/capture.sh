#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Need the log dir and the PID"
    exit 1
fi

PID=$1
LOG_DIR=$2
SLEEP_DURATION=$3

QUIT_FILE="/tmp/alloctest-bench.quit"

echo "$0: PID is $2"
while :; do

    if ! ps -p $PID > /dev/null 2>&1; then
        echo "[capture.sh] Process $PID not found. Exiting"
        exit 1
    fi

    if [ -f  ${QUIT_FILE} ]; then
        echo "Got QUIT command"
        exit 1
    fi  

    cat /sys/kernel/debug/extfrag/unusable_index >> $LOG_DIR/unusable_index
    cat /sys/kernel/debug/extfrag/extfrag_index >> $LOG_DIR/extfrag_index
    cat /proc/vmstat >> $LOG_DIR/vmstat
    cat /proc/meminfo >> $LOG_DIR/meminfo
    cat /proc/buddyinfo >> $LOG_DIR/buddyinfo
    cat /proc/pagetypeinfo >> $LOG_DIR/pagetypeinfo

    if [ -f /home/sandeep/Desktop/work/phd/SecureFS/securefs_bench/scripts/test_ioctl.o ]; then
        /home/sandeep/Desktop/work/phd/SecureFS/securefs_bench/scripts/test_ioctl.o >> $LOG_DIR/sgxtrend
    fi

    cat /proc/interrupts |grep TLB >> $LOG_DIR/tlbshootdowns

    sleep $SLEEP_DURATION
done

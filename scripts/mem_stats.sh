#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Need the log file and the PID"
    exit 1
fi

PID=$1
LOG_FILE=$2
SLEEP_DURATION=$3
QUIT_FILE="/tmp/alloctest-bench.quit"

echo "$0: PID is $PID"

while :; do
    if ! ps -p $PID > /dev/null 2>&1; then
        echo "[mem_stat.sh] Process $PID not found. Exiting"
        exit 1
    fi

    if [ -f  ${QUIT_FILE} ]; then
        echo "Got QUIT command"
        exit 1
    fi  

    cat /sys/devices/system/node/node*/meminfo >>$LOG_FILE
    sleep $SLEEP_DURATION
done

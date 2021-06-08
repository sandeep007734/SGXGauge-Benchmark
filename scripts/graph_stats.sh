#!/bin/bash

if [ $# -lt 3 ]; then
    echo "Need the pid log_file log_file2"
    exit 1
fi

PID=$1
LOG_FILE=$2
SLEEP_DURATION=$3
QUIT_FILE="/tmp/alloctest-bench.quit"

echo "$0: PID is $1"

while :; do
    if ! ps -p $PID > /dev/null 2>&1; then
        echo "[graph.sh] Process $PID not found. Exiting"
        exit 1
    fi

    if [ -f  ${QUIT_FILE} ]; then
        echo "Got QUIT command"
        exit 1
    fi  

    cat /proc/$PID/status >>$LOG_FILE

    
    sleep $SLEEP_DURATION
done

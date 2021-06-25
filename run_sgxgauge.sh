#!/bin/bash

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 
   exit 1
fi

if [ $# -ne 1 ]; then
    echo "Need a workload type. LOW_ | MEDIUM_ | HIGH_"
    exit 1
fi

declare -a suite=("bfs-2" "libcatena" "openssl" "pte-hashjoin" "pte-btree" "pagerank-2")
# declare -a suite=("pagerank-2")
# declare -a suite=("xsbench")
# declare -a suite=("libcatena")

workload_type=$1
CURR_DIR=$(pwd)

for benchmark in "${suite[@]}";
do
    if [ -d ${benchmark}-vanilla ];then
        cd ${benchmark}-vanilla
        # sudo -H -E ./run_local.sh 3 ${workload_type}

        # Only do this for graphene
        make clean
        make
        sudo -H -E ./run_local.sh 1 ${workload_type}
        cd ${CURR_DIR}
    fi

    # if [ -d ${benchmark}-native ];then
    #     cd ${benchmark}-native
    #     sudo -H -E ./run_local.sh 4 ${workload_type}
    #     cd ${CURR_DIR}
    # fi

done
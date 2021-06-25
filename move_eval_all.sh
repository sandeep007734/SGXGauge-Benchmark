#!/bin/bash


declare -a suite=("bfs-2" "libcatena" "openssl" "pte-hashjoin" "pte-btree" "pagerank-2")
# declare -a suite=("pte-btree" "pagerank-2")
# declare -a suite=("xsbench")
# declare -a suite=("libcatena")

workload_type=$1
user=$(who|awk '{print $1}')

CURR_DIR=$(pwd)

move_dir(){
    cd $1
    if [ -d evaluation ];then
        ../move_eval.sh
    fi
    cd ${CURR_DIR}
}

test_dir(){
    ls -lah|grep evaluation*
}

echo "****************"
echo "Moving"
echo "****************"

for benchmark in "${suite[@]}";
do
    echo $benchmark
    if [ -d ${benchmark}-vanilla ];then
        test_dir ${benchmark}-vanilla
        move_dir ${benchmark}-vanilla
    fi

    if [ -d ${benchmark}-vanilla ];then
        test_dir ${benchmark}-native
        move_dir ${benchmark}-native
    fi
done

#!/bin/bash

declare -a suite=("bfs-2" "libcatena" "openssl" "pagerank-2" "pte-hashjoin" "pte-btree" "svm")
workload_type=$1

rm -rf ~/exp_${workload_type}
mkdir ~/exp_${workload_type}

for benchmark in "${suite[@]}";
do
    if [ "$benchmark" = "svm" ]; then
        cd ${benchmark}-vanilla
        sudo rm -rf evaluation
        make clean
        make WORKLOAD_TYPE=${workload_type}
        sudo -H -E ./run_local.sh 3 ${workload_type}
        # sudo -H -E ./run_local.sh 1 ${workload_type}
        cd ../
    else
        cd ${benchmark}-vanilla
        sudo rm -rf evaluation
        make clean
        make WORKLOAD_TYPE=${workload_type}
        sudo -H -E ./run_local.sh 3
        cd ../
        # sudo -H -E ./run_local.sh 1
        cd ${benchmark}-native
        sudo rm -rf evaluation
        make clean
        make WORKLOAD_TYPE=${workload_type}
        sudo -H -E ./run_local.sh 4
        cd ../
    fi
    # Copy the evaluation into a folder
    cp -rf ${benchmark}-*/evaluation/ ~/exp_${workload_type}
done
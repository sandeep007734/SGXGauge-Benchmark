#!/bin/bash

#!/bin/bash
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 
   exit 1
fi

if [ $# -ne 1 ]; then
    echo "Need a workload type. LOW_ | MEDIUM_ | HIGH_"
    exit 1
fi

declare -a suite=("bfs-2" "libcatena" "openssl" "pte-hashjoin" "pte-btree" "xsbench")
# declare -a suite=("xsbench")
workload_type=$1
user=$(who|awk '{print $1}')
make clean; 
make WORKLOAD_TYPE=${WORKLOAD_TYPE}
CURR_DIR=$(pwd)

if [ "$user" == "sandeep" ]; then
    rm -rf /home/sandeep/Downloads/exp_sgxgauge_${workload_type}
    mkdir /home/sandeep/Downloads/exp_sgxgauge_${workload_type}
else
    rm -rf ~/exp_${workload_type}
    mkdir ~/exp_${workload_type}
fi



for benchmark in "${suite[@]}";
do
    cd ${benchmark}-vanilla
    sudo rm -rf evaluation
    # Moving clean step to run_local.sh
    # make clean
    # make WORKLOAD_TYPE=${WORKLOAD_TYPE}
    sudo -H -E ./run_local.sh 3 ${workload_type}
    sudo -H -E ./run_local.sh 1 ${workload_type}
    cd ${CURR_DIR}

    if [ "$benchmark" != "svm" ] && [ "$benchmark" != "xsbench" ]; then
        cd ${benchmark}-native
        sudo rm -rf evaluation
        # Moving clean step to run_local.sh
        # make clean
        # make WORKLOAD_TYPE=${WORKLOAD_TYPE}
        sudo -H -E ./run_local.sh 4 ${workload_type}
        cd ${CURR_DIR}
    fi

    # Copy the evaluation into a folder
    if [ "$user" == "sandeep" ]; then
        cp -rf ${benchmark}-*/evaluation/ /home/sandeep/Downloads/exp_sgxgauge_${workload_type}
        sudo chown -R sandeep:sandeep /home/sandeep/Downloads/exp_sgxgauge_${workload_type}
    else
        cp -rf ${benchmark}-*/evaluation/ ~/exp_sgxgauge_${workload_type}
    fi
done
#!/bin/bash
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 
   exit 1
fi

declare -a suite=("blackscholes" "bodytrack" "canneal" "dedup" "fluidanimate" "freqmine" "streamcluster" "swaptions" "vips")
#declare -a suite=("canneal" "streamcluster")
for benchmark in "${suite[@]}";
do
    sudo -H -E ./run_local.sh 3 ${benchmark}
    #sudo -H -E ./run_local.sh 1 ${benchmark}
done
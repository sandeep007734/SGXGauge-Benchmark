#!/bin/bash

if [[ $EUID -ne 0 ]];then
    echo "Please run as root"
    exit
fi
for cnf in $(cat all_config)
do
    echo "ALLSTATS=T" > CONF.DAT
    echo "CUSTOMRUN=T" >> CONF.DAT
    echo DO${cnf}=T >> CONF.DAT
    cat GLOBSETTING >> CONF.DAT
    
    sudo -H -E ./run_local.sh 3 $cnf
done

# for cnf in $(cat all_config)
# do
#    echo "ALLSTATS=T" > CONF.DAT
#    echo "CUSTOMRUN=T" >> CONF.DAT
#    echo DO${cnf}=T >> CONF.DAT
#    cat GLOBSETTING >> CONF.DAT

#    sudo -H -E ./run_local.sh 1 $cnf
# done
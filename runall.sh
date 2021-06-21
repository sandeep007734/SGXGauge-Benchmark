benchmark=$1

for s in LOW_ MEDIUM_ HIGH_
do
    if [ "$benchmark" == "sgxgauge" ]; then
        sudo -H -E ./run_sgxgauge.sh $s
    else
        sudo -H -E ./run_lmbench.sh $s
done
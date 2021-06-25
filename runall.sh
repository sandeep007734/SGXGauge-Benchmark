
rm run_log
touch run_log
for i in $(seq 1 10)
do
    echo $i  >> run_log
    for s in LOW_ MEDIUM_ HIGH_
    do
        echo $s >> run_log
        sudo -H -E ./run_sgxgauge.sh $s
    done
done

poweroff
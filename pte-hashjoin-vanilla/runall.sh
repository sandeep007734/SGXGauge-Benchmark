for s in LOW_ MEDIUM_ HIGH_
do
    sudo -H -E ./run_local.sh 3 $s
done

for s in LOW_ MEDIUM_ HIGH_
do
    sudo -H -E ./run_local.sh 1 $s
done
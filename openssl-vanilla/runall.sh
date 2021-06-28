# for s in LOW_ MEDIUM_ HIGH_
# do
#     sudo -H -E ./run_local.sh 1 $s
# done

for i in $(seq 1 10)
do
    for s in LOW_ MEDIUM_ HIGH_
    do
        sudo -H -E ./run_local.sh 3 $s
    done
done
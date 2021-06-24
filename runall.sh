echo "DO NOT USE THIS. RUN SETTINGS INDIVIDUALLY AS THE EVAL FOLDERS ARE MOVED AFTER EACH SETTING RUN"
# benchmark=$1

# for s in LOW_ MEDIUM_ HIGH_
# do
#     if [ "$benchmark" == "sgxgauge" ]; then
#         sudo -H -E ./run_sgxgauge.sh $s
#     else
#         sudo -H -E ./run_lmbench.sh $s
# done
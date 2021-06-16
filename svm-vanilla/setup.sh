git clone https://github.com/cjlin1/libsvm.git
cd libsvm
wget https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/multiclass/aloi.bz2
bzip2 -d aloi.bz2 

python tools/subset.py aloi 4000 aloi.tr_low aloi.te
python tools/subset.py aloi 10000 aloi.tr_high aloi.te
python tools/subset.py aloi 6000 aloi.tr_medium aloi.te

mv aloi.tr_* ../
cd ../
rm -rf libsvm
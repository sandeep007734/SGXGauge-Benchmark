git clone https://github.com/cjlin1/libsvm.git
cd libsvm
wget https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/multiclass/aloi.bz2
bzip2 -d aloi.bz2 
make
python tools/subset.py aloi 80000 aloi.tr aloi.te
./svm-train aloi.tr
python tools/subset.py aloi.te 80000 aloi.te1 aloi.te2
mv aloi.tr.model aloi.te1 ../
cd ../
rm -rf libsvm
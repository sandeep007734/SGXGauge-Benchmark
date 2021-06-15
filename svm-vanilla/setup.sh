git clone https://github.com/cjlin1/libsvm.git
cd libsvm
wget https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/multiclass/usps.bz2
bzip2 -d usps.bz2
make
python tools/subset.py usps 5000 usps.tr usps.te
./svm-train usps.tr
mv usps.tr.model usps.te ../
rm -rf libsvm
git clone https://github.com/cjlin1/libsvm.git
cd libsvm
wget https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/multiclass/news20.bz2
bzip2 -d news20.bz2
make
python tools/subset.py news20 12000 news20.tr news20.te
./svm-train news20.tr
mv news20.tr.model news20.te ../
rm -rf libsvm
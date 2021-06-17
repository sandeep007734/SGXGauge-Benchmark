curr_dir=$(pwd)

mkdir -p /home/sandeep/Desktop/work/phd/plots_lib/PYLAB/benchmark_work/data/sgxgauge_high
mkdir -p /home/sandeep/Desktop/work/phd/plots_lib/PYLAB/benchmark_work/data/sgxgauge_medium
mkdir -p /home/sandeep/Desktop/work/phd/plots_lib/PYLAB/benchmark_work/data/sgxgauge_low

cd evaluation/sgxgauge_HIGH_
cp -r * /home/sandeep/Desktop/work/phd/plots_lib/PYLAB/benchmark_work/data/sgxgauge_high
cd $curr_dir

cd evaluation/sgxgauge_MEDIUM_
cp -r * /home/sandeep/Desktop/work/phd/plots_lib/PYLAB/benchmark_work/data/sgxgauge_medium
cd $curr_dir

cd evaluation/sgxgauge_LOW_
cp -r * /home/sandeep/Desktop/work/phd/plots_lib/PYLAB/benchmark_work/data/sgxgauge_low
cd $curr_dir
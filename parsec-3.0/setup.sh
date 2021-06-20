mkdir -p ./input
cd ./input

# Get the inputs from parsec server:
wget http://parsec.cs.princeton.edu/download/3.0/parsec-3.0-input-sim.tar.gz
wget http://parsec.cs.princeton.edu/download/3.0/parsec-3.0-input-native.tar.gz
wget http://parsec.cs.princeton.edu/download/3.0/parsec-3.0-core.tar.gz

#Untar the input tars:
tar -xzvf parsec-3.0-core.tar.gz
tar -xzvf parsec-3.0-input-sim.tar.gz
tar -xzvf parsec-3.0-input-native.tar.gz

#Move the necessary input files into current directory
cp ./parsec-3.0/pkgs/apps/blackscholes/inputs/input_native.tar blackscholes_input.tar
cp ./parsec-3.0/pkgs/apps/bodytrack/inputs/input_simlarge.tar bodytrack_input.tar
cp ./parsec-3.0/pkgs/apps/fluidanimate/inputs/input_simlarge.tar fluidanimate_input.tar
cp ./parsec-3.0/pkgs/apps/freqmine/inputs/input_simlarge.tar freqmine_input.tar
cp ./parsec-3.0/pkgs/apps/raytrace/inputs/input_simlarge.tar raytrace_input.tar
cp ./parsec-3.0/pkgs/apps/vips/inputs/input_simlarge.tar vips_input.tar
cp ./parsec-3.0/pkgs/kernels/canneal/inputs/input_simlarge.tar canneal_input.tar
cp ./parsec-3.0/pkgs/kernels/dedup/inputs/input_simlarge.tar dedup_input.tar

#Untar the files

for i in *.tar
do
    tar xf "$i"
done


# Remove the files if you have good bandwidth to download again. :P
# rm -rf ./parsec-3.0
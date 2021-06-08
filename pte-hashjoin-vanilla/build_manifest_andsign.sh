if [ $# -ne 1 ]
then
	echo "Need the name of the manifest file."
	exit 1
fi
name=$(echo "$1" | cut -f 1 -d '.')

GRAPHENE_HOME="/home/sandeep/Desktop/work/phd/SecureFS/graphene_master"
graphene-sgx-sign --key ${GRAPHENE_HOME}/Pal/src/host/Linux-SGX/signer/enclave-key.pem --manifest ${name}.manifest --output ${name}.manifest.sgx

graphene-sgx-get-token --output ${name}.token --sig ${name}.sig

# graphene-sgx niozone -i 0 -i 1 -i 2 -s 1000m -r 4m -f /home/sandeep/Desktop/work/phd/SecureFS/securefs_bench/iozone/iozone.tmp

# #GRAPHENE_HOME=/home/sandeep/Desktop/work/phd/securefs_work/graphene_work/oscar-graphene
# GRAPHENE_HOME=/home/sandeep/Desktop/work/phd/SecureFS/graphene
# $GRAPHENE_HOME/Pal/src/host/Linux-SGX/signer/pal-sgx-sign -libpal $GRAPHENE_HOME/Runtime/libpal-Linux-SGX.so -key $GRAPHENE_HOME/Pal/src/host/Linux-SGX/signer/enclave-key.pem 	-manifest $name.manifest -output $name.manifest.sgx 
# $GRAPHENE_HOME/Pal/src/host/Linux-SGX/signer/pal-sgx-get-token -output $name.token -sig $name.sig

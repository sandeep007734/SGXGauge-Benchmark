if [ $# -ne 1 ]
then
	echo "Need the name of the manifest file."
	exit 1
fi
name=$(echo "$1" | cut -f 1 -d '.')

GRAPHENE_HOME="/home/sandeep/Desktop/work/phd/SecureFS/graphene_master"
graphene-sgx-sign --key ${GRAPHENE_HOME}/Pal/src/host/Linux-SGX/signer/enclave-key.pem --manifest ${name}.manifest --output ${name}.manifest.sgx

graphene-sgx-get-token --output ${name}.token --sig ${name}.sig

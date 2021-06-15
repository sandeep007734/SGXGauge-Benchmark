graphene-sgx-sign --key /home/sandeep/Desktop/work/phd/SecureFS/graphene_master/Pal/src/host/Linux-SGX/signer/enclave-key.pem --manifest nbench.manifest --output nbench.manifest.sgx

graphene-sgx-get-token --output nbench.token --sig nbench.sig


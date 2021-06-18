

all:
	@echo "Not required. makefile"

memcached.manifest.sgx: memcached.manifest memcached
	./build_manifest_andsign.sh memcached

pmemcached.manifest.sgx: pmemcached.manifest memcached
	./build_manifest_andsign.sh pmemcached


clean:
	$(RM) *.token *.sig *.manifest.sgx


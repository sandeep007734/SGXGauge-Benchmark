all: xsbench.manifest.sgx pxsbench.manifest.sgx

xsbench.manifest.sgx : xsbench.manifest
	@echo "Building xsbench"
	./build_manifest_andsign.sh xsbench

pxsbench.manifest.sgx : pxsbench.manifest
	@echo "Building pxsbench"
	./build_manifest_andsign.sh pxsbench
clean:
	rm -f *.sig *.manifest.sgx

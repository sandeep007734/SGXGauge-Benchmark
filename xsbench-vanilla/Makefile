all:
	@echo "Building xsbench - NOT required here"

xsbench.manifest.sgx : xsbench.manifest ./openmp-threading/XSBench
	@echo "Building xsbench"
	./build_manifest_andsign.sh xsbench

# pxsbench.manifest.sgx : pxsbench.manifest
# 	@echo "Building pxsbench"
# 	./build_manifest_andsign.sh pxsbench
clean:
	rm -f *.sig *.manifest.sgx

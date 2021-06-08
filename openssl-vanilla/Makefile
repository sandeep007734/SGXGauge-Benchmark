NONPF ?= 1

FLAGS=
ifeq ($(NONPF), 1)
	# $(info BUilding in NON PF mode)
	FLAGS += -DNONPF=1
else
	# $(info BUilding in PF mode)
endif

all: encrypt

encrypt: encrypt.c
	@echo $(FLAGS)
	gcc encrypt.c -L/usr/lib -lssl -lcrypto -ldl -lpthread -o encrypt $(FLAGS)


openssl.manifest.sgx : openssl.manifest encrypt
	@echo "Building openssl"
	./build_manifest_andsign.sh openssl

popenssl.manifest.sgx : popenssl.manifest encrypt
	@echo "Building popenssl"
	./build_manifest_andsign.sh popenssl


clean:
	rm -f encrypt openssl.manifest.sgx popenssl.manifest.sgx
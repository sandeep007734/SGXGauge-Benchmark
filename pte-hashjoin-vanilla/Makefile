CC = gcc
CXX = g++

WORKLOAD_TYPE= MEDIUM_

# cflags used
CFLAGS=-O3 -march=native -D$(WORKLOAD_TYPE)
CXXFLAGS=-O3 -march=native -D$(WORKLOAD_TYPE)

# common libraries used
COMMON_LIBS=-lrt -ldl -lpthread -lm

# common main functions for multithreaded, single threaded and page-table dump
SRC_M=src/main.c src/hashjoin.c src/murmurhash.c
SRC_S=src/main.c src/hashjoin.c src/murmurhash.c
DEPS=src/config.h src/murmur3.h

# the targets
all : bin/bench_hashjoin_st 

###############################################################################
# HashJoin
###############################################################################

bin/bench_hashjoin_st: $(SRC_S) $(DEPS)
	$(CC) $(INCLUDES) $(CFLAGS) $(SRC_S) -o $@  $(COMMON_LIBS) -g

hashjoin.manifest.sgx : hashjoin.manifest bin/bench_hashjoin_st
	@echo "Building hashjoin"
	./build_manifest_andsign.sh hashjoin

# phashjoin.manifest.sgx : phashjoin.manifest bin/bench_hashjoin_st
# 	@echo "Building phashjoin"
# 	./build_manifest_andsign.sh phashjoin

clean:
	rm -rf bin/* hashjoin.manifest.sgx phashjoin.manifest.sgx

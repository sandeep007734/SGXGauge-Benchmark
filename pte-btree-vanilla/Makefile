CC = gcc
CXX = g++

WORKLOAD_TYPE= MEDIUM_


# cflags used
CFLAGS=-O3 -march=native -D$(WORKLOAD_TYPE)
CXXFLAGS=-O3 -march=native -D$(WORKLOAD_TYPE)

# common libraries used
COMMON_LIBS=-lrt -ldl -lpthread -lm

# common main functions for multithreaded, single threaded and page-table dump
SRC_M=src/main.c src/btree1.c
SRC_S=src/main.c src/btree1.c
DEPS=src/config.h

# the targets
all : bin/bench_btree_st btree.manifest.sgx pbtree.manifest.sgx

###############################################################################
# btree
###############################################################################

bin/bench_btree_st: $(SRC_S) $(DEPS)
	$(CC) $(INCLUDES) $(CFLAGS) $(SRC_S) -o $@  $(COMMON_LIBS)

btree.manifest.sgx : btree.manifest bin/bench_btree_st
	@echo "Building btree"
	./build_manifest_andsign.sh btree

# pbtree.manifest.sgx : pbtree.manifest bin/bench_btree_st
# 	@echo "Building pbtree"
# 	./build_manifest_andsign.sh pbtree

clean:
	rm -rf bin/*

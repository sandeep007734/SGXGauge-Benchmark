CC = gcc
CXX = g++

WORKLOAD_TYPE = MEDIUM_

# cflags used
CFLAGS=-O3 -march=native -D$(WORKLOAD_TYPE)
CXXFLAGS=-O3 -march=native -D$(WORKLOAD_TYPE)

# common libraries used
COMMON_LIBS=-lrt -ldl -lpthread -lm

SRC_S=src/bfs_mod.cpp src/common.cpp src/common_rand.cpp
SRC_G=src/graph_gen.cpp src/common.cpp src/common_rand.cpp

DEPS=src/config.h

# the targets
all : bin/bfs bin/graph_gen

###############################################################################
# bfs
###############################################################################

bin/bfs: $(SRC_S) $(DEPS)
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(SRC_S) -o $@  $(COMMON_LIBS)

bin/graph_gen: $(SRC_G) $(DEPS)
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(SRC_G) -o $@  $(COMMON_LIBS)

bfs.manifest.sgx : bfs.manifest bin/bfs vertex_medium.dat vertex_high.dat vertex_low.dat
	@echo "Building bfs"
	./build_manifest_andsign.sh bfs
	
clean:
	rm -rf bin/*

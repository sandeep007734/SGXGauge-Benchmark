CC = gcc
CXX = g++

# cflags used
CFLAGS=-O3 -march=native
CXXFLAGS=-O3 -march=native

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

clean:
	rm -rf bin/*

###############################################################################
# Makefile to build the HashJoin workload
#
# Paper: Mitosis - Mitosis: Transparently Self-Replicating Page-Tables
#                  for Large-Memory Machines
# Authors: Reto Achermann, Jayneel Gandhi, Timothy Roscoe,
#          Abhishek Bhattacharjee, and Ashish Panwar
###############################################################################

CC = gcc
CXX = g++

# cflags used
CFLAGS=-O3 -march=native
CXXFLAGS=-O3 -march=native

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

# bin/bench_hashjoin_mt:  $(SRC_M) $(DEPS)
	# $(CC) $(INCLUDES) $(CFLAGS) -fopenmp $(SRC_M) -o $@  $(COMMON_LIBS)

###############################################################################
# Clean
###############################################################################

clean:
	rm -rf bin/*

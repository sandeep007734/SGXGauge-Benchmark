CC = gcc
CXX = g++

# cflags used
CFLAGS=-O3 -march=native
CXXFLAGS=-O3 -march=native

# common libraries used
COMMON_LIBS=-lrt -ldl -lpthread -lm

SRC_S=src/page_mod.c src/common.c src/common_rand.c
SRC_G=src/page_gen.c src/common.c src/common_rand.c

DEPS=src/config.h

# the targets
all : bin/bfs bin/graph_gen

###############################################################################
# bfs
###############################################################################

bin/pagerank: $(SRC_S) $(DEPS)
	$(CC) $(INCLUDES) $(CFLAGS) $(SRC_S) -o $@  $(COMMON_LIBS)

bin/page_gen: $(SRC_G) $(DEPS)
	$(CC) $(INCLUDES) $(CFLAGS) $(SRC_G) -o $@  $(COMMON_LIBS)

clean:
	rm -rf bin/*

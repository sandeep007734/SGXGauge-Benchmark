CC=g++
WORKLOAD_TYPE = MEDIUM_
CXXFLAGS=-Iinclude -Isrc -Ithird-party -std=c++1y -D${WORKLOAD_TYPE}
LDFLAGS=-lpthread
PROJNAME=catena
EXENAME=bin/$(PROJNAME)
TPSRCS=third-party/zedwood/sha256.cpp
LIBSRCS=src/catena/Commons.cpp \
src/catena/Hash.cpp \
src/catena/NonceFinder.cpp \
src/catena/Block.cpp \
src/catena/BlockChain.cpp
SRCS=src/Main.cpp $(TPSRCS) $(LIBSRCS)

all: build

build:
	$(CC) -o $(EXENAME) $(SRCS) $(CXXFLAGS) $(LDFLAGS)

catena.manifest.sgx : catena.manifest
	@echo "Building catena"
	./build_manifest_andsign.sh catena

# pcatena.manifest.sgx : pcatena.manifest
# 	@echo "Building pcatena"
# 	./build_manifest_andsign.sh pcatena

clean:
	rm -rf $(EXENAME) *.sig *.manifest.sgx

rebuild: clean build



##################################
# Aisha Hasan #

# Makefile for Emtpy Enclave - Coding with SGX
##################################


SHELL=/bin/sh

CC=`./scripts/compiler`
MAKE=`./scripts/make`
AR=ar
ARCREATE=cr

# base of installation location
O= ./bin/unknown
OS=`./scripts/os`
BINDIR=./bin/$(OS)
UTILS=./scripts/os \
		./scripts/compiler \
		./scripts/make \


COMPILE=$(CC) $(CFLAGS)



ENC_INCS =	Enclave/bench.h Enclave/timing.h App/EnclaveBridge.h ##### HERE #######
INCS =	App/bench_app.h App/EnclaveBridge.h ########################## HERE #######

SRCS =  getopt.c lib_timing.c bench.h timing.h ####################### HERE ########


OPT_EXES=$O/lat_syscall

LIBOBJS= $O/Enclave_lib_timing.o $O/Enclave_lat_syscall.o $O/getopt.o $O/EnclaveBridge.o #start here ######### HERE ##############

LAT_SYSCALL= $O/Enclave_lib_timing.o $O/Enclave_lat_syscall.o $O/getopt.o $O/EnclaveBridge.o #start here ##### HERE ##############

CFLAGS=-O -DRUSAGE -DHAVE_uint=1 -DHAVE_int64_t=1 -DHAVE_DRAND48 -DHAVE_SCHED_SETAFFINITY=1
LDLIBS=-lm
${O}=${BINDIR}



lmbench: $(UTILS)
	$(info "building this first")
	if [ ! -d ${BINDIR} ]; then mkdir -p ${BINDIR}; fi
	${MAKE} OS="${OS}" CC="${CC}" CFLAGS="${CFLAGS}" LDLIBS="${LDLIBS}" O="${BINDIR}" opt


opt: $(OPT_EXES)


clean:
	/bin/rm -f ./bin/*/CONFIG ./bin/*/*.[oas]
	/bin/rm -f *.[oas]
	/bin/rm -f ./bin/*/lat_syscall

.PHONY: lmbench all clean 



$O/lmbench.a: $(LAT_SYSCALL)
	/bin/rm -f $O/lmbench.a
	$(AR) $(ARCREATE) $O/lmbench.a $(LAT_SYSCALL)
	-ranlib $O/lmbench.a


#$O/lat_syscall.a: $(LIBOBJS)
#	/bin/rm -f $O/lat_syscall.a
#	$(AR) $(ARCREATE) $O/lat_syscall.a $(LIBOBJS)
#	-ranlib $O/lat_syscall.a


$O/Enclave_lib_timing.o: Enclave/Enclave_lib_timing.c $(ENC_INCS)
	$(COMPILE) -c Enclave/Enclave_lib_timing.c -o $O/Enclave_lib_timing.o
$O/Enclave_lat_syscall.o: Enclave/Enclave_lat_syscall.c $(ENC_INCS)
	$(COMPILE) -c Enclave/Enclave_lat_syscall.c -o $O/Enclave_lat_syscall.o
$O/getopt.o: App/getopt.c $(INCS)
	$(COMPILE) -c App/getopt.c -o $O/getopt.o
$O/EnclaveBridge.o: App/EnclaveBridge.c $(INCS)
	$(COMPILE) -c App/EnclaveBridge.c -o $O/EnclaveBridge.o

#$O/lat_rand:  App/lat_rand.c App/EnclaveBridge.h App/bench_app.h $O/lmbench.a
#	$(COMPILE) -o $O/lat_rand App/lat_rand.c $O/lmbench.a $(LDLIBS)

$O/lat_syscall: App/lat_syscall.c App/EnclaveBridge.h App/bench_app.h $O/lmbench.a
	$(COMPILE) -o $O/lat_syscall App/lat_syscall.c $O/lmbench.a $(LDLIBS)

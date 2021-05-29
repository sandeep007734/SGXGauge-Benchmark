#
# Copyright (C) 2011-2020 Intel Corporation. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#   * Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in
#     the documentation and/or other materials provided with the
#     distribution.
#   * Neither the name of Intel Corporation nor the names of its
#     contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#



######## SGX SDK Settings ########
SGX_MODE ?= HW
SGX_ARCH ?= x64
UNTRUSTED_DIR=App

include $(SGX_SDK)/buildenv.mk

ifeq ($(shell getconf LONG_BIT), 32)
	SGX_ARCH := x86
else ifeq ($(findstring -m32, $(CXXFLAGS)), -m32)
	SGX_ARCH := x86
endif

ifeq ($(SGX_ARCH), x86)
	$(error x86 build is not supported, only x64!!)
else
	SGX_COMMON_CFLAGS := -m64 -Wall
	SGX_LIBRARY_PATH := $(SGX_SDK)/lib64
	SGX_EDGER8R := $(SGX_SDK)/bin/x64/sgx_edger8r
	SGX_SDK_INC := $(SGX_SDK)/include
endif

ifeq ($(DEBUG), 1)
ifeq ($(SGX_PRERELEASE), 1)
$(error Cannot set DEBUG and SGX_PRERELEASE at the same time!!)
endif
endif

OPENSSL_LIBRARY_PATH := $(PACKAGE_LIB)
ifeq ($(DEBUG), 1)
        SGX_COMMON_FLAGS += -O0 -g
		SgxSSL_Link_Libraries := sgx_usgxssld
else
        SGX_COMMON_FLAGS += -O2 -D_FORTIFY_SOURCE=2
		SgxSSL_Link_Libraries := sgx_usgxssl
endif

SGX_COMMON_FLAGS += -Wall -Wextra -Winit-self -Wpointer-arith -Wreturn-type \
                    -Waddress -Wsequence-point -Wformat-security \
                    -Wmissing-include-dirs -Wfloat-equal -Wundef -Wshadow \
                    -Wcast-align -Wcast-qual -Wconversion -Wredundant-decls
SGX_COMMON_CFLAGS := $(SGX_COMMON_FLAGS) -Wjump-misses-init -Wstrict-prototypes -Wunsuffixed-float-constants
SGX_COMMON_CXXFLAGS := $(SGX_COMMON_FLAGS) -Wnon-virtual-dtor -std=c++11

######## App Settings ########

ifneq ($(SGX_MODE), HW)
	Urts_Library_Name := sgx_urts_sim
	UaeService_Library_Name := sgx_uae_service_sim
else
	Urts_Library_Name := sgx_urts
	UaeService_Library_Name := sgx_uae_service
endif

App_Cpp_Files := $(UNTRUSTED_DIR)/App.cpp
App_Cpp_Objects := $(App_Cpp_Files:.cpp=.o)

App_Include_Paths := -I$(UNTRUSTED_DIR) -I$(SGX_SDK_INC)

App_C_Flags := $(SGX_COMMON_CFLAGS) -fpic -fpie -fstack-protector -Wformat -Wno-attributes $(App_Include_Paths)
App_Cpp_Flags := $(App_C_Flags) -std=c++11


Security_Link_Flags := -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -pie

App_Link_Flags := $(SGX_COMMON_CFLAGS) $(Security_Link_Flags) $(SGX_SHARED_LIB_FLAG) -L$(SGX_LIBRARY_PATH) -l$(Urts_Library_Name) -l$(UaeService_Library_Name) -L$(OPENSSL_LIBRARY_PATH) -l$(SgxSSL_Link_Libraries) -lpthread 


.PHONY: all test

all: app

test: all
	@$(CURDIR)/TestApp
	@echo "RUN  =>  TestApp [$(SGX_MODE)|$(SGX_ARCH), OK]"

######## App Objects ########
$(UNTRUSTED_DIR)/Enclave_u.h: $(SGX_EDGER8R) Enclave/Enclave.edl
	@cd $(UNTRUSTED_DIR) && $(SGX_EDGER8R) --untrusted ../Enclave/Enclave.edl --search-path ../Enclave --search-path $(PACKAGE_INC) --search-path $(SGX_SDK_INC)
	@echo "GEN  =>  $@"

App/Enclave_u.c: App/Enclave_u.h

$(UNTRUSTED_DIR)/Enclave_u.o: $(UNTRUSTED_DIR)/Enclave_u.c
	$(VCC) $(SGX_COMMON_CFLAGS) $(App_C_Flags) -c $< -o $@
	@echo "CC   <=  $<"

$(UNTRUSTED_DIR)/%.o: $(UNTRUSTED_DIR)/%.cpp $(UNTRUSTED_DIR)/Enclave_u.h
	$(VCXX) $(SGX_COMMON_CXXFLAGS) $(App_Cpp_Flags) -c $< -o $@
	@echo "CXX  <=  $<"

app: $(UNTRUSTED_DIR)/Enclave_u.o $(App_Cpp_Objects)
	$(VCXX) $^ -o $@ $(App_Link_Flags)
	@echo "LINK =>  $@"




.PHONY: clean

clean:
	@rm -f App  $(App_Cpp_Objects) $(UNTRUSTED_DIR)/Enclave_u.*

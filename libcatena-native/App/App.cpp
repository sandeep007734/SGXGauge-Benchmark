/*
 * Copyright (C) 2011-2020 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include <assert.h>

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <inttypes.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>     /* open */
#include <sys/ioctl.h> /* ioctl */
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>

#include "include/Commons.hpp"
#include "include/Hash.hpp"
#include "include/NonceFinder.hpp"
#include "include/Block.hpp"
#include "include/BlockChain.hpp"
#include "include/Clock.hpp"

#include <iostream>
#include <fstream>

# include <unistd.h>
# include <pwd.h>
# define MAX_PATH FILENAME_MAX

#include "sgx_urts.h"
#include "App.h"
#include "Enclave_u.h"

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

typedef struct _sgx_errlist_t {
    sgx_status_t err;
    const char *msg;
    const char *sug; /* Suggestion */
} sgx_errlist_t;

/* Error code returned by sgx_create_enclave */
static sgx_errlist_t sgx_errlist[] = {
    {
        SGX_ERROR_UNEXPECTED,
        "Unexpected error occurred.",
        NULL
    },
    {
        SGX_ERROR_INVALID_PARAMETER,
        "Invalid parameter.",
        NULL
    },
    {
        SGX_ERROR_OUT_OF_MEMORY,
        "Out of memory.",
        NULL
    },
    {
        SGX_ERROR_ENCLAVE_LOST,
        "Power transition occurred.",
        "Please refer to the sample \"PowerTransition\" for details."
    },
    {
        SGX_ERROR_INVALID_ENCLAVE,
        "Invalid enclave image.",
        NULL
    },
    {
        SGX_ERROR_INVALID_ENCLAVE_ID,
        "Invalid enclave identification.",
        NULL
    },
    {
        SGX_ERROR_INVALID_SIGNATURE,
        "Invalid enclave signature.",
        NULL
    },
    {
        SGX_ERROR_OUT_OF_EPC,
        "Out of EPC memory.",
        NULL
    },
    {
        SGX_ERROR_NO_DEVICE,
        "Invalid SGX device.",
        "Please make sure SGX module is enabled in the BIOS, and install SGX driver afterwards."
    },
    {
        SGX_ERROR_MEMORY_MAP_CONFLICT,
        "Memory map conflicted.",
        NULL
    },
    {
        SGX_ERROR_INVALID_METADATA,
        "Invalid enclave metadata.",
        NULL
    },
    {
        SGX_ERROR_DEVICE_BUSY,
        "SGX device was busy.",
        NULL
    },
    {
        SGX_ERROR_INVALID_VERSION,
        "Enclave version was invalid.",
        NULL
    },
    {
        SGX_ERROR_INVALID_ATTRIBUTE,
        "Enclave was not authorized.",
        NULL
    },
    {
        SGX_ERROR_ENCLAVE_FILE_ACCESS,
        "Can't open enclave file.",
        NULL
    },
};

/* Check error conditions for loading enclave */
void print_error_message(sgx_status_t ret)
{
    size_t idx = 0;
    size_t ttl = sizeof sgx_errlist/sizeof sgx_errlist[0];

    for (idx = 0; idx < ttl; idx++) {
        if(ret == sgx_errlist[idx].err) {
            if(NULL != sgx_errlist[idx].sug)
                printf("Info: %s\n", sgx_errlist[idx].sug);
            printf("Error: %s\n", sgx_errlist[idx].msg);
            break;
        }
    }
    
    if (idx == ttl)
    	printf("Error code is 0x%X. Please refer to the \"Intel SGX SDK Developer Reference\" for more details.\n", ret);
}

/* Initialize the enclave:
 *   Call sgx_create_enclave to initialize an enclave instance
 */
int initialize_enclave(void)
{
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    
    /* Call sgx_create_enclave to initialize an enclave instance */
    /* Debug Support: set 2nd parameter to 1 */
    ret = sgx_create_enclave(ENCLAVE_FILENAME, SGX_DEBUG_FLAG, NULL, NULL, &global_eid, NULL);
    if (ret != SGX_SUCCESS) {
        print_error_message(ret);
        return -1;
    }

    return 0;
}

/* OCall functions */
void ocall_print_string(const char *str)
{
    /* Proxy/Bridge will check the length and null-terminate 
     * the input string to prevent buffer overflow. 
     */
    printf("%s", str);
}

namespace catena {
class Main {
public:
  static void testNonceFinder() {

      std::string input = Hash::sha256("LibCatena by Saurav Mohapatra (mohaps@gmail.com)");
      size_t difficulty = 2;
      std::cout << "Trying to find nonce ..." << std::endl;
      std::cout << "  Input => \""<<input<<"\"" << std::endl;
      std::cout << "  Difficulty => "<< difficulty << std::endl;
      NonceFinder finder(input, difficulty);
      auto result = finder.find();
      if (result.first) {
        std::cout << "!!! Found Result hash = " << result.second.first << " nonce = " << result.second.second << std::endl;
      } else {
        throw std::runtime_error("could not find a valid nonce!");
      }
  }

  static void testMerkle() {
    std::vector<std::string> inputs {
      "Block 1",
      "Block 2",
      "Block 3",
      "Block 4",
      "Block 5"
    };
    // round 1
    std::string h_0_1 = Hash::merkle2(inputs[0], inputs[1]);
    std::string h_2_3 = Hash::merkle2(inputs[2], inputs[3]); 
    std::string h_4 = Hash::merkle1(inputs[4]);
    std::string h_0_3 = Hash::merkle2(h_0_1, h_2_3);

    std::string hRoot = Hash::merkle2(h_0_3, Hash::merkle1(h_4));
    std::cout << "Merkle Root (Unrolled) => \""<<hRoot<<"\"" << std::endl;
    std::string merkleRoot = Hash::merkle(inputs);
    std::cout << "Merkle Root: " << merkleRoot << "\"" << std::endl;
    if (hRoot == merkleRoot) {
      std::cout << "**** merkle root computation is valid!" << std::endl;
    } else {
      throw std::runtime_error("merkle root computation is invalid!");
    }  
  }

  #define DIFFC 5

  #ifdef HIGH_
    #define LEN 8
  #elif MEDIUM_
    #define LEN 5
  #else
    #define LEN 3
  #endif
  
  static BlockChain testBlockChain() 
  {
    BlockChain blockChain("Catena Test", DIFFC);
    blockChain.dump(std::cout);

    Scope scope("Catena_Basic_Blockchain");
    
    for (auto i = 1; i < LEN; i++) {
      std::ostringstream os;
      os << "Block #" << i;
      std::string blockData = os.str();

      std::cout << "... Mining for block #" << i << "..." << std::endl;
      Scope scope(blockData, "Blockchain", 2);
      auto b = blockChain.newBlock({blockData}); 
      std::cout << "*** Block #"<<i<<" added to blockchain!" << std::endl;
      b->dump(std::cout);
      std::cout << "!---------------------------------------------------------------!" << std::endl;
    }
    std::cout << "... final blockchain ..." << std::endl;
    blockChain.dump(std::cout);
    return blockChain;
  }

  static void run(int argc, char** argv) {
    BlockChain blockchain = testBlockChain();
  }

};
}

int SGX_CDECL main(int argc, char *argv[])
{

    int result;

    /* Initialize the enclave */
    if(initialize_enclave() < 0){
        printf("Info: Enclave raised unhandled exception.\n");
        return -1; 
    }

    try 
    {
        struct timeval stop, start;
        gettimeofday(&start, NULL);

        // Main call
        catena::Main::run(argc, argv);
        
        gettimeofday(&stop, NULL);
        printf("SECUREFS_TIME %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); 
    } 
    catch (std::exception& ex) {
        std::cerr << "[error] " << ex.what() << std::endl;
        return -1;
    }
  return 0;
}
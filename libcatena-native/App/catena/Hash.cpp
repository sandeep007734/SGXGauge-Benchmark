/*
  LibCatena - a simple blockchain implementation in C++
  https://github.com/mohaps/libcatena
  
  Copyright 2018 Saurav Mohapatra. 

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/
#include "include/Hash.hpp"
#include "Enclave_u.h"
#include "App.h"
#include <stdexcept>
#include <cstdlib>
#include <cstring>
namespace catena {
const std::string Hash::kBlankHash = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";
const std::string Hash::kAllZeroesHash(64, '0');
std::string Hash::sha256(const std::string& input) {
  if (input.empty()) { return kBlankHash; }
  unsigned char *output = (unsigned char *)malloc(sizeof(unsigned char) * 33);
  memset( output, '0', sizeof(unsigned char)*33);
  output[32] = '\0';

  ecall_sha256(global_eid, (unsigned char*) input.c_str(), output, input.length());

  char buf[65];
  buf[65] = 0;
  for (int i = 0; i < 32; i++)
      sprintf(buf+i*2, "%02x", output[i]);
  // printf("The hash value is: %s \n", buf);
  free(output);

  return std::string(buf);
}

std::string Hash::merkle1(const std::string& s1) {
  return sha256(s1);
}

std::string Hash::merkle2(const std::string& s1, const std::string& s2) {
  return sha256(sha256(s1) + sha256(s2));
}

bool Hash::satisfiesDifficulty(const std::string& hashString, size_t difficulty) {
  if (hashString.size() != 64) {
    std::cout<<"hash is: "<< hashString << "  ::: size is" << hashString.size() << std::endl;
    throw std::invalid_argument("hash value provided is not a valid hash");
  }
  if (difficulty == 0) { return true; }
  size_t n = difficulty;
  if (n > 64) {
    n = 64;
  }
  return ::memcmp(Hash::kAllZeroesHash.c_str(), hashString.c_str(), n) == 0;
}

std::string Hash::merkle(const std::vector<std::string>& data) {
  if (data.empty()) { return kBlankHash; }
  if (data.size() == 1) { return merkle1(data.front()); }
  if (data.size() == 2) { return merkle2(data[0], data[1]); }
  std::vector<std::string> output;
  merkleRange(std::inserter(output, output.end()), data.begin(), data.end());
  if (output.size() == 1) { return output[0]; }
  return merkle(output);
}
}
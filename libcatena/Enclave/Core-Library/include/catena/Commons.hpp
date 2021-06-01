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
#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <limits>
#include <iostream>
#include <sstream>
namespace catena {
class Block;
class BlockChain;
struct Types {
  using BlockIndex = size_t;
  using BlockNonce = uint32_t;
  using BlockData = std::vector<std::string>;
  static constexpr BlockNonce kMaxNonceValue = std::numeric_limits<BlockNonce>::max() - 1;
  static const std::string kMagic;
};
}
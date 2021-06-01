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
#include "catena/Block.hpp"

namespace catena {
class BlockChain {
public:
  using Blocks = std::vector<Block::Ptr>;
  BlockChain(const std::string& name, size_t difficulty);
  ~BlockChain() = default;
  Block::Ptr newBlock(const Block::Data& data);
  Block::Ptr getLastBlock() { return blocks_.back(); }
  //std::ostream& dump(std::ostream& os) const;
private:
  std::string name_;
  size_t difficulty_;
  Blocks blocks_;
};
}
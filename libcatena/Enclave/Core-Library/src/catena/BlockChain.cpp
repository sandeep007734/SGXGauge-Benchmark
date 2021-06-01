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
#include "catena/BlockChain.hpp"

namespace catena {

BlockChain::BlockChain(const std::string& name, size_t difficulty) : name_(name), difficulty_(difficulty) {
  Block::Ptr block = Block::newBlock(difficulty_, 0, Hash::kAllZeroesHash, {"Genesis_Block", name_});
  blocks_.push_back(block);
}
Block::Ptr BlockChain::newBlock(const Block::Data& data) {
  // TODO: lock this for thread safety
  Block::Ptr block = Block::newBlock(difficulty_, blocks_.size(), getLastBlock()->getHash(), data);
  blocks_.push_back(block);
  return block;
}

/*std::ostream& BlockChain::dump(std::ostream& os) const {
  os << std::endl;
  os << "BlockChain : [" << name_ << "] (difficulty=" << difficulty_ << ")" << std::endl;
  os << "Block Count => " << blocks_.size() << std::endl;
  for (const auto& b : blocks_) {
    b->dump(os);
  }
  return os << std::endl;
}*/  
}
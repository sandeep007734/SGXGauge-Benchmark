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
#include "include/Commons.hpp"
#include "include/Hash.hpp"
#include "include/NonceFinder.hpp"
#include <memory>
namespace catena {
class Block {
  friend class BlockChain;
public:
  using Ptr = std::shared_ptr<Block>;
  using Index = Types::BlockIndex;
  using Nonce = Types::BlockNonce;
  using Data = Types::BlockData;
  ~Block() = default;
  void verify() const;
  std::ostream& dump(std::ostream& os) const;
  void dump_file(int fd) const;
  std::string dumps() const;
  std::string getRawSignaturePrefix() const;
  const Index& getIndex() const { return index_; }
  const std::string& getPreviousHash() const { return previousHash_; }
  const std::string& getHash() const { return hash_; }
  const Nonce& getNonce() const { return nonce_; }
  const Data& getData() const { return data_; }
  Data::const_iterator beginData() const { return data_.begin(); }
  Data::const_iterator endData() const { return data_.end(); }
protected:
  Block(const Index& index, const std::string& previousHash, const std::string& currentHash, const Nonce& nonce, const Data& data);
  static Ptr newBlock(size_t difficulty, const Index& index, const std::string& previousHash, const Data& data);
private:
  Index index_;
  std::string previousHash_, hash_;
  Nonce nonce_;
  Data data_;
};
}
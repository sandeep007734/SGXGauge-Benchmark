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
#include "include/Block.hpp"
#include <sstream>
#include <functional>
#include <algorithm>
namespace catena {
Block::Ptr Block::newBlock(size_t difficulty, const Block::Index& index, const std::string& previousHash, const Block::Data& data) {
  std::ostringstream os;
  os << Types::kMagic << index << previousHash << Hash::merkle(data);
  std::string input = Hash::sha256(os.str());
  NonceFinder finder(input, difficulty);
  auto result = finder.find();
  if (!result.first) {
    throw std::runtime_error("failed to find nonce at difficulty level for a new block");
  }
  auto currentHash = result.second.first;
  auto nonce = result.second.second;
  return Block::Ptr(new Block(index, previousHash, currentHash, nonce, data));
}
Block::Block(const Block::Index& index, 
  const std::string& previousHash, 
  const std::string& currentHash, 
  const Block::Nonce& nonce, 
  const Block::Data& data) : index_(index), previousHash_(previousHash), hash_(currentHash), nonce_(nonce), data_(data) {

}
std::string Block::getRawSignaturePrefix() const {
  std::ostringstream os;
  os << Types::kMagic << index_ << previousHash_ << Hash::merkle(data_);
  return os.str();
}
void Block::verify() const {
  std::ostringstream os;
  os << Hash::sha256(getRawSignaturePrefix()) << getNonce();
  std::string computedHash = Hash::sha256(os.str());
  os.str("");
  os.clear();
  if (computedHash != hash_) {
    os << "computed hash \""<<computedHash<<"\" != \""<<hash_<<"\"";
    throw std::invalid_argument(os.str());
  }
}

std::ostream& Block::dump(std::ostream& os) const {
  std::cout << std::endl;
  std::cout << "  Block #" << index_ << " {" << std::endl;
  std::cout << "    previous_hash => " << previousHash_ << std::endl;
  std::cout << "    current_hash  => " << hash_ << std::endl;
  std::cout << "    nonce => " << nonce_ << std::endl;
  std::cout << "    data (" << data_.size() << " entries) => [" << std::endl;
  std::for_each(data_.begin(), data_.end(), [&os](const auto& s) {
    std::cout << "      \"" << s << "\"," << std::endl;
  });
  std::cout << "    ]" << std::endl;
  std::cout << "  }" << std::endl;
  return std::cout << std::endl;
}

// void Block::dump_file(int fd) const {
//   char msg[200];
//   sprintf(msg,"  Block # %ld\n",index_);
//   std::cout<<fd<<" "<<msg;
//   if(!write(fd,msg,sizeof(msg)))
//     perror("write");

//   sprintf(msg,"  previous_hash # %s\n",previousHash_);
//   std::cout<<msg<<std::endl;
//   if(!write(fd,msg,sizeof(msg)))
//     perror("write");

//   sprintf(msg,"  current_hash # %s\n",hash_);
//   std::cout<<msg<<std::endl;
//   if(!write(fd,msg,sizeof(msg)))
//     perror("write");

//   // sprintf(msg,"  nonce_ # %s\n",nonce_);
//   // std::cout<<msg<<std::endl;
//   // write(fd,msg,sizeof(msg));

//   sprintf(msg,"  data # %d\n",data_.size());
//   std::cout<<msg<<std::endl;
//   if(!write(fd,msg,sizeof(msg)))
//     perror("write")

//   // os << "    ]" << std::endl;
//   // os << "  }" << std::endl;
// }


std::string Block::dumps() const {
  std::ostringstream os;
  dump(os);
  return os.str();
}
}
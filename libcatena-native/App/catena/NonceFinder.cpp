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
#include "include/NonceFinder.hpp"
#include <stdexcept>
#include <functional>
#include <iterator>

namespace catena {

NonceFinder::NonceFinder(const std::string& input, size_t difficulty) : input_(input), difficulty_(difficulty) {
  if (difficulty_ > 64) {
    difficulty = 64;
  }
  running_.store(false);
  cancelled_.store(false);
  resultFound_.store(false);
}
NonceFinder::~NonceFinder() {
  for (auto& w : workers_) {
    w->join();
  }
}
std::pair<bool, NonceFinder::Result> NonceFinder::find() {
  if (running_) { throw std::runtime_error("nonce finder is already running"); }
  size_t workerCount = std::thread::hardware_concurrency();
  // clock_.reset();
  running_.store(true);
  cancelled_.store(false);
  resultFound_.store(false);
  size_t progressInterval = 7 * 1000 * 1000;
  for (size_t idx = 0; idx < workerCount; idx++) {
    auto workerFunc = [&] (size_t i) {
      Types::BlockNonce nonce = i;
      Types::BlockNonce maxNonce = Types::kMaxNonceValue - i;
      std::ostringstream os;
      size_t loopCounter = 0;
      while (nonce <= maxNonce && !resultFound_ && !cancelled_) {
        ++loopCounter;
        os << input_ << nonce;
        std::string h = Hash::sha256(os.str());
        if (Hash::satisfiesDifficulty(h, difficulty_)) {
          if (resultFound_) { break; }
          // ------------------------------------------------------------------------
          // TODO: we should lock this block with a mutex to avoid conflicts, when
          // we find multiple simultenous results in two or more workers.
          // ------------------------------------------------------------------------
          result_.first = h;
          result_.second = nonce;
          resultFound_.store(true);
          break;
        }
        os.str("");
        os.clear();
        if (loopCounter % progressInterval == 0) {
          // std::cout << "[PROGRESS] [Worker #"<<i<<"] => (loops=" << loopCounter 
          //   << ") (nonce="<<nonce<<") (elapsed = " << clock_.elapsedSeconds() << " seconds) ... " << std::endl;
            std::cout << "[PROGRESS] [Worker #"<<i<<"] => (loops=" << loopCounter 
            << ") (nonce="<<nonce<<") ... " << std::endl;
        }
        nonce += workerCount;
      }
    };
    workers_.push_back(std::make_shared<std::thread>(std::bind(workerFunc, idx)));
  }
  for (auto& w : workers_) {
    w->join();
  }
  workers_.clear();
  running_ = false;
  return { static_cast<bool>(resultFound_), static_cast<Result>(result_) };
}
}
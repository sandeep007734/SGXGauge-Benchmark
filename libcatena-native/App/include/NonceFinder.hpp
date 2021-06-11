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
#include "include/Clock.hpp"
#include "include/Hash.hpp"
#include <thread>
#include <memory>
#include <atomic>

namespace catena {
class NonceFinder {
public:
  using Result = std::pair<std::string, Types::BlockNonce>;
  using ThreadPtr = std::shared_ptr<std::thread>;
  NonceFinder(const std::string& input, size_t difficulty);
  ~NonceFinder();
  std::pair<bool, Result> find();
  //SystemClock& getClock() { return clock_; }
private:
  std::string input_;
  size_t difficulty_;
  std::atomic<bool> running_;
  std::atomic<bool> resultFound_;
  std::atomic<bool> cancelled_;
  Result result_;
  std::vector<ThreadPtr> workers_;
  //SystemClock clock_;
};

}
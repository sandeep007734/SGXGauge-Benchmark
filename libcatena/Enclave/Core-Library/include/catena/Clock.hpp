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
#include "catena/Commons.hpp"
#include <ctime>
#include <chrono>

using namespace std::chrono;

namespace catena {

template <typename T>
class Clock {
public:
  using TimePoint = chrono::time_point<T>;
  static TimePoint timeSinceEpoch() { return T::time_since_epoch(); }
  static uint64_t currentTimeMicros() { 
    return chrono::duration_cast<chrono::microseconds>(timeSinceEpoch()).count();
  }
  static uint64_t currentTimeMillis() { 
    return currentTimeMicros() / 1000;
  }
  static TimePoint now() { return T::now(); }
  Clock() { reset(); }
  Clock& reset() { start_ = mark_ = now(); return *this; }
  Clock& mark() { mark_ = now(); return *this; }
  auto elapsed() const { return now() - start_; }
  auto marked() const { return now() - mark_; }
  uint64_t elapsedMicros() const {
    return chrono::duration_cast<chrono::microseconds>(elapsed()).count();
  }
  uint64_t markedMicros() const {
    return chrono::duration_cast<chrono::microseconds>(marked()).count();
  }
  uint64_t elapsedMillis() const {
    return elapsedMicros() / 1000;
  }
  uint64_t markedMillis() const {
    return markedMicros() / 1000; 
  }
  double elapsedSeconds() const {
    return 0.001 * 0.001 * static_cast<double>(elapsedMicros());
  }
  double markedSeconds() const {
    return 0.001 * 0.001 * static_cast<double>(markedMicros());
  }
private:
  TimePoint start_;
  TimePoint mark_;
};
using SystemClock = Clock<chrono::system_clock>;
using SteadyClock = Clock<chrono::steady_clock>;


class Scope {
public:
  explicit Scope(
    const std::string& scopeName, 
    const std::string& parentName = "", 
    size_t indent = 0
  ) : name_(scopeName), parentName_(parentName), indent_(indent) {}
  ~Scope() {
    std::ostringstream os;
    std::string indentPrefix(indent_ * 2 + 1, ' ');
    os << " << FINISHED: " << parentName_ << "::"<< name_ << " (in " << clock_.elapsedSeconds()<<" s)" << std::endl;
    std::cout << os.str(); 
  }
protected:
  Scope(const Scope&) = delete;
  const Scope& operator = (const Scope&) = delete;
  Scope(Scope&&) = default;
private:
  std::string name_;
  std::string parentName_;
  size_t indent_;
  SystemClock clock_;
};
}
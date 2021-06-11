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
#include <vector>
namespace catena {
class Hash {
public:
  static const std::string kBlankHash;
  static const std::string kAllZeroesHash;
  static std::string sha256(const std::string& input);
  static std::string merkle1(const std::string& s);
  static std::string merkle2(const std::string& s1, const std::string& s2);
  template <typename InputIterator, typename OutputIterator>
  static void merkleRange(OutputIterator out, InputIterator begin, InputIterator end) {
    if (begin == end) {
      *out = kBlankHash;
      out++;
      return;
    }
    for (auto iter = begin; iter != end; ) {
      std::string s = *iter;
      iter++;
      if (iter == end) {
        *out = merkle1(s);
      } else {
        *out = merkle2(s, *iter);
        iter++;
      }
      out++;
    }
  }
  static std::string merkle(const std::vector<std::string>& data);
  static bool satisfiesDifficulty(const std::string& hashString, size_t difficulty);
};
}
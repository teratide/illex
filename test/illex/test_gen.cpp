// Copyright 2020 Teratide B.V.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <gtest/gtest.h>
#include <rapidjson/writer.h>

#include "illex/document.h"
#include "illex/value.h"

namespace illex::test {

TEST(Generators, UniformIntDistribution) {
  std::mt19937 gen(0);
  UniformIntDistribution<int64_t> dist(-1, 1);
  for (int i = 0; i < 4096; i++) {
    ASSERT_GT(dist(gen), -2);
    ASSERT_LT(dist(gen), 2);
  }
}

TEST(Generators, EmptyDocument) {
  DocumentGenerator doc(0);
  rapidjson::StringBuffer b;
  rapidjson::Writer p(b);
  doc.Get().Accept(p);
  ASSERT_STREQ(b.GetString(), "null");
}

TEST(Generators, TwoValuesFromOneGenerator) {
  DocumentGenerator doc(0);
  rapidjson::StringBuffer b;
  rapidjson::Writer p(b);
  doc.Get().Accept(p);
  p.Reset(b);
  doc.Get().Accept(p);
  ASSERT_STREQ(b.GetString(), "nullnull");
}

}  // namespace illex::test
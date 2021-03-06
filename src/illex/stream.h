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

#pragma once

#include <arrow/api.h>
#include <illex/protocol.h>

#include "illex/document.h"
#include "illex/producer.h"
#include "illex/server.h"
#include "illex/status.h"

namespace illex {

/// \brief Options for the stream subcommand.
struct StreamOptions {
  /// Properties of the message protocol.
  ServerOptions server;
  /// Options for the JSON production facilities.
  ProducerOptions production;
  /// Options for repeated streaming mode
  RepeatOptions repeat;
  /// Whether to log statistics
  bool statistics = true;
  /// Repeat server creation, connecting, and sending JSONs indefinitely
  bool repeat_server = false;
};

/// \brief Run the stream subcommand.
auto RunStream(const StreamOptions& options) -> Status;

}  // namespace illex

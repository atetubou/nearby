// Copyright 2022 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef THIRD_PARTY_NEARBY_FASTPAIR_INTERNAL_PUBLIC_TIMER_IMPL_H_
#define THIRD_PARTY_NEARBY_FASTPAIR_INTERNAL_PUBLIC_TIMER_IMPL_H_

#include <functional>
#include <memory>

#include "fastpair/internal/api/fast_pair_platform.h"
#include "fastpair/internal/public/timer.h"

namespace nearby {
namespace fastpair {

class TimerImpl : public Timer {
 public:
  ~TimerImpl() override { Stop(); }

  bool Start(int delay, int period, std::function<void()> callback) override;
  bool Stop() override;
  bool IsRunning() override;
  bool FireNow() override;

 private:
  int delay_ = 0;
  int period_ = 0;
  std::function<void()> callback_ = nullptr;
  std::unique_ptr<api::Timer> internal_timer_ = nullptr;
};

}  // namespace fastpair
}  // namespace nearby

#endif  // THIRD_PARTY_NEARBY_FASTPAIR_INTERNAL_PUBLIC_TIMER_IMPL_H_

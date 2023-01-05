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

#include "fastpair/internal/impl/windows/timer.h"

#include <functional>
#include <memory>
#include <utility>

#include "internal/platform/logging.h"

namespace nearby {
namespace windows {

Timer::~Timer() { Stop(); }

bool Timer::Create(int delay, int interval, std::function<void()> callback) {
  if ((delay < 0) || (interval < 0)) {
    NEARBY_LOGS(WARNING) << "Delay and interval shouldn\'t be negative value.";
    return false;
  }

  if (timer_queue_handle_ != nullptr) {
    return false;
  }

  // Creates a queue for timers.
  // If succeeds, return a handle to the timer queue.
  // If fails, the return value is NULL.
  timer_queue_handle_ = CreateTimerQueue();
  if (timer_queue_handle_ == nullptr) {
    NEARBY_LOGS(ERROR) << "Failed to create timer queue.";
    return false;
  }

  delay_ = delay;
  interval_ = interval;
  callback_ = std::move(callback);

  // Creates a timer-queue timer. This timer expires at the specified due time,
  // then after every specified period. When the timer expires,
  // the callback function is called.
  if (!CreateTimerQueueTimer(&handle_, timer_queue_handle_,
                             static_cast<WAITORTIMERCALLBACK>(TimerRoutine),
                             &callback_, delay, interval, WT_EXECUTEDEFAULT)) {
    // Deletes a timer queue.
    // Any pending timers in the queue are canceled and deleted.
    if (!DeleteTimerQueueEx(timer_queue_handle_, nullptr)) {
      NEARBY_LOGS(ERROR) << "Failed to create timer in timer queue.";
    }
    timer_queue_handle_ = nullptr;
    return false;
  }

  return true;
}

bool Timer::Stop() {
  if (timer_queue_handle_ == nullptr) {
    return true;
  }

  // Deletes a timer queue.
  // Any pending timers in the queue are canceled and deleted.
  if (!DeleteTimerQueueTimer(timer_queue_handle_, handle_, nullptr)) {
    if (GetLastError() != ERROR_IO_PENDING) {
      NEARBY_LOGS(ERROR) << "Failed to delete timer from timer queue.";
      return false;
    }
  }

  handle_ = nullptr;

  if (!DeleteTimerQueueEx(timer_queue_handle_, nullptr)) {
    NEARBY_LOGS(ERROR) << "Failed to delete timer queue.";
    return false;
  }

  timer_queue_handle_ = nullptr;
  return true;
}

bool Timer::FireNow() {
  if (!timer_queue_handle_ || !callback_) {
    return false;
  }

  callback_();
  return true;
}

void CALLBACK Timer::TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired) {
  std::function<void()>* callback =
      reinterpret_cast<std::function<void()>*>(lpParam);
  if (*callback != NULL) {
    (*callback)();
  }
}

}  // namespace windows
}  // namespace nearby

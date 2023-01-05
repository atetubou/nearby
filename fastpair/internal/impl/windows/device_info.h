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

#ifndef THIRD_PARTY_NEARBY_FASTPAIR_INTERNAL_IMPL_WINDOWS_DEVICE_INFO_H_
#define THIRD_PARTY_NEARBY_FASTPAIR_INTERNAL_IMPL_WINDOWS_DEVICE_INFO_H_

#include <guiddef.h>
#include <windows.h>

#include <functional>
#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "fastpair/internal/api/device_info.h"

namespace nearby {
namespace windows {

class DeviceInfo : public api::DeviceInfo {
 public:
  ~DeviceInfo() override;

  api::DeviceInfo::OsType GetOsType() const override;

  bool IsScreenLocked() const override;
  void RegisterScreenLockedListener(
      absl::string_view listener_name,
      std::function<void(api::DeviceInfo::ScreenStatus)> callback) override;
  void UnregisterScreenLockedListener(absl::string_view listener_name) override;
  absl::flat_hash_map<std::string,
                      std::function<void(api::DeviceInfo::ScreenStatus)>>
      screen_locked_listeners_;

  HINSTANCE instance_ = nullptr;
  ATOM registered_class_ = NULL;
  HWND message_window_handle_ = nullptr;
};

}  // namespace windows
}  // namespace nearby

#endif  // THIRD_PARTY_NEARBY_FASTPAIR_INTERNAL_IMPL_WINDOWS_DEVICE_INFO_H_

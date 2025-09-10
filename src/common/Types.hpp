#pragma once

#include <optional>
#include <string>

namespace Types {
using PathString         = std::string; // std::filesystem::path
using OptionalPathString = std::optional<PathString>;

using TimeString         = std::string; // std::chrono::system_clock::time_point
using OptionalTimeString = std::optional<TimeString>;
}

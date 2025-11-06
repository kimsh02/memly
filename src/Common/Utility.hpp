#pragma once

#include <source_location>
#include <string>

namespace Utility {
[[noreturn]] void
LogAndExit(const std::string&,
           const std::source_location& = std::source_location::current());
}

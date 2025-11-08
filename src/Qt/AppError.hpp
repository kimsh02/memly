#pragma once

#include <string>

namespace AppError {
[[noreturn]] void Exit(const std::string&);
}

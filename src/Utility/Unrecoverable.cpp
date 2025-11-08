#include "Unrecoverable.hpp"

#include <format>
#include <stdexcept>

[[noreturn]] void
Unrecoverable::Throw(const std::source_location& SourceLocation) {
    throw std::runtime_error(std::format("{}, {}, {}",
                                         SourceLocation.file_name(),
                                         SourceLocation.function_name(),
                                         SourceLocation.line()));
}

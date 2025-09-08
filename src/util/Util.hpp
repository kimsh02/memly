#pragma once

#include <QtGlobal>
#include <source_location>
#include <string>

[[noreturn]] inline void Fatal(const std::string&         msg,
                               const std::source_location loc = std::source_location::current()) {
    qFatal("%s:%u in %s: %s", loc.file_name(), loc.line(), loc.function_name(), msg.c_str());
}

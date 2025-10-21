#pragma once

#include <QtGlobal>
#include <source_location>
#include <string>

[[noreturn]] inline void
Fatal(const std::string&         Msg,
      const std::source_location Loc = std::source_location::current()) {
    qFatal("%s:%u in %s: %s",
           Loc.file_name(),
           Loc.line(),
           Loc.function_name(),
           Msg.c_str());
}

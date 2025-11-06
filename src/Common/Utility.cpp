#include "Utility.hpp"

#include <QtGlobal>

[[noreturn]] void Utility::LogAndExit(const std::string&          Msg,
                                      const std::source_location& Loc) {
    qFatal("%s:%u in %s\nFailed: %s",
           Loc.file_name(),
           Loc.line(),
           Loc.function_name(),
           Msg.c_str());
}

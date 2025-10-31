#pragma once

#include <string>

struct MigrationsRow {
    const std::size_t Version;
    const std::string AppliedAt;
};

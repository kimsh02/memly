#pragma once

#include <duckdb.hpp>

struct MigrationsRow {
    const std::size_t         Version;
    const duckdb::timestamp_t AppliedAt;
};

#include "SqlResource.hpp"

#include <cassert>
#include <fstream>

static std::string SqlResourceFileToString(const std::string& FilePath) {
    std::ifstream Stream{ FilePath, std::ios::binary };
    assert(Stream.good() && "Failed to open SQL file");
    return std::string{ std::istreambuf_iterator<char>(Stream),
                        std::istreambuf_iterator<char>() };
}

std::string TestApp::SqlResource::InitializeSchemaSql() {
    return SqlResourceFileToString(
        std::string{ CMAKE_GENERATED_PROJECT_ROOT_DIR } +
        "/src/Sql/Migrations/Schema.sql");
}

#include "TestSqlResource.hpp"

#include <cassert>
#include <fstream>

static std::string SqlFileToString(const std::string& FilePath) {
    std::ifstream Stream(FilePath, std::ios::binary);
    assert(Stream && "Failed to open SQL file");
    return std::string(std::istreambuf_iterator<char>(Stream),
                       std::istreambuf_iterator<char>());
}

std::string TestSqlResource::InitializeSchema() {
    return SqlFileToString(std::string(CMAKE_GENERATED_PROJECT_ROOT_DIR) +
                           "/src/Database/Sql/Schema.sql");
}

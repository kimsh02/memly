#include "TestAppData.hpp"

#include <filesystem>
#include <string>

static std::string EnsureDirectory(const std::string& Directory) {
    std::filesystem::create_directories(Directory);
    return Directory;
}

std::string TestAppData::BaseDirectoryPath() {
    return EnsureDirectory(std::string{ CMAKE_GENERATED_PROJECT_ROOT_DIR } +
                           "/test");
}

std::string TestAppData::DatabaseFilePath() {
    const std::string FilePath{ TestAppData::BaseDirectoryPath() +
                                "/test.duckdb" };
    std::error_code Ec{};
    std::filesystem::remove(FilePath, Ec);
    return FilePath;
}

std::string TestAppData::AudioDirectoryPath() {
    return EnsureDirectory(TestAppData::BaseDirectoryPath() + "/Audio");
}

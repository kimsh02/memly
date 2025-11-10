#include "TestAppSupportData.hpp"

#include <filesystem>
#include <string>

static std::string EnsureDirectory(const std::string& Directory) {
    std::filesystem::create_directories(Directory);
    return Directory;
}

std::string TestAppSupportData::BaseDirectoryPath() {
    return EnsureDirectory(std::string{ CMAKE_GENERATED_PROJECT_ROOT_DIR } +
                           "/test");
}

std::string TestAppSupportData::DatabaseFilePath() {
    const std::string FilePath{ TestAppSupportData::BaseDirectoryPath() +
                                "/test.duckdb" };
    std::error_code Ec{};
    std::filesystem::remove(FilePath, Ec);
    return FilePath;
}

std::string TestAppSupportData::AudioDirectoryPath() {
    return EnsureDirectory(TestAppSupportData::BaseDirectoryPath() + "/Audio");
}

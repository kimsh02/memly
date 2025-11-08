#include "AppData.hpp"

#include <QDir>
#include <QStandardPaths>

static std::string EnsureDirectory(const std::string& Directory) {
    QDir().mkpath(Directory.c_str());
    return Directory;
}

std::string UserData::BaseDirectoryPath() {
    return EnsureDirectory(
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
            .toStdString());
}

std::string UserData::DatabaseFilePath() {
    return UserData::BaseDirectoryPath() + "/memly.duckdb";
}

std::string UserData::AudioDirectoryPath() {
    return EnsureDirectory(UserData::BaseDirectoryPath() + "/Audio");
}

std::string TestData::BaseDirectoryPath() {
    return EnsureDirectory("../../test");
}

std::string TestData::DatabaseFilePath() {
    const std::string FilePath = TestData::BaseDirectoryPath() + "/test.duckdb";
    QFile::remove(FilePath.c_str());
    return FilePath;
}

std::string TestData::AudioDirectoryPath() {
    return EnsureDirectory(TestData::BaseDirectoryPath() + "/Audio");
}

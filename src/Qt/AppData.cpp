#include "AppData.hpp"

#include <QDir>
#include <QStandardPaths>

static std::string EnsureDirectory(const std::string& Directory) {
    QDir().mkpath(Directory.c_str());
    return Directory;
}

std::string AppData::BaseDirectoryPath() {
    return EnsureDirectory(
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
            .toStdString());
}

std::string AppData::DatabaseFilePath() {
    return AppData::BaseDirectoryPath() + "/memly.duckdb";
}

std::string AppData::AudioDirectoryPath() {
    return EnsureDirectory(AppData::BaseDirectoryPath() + "/Audio");
}

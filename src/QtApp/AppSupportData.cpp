#include "AppSupportData.hpp"

#include <QDir>
#include <QStandardPaths>

static std::string EnsureDirectory(const std::string& Directory) {
    QDir().mkpath(Directory.c_str());
    return Directory;
}

std::string AppSupportData::BaseDirectoryPath() {
    return EnsureDirectory(
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
            .toStdString());
}

std::string AppSupportData::DatabaseFilePath() {
    return AppSupportData::BaseDirectoryPath() + "/memly.duckdb";
}

std::string AppSupportData::AudioDirectoryPath() {
    return EnsureDirectory(AppSupportData::BaseDirectoryPath() + "/Audio");
}

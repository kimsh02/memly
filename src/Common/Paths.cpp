#include "Paths.hpp"

#include <QDir>
#include <QStandardPaths>

static std::string EnsureDir(const std::string& Dir) {
    QDir().mkpath(Dir.c_str());
    return Dir;
}

std::string Paths::AppDataDir() {
    return EnsureDir(
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
            .toStdString());
}

std::string Paths::DatabaseFile() { return AppDataDir() + "/memly.duckdb"; }

std::string Paths::AudioDir() { return EnsureDir(AppDataDir() + "/audio"); }

std::string Paths::ImagesDir() { return EnsureDir(AppDataDir() + "/images"); }

#include "Paths.hpp"

#include <QDir>
#include <QStandardPaths>

#include "Common/Utility.hpp"

static std::string EnsureDir(const std::string& Dir) {
    QDir().mkpath(Dir.c_str());
    return Dir;
}

std::string AppData::AppDataDirectory() {
    return EnsureDir(
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
            .toStdString());
}

std::string AppData::DatabaseFile() {
    return AppDataDirectory() + "/memly.duckdb";
}

std::string AppData::AudioDirectory() {
    return EnsureDir(AppDataDirectory() + "/Audio");
}

std::string AppData::ImagesDirectory() {
    return EnsureDir(AppDataDirectory() + "/Images");
}

std::string AppResources::QMLMainURL() {
    return std::string{ "qrc:/QML/Main.qml" };
}

std::string AppResources::SQLSchema() {
    QFile File("qrc:/DuckDB/SQL/Schema.sql");
    if (!File.open(QIODevice::ReadOnly | QIODevice::Text))
        Utility::LogAndExit();
    return QTextStream(&File).readAll().toStdString();
}

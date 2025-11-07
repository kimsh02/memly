#include "Files.hpp"

#include <QDir>
#include <QStandardPaths>

// #include "Common/Utility.hpp"

static std::string EnsureDirectory(const std::string& Directory) {
    QDir().mkpath(Directory.c_str());
    return Directory;
}

std::string AppData::AppDataDirectory() {
    return EnsureDirectory(
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
            .toStdString());
}

std::string AppData::DatabaseFile() {
    return AppDataDirectory() + "/memly.duckdb";
}

std::string AppData::AudioDirectory() {
    return EnsureDirectory(AppDataDirectory() + "/Audio");
}

std::string AppData::ImagesDirectory() {
    return EnsureDirectory(AppDataDirectory() + "/Images");
}

std::string AppResources::QMLMainURL() {
    return std::string("qrc:/memly/QML/Main.qml");
}

static std::string SQLString(const std::string& QtResourcePath) {
    QFile File(QtResourcePath.c_str());
    bool  FileOpened = File.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(FileOpened);
    if (!File.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Utility::LogAndExit(
        //     std::format("Read Qt resource file \"{}\"", QtResourcePath));
    }
    return QTextStream(&File).readAll().toStdString();
}

std::string AppResources::SQLSchema() {
    return SQLString(":/memly/Database/SQL/Schema.sql");
}

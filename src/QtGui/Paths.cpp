#include "Paths.hpp"

#include <QDir>
#include <QStandardPaths>

static QString EnsureDir(const QString& Dir) {
    QDir().mkpath(Dir);
    return Dir;
}

QString Paths::AppDataDir() {
    return EnsureDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
}

QString Paths::DatabaseFile() { return AppDataDir() + "/repetition.db"; }

QString Paths::AudioDir() { return EnsureDir(AppDataDir() + "/audio"); }

QString Paths::ImagesDir() { return EnsureDir(AppDataDir() + "/images"); }

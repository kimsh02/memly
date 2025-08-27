#include "Paths.hpp"

#include <QDir>
#include <QStandardPaths>

static QString ensureDir(const QString& dir) {
    QDir().mkpath(dir);
    return dir;
}

QString Paths::AppDataDir() {
    return ensureDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
}

QString Paths::DatabaseFile() { return AppDataDir() + "/repetition.db"; }

QString Paths::AudioDir() { return ensureDir(AppDataDir() + "/audio"); }

QString Paths::ImagesDir() { return ensureDir(AppDataDir() + "/images"); }

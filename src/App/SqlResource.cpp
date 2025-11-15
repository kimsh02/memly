#include "SqlResource.hpp"

#include <QFile>
#include <QTextStream>

static std::string SqlResourceFileToString(const QString& QtResourcePath) {
    QFile File{ QtResourcePath };
    File.open(QIODevice::ReadOnly);
    assert(File.error() == QFileDevice::NoError);
    return QTextStream{ &File }.readAll().toStdString();
}

std::string App::SqlResource::InitializeSchemaSql() {
    return SqlResourceFileToString(":/Memly/Migrations/Schema.sql");
}

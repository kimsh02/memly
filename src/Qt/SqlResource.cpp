#include "SqlResource.hpp"

#include <QFile>
#include <QTextStream>

static std::string SqlResourceFileToString(const std::string& QtResourcePath) {
    QFile File{ QtResourcePath.c_str() };
    File.open(QIODevice::ReadOnly);
    assert(File.error() == QFileDevice::NoError);
    return QTextStream{ &File }.readAll().toStdString();
}

std::string SqlResource::InitializeSchema() {
    return SqlResourceFileToString(":/memly/Database/Sql/Schema.sql");
}

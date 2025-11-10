#include "AppSqlResource.hpp"

#include <QFile>
#include <QTextStream>

static std::string SqlResourceFileToString(const std::string& QtResourcePath) {
    QFile File{ QtResourcePath.c_str() };
    File.open(QIODevice::ReadOnly);
    assert(File.error() == QFileDevice::NoError);
    return QTextStream{ &File }.readAll().toStdString();
}

std::string AppSqlResource::InitializeSchemaSql() {
    return SqlResourceFileToString(":/Memly/Database/Sql/Schema.sql");
}

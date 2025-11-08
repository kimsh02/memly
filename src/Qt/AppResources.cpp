#include "AppResources.hpp"

#include <QFile>
#include <QTextStream>

#include "Qt/AppError.hpp"

QString AppResources::MainQmlPath() {
    return QString("qrc:/memly/QML/Main.qml");
}

static std::string SqlFileToString(const std::string& QtResourcePath) {
    QFile File(QtResourcePath.c_str());
    File.open(QIODevice::ReadOnly);
    assert(File.error() == QFileDevice::NoError);
    if (File.error() != QFileDevice::NoError) {
        AppError::Exit(
            std::format("Open Qt resource file \"{}\"", QtResourcePath));
    }
    return QTextStream(&File).readAll().toStdString();
}

std::string AppResources::SchemaSqlString() {
    return SqlFileToString(":/memly/Database/Sql/Schema.sql");
}

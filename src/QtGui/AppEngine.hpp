#pragma once
#include <QQmlApplicationEngine>
#include <QUrl>

class AppEngine {
public:
    // Returns true on successful QML load.
    bool Init(const QUrl& MainUrl = QUrl("qrc:/Qml/Main.qml"));

private:
    QQmlApplicationEngine m_Engine; // Lives after QGuiApplication is constructed.
};

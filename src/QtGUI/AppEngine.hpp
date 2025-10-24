#pragma once
#include <QQmlApplicationEngine>
#include <QUrl>

class AppEngine {
public:
    AppEngine(const QUrl& = QUrl("qrc:/Qml/Main.qml")) noexcept;

private:
    QQmlApplicationEngine m_Engine;
};

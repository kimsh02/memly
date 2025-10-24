#pragma once
#include <QQmlApplicationEngine>
#include <QUrl>

class AppEngine {
public:
    AppEngine(const QUrl& MainURL = QUrl("qrc:/Qml/Main.qml")) noexcept;

private:
    QQmlApplicationEngine m_Engine;
};

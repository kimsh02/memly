#pragma once
#include <QQmlApplicationEngine>
#include <QUrl>

class AppEngine {
public:
    AppEngine(const QUrl& MainURL = QUrl("qrc:/Qml/Main.qml"));

private:
    QQmlApplicationEngine m_Engine;
};

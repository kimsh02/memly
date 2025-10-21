#pragma once
#include <QQmlApplicationEngine>
#include <QUrl>

class AppEngine {
public:
    void Init(const QUrl& MainUrl = QUrl("qrc:/Qml/Main.qml"));

private:
    QQmlApplicationEngine m_Engine;
};

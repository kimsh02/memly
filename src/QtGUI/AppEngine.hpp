#pragma once
#include <QQmlApplicationEngine>
#include <QUrl>

#include "Common/Files.hpp"

class AppEngine {
public:
    AppEngine(const QUrl& = QUrl(AppResources::QMLMainURL().c_str())) noexcept;

private:
    QQmlApplicationEngine m_Engine;
};

#pragma once
#include <QQmlApplicationEngine>
#include <QUrl>

#include "Common/Paths.hpp"

class AppEngine {
public:
    AppEngine(const QUrl& = QUrl(AppResources::QMLMainURL().c_str())) noexcept;

private:
    QQmlApplicationEngine m_Engine;
};

#pragma once
#include <QQmlApplicationEngine>
#include <QUrl>

#include "Common/Files.hpp"

class AppEngine {
public:
    explicit AppEngine(
        const QUrl& = QUrl(AppResources::QMLMainURL().c_str())) noexcept;

    AppEngine(const AppEngine&)            = delete;
    AppEngine& operator=(const AppEngine&) = delete;
    AppEngine(AppEngine&&)                 = delete;
    AppEngine& operator=(AppEngine&&)      = delete;

private:
    QQmlApplicationEngine m_Engine;
};

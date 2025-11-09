#pragma once

#include <QQmlApplicationEngine>

class AppEngine {
public:
    explicit AppEngine() noexcept;

    AppEngine(const AppEngine&) = delete;
    AppEngine& operator=(const AppEngine&) = delete;
    AppEngine(AppEngine&&) = delete;
    AppEngine& operator=(AppEngine&&) = delete;

private:
    QQmlApplicationEngine m_Engine;
};

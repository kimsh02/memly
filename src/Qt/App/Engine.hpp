#pragma once

#include <QQmlApplicationEngine>

namespace App {
class Engine {
public:
    explicit Engine() noexcept;

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;

private:
    QQmlApplicationEngine m_Engine{};
};
}

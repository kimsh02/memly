#include "Engine.hpp"

#include <QCoreApplication>

#include "QmlResource.hpp"

App::Engine::Engine() noexcept {
    QObject::connect(
        &m_Engine,
        &QQmlApplicationEngine::objectCreationFailed,
        QCoreApplication::instance(),
        [] { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    m_Engine.load(App::QmlResource::MainWindow());
}

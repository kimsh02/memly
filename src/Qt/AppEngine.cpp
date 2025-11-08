#include "AppEngine.hpp"

#include <QCoreApplication>

#include "Qt/QmlResource.hpp"

AppEngine::AppEngine() noexcept {
    QObject::connect(
        &m_Engine,
        &QQmlApplicationEngine::objectCreationFailed,
        QCoreApplication::instance(),
        [] { QCoreApplication::exit(1); },
        Qt::QueuedConnection);

    m_Engine.load(QmlResource::MainWindow());
}

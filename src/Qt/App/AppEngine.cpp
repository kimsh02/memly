#include "AppEngine.hpp"

#include <QCoreApplication>

#include "Qt/App/AppQmlResource.hpp"

AppEngine::AppEngine() noexcept {
    QObject::connect(
        &m_Engine,
        &QQmlApplicationEngine::objectCreationFailed,
        QCoreApplication::instance(),
        [] { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    m_Engine.load(AppQmlResource::MainWindow());
}

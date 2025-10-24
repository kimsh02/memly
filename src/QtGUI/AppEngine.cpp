#include "AppEngine.hpp"

#include <QCoreApplication>

AppEngine::AppEngine(const QUrl& MainUrl) noexcept {
    QObject::connect(
        &m_Engine,
        &QQmlApplicationEngine::objectCreationFailed,
        QCoreApplication::instance(),
        [] { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    m_Engine.load(MainUrl);
}

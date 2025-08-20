#include "gui/App.hpp"

#include <QCoreApplication>

bool App::Init(const QUrl& mainUrl) {
    QObject::connect(
        &m_Engine,
        &QQmlApplicationEngine::objectCreated,
        QCoreApplication::instance(),
        [mainUrl](QObject* obj, const QUrl& objUrl) {
            if (!obj && objUrl == mainUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    m_Engine.load(mainUrl);
    return !m_Engine.rootObjects().isEmpty();
}

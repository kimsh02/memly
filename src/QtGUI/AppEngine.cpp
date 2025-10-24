#include "AppEngine.hpp"

#include <QCoreApplication>

#include "Common/Utility.hpp"

AppEngine::AppEngine(const QUrl& MainUrl) {
    QObject::connect(
        &m_Engine,
        &QQmlApplicationEngine::objectCreated,
        QCoreApplication::instance(),
        [MainUrl](QObject* Obj, const QUrl& ObjUrl) {
            if (!Obj && ObjUrl == MainUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    m_Engine.load(MainUrl);
    if (m_Engine.rootObjects().isEmpty()) {
        Utility::Fatal("AppEngine failed to start.");
    };
}

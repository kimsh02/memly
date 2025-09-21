#include "gui/App.hpp"

#include <QCoreApplication>

bool App::Init(const QUrl& MainUrl) {
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
    return !m_Engine.rootObjects().isEmpty();
}

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "gui/App.hpp"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    app.setApplicationDisplayName("RepetitionAI");

    App gui;
    if (!gui.Init()) { return -1; }

    return app.exec();
}

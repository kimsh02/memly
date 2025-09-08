#include <QGuiApplication>
#include <cstdlib>
#include <stdexcept>

#include "SettingsStore.hpp"
#include "database_qt/DatabaseQt.hpp"
#include "gui/App.hpp"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    std::string     name = "RepetitionAI";
    app.setApplicationDisplayName(name.c_str());
    app.setApplicationName(name.c_str());

    try {
        App gui;
        if (!gui.Init()) return EXIT_FAILURE;
        DatabaseQt    db;
        SettingsStore ss(db);
        return app.exec();
    } catch (const std::runtime_error& e) {
        qCritical() << "Fatal error: " << e.what();
        return EXIT_FAILURE;
    }
}

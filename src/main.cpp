#include <QGuiApplication>
// #include <cstdlib>
// #include <stdexcept>

#include "database_qt/DatabaseQt.hpp"
#include "gui/App.hpp"
#include "repository/DeckStore.hpp"
#include "repository/SettingsStore.hpp"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    std::string     name = "RepetitionAI";
    app.setApplicationDisplayName(name.c_str());
    app.setApplicationName(name.c_str());

    try {
        App gui;
        if (!gui.Init())
            return EXIT_FAILURE;
        DatabaseQt    db;
        SettingsStore ss(db);
        CardStore     cs;
        DeckStore     ds(db, cs);
        return app.exec();
    } catch (const std::runtime_error& e) {
        qCritical() << "Fatal error: " << e.what();
        return EXIT_FAILURE;
    }
}

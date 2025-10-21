#include <QGuiApplication>

#include "DuckDB/Database.hpp"
#include "QtGui/AppEngine.hpp"

int main(int argc, char* argv[]) {
    QGuiApplication App(argc, argv);
    std::string     AppName = "Memly";
    App.setApplicationDisplayName(AppName.c_str());
    App.setApplicationName(AppName.c_str());

    try {
        AppEngine Engine;
        if (!Engine.Init())
            return EXIT_FAILURE;

        Database Database;

        return App.exec();
    } catch (const std::runtime_error& e) {
        qCritical() << "Fatal error: " << e.what();
        return EXIT_FAILURE;
    }
}

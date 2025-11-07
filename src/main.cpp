#include <QGuiApplication>

#include "Database/DuckDB.hpp"
#include "QtGUI/AppEngine.hpp"

int main(int argc, char* argv[]) {
    try {
        QGuiApplication App(argc, argv);
        std::string     AppName = "Memly";
        App.setApplicationDisplayName(AppName.c_str());
        App.setApplicationName(AppName.c_str());

        AppEngine AppEngine;

        DuckDB DuckDB;

        return App.exec();
    } catch (const std::exception& Exception) {
        // Placeholder
        std::println("{}", Exception.what());
        assert(!"Uncaught exception");
        // TODO: Unknown exception is notified to user in GUI
    }
}

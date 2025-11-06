#include <QGuiApplication>

#include "Common/Utility.hpp"
#include "Database/DuckDB.hpp"
#include "QtGUI/AppEngine.hpp"

int main(int argc, char* argv[]) {
    try {
        QGuiApplication App(argc, argv);
        std::string     AppName = "Memly";
        App.setApplicationDisplayName(AppName.c_str());
        App.setApplicationName(AppName.c_str());

        AppEngine AppEngine;

        DuckDB DuckDB; // TODO

        return App.exec();
    } catch (const std::exception& Exception) {
        Utility::LogAndExit(Exception.what());
    }
}

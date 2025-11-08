#include <QGuiApplication>

#include "Database/DuckDb.hpp"
#include "Qt/AppEngine.hpp"
#include "Qt/AppError.hpp"

int main(int argc, char* argv[]) {
    try {
        QGuiApplication App(argc, argv);
        std::string     AppName = "Memly";
        App.setApplicationDisplayName(AppName.c_str());
        App.setApplicationName(AppName.c_str());
        AppEngine AppEngine;
        DuckDb    DuckDb = CreateProductionDuckDb();
        return App.exec();
    } catch (const std::exception& Exception) {
        Q_ASSERT_X(false, "", Exception.what());
        AppError::Exit(Exception.what());
    }
}

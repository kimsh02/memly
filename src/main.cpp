#include <QGuiApplication>

#include "Database/DuckDb.hpp"
#include "Qt/AppData.hpp"
#include "Qt/AppEngine.hpp"
#include "Qt/AppError.hpp"
#include "Qt/SqlResource.hpp"

int main(int argc, char* argv[]) {
    try {
        QGuiApplication App(argc, argv);
        std::string     AppName = "Memly";
        App.setApplicationDisplayName(AppName.c_str());
        App.setApplicationName(AppName.c_str());
        AppEngine AppEngine;
        DuckDb    DuckDb(AppData::DatabaseFilePath());
        DuckDb.Query(SqlResource::InitializeSchema());
        return App.exec();
    } catch (const std::exception& Exception) {
        Q_ASSERT_X(false, "", Exception.what());
        AppError::Exit(std::format("Caught unrecoverable exception from \"{}\"",
                                   Exception.what()));
    }
}

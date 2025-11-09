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

        // DuckDb.Query("insert into decks (name) values('deck1');");
        auto Result = DuckDb.Query("insert into decks (name) values('deck1');");
        auto ErrorType = Result->GetErrorType();
        std::cout << static_cast<int>(ErrorType) << "\n";
        auto ErrorObject = Result->GetErrorObject();
        ErrorObject.ConvertErrorToJSON();
        std::cout << ErrorObject.RawMessage() << "\n";
        for (const auto& [Key, Value] : ErrorObject.ExtraInfo()) {
            std::cout << Key << ": " << Value << "\n";
        }

        return App.exec();
    } catch (const std::exception& Exception) {
        Q_ASSERT_X(false, "", Exception.what());
        AppError::Exit(std::format("Caught unrecoverable exception from \"{}\"",
                                   Exception.what()));
    }
}

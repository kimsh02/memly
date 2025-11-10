#include <duckdb.hpp>

#include <QGuiApplication>

#include "Database/DuckDb.hpp"
#include "Qt/App/Engine.hpp"
#include "Qt/App/Error.hpp"
#include "Qt/App/SqlResource.hpp"
#include "Qt/App/SupportData.hpp"

int main(int argc, char* argv[]) {
    try {
        QGuiApplication App{ argc, argv };
        std::string AppName{ "Memly" };
        App.setApplicationDisplayName(AppName.c_str());
        App.setApplicationName(AppName.c_str());
        App::Engine AppEngine{};
        DuckDb DuckDb{ App::SupportData::DatabaseFilePath() };
        DuckDb.Query(App::SqlResource::InitializeSchemaSql());

        auto Result{ DuckDb.Query(
            "insert into decks (name) values('deutsch');") };
        auto ErrorType{ Result->GetErrorType() };
        std::cout << static_cast<int>(ErrorType) << "\n";
        auto ErrorObject{ Result->GetErrorObject() };
        ErrorObject.ConvertErrorToJSON();
        std::cout << ErrorObject.Message() << "\n";
        for (const auto& [Key, Value] : ErrorObject.ExtraInfo()) {
            std::cout << Key << ": " << Value << "\n";
        }

        return App.exec();
    } catch (const std::exception& Exception) {
        Q_ASSERT_X(false, "", Exception.what());
        App::Error::Exit(std::format(
            "Caught unrecoverable exception from \"{}\"", Exception.what()));
    }
}

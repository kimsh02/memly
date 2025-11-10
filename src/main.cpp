#include <duckdb.hpp>

#include <QGuiApplication>

#include "Database/DuckDb.hpp"
#include "Qt/AppEngine.hpp"
#include "Qt/AppError.hpp"
#include "Qt/AppSqlResource.hpp"
#include "Qt/AppSupportData.hpp"

int main(int argc, char* argv[]) {
    try {
        QGuiApplication App{ argc, argv };
        std::string AppName{ "Memly" };
        App.setApplicationDisplayName(AppName.c_str());
        App.setApplicationName(AppName.c_str());
        AppEngine AppEngine{};
        DuckDb DuckDb{ AppSupportData::DatabaseFilePath() };
        DuckDb.Query(AppSqlResource::InitializeSchemaSql());

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
        AppError::Exit(std::format("Caught unrecoverable exception from \"{}\"",
                                   Exception.what()));
    }
}

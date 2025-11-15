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
        std::cout << App::SupportData::DatabaseFilePath() << "\n";
        DuckDb DuckDb{ App::SupportData::DatabaseFilePath() };
        auto Result{ DuckDb.Query(App::SqlResource::InitializeSchemaSql()) };
        auto ErrorType{ Result->GetErrorType() };
        std::cout << static_cast<int>(ErrorType) << "\n";
        auto ErrorObject{ Result->GetErrorObject() };
        ErrorObject.ConvertErrorToJSON();
        std::cout << ErrorObject.Message() << "\n";

        auto Result1{ DuckDb.Query(
            "insert into decks (name) values('deutsch');") };
        auto ErrorType1{ Result1->GetErrorType() };
        std::cout << static_cast<int>(ErrorType1) << "\n";
        auto ErrorObject1{ Result1->GetErrorObject() };
        ErrorObject1.ConvertErrorToJSON();
        std::cout << ErrorObject1.Message() << "\n";
        for (const auto& [Key, Value] : ErrorObject1.ExtraInfo()) {
            std::cout << Key << ": " << Value << "\n";
        }

        return App.exec();
    } catch (const std::exception& Exception) {
        Q_ASSERT_X(false, "", Exception.what());
        App::Error::Exit(std::format(
            "Caught unrecoverable exception from \"{}\"", Exception.what()));
    }
}

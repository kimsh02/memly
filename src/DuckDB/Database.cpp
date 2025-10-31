#include "Database.hpp"

#include <duckdb.hpp>

#include "Common/Files.hpp"
#include "Common/Utility.hpp"

Database::Database()
    : m_Database(AppData::DatabaseFile())
    , m_Connection(m_Database) {
    EnsureSchema();
    RunMigrations();
}

std::unique_ptr<duckdb::MaterializedQueryResult>
Database::Query(const std::string& Sql) {
    std::unique_ptr<duckdb::MaterializedQueryResult> Result =
        m_Connection.Query(Sql);
    if (Result->HasError()) {
        Utility::LogAndExit(Result->GetError());
    }
    return Result;
}

void Database::EnsureSchema() {
    Query(AppResources::SQLSchema());
}

void Database::RunMigrations() {
}

[[nodiscard]] Database::PreparedStatement
Database::PrepareStatement(const std::string& Sql) {
    std::unique_ptr<duckdb::PreparedStatement> PreparedStatement =
        m_Connection.Prepare(Sql);
    if (PreparedStatement->HasError()) {
        Utility::LogAndExit(PreparedStatement->GetError());
    }
    return Database::PreparedStatement(std::move(PreparedStatement));
}

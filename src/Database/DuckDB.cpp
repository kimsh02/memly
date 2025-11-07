#include "DuckDB.hpp"

#include <duckdb.hpp>

#include "Common/Files.hpp"

// #include "Common/Utility.hpp"

DuckDB::DuckDB()
    : m_DuckDB(AppData::DatabaseFile())
    , m_Connection(m_DuckDB) {
    EnsureSchema();
    RunMigrations();
}

std::unique_ptr<duckdb::MaterializedQueryResult>
DuckDB::Query(const std::string& Sql) {
    std::unique_ptr<duckdb::MaterializedQueryResult> Result =
        m_Connection.Query(Sql);
    if (Result->HasError()) {
        // Utility::LogAndExit(Result->GetError());
        
    }
    return Result;
}

void DuckDB::EnsureSchema() {
    Query(AppResources::SQLSchema());
}

void DuckDB::RunMigrations() {
    // TODO
}

[[nodiscard]] DuckDB::PreparedStatement
DuckDB::PrepareStatement(const std::string& Sql) {
    std::unique_ptr<duckdb::PreparedStatement> PreparedStatement =
        m_Connection.Prepare(Sql);
    if (PreparedStatement->HasError()) {
        // Utility::LogAndExit(PreparedStatement->GetError());
    }
    return DuckDB::PreparedStatement(std::move(PreparedStatement));
}

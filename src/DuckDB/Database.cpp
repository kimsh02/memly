#include "Database.hpp"

#include <duckdb.hpp>

#include "Common/Files.hpp"
#include "Common/Utility.hpp"

Database::Database()
    : m_Database(AppData::DatabaseFile())
    , m_Connection(m_Database) {
    EnsureSchema();

    // TODO: Append to schema_migrations table
}

void Database::Query(const std::string& Sql) {
    std::unique_ptr<duckdb::MaterializedQueryResult> Result =
        m_Connection.Query(Sql);
    if (Result->HasError())
        Utility::LogAndExit(Result->GetError());
}

void Database::EnsureSchema() { Query(AppResources::SQLSchema()); }

[[nodiscard]] std::unique_ptr<duckdb::PreparedStatement>
Database::PrepareStatement(const std::string& Sql) {
    try {
        return m_Connection.Prepare(Sql);
    } catch (const std::exception& Exception) {
        Utility::LogAndExit(Exception.what());
    }
}

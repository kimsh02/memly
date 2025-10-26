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

void Database::EnsureSchema() {
    std::unique_ptr<duckdb::MaterializedQueryResult> Result =
        m_Connection.Query(AppResources::SQLSchema());
    if (Result->HasError())
        Utility::LogAndExit(Result->GetError());
}

[[nodiscard]] std::unique_ptr<duckdb::PreparedStatement>
Database::PrepareStatement(const std::string& Sql) {
    try {
        return m_Connection.Prepare(Sql);
    } catch (const std::exception& Exception) {
        Utility::LogAndExit(Exception.what());
    }
}

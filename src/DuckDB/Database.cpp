#include "Database.hpp"

#include "Common/Paths.hpp"
#include "Common/Utility.hpp"

Database::Database()
    : m_Database(Paths::DatabaseFile())
    , m_Connection(m_Database) {}

void Database::EnsureSchema() {}

[[nodiscard]] std::unique_ptr<duckdb::PreparedStatement>
Database::PrepareStatement(const std::string& Sql) {
    try {
        return m_Connection.Prepare(Sql);
    } catch (const std::exception& Exception) {
        Utility::FatalLogAndExit(Exception.what());
    }
}

#include "Database.hpp"

#include "Common/Paths.hpp"

Database::Database()
    : m_Database(Paths::DatabaseFile())
    , m_Connection(m_Database) {}

void Database::EnsureSchema() const {
    
}

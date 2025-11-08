#include "DuckDb.hpp"

#include <duckdb.hpp>

DuckDb::DuckDb(const std::string& DatabaseFile)
    : m_DuckDb(DatabaseFile)
    , m_Connection(m_DuckDb) {
}

std::unique_ptr<duckdb::MaterializedQueryResult>
DuckDb::Query(const std::string& Sql) {
    std::unique_ptr<duckdb::MaterializedQueryResult> Result =
        m_Connection.Query(Sql);
    assert(Result != nullptr);
    if (Result->HasError()) {
        // TODO
    }
    return Result;
}

[[nodiscard]] DuckDb::PreparedStatement
DuckDb::PrepareStatement(const std::string& Sql) {
    std::unique_ptr<duckdb::PreparedStatement> PreparedStatement =
        m_Connection.Prepare(Sql);
    assert(PreparedStatement != nullptr && !PreparedStatement->HasError());
    return DuckDb::PreparedStatement(std::move(PreparedStatement));
}

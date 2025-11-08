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
    assert(!Result->HasError());
    if (Result->HasError()) {
        AppError::Exit(Result->GetError());
    }
    return Result;
}

[[nodiscard]] DuckDb::PreparedStatement
DuckDb::PrepareStatement(const std::string& Sql) {
    std::unique_ptr<duckdb::PreparedStatement> PreparedStatement =
        m_Connection.Prepare(Sql);
    assert(!PreparedStatement->HasError());
    if (PreparedStatement->HasError()) {
        AppError::Exit(PreparedStatement->GetError());
    }
    return DuckDb::PreparedStatement(std::move(PreparedStatement));
}

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
    if (Result == nullptr) {
        Unrecoverable::Throw();
    }
    if (Result->HasError()) {
        // TODO
    }
    return Result;
}

[[nodiscard]] PreparedStatement DuckDb::Prepare(const std::string& Sql) {
    std::unique_ptr<duckdb::PreparedStatement> Prepared =
        m_Connection.Prepare(Sql);
    assert(Prepared != nullptr && !Prepared->HasError());
    if (Prepared == nullptr) {
        Unrecoverable::Throw();
    }
    return PreparedStatement(std::move(Prepared));
}

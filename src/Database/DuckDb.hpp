#pragma once

#include <duckdb.hpp>

#include "Utility/Unrecoverable.hpp"

class PreparedStatement {
public:
    PreparedStatement(const PreparedStatement&) = delete;
    PreparedStatement& operator=(const PreparedStatement&) = delete;
    PreparedStatement(PreparedStatement&&) = delete;
    PreparedStatement& operator=(PreparedStatement&&) = delete;

    template <typename... Params>
    std::unique_ptr<duckdb::QueryResult> Execute(Params&&... Args) {
        std::unique_ptr<duckdb::QueryResult> Result =
            m_PreparedStatement->Execute(Args...);
        assert(Result != nullptr);
        if (Result == nullptr) {
            Unrecoverable::Throw();
        }
        if (Result->HasError()) {
            // TODO
        }
        return Result;
    }

private:
    friend class DuckDb;

    std::unique_ptr<duckdb::PreparedStatement> m_PreparedStatement;

    explicit PreparedStatement(
        std::unique_ptr<duckdb::PreparedStatement>&& PreparedStatement)
        : m_PreparedStatement(std::move(PreparedStatement)) {};
};

class DuckDb {
public:
    explicit DuckDb(const std::string&);

    DuckDb(const DuckDb&) = delete;
    DuckDb& operator=(const DuckDb&) = delete;
    DuckDb(DuckDb&&) = delete;
    DuckDb& operator=(DuckDb&&) = delete;

    [[nodiscard]] PreparedStatement Prepare(const std::string& Sql);

    std::unique_ptr<duckdb::MaterializedQueryResult> Query(const std::string&);

private:
    duckdb::DuckDB m_DuckDb;
    duckdb::Connection m_Connection;
};

struct DuckDbError {};

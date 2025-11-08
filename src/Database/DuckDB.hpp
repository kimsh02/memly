#pragma once

#include <duckdb.hpp>

#include "Qt/AppError.hpp"

class DuckDb {
public:
    explicit DuckDb(const std::string&);

    class PreparedStatement {
    public:
        template <typename... Params>
        std::unique_ptr<duckdb::QueryResult> Execute(Params&&... Args) {
            std::unique_ptr<duckdb::QueryResult> Result =
                m_PreparedStatement->Execute(Args...);
            assert(!Result->HasError());
            if (Result->HasError()) {
                AppError::Exit(Result->GetError());
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

    [[nodiscard]] DuckDb::PreparedStatement
    PrepareStatement(const std::string& Sql);

private:
    duckdb::DuckDB     m_DuckDb;
    duckdb::Connection m_Connection;

    std::unique_ptr<duckdb::MaterializedQueryResult> Query(const std::string&);

    void EnsureSchema();
    void RunMigrations();
};

DuckDb CreateProductionDuckDb();
DuckDb CreateTestDuckDb();

struct DuckDbError {};

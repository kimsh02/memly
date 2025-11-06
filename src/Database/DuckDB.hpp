#pragma once

#include <duckdb.hpp>

#include "Common/Utility.hpp"

class DuckDB {
public:
    explicit DuckDB();

    DuckDB(const DuckDB&)            = delete;
    DuckDB& operator=(const DuckDB&) = delete;
    DuckDB(DuckDB&&)                 = delete;
    DuckDB& operator=(DuckDB&&)      = delete;

    class PreparedStatement {
    public:
        PreparedStatement(const PreparedStatement&)            = delete;
        PreparedStatement& operator=(const PreparedStatement&) = delete;
        PreparedStatement(PreparedStatement&&)                 = delete;
        PreparedStatement& operator=(PreparedStatement&&)      = delete;

        template <typename... Params>
        std::unique_ptr<duckdb::QueryResult> Execute(Params... Args) {
            std::unique_ptr<duckdb::QueryResult> Result =
                m_PreparedStatement->Execute(Args...);
            if (Result->HasError()) {
                Utility::LogAndExit(Result->GetError());
            }
            return Result;
        }

    private:
        friend class DuckDB;
        std::unique_ptr<duckdb::PreparedStatement> m_PreparedStatement;

        explicit PreparedStatement(
            std::unique_ptr<duckdb::PreparedStatement>&& PreparedStatement)
            : m_PreparedStatement(std::move(PreparedStatement)) {};
    };

    [[nodiscard]] DuckDB::PreparedStatement
    PrepareStatement(const std::string& Sql);

private:
    duckdb::DuckDB     m_DuckDB;
    duckdb::Connection m_Connection;

    std::unique_ptr<duckdb::MaterializedQueryResult> Query(const std::string&);

    void EnsureSchema();
    void RunMigrations();
};

struct DuckDBError {};

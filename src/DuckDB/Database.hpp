#pragma once

#include <duckdb.hpp>

#include "Common/Utility.hpp"

class Database {
public:
    Database();

    class PreparedStatement {
    public:
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
        friend class Database;
        std::unique_ptr<duckdb::PreparedStatement> m_PreparedStatement;

        PreparedStatement(
            std::unique_ptr<duckdb::PreparedStatement>&& PreparedStatement)
            : m_PreparedStatement(std::move(PreparedStatement)) {};
    };

    [[nodiscard]] Database::PreparedStatement
    PrepareStatement(const std::string& Sql);

private:
    duckdb::DuckDB     m_Database;
    duckdb::Connection m_Connection;

    std::unique_ptr<duckdb::MaterializedQueryResult> Query(const std::string&);

    void EnsureSchema();
};

struct DatabaseError {};

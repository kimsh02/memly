#pragma once

#include <duckdb.hpp>

class Database {
public:
    Database(const Database&)          = delete;
    Database operator=(const Database) = delete;

    Database(Database&&) noexcept            = delete;
    Database& operator=(Database&&) noexcept = delete;

    Database();

    [[nodiscard]] std::unique_ptr<duckdb::PreparedStatement>
    PrepareStatement(const std::string& Sql);

    class PreparedStatement {};

private:
    duckdb::DuckDB     m_Database;
    duckdb::Connection m_Connection;

    void Query(const std::string&);
    void EnsureSchema();
};

struct DatabaseError {};

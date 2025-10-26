#pragma once

#include <duckdb.hpp>

#include <filesystem>

class Database {
public:
    Database(const Database&)          = delete;
    Database operator=(const Database) = delete;

    Database(Database&&) noexcept            = delete;
    Database& operator=(Database&&) noexcept = delete;

    Database();

    // void BeginTransaction() { m_Connection.Query("BEGIN TRANSACTION"); }

    // void CommitTransaction() { m_Connection.Query("COMMIT"); }

    // void RollbackTransaction() { m_Connection.Query("ROLLBACK"); }

    [[nodiscard]] std::unique_ptr<duckdb::PreparedStatement>
    PrepareStatement(const std::string& Sql);

    class PreparedStatemet {};

private:
    duckdb::DuckDB     m_Database;
    duckdb::Connection m_Connection;

    static constexpr std::filesystem::path Path();

    void EnsureSchema();
};

struct DatabaseError {};

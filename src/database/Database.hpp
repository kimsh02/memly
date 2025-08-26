#pragma once

#include <string_view>

class Database final {
public:
    Database(const Database&)            = delete;
    Database& operator=(const Database&) = delete;

    Database(Database&&) noexcept            = delete;
    Database& operator=(Database&&) noexcept = delete;

    explicit Database(std::string_view path);

    // using Migration = void (*)(sqlite3* db); // simple function ptr or std::function

    // void RegisterMigrations(int targetVersion, std::vector<Migration> migs);
    // void EnsureSchema(); // reads PRAGMA user_version, applies up to max

    // sqlite3* Handle() const noexcept { return m_Db; }

private:
    // sqlite3*                                            m_Db{};
    // int                                                 m_TargetVersion{ 0 };
    // std::vector<std::pair<int, std::vector<Migration>>> m_All; // version → steps
};

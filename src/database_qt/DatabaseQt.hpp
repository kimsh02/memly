#pragma once

#include <QtSql>

class DatabaseQt final {
public:
    DatabaseQt(const DatabaseQt&)            = delete;
    DatabaseQt& operator=(const DatabaseQt&) = delete;

    DatabaseQt(DatabaseQt&&) noexcept            = delete;
    DatabaseQt& operator=(DatabaseQt&&) noexcept = delete;

    DatabaseQt();
    ~DatabaseQt();

    // bool BeginTransaction() const;
    // bool CommitTransaction() const;
    // bool RollbackTransaction() const;

    class Stmt {
    public:
        Stmt(const Stmt&)            = delete;
        Stmt& operator=(const Stmt&) = delete;

        Stmt(Stmt&&) noexcept            = delete;
        Stmt& operator=(Stmt&&) noexcept = delete;

        template <class... Ts>
        Stmt& Bind(Ts&&... args);

        void Exec();

        [[nodiscard]] bool Next() noexcept { return m_Query.next(); }

        [[nodiscard]] QVariant Value(std::size_t i) const { return m_Query.value(i); };

    private:
        friend class DatabaseQt;

        QSqlQuery m_Query;

        explicit Stmt(QSqlQuery&& q)
            : m_Query(std::move(q)) {}
    };

    [[nodiscard]] Stmt Prepare(const char* sql) const;

private:
    QSqlDatabase m_Db;

    void exec(const char* sql) const;

    void ensureSchema() const;
};

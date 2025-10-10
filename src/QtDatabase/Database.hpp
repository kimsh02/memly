#pragma once

#include <QtSql>
#include <expected>

class DatabaseQt final {
public:
    DatabaseQt(const DatabaseQt&)            = delete;
    DatabaseQt& operator=(const DatabaseQt&) = delete;

    DatabaseQt(DatabaseQt&&) noexcept            = delete;
    DatabaseQt& operator=(DatabaseQt&&) noexcept = delete;

    DatabaseQt();
    ~DatabaseQt();

    bool BeginTx() { return m_Db.transaction(); }

    bool CommitTx() { return m_Db.commit(); }

    bool RollbackTx() { return m_Db.rollback(); }

    class Stmt {
    public:
        Stmt(const Stmt&)            = delete;
        Stmt& operator=(const Stmt&) = delete;

        Stmt(Stmt&&) noexcept            = delete;
        Stmt& operator=(Stmt&&) noexcept = delete;

        // template <class T>
        // static inline QVariant ToVariant(T&& v) {
        //     return QVariant::fromValue(std::forward<T>(v));
        // }

        // static inline QVariant ToVariant(const char* s) { return QVariant(QString::fromUtf8(s)); }

        // template <std::size_t N>
        // static inline QVariant ToVariant(const char (&s)[N]) {
        //     return QVariant(QString::fromUtf8(s, N ? (N - 1) : 0));
        // }

        // static inline QVariant ToVariant(int v) { return QVariant(v); }

        template <class... Ts>
        Stmt& Bind(Ts&&... Args) {
            (m_Query.addBindValue(ToVariant(std::forward<Ts>(Args))), ...);
            return *this;
        }

        std::expected<void, std::string> Exec();

        [[nodiscard]] bool Next() noexcept { return m_Query.next(); }

        [[nodiscard]] QVariant Value(std::size_t I) const {
            return m_Query.value(I);
        };

        // [[nodiscard]] Stmt&& BindValue(std::size_t i, const QVariant& qv) noexcept {
        //     m_Query.bindValue(i, qv);
        //     return std::move(*this);
        // }

        void Finish() noexcept { m_Query.finish(); }

        [[nodiscard]] std::size_t LastInsertId() const {
            return m_Query.lastInsertId().toULongLong();
        }

    private:
        friend class DatabaseQt;

        QSqlQuery m_Query;

        explicit Stmt(QSqlQuery&& Q) noexcept
            : m_Query(std::move(Q)) {}

        static QVariant ToVariant(const std::string& S) {
            return QVariant(QString(S.c_str()));
        }

        static QVariant ToVariant(std::size_t V) {
            return QVariant(qulonglong(V));
        }
    };

    [[nodiscard]] Stmt Prepare(const char* Sql) const;

private:
    QSqlDatabase m_Db;

    void Exec(const char* Sql) const;

    void EnsureSchema() const;
};

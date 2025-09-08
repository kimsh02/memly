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
        Stmt& Bind(Ts&&... args) {
            (m_Query.addBindValue(toVariant(std::forward<Ts>(args))), ...);
            return *this;
        }

        void Exec();

        [[nodiscard]] bool Next() noexcept { return m_Query.next(); }

        [[nodiscard]] QVariant Value(std::size_t i) const { return m_Query.value(i); };

        // [[nodiscard]] Stmt&& BindValue(std::size_t i, const QVariant& qv) noexcept {
        //     m_Query.bindValue(i, qv);
        //     return std::move(*this);
        // }

        void Finish() noexcept { m_Query.finish(); }

    private:
        friend class DatabaseQt;

        QSqlQuery m_Query;

        explicit Stmt(QSqlQuery&& q)
            : m_Query(std::move(q)) {}

        static QVariant toVariant(const std::string& s) { return QVariant(QString(s.c_str())); }

        static QVariant toVariant(std::size_t v) { return QVariant(qulonglong(v)); }
    };

    [[nodiscard]] Stmt Prepare(const char* sql) const;

private:
    QSqlDatabase m_Db;

    void exec(const char* sql) const;

    void ensureSchema() const;
};

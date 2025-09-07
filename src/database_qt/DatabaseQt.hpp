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
        Stmt&& Bind(Ts&&... args) {
            auto toVar = [](auto&& x) -> QVariant {
                using U = std::decay_t<decltype(x)>;
                if constexpr (std::is_same_v<U, QString>)
                    return QVariant(std::forward<decltype(x)>(x));
                else if constexpr (std::is_same_v<U, const char*> || std::is_same_v<U, char*>)
                    return QVariant(QString::fromUtf8(x));
                else if constexpr (std::is_array_v<U> &&
                                   std::is_same_v<std::remove_extent_t<U>, char>)
                    return QVariant(QString::fromUtf8(x));
                else if constexpr (std::is_same_v<U, std::string>)
                    return QVariant(QString::fromUtf8(x.c_str(), int(x.size())));
                else if constexpr (std::is_same_v<U, int> || std::is_same_v<U, double>)
                    return QVariant(x);
                else if constexpr (std::is_same_v<U, size_t>)
                    return QVariant(qulonglong(x));
                else
                    return QVariant::fromValue(std::forward<decltype(x)>(x));
            };
            (m_Query.addBindValue(toVar(std::forward<Ts>(args))), ...);
            return std::move(*this);
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
    };

    [[nodiscard]] Stmt Prepare(const char* sql) const;

private:
    QSqlDatabase m_Db;

    void exec(const char* sql) const;

    void ensureSchema() const;
};

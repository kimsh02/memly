// #pragma once

// #include <QtSql>

// // DatabaseQt.hpp
// #pragma once
// #include <QtSql>

// class DatabaseQt final {
// public:
//     DatabaseQt(const DatabaseQt&)            = delete;
//     DatabaseQt& operator=(const DatabaseQt&) = delete;

//     DatabaseQt(DatabaseQt&&) noexcept            = delete;
//     DatabaseQt& operator=(DatabaseQt&&) noexcept = delete;

//     explicit DatabaseQt(const QString& path) {
//         m_Db = QSqlDatabase::addDatabase("QSQLITE"); // default connection
//         m_Db.setDatabaseName(path);
//         m_Db.setConnectOptions("QSQLITE_BUSY_TIMEOUT=5000"); // optional
//         if (!m_Db.open()) throw std::runtime_error(m_Db.lastError().text().toStdString());
//         Exec("PRAGMA journal_mode=WAL;");
//         Exec("PRAGMA foreign_keys=ON;");
//     }

//     // one-shot SQL (no results)
//     void Exec(QStringView sql) {
//         QSqlQuery q(m_Db);
//         if (!q.exec(QString(sql))) throw std::runtime_error(q.lastError().text().toStdString());
//     }

//     // migrations via PRAGMA user_version
//     void EnsureSchema(const QList<std::function<void(QSqlDatabase&)>>& steps) {
//         int cur = 0;
//         {
//             QSqlQuery q("PRAGMA user_version;", m_Db);
//             q.next();
//             cur = q.value(0).toInt();
//         }
//         const int target = steps.size();
//         if (cur >= target) return;

//         if (!m_Db.transaction()) throw std::runtime_error(m_Db.lastError().text().toStdString());
//         for (int v = cur; v < target; ++v) {
//             steps[v](m_Db);
//             Exec(QString("PRAGMA user_version=%1;").arg(v + 1));
//         }
//         if (!m_Db.commit()) throw std::runtime_error(m_Db.lastError().text().toStdString());
//     }

//     // prepared statement helper (example)
//     QSqlQuery Prepare(QStringView sql) {
//         QSqlQuery q(m_Db);
//         if (!q.prepare(QString(sql))) throw std::runtime_error(q.lastError().text().toStdString());
//         return q; // implicit sharing, copy is cheap
//     }

//     QSqlDatabase& Handle() noexcept { return m_Db; }

// private:
//     QSqlDatabase m_Db;
// };

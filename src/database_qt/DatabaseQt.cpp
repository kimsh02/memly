#include "database_qt/DatabaseQt.hpp"

#include "gui/Paths.hpp"
#include "util/Util.hpp"

DatabaseQt::DatabaseQt() {
    m_Db = QSqlDatabase::addDatabase("QSQLITE");
    m_Db.setDatabaseName(Paths::DatabaseFile());

    if (!m_Db.open()) { throw std::runtime_error(m_Db.lastError().text().toStdString()); }

    exec("PRAGMA user_version=1;");
    exec("PRAGMA foreign_keys=ON;");
    exec("PRAGMA journal_mode=WAL;");
    exec("PRAGMA optimize;");

    ensureSchema();
}

DatabaseQt::~DatabaseQt() { m_Db.close(); }

void DatabaseQt::exec(const char* sql) const {
    QSqlQuery query(m_Db);
    if (!query.exec(sql)) { Fatal(query.lastError().text().toStdString()); }
}

[[nodiscard]] DatabaseQt::Stmt DatabaseQt::Prepare(const char* sql) const {
    QSqlQuery query(m_Db);
    if (!query.prepare(sql)) { Fatal(query.lastError().text().toStdString()); }
    return Stmt(std::move(query));
}

void DatabaseQt::ensureSchema() const {
    exec(R"(CREATE TABLE IF NOT EXISTS settings (
        id               INTEGER PRIMARY KEY,
        target_lang_idx  INTEGER NOT NULL,
        name             TEXT
    );)");
    exec(R"(CREATE TABLE IF NOT EXISTS decks (
        id             INTEGER PRIMARY KEY,
        name           TEXT NOT NULL UNIQUE,
        card_count     INTEGER NOT NULL
    );)");
    exec(R"(CREATE TABLE IF NOT EXISTS cards (
        id             INTEGER PRIMARY KEY,
        deck_id        INTEGER NOT NULL,
        created        TEXT    NOT NULL,
        last_updated   TEXT,
        last_reviewed  TEXT,

        front_text     TEXT    NOT NULL,
        back_text      TEXT    NOT NULL,
        audio_path     TEXT    NOT NULL,
        image_path     TEXT,

        difficulty     REAL    NOT NULL,
        stability      REAL    NOT NULL,
        reviews        INTEGER NOT NULL,
        lapses         INTEGER NOT NULL,
        due            TEXT,

        UNIQUE(deck_id, front_text),
        FOREIGN KEY(deck_id) REFERENCES decks(id) ON DELETE CASCADE
    );)");

    // // Maintain decks.card_count via triggers (SQLite disallows subqueries in generated columns)
    // exec(R"(CREATE TRIGGER IF NOT EXISTS trg_cards_insert AFTER INSERT ON cards
    // BEGIN
    //   UPDATE decks SET card_count = card_count + 1 WHERE id = NEW.deck_id;
    // END;)");

    // exec(R"(CREATE TRIGGER IF NOT EXISTS trg_cards_delete AFTER DELETE ON cards
    // BEGIN
    //   UPDATE decks SET card_count = card_count - 1 WHERE id = OLD.deck_id;
    // END;)");

    // exec(R"(CREATE TRIGGER IF NOT EXISTS trg_cards_update_deck AFTER UPDATE OF deck_id ON cards
    // BEGIN
    //   UPDATE decks SET card_count = card_count - 1 WHERE id = OLD.deck_id;
    //   UPDATE decks SET card_count = card_count + 1 WHERE id = NEW.deck_id;
    // END;)");

    // // Backfill counts in case table existed before triggers
    // exec(R"(UPDATE decks
    //         SET card_count = COALESCE((
    //             SELECT COUNT(*) FROM cards WHERE cards.deck_id = decks.id
    //         ), 0);)");
}

// template <class... Ts>
// DatabaseQt::Stmt& DatabaseQt::Stmt::Bind(Ts&&... args) {
//     auto toVar = [](auto&& x) -> QVariant {
//         using U = std::decay_t<decltype(x)>;
//         if constexpr (std::is_same_v<U, QString>)
//             return QVariant(std::forward<decltype(x)>(x));
//         else if constexpr (std::is_same_v<U, const char*> || std::is_same_v<U, char*>)
//             return QVariant(QString::fromUtf8(x));
//         else if constexpr (std::is_array_v<U> && std::is_same_v<std::remove_extent_t<U>, char>)
//             return QVariant(QString::fromUtf8(x));
//         else if constexpr (std::is_same_v<U, std::string>)
//             return QVariant(QString::fromUtf8(x.c_str(), int(x.size())));
//         else if constexpr (std::is_same_v<U, int> || std::is_same_v<U, double>)
//             return QVariant(x);
//         else if constexpr (std::is_same_v<U, size_t>)
//             return QVariant(qulonglong(x));
//         else
//             return QVariant::fromValue(std::forward<decltype(x)>(x));
//     };
//     (m_Query.addBindValue(toVar(std::forward<Ts>(args))), ...);
//     return *this;
// }

void DatabaseQt::Stmt::Exec() {
    if (!m_Query.exec()) { Fatal(m_Query.lastError().text().toStdString()); }
}

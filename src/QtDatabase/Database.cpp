#include "Database.hpp"

#include "Common/Utility.hpp"
#include "QtGui/Paths.hpp"

DatabaseQt::DatabaseQt() {
    m_Db = QSqlDatabase::addDatabase("QSQLITE");
    m_Db.setDatabaseName(Paths::DatabaseFile());

    if (!m_Db.open()) {
        throw std::runtime_error(m_Db.lastError().text().toStdString());
    }

    Exec("PRAGMA user_version=1;");
    Exec("PRAGMA foreign_keys=ON;");
    Exec("PRAGMA journal_mode=WAL;");
    Exec("PRAGMA synchronous = FULL;");
    Exec("PRAGMA optimize;");

    EnsureSchema();
}

DatabaseQt::~DatabaseQt() { m_Db.close(); }

void DatabaseQt::Exec(const char* Sql) const {
    QSqlQuery Query(m_Db);
    if (!Query.exec(Sql)) {
        Fatal(Query.lastError().text().toStdString());
    }
}

[[nodiscard]] DatabaseQt::Stmt DatabaseQt::Prepare(const char* Sql) const {
    QSqlQuery Query(m_Db);
    if (!Query.prepare(Sql)) {
        Fatal(Query.lastError().text().toStdString());
    }
    return Stmt(std::move(Query));
}

void DatabaseQt::EnsureSchema() const {
    Exec(R"(CREATE TABLE IF NOT EXISTS settings (
        id               INTEGER PRIMARY KEY,
        target_lang_idx  INTEGER NOT NULL,
        name             TEXT
    );)");
    Exec(R"(CREATE TABLE IF NOT EXISTS decks (
        id             INTEGER PRIMARY KEY AUTOINCREMENT,
        card_count     INTEGER NOT NULL DEFAULT 0,
        name           TEXT NOT NULL UNIQUE
    );)");
    Exec(R"(CREATE TABLE IF NOT EXISTS cards (
        id              INTEGER PRIMARY KEY AUTOINCREMENT,
        deck_id         INTEGER NOT NULL,
        created_at      TEXT    NOT NULL,
        updated_at      TEXT,
        reviewed_at     TEXT,
 
        difficulty      REAL    NOT NULL,
        stability       REAL    NOT NULL,
 
        front_text      TEXT    NOT NULL,
        front_norm_text TEXT    NOT NULL,
        back_text       TEXT    NOT NULL,
        back_norm_text  TEXT    NOT NULL,
        audio_path      TEXT    NOT NULL,
        image_path      TEXT,

        UNIQUE(deck_id, front_text),
        FOREIGN KEY(deck_id) REFERENCES decks(id) ON DELETE CASCADE
    );)");

    Exec(R"(CREATE TRIGGER IF NOT EXISTS trg_cards_insert AFTER INSERT ON cards
    BEGIN
      UPDATE decks SET card_count = card_count + 1 WHERE id = NEW.deck_id;
    END;)");

    Exec(R"(CREATE TRIGGER IF NOT EXISTS trg_cards_delete AFTER DELETE ON cards
    BEGIN
      UPDATE decks SET card_count = card_count - 1 WHERE id = OLD.deck_id;
    END;)");

    Exec(
        R"(CREATE TRIGGER IF NOT EXISTS trg_cards_update_deck AFTER UPDATE OF deck_id ON cards
    BEGIN
      UPDATE decks SET card_count = card_count - 1 WHERE id = OLD.deck_id;
      UPDATE decks SET card_count = card_count + 1 WHERE id = NEW.deck_id;
    END;)");
}

void DatabaseQt::Stmt::Exec() {
    if (!m_Query.exec()) {
        Fatal(m_Query.lastError().text().toStdString());
    }
}

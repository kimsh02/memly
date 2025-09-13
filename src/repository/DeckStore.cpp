#include "repository/DeckStore.hpp"

[[nodiscard]] Types::IDVector DeckStore::GetDeckIDs() const {
    Types::IDVector idv;
    idv.reserve(m_DeckRecords.size());
    for (auto it = m_DeckRecords.begin(); it != m_DeckRecords.end(); it++) {
        idv.push_back(it->second.DeckContext.ID());
    }
    return idv;
}

DeckStore::DeckStore(const DatabaseQt& db, CardStore& cs) noexcept
    : m_Db(db)
    , m_CardStore(cs) {
    auto readAll = db.Prepare(SQL::s_ReadAllSQL);
    readAll.Exec();
    while (readAll.Next()) {
        DeckContext deckContext(readAll.Value(0).toULongLong());
        Deck        deck(readAll.Value(1).toString().toStdString(), readAll.Value(2).toULongLong());
        m_DeckRecords.emplace(deckContext.ID(),
                              DeckRecord{ std::move(deckContext), std::move(deck) });
    }
}

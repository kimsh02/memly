#pragma once

#include "database_qt/DatabaseQt.hpp"
#include "repository/CardStore.hpp"
#include "repository/DeckStore.hpp"

class DeckCardService final {
public:
    DeckCardService(const DeckCardService&)            = delete;
    DeckCardService& operator=(const DeckCardService&) = delete;

    DeckCardService(DeckCardService&&) noexcept            = delete;
    DeckCardService& operator=(DeckCardService&&) noexcept = delete;

    DeckCardService(DatabaseQt& db, DeckStore& ds, CardStore& cs) noexcept
        : m_Db(db)
        , m_DeckStore(ds)
        , m_CardStore(cs) {}

    // void CreateCard()
    void DeleteDeck(int deckId); // begins tx, deletes deck, evicts caches

private:
    DatabaseQt& m_Db;
    DeckStore&  m_DeckStore;
    CardStore&  m_CardStore;
};

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

    DeckCardService(DatabaseQt& Db, DeckStore& DeckStore, CardStore& CardStore) noexcept
        : m_Db(Db)
        , m_DeckStore(DeckStore)
        , m_CardStore(CardStore) {}

    void DeleteDeck(int DeckId); // TODO: begins tx, deletes deck, evicts caches

private:
    DatabaseQt& m_Db;
    DeckStore&  m_DeckStore;
    CardStore&  m_CardStore;
};

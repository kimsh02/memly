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

    DeckCardService(const DatabaseQt& db, const DeckStore& ds, const CardStore& cs) noexcept;

    void DeleteDeck(int deckId); // begins tx, deletes deck, evicts caches

private:
    const DatabaseQt m_Db;

    DeckStore& m_DeckStore;
    CardStore& m_CardStore;
};

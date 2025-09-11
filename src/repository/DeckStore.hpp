#pragma once

#include <unordered_map>

#include "common/Types.hpp"
#include "database_qt/DatabaseQt.hpp"
#include "models/Deck.hpp"

class DeckStore final {
public:
    DeckStore(const DeckStore&)            = delete;
    DeckStore& operator=(const DeckStore&) = delete;

    DeckStore(DeckStore&&) noexcept            = delete;
    DeckStore& operator=(DeckStore&&) noexcept = delete;

    explicit DeckStore(const DatabaseQt& db) noexcept
        : m_Db(db) {}

    enum class UserField { Name };

    using UVResult = Types::VResult<UserField>;

    [[nodiscard]] std::size_t Create(Deck&& deck) noexcept;

    [[nodiscard]] const Deck* Read(std::size_t deckID) const noexcept;

    [[nodiscard]] UVResult Update(std::size_t deckID, Deck&& deck);

    [[nodiscard]] bool Delete(std::size_t deckID) noexcept;

private:
    std::unordered_map<std::size_t, Deck> m_Decks;

    const DatabaseQt& m_Db;
};

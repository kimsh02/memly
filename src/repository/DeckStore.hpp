#pragma once

#include <unordered_map>

#include "models/Deck.hpp"

class DeckStore final {
public:
    DeckStore(const DeckStore&)            = delete;
    DeckStore& operator=(const DeckStore&) = delete;

    DeckStore(DeckStore&&) noexcept            = default;
    DeckStore& operator=(DeckStore&&) noexcept = default;

    [[nodiscard]] std::size_t Create(Deck&& deck);
    [[nodiscard]] const Deck* Read(std::size_t id) const noexcept;
    // [[nodiscard]] bool Update()

private:
    std::unordered_map<std::size_t, Deck> m_Decks;
};

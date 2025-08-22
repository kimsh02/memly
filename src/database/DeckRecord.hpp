#pragma once

#include "models/Deck.hpp"

struct DeckRecord final {
    Deck Deck;

    DeckRecord(const DeckRecord&)                = delete;
    DeckRecord& operator=(const DeckRecord&)     = delete;
    DeckRecord(DeckRecord&&) noexcept            = delete;
    DeckRecord& operator=(DeckRecord&&) noexcept = delete;

    DeckRecord(struct Deck&& deck)
        : Deck(std::move(deck)) {}
};

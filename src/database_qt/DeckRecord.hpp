#pragma once

#include "models/Deck.hpp"

struct DeckRecordContext {
    std::size_t ID;

    DeckRecordContext(const DeckRecordContext&)            = delete;
    DeckRecordContext& operator=(const DeckRecordContext&) = delete;

    DeckRecordContext(DeckRecordContext&&) noexcept            = default;
    DeckRecordContext& operator=(DeckRecordContext&&) noexcept = default;

    explicit DeckRecordContext(std::size_t id) noexcept
        : ID(id) {}
};

struct DeckRecord final {
    DeckRecordContext DeckRecordContext;
    Deck              Deck;

    DeckRecord(const DeckRecord&)            = delete;
    DeckRecord& operator=(const DeckRecord&) = delete;

    DeckRecord(DeckRecord&&) noexcept            = default;
    DeckRecord& operator=(DeckRecord&&) noexcept = default;

    explicit DeckRecord(struct DeckRecordContext&& deckRecordContext, struct Deck&& deck) noexcept
        : DeckRecordContext(std::move(deckRecordContext))
        , Deck(std::move(deck)) {}
};

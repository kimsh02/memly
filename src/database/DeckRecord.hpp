#pragma once

#include "common/Types.hpp"
#include "models/Deck.hpp"

struct DeckRecordContext {
    const Types::ID ID;

    DeckRecordContext(const DeckRecordContext&)            = delete;
    DeckRecordContext& operator=(const DeckRecordContext&) = delete;

    DeckRecordContext(DeckRecordContext&&) noexcept            = default;
    DeckRecordContext& operator=(DeckRecordContext&&) noexcept = delete;

    explicit DeckRecordContext(const Types::ID& id)
        : ID(id) {}
};

struct DeckRecord final {
    DeckRecordContext DeckRecordContextContext;
    Deck              Deck;

    DeckRecord(const DeckRecord&)                = delete;
    DeckRecord& operator=(const DeckRecord&)     = delete;
    DeckRecord(DeckRecord&&) noexcept            = delete;
    DeckRecord& operator=(DeckRecord&&) noexcept = delete;

    explicit DeckRecord(struct DeckRecordContext&& deckRecordContext, struct Deck&& deck)
        : DeckRecordContextContext(std::move(deckRecordContext))
        , Deck(std::move(deck)) {}
};

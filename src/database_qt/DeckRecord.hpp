#pragma once

#include "models/Deck.hpp"

struct DeckContext {
    DeckContext(const DeckContext&)            = delete;
    DeckContext& operator=(const DeckContext&) = delete;

    DeckContext(DeckContext&&) noexcept            = default;
    DeckContext& operator=(DeckContext&&) noexcept = default;

    explicit DeckContext(std::size_t id) noexcept
        : m_ConstID(id) {}

    std::size_t ID() const noexcept { return m_ConstID; }

private:
    std::size_t m_ConstID;
};

struct DeckRecord final {
    DeckContext DeckContext;
    Deck        Deck;

    DeckRecord(const DeckRecord&)            = delete;
    DeckRecord& operator=(const DeckRecord&) = delete;

    DeckRecord(DeckRecord&&) noexcept            = default;
    DeckRecord& operator=(DeckRecord&&) noexcept = default;

    explicit DeckRecord(struct DeckContext&& deckContext, struct Deck&& deck) noexcept
        : DeckContext(std::move(deckContext))
        , Deck(std::move(deck)) {}
};

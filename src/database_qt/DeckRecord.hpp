#pragma once

#include "models/Deck.hpp"

struct DeckContext {
    DeckContext(const DeckContext&)            = delete;
    DeckContext& operator=(const DeckContext&) = delete;

    DeckContext(DeckContext&&) noexcept            = default;
    DeckContext& operator=(DeckContext&&) noexcept = default;

    explicit DeckContext(std::size_t id) noexcept
        : m_ConstID(id) {}

    [[nodiscard]] std::size_t ID() const noexcept { return m_ConstID; }

private:
    std::size_t m_ConstID;
};

struct DeckStats {
    DeckStats(const DeckStats&)                     = delete;
    DeckStats& operator=(const DeckStats&) noexcept = delete;

    DeckStats(DeckStats&&)                     = default;
    DeckStats& operator=(DeckStats&&) noexcept = default;

    explicit DeckStats(std::size_t cardCount) noexcept
        : m_ConstCardCount(cardCount) {}

    [[nodiscard]] std::size_t CardCount() const noexcept { return m_ConstCardCount; }

private:
    std::size_t m_ConstCardCount;
};

struct DeckRecord final {
    DeckContext DeckContext;
    DeckStats   DeckStats;
    Deck        Deck;

    DeckRecord(const DeckRecord&)            = delete;
    DeckRecord& operator=(const DeckRecord&) = delete;

    DeckRecord(DeckRecord&&) noexcept            = default;
    DeckRecord& operator=(DeckRecord&&) noexcept = default;

    explicit DeckRecord(struct DeckContext&& deckContext,
                        struct DeckStats&&   deckStats,
                        struct Deck&&        deck) noexcept
        : DeckContext(std::move(deckContext))
        , DeckStats(std::move(deckStats))
        , Deck(std::move(deck)) {}
};

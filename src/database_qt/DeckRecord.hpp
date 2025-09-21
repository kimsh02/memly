#pragma once

#include "models/Deck.hpp"

struct DeckContext {
    DeckContext(const DeckContext&)            = delete;
    DeckContext& operator=(const DeckContext&) = delete;

    DeckContext(DeckContext&&) noexcept            = default;
    DeckContext& operator=(DeckContext&&) noexcept = default;

    explicit DeckContext(std::size_t Id) noexcept
        : m_ConstId(Id) {}

    [[nodiscard]] std::size_t Id() const noexcept { return m_ConstId; }

private:
    std::size_t m_ConstId;
};

struct DeckStats {
    DeckStats(const DeckStats&)                     = delete;
    DeckStats& operator=(const DeckStats&) noexcept = delete;

    DeckStats(DeckStats&&)                     = default;
    DeckStats& operator=(DeckStats&&) noexcept = default;

    explicit DeckStats(std::size_t CardCount) noexcept
        : m_ConstCardCount(CardCount) {}

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

    explicit DeckRecord(struct DeckContext&& DeckContext,
                        struct DeckStats&&   DeckStats,
                        struct Deck&&        Deck) noexcept
        : DeckContext(std::move(DeckContext))
        , DeckStats(std::move(DeckStats))
        , Deck(std::move(Deck)) {}
};

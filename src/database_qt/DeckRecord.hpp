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

struct DeckRecord final {
    DeckContext DeckContext;
    Deck        Deck;

    DeckRecord(const DeckRecord&)            = delete;
    DeckRecord& operator=(const DeckRecord&) = delete;

    DeckRecord(DeckRecord&&) noexcept            = default;
    DeckRecord& operator=(DeckRecord&&) noexcept = default;

    explicit DeckRecord(struct DeckContext&& DeckContext, struct Deck&& Deck) noexcept
        : DeckContext(std::move(DeckContext))
        , Deck(std::move(Deck)) {}
};

#pragma once

#include "common/Types.hpp"
#include "models/FlashCard.hpp"

struct FlashCardContext {
    const Types::TimeString   Created;
    const std::size_t         Id;
    std::size_t               DeckId;
    Types::OptionalTimeString LastUpdated, LastReviewed;

    FlashCardContext(const FlashCardContext&)            = delete;
    FlashCardContext& operator=(const FlashCardContext&) = delete;

    FlashCardContext(FlashCardContext&&) noexcept            = default;
    FlashCardContext& operator=(FlashCardContext&&) noexcept = delete;

    explicit FlashCardContext(Types::TimeString&& Created,
                              std::size_t&        Id,
                              std::size_t&        DeckId) noexcept
        : Created(std::move(Created))
        , Id(Id)
        , DeckId(DeckId) {}
};

struct FlashCardRecord final {
    FlashCardContext FlashCardContext;
    FlashCard        FlashCard;

    FlashCardRecord(const FlashCardRecord&)                = delete;
    FlashCardRecord& operator=(const FlashCardRecord&)     = delete;
    FlashCardRecord(FlashCardRecord&&) noexcept            = delete;
    FlashCardRecord& operator=(FlashCardRecord&&) noexcept = delete;

    explicit FlashCardRecord(struct FlashCardContext&& FlashCardContext,
                             struct FlashCard&&        FlashCard) noexcept
        : FlashCardContext(std::move(FlashCardContext))
        , FlashCard(std::move(FlashCard)) {}
};

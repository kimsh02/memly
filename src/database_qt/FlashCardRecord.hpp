#pragma once

#include "common/Types.hpp"
#include "models/FlashCard.hpp"

struct FlashCardContext {
    const Types::TimeString   Created;
    const std::size_t         ID;
    std::size_t               DeckID;
    Types::OptionalTimeString LastUpdated, LastReviewed;

    FlashCardContext(const FlashCardContext&)            = delete;
    FlashCardContext& operator=(const FlashCardContext&) = delete;

    FlashCardContext(FlashCardContext&&) noexcept            = default;
    FlashCardContext& operator=(FlashCardContext&&) noexcept = delete;

    explicit FlashCardContext(Types::TimeString&& created,
                              std::size_t&        id,
                              std::size_t&        deckID) noexcept
        : Created(std::move(created))
        , ID(id)
        , DeckID(deckID) {}
};

struct FlashCardRecord final {
    FlashCardContext FlashCardContext;
    FlashCard        FlashCard;

    FlashCardRecord(const FlashCardRecord&)                = delete;
    FlashCardRecord& operator=(const FlashCardRecord&)     = delete;
    FlashCardRecord(FlashCardRecord&&) noexcept            = delete;
    FlashCardRecord& operator=(FlashCardRecord&&) noexcept = delete;

    explicit FlashCardRecord(struct FlashCardContext&& flashCardContext,
                             struct FlashCard&&        flashCard) noexcept
        : FlashCardContext(std::move(flashCardContext))
        , FlashCard(std::move(flashCard)) {}
};

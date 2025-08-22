#pragma once

#include "common/Types.hpp"
#include "models/FlashCard.hpp"

struct FlashCardContext {
    const Types::TimeString   Created;
    Types::ID                 DeckID;
    Types::OptionalTimeString LastUpdated, LastReviewed;

    FlashCardContext(const FlashCardContext&)            = delete;
    FlashCardContext& operator=(const FlashCardContext&) = delete;

    FlashCardContext(FlashCardContext&&) noexcept            = default;
    FlashCardContext& operator=(FlashCardContext&&) noexcept = delete;

    FlashCardContext(Types::TimeString&& created, const Types::ID& deckID)
        : Created(std::move(created))
        , DeckID(std::move(deckID)) {}
};

struct FlashCardRecord final {
    FlashCardContext FlashCardContext;
    FlashCard        FlashCard;

    FlashCardRecord(const FlashCardRecord&)                = delete;
    FlashCardRecord& operator=(const FlashCardRecord&)     = delete;
    FlashCardRecord(FlashCardRecord&&) noexcept            = delete;
    FlashCardRecord& operator=(FlashCardRecord&&) noexcept = delete;

    explicit FlashCardRecord(struct FlashCardContext&& flashCardContext,
                             struct FlashCard&&        flashCard)
        : FlashCardContext(std::move(flashCardContext))
        , FlashCard(std::move(flashCard)) {}
};

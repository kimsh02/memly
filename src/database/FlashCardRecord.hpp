#pragma once

#include <boost/uuid/uuid.hpp>

#include <optional>
#include <string>

#include "models/FlashCard.hpp"

struct FlashCardRecord {
    using TimeString = std::string; // SQLite "YYYY-MM-DD HH:MM:SS" TEXT
    using ID         = boost::uuids::uuid;

    const ID                  DeckID;
    FlashCard                 Card;
    const TimeString          TimeCreated;
    std::optional<TimeString> TimeUpdated;

    FlashCardRecord(const FlashCardRecord&)               = delete;
    FlashCardRecord& operator=(const FlashCardRecord&)    = delete;
    FlashCardRecord(FlashCardRecord&&) noexcept           = delete;
    FlashCardRecord operator=(FlashCardRecord&&) noexcept = delete;

    explicit FlashCardRecord(ID&&                        deckID,
                             FlashCard&&                 card,
                             TimeString&&                timeCreated,
                             std::optional<TimeString>&& timeUpdated = std::nullopt) noexcept
        : DeckID(std::move(deckID))
        , Card(std::move(card))
        , TimeCreated(std::move(timeCreated))
        , TimeUpdated(std::move(timeUpdated)) {}
};

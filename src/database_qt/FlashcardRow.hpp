#pragma once

#include "common/Types.hpp"

struct FlashcardRow {
    const std::size_t         RowId;
    const std::size_t         DeckRowId;
    const Types::TimeString   CreatedAt;
    Types::OptionalTimeString UpdatedAt;
    Types::OptionalTimeString ReviewedAt;

    float                     Difficulty;
    float                     Stability;
    Types::OptionalTimeString DueAt;

    std::string               FrontText;
    std::string               BackText;
    Types::PathString         Audio;
    Types::OptionalPathString Image;
};

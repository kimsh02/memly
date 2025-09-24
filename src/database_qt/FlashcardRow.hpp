#pragma once

#include <string>

#include "common/Types.hpp"

struct FlashcardRow {
    const std::size_t         Id;
    const std::size_t         DeckId;
    const Types::TimeString   CreatedAt;
    Types::OptionalTimeString UpdatedAt;
    Types::OptionalTimeString ReviewedAt;

    float Difficulty;
    float Stability;

    std::string               FrontText;
    std::string               BackText;
    Types::PathString         AudioPath;
    Types::OptionalPathString ImagePath;
};

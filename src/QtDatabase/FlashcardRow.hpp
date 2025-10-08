#pragma once

#include <optional>
#include <string>

struct FlashcardRow {
    const std::size_t          Id;
    const std::size_t          DeckId;
    const std::string          CreatedAt;
    std::string                UpdatedAt;
    std::optional<std::string> ReviewedAt;

    float Difficulty;
    float Stability;

    std::string                FrontText;
    std::string                FrontNormText;
    std::string                BackText;
    std::string                AudioPath;
    std::optional<std::string> ImagePath;
};

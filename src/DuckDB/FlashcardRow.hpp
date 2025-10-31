#pragma once

#include <duckdb.hpp>

#include <optional>
#include <string>

struct FlashcardRow {
    const std::size_t                  Id;
    const std::size_t                  DeckId;
    const duckdb::timestamp_t          CreatedAt;
    duckdb::timestamp_t                UpdatedAt;
    std::optional<duckdb::timestamp_t> ReviewedAt;

    double Difficulty;
    double Stability;

    std::string                FrontText;
    std::string                FrontNormText;
    std::string                BackText;
    std::string                BackNormText;
    std::optional<std::string> AudioPath;
    std::optional<std::string> ImagePath;
};
